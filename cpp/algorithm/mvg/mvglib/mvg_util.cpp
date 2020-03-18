//
// Created by books on 19-10-11.
//

#include "mvg_util.h"

namespace mvg
{
	namespace util
	{
		static std::default_random_engine e(std::time(nullptr));
		

		
		Transform2D buildRandomProjective2D ()
		{
			Transform2D random = Transform2D::Random();
			while (!isProjective2D(random) || random(2, 2) == 0)
			{
				random = Transform2D::Random();
			}
			
			double scale = 1.0 / random(2, 2);
			return scale * random;
		}
		
		
		Pt2DVec buildRandomPts2D (int n)
		{
			std::normal_distribution<double> normalDistribution(4, 10);
			mvg::Pt2DVec pts;
			for (int i = 0; i < n; i++)
			{
				mvg::Pt2D pt1;
				pt1[0] = normalDistribution(e);
				pt1[1] = normalDistribution(e);
				pt1[2] = 1.0;
				pts.push_back(pt1);
			}
			return pts;
		}
		
		Pt2DVec buildRandomPts2D (const Pt2DVec &pts, const Transform2D &T)
		{
			Pt2DVec res;
			for (const Pt2D &pt : pts)
			{
				res.push_back(T * pt);
			}
			return res;
		}
		
		Pt2DVec buildRandomPts2D (const Pt2DVec &pts, double noise)
		{
			std::normal_distribution<double> noiseDis(0, noise);
			Pt2DVec res;
			for (const Pt2D &pt : pts)
			{
				Pt2D ptNoise = Pt2D::Ones();
				ptNoise(0) = pt(0) + noiseDis(e);
				ptNoise(1) = pt(1) + noiseDis(e);
				res.push_back(ptNoise);
			}
			return res;
		}
		
	}
	
}