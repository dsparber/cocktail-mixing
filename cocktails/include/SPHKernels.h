#pragma once

#include <math.h>
#include <Eigen/Dense>

/*
 * All of the following methods assume 0 <= r <= h and
 */

class SPHKernels{

public:
    SPHKernels(double kernelRadius);

    double wPoly6(double r2);

    Eigen::Vector3d gwPoly6(const Eigen::Vector3d& r);

    double lwPoly6(double r2);

    double wSpiky(double r);

    Eigen::Vector3d gwSpiky(const Eigen::Vector3d& r);

    double lwVisc(double r);

private:
    double m_h;
    double m_h2;

    double cPoly6;
    double cgPoly6;
    double cSpiky;
    double cgSpiky;
    double clVisc;
};
