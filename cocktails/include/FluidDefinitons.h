#ifndef PBS_FLUID_DEFINITIONS_H
#define PBS_FLUID_DEFINITIONS_H

#include <vector>
#include <string>
#include "Fluid.h"

namespace fluids {

    Fluid *water = new Fluid(
            "Water",
            pow(0.1, 3),
            2,
            1000,
            1000,
            1
    );

    Fluid *honey = new Fluid(
            "Honey",
            pow(0.1, 3),
            100,
            1000,
            1000,
            1
    );

    std::vector<Fluid*> all = {water, honey};

}

#endif //PBS_FLUID_DEFINITIONS_H
