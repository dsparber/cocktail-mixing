#ifndef PBS_SOURCE_H
#define PBS_SOURCE_H

#include <vector>
#include "Fluid.h"

/// Base class for all sources. A source is responsible for generating particles.
class Source {

public:
    Fluid* m_fluid;

    explicit Source(Fluid *fluid) : m_fluid(fluid) {}

    virtual void init() {}

    virtual bool advance(double t) {
        return false;
    };

    virtual std::string toString() {
        return "Source (" + m_fluid->m_name +")";
    }

};


#endif // PBS_SOURCE_H
