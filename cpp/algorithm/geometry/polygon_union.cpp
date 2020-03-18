//
// Created by books on 19-9-5.
//

#include <iostream>
#include <gtest/gtest.h>

#include <gdal.h>
#include <gdal_priv.h>
#include <ogr_api.h>
#include <ogr_spatialref.h>
#include <ogr_feature.h>
#include <ogrsf_frmts.h>
#include <ogr_geometry.h>

#include <opencv2/opencv.hpp>

void showPolygon(OGRPolygon *pg, std::string name)
{
    OGRLinearRing *ring = pg->getExteriorRing();

    std::vector<cv::Point2d> pts;
    std::cout << ring->getNumPoints() << std::endl;
    for (int i = 0; i < ring->getNumPoints(); i++)
    {
        double x = ring->getX(i);
        double y = ring->getY(i);
        cv::Point2d p(10.0 * x, 10.0 * y);
        pts.push_back(p);
    }

    cv::Mat img(cv::Size(500, 500), CV_8UC1);
    img.setTo(255);
    for (int i = 0; i < pts.size(); i++)
    {
        cv::Point2d p1 = pts[i];
        cv::Point2d p2 = pts[(i + 1) % pts.size()];
        cv::line(img, p1, p2, cv::Scalar(0), 2);
    }
    cv::imshow(name, img);
}

void showPolygon(OGRPolygon *pg1, OGRPolygon *pg2, OGRPolygon *pg3, std::string name)
{
    cv::Mat img(cv::Size(500, 500), CV_8UC1);
    img.setTo(255);
    {
        std::vector<cv::Point2d> pts;
        OGRLinearRing *ring = pg1->getExteriorRing();
        for (int i = 0; i < ring->getNumPoints(); i++)
        {
            double x = ring->getX(i);
            double y = ring->getY(i);
            cv::Point2d p(10.0 * x, 10.0 * y);
            pts.push_back(p);
        }
        for (int i = 0; i < pts.size(); i++)
        {
            cv::Point2d p1 = pts[i];
            cv::Point2d p2 = pts[(i + 1) % pts.size()];
            cv::line(img, p1, p2, cv::Scalar(0), 2);
        }
    }
    {
        std::vector<cv::Point2d> pts;
        OGRLinearRing *ring = pg2->getExteriorRing();
        for (int i = 0; i < ring->getNumPoints(); i++)
        {
            double x = ring->getX(i);
            double y = ring->getY(i);
            cv::Point2d p(10.0 * x, 10.0 * y);
            pts.push_back(p);
        }
        for (int i = 0; i < pts.size(); i++)
        {
            cv::Point2d p1 = pts[i];
            cv::Point2d p2 = pts[(i + 1) % pts.size()];
            cv::line(img, p1, p2, cv::Scalar(0), 2);
        }
    }
    {
        std::vector<cv::Point2d> pts;
        OGRLinearRing *ring = pg3->getExteriorRing();
        for (int i = 0; i < ring->getNumPoints(); i++)
        {
            double x = ring->getX(i);
            double y = ring->getY(i);
            cv::Point2d p(10.0 * x, 10.0 * y);
            pts.push_back(p);
        }
        for (int i = 0; i < pts.size(); i++)
        {
            cv::Point2d p1 = pts[i];
            cv::Point2d p2 = pts[(i + 1) % pts.size()];
            cv::line(img, p1, p2, cv::Scalar(0), 2);
        }
    }
    cv::imshow(name, img);
}

