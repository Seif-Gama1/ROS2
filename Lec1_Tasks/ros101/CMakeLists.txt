cmake_minimum_required(VERSION 3.8)
project(ros101)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

add_executable(int32_publisher src/MyInt32Publisher.cpp)
ament_target_dependencies(int32_publisher rclcpp std_msgs)

# Subscriber
add_executable(int32_subscriber src/MyInt32Subscriber.cpp)
ament_target_dependencies(int32_subscriber rclcpp std_msgs)

install(TARGETS
  int32_publisher
  int32_subscriber
  DESTINATION lib/${PROJECT_NAME})

ament_package()
