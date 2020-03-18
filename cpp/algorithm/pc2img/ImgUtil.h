//
// Created by books on 19-3-13.
//

#ifndef PROJECT_IMGUTIL_H
#define PROJECT_IMGUTIL_H

#include <iostream>
#include <vector>
#include <algorithm>

#include <cmath>
#include <opencv2/opencv.hpp>

namespace VOLib{

    static const double ERRORVAL=-100000000000;

    static std::vector<double> normalize(const std::vector<double> &vec)
    {
        double min_val=10000000,max_val=-100000000;
        for(const double &val : vec)
        {
            if(val==ERRORVAL)
                continue;

            min_val=min_val<val?min_val:val;
            max_val=max_val>val?max_val:val;
        }

        std::vector<double> result(vec.size(),0);
        for(int i=0;i<vec.size();i++)
        {
            if(vec[i]==ERRORVAL)
                continue;

            result[i]=(vec[i]-min_val)/(max_val-min_val);
        }
        return result;
    }

    static std::vector<uchar> double2Gray(const std::vector<double> &vec)
    {
        std::vector<uchar> result;
        for(const double &val : vec)
            result.push_back(cv::saturate_cast<uchar>(val*255));
        return result;
    }

}


#endif //PROJECT_IMGUTIL_H
