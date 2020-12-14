import trimesh

path = "/home/viviane/HS2020/PBS/fluid-pbs/data/water_glass_5.obj"
mesh = trimesh.load(path)

points = trimesh.sample.volume_mesh(mesh, 10000)
print(points.shape)

import numpy as np 

np.savetxt("ip_5.xyz", points)
