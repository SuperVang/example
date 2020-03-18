#include <iostream>
#include <gtest/gtest.h>

#include <gdal.h>
#include <gdal_priv.h>
#include <ogr_api.h>
#include <ogr_spatialref.h>
#include <ogr_feature.h>
#include <ogrsf_frmts.h>

TEST(OGR_DATASET,shp_capability)
{
    GDALAllRegister();

    std::string shpName="../../gdal/data/shapefile/asm.shp";
    GDALDatasetH shpDSH=GDALOpenEx(shpName.c_str(),GDAL_OF_VECTOR|GDAL_OF_UPDATE,nullptr,nullptr,nullptr);

    EXPECT_TRUE(shpDSH!=nullptr);

    GDALDataset *shpDS=(GDALDataset*)shpDSH;
    EXPECT_EQ(1,shpDS->GetLayerCount());

    EXPECT_TRUE(shpDS->TestCapability(ODsCCreateLayer));
    EXPECT_TRUE(shpDS->TestCapability(ODsCDeleteLayer));
    EXPECT_FALSE(shpDS->TestCapability(ODsCCreateGeomFieldAfterCreateLayer));
    EXPECT_FALSE(shpDS->TestCapability(ODsCCurveGeometries));
    EXPECT_FALSE(shpDS->TestCapability(ODsCTransactions));
    EXPECT_FALSE(shpDS->TestCapability(ODsCEmulatedTransactions));
    EXPECT_TRUE(shpDS->TestCapability(ODsCMeasuredGeometries));
    EXPECT_FALSE(shpDS->TestCapability(ODsCRandomLayerRead));
    EXPECT_TRUE(shpDS->TestCapability(ODsCRandomLayerWrite));

    EXPECT_EQ(OGRERR_NONE,shpDS->Release());
 
    GDALDestroyDriverManager();
}

TEST(OGR_DATASET,mapinfo_capability)
{
    GDALAllRegister();

    std::string mifName="../../gdal/data/mapinfo/all_geoms.mif";
    GDALDatasetH mifDSH=GDALOpenEx(mifName.c_str(),GDAL_OF_ALL,nullptr,nullptr,nullptr);

    GDALDataset *mifDS=(GDALDataset*)mifDSH;

    EXPECT_EQ(1,mifDS->GetLayerCount());

    EXPECT_FALSE(mifDS->TestCapability(ODsCCreateLayer));
    EXPECT_FALSE(mifDS->TestCapability(ODsCDeleteLayer));
    EXPECT_FALSE(mifDS->TestCapability(ODsCCreateGeomFieldAfterCreateLayer));
    EXPECT_FALSE(mifDS->TestCapability(ODsCCurveGeometries));
    EXPECT_FALSE(mifDS->TestCapability(ODsCTransactions));
    EXPECT_FALSE(mifDS->TestCapability(ODsCEmulatedTransactions));
    EXPECT_FALSE(mifDS->TestCapability(ODsCMeasuredGeometries));
    EXPECT_FALSE(mifDS->TestCapability(ODsCRandomLayerRead));
    EXPECT_FALSE(mifDS->TestCapability(ODsCRandomLayerWrite));

    EXPECT_EQ(OGRERR_NONE,mifDS->Release());

    GDALDestroyDriverManager();
}

TEST(OGR_DATASET,csv_capability)
{
    GDALAllRegister();
    
    std::string csvName="../../gdal/data/csv/all_geoms.mif.golden.csv";

    GDALDatasetH csvDSH=GDALOpenEx(csvName.c_str(),GDAL_OF_VECTOR|GDAL_OF_UPDATE,nullptr,nullptr,nullptr);
    EXPECT_TRUE(csvDSH!=nullptr);

    GDALDataset *csvDS=(GDALDataset*)csvDSH;
    EXPECT_EQ(1,csvDS->GetLayerCount());

    EXPECT_TRUE(csvDS->TestCapability(ODsCCreateLayer));
    EXPECT_TRUE(csvDS->TestCapability(ODsCDeleteLayer));
    EXPECT_FALSE(csvDS->TestCapability(ODsCCreateGeomFieldAfterCreateLayer));
    EXPECT_TRUE(csvDS->TestCapability(ODsCCurveGeometries));
    EXPECT_FALSE(csvDS->TestCapability(ODsCTransactions));
    EXPECT_FALSE(csvDS->TestCapability(ODsCEmulatedTransactions));
    EXPECT_TRUE(csvDS->TestCapability(ODsCMeasuredGeometries));
    EXPECT_FALSE(csvDS->TestCapability(ODsCRandomLayerRead));
    EXPECT_TRUE(csvDS->TestCapability(ODsCRandomLayerWrite));

    EXPECT_EQ(OGRERR_NONE,csvDS->Release());

    GDALDestroyDriverManager();
}

