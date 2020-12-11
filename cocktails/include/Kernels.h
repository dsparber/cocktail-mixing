#ifndef PBS_KERNELS_H
#define PBS_KERNELS_H

#include <math.h>
#include <Eigen/Dense>

/*
 * All of the following methods assume 0 <= r <= h and
 */
namespace kernels {

    inline double cPoly6 = 315 / (64 * M_PI);


    static double wPoly6(double r2, double h) {
        double h2 = h*h;
        return  cPoly6 * pow(h2 - r2, 3) / pow(h, 9);
    }

    static double wSpiky(double r, double h) {
        return 15 * pow(h - r, 3) / (M_PI * pow(h, 6));
    }

    static Eigen::Vector3d gradWSpiky(const Eigen::Vector3d& r, double h) {
        auto gradient = -r * 45 * pow(h - r.norm(), 2) / (M_PI * pow(h,6));
        return gradient;
    }

    static double lapWViscosity(double r, double h) {
        return 45 * (h - r) / (M_PI * pow(h, 6));
    }

    static double taitEq(double k, double rest_density, double density) {
        return k * rest_density * (pow(density/rest_density, 7) - 1) / 7.0;
    }
} // namespace kernels


#endif // PBS_KERNELS_H