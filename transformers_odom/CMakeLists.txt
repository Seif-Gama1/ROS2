cmake_minimum_required(VERSION 3.8)
project(transformers_odom)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclpy REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tf2_ros REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

install(PROGRAMS
  scripts/orientation_analysis.py
  DESTINATION lib/${PROJECT_NAME}
)

# 1. Install your launch directory into the share folder
install(DIRECTORY 
  launch
  config
  DESTINATION share/${PROJECT_NAME}
)

# 2. Force ament to register this package into the share index properly
# This explicitly tells ROS 2 where to find files for this package name
ament_package()