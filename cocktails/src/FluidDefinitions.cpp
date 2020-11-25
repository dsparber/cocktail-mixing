#include "../include/FluidDefinitons.h"

namespace fluids {

    Fluid *boundary = new Fluid(
            "Boundary",
            1,
            1000,
            1000,
            1,
            Eigen::Vector3d(.1, .1, .1)
    );

    Fluid *water = new Fluid(
            "Water",
            2,
            1000,
            1000,
            1,
            Eigen::Vector3d(0.0, 0.0, 1.0)
    );


    Fluid *honey = new Fluid(
            "Honey",
            100,
            1000,
            1400,
            1,
            Eigen::Vector3d(238., 195., 109.).normalized()
    );

    std::vector<Fluid*> all = {boundary, water, honey};

    std::vector<Fluid*> allFluids = {water, honey};
}