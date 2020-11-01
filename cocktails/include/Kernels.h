#ifndef PBS_KERNELS_H
#define PBS_KERNELS_H

#include <math.h>
#include <Eigen/Dense>

namespace kernels {
    
    double Wpoly6(double r2, double h) {
        double h2 = h*h;
        return (r2 < h2) ? 315.0 * pow(h2 - r2, 3) / (64.0 * pow(h, 9)) : 0.0;
    }

    double Wspiky(double r, double h) {
        return (r < h) ? (15.0 * pow(h-r, 3) / (M_PI * pow(h, 6))) : 0.0;
    }

    Eigen::Vector3d gradWspiky(const Eigen::Vector3d& r, double h) {
        if(r.norm() < h*h) {
            Eigen::Vector3d gradient = r.normalized();
            gradient *= - 45.0 * pow(h - r.squaredNorm(), 2) / (M_PI * pow(h,6));
            return gradient;
        }else{
            return Eigen::Vector3d(0.0,0.0,0.0);
        }
    }

    double lapWViscosity(double r, double h) {
        return (r < h) ? (45 * (h - r) / (M_PI*pow(h, 6))) : 0.0;
    } 

} // namespace kernels


#endif // PBS_KERNELS_H