#ifndef PBS_EMITTING_SOURCE_H
#define PBS_EMITTING_SOURCE_H

#include "Source.h"
#include <Eigen/Dense>
#include <random>

class EmittingSource : public Source {

public:
    int m_maxParticles;
    double m_particlesPerSecond;
    int m_x, m_y, m_z;
    double m_spacing;
    Eigen::Vector3d m_position;
    Eigen::Vector3d m_particleVelocity;

    explicit EmittingSource(Fluid *fluid);

    void init() override;

    bool advance(double t) override;

    std::string toString() override;

private:
    int m_particleCount;

};


#endif //PBS_EMITTING_SOURCE_H
