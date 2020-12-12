#ifndef PBS_BLOCK_SOURCE_H
#define PBS_BLOCK_SOURCE_H

#include "Source.h"
#include <Eigen/Dense>


class BlockSource : public Source {

public:

    explicit BlockSource(Fluid *fluid);

    BlockSource(Fluid *fluid, const Eigen::Vector3i& initialDimension, double initialSpacing, const Eigen::Vector3d& initialOffset,
                const Eigen::Vector3d& initialVelocity = Eigen::Vector3d::Zero());

    BlockSource(Fluid *fluid, const Eigen::Vector3i& initialDimension, const Eigen::Vector3d& initialOffset,
                const Eigen::Vector3d& intialVelocity = Eigen::Vector3d::Zero());

    void init() override;

    double m_initialSpacing;

    Eigen::Vector3i m_initialDimension;
    Eigen::Vector3d m_initialOffset;
    Eigen::Vector3d m_initialVelocity;

    std::string toString() override;

};


#endif // PBS_BLOCK_SOURCE_H
