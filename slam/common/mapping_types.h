#ifndef __MAPPING_TYPES_H
#define __MAPPING_TYPES_H

#include <vector>
#include <map>
#include <Eigen/Geometry>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <opencv2/opencv.hpp>

#include "readerwriterqueue.h"

namespace g2o {
class VertexSE3;
}  // namespace g2o

template <typename T>
using RWQueue = moodycamel::BlockingReaderWriterQueue<T>;

typedef pcl::PointXYZI Point;
typedef pcl::PointXYZRGB PointRGB;

typedef pcl::PointCloud<Point> PointCloud;
typedef pcl::PointCloud<PointRGB> PointCloudRGB;

struct PointAttr {
  int id;
  uint32_t stamp; //relative to header stamp
};

struct PointCloudAttr {
  PointCloudAttr() {
    cloud = PointCloud::Ptr(new PointCloud());
  }
  PointCloud::Ptr cloud;
  std::vector<PointAttr> attr;
  Eigen::Matrix4d T;
};

typedef std::shared_ptr<PointCloudAttr> PointCloudAttrPtr;

struct PoseType {
  PoseType() {
    latitude = 0;
    longitude = 0;
    altitude = 0;
    status = 0;
    state = "";
    timestamp = 0;
    T = Eigen::Matrix4d::Zero();
  }
  double latitude;           // deg
  double longitude;          // deg
  double altitude;           // deg
  int status;
  std::string state;
  uint64_t timestamp;
  Eigen::Matrix4d T;
};

struct RTKType {
  RTKType() {
    timestamp = 0;
    heading = 0;
    pitch = 0;
    roll = 0;
    gyro_x = 0;
    gyro_y = 0;
    gyro_z = 0;
    acc_x = 0;
    acc_y = 0;
    acc_z = 0;
    latitude = 0;
    longitude = 0;
    altitude = 0;
    Ve = 0;
    Vn = 0;
    Vu = 0;
    status = 0;
    state = "";
    dimension = 2;
    precision = 100.0;
    T = Eigen::Matrix4d::Identity();
  }
  uint64_t timestamp;
  double heading;
  double pitch;
  double roll;
  double gyro_x;
  double gyro_y;
  double gyro_z;
  double acc_x;
  double acc_y;
  double acc_z;
  double latitude;
  double longitude;
  double altitude;
  double Ve;
  double Vn;
  double Vu;
  int status;
  std::string state;
  int dimension;
  double precision;
  Eigen::Matrix4d T;
  Eigen::VectorXf mean;
};

struct ImuType {
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  ImuType() {
    stamp = 0;
    acc << 0, 0, 0;
    gyr << 0, 0, 0;
    rot = Eigen::Quaterniond::Identity();
  }
  double stamp;
  Eigen::Vector3d acc;
  Eigen::Vector3d gyr;
  Eigen::Quaterniond rot;
};

struct CamParamType {
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  CamParamType() {
    K = cv::Mat(3, 3, cv::DataType<float>::type);
    DistCoef = cv::Mat(5, 1, CV_32F);
    DistCoef.at<float>(0) = 0;
    DistCoef.at<float>(1) = 0;
    DistCoef.at<float>(2) = 0;
    DistCoef.at<float>(3) = 0;
    DistCoef.at<float>(4) = 0;
    staticTrans = Eigen::Matrix4d::Identity();
  }
  cv::Mat K;
  cv::Mat DistCoef;
  Eigen::Matrix4d staticTrans;
};

struct FloorCoeffs {
  uint64_t timestamp;
  std::vector<float> coeffs;
};

struct PointCloudType {
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  PointCloudType(const PointCloud::Ptr &in) {
    points = in;
    T = Eigen::Matrix4d::Identity();
  }
  PointCloudType(const PointCloud::Ptr &in, const Eigen::Matrix4d &t) {
    points = in;
    T = t;
  }
  PointCloud::Ptr points;
  Eigen::Matrix4d T;
};

struct PointCloudAttrImagePose {
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  PointCloudAttrImagePose() {}
  PointCloudAttrImagePose(const PointCloudAttrPtr &in) {
    points = in;
    T = Eigen::Isometry3d::Identity();
  }
  PointCloudAttrImagePose(const PointCloudAttrPtr &in, const std::map<std::string, cv::Mat> &im,
                          const std::map<std::string, cv::Mat> &im_stream, const Eigen::Isometry3d &t) {
    points = in;
    images = im;
    images_stream = im_stream;
    T = t;
  }
  PointCloudAttrImagePose(const PointCloudAttrPtr &in, const Eigen::Isometry3d &t) {
    points = in;
    T = t;
  }
  PointCloudAttrPtr points;
  std::map<std::string, cv::Mat> images;
  std::map<std::string, cv::Mat> images_stream;
  Eigen::Isometry3d T;
};

struct PointCloudImageType {
  PointCloudImageType(const PointCloud::Ptr &in_points, const std::pair<std::string, cv::Mat> &in_image) {
    points = in_points;
    image = in_image;
  }
  PointCloud::Ptr points;
  std::pair<std::string, cv::Mat> image;
};

struct EdgeType {
  int id;
  int prev;
  int next;
};

struct VertexInfo {
  bool fix;
  int edge_num;
};

struct EdgeInfo {
  int vertex_num;
};

struct GraphInfo {
  std::map<int, VertexInfo> vertex;
  std::map<int, EdgeInfo> edge;
};

#endif