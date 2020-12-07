#include "../include/SurfaceExtractor.h"
#include "../include/UniformGridNeighborSearch.h"
#include "../include/Particle.h"
#include "../include/Kernels.h"

#include <igl/writeOBJ.h>
#include <igl/copyleft/marching_cubes.h>

SurfaceExtractor::SurfaceExtractor() {
    m_info = SurfaceExtractorInfo(0.05, 0.4, 0.1, 0.5);
}

void SurfaceExtractor::createMesh(std::vector<Particle>& particles, std::string file_path) {
    // initialize uniform grid
    auto& res = m_info._res;
    auto& kernelRadius = m_info._kernel_radius;
    auto& isosurface = m_info._isosurface;

    UniformGridNeighborSearch* neighborSearch = new UniformGridNeighborSearch(0.1); // take resolution of the simulation

    if(particles.size() <= 0) {
        return;
    }

    // find bounding box
    Eigen::Vector3d m(particles.begin()->m_pos), M(particles.begin()->m_pos);
    for(auto& particle : particles){
        neighborSearch->addParticle(&particle);
        m = m.array().min(particle.m_pos.array());
        M = M.array().max(particle.m_pos.array());
    }


    // enlarge min max box a bit

    m -= 0.3 * (M - m);
    M += 0.3 * (M - m);

    Eigen::Vector3i gridDim;
    gridDim = ((M - m).array() / res).cast<int>();
    std::cout << "grid dim " << gridDim.transpose() << '\n';

    int gs = gridDim.x() * gridDim.y() * gridDim.z();
    std::cout << "gridsize " << gs << '\n';  

    Eigen::MatrixXd densities(gs, 1);
    densities.setZero();
    Eigen::MatrixXd points(gs, 3);

    std::cout << "grid points and densities set\n";
    int c = 0;
    for(int x = 0; x < gridDim.x(); x++) {
        Eigen::Vector3d pos;
        pos[0] = m.x() + x*res;
        for(int y = 0; y < gridDim.y(); y++) {
            pos[1] = m.y() + y*res;
            for(int z = 0; z  < gridDim.z(); z++) {
                pos[2] = m.z() + z*res;
                points.row(c) = pos.transpose();

                //calculate density using SPH kernel
                std::vector<Particle*> neighbors;
                neighbors = neighborSearch->getNeighbors(pos, kernelRadius);
                double density = 0.0;
                for(Particle* neighbor : neighbors) {
                    double r2 = (pos - neighbor->m_pos).squaredNorm();
                    density += neighbor->m_mass * kernels::wPoly6(r2, kernelRadius);
                }
                densities(c) = density;
                c++;
            }
        }
    }

    Eigen::MatrixXd vertices;
    Eigen::MatrixXi faces;

    // marching cubes
    igl::copyleft::marching_cubes(densities, points, gridDim.x(), gridDim.y(), gridDim.z(), isosurface, vertices, faces);

    // write to obj
    igl::writeOBJ(file_path, vertices, faces);
    std::cout << "save mesh to " << file_path << '\n';

}