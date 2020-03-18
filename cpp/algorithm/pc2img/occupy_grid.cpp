//
// Created by books on 20-2-10.
//

#include <iostream>
#include <vector>

#include <pcl/point_cloud.h>
#include <pcl/io/pcd_io.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Grid.h"

int main ()
{
	VOLib::Grid g_grid;
	
	int g_rows = 600;
	int g_cols = 600;
	double g_width = 0.2;
	double g_height = 0.2;
	
	g_grid.setRows(g_rows);
	g_grid.setCols(g_cols);
	g_grid.setWidth(g_width);
	g_grid.setHeight(g_height);
	
	std::string pcd_dir = "/home/books/autoware_data/pcd/";
	
	for(int index=0;index<4000;index++)
	{
		std::string pcd_file=pcd_dir+std::to_string(index)+".pcd";
		
		CloudXYZI::Ptr cloud_xyzi(new CloudXYZI);
		pcl::io::loadPCDFile(pcd_file, *cloud_xyzi);
		
		if (cloud_xyzi->empty())
			continue;
		
		g_grid.process(*cloud_xyzi);
		
		std::vector<double> heights=g_grid.toHeightArray();
		std::vector<double> intensitys=g_grid.toIntensityArray();
		
		std::vector<uchar> heights_gray =VOLib::double2Gray(heights);
		std::vector<uchar> intensitys_gray = VOLib::double2Gray(intensitys);
		
		int rows = g_grid.getRows(), cols = g_grid.getCols();
		cv::Mat img_height(rows, cols, CV_8UC1, heights_gray.data());
		cv::Mat img_intensity(rows, cols, CV_8UC1, intensitys_gray.data());
		
		cv::equalizeHist(img_height,img_height);
		cv::equalizeHist(img_intensity,img_intensity);
		
		cv::Mat img_merge(img_height.rows, img_height.cols, CV_8UC3);
		for (int i = 0; i < img_height.rows; i++) {
			uchar *hgt_row = img_height.ptr<uchar>(i);
			uchar *its_row = img_intensity.ptr<uchar>(i);
			cv::Vec3b *merge_row = img_merge.ptr<cv::Vec3b>(i);
			for (int j = 0; j < img_height.cols; j++) {
				merge_row[j][0] = hgt_row[j];
				merge_row[j][2] = its_row[j];
				merge_row[j][1] = 0;
			}
		}
		
		
		for(int i=1;i<10;i++)
		{
			int s_x=i*g_cols/10;
			int s_y=i*g_rows/10;
			
			cv::line(img_merge,cv::Point2i(s_x,0),cv::Point2i(s_x,g_cols),cv::Scalar(100,100,100));
			cv::line(img_merge,cv::Point2i(0,s_y),cv::Point2i(g_rows,s_y),cv::Scalar(100,100,100));
		}
		
		cv::imshow("img_merge",img_merge);
		
//		int img_rows=rows*2;
//		int img_cols=cols*2;
//		cv::resize(img_height,img_height,cv::Size(img_rows,img_cols));
//		cv::resize(img_intensity,img_intensity,cv::Size(img_rows,img_cols));
		
//		cv::imshow("img_height",img_height);
//		cv::imshow("img_intensity",img_intensity);
		
//		cv::Mat img_height_show,img_intensity_show;
//		cv::applyColorMap(img_height,img_height_show,cv::COLORMAP_HOT);
//		cv::applyColorMap(img_intensity,img_intensity_show,cv::COLORMAP_HOT);
//		cv::imshow("img_height",img_height_show);
//		cv::imshow("img_intensity",img_intensity_show);
		
		cv::waitKey(10);
		
	}
	
	return 0;
}