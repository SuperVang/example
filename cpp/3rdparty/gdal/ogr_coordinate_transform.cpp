#include <iostream>
#include <gtest/gtest.h>

#include <gdal.h>
#include <gdal_priv.h>
#include <ogr_spatialref.h>

TEST(GDAL_OGR,coordinate_transform)
{
    OGRSpatialReference srsSource,srsTarget;
    EXPECT_EQ(OGRERR_NONE,srsSource.SetWellKnownGeogCS("WGS84"));
    EXPECT_EQ(OGRERR_NONE,srsSource.SetUTM(17));
    EXPECT_EQ(OGRERR_NONE,srsTarget.SetWellKnownGeogCS("WGS84"));
    
    OGRCoordinateTransformation *ct=OGRCreateCoordinateTransformation(&srsSource,&srsTarget);
    EXPECT_TRUE(ct!=NULL);

    double x=10000.0,y=10000.0;
    EXPECT_TRUE(ct->Transform(1,&x,&y));

    OGRCoordinateTransformation::DestroyCT(ct);
}