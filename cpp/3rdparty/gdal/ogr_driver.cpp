#include <iostream>
#include <gtest/gtest.h>

#include <gdal.h>
#include <gdal_priv.h>
#include <ogr_api.h>
#include <ogr_spatialref.h>
#include <ogr_feature.h>
#include <ogrsf_frmts.h>

TEST(OGR_DRIVER,driver_manager)
{
    GDALAllRegister();
    GDALDriverManager *driverManager=GetGDALDriverManager();
    const char *driverManagerDescription=driverManager->GetDescription();
    std::cout<<driverManagerDescription<<std::endl;
    int driverCount=driverManager->GetDriverCount();
    for(int i=0;i<driverCount;i++)
    {
        GDALDriver *driver=driverManager->GetDriver(i);
        const char *driverDescription=driver->GetDescription();
        std::cout<<driverDescription<<"\t";
    }
    std::cout<<std::endl;
    GDALDestroyDriverManager();
}

TEST(OGR_DRIVER,driver_shp)
{
    GDALAllRegister();

    std::string shpName="../../gdal/data/shapefile/asm.shp";
    GDALDriverH shpDriverH=nullptr;
    GDALDatasetH shpDSH1=OGROpen(shpName.c_str(),FALSE,&shpDriverH);
    GDALDatasetH shpDSH2=GDALOpenEx(shpName.c_str(),GDAL_OF_VECTOR|GDAL_OF_UPDATE,nullptr,nullptr,nullptr);

    EXPECT_TRUE(shpDSH1!=nullptr);
    EXPECT_TRUE(shpDriverH!=nullptr);
    EXPECT_TRUE(shpDSH2!=nullptr);

    GDALDriver *shpDriver=(GDALDriver*)shpDriverH;
    GDALDataset *shpDS1=(GDALDataset*)shpDSH1;
    GDALDataset *shpDS2=(GDALDataset*)shpDSH2;
    EXPECT_STREQ("ESRI Shapefile",shpDriver->GetDescription());


    EXPECT_EQ(OGRERR_NONE,shpDS1->Release());
    EXPECT_EQ(OGRERR_NONE,shpDS2->Release());
 
    GDALDestroyDriverManager();
}

TEST(OGR_DRIVER,driver_mapinfo)
{
    GDALAllRegister();

    std::string mifName="../../gdal/data/mapinfo/all_geoms.mif";
    GDALDriverH mifDriverH=nullptr;
    GDALDatasetH mifDSH1=OGROpen(mifName.c_str(),FALSE,&mifDriverH);
    GDALDatasetH mifDSH2=GDALOpenEx(mifName.c_str(),GDAL_OF_ALL,nullptr,nullptr,nullptr);

    EXPECT_TRUE(mifDSH1!=nullptr);
    EXPECT_TRUE(mifDriverH!=nullptr);
    
    GDALDriver *mifDriver=(GDALDriver*)mifDriverH;
    GDALDataset *mifDS1=(GDALDataset*)mifDSH1;
    GDALDataset *mifDS2=(GDALDataset*)mifDSH2;

    EXPECT_STREQ("MapInfo File",mifDriver->GetDescription());

    EXPECT_EQ(OGRERR_NONE,mifDS1->Release());
    EXPECT_EQ(OGRERR_NONE,mifDS2->Release());

    GDALDestroyDriverManager();
}

TEST(OGR_DRIVER,driver_csv)
{
    GDALAllRegister();
    
    std::string csvName="../../gdal/data/csv/all_geoms.mif.golden.csv";
    GDALDriverH csvDriverH=nullptr;
    GDALDatasetH csvDSH1=OGROpen(csvName.c_str(),FALSE,&csvDriverH);
    GDALDatasetH csvDSH2=GDALOpenEx(csvName.c_str(),GDAL_OF_VECTOR|GDAL_OF_UPDATE,nullptr,nullptr,nullptr);

    EXPECT_TRUE(csvDSH1!=nullptr);
    EXPECT_TRUE(csvDSH2!=nullptr);
    EXPECT_TRUE(csvDriverH!=nullptr);

    GDALDataset *csvDS1=(GDALDataset*)csvDSH1;
    GDALDataset *csvDS2=(GDALDataset*)csvDSH2;
    GDALDriver *csvDriver=(GDALDriver*)csvDriverH;

    EXPECT_STREQ("CSV",csvDriver->GetDescription());

    EXPECT_EQ(OGRERR_NONE,csvDS1->Release());
    EXPECT_EQ(OGRERR_NONE,csvDS2->Release());

    GDALDestroyDriverManager();
}
