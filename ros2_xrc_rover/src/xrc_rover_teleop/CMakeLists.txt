cmake_minimum_required(VERSION 3.8)
project(xrc_rover_teleop)

# Find ROS 2 packages
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)

# Add your other dependencies here

# Find and include ncurses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Create an executable
add_executable(keyboard_teleop_node src/xrc_rover_teleop_key.cpp)

# Link against ROS 2 libraries
ament_target_dependencies(keyboard_teleop_node rclcpp geometry_msgs)

# Link against ncurses library
target_link_libraries(keyboard_teleop_node ${CURSES_LIBRARIES})

# Install the executable
install(TARGETS
  keyboard_teleop_node
  DESTINATION lib/${PROJECT_NAME})

# Install other files if needed (config, launch files, etc.)

ament_package()
