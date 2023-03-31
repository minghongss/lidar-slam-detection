import os

import torch
import torch.nn as nn

from sensor_inference.model.pillar_vfe import PillarVFE
from sensor_inference.model.pointpillar_scatter import PointPillarScatter
from sensor_inference.model.multi_center_point_head import MultiCenterHead
from sensor_inference.model.CSPDarknet53Large import CSPDarknet53Large
from sensor_inference.model.CSPDarknet53Small import CSPDarknet53Small

class PointPillar(nn.Module):
    def __init__(self, model_cfg, num_class, dataset):
        super().__init__()
        self.model_cfg = model_cfg
        self.num_class = num_class
        self.dataset = dataset
        self.class_names = dataset.class_names
        model_info_dict = self.build_vfe_networks()
        model_info_dict = self.build_scatter_networks(model_info_dict=model_info_dict)
        model_info_dict = self.build_rpn_networks(model_info_dict=model_info_dict)

    def forward(self, input_list):
        voxel_features = input_list[0]
        coords = input_list[1]
        mask = input_list[2]

        vfe_feature = self.vfe(voxel_features, coords, mask)
        batch_spatial_features = self.map_to_bev_module(vfe_feature, coords)
        spatial_features_2d, spatial_features_2d_4x = self.backbone_2d(batch_spatial_features)

        batch_cls_preds, batch_box_preds = self.dense_head(spatial_features_2d, spatial_features_2d_4x)
        cls_preds, box_preds, label_preds = self.post_processing(batch_cls_preds, batch_box_preds)

        return cls_preds, box_preds, label_preds

    def build_vfe_networks(self):
        model_info_dict = {
            'module_list': [],
            'num_rawpoint_features': self.dataset.point_feature_encoder.num_point_features,
            'num_point_features': self.dataset.point_feature_encoder.num_point_features,
            'grid_size': self.dataset.grid_size,
            'point_cloud_range': self.dataset.point_cloud_range,
            'voxel_size': self.dataset.voxel_size
        }
        module, model_info_dict = self.build_vfe(model_info_dict=model_info_dict)
        self.add_module('vfe', module)

        return model_info_dict

    def build_vfe(self, model_info_dict):
        if self.model_cfg.get('VFE', None) is None:
            return None, model_info_dict

        vfe_module = PillarVFE(
            model_cfg=self.model_cfg.VFE,
            num_point_features=model_info_dict['num_rawpoint_features'],
            point_cloud_range=model_info_dict['point_cloud_range'],
            voxel_size=model_info_dict['voxel_size']
        )
        model_info_dict['num_point_features'] = vfe_module.get_output_feature_dim()
        model_info_dict['module_list'].append(vfe_module)
        return vfe_module, model_info_dict

    def build_scatter_networks(self, model_info_dict):
        module, model_info_dict = self.build_map_to_bev_module(model_info_dict=model_info_dict)
        self.add_module('map_to_bev_module', module)

        return model_info_dict

    def build_map_to_bev_module(self, model_info_dict):
        if self.model_cfg.get('MAP_TO_BEV', None) is None:
            return None, model_info_dict

        map_to_bev_module = PointPillarScatter(
            model_cfg=self.model_cfg.MAP_TO_BEV,
            grid_size=model_info_dict['grid_size']
        )
        model_info_dict['module_list'].append(map_to_bev_module)
        model_info_dict['num_bev_features'] = map_to_bev_module.num_bev_features
        return map_to_bev_module, model_info_dict

    def build_rpn_networks(self, model_info_dict):
        module, model_info_dict = self.build_backbone_2d(model_info_dict=model_info_dict)
        self.add_module('backbone_2d', module)

        module, model_info_dict = self.build_dense_head(model_info_dict=model_info_dict)
        self.add_module('dense_head', module)

        return model_info_dict

    def build_backbone_2d(self, model_info_dict):
        if self.model_cfg.get('BACKBONE_2D', None) is None:
            return None, model_info_dict

        if self.model_cfg.BACKBONE_2D.NAME == 'CSPDarknet53Large':
            Backbone2D = CSPDarknet53Large
        elif self.model_cfg.BACKBONE_2D.NAME == 'CSPDarknet53Small':
            Backbone2D = CSPDarknet53Small
        backbone_2d_module  = Backbone2D (
            model_cfg=self.model_cfg.BACKBONE_2D,
            input_channels=model_info_dict['num_bev_features']
        )
        model_info_dict['module_list'].append(backbone_2d_module)
        model_info_dict['num_bev_features'] = backbone_2d_module.num_bev_features
        return backbone_2d_module, model_info_dict

    def build_dense_head(self, model_info_dict):
        if self.model_cfg.get('DENSE_HEAD', None) is None:
            return None, model_info_dict
        dense_head_module = MultiCenterHead(
            model_cfg=self.model_cfg.DENSE_HEAD,
            input_channels=model_info_dict['num_bev_features'],
            num_class=self.num_class if not self.model_cfg.DENSE_HEAD.CLASS_AGNOSTIC else 1,
            class_names=self.class_names,
            grid_size=model_info_dict['grid_size'],
            point_cloud_range=model_info_dict['point_cloud_range'],
            voxel_size=model_info_dict['voxel_size'],
            predict_boxes_when_training=self.model_cfg.get('ROI_HEAD', False)
        )
        model_info_dict['module_list'].append(dense_head_module)
        return dense_head_module, model_info_dict

    def post_processing(self, cls_preds, box_preds, multihead_label_mapping=None):
        """
        Args:
            batch_dict:
                batch_size:
                batch_cls_preds: (B, num_boxes, num_classes | 1) or (N1+N2+..., num_classes | 1)
                                or [(B, num_boxes, num_class1), (B, num_boxes, num_class2) ...]
                multihead_label_mapping: [(num_class1), (num_class2), ...]
                batch_box_preds: (B, num_boxes, 7+C) or (N1+N2+..., 7+C)
                cls_preds_normalized: indicate whether batch_cls_preds is normalized
                batch_index: optional (N1+N2+...)
                has_class_labels: True/False
                roi_labels: (B, num_rois)  1 .. num_classes
                batch_pred_labels: (B, num_boxes, 1)
        Returns:

        """
        post_process_cfg = self.model_cfg.POST_PROCESSING

        if post_process_cfg.NMS_CONFIG.MULTI_CLASSES_NMS:
            nms_topk = dict()
            for name, k in self.model_cfg.POST_PROCESSING.NMS_CONFIG.NMS_PRE_MAXSIZE.items():
                nms_topk[self.class_names.index(name)+1] = k

            cur_start_idx = 0
            pred_scores, pred_labels, pred_boxes = [], [], []
            for cur_cls_preds, cur_label_mapping in zip(cls_preds, multihead_label_mapping):
                cur_box_preds = box_preds[cur_start_idx: cur_start_idx + cur_cls_preds.shape[0]]

                pred_scores_nms, pred_labels_nms, pred_boxes_nms = [], [], []
                for k in range(cur_cls_preds.shape[1]):
                    box_scores = cur_cls_preds[:, k]
                    _, indices = torch.topk(box_scores, k=nms_topk[cur_label_mapping.cpu().numpy()[0]])
                    pred_scores_nms.append(box_scores[indices])
                    pred_labels_nms.append(box_scores.new_ones(len(indices)).long() * k)
                    pred_boxes_nms.append(cur_box_preds[indices])
                cur_pred_scores = torch.cat(pred_scores_nms, dim=0)
                cur_pred_labels = torch.cat(pred_labels_nms, dim=0)
                cur_pred_boxes = torch.cat(pred_boxes_nms, dim=0)

                cur_pred_labels = cur_label_mapping[cur_pred_labels]
                pred_scores.append(cur_pred_scores)
                pred_labels.append(cur_pred_labels)
                pred_boxes.append(cur_pred_boxes)
                cur_start_idx += cur_cls_preds.shape[0]
            cls_preds = torch.cat(pred_scores, dim=0)
            label_preds = torch.cat(pred_labels, dim=0)
            box_preds = torch.cat(pred_boxes, dim=0)
        else:
            cls_preds, label_preds = torch.max(cls_preds, dim=-1)
            label_preds = label_preds + 1

            cls_preds, indices = torch.topk(cls_preds, k=post_process_cfg.NMS_CONFIG.NMS_PRE_MAXSIZE)
            box_preds = box_preds[indices]
            label_preds = label_preds[indices]

        return cls_preds, box_preds, label_preds


    def load_params_from_file(self, filename, to_cpu=False):
        if not os.path.isfile(filename):
            raise FileNotFoundError

        print('==> Loading parameters from checkpoint %s to %s' % (filename, 'CPU' if to_cpu else 'GPU'))
        loc_type = torch.device('cpu') if to_cpu else None
        checkpoint = torch.load(filename, map_location=loc_type)
        model_state_disk = checkpoint['model_state']

        epoch = checkpoint.get('epoch', -1)
        if 'version' in checkpoint:
            print('==> Checkpoint trained from version: %s, epoch %d' % (checkpoint['version'], epoch))

        update_model_state = {}
        for key, val in model_state_disk.items():
            if key in self.state_dict() and self.state_dict()[key].shape == model_state_disk[key].shape:
                update_model_state[key] = val

        state_dict = self.state_dict()
        state_dict.update(update_model_state)
        self.load_state_dict(state_dict)

        for key in state_dict:
            if key not in update_model_state:
                print('Not updated weight %s: %s' % (key, str(state_dict[key].shape)))

        print('==> Done (loaded %d/%d)' % (len(update_model_state), len(self.state_dict())))