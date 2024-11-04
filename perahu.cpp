#include <rclcpp/rclcpp.hpp>
#include "geometry_msgs/msg/twist.hpp"
#include <turtlesim/srv/teleport_absolute.hpp>
#include <turtlesim/srv/set_pen.hpp>
#include <math.h>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

class GambarPerahu : public rclcpp::Node {
public:
    GambarPerahu() : Node("gambar_perahu") {
        turtlePublisher = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
        teleportTurtle = this->create_client<turtlesim::srv::TeleportAbsolute>("/turtle1/teleport_absolute");
        setPen = this->create_client<turtlesim::srv::SetPen>("/turtle1/set_pen");
        RCLCPP_INFO(this->get_logger(), "Mulai menggambar perahu!!!");
    }

    geometry_msgs::msg::Twist message;

    void moveTurtle(double horizontal, double vertical, double degree, double durasi) {
        message.linear.x = horizontal;
        message.linear.y = vertical;
        message.angular.z = degree;
        turtlePublisher->publish(message);
        std::this_thread::sleep_for(std::chrono::duration<double>(durasi));
        message.linear.x = 0.0;
        message.linear.y = 0.0;
        message.angular.z = 0.0;
        turtlePublisher->publish(message);
    }

    void teleport_turtle(float x, float y, float theta) {
        auto request = std::make_shared<turtlesim::srv::TeleportAbsolute::Request>();
        request->x = x;
        request->y = y;
        request->theta = theta;

        auto future = teleportTurtle->async_send_request(request);
        if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), future) ==
            rclcpp::FutureReturnCode::SUCCESS) {
            RCLCPP_INFO(this->get_logger(), "Turtle berhasil dipindahkan ke (%f, %f) dengan orientasi %f.",
                        x, y, theta);
        } else {
            RCLCPP_ERROR(this->get_logger(), "Gagal memindahkan turtle.");
        }
    }

    void setpen(int r, int g, int b, int width, bool off) {
        auto request = std::make_shared<turtlesim::srv::SetPen::Request>();
        request->r = r;
        request->g = g;
        request->b = b;
        request->width = width;
        request->off = off;

        auto future = setPen->async_send_request(request);
        if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), future) ==
            rclcpp::FutureReturnCode::SUCCESS) {
            RCLCPP_INFO(this->get_logger(), "Pen set to (%d, %d, %d) with width %d. Pen off: %s",
                        r, g, b, width, off ? "true" : "false");
        } else {
            RCLCPP_ERROR(this->get_logger(), "Gagal mengatur pen.");
        }
    }

    void drawBoat() {
        // Mengatur pena ke warna biru untuk menggambar perahu
        setpen(0, 0, 255, 2, false);
        
        // Bagian bawah perahu
        moveTurtle(2.0, 0.0, 0.0, 1.0);  // Maju ke depan
        moveTurtle(-1.0, 0.0, 0.0, 1.0); // Mundur sedikit
        moveTurtle(0.0, 1.0, 0.0, 1.0);  // Belok kiri
        moveTurtle(1.0, 0.0, 0.0, 1.0);  // Maju lagi
        moveTurtle(0.0, -1.0, 0.0, 1.0); // Belok kanan
        moveTurtle(2.0, 0.0, 0.0, 1.0);  // Maju lagi
        moveTurtle(0.0, 1.0, 0.0, 1.0);  // Belok kiri
        moveTurtle(1.0, 0.0, 0.0, 1.0);  // Maju ke depan

        // Bagian atas perahu
        moveTurtle(0.5, 0.5, 0.0, 1.0);  // Gerakan ke kanan atas
        moveTurtle(1.0, 0.0, 0.0, 1.0);  // Maju ke depan
        moveTurtle(0.5, -0.5, 0.0, 1.0); // Kembali ke posisi semula

        // Mengatur pena ke putih untuk garis
        setpen(255, 255, 255, 2, false);
        teleport_turtle(2.5, 5.5, 0.0); // Teleportasi ke lokasi baru
    }

private:
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr turtlePublisher;
    rclcpp::Client<turtlesim::srv::TeleportAbsolute>::SharedPtr teleportTurtle;
    rclcpp::Client<turtlesim::srv::SetPen>::SharedPtr setPen;
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<GambarPerahu>();
    node->drawBoat(); // Panggil fungsi untuk menggambar perahu
    rclcpp::shutdown();
    return 0;
}
