#ifndef SURFACE_EXTRACTOR
#define SURFACE_EXTRACTOR

#include <vector>
#include <string>

class NeighborSearch;
class Particle;

struct SurfaceExtractorInfo{
public:
	SurfaceExtractorInfo() = default;
    SurfaceExtractorInfo(double res, double kernel_radius, double particle_radius, double isosurface) 
        : _res(res), _kernel_radius(kernel_radius), _particle_radius(particle_radius), _isosurface(isosurface) {}
    double _res;
    double _kernel_radius;
    double _particle_radius;
    double _isosurface;
};


class SurfaceExtractor {
public:

	SurfaceExtractor();

	/* Given particle locations from an SPH simulation, calculate the densities and extract an isosurface 
	   positions: particle positions of the same fluid
	   file_path: where to save as obj file
	*/
	void createMesh(std::vector<Particle>& particles, std::string file_path);
	
	SurfaceExtractorInfo m_info;

};


#endif // SURFACE_EXTRACTOR