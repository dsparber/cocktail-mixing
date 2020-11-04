#ifndef PBS_CONSTANTS_H
#define PBS_CONSTANTS_H

#include "Fluid.h"

namespace fluids {

    Fluid water() {
        Fluid fluid;
        fluid.m_V = pow(0.1, 3);
        fluid.m_viscosity = 2;
        fluid.m_stiffness = 1000;
        fluid.m_particleMass = 1;
        fluid.m_restDensity = 1000;
        return fluid;
    }

};



#endif //PBS_CONSTANTS_H
