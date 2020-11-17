#ifndef PBS_FLUID_DEFINITIONS_H
#define PBS_FLUID_DEFINITIONS_H

#include <vector>
#include <string>
#include "Fluid.h"

namespace fluids {

    Fluid *water = new Fluid(
            "Water",
            2,
            1000,
            1000,
            1,
            Eigen::Vector3d(0.0,0.0,1.0)
    );

    Fluid *honey = new Fluid(
            "Honey",
            100,
            1000,
            1400,
            1,
            Eigen::Vector3d(238.,195.,109.).normalized()
    );

    std::vector<Fluid*> all = {water, honey};

}

#endif //PBS_FLUID_DEFINITIONS_H
