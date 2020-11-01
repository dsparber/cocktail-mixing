#ifndef PBS_PARTICLE_H
#define PBS_PARTICLE_H

#include <Eigen/Dense>

class Particle {

public:
    Particle() { 
        m_mass = 1.0;
        m_pressure = 0.0;
        m_density = 0.0;
        m_pos.setZero();
        m_vel.setZero();
        m_acc.setZero();
        m_color << 1, 0,  0;
    }
    double m_mass;
    double m_pressure;
    double m_density;
    Eigen::Vector3d m_pos;
    Eigen::Vector3d m_vel;
    Eigen::Vector3d m_acc;
    Eigen::Vector3d m_color;

};


#endif //PBS_PARTICLE_H
