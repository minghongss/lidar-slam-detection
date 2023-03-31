#ifndef __INTER_PROCESS_H
#define __INTER_PROCESS_H

#include <stdint.h>
#include <string>
#include <memory>
#include <zcm/zcm-cpp.hpp>
#include "std_msgs/Header.hpp"

#include "geometry_msgs/Point.hpp"
#include "geometry_msgs/Point32.hpp"
#include "geometry_msgs/Pose.hpp"
#include "geometry_msgs/PoseStamped.hpp"
#include "geometry_msgs/PoseWithCovariance.hpp"
#include "geometry_msgs/Quaternion.hpp"
#include "geometry_msgs/Twist.hpp"
#include "geometry_msgs/TwistWithCovariance.hpp"
#include "geometry_msgs/Vector3.hpp"

#include "nav_msgs/Odometry.hpp"
#include "nav_msgs/Path.hpp"

#include "sensor_msgs/CompressedImage.hpp"
#include "sensor_msgs/Image.hpp"
#include "sensor_msgs/Imu.hpp"
#include "sensor_msgs/NavSatFix.hpp"
#include "sensor_msgs/NavSatStatus.hpp"
#include "sensor_msgs/PointCloud.hpp"
#include "sensor_msgs/PointField.hpp"

std::shared_ptr<zcm::ZCM> get_core();
std::shared_ptr<zcm::ZCM> create_core();
void set_core_enable(bool enable);
bool get_core_enable();

void publish_msgs(const std::string &topic, const char &data);
void publish_msgs(const std::string &topic, const float &data);
void publish_msgs(const std::string &topic, const double &data);
void publish_msgs(const std::string &topic, const int32_t &data);
void publish_msgs(const std::string &topic, const int64_t &data);
void publish_msgs(const std::string &topic, const std::string &data);

#define PUBLISH_STD_MSG(topic, data)  do {     \
    if (get_core_enable()) {                   \
        publish_msgs(topic, data);             \
    }                                          \
} while (0)

#define PUBLISH_MSG(topic, msg)       do {     \
    if (get_core_enable()) {                   \
        get_core()->publish(topic, &msg);      \
    }                                          \
} while (0)

#endif //__INTER_PROCESS_H