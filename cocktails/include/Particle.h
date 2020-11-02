#ifndef PBS_PARTICLE_H
#define PBS_PARTICLE_H

#include <Eigen/Dense>

class Particle {

public:
    Particle() : m_mass(1), m_pressure(0), m_density(1), m_pos(0), m_vel(0), m_acc(0), m_color(0) { }

    double m_mass;
    double m_pressure;
    double m_density;
    Eigen::Vector3d m_pos;
    Eigen::Vector3d m_vel;
    Eigen::Vector3d m_acc;
    Eigen::Vector3d m_color;

};


#endif //PBS_PARTICLE_H
