#ifndef PBS_KERNELS_H
#define PBS_KERNELS_H

#include <math.h>
#include <Eigen/Dense>

/*
 * All of the following methods assume 0 <= r <= h and
 */
namespace kernels {

    double wPoly6(double r2, double h) {
        double h2 = h*h;
        return 315 / (64 * M_PI) * pow(h2 - r2, 3) / pow(h, 9);
    }

    double wSpiky(double r, double h) {
        return 15 / M_PI * pow(h - r, 3) / M_PI * pow(h, 6);
    }

    Eigen::Vector3d gradWSpiky(const Eigen::Vector3d& r, double h) {
        auto gradient = -r * 45 * pow(h - r.norm(), 2) / (M_PI * pow(h,6));
        return gradient;
    }

    double lapWViscosity(double r, double h) {
        return 45 * (h - r) / (M_PI * pow(h, 6));
    }

} // namespace kernels


#endif // PBS_KERNELS_H