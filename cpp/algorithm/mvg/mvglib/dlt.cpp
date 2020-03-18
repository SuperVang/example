#include "dlt.h"

namespace mvg
{
	
	namespace dlt
	{
		
		Transform2D solveProjective2D (const Pt2DVec &pts1, const Pt2DVec &pts2)
		{
			assert(pts1.size() == pts2.size() && pts1.size() >= 4);
			
			const int n = pts1.size();
			
			Eigen::MatrixXd A = Eigen::MatrixXd::Zero(2 * n, 9);
			for (int i = 0; i < n; i++)
			{
				Pt2D pt1 = pts1[i], pt2 = pts2[i];
				A.block(2 * i, 0, 2, 9) = algeba::linearAProjective2D(pt1, pt2);
			}
			
			Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
			Eigen::MatrixXd U = svd.matrixU();
			Eigen::MatrixXd D = svd.singularValues();
			Eigen::MatrixXd V = svd.matrixV();
			
			Eigen::Matrix<double, 9, 1> h = V.col(V.cols() - 1);
			Transform2D T = buildTransform2D(h);
			
			double scale = 1.0 / T(2, 2);
			return scale * T;
			
		}
		
		Transform2D normalize (const Pt2DVec &pts)
		{
			double sumX = 0, sumY = 0;
			for (const Pt2D &pt : pts)
			{
				sumX += pt[0];
				sumY += pt[1];
			}
			double cenX = sumX / pts.size();
			double cenY = sumY / pts.size();
			
			double sumDis = 0;
			for (const Pt2D &pt : pts)
			{
				sumDis += std::hypot(pt[0] - cenX, pt[1] - cenY);
			}
			sumDis /= pts.size();
			
			double scale = std::sqrt(2.0) / sumDis;
			double dx = -scale * sumX / pts.size();
			double dy = -scale * sumY / pts.size();
			return buildTransform2D(0, dx, dy, scale);
			
		}
		
		Transform2D solveProjective2DNormalize (const Pt2DVec &pts1, const Pt2DVec &pts2)
		{
			assert(pts1.size() == pts2.size() && pts1.size() >= 4);
			
			const int n = pts1.size();
			
			Transform2D T1 = normalize(pts1);
			Transform2D T2 = normalize(pts2);
			
			Pt2DVec norPts1, norPts2;
			for (const Pt2D &pt : pts1)
			{
				norPts1.push_back(T1 * pt);
			}
			for (const Pt2D &pt : pts2)
			{
				norPts2.push_back(T2 * pt);
			}
			
			Eigen::MatrixXd A = Eigen::MatrixXd::Zero(2 * n, 9);
			for (int i = 0; i < n; i++)
			{
				Pt2D pt1 = norPts1[i], pt2 = norPts2[i];
				A.block(2 * i, 0, 2, 9) = algeba::linearAProjective2D(pt1, pt2);
			}
			
			Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
			Eigen::MatrixXd U = svd.matrixU();
			Eigen::MatrixXd D = svd.singularValues();
			Eigen::MatrixXd V = svd.matrixV();
			
			Eigen::Matrix<double, 9, 1> h = V.col(V.cols() - 1);
			Transform2D T = T2.inverse() * buildTransform2D(h) * T1;
			
			double scale = 1.0 / T(2, 2);
			
			return scale * T;
		}
		

	}
}

