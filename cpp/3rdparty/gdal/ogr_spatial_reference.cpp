#include <iostream>
#include <gtest/gtest.h>

#include <gdal.h>
#include <gdal_priv.h>
#include <ogr_spatialref.h>

TEST(GDAL_OGR,geographic_spatial_reference)
{
    OGRSpatialReference srs1;
    EXPECT_EQ(OGRERR_NONE,srs1.SetGeogCS("Mygeographic coordinate system",
    "WGS_1984","My WGS84 Spheroid",SRS_WGS84_SEMIMAJOR,SRS_WGS84_INVFLATTENING,
    "Greenwich",0.0,"degree",std::atof(SRS_UA_DEGREE_CONV)));
    char *wkt1=NULL;
    EXPECT_EQ(OGRERR_NONE,srs1.exportToPrettyWkt(&wkt1));
    printf("%s\n\n",wkt1);

    OGRSpatialReference srs2;
    EXPECT_EQ(OGRERR_NONE,srs2.SetWellKnownGeogCS("WGS84"));
    char *wkt2=NULL;
    EXPECT_EQ(OGRERR_NONE,srs2.exportToPrettyWkt(&wkt2));
    printf("%s\n\n",wkt2);

    OGRSpatialReference srs3;
    EXPECT_EQ(OGRERR_NONE,srs3.SetWellKnownGeogCS("EPSG:4326"));
    char *wkt3=NULL;
    EXPECT_EQ(OGRERR_NONE,srs3.exportToPrettyWkt(&wkt3));
    printf("%s\n\n",wkt3);

    OGRSpatialReference srs4(wkt2);
    OGRSpatialReference *srs5=srs2.Clone();

    EXPECT_TRUE(srs1.IsGeographic());
    EXPECT_TRUE(srs2.IsGeographic());
    EXPECT_TRUE(srs3.IsGeographic());
    EXPECT_TRUE(srs2.IsSame(&srs4));
    EXPECT_TRUE(srs2.IsSame(srs5));

    CPLFree(wkt1);
    CPLFree(wkt2);
    CPLFree(wkt3);
}

TEST(GDAL_OGR,projected_spatial_reference)
{
    OGRSpatialReference srsUTM;
    EXPECT_EQ(OGRERR_NONE,srsUTM.SetProjCS("UTM 17(WGS84) on northern hemisphere.")); 
    EXPECT_EQ(OGRERR_NONE,srsUTM.SetWellKnownGeogCS("WGS84"));
    EXPECT_EQ(OGRERR_NONE,srsUTM.SetUTM(17));
    char *wktUTM=NULL;
    EXPECT_EQ(OGRERR_NONE,srsUTM.exportToPrettyWkt(&wktUTM));
    printf("%s\n\n",wktUTM);
    EXPECT_TRUE(srsUTM.IsProjected());
    CPLFree(wktUTM);

    OGRSpatialReference srsTM;
    EXPECT_EQ(OGRERR_NONE,srsTM.SetProjCS("TM Self."));
    EXPECT_EQ(OGRERR_NONE,srsTM.SetWellKnownGeogCS("WGS84"));
    EXPECT_EQ(OGRERR_NONE,srsTM.SetTM(0,0,0,0,0));
    char *wktTM=NULL;
    EXPECT_EQ(OGRERR_NONE,srsTM.exportToPrettyWkt(&wktTM));
    printf("%s\n\n",wktTM);
    EXPECT_TRUE(srsTM.IsProjected());
    CPLFree(wktTM);
}

TEST(GDAL_OGR,parse_spatial_reference)
{
    OGRSpatialReference srs;
    EXPECT_EQ(OGRERR_NONE,srs.SetWellKnownGeogCS("WGS84"));
    EXPECT_EQ(OGRERR_NONE,srs.SetUTM(17));
    double semiMajor=srs.GetSemiMajor();
    double semiMinor=srs.GetSemiMinor();
    double invFlattening=srs.GetInvFlattening();
    const char *projCS=srs.GetAttrValue("PROJCS");
    const char *geogCS=srs.GetAttrValue("GEOGCS");
    const char *datum=srs.GetAttrValue("DATUM");
    const char *spherOID=srs.GetAttrValue("SPHEROID");
    const char *projection=srs.GetAttrValue("PROJECTION");
    double centralMeridian=srs.GetProjParm(SRS_PP_CENTRAL_MERIDIAN,0.0);
    double standardParallel=srs.GetProjParm(SRS_PP_STANDARD_PARALLEL_1,0.0);
    double falseEasting=srs.GetProjParm(SRS_PP_FALSE_EASTING,0.0);
    double falseNorthing=srs.GetProjParm(SRS_PP_FALSE_NORTHING,0.0);

    std::cout<<semiMajor<<"\t"<<semiMinor<<"\t"<<invFlattening<<std::endl;
    std::cout<<projCS<<std::endl;
    std::cout<<geogCS<<std::endl;
    std::cout<<datum<<std::endl;
    std::cout<<spherOID<<std::endl;
    std::cout<<projection<<std::endl;
    std::cout<<centralMeridian<<"\t"<<standardParallel<<"\t"<<falseEasting<<"\t"<<falseNorthing<<std::endl;
}

