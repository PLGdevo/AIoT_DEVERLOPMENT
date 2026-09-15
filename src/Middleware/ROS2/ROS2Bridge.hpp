#pragma once
#include "ROS2.h"

/*
 * ROS 2 integration boundary.
 *
 * This header intentionally does not include rcl/rclcpp.
 * A platform-specific ROS 2 bridge can be implemented outside
 * the embedded library and connected through this interface.
 */
