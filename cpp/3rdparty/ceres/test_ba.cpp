//
// Created by books on 19-12-8.
//

#include <iostream>
#include <random>
#include <cmath>
#include <gtest/gtest.h>

#include <Eigen/Core>
#include <ceres/ceres.h>
#include <ceres/rotation.h>

class BALCameraProjectionError
{
    //	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
public:
    BALCameraProjectionError (const Eigen::Matrix3d &camera, const double &x, const double &y)
            : m_camera(camera), m_observe_x(x), m_observe_y(y)
    {
    }

    template<typename T>
    bool operator() (const T *const transform, const T *const point, T *residual) const
    {
//        // camera[0,1,2] are the angle-axis rotation.
//        T p[3];
//        ceres::AngleAxisRotatePoint(transform, point, p);
//        // camera[3,4,5] are the translation.
//        p[0] += transform[3]; p[1] += transform[4]; p[2] += transform[5];
//        T xp =  p[0] / p[2];
//        T yp =  p[1] / p[2];
//        residual[0] = xp - T(m_observe_x);
//        residual[1] = yp - T(m_observe_y);
//        return true;

		Eigen::AngleAxis<T> angle_z(transform[2], Eigen::Matrix<T, 3, 1>::UnitZ());
		Eigen::AngleAxis<T> angle_y(transform[1], Eigen::Matrix<T, 3, 1>::UnitY());
		Eigen::AngleAxis<T> angle_x(transform[0], Eigen::Matrix<T, 3, 1>::UnitX());
		Eigen::Matrix<T, 3, 3> rot;
		rot = angle_z * angle_y * angle_x;
		Eigen::Map<const Eigen::Matrix<T, 3, 1>> translation(transform + 3);

		/// 世界坐标
		Eigen::Map<const Eigen::Matrix<T, 3, 1>> p(point);
		/// 相机坐标
		Eigen::Matrix<T, 3, 1> p_c = rot * p + translation;
		/// 归一化相机坐标
		Eigen::Matrix<T, 3, 1> p_cn;
		p_cn << p_c(0) / p_c(2), p_c(1) / p_c(2), T(1.0);
		/// 图像坐标
		Eigen::Matrix<T, 3, 1> p_i = m_camera.cast<T>() * p_cn;

		residual[0] = p_i(0) - T(m_observe_x);
		residual[1] = p_i(1) - T(m_observe_y);

		return true;
    }

    static ceres::CostFunction *Create (const Eigen::Matrix3d &camera, const double &x, const double &y)
    {
        return (new ceres::AutoDiffCostFunction<BALCameraProjectionError, 2, 6, 3>(new BALCameraProjectionError(camera, x, y)));
    }


private:
    const Eigen::Matrix3d m_camera;
    const double m_observe_x;
    const double m_observe_y;
};

TEST(CERES_BA, camera_3d_2d)
{
    std::default_random_engine e(std::time(nullptr));
	std::uniform_real_distribution<> xyzUniform(15.0, 50.0);
	std::normal_distribution<double> angleNormal(0, 15 * M_PI / 180.0);
	std::normal_distribution<double> xyzNormal(0, 0.1);
	
	Eigen::Matrix3d K = Eigen::Matrix3d::Identity();
	
	double init_roll = 30 * M_PI / 180.0;
	double init_pitch = 45 * M_PI / 180.0;
	double init_yaw = 20 * M_PI / 180.0;
	double init_dx = 2.0;
	double init_dy = 1.0;
	double init_dz = 1.5;
	std::vector<double> init_transform = {init_roll, init_pitch, init_yaw, init_dx, init_dy, init_dz};
	Eigen::Matrix3d init_rotation;
	init_rotation = Eigen::AngleAxisd(init_yaw, Eigen::Vector3d::UnitZ()) *
					Eigen::AngleAxisd(init_pitch, Eigen::Vector3d::UnitY()) *
					Eigen::AngleAxisd(init_roll, Eigen::Vector3d::UnitX());
	Eigen::Vector3d init_translation(init_dx, init_dy, init_dz);
	
	std::vector<std::vector<double>> init_pts;
	std::vector<std::vector<double>> observe_coors;
	for (int i = 0; i < 10; i++)
	{
		double init_tmpx = xyzUniform(e);
		double init_tmpy = xyzUniform(e);
		double init_tmpz = xyzUniform(e);
		init_pts.push_back({init_tmpx, init_tmpy, init_tmpz});
		
		Eigen::Vector3d init_pt_w(init_tmpx, init_tmpy, init_tmpz);
		Eigen::Vector3d init_pt_c = init_rotation * init_pt_w + init_translation;
		Eigen::Vector3d init_pt_cn(init_pt_c(0) / init_pt_c(2), init_pt_c(1) / init_pt_c(2), 1.0);
		Eigen::Vector3d init_pt_i = K * init_pt_cn;
		observe_coors.push_back({init_pt_i(0), init_pt_i(1)});
	}
	
	std::vector<double> transfrom({init_roll + angleNormal(e), init_pitch + angleNormal(e),
								   init_yaw + angleNormal(e), init_dx + xyzNormal(e),
								   init_dy + xyzNormal(e), init_dz + xyzNormal(e)});
	std::vector<std::vector<double>> pts;
	for (const std::vector<double> &init_pt : init_pts)
	{
		pts.push_back({init_pt[0] + xyzNormal(e), init_pt[1] + xyzNormal(e), init_pt[2] + xyzNormal(e)});
	}
	
	std::cout << "Origin Transform = (" << init_transform[0] << " " << init_transform[1] << " " << init_transform[2]
			  << " " << init_transform[3] << " " << init_transform[4] << " " << init_transform[5] << ")" << std::endl;
	std::cout << "Estimate Transform = (" << transfrom[0] << " " << transfrom[1] << " " << transfrom[2]
			  << " " << transfrom[3] << " " << transfrom[4] << " " << transfrom[5] << ")" << std::endl;
//    for (size_t i = 0; i < init_pts.size(); i++)
//    {
//        std::cout << "Origin Pt = (" << init_pts[i][0] << " " << init_pts[i][1] << " " << init_pts[i][2] << ")"
//                  << std::endl;
//        std::cout << "Estimate Pt = (" << pts[i][0] << " " << pts[i][1] << " " << pts[i][2] << ")" << std::endl;
//    }
	
	ceres::Problem problem;
	for (size_t i = 0; i < pts.size(); i++)
	{
		ceres::CostFunction *cost_func = BALCameraProjectionError::Create(K, observe_coors[i][0], observe_coors[i][1]);
		problem.AddResidualBlock(cost_func, new ceres::CauchyLoss(0.5), transfrom.data(), pts[i].data());
		problem.SetParameterBlockConstant(pts[i].data());
	}
	
	ceres::Solver::Options options;
	options.linear_solver_type = ceres::DENSE_SCHUR;
	options.minimizer_progress_to_stdout = true;
	ceres::Solver::Summary summary;
	ceres::Solve(options, &problem, &summary);
	std::cout<<summary.BriefReport()<<std::endl;
	

	std::cout << "Estimate Transform = (" << transfrom[0] << " " << transfrom[1] << " " << transfrom[2]
			  << " " << transfrom[3] << " " << transfrom[4] << " " << transfrom[5] << ")" << std::endl;

	
//	for (size_t i = 0; i < init_pts.size(); i++)
//	{
//		std::cout << "Estimate Pt = (" << pts[i][0] << " " << pts[i][1] << " " << pts[i][2] << ")" << std::endl;
//	}
	
}
