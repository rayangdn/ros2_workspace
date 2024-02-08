#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <ncurses.h>
#include <csignal>

class KeyboardTeleopNode : public rclcpp::Node
{
public:
  KeyboardTeleopNode() : Node("keyboard_teleop_node")
  {
    // // Initialize ncurses
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    // Create a publisher for the cmd_vel topic
    cmd_vel_publisher_ = create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
    RCLCPP_INFO(this->get_logger(), "KeyboardTeleopeNode has been started.");

    // Set up signal handler for Ctrl+C
    signal(SIGINT, [](int signo) {
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Received Ctrl+C. Exiting...");
      rclcpp::shutdown();
    });

    // Create a timer to handle key presses
    key_timer_ = create_wall_timer(std::chrono::milliseconds(50), [this]() {
      handleKeyPress();
    });
  }

  ~KeyboardTeleopNode()
  {
    // End ncurses
    endwin();
  }

private:

  void handleKeyPress()
  {
    int ch = getch();
    geometry_msgs::msg::Twist twist_msg;

    switch (ch) {
      case 'w':
        // Increment linear x by 0.1
        twist_msg.linear.x += 1;
        break;
      case 's':
        // Reset linear and angular values to 0
        twist_msg.linear.x = 0.0;
        twist_msg.linear.y = 0.0;
        twist_msg.linear.z = 0.0;
        twist_msg.angular.x = 0.0;
        twist_msg.angular.y = 0.0;
        twist_msg.angular.z = 0.0;
        break;
      case 'y':
        // Decrement linear x by 0.1
        twist_msg.linear.x -= 0.5;
        break;
      case 'a':
      
        // Increment angular z by 0.1
        twist_msg.angular.z += 0.5;
        break;
      case 'd':
        // Decrement angular z by 0.1
        twist_msg.angular.z -= 0.5;
        break;
    }

    // Publish the Twist message
    cmd_vel_publisher_->publish(twist_msg);
  }

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_publisher_;
  rclcpp::TimerBase::SharedPtr key_timer_;
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<KeyboardTeleopNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
