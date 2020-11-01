#include "ParticleSimulation.h"

/*
 * Example simulation that changes the colors of a cube.
 */
class SphSimulation : public ParticleSimulation {
public:
    SphSimulation();

	void init() override;
	void resetMembers() override;
	bool advance() override;

private:

};