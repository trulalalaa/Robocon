#include <bits/stdc++.h>
#include <fstream>  

using namespace std;

#define phi 3.14159265359
#define rad (phi / 180.0)

class RobotOmniwheel {
public:
    float matrix_invers[3][4];   
    float laju_roda[4];   
    float translasi[3];  

    void KelajuanRoda(fstream &file) {
        for (int i = 0; i < 4; i++) {
            cout << "Input wheel " << i + 1 << " velocity (m/s): ";
            cin >> laju_roda[i];
            file << "Wheel " << i + 1 << " velocity: " << laju_roda[i] << " m/s\n";  
        }
        float sudut[] = {45.0, 135.0, 225.0, 315.0};
        float radius = 1.0;  
        for (int i = 0; i < 4; i++) {
            matrix_invers[0][i] = sinf(sudut[i] * rad);  
            matrix_invers[1][i] = cosf(sudut[i] * rad);  
            matrix_invers[2][i] = radius;  
        }
        file << "\n";  
    }
    void KelajuanTranslasi(fstream &file) {
        for (int i = 0; i < 3; i++) {
            translasi[i] = 0;
            for (int j = 0; j < 4; j++) {
                translasi[i] += matrix_invers[i][j] * laju_roda[j];
            }
        }
        cout << "Translational velocity X: " << fixed << translasi[0] << " m/s\n";
        cout << "Translational velocity Y: " << fixed << translasi[1] << " m/s\n";
        cout << "Rotational velocity Theta: " << fixed << translasi[2] << " rad/s\n";
        
        file << "=========================\n";
        file << "Translational velocity X: " << fixed << translasi[0] << " m/s\n";
        file << "Translational velocity Y: " << fixed << translasi[1] << " m/s\n";
        file << "Rotational velocity Theta: " << fixed << translasi[2] << " rad/s\n";
        file << "=========================\n";  
        file << "\n" << "Don Sir";
    }
};

int main() {
    fstream file;
    file.open("roda.txt", ios::out); 

    RobotOmniwheel robot1;
    robot1.KelajuanRoda(file);
    robot1.KelajuanTranslasi(file);  

    file.close(); 
}