TEST(ALGORITHM_GEOMETRY, polygon_from_wkt)
{
    {
        OGRPolygon *polygon = new OGRPolygon();
        char *ringStr =(char*) "POLYGON ((38.017377 35.91519409,36.1811004 40.58078144,33.1129999 48.31658678,34.7566278 34.61434749,33.0365826 38.97892604,29.8291134 47.01570223,38.017377 35.91519409))";
        polygon->importFromWkt(&ringStr);

        std::cout << "WKT1 Ring ? : " << polygon->IsRing() << std::endl;
        std::cout << "WKT1 Simple ? :" << polygon->IsSimple() << std::endl;
        std::cout << "WKT1 Valid ? :" << polygon->IsValid() << std::endl;

        OGRLinearRing *ring=polygon->getExteriorRing();
        std::cout << "Ring1 Ring ? : " << ring->IsRing() << std::endl;
        std::cout << "Ring1 Simple ? :" << ring->IsSimple() << std::endl;
        std::cout << "Ring1 Valid ? :" << ring->IsValid() << std::endl;


        char *output = nullptr;
        polygon->exportToWkt(&output);
        std::cout << output << std::endl;

        ring->exportToWkt(&output);
        std::cout<<output<<std::endl;

        showPolygon(polygon, "WKT1");
    }

    {
        OGRPolygon *polygon = new OGRPolygon();
        char *ringStr =(char *) "POLYGON ((33.1129999 48.31658678,33.1165424 46.26836082,33.0719781 45.27428876,32.9581448 44.18973372,32.5903774 42.74328679,32.1757012 41.65821147,31.3914861 40.15080228,30.2837547 38.37174421,28.8661701 41.80309388,29.1194795 42.52643729,29.326297 43.37018456,29.4867268 44.27409375,29.601498 44.81647128,29.6697775 45.47925266,29.7384214 45.93118725,29.8291134 47.01570223,33.1129999 48.31658678))";
        polygon->importFromWkt(&ringStr);

        std::cout << "WKT2 Ring ? : " << polygon->IsRing() << std::endl;
        std::cout << "WKT2 Simple ? :" << polygon->IsSimple() << std::endl;
        std::cout << "WKT2 Valid ? :" << polygon->IsValid() << std::endl;

        OGRLinearRing *ring=polygon->getExteriorRing();
        std::cout << "Ring2 Ring ? : " << ring->IsRing() << std::endl;
        std::cout << "Ring2 Simple ? :" << ring->IsSimple() << std::endl;
        std::cout << "Ring2 Valid ? :" << ring->IsValid() << std::endl;


        char *output = nullptr;
        polygon->exportToWkt(&output);
        std::cout << output << std::endl;

        ring->exportToWkt(&output);
        std::cout<<output<<std::endl;

        showPolygon(polygon, "WKT2");
    }

    cv::waitKey(0);
}

TEST(ALGORITHM_GEOMETRY, polygon_intersection)
{
    OGRPolygon *pg1 = new OGRPolygon();
    {
        OGRLinearRing *ring = new OGRLinearRing();
        ring->addPoint(10, 12);
        ring->addPoint(12, 10);
        ring->addPoint(32, 30);
        ring->addPoint(30, 32);
        ring->addPoint(10, 12);
        OGRBoolean bl = ring->IsRing();
        pg1->addRing(ring);
    }
    showPolygon(pg1, "pg1");

    OGRPolygon *pg2 = new OGRPolygon();
    {
        OGRLinearRing *ring = new OGRLinearRing();
        ring->addPoint(30, 10);
        ring->addPoint(32, 12);
        ring->addPoint(12, 32);
        ring->addPoint(10, 30);
        ring->addPoint(30, 10);
        pg2->addRing(ring);
    }
    showPolygon(pg2, "pg2");

    OGRPolygon *pg3 = new OGRPolygon();
    {
        OGRLinearRing *ring = new OGRLinearRing();
        ring->addPoint(10, 25);
        ring->addPoint(32, 25);
        ring->addPoint(32, 27);
        ring->addPoint(10, 27);
        ring->addPoint(10, 25);
        pg3->addRing(ring);
    }
    showPolygon(pg3, "pg3");

    OGRGeometry *tmpGeo = pg1->Union(pg2);
    OGRGeometry *geo = tmpGeo->Union(pg3);
    std::cout << "Union Geometry Type : " << geo->getGeometryName() << std::endl;
    char *geoWKT = nullptr;
    geo->exportToWkt(&geoWKT);
    std::cout << "Union Geometry WKT : " << geoWKT << std::endl;

    OGRPolygon *unionPg = (OGRPolygon *) geo;

    showPolygon(pg1, pg2, pg3, "overlap");
    showPolygon(unionPg, "union");
    cv::waitKey(0);

}