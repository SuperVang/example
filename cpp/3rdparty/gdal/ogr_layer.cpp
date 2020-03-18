#include <iostream>
#include <gtest/gtest.h>

#include <gdal.h>
#include <gdal_priv.h>
#include <ogr_api.h>
#include <ogr_spatialref.h>
#include <ogr_feature.h>
#include <ogrsf_frmts.h>

TEST(OGR_LAYER, layer_name)
{
    GDALAllRegister();

    std::string mifName = "../../gdal/data/mapinfo/all_geoms.mif";
    GDALDatasetH mifDSH = GDALOpenEx(mifName.c_str(), GDAL_OF_ALL, nullptr, nullptr, nullptr);
    GDALDataset *mifDS = (GDALDataset *)mifDSH;
    OGRLayer *mifLayer = mifDS->GetLayer(0);

    EXPECT_STREQ("all_geoms", mifLayer->GetName());

    EXPECT_EQ(OGRERR_NONE, mifDS->Release());
    GDALDestroyDriverManager();
}

TEST(OGR_LAYER, spatial_filter)
{
    GDALAllRegister();

    std::string mifName = "../../gdal/data/mapinfo/all_geoms.mif";
    GDALDatasetH mifDSH = GDALOpenEx(mifName.c_str(), GDAL_OF_ALL, nullptr, nullptr, nullptr);
    GDALDataset *mifDS = (GDALDataset *)mifDSH;
    OGRLayer *mifLayer = mifDS->GetLayer(0);

    OGRGeometry *mifSptialGeo = mifLayer->GetSpatialFilter();
    EXPECT_FALSE(mifSptialGeo != nullptr);

    mifLayer->SetSpatialFilterRect(0, 0, 100, 100);
    mifSptialGeo = mifLayer->GetSpatialFilter();
    EXPECT_TRUE(mifSptialGeo != nullptr);

    mifLayer->SetSpatialFilter(nullptr);
    mifSptialGeo = mifLayer->GetSpatialFilter();
    EXPECT_FALSE(mifSptialGeo != nullptr);

    EXPECT_EQ(OGRERR_NONE, mifDS->Release());
    GDALDestroyDriverManager();
}

TEST(OGR_LAYER, attr_filter)
{
    GDALAllRegister();

    std::string mifName = "../../gdal/data/mapinfo/all_geoms.mif";
    GDALDatasetH mifDSH = GDALOpenEx(mifName.c_str(), GDAL_OF_ALL, nullptr, nullptr, nullptr);
    GDALDataset *mifDS = (GDALDataset *)mifDSH;

    OGRLayer *mifLayer = mifDS->GetLayer(0);
    EXPECT_EQ(OGRERR_NONE, mifLayer->SetAttributeFilter(""));
    // EXPECT_STREQ("",mifLayer->GetAttrQueryString());

    EXPECT_EQ(OGRERR_NONE, mifDS->Release());
    GDALDestroyDriverManager();
}

TEST(OGR_LAYER, read_feature)
{
    GDALAllRegister();

    std::string mifName = "../../gdal/data/mapinfo/all_geoms.mif";
    GDALDatasetH mifDSH = GDALOpenEx(mifName.c_str(), GDAL_OF_ALL, nullptr, nullptr, nullptr);
    GDALDataset *mifDS = (GDALDataset *)mifDSH;
    OGRLayer *mifLayer = mifDS->GetLayer(0);

    mifLayer->ResetReading();
    int mifFeatureCount = mifLayer->GetFeatureCount();
    EXPECT_EQ(19, mifLayer->GetFeatureCount());
    OGRFeature *mifFeature = nullptr;
    while (mifFeature = mifLayer->GetNextFeature())
    {
        std::cout << mifFeature->GetFID() << "\t";
    }
    std::cout << std::endl;

    mifLayer->SetNextByIndex(3);
    mifFeature = mifLayer->GetNextFeature();
    EXPECT_EQ(4, mifFeature->GetFID());
    mifLayer->ResetReading();

    for (int i = 1; i <= 16; i++)
    {
        mifFeature = mifLayer->GetFeature(i);
        std::cout << mifFeature->GetFID() << "\t";
    }
    std::cout << std::endl;

    OGRFeature::DestroyFeature(mifFeature);
    EXPECT_EQ(OGRERR_NONE, mifDS->Release());
    GDALDestroyDriverManager();
}

TEST(OGR_LAYER, update_feature)
{
}