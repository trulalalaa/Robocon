#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

// Class Robot untuk konsep OOP
class Robot {
private:
    string name;
    double armLength1;
    double armLength2;

public:
    // Constructor untuk inisialisasi
    Robot(string n, double a1, double a2) : name(n), armLength1(a1), armLength2(a2) {}

    // Fungsi untuk menampilkan informasi robot
    void showInfo() {
        cout << "Robot: " << name << endl;
        cout << "Arm Length 1: " << armLength1 << " units" << endl;
        cout << "Arm Length 2: " << armLength2 << " units" << endl;
    }

    // Fungsi untuk melakukan inverse kinematics (menghitung sudut)
    void calculateIK(double x, double y) {
        double d = (x*x + y*y - armLength1*armLength1 - armLength2*armLength2) / (2 * armLength1 * armLength2);
        double theta2 = atan2(sqrt(1 - d*d), d); // Sudut kedua
        double theta1 = atan2(y, x) - atan2(armLength2 * sin(theta2), armLength1 + armLength2 * cos(theta2)); // Sudut pertama

        // Konversi ke derajat
        theta1 = theta1 * 180 / M_PI;
        theta2 = theta2 * 180 / M_PI;

        cout << "Theta1: " << theta1 << " degrees\n";
        cout << "Theta2: " << theta2 << " degrees\n";
    }

    // Fungsi untuk menyimpan informasi robot dan hasil inverse kinematics ke file
    void saveToFile(double x, double y) {
        ofstream myfile;
        myfile.open("robotdata.txt");
        myfile << "Robot Name: " << name << "\n";
        myfile << "Arm Length 1: " << armLength1 << " units\n";
        myfile << "Arm Length 2: " << armLength2 << " units\n";
        myfile << "Target Position: (" << x << ", " << y << ")\n";

        // Inverse Kinematics Calculation
        double d = (x*x + y*y - armLength1*armLength1 - armLength2*armLength2) / (2 * armLength1 * armLength2);
        double theta2 = atan2(sqrt(1 - d*d), d); // Sudut kedua
        double theta1 = atan2(y, x) - atan2(armLength2 * sin(theta2), armLength1 + armLength2 * cos(theta2)); // Sudut pertama

        // Konversi ke derajat
        theta1 = theta1 * 180 / M_PI;
        theta2 = theta2 * 180 / M_PI;

        myfile << "Theta1: " << theta1 << " degrees\n";
        myfile << "Theta2: " << theta2 << " degrees\n";

        myfile.close();
        cout << "Data saved to file 'robotdata.txt'." << endl;
    }
};

// Fungsi utama (main)
int main() {
    // Membuat objek robot
    Robot myRobot("RoboArm", 5.0, 3.0);

    // Menampilkan informasi robot
    myRobot.showInfo();

    // Input target posisi untuk inverse kinematics
    double targetX, targetY;
    cout << "Enter target X position: ";
    cin >> targetX;
    cout << "Enter target Y position: ";
    cin >> targetY;

    // Melakukan perhitungan inverse kinematics
    myRobot.calculateIK(targetX, targetY);

    // Menyimpan hasil ke file
    myRobot.saveToFile(targetX, targetY);

    return 0;
}
