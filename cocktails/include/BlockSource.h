#ifndef PBS_BLOCK_SOURCE_H
#define PBS_BLOCK_SOURCE_H

#include "Source.h"
#include <Eigen/Dense>


class BlockSource : public Source {

public:

    explicit BlockSource(Fluid *fluid);

    void init() override;

    double m_initialSpacing;
    Eigen::Vector3i m_initialDimension;
    Eigen::Vector3d m_initialOffset;

    std::string toString() override;

};


#endif // PBS_BLOCK_SOURCE_H
