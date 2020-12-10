#include "../include/FluidDefinitons.h"

namespace fluids {

    Fluid *boundary = new Fluid(
            "Boundary",
            1,
            1000,
            800,
            10,
            Eigen::Vector3d(.1, .1, .1),
            true
    );

    Fluid *water = new Fluid(
            "Water",
            2,
            10000,
            1000,
            1,
            Eigen::Vector3d(0.0, 0.0, 1.0),
            false
    );

    Fluid *rum = new Fluid(
            "Rum",
            1.5,
            10000,
            900,
            1,
            Eigen::Vector3d(245, 188, 147) / 255,
            false
    );

    Fluid *orangeJuice = new Fluid(
            "Orange Juice",
            4,
            10000,
            1200,
            1,
            Eigen::Vector3d(245, 139, 0) / 255,
            false
    );


    Fluid *honey = new Fluid(
            "Honey",
            100,
            10000,
            1400,
            1,
            Eigen::Vector3d(238., 195., 109.).normalized(),
            false
    );

    std::vector<Fluid*> all = {boundary, water, rum, orangeJuice, honey};

    std::vector<Fluid*> regularFluids = {water, rum, orangeJuice, honey};
}