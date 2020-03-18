//
// Created by books on 2019/11/10.
//

#include <iostream>
#include <gtest/gtest.h>

#include <Eigen/Core>
#include <Eigen/Geometry>

TEST(EIGEN_GEOMETRY, vector_cross)
{
    Eigen::Vector3d v1(1, 0, 0);
    Eigen::Vector3d v2(0, 1, 0);
    Eigen::Vector3d v12 = v2.cross(v1);
    EXPECT_EQ(v12, Eigen::Vector3d(0, 0, -1));
}

TEST(EIGEN_GEOMETRY, point_2d)
{
    Eigen::Vector2d pt(0.3, 0.5);
    Eigen::Vector3d pt_h = pt.homogeneous();
    Eigen::Vector2d pt2 = pt_h.hnormalized();
    EXPECT_EQ(pt_h(2), 1.0);
    EXPECT_EQ(pt, pt2);
}

TEST(EIGEN_GEOMETRY, angle_axis)
{
    double theta = 0.25 * M_PI;
    Eigen::AngleAxisd aa_x = Eigen::AngleAxisd(theta, Eigen::Vector3d::UnitX());
    Eigen::AngleAxisd aa_y = Eigen::AngleAxisd(theta, Eigen::Vector3d::UnitY());
    Eigen::AngleAxisd aa_z = Eigen::AngleAxisd(theta, Eigen::Vector3d::UnitZ());
    Eigen::Matrix3d rot;
    rot = aa_x * aa_y * aa_z;
    Eigen::Vector3d euler_angles = rot.eulerAngles(0, 1, 2);
    EXPECT_EQ(euler_angles, Eigen::Vector3d::Constant(theta));
}

TEST(EIGEN_GEOMETRY, rotation_2d)
{
    double theta = 0.25 * M_PI;
    Eigen::Rotation2Dd r(theta);
    EXPECT_EQ(r.angle(), theta);

    /// R*pt
    Eigen::Vector2d pt(1, 1);
    Eigen::Vector2d pt_r = r * pt;
    EXPECT_DOUBLE_EQ(std::sqrt(2), pt_r.norm());
}

TEST(EIGEN_GEOMETRY, translation_2d)
{
    Eigen::Translation2d t(1.0, 2.0);
    EXPECT_EQ(t.x(), 1.0);
    EXPECT_EQ(t.y(), 2.0);

    /// use multiply to express two translations' concatenate
    Eigen::Translation2d t2(1.0, 1.0);
    Eigen::Translation2d t3 = t * t2;
    EXPECT_EQ(t3.x(), 2.0);
    EXPECT_EQ(t3.y(), 3.0);

    Eigen::Vector2d pt(1, 1);
    Eigen::Vector2d pt2 = t * pt;
    EXPECT_EQ(pt2, Eigen::Vector2d(2, 3));
}

TEST(EIGEN_GEOMETRY, scale_2d)
{
    Eigen::UniformScaling<double> us1 = Eigen::Scaling(0.1);
    Eigen::UniformScaling<double> us2(0.9);
    Eigen::UniformScaling<double> us3 = us1 * us2;
    EXPECT_DOUBLE_EQ(us3.factor(), 0.09);

    Eigen::Vector2d pt(1.0, 1.0);
    Eigen::Vector2d pt2 = us1 * pt;
    EXPECT_EQ(pt2, Eigen::Vector2d(0.1, 0.1));

    Eigen::AlignedScaling2d as1 = Eigen::Scaling(0.1, 0.2);
    EXPECT_EQ(as1.diagonal(), Eigen::Vector2d(0.1, 0.2));

    Eigen::Vector2d pt3 = as1 * pt;
    EXPECT_EQ(pt3, Eigen::Vector2d(0.1, 0.2));

}

TEST(EIGEN_GEOMETRY, isometry_2d)
{
    Eigen::Isometry2d T1;
    T1.setIdentity();
    EXPECT_EQ(T1.matrix(), Eigen::Matrix3d::Identity());
    EXPECT_EQ(T1.linear(), Eigen::Matrix2d::Identity());

    Eigen::Vector2d pt1(1.0, 2.0);
    Eigen::Vector2d pt2 = T1 * pt1;
    EXPECT_EQ(pt1, pt2);

    Eigen::Isometry2d T2 = Eigen::Isometry2d::Identity();
    T1.prerotate(Eigen::Rotation2Dd(0.25 * M_PI));
    T2.rotate(Eigen::Rotation2Dd(0.25 * M_PI));
    EXPECT_EQ(T1.matrix(), T2.matrix());

    T1.translate(Eigen::Vector2d(0.5, 0.3));
    T2.pretranslate(Eigen::Vector2d(0.5, 0.3));
    EXPECT_NE(T1.matrix(), T2.matrix());

    std::cout << T1.matrix() << std::endl;
    std::cout << T2.matrix() << std::endl;
}

TEST(EIGEN_GEOMETRY, affine_2d)
{
    Eigen::Affine2d A1 = Eigen::Affine2d::Identity();
    EXPECT_EQ(A1.matrix(), Eigen::Matrix3d::Identity());
    EXPECT_EQ(A1.linear(), Eigen::Matrix2d::Identity());

    A1.translate(Eigen::Vector2d(3, 4));
    A1.prescale(0.1);
    A1.rotate(Eigen::Rotation2Dd(0.25 * M_PI));
    std::cout << A1.matrix() << std::endl;
}

TEST(EIGEN_GEOMETRY, projective_2d)
{
    Eigen::Projective2d p;
    p.setIdentity();
    EXPECT_EQ(p.matrix(), Eigen::Matrix3d::Identity());
    std::cout << p.affine().matrix() << std::endl;
}

TEST(EIGEN_GEOMETRY,transform_3d)
{
    
}