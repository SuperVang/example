#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

#include <gtest/gtest.h>

#include <opencv2/opencv.hpp>

typedef std::vector<cv::Point2d> PtVec;

PtVec initRandomPts()
{
    static std::default_random_engine e;
    e.seed(std::time(nullptr));
    std::normal_distribution<double> normalDist(200, 50);
    PtVec ptVec;
    for (int i = 0; i < 100; i++)
        ptVec.emplace_back(normalDist(e), normalDist(e));
    return ptVec;
}

void initDisplay(const PtVec &ptVec, std::vector<int> indexVec = std::vector<int>())
{
    cv::Mat img(cv::Size(400, 400), CV_8UC1);
    img.setTo(255);
    for (size_t i = 0; i < ptVec.size(); i++)
    {
        cv::Point2d pt = ptVec[i];
        cv::circle(img, pt, 1, cv::Scalar(0), -1);
    }
    if (!indexVec.empty())
    {
        for (int i = 0; i < indexVec.size(); i++)
        {
            cv::Point2d ptS = ptVec[indexVec[i]];
            cv::Point2d ptE = ptVec[indexVec[(i + 1) % indexVec.size()]];
            cv::line(img, ptS, ptE, cv::Scalar(0), 2);
        }
    }
    std::string winName = indexVec.empty() ? "PtVec" : "ConvexHull";
    cv::imshow(winName, img);
    cv::waitKey(0);
    cv::destroyWindow(winName);
}

TEST(ALGORITHM_GEOMETRY, init_random_pts)
{
    PtVec ptVec = initRandomPts();
    initDisplay(ptVec);
}

TEST(ALGORITHM_GEOMETRY, convex_hull)
{
    PtVec ptVec = initRandomPts();

    double ymin = 10000000;
    int firstIndex = -1;
    for (size_t i = 0; i < ptVec.size(); i++)
    {
        if (ptVec[i].y < ymin)
        {
            ymin = ptVec[i].y;
            firstIndex = i;
        }
    }
    ASSERT_TRUE(firstIndex != -1);

    int secondIndex = -1;
    double minHorizpntalAngle = 1000000;
    for (int i = 0; i < ptVec.size(); i++)
    {
        if (i == firstIndex || ptVec[i] == ptVec[firstIndex])
            continue;

        double tmpDx = std::abs(ptVec[i].x - ptVec[firstIndex].x);
        double tmpDy = std::abs(ptVec[i].y - ptVec[firstIndex].y);
        double tmpDis = std::sqrt(tmpDx * tmpDx + tmpDy * tmpDy);
        double tmpHorizontalAngle = std::acos(tmpDx / tmpDis);

        if (tmpHorizontalAngle < minHorizpntalAngle)
        {
            minHorizpntalAngle = tmpHorizontalAngle;
            secondIndex = i;
        }
    }
    std::cout << "Min Horizaontal Angle = " << minHorizpntalAngle << std::endl;

    ASSERT_TRUE(secondIndex != -1);

    std::vector<int> indexVec = {firstIndex, secondIndex};
    initDisplay(ptVec, indexVec);

    std::vector<int> flagVec(ptVec.size(), 0);
    // flagVec[firstIndex] = 1;
    flagVec[secondIndex] = 1;
    int nxtIndex = -1;
    while (nxtIndex != firstIndex)
    {
        double maxAngle = -1000000;
        double maxAngleIndex = -1;
        cv::Point2d A = ptVec[indexVec[indexVec.size() - 2]];
        cv::Point2d B = ptVec[indexVec[indexVec.size() - 1]];
        for (int i = 0; i < ptVec.size(); i++)
        {
            if (flagVec[i] == 1)
                continue;
            cv::Point2d C = ptVec[i];
            double xBA = A.x - B.x;
            double yBA = A.y - B.y;
            double xBC = C.x - B.x;
            double yBC = C.y - B.y;
            double xAC = C.x - A.x;
            double yAC = C.y - A.y;
            double lBA = std::sqrt(xBA * xBA + yBA * yBA);
            double lBC = std::sqrt(xBC * xBC + yBC * yBC);
            double lAC = std::sqrt(xAC * xAC + yAC * yAC);
            double tmpCosAngle = (lBC * lBC + lBA * lBA - lAC * lAC) / (2 * lBA * lBC);
            double tmpAngle = std::acos(tmpCosAngle);
            if (tmpAngle > maxAngle)
            {
                maxAngle = tmpAngle;
                maxAngleIndex = i;
            }
        }
        ASSERT_TRUE(maxAngleIndex != -1);
        nxtIndex = maxAngleIndex;
        indexVec.push_back(nxtIndex);
        flagVec[nxtIndex] = 1;

        std::cout << "Max Angle = " << maxAngle << std::endl;
        initDisplay(ptVec, indexVec);
    }

    for (const int &index : indexVec)
        std::cout << index << "\t";
    std::cout << std::endl;

    initDisplay(ptVec, indexVec);
}