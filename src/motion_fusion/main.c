/**
 * main (entrypoint) implementation
 */
#include "sam.h"
#include "system_samd51.h"
#include "init.h"
#include <math.h> //sin and cos

// might be able to use a global or something for dt
double** compute_A(double x, double y, double z, double dr, double dp, double dyaw, double dt) {
    //there's no malloc() in this system, so just pass around a reference to statically declared memory
    static double A[12][12] = {0};

    double cos_dp_dt = cos(dp*dt);
    double cos_dt_dyaw = cos(dt*dyaw);
    double cos_dr_dt = cos(dr*dt);
    double sin_dt_dyaw = sin(dt*dyaw);
    double sin_dr_dt = sin(dr*dt);
    double sin_dp_dt = sin(dp*dt);

    A[0][0] = cos_dp_dt*cos_dt_dyaw;
    A[0][1] = -cos_dr_dt*sin_dt_dyaw+cos_dt_dyaw*sin_dp_dt*sin_dr_dt;
    A[0][2] = sin_dr_dt*sin_dt_dyaw+cos_dr_dt*cos_dt_dyaw*sin_dp_dt;
    A[0][6] = dt;
    A[0][9] = dt*(y*(sin_dr_dt*sin_dt_dyaw+cos_dr_dt*cos_dt_dyaw*sin_dp_dt)+z*(cos_dr_dt*sin_dt_dyaw-cos_dt_dyaw*sin_dp_dt*sin_dr_dt));
    A[0][10] = dt*cos_dt_dyaw*(-x*sin_dp_dt+z*cos_dp_dt*cos_dr_dt+y*cos_dp_dt*sin_dr_dt);
    A[0][11] = -dt*y*(cos_dr_dt*cos_dt_dyaw+sin_dp_dt*sin_dr_dt*sin_dt_dyaw)+dt*z*(cos_dt_dyaw*sin_dr_dt-cos_dr_dt*sin_dp_dt*sin_dt_dyaw)-dt*x*cos_dp_dt*sin_dt_dyaw;
    A[1][0] = cos_dp_dt*sin_dt_dyaw;
    A[1][1] = cos_dr_dt*cos_dt_dyaw+sin_dp_dt*sin_dr_dt*sin_dt_dyaw;
    A[1][2] = -cos_dt_dyaw*sin_dr_dt+cos_dr_dt*sin_dp_dt*sin_dt_dyaw;
    A[1][7] = dt;
    A[1][9] = dt*(-y*(cos_dt_dyaw*sin_dr_dt-cos_dr_dt*sin_dp_dt*sin_dt_dyaw)-z*(cos_dr_dt*cos_dt_dyaw+sin_dp_dt*sin_dr_dt*sin_dt_dyaw));
    A[1][10] = dt*sin_dt_dyaw*(-x*sin_dp_dt+z*cos_dp_dt*cos_dr_dt+y*cos_dp_dt*sin_dr_dt);
    A[1][11] = dt*(-y*(cos_dr_dt*sin_dt_dyaw-cos_dt_dyaw*sin_dp_dt*sin_dr_dt)+z*(sin_dr_dt*sin_dt_dyaw+cos_dr_dt*cos_dt_dyaw*sin_dp_dt)+x*cos_dp_dt*cos_dt_dyaw);
    A[2][0] = -sin_dp_dt;
    A[2][1] = cos_dp_dt*sin_dr_dt;
    A[2][2] = cos_dp_dt*cos_dr_dt;
    A[2][8] = dt;
    A[2][9] = dt*cos_dp_dt*(y*cos_dr_dt-z*sin_dr_dt);
    A[2][10] = -dt*(x*cos_dp_dt+z*cos_dr_dt*sin_dp_dt+y*sin_dp_dt*sin_dr_dt);
    A[3][3] = 1.0;
    A[3][9] = dt;
    A[4][4] = 1.0;
    A[4][10] = dt;
    A[5][5] = 1.0;
    A[5][11] = dt;
    A[6][6] = 1.0;
    A[7][7] = 1.0;
    A[8][8] = 1.0;
    A[9][9] = 1.0;
    A[10][10] = 1.0;
    A[11][11] = 1.0;

    return A;
}

double** compute_H(double dt) {
    static double H[13][12] = {0};
    //the circumference of the wheel in meters
    #define WHEEL_CIRCUMFERENCE 1.0
    //the radius in meters of the robot (e.g. the distance from the robot's center of rotation
    //to any one of its wheels (assume the wheels are equidistant from the center of rotation))
    #define ROBOT_RADIUS 1.0
    //the cosine of the angle made between the horizontal line that goes intersects the robot's
    //center of rotation and the ray that joins the robot's center of rotation to its front right wheel
    #define COS_THETA 0.5

    #define K ROBOT_RADIUS * COS_THETA / WHEEL_CIRCUMFERENCE

    //the scale of the GPS X measurement in values/meter
    #define GPS_X_SCALE 1.0
    //the scale of the GPS Y measurement in values/meter
    #define GPS_Y_SCALE 1.0
    //the scale of the GPS Z measurement in values/meter
    #define GPS_Z_SCALE 1.0

    //the scale of the gyroscope in values/radian (degrees?)
    #define GYRO_SCALE 1.0

    //the scale of the IMU in values/m/sec
    #define IMU_SCALE 1.0

    H[0][0] = GPS_X_SCALE;
    H[1][1] = GPS_Y_SCALE;
    H[2][2] = GPS_Z_SCALE;
    H[3][3] = GYRO_SCALE;
    H[4][4] = GYRO_SCALE;
    H[5][5] = GYRO_SCALE;
    H[6][6] = IMU_SCALE;
    H[7][7] = IMU_SCALE;
    H[8][8] = IMU_SCALE;
    H[9][6] = dt/WHEEL_CIRCUMFERENCE;
    H[9][11] = K*dt;
    H[10][6] = dt/WHEEL_CIRCUMFERENCE;
    H[10][11] = -K*dt;
    H[11][6] = dt/WHEEL_CIRCUMFERENCE;
    H[11][11] = -K*dt;
    H[12][6] = dt/WHEEL_CIRCUMFERENCE;
    H[12][11] = K*dt;

    return H;
}


// returns the identity matrix I_12
double** compute_B(void) {
    static double B[12][12];
    
    for (char i = 0; i < 12; i++) {
        for (char j = 0; j < 12; j++) {
            // 1 along diagonal; 0 elsewhere
            B[i][j] = i == j;
        }
    }
    
}

void loop(void) {
    //implement
}

int main(void) {
    SystemInit();
    AppInit();
    for(;;) loop();
}
