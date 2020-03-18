//
// Created by books on 20-2-11.
//

#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>

int main ()
{
	std::string dir = "/home/books/Pictures/pic_grid/";
	for (int index = 0; index < 11; index++)
	{
		std::string name = dir + std::to_string(index) + "-4.png";
		cv::Mat img_merge = cv::imread(name);
		
		cv::Mat img_hgt(img_merge.rows, img_merge.cols, CV_8UC1);
		cv::Mat img_its(img_merge.rows, img_merge.cols, CV_8UC1);
		for (int i = 0; i < img_merge.rows; i++)
		{
			uchar *hgt_row = img_hgt.ptr<uchar>(i);
			uchar *its_row = img_its.ptr<uchar>(i);
			cv::Vec3b *merge_row = img_merge.ptr<cv::Vec3b>(i);
			for (int j = 0; j < img_merge.cols; j++)
			{
				hgt_row[j] = merge_row[j][1];
				its_row[j] = merge_row[j][2];
			}
		}
		
		cv::Mat img_height_show, img_intensity_show;
		cv::applyColorMap(img_hgt, img_height_show, cv::COLORMAP_HOT);
		cv::applyColorMap(img_its, img_intensity_show, cv::COLORMAP_HOT);
		std::string hgt_name = dir + std::to_string(index) + "-height.png";
		std::string its_name = dir + std::to_string(index) + "-intensity.png";
		
		cv::imwrite(hgt_name, img_height_show);
		cv::imwrite(its_name, img_intensity_show);
	}
	return 0;
}