//
// Created by books on 19-2-27.
//

#include "Grid.h"

namespace VOLib
{
	
	Grid::Grid ()
	{
		m_rows = 200;
		m_cols = 200;
		
		m_width = 0.2;
		m_height = 0.2;
		
	}
	
	Grid::~Grid ()
	{
	
	}
	
	void Grid::process (const CloudXYZI &input)
	{
		m_data.clear();
		
		m_data.resize(m_rows * m_cols);
		
		double lt_x = -m_cols * m_width / 2.0;
		double lt_y = -m_rows * m_height / 2.0;
		
		double rb_x = m_cols * m_width / 2.0;
		double rb_y = m_rows * m_height / 2.0;
		
		for (const pcl::PointXYZI &pt : input)
		{
			if (pt.z <-1.9)
				continue;
			if (pt.x < lt_x || pt.x >= rb_x || pt.y < lt_y || pt.y >= rb_y)
				continue;
			
			double x = pt.x - lt_x;
			double y = pt.y - lt_y;
			
			int row = std::floor(y / m_height);
			int col = std::floor(x / m_width);
			
			if (row < 0 || row >= m_rows || col < 0 || col >= m_cols)
				continue;
			
			m_data[row * m_cols + col].push_back(pt);
		}
		
	}
	
	
	std::vector<double> Grid::toHeightArray ()
	{
		std::vector<double> height_array(m_rows * m_cols, -1000);
		double min_height = 1000000, max_height = -10000000;
		for (int i = 0; i < m_rows; i++)
		{
			for (int j = 0; j < m_cols; j++)
			{
				CloudXYZI pts = m_data[i * m_cols + j];
				if (pts.empty())
					continue;
				
				double height = (std::max_element(pts.begin(), pts.end(), [] (const pcl::PointXYZI &pt1,
																			  const pcl::PointXYZI &pt2) { return pt1.z < pt2.z; }))->z;
				height_array[i * m_cols + j] = height;
				min_height = min_height < height ? min_height : height;
				max_height = max_height > height ? max_height : height;
			}
		}
		
		std::vector<double> result(m_rows * m_cols, 0);
		for (int i = 0; i < height_array.size(); i++)
		{
			if (height_array[i] == -1000)
				continue;
			result[i] = (height_array[i] - min_height) / (max_height - min_height);
		}
		return result;
	}
	
	std::vector<double> Grid::toIntensityArray ()
	{
		std::vector<double> intensity_array(m_rows * m_cols, -1000);
		double min_intensity = 10000000, max_intensity = -10000000;
		for (int i = 0; i < m_rows; i++)
		{
			for (int j = 0; j < m_cols; j++)
			{
				CloudXYZI pts = m_data[i * m_cols + j];
				if (pts.empty())
					continue;
				
				double intensity = (std::max_element(pts.begin(), pts.end(), [] (const pcl::PointXYZI &pt1,
																				 const pcl::PointXYZI &pt2) { return pt1.intensity < pt2.intensity; }))->intensity;
				//                double intensity = 0.0;
				//                for (const pcl::PointXYZI &pt : pts)
				//                    intensity += pt.intensity;
				//                intensity/=pts.size();
				intensity_array[i * m_cols + j] = intensity;
				max_intensity = max_intensity > intensity ? max_intensity : intensity;
				min_intensity = min_intensity < intensity ? min_intensity : intensity;
			}
		}
		
		std::vector<double> result(m_rows * m_cols, 0);
		for (int i = 0; i < intensity_array.size(); i++)
		{
			if (intensity_array[i] == -1000)
				continue;
			double tmp = (intensity_array[i] - min_intensity) / (max_intensity - min_intensity);
			result[i] = (intensity_array[i] - min_intensity) / (max_intensity - min_intensity);
		}
		
		return result;
	}
	
	std::vector<double> Grid::toHeightDiffArray ()
	{
		std::vector<double> height_diff_array(m_rows * m_cols, -1000);
		double height_diff_max = -100000000, height_diff_min = 1000000000;
		for (int i = 0; i < m_rows; i++)
		{
			for (int j = 0; j < m_cols; j++)
			{
				CloudXYZI pts = m_data[i * m_cols + j];
				if (pts.empty())
					continue;
				
				double height_max = (std::max_element(pts.begin(), pts.end(), [] (const pcl::PointXYZI &pt1,
																				  const pcl::PointXYZI &pt2) { return pt1.z < pt2.z; }))->z;
				double height_min = (std::min_element(pts.begin(), pts.end(), [] (const pcl::PointXYZI &pt1,
																				  const pcl::PointXYZI &pt2) { return pt1.z < pt2.z; }))->z;
				double height_diff = height_max - height_min;
				height_diff_array[i] = height_diff;
				height_diff_max = height_diff_max > height_diff ? height_diff_max : height_diff;
				height_diff_min = height_diff_min < height_diff ? height_diff_min : height_diff;
			}
		}
		
		/// normalize to [0,1]
		std::vector<double> result(m_rows * m_cols, 0);
		for (int i = 0; i < height_diff_array.size(); i++)
		{
			if (height_diff_array[i] == -1000)
				continue;
			result[i] = 255 * (height_diff_array[i] - height_diff_min) / (height_diff_max - height_diff_min);
		}
		
		return result;
	}
	
	CloudXYZI Grid::getData () const
	{
		CloudXYZI cloud;
		for (const CloudXYZI &pts : m_data)
		{
			cloud += pts;
		}
		return cloud;
	}
	
	
}
