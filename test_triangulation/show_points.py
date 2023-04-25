import numpy as np
import open3d as o3d


if __name__ == "__main__":
    # load points
    points_file = "build/points.txt"
    points = np.loadtxt(points_file)

    # points[:, 2] = -points[:, 2]
    pcd = o3d.geometry.PointCloud()
    pcd.points = o3d.utility.Vector3dVector(points)
    # o3d.io.write_point_cloud("../../TestData/sync.ply", pcd)

    # Load saved point cloud and visualize it
    # pcd_load = o3d.io.read_point_cloud("../../TestData/sync.ply")

    coord = o3d.geometry.create_mesh_coordinate_frame(size=5.0, origin=np.array([0., 0., 0.]))

    o3d.visualization.draw_geometries([pcd, coord])

