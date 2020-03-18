//
// Created by books on 19-10-12.
//

#include "ransac.h"


namespace mvg
{
	
	namespace ransac
	{
		
		static std::default_random_engine e(std::time(nullptr));
		const int minRansacNum2D = 6;
		const double threshold2D=0.1;
		
		Transform2D solveProjective2D (Pt2DVec &pts1, Pt2DVec &pts2, int maxRun)
		{
			assert(pts1.size() == pts2.size() && pts1.size() >= 4);
			
			if (pts1.size() <= minRansacNum2D)
				return dlt::solveProjective2DNormalize(pts1, pts2);
			
			const int n = pts1.size();
			std::uniform_int_distribution<> uniformDis(0, n);
			
			double bestResidual = 100000 * n;
			Pt2DVec bestPts1, bestPts2;
			int run = 0;
			while (run++ < maxRun)
			{
				std::set<int> indexSet;
				while (indexSet.size() < minRansacNum2D)
				{
					int index = uniformDis(e);
					if (indexSet.count(index) == 0)
						indexSet.insert(index);
				}
				
				Pt2DVec subPts1, subPts2;
				for (int index : indexSet)
				{
					subPts1.push_back(pts1[index]);
					subPts2.push_back(pts2[index]);
				}
				
				Transform2D tmpT = dlt::solveProjective2DNormalize(subPts1, subPts2);
				double tmpResidual = algeba::transferResidual(subPts1, subPts2, tmpT);
				
				for (int i = 0; i < n; i++)
				{
					if (indexSet.count(i) == 1)
						continue;
					double tmpDis = algeba::transferResidual(pts1[i], pts2[i], tmpT);
					if (tmpDis < threshold2D)
					{
						subPts1.push_back(pts1[i]);
						subPts2.push_back(pts2[i]);
						tmpResidual += tmpDis;
					}
				}
				tmpResidual /= subPts1.size();
				
				if (tmpResidual < bestResidual)
				{
					bestResidual = tmpResidual;
					bestPts1.swap(subPts1);
					bestPts2.swap(subPts2);
				}
			}
			pts1.swap(bestPts1);
			pts2.swap(bestPts2);
			return dlt::solveProjective2DNormalize(pts1, pts2);
		}
		
		Transform2D solveProjective2DAdaptive (Pt2DVec &pts1, Pt2DVec &pts2)
		{
			assert(pts1.size() == pts2.size() && pts1.size() >= 4);
			
			if (pts1.size() <= minRansacNum2D)
				return dlt::solveProjective2DNormalize(pts1, pts2);
			
			const int n = pts1.size();
			std::uniform_int_distribution<> uniformDis(0, n);
			
			double bestResidual = 100000 * n;
			Pt2DVec bestPts1, bestPts2;
			int run = 0;
			int maxRun = 100;
			while (run++ < maxRun)
			{
				std::set<int> indexSet;
				while (indexSet.size() < minRansacNum2D)
				{
					int index = uniformDis(e);
					if (indexSet.count(index) == 0)
						indexSet.insert(index);
				}
				
				Pt2DVec subPts1, subPts2;
				for (int index : indexSet)
				{
					subPts1.push_back(pts1[index]);
					subPts2.push_back(pts2[index]);
				}
				
				Transform2D tmpT = dlt::solveProjective2DNormalize(subPts1, subPts2);
				double tmpResidual = algeba::transferResidual(subPts1, subPts2, tmpT);
				
				for (int i = 0; i < n; i++)
				{
					if (indexSet.count(i) == 1)
						continue;
					double tmpDis = algeba::transferResidual(pts1[i], pts2[i], tmpT);
					if (tmpDis < threshold2D)
					{
						subPts1.push_back(pts1[i]);
						subPts2.push_back(pts2[i]);
						tmpResidual += tmpDis;
					}
				}
				tmpResidual /= subPts1.size();
				
				if (tmpResidual < bestResidual)
				{
					bestResidual = tmpResidual;
					bestPts1.swap(subPts1);
					bestPts2.swap(subPts2);
					double w = (double) bestPts1.size() / pts1.size();
					maxRun = std::ceil(std::log(1 - 0.99) / std::log(1 - std::pow(w, minRansacNum2D)));
				}
			}
			pts1.swap(bestPts1);
			pts2.swap(bestPts2);
			return dlt::solveProjective2DNormalize(pts1, pts2);
		}
	}
	
}