#ifndef PBS_GENERATING_SOURCE_H
#define PBS_GENERATING_SOURCE_H

#include "Source.h"
#include <Eigen/Dense>
#include <random>


/// Continuously generates particles
class GeneratingSource : public Source {

public:
    int m_maxParticles;
    double m_particlesPerSecond;

    Eigen::Vector3d m_position;
    Eigen::Vector3d m_particleVelocity;

    double m_positionStdDeviation;
    double m_particleVelocityStdDeviation;

    explicit GeneratingSource(Fluid *fluid);

    GeneratingSource(Fluid *fluid, int maxParticles, int particlesPerSecond,
                                   const Eigen::Vector3d& pos, double posStd, 
                                   const Eigen::Vector3d& vel, double velStd);

    void init() override;

    bool advance(double t) override;

    std::string toString() override;

private:
    int m_particleCount;

    std::mt19937 m_gen;
    Eigen::Vector3d random(double variance);

};


#endif //PBS_GENERATING_SOURCE_H
