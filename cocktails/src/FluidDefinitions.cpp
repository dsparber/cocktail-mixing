#include "../include/FluidDefinitons.h"

namespace fluids {

    Fluid *boundary = new Fluid(
            "Boundary",
            1.,
            1000.,
            800.,
            10.,
            Eigen::Vector3d(.1, .1, .1),
            true
    );

    Fluid *water = new Fluid(
            "Water",
            4.,
            2000.,
            1000.,
            1.,
            Eigen::Vector3d(0.0, 0.0, 1.0),
            false,
            5.,
            1e-6
    );

    Fluid *liquid1 = new Fluid(
            "Liquid1",
            20,
            2000.,
            2000.,
            1,
            Eigen::Vector3d(255., 255., 255.) / 255.,
            false,
            10.,
            1e-6
    );

    Fluid *liquid2 = new Fluid(
            "Liquid2",
            4,
            2000,
            800,
            1,
            Eigen::Vector3d(56., 200., 218) / 255.,
            false,
            20.,
            1e-6
    );


    Fluid *honey = new Fluid(
            "Honey",
            100,
            2000,
            1400,
            1,
            Eigen::Vector3d(238., 195., 109.) / 255.,
            false,
            5.,
            1e-6
    );

    std::vector<Fluid*> all = {boundary, water, liquid1, liquid2, honey};

    std::vector<Fluid*> regularFluids = {water, liquid1, liquid2, honey};
}