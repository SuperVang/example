#include <iostream>
#include <gtest/gtest.h>

#include <Eigen/Core>

TEST(EIGEN_MATRIX, init_const_matrix)
{
    Eigen::Matrix3d mat;
    mat.setIdentity();
}

TEST(EIGEN_MATRIX, init_dynamic_matrix)
{
    const int m = 3, n = 3;
    Eigen::MatrixXd mat = Eigen::MatrixXd::Random(m, n);
    std::cout<<mat(0,0)<<std::endl;
}

TEST(EIGEN_MATRIX, gtest_comparasion)
{
    Eigen::Matrix3d iden = Eigen::Matrix3d::Identity();
    EXPECT_EQ(iden, Eigen::Matrix3d::Identity());

    EXPECT_NE(iden, Eigen::Matrix3d::Ones());
}

TEST(EIGEN_MATRIX, diagonal_matrix)
{
    Eigen::Vector3d vec(1, 2, 3);
    Eigen::Matrix3d mat = vec.asDiagonal();
    std::cout << mat << std::endl;
}

TEST(EIGEN_MATRIX,pointer)
{
	double *pt=new double[5];
	pt[0]=1.0;
	pt[1]=2.0;
	pt[2]=3.0;
	pt[3]=4.0;
	pt[4]=5.0;
//	pt++;
	Eigen::Map<Eigen::Matrix<double,3,1>> p(pt+1);
	std::cout<<p<<std::endl;
	
}