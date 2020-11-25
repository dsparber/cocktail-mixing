import numpy as np
import sys
import os
import trimesh


def tf_scale(points, scale_factor):
    tl = np.mean(points, axis=0)
    tl = np.tile(tl, points.shape[0])
    tl = np.reshape(tl, (-1, 3))
    points -= tl
    points *= scale_factor
    points += tl
    return points


def tf_translate(points, trans_dir):
    points -= np.reshape(np.tile(trans_dir, points.shape[0]), (-1, 3))
    return points


def preprocess(in_points, out_file, max_samples=100, normalize=False, scale=None, translate=None):
    num_points = in_points.shape[0]

    if num_points > max_samples:
        indices = np.random.choice(num_points, max_samples, replace=False)
        in_points = in_points[indices, :]
        num_points = in_points.shape[0]

    # put in unit cube box
    if normalize:
        # translate by mean
        mean_dir = np.mean(in_points, axis=0)
        in_points = tf_translate(in_points, mean_dir)
        # scale by one
        max_dist = np.max(np.abs(in_points))
        in_points = tf_scale(in_points, 0.5 / max_dist)
        in_points = tf_translate(in_points, np.array([-0.5, -0.5, -0.5]))

    if scale:
        in_points = tf_scale(in_points, scale)

    if translate is not None:
        in_points = tf_translate(in_points, translate)

    np.savetxt(out_file, in_points)


def preprocess_mesh(mesh, out_file, max_samples=100, normalize=False, scale=None, translate=None):
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

    file_extension = IN_FILE.split('.')[-1]

    if not os.path.isfile(IN_FILE) or file_extension not in ['xyz', 'obj']:
        print(f"File {IN_FILE} not found")
        print("Abort...")
        exit()

    if file_extension not in ['xyz', 'obj']:
        print(f"Cannot process file {IN_FILE}")
        print("Abort...")
        exit()

    if len(sys.argv) > 2:
        OUT_FILE = sys.argv[2]
    else:
        OUT_FILE = IN_FILE

    if file_extension == 'xyz':
        in_points = np.genfromtxt(IN_FILE)
        preprocess(in_points, OUT_FILE, max_samples=1000, normalize=True, scale=2, translate=np.array([0.0, -2.0, 0.0]))

    elif file_extension == 'obj':
        mesh = trimesh.load(IN_FILE)
        preprocess_mesh(mesh, OUT_FILE, max_samples=1000, normalize=True, scale=2, translate=np.array([0.0, -1.0, 0.0]))
