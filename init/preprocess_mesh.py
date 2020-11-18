import numpy as np
import sys
import os
import trimesh
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), os.pardir))

from preprocess import preprocess

def preprocess_mesh(mesh, out_file, max_samples=100, normalize = False, scale=None, translate=None):
    inside_points = trimesh.sample.volume_mesh(mesh, 10 * max_samples // 2)
    print(f"{inside_points.shape[0]} inside points")
    surface_points, _ = trimesh.sample.sample_surface_even(mesh, max_samples - inside_points.shape[0])
    in_points = np.concatenate((surface_points, inside_points))
    preprocess(in_points, out_file, max_samples, normalize, scale, translate)

if __name__ == "__main__":

    if len(sys.argv) < 2:
        print("python preprocess.py FILE_NAME")
        exit()

    IN_FILE = sys.argv[1]

    if not os.path.isfile(IN_FILE):
        print(f"Cannot open file {IN_FILE}")
        print("Abort...")
        exit()

    if len(sys.argv) > 3:
        OUT_FILE = sys.argv[2]
    else:
        OUT_FILE = IN_FILE

    mesh = trimesh.load(IN_FILE)
    preprocess_mesh(mesh, OUT_FILE, max_samples=1000, normalize=True, scale=2, translate=np.array([0.0, -1.0, 0.0]))