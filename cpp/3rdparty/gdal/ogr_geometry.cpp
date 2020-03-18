#include <iostream>
#include <gtest/gtest.h>

#include <gdal.h>
#include <gdal_priv.h>
#include <ogr_api.h>
#include <ogr_spatialref.h>
#include <ogr_feature.h>
#include <ogrsf_frmts.h>
#include <ogr_geometry.h>

TEST(OGR_GEOMETRY, point_attribute)
{
    OGRPoint *pt = new OGRPoint();

    EXPECT_TRUE(pt->IsEmpty());
    EXPECT_TRUE(pt->IsSimple());
    EXPECT_TRUE(pt->IsValid());

    pt->setX(1);
    pt->setY(2);

    EXPECT_TRUE(pt->IsSimple());
    EXPECT_TRUE(pt->IsValid());
    EXPECT_FALSE(pt->IsEmpty());
}

TEST(OGR_GEOMETRY, polyline_attribute)
{
    OGRLineString *pl = new OGRLineString();
    EXPECT_TRUE(pl->IsEmpty());
    EXPECT_TRUE(pl->IsValid());
    EXPECT_TRUE(pl->IsSimple());
    EXPECT_FALSE(pl->IsRing());

    pl->addPoint(1, 1);
    // EXPECT_FALSE(pl->IsValid());

    pl->addPoint(2, 2);
    pl->addPoint(2, 2);
    EXPECT_TRUE(pl->IsValid());
    EXPECT_TRUE(pl->IsSimple());
    EXPECT_FALSE(pl->IsRing());

    pl->setPoint(2, 2, 1);
    pl->addPoint(1, 1);
    EXPECT_TRUE(pl->IsSimple());
    EXPECT_TRUE(pl->IsRing());

    pl->addPoint(0, 1);
    EXPECT_FALSE(pl->IsSimple());
    EXPECT_FALSE(pl->IsRing());
    EXPECT_TRUE(pl->IsValid());
}

TEST(OGR_GEOMETRY, polyline_close_ring)
{
    OGRLineString *pl = new OGRLineString();

    pl->addPoint(1, 1);
    pl->addPoint(2, 2);
    pl->addPoint(2, 1);
    // pl->addPoint(1, 1);

    pl->closeRings();

    EXPECT_EQ(pl->getX(2), 2);
    EXPECT_EQ(pl->getY(2), 1);

    EXPECT_STREQ("LINESTRING", pl->getGeometryName());

    EXPECT_EQ(3, pl->getNumPoints());
}

TEST(OGR_GEOMETRY, linearring_attribute)
{
    OGRLinearRing *lr = new OGRLinearRing();
    EXPECT_TRUE(lr->IsEmpty());
    EXPECT_FALSE(lr->IsValid());
    EXPECT_FALSE(lr->IsSimple());
    EXPECT_FALSE(lr->IsRing());

    lr->addPoint(1, 1);
    lr->addPoint(2, 2);
    lr->addPoint(2, 1);

    EXPECT_FALSE(lr->IsRing());
    EXPECT_FALSE(lr->IsValid());
    EXPECT_FALSE(lr->IsSimple());
    EXPECT_EQ(0, lr->get_IsClosed());

    lr->closeRings();

    EXPECT_EQ(4, lr->getNumPoints());
    EXPECT_EQ(lr->getX(3), 1);
    EXPECT_EQ(lr->getY(3), 1);

    EXPECT_FALSE(lr->IsRing());
    EXPECT_FALSE(lr->IsValid());
    EXPECT_FALSE(lr->IsSimple());
    EXPECT_EQ(1, lr->get_IsClosed());

    lr->addPoint(1, 1);

    EXPECT_FALSE(lr->IsRing());
    EXPECT_FALSE(lr->IsValid());
    EXPECT_FALSE(lr->IsSimple());

    lr->addPoint(1, 2);
    lr->addPoint(0, 2);
    EXPECT_EQ(0, lr->get_IsClosed());
    lr->addPoint(1, 1);
    EXPECT_EQ(1, lr->get_IsClosed());
}

TEST(OGR_GEOMETRY, polygon_attribute)
{
    OGRPolygon *pg = new OGRPolygon();
    EXPECT_TRUE(pg->IsEmpty());
    EXPECT_TRUE(pg->IsValid());
    EXPECT_TRUE(pg->IsSimple());
    EXPECT_FALSE(pg->IsRing());

    OGRLinearRing *lr = new OGRLinearRing();

    lr->addPoint(1, 1);
    lr->addPoint(2, 2);
    lr->addPoint(2, 1);
    lr->addPoint(1, 1);

    pg->addRing(lr);

    EXPECT_EQ(pg->getExteriorRing()->getNumPoints(), 4);

    EXPECT_FALSE(pg->IsEmpty());
    EXPECT_TRUE(pg->IsValid());
    EXPECT_TRUE(pg->IsSimple());
    EXPECT_FALSE(pg->IsRing());

    pg->empty();
    EXPECT_FALSE(pg->getExteriorRing());

    lr->addPoint(1, 2);
    lr->addPoint(0, 2);
    lr->addPoint(1, 1);
    EXPECT_EQ(lr->getNumPoints(), 7);
    pg->addRing(lr);

    EXPECT_FALSE(pg->IsEmpty());
    EXPECT_FALSE(pg->IsValid());
    EXPECT_TRUE(pg->IsSimple());
    EXPECT_FALSE(pg->IsRing());
    EXPECT_STREQ(pg->getGeometryName(), "POLYGON");
    EXPECT_TRUE(pg->getExteriorRing());
}