TEST(OGR_DATASET,query_layer)
{
    GDALAllRegister();

    std::string mifName="../../gdal/data/mapinfo/all_geoms.mif";
    GDALDatasetH mifDSH=GDALOpenEx(mifName.c_str(),GDAL_OF_VECTOR,nullptr,nullptr,nullptr);

    EXPECT_TRUE(mifDSH!=nullptr);
    GDALDataset *mifDS=(GDALDataset*)mifDSH;

    EXPECT_EQ(1,mifDS->GetLayerCount());
    EXPECT_STREQ("all_geoms",mifDS->GetLayer(0)->GetName());

    EXPECT_EQ(OGRERR_NONE,mifDS->Release());

    GDALDestroyDriverManager();
}

TEST(OGR_DATASET,create_layer)
{
    GDALAllRegister();

    std::string shpName="../../gdal/data/shapefile/asm.shp";
    GDALDatasetH shpDSH=GDALOpenEx(shpName.c_str(),GDAL_OF_VECTOR|GDAL_OF_UPDATE,nullptr,nullptr,nullptr);

    GDALDataset *shpDS=(GDALDataset*)shpDSH;
    OGRLayer *shpLayer=shpDS->CreateLayer("TestLayer",nullptr);
    EXPECT_EQ(2,shpDS->GetLayerCount());
    EXPECT_TRUE(shpLayer!=nullptr);

    EXPECT_EQ(OGRERR_NONE,shpDS->Release());

    GDALDestroyDriverManager();
}

TEST(OGR_DATASET,delete_layer)
{
    GDALAllRegister();

    std::string shpName="../../gdal/data/shapefile/TestLayer.shp";
    GDALDatasetH shpDSH=GDALOpenEx(shpName.c_str(),GDAL_OF_VECTOR|GDAL_OF_UPDATE,nullptr,nullptr,nullptr);

    GDALDataset *shpDS=(GDALDataset*)shpDSH;
    EXPECT_EQ(1,shpDS->GetLayerCount());
    EXPECT_STREQ("TestLayer",shpDS->GetLayer(0)->GetName());
    EXPECT_EQ(OGRERR_NONE,shpDS->DeleteLayer(0));
    EXPECT_EQ(0,shpDS->GetLayerCount());

    EXPECT_EQ(OGRERR_NONE,shpDS->Release());

    GDALDestroyDriverManager();
}

TEST(OGR_DATASET,style_table)
{
    GDALAllRegister();

    std::string mifName="../../gdal/data/mapinfo/all_geoms.mif";
    GDALDatasetH mifDSH=GDALOpenEx(mifName.c_str(),GDAL_OF_VECTOR,nullptr,nullptr,nullptr);

    GDALDataset *mifDS=(GDALDataset*)mifDSH;
    OGRStyleTable *mifStyleTable=mifDS->GetStyleTable();

    EXPECT_EQ(OGRERR_NONE,mifDS->Release());

    GDALDestroyDriverManager();
}

TEST(OGR_DATASET,execute_sql)
{
    GDALAllRegister();

    std::string mifName="../../gdal/data/mapinfo/all_geoms.mif";
    GDALDatasetH mifDSH=GDALOpenEx(mifName.c_str(),GDAL_OF_VECTOR,nullptr,nullptr,nullptr);

    GDALDataset *mifDS=(GDALDataset*)mifDSH;
    // OGRLayer *mifLayer=mifDS->ExecuteSQL("",nullptr,nullptr);

    EXPECT_EQ(OGRERR_NONE,mifDS->Release());
    GDALDestroyDriverManager();
}