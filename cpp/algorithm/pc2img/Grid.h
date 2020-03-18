//
// Created by books on 19-2-27.
//

#ifndef PROJECT_GRID_H
#define PROJECT_GRID_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <numeric>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <opencv2/opencv.hpp>

#include <pcl/point_cloud.h>
#include <pcl/search/kdtree.h>

#include "ImgUtil.h"

typedef pcl::PointCloud<pcl::PointXYZ> CloudXYZ;
typedef pcl::PointCloud<pcl::PointXYZI> CloudXYZI;
typedef pcl::PointCloud<pcl::PointXYZRGB> CloudXYZRGB;


namespace VOLib {

    class Grid
    {
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    public:

        Grid();

        ~Grid();

        void setWidth(double width)
        { m_width = width; };

        double getWidth() const
        { return m_width; };

        void setHeight(double height)
        { m_height = height; };

        double getHeight() const
        { return m_height; };

        void setRows(int rows)
        { m_rows = rows; };

        int getRows() const
        { return m_rows; };

        void setCols(int cols)
        { m_cols = cols; };

        int getCols() const
        { return m_cols; };

        void process(const CloudXYZI &input);

        CloudXYZI getData()const;

        std::vector<double> toHeightArray();
        std::vector<double> toIntensityArray();
        std::vector<double> toHeightDiffArray();


        template <typename T>
        double getOriginX(T x)
        {
            return ((double)x-m_cols/2.0)*m_width;
        }

        template <typename T>
        double getOriginY(T y)
        {
            return ((double)y-m_rows/2.0)*m_height;
        }

        template <typename T>
        Eigen::Vector2d getOriginCoor(T x,T y)
        {
            double x_ori = getOriginX(x);
            double y_ori = getOriginY(y);
            Eigen::Vector2d coor_ori(x_ori, y_ori);
            return coor_ori;
        }

    private:

        double m_width;
        double m_height;

        int m_rows;
        int m_cols;

        std::vector<CloudXYZI, Eigen::aligned_allocator<CloudXYZI>> m_data;

    };


}




#endif //PROJECT_GRID_H
