/*
  /*
 * Copyright (c) 2013, Christian Gehring, Hannes Sommer, Paul Furgale, Remo Diethelm
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Autonomous Systems Lab, ETH Zurich nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Christian Gehring, Hannes Sommer, Paul Furgale,
 * Remo Diethelm BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

#include <iostream>

#include <Eigen/Core>

#include <gtest/gtest.h>

#include "kindr/vectors/VectorEigen.hpp"
#include "kindr/rotations/RotationEigen.hpp"
#include "kindr/common/gtest_eigen.hpp"



namespace vectors = kindr::vectors::eigen_impl;
namespace rotations = kindr::rotations::eigen_impl;







template <typename VectorImplementation>
struct VectorTest: public ::testing::Test {
  typedef VectorImplementation Vector;
  typedef typename Vector::Scalar Scalar;
  typedef typename Vector::Implementation EigenVector;

  Scalar tol, sum, max, min, mean;
  EigenVector vecZero, vec1, vec2, vecAdd, vecSubtract;

  Vector vectorDefault;
  Vector vectorFromMultipleValues;
  Vector vector1FromEigen;
  Vector vector2FromEigen;
  Vector vectorFromVector;

  VectorTest() : tol(1e-6),
                 sum(150),
                 max(50),
                 min(10),
                 mean(30)
  {
    vecZero = EigenVector::Zero();
    vec1 << 10,20,30,40,50;
    vec2 << 1,2,3,4,5;
    vecAdd << 11,22,33,44,55;
    vecSubtract << 9,18,27,36,45;
    vectorFromMultipleValues << vec1(0),vec1(1),vec1(2),vec1(3),vec1(4);
    vector1FromEigen = Vector(vec1);
    vector2FromEigen = Vector(vec2);
    vectorFromVector = vector1FromEigen;
  }
};



typedef ::testing::Types<
    vectors::Vector<kindr::phys_quant::PhysicalType::Typeless, double, 5>,
    vectors::Vector<kindr::phys_quant::PhysicalType::Typeless, float,  5>,
    vectors::Vector<kindr::phys_quant::PhysicalType::Force, double, 5>,
    vectors::Vector<kindr::phys_quant::PhysicalType::Force, float,  5>,
    vectors::Vector<kindr::phys_quant::PhysicalType::Position, double, 5>,
    vectors::Vector<kindr::phys_quant::PhysicalType::Position, float,  5>
> Types5;


TYPED_TEST_CASE(VectorTest, Types5);

TYPED_TEST(VectorTest, testVector)
{
  // vector typedefs
  typedef typename TestFixture::Vector Vector;
  typedef vectors::Vector<kindr::phys_quant::PhysicalType::Typeless, typename TestFixture::Scalar, 5> OtherVector;
  OtherVector vector2FromEigenOtherType(this->vec2);
  typedef typename TestFixture::EigenVector EigenVector;

  // vector typedefs (dimension 3)
  typedef Eigen::Matrix<double, 3, 1> EigenVector3;
  typedef vectors::Vector<kindr::phys_quant::PhysicalType::Position, double, 3> Length3d;
  typedef vectors::Vector<kindr::phys_quant::PhysicalType::Force, double, 3> Force3d;
  typedef vectors::Vector<kindr::phys_quant::PhysicalType::Torque, double, 3> Torque3d;

  // default constructor
  ASSERT_EQ(this->vectorDefault(0), this->vecZero(0)) << "Default constructor needs to initialize component 1 to zero!";
  ASSERT_EQ(this->vectorDefault(1), this->vecZero(1)) << "Default constructor needs to initialize component 2 to zero!";
  ASSERT_EQ(this->vectorDefault(2), this->vecZero(2)) << "Default constructor needs to initialize component 3 to zero!";
  ASSERT_EQ(this->vectorDefault(3), this->vecZero(3)) << "Default constructor needs to initialize component 4 to zero!";
  ASSERT_EQ(this->vectorDefault(4), this->vecZero(4)) << "Default constructor needs to initialize component 5 to zero!";

  // constructor with multiple values
  ASSERT_EQ(this->vectorFromMultipleValues(0), this->vec1(0)) << "Multi-Value Constructor needs to initialize component 1!";
  ASSERT_EQ(this->vectorFromMultipleValues(1), this->vec1(1)) << "Multi-Value Constructor needs to initialize component 2!";
  ASSERT_EQ(this->vectorFromMultipleValues(2), this->vec1(2)) << "Multi-Value Constructor needs to initialize component 3!";
  ASSERT_EQ(this->vectorFromMultipleValues(3), this->vec1(3)) << "Multi-Value Constructor needs to initialize component 4!";
  ASSERT_EQ(this->vectorFromMultipleValues(4), this->vec1(4)) << "Multi-Value Constructor needs to initialize component 5!";

  // constructor with Eigen vector
  ASSERT_EQ(this->vector1FromEigen(0), this->vec1(0)) << "Base Constructor needs to initialize component 1!";
  ASSERT_EQ(this->vector1FromEigen(1), this->vec1(1)) << "Base Constructor needs to initialize component 2!";
  ASSERT_EQ(this->vector1FromEigen(2), this->vec1(2)) << "Base Constructor needs to initialize component 3!";
  ASSERT_EQ(this->vector1FromEigen(3), this->vec1(3)) << "Base Constructor needs to initialize component 4!";
  ASSERT_EQ(this->vector1FromEigen(4), this->vec1(4)) << "Base Constructor needs to initialize component 5!";

  // constructor with Vector
  ASSERT_EQ(this->vectorFromVector(0), this->vec1(0));
  ASSERT_EQ(this->vectorFromVector(1), this->vec1(1));
  ASSERT_EQ(this->vectorFromVector(2), this->vec1(2));
  ASSERT_EQ(this->vectorFromVector(3), this->vec1(3));
  ASSERT_EQ(this->vectorFromVector(4), this->vec1(4));

  // constructor with x, y, z
  Length3d constructorXYZ(1, 2, 3);
  ASSERT_EQ(constructorXYZ.x(), 1);
  ASSERT_EQ(constructorXYZ.y(), 2);
  ASSERT_EQ(constructorXYZ.z(), 3);

  // Zero()
  Vector zero;
  zero << 1,2,3,4,5;
  zero = Vector::Zero();
  ASSERT_EQ(zero(0), 0);
  ASSERT_EQ(zero(1), 0);
  ASSERT_EQ(zero(2), 0);
  ASSERT_EQ(zero(3), 0);
  ASSERT_EQ(zero(4), 0);

  // setZero()
  zero << 1,2,3,4,5;
  zero.setZero();
  ASSERT_EQ(zero(0), 0);
  ASSERT_EQ(zero(1), 0);
  ASSERT_EQ(zero(2), 0);
  ASSERT_EQ(zero(3), 0);
  ASSERT_EQ(zero(4), 0);

  // UnitX()
  Length3d unit;
  unit = Length3d::UnitX();
  ASSERT_EQ(unit(0), 1);
  ASSERT_EQ(unit(1), 0);
  ASSERT_EQ(unit(2), 0);

  // UnitY()
  unit = Length3d::UnitY();
  ASSERT_EQ(unit(0), 0);
  ASSERT_EQ(unit(1), 1);
  ASSERT_EQ(unit(2), 0);

  // UnitZ()
  unit = Length3d::UnitZ();
  ASSERT_EQ(unit(0), 0);
  ASSERT_EQ(unit(1), 0);
  ASSERT_EQ(unit(2), 1);

  // x(), y(), z()
  Length3d xyz;
  xyz.x() = 1;
  xyz.y() = 2;
  xyz.z() = 3;
  ASSERT_EQ(xyz(0), 1);
  ASSERT_EQ(xyz(1), 2);
  ASSERT_EQ(xyz(2), 3);

  // toImplementation
  ASSERT_EQ(this->vectorFromMultipleValues.toImplementation()(0,0), this->vec1(0)) << "Component 1 needs to correspond to the matrix entry (0,0)!";
  ASSERT_EQ(this->vectorFromMultipleValues.toImplementation()(1,0), this->vec1(1)) << "Component 2 needs to correspond to the matrix entry (1,0)!";
  ASSERT_EQ(this->vectorFromMultipleValues.toImplementation()(2,0), this->vec1(2)) << "Component 3 needs to correspond to the matrix entry (2,0)!";
  ASSERT_EQ(this->vectorFromMultipleValues.toImplementation()(3,0), this->vec1(3)) << "Component 4 needs to correspond to the matrix entry (1,0)!";
  ASSERT_EQ(this->vectorFromMultipleValues.toImplementation()(4,0), this->vec1(4)) << "Component 5 needs to correspond to the matrix entry (2,0)!";

  // addition
  Vector vectorAdd = this->vector1FromEigen+this->vector2FromEigen;
  ASSERT_EQ(vectorAdd(0), this->vecAdd(0));
  ASSERT_EQ(vectorAdd(1), this->vecAdd(1));
  ASSERT_EQ(vectorAdd(2), this->vecAdd(2));
  ASSERT_EQ(vectorAdd(3), this->vecAdd(3));
  ASSERT_EQ(vectorAdd(4), this->vecAdd(4));

  // addition and assignment
  Vector vectorAddandAssign(this->vector1FromEigen);
  vectorAddandAssign += this->vector2FromEigen;
  ASSERT_EQ(vectorAddandAssign(0), this->vecAdd(0));
  ASSERT_EQ(vectorAddandAssign(1), this->vecAdd(1));
  ASSERT_EQ(vectorAddandAssign(2), this->vecAdd(2));
  ASSERT_EQ(vectorAddandAssign(3), this->vecAdd(3));
  ASSERT_EQ(vectorAddandAssign(4), this->vecAdd(4));

  // subtract
  Vector vectorSubtract = this->vector1FromEigen-this->vector2FromEigen;
  ASSERT_EQ(vectorSubtract(0), this->vecSubtract(0));
  ASSERT_EQ(vectorSubtract(1), this->vecSubtract(1));
  ASSERT_EQ(vectorSubtract(2), this->vecSubtract(2));
  ASSERT_EQ(vectorSubtract(3), this->vecSubtract(3));
  ASSERT_EQ(vectorSubtract(4), this->vecSubtract(4));

  // subtract and assignment
  Vector vectorSubtractandAssign(this->vector1FromEigen);
  vectorSubtractandAssign -= this->vector2FromEigen;
  ASSERT_EQ(vectorSubtractandAssign(0), this->vecSubtract(0));
  ASSERT_EQ(vectorSubtractandAssign(1), this->vecSubtract(1));
  ASSERT_EQ(vectorSubtractandAssign(2), this->vecSubtract(2));
  ASSERT_EQ(vectorSubtractandAssign(3), this->vecSubtract(3));
  ASSERT_EQ(vectorSubtractandAssign(4), this->vecSubtract(4));

  // negation
  Vector vectorNegated = - this->vector1FromEigen;
  ASSERT_EQ(vectorNegated(0), - this->vector1FromEigen(0));
  ASSERT_EQ(vectorNegated(1), - this->vector1FromEigen(1));
  ASSERT_EQ(vectorNegated(2), - this->vector1FromEigen(2));
  ASSERT_EQ(vectorNegated(3), - this->vector1FromEigen(3));
  ASSERT_EQ(vectorNegated(4), - this->vector1FromEigen(4));

  // bracket assign 1
  Vector vector2FromEigenBackup(this->vec2);
  this->vector2FromEigen(0) = vector2FromEigenBackup(0);
  ASSERT_EQ(this->vector2FromEigen(0), vector2FromEigenBackup(0));

  // << assign 2
  this->vector2FromEigen << vector2FromEigenBackup(0),vector2FromEigenBackup(1),vector2FromEigenBackup(2),vector2FromEigenBackup(3),vector2FromEigenBackup(4);
  ASSERT_EQ(this->vector2FromEigen(0), vector2FromEigenBackup(0));
  ASSERT_EQ(this->vector2FromEigen(1), vector2FromEigenBackup(1));
  ASSERT_EQ(this->vector2FromEigen(2), vector2FromEigenBackup(2));
  ASSERT_EQ(this->vector2FromEigen(3), vector2FromEigenBackup(3));
  ASSERT_EQ(this->vector2FromEigen(4), vector2FromEigenBackup(4));

  // norm
  ASSERT_NEAR(this->vector2FromEigen.norm(), std::sqrt(this->vector2FromEigen(0)*this->vector2FromEigen(0) + this->vector2FromEigen(1)*this->vector2FromEigen(1) + this->vector2FromEigen(2)*this->vector2FromEigen(2) + this->vector2FromEigen(3)*this->vector2FromEigen(3) + this->vector2FromEigen(4)*this->vector2FromEigen(4)), 1e-6);

  // normalized
  EigenVector eigenVectorNormalized(this->vec2.normalized());
  Vector vectorNormalized(this->vector2FromEigen.normalized());
  ASSERT_NEAR(vectorNormalized(0), eigenVectorNormalized(0), this->tol);
  ASSERT_NEAR(vectorNormalized(1), eigenVectorNormalized(1), this->tol);
  ASSERT_NEAR(vectorNormalized(2), eigenVectorNormalized(2), this->tol);
  ASSERT_NEAR(vectorNormalized(3), eigenVectorNormalized(3), this->tol);
  ASSERT_NEAR(vectorNormalized(4), eigenVectorNormalized(4), this->tol);

  // normalize
  Vector vectorNormalized2(this->vec2);
  vectorNormalized2.normalize();
  ASSERT_NEAR(vectorNormalized2(0), eigenVectorNormalized(0), this->tol);
  ASSERT_NEAR(vectorNormalized2(1), eigenVectorNormalized(1), this->tol);
  ASSERT_NEAR(vectorNormalized2(2), eigenVectorNormalized(2), this->tol);
  ASSERT_NEAR(vectorNormalized2(3), eigenVectorNormalized(3), this->tol);
  ASSERT_NEAR(vectorNormalized2(4), eigenVectorNormalized(4), this->tol);

  // is similar to
  ASSERT_TRUE(vectorNormalized2.isSimilarTo(Vector(eigenVectorNormalized), this->tol));

//  // ==
//  ASSERT_TRUE(vectorNormalized2 == Vector(eigenVectorNormalized));
//
//  // !=
//  ASSERT_FALSE(vectorNormalized2 != Vector(eigenVectorNormalized));

  // sum
  ASSERT_EQ(this->vector1FromEigen.sum(), this->sum);

  // max
  ASSERT_EQ(this->vector1FromEigen.max(), this->max);

  // min
  ASSERT_EQ(this->vector1FromEigen.min(), this->min);

  // mean
  ASSERT_EQ(this->vector1FromEigen.mean(), this->mean);

  // dot
  ASSERT_NEAR(this->vector1FromEigen.dot(this->vector1FromEigen), this->vec1.dot(this->vec1), this->tol);

  // cross
  EigenVector3 crossVectorEigen1(1,2,3);
  EigenVector3 crossVectorEigen2(3,2,1);
  Length3d crossVector1(crossVectorEigen1);
  Force3d crossVector2(crossVectorEigen2);
  EigenVector3 crossProductResult(crossVectorEigen1.cross(crossVectorEigen2));
  Torque3d crossProductVector(crossVector1.cross(crossVector2));
  ASSERT_NEAR(crossProductVector(0), crossProductResult(0), this->tol);
  ASSERT_NEAR(crossProductVector(1), crossProductResult(1), this->tol);
  ASSERT_NEAR(crossProductVector(2), crossProductResult(2), this->tol);

  // elementwise multiplication
  Vector elementwiseMultiplicationVector(this->vector1FromEigen.elementwiseMultiplication(vector2FromEigenOtherType));
  ASSERT_EQ(elementwiseMultiplicationVector(0), this->vec1(0)*this->vec2(0));
  ASSERT_EQ(elementwiseMultiplicationVector(1), this->vec1(1)*this->vec2(1));
  ASSERT_EQ(elementwiseMultiplicationVector(2), this->vec1(2)*this->vec2(2));
  ASSERT_EQ(elementwiseMultiplicationVector(3), this->vec1(3)*this->vec2(3));
  ASSERT_EQ(elementwiseMultiplicationVector(4), this->vec1(4)*this->vec2(4));

  // elementwise division
  Vector elementwiseDivisionVector(this->vector1FromEigen.elementwiseDivision(vector2FromEigenOtherType));
  ASSERT_EQ(elementwiseDivisionVector(0), this->vec1(0)/this->vec2(0));
  ASSERT_EQ(elementwiseDivisionVector(1), this->vec1(1)/this->vec2(1));
  ASSERT_EQ(elementwiseDivisionVector(2), this->vec1(2)/this->vec2(2));
  ASSERT_EQ(elementwiseDivisionVector(3), this->vec1(3)/this->vec2(3));
  ASSERT_EQ(elementwiseDivisionVector(4), this->vec1(4)/this->vec2(4));

  // head & tail
  auto headAndTailResult(this->vec1.head(2) + this->vec1.tail(2));
  Vector vector1FromEigen2(this->vector1FromEigen);
  auto headAndTailVector(vector1FromEigen2.template head<2>() + vector1FromEigen2.template tail<2>());
  ASSERT_EQ(headAndTailResult(0), headAndTailVector(0));
  ASSERT_EQ(headAndTailResult(1), headAndTailVector(1));

  // segment
  EigenVector vec12(this->vec1);
  auto segmentResult(vec12.block(1,0,3,1));
  auto segmentVector(vector1FromEigen2.template segment<1,3>());
  ASSERT_EQ(segmentResult(0), segmentVector(0));
  ASSERT_EQ(segmentResult(1), segmentVector(1));
  ASSERT_EQ(segmentResult(2), segmentVector(2));

  // rotate
  Length3d length(1,2,3);
  rotations::RotationMatrixPD rot(rotations::AngleAxisPD(M_PI, 0, 0, 1));
  Length3d result = rot.rotate(length);
  ASSERT_NEAR(result(0), - length(0), this->tol);
  ASSERT_NEAR(result(1), - length(1), this->tol);
  ASSERT_NEAR(result(2),   length(2), this->tol);
}