TEST(OGR_GEOMETRY, polygon_union_polygon)
{
    OGRPolygon *pg1 = new OGRPolygon();
    {
        OGRLinearRing *ring = new OGRLinearRing();
        ring->addPoint(10, 20);
        ring->addPoint(20, 20);
        ring->addPoint(20, 40);
        ring->addPoint(10, 40);
        ring->addPoint(10, 20);
        pg1->addRing(ring);
    }

    OGRPolygon *pg2 = new OGRPolygon();
    {
        OGRLinearRing *ring = new OGRLinearRing();
        ring->addPoint(15, 20);
        ring->addPoint(25, 20);
        ring->addPoint(25, 40);
        ring->addPoint(15, 40);
        ring->addPoint(15, 20);
        pg2->addRing(ring);
    }

    OGRGeometry *geo = pg1->Union(pg2);
    OGRPolygon *unionPg = (OGRPolygon *)geo;

    OGRGeometry *bound = unionPg->Boundary();
    OGRLinearRing *exRing = unionPg->getExteriorRing();

    std::cout << "Union Geometry Type : " << unionPg->getGeometryName() << std::endl;
    char *geoWKT = nullptr;
    exRing->exportToWkt(&geoWKT);

    std::cout << "Union Geometry WKT : " << geoWKT << std::endl;

    CPLFree(exRing);
}

TEST(OGR_GEOMETRY, polygon_intersect_polyline)
{

    OGRPolygon *pg1 = new OGRPolygon();
    {
        OGRLinearRing *ring = new OGRLinearRing();
        ring->addPoint(10, 20);
        ring->addPoint(20, 20);
        ring->addPoint(20, 40);
        ring->addPoint(10, 40);
        ring->addPoint(10, 20);
        pg1->addRing(ring);
    }

    OGRLineString *pl = new OGRLineString();
    pl->addPoint(15, 20);
    pl->addPoint(25, 20);
    pl->addPoint(25, 40);
    pl->addPoint(15, 40);

    EXPECT_TRUE(pl->Intersects(pg1));

    OGRGeometry *inter = pl->Intersection(pg1);
    EXPECT_EQ(inter->getGeometryType(), wkbMultiLineString);

    char *geoTxt = nullptr;
    inter->exportToWkt(&geoTxt);
    std::cout << geoTxt << std::endl;
}

TEST(OGR_GEOMETRY, polyline_intersect_polyline)
{
    OGRLineString *ring = new OGRLineString();
    ring->addPoint(10, 20);
    ring->addPoint(20, 20);
    ring->addPoint(20, 40);
    ring->addPoint(10, 40);
    ring->addPoint(10, 20);

    OGRLineString *pl = new OGRLineString();
    pl->addPoint(15, 20);
    pl->addPoint(25, 20);
    pl->addPoint(25, 40);
    pl->addPoint(15, 40);

    EXPECT_TRUE(pl->Intersects(ring));

    OGRGeometry *inter = pl->Intersection(ring);
    EXPECT_EQ(inter->getGeometryType(), wkbMultiLineString);

    char *geoTxt = nullptr;
    inter->exportToWkt(&geoTxt);
    std::cout << geoTxt << std::endl;
}

TEST(OGR_GEOMETRY, polyline_contain_point)
{
    OGRLineString *ls = new OGRLineString();
    ls->addPoint(1, 1);
    ls->addPoint(5, 5);

    OGRPoint *pt1 = new OGRPoint(0, 0);
    OGRPoint *pt2 = new OGRPoint(3, 3);
    OGRPoint *pt3 = new OGRPoint(2, 4);
    OGRPoint *pt4 = new OGRPoint(1, 1);

    EXPECT_FALSE(ls->Contains(pt1));
    EXPECT_TRUE(ls->Contains(pt2));
    EXPECT_FALSE(ls->Contains(pt3));
    EXPECT_FALSE(ls->Contains(pt4));
}

TEST(OGR_GEOMETRY, polyline_touch_point)
{
    OGRLineString *ls = new OGRLineString();
    ls->addPoint(1, 1);
    ls->addPoint(5, 5);

    OGRPoint *pt1 = new OGRPoint(0, 0);
    OGRPoint *pt2 = new OGRPoint(3, 3);
    OGRPoint *pt3 = new OGRPoint(2, 4);
    OGRPoint *pt4 = new OGRPoint(1, 1);

    EXPECT_FALSE(ls->Touches(pt1));
    EXPECT_FALSE(ls->Touches(pt2));
    EXPECT_FALSE(ls->Touches(pt3));
    EXPECT_TRUE(ls->Touches(pt4));
}