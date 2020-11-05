#include "../include/FluidDefinitons.h"

Fluid fluids::water() {
    Fluid fluid;
    fluid.m_V = pow(0.1, 3);
    fluid.m_viscosity = 2;
    fluid.m_stiffness = 1000;
    fluid.m_restDensity = 1000;
    fluid.m_particleMass = fluid.m_restDensity * fluid.m_V;
    return fluid;
}

Fluid fluids::waterBlock() {
    Fluid fluid;
    fluid.m_V = pow(0.1, 3);
    fluid.m_viscosity = 2;
    fluid.m_stiffness = 1000;
    fluid.m_particleMass = 1;
    fluid.m_restDensity = 1000;
    fluid.m_initialDimension << 10, 10, 10;
    fluid.m_initialSpacing = .1;
    fluid.m_initialOffset << 1.5, 2.5, 1.5;
    return fluid;
}