#include <gtest/gtest.h>

#include <Core/Maths/Matrix/Matrix.hpp>
#include <Core/Maths/Vector/DistanceVector.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtx/euler_angles.hpp"

USING_RUKEN_NAMESPACE

static Vector3m GlmVecToRkVec(glm::vec3 const& in_vec)
{ return Vector3m {static_cast<Meters>(in_vec.x), static_cast<Meters>(in_vec.y), static_cast<Meters>(in_vec.z)}; }

TEST(Matrix, Equality)
{
	constexpr Matrix3x3 a {1, 2, 3, 4, 5, 6, 7, 8, 9};
	constexpr Matrix3x3 b {2, 0, 3, 4, 5, 6, 7, 8, 9};

	EXPECT_EQ(a, a);
	EXPECT_NE(a, b);
}

TEST(Matrix, SizeConversion)
{
	constexpr Matrix3x3 		 a {1, 2, 3, 4, 5, 6, 7, 8, 9};
	constexpr Matrix4x4 		 b {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	constexpr Matrix4x4 expected_a {1, 2, 3, 0, 4, 5, 6, 0, 7, 8, 9, 0, 0, 0, 0, 1};
	constexpr Matrix3x3 expexted_b {1, 2, 3, 5, 6, 7, 9, 10, 11};

	Matrix4x4 const upgraded_a   {a};
	Matrix3x3 const downgraded_b {b};

	EXPECT_EQ(upgraded_a  , expected_a);
	EXPECT_EQ(downgraded_b, expexted_b);
}

TEST(Matrix, Access)
{
	constexpr Matrix3x3 a {1, 2, 3, 4, 5, 6, 7, 8, 9};

	constexpr auto value1 {a[0, 0]};
	constexpr auto value2 {a[1, 0]};
	constexpr auto value3 {a[2, 2]};

	EXPECT_FLOAT_EQ(value1, 1);
	EXPECT_FLOAT_EQ(value2, 4);
	EXPECT_FLOAT_EQ(value3, 9);
}

TEST(Matrix, Transposition)
{
	constexpr Matrix3x3 a {1, 2, 3, 4, 5, 6, 7, 8, 9};
	constexpr Matrix3x3 b {a.Transposed()};

	constexpr auto value1   {a[0, 2]};
	constexpr auto value1_b {b[2, 0]};
	constexpr auto value2   {a[0, 1]};
	constexpr auto value2_b {b[1, 0]};

	EXPECT_FLOAT_EQ(value1, value1_b);
	EXPECT_FLOAT_EQ(value2, value2_b);
}

TEST(Matrix, Translation)
{
	Vector2m const pos2 {(Matrix3x3::TranslationMatrix(Vector2m {10_m, 10_m	     }) * Vector3m {0_m, 0_m, 1_m}     ).Homogenized()};
	Vector3m const pos3 {(Matrix4x4::TranslationMatrix(Vector3m {10_m, 10_m, 10_m}) * Vector4m {0_m, 0_m, 0_m, 1_m}).Homogenized()};

	constexpr Vector2m expected_pos2 {10_m, 10_m};
	constexpr Vector3m expected_pos3 {10_m, 10_m, 10_m};

	EXPECT_EQ(pos2, expected_pos2);
	EXPECT_EQ(pos3, expected_pos3);
}

TEST(Matrix, Scale)
{
	Vector2m const scale2 {Matrix2x2::ScaleMatrix(Vector2m {10_m, 10_m	    }) * Constants<Vector2m>::one};
	Vector3m const scale3 {Matrix3x3::ScaleMatrix(Vector3m {10_m, 10_m, 10_m}) * Constants<Vector3m>::one};

	constexpr Vector2m expected_scale2 {10_m, 10_m};
	constexpr Vector3m expected_scale3 {10_m, 10_m, 10_m};

	EXPECT_EQ(scale2, expected_scale2);
	EXPECT_EQ(scale3, expected_scale3);
}

TEST(Matrix, RotationX)
{
	// Ruken
	auto const rot_3d_euler {Matrix3x3::RotationMatrix3D({90_deg, 0_deg, 0_deg})};
	auto const rot_3d_axis  {Matrix3x3::RotationMatrix3D(Constants<Vector3m>::right, 90_deg)};
	auto const rot_x	    {Matrix3x3::RotationMatrixX (90_deg)};

	Vector3m const pos_3d_euler {rot_3d_euler * Constants<Vector3m>::forward};
	Vector3m const pos_3d_axis  {rot_3d_axis  * Constants<Vector3m>::forward};
	Vector3m const pos_x	    {rot_x	      * Constants<Vector3m>::forward};

	EXPECT_EQ(pos_3d_euler, Constants<Vector3m>::down);
	EXPECT_EQ(pos_3d_axis , Constants<Vector3m>::down);
	EXPECT_EQ(pos_x       , Constants<Vector3m>::down);

	// Test GLM parity
	glm::mat4 const glm_rot_matrix {glm::rotate(glm::mat4{1.0f}, glm::radians(90.0f), glm::vec3 {1, 0, 0})};
	glm::vec4 const rotated_vec    {glm_rot_matrix * glm::vec4 {0, 0, 1, 1}};

	EXPECT_NEAR(rotated_vec.x,  0.f, 1.0e-5f);
	EXPECT_NEAR(rotated_vec.y, -1.f, 1.0e-5f);
	EXPECT_NEAR(rotated_vec.z,  0.f, 1.0e-5f);
	EXPECT_NEAR(rotated_vec.w,  1.f, 1.0e-5f);
}

TEST(Matrix, RotationY)
{
	// Ruken
	auto const rot_3d_euler {Matrix3x3::RotationMatrix3D({0_deg, 90_deg, 0_deg})};
	auto const rot_3d_axis  {Matrix3x3::RotationMatrix3D(Constants<Vector3m>::up, 90_deg)};
	auto const rot_y	    {Matrix3x3::RotationMatrixY (90_deg)};

	Vector3m const pos_3d_euler {rot_3d_euler * Constants<Vector3m>::forward};
	Vector3m const pos_3d_axis  {rot_3d_axis  * Constants<Vector3m>::forward};
	Vector3m const pos_y	    {rot_y	      * Constants<Vector3m>::forward};

	EXPECT_EQ(pos_3d_euler, Constants<Vector3m>::right);
	EXPECT_EQ(pos_3d_axis , Constants<Vector3m>::right);
	EXPECT_EQ(pos_y       , Constants<Vector3m>::right);

	// Test GLM parity
	glm::mat4 const glm_rot_matrix {glm::rotate(glm::mat4{1.0f}, glm::radians(90.0f), glm::vec3 {0, 1, 0})};
	glm::vec4 const rotated_vec    {glm_rot_matrix * glm::vec4 {0, 0, 1, 1}};

	EXPECT_NEAR(rotated_vec.x, 1.f, 1.0e-5f);
	EXPECT_NEAR(rotated_vec.y, 0.f, 1.0e-5f);
	EXPECT_NEAR(rotated_vec.z, 0.f, 1.0e-5f);
	EXPECT_NEAR(rotated_vec.w, 1.f, 1.0e-5f);
}

TEST(Matrix, RotationZ)
{
	// Ruken
	auto const rot_3d_euler {Matrix3x3::RotationMatrix3D({0_deg, 0_deg, 90_deg})};
	auto const rot_3d_axis  {Matrix3x3::RotationMatrix3D(Constants<Vector3m>::forward, 90_deg)};
	auto const rot_z	    {Matrix3x3::RotationMatrixZ (90_deg)};

	Vector3m const pos_3d_euler {rot_3d_euler * Constants<Vector3m>::up};
	Vector3m const pos_3d_axis  {rot_3d_axis  * Constants<Vector3m>::up};
	Vector3m const pos_z	    {rot_z	      * Constants<Vector3m>::up};

	EXPECT_EQ(pos_3d_euler, Constants<Vector3m>::left);
	EXPECT_EQ(pos_3d_axis , Constants<Vector3m>::left);
	EXPECT_EQ(pos_z       , Constants<Vector3m>::left);

	// Test GLM parity
	glm::mat4 const glm_rot_matrix {glm::rotate(glm::mat4{1.0f}, glm::radians(90.0f), glm::vec3 {0, 0, 1})};
	glm::vec4 const rotated_vec    {glm_rot_matrix * glm::vec4 {0, 1, 0, 1}};

	EXPECT_NEAR(rotated_vec.x, -1.f, 1.0e-5f);
	EXPECT_NEAR(rotated_vec.y,  0.f, 1.0e-5f);
	EXPECT_NEAR(rotated_vec.z,  0.f, 1.0e-5f);
	EXPECT_NEAR(rotated_vec.w,  1.f, 1.0e-5f);
}

TEST(Matrix, RotationOrder)
{
	auto const glm_mat {glm::mat3x3(glm::quat(glm::vec3{glm::radians(45.0f)}))};
	auto const rk_mat  {Matrix3x3::RotationMatrix3D({45_deg, 45_deg, 45_deg})};

	auto const glm_value {glm_mat * glm::vec3(1)};
	auto const rk_value  {rk_mat  * Constants<Vector3m>::one};

	EXPECT_NEAR(glm_value.x, static_cast<RkFloat>(rk_value.x()), 1.0e-5f);
	EXPECT_NEAR(glm_value.y, static_cast<RkFloat>(rk_value.y()), 1.0e-5f);
	EXPECT_NEAR(glm_value.z, static_cast<RkFloat>(rk_value.z()), 1.0e-5f);
}

TEST(Matrix, InverseProjection)
{
	Matrix4x4 const rk_projection {Matrix4x4::PerspectiveProjection(90_deg, 1.0f, 1_cm, 1_km)};
	Matrix4x4 const rk_inverse    {rk_projection.Inverted()};
	Matrix4x4 const rk_result     {rk_projection * rk_inverse};
	Vector3m  const far_value  {(rk_inverse * Vector4m {0_m, 0_m, 1_m, 1_m}).Homogenized()};
	Vector3m  const near_value {(rk_inverse * Vector4m {0_m, 0_m, 0_m, 1_m}).Homogenized()};

	EXPECT_EQ  (rk_result , Matrix4x4());
	EXPECT_NEAR(static_cast<RkFloat>(far_value .z()), 1000.0f, 1.0f);
	EXPECT_NEAR(static_cast<RkFloat>(near_value.z()),   0.01f, 1.0e-5f);
}

TEST(Matrix, InverseView)
{
	constexpr Vector3m position {10_m, 10_m, 20_m};
	Matrix4x4 const rk_view     {Matrix4x4::ModelMatrix(position, Quaternion {}).Inverted()};
	Matrix4x4 const rk_inverse  {rk_view.Inverted()};
	Matrix4x4 const rk_result   {rk_view * rk_inverse};
	Vector3m  const inv_value   {(rk_inverse * Vector4m {0_m, 0_m, 0_m, 1_m}).Homogenized()};
	Vector3m  const zero_value  {(rk_inverse * Vector4m {0_m, 0_m, 0_m, 0_m}).xyz()};

	EXPECT_EQ(rk_result, Matrix4x4());
	EXPECT_EQ(position, inv_value);
	EXPECT_EQ(zero_value, Constants<Vector3m>::zero);
}

TEST(Matrix, InverseViewProjection)
{
	Matrix4x4 const rk_view_proj {
		Matrix4x4::ModelMatrix({10_m, 10_m, 20_m}, Quaternion {{20_deg, 0_deg}}).Inverted() *
		Matrix4x4::PerspectiveProjection(90_deg, 1.0f, 1_cm, 1_km)
	};
	glm::mat4 const glm_view_proj {
		glm::perspective(glm::radians(90.0f), 1.0f, 0.01f, 1000.0f) *
		glm::inverse    (glm::translate(glm::mat4 {1.0f}, {10, 10, 20}) *
						 glm::eulerAngleXYZ(glm::radians(20.0f), 0.0f, 0.0f)
		)
	};

	Matrix4x4 const rk_inverse      {rk_view_proj.Inverted()};
	glm::mat4 const glm_inverse     {glm::inverse(glm_view_proj)};
	Vector3m  const camera_position {(rk_inverse * Vector4m(0_m, 0_m, 0_m, 1_m)).Homogenized()};
	glm::vec4 const glm_position    {glm_inverse * glm::vec4(0, 0, 0, 1)};
	glm::vec3 const glm_pos         {glm::vec3(glm_position) / glm_position.w};

	// Float precision becomes awful really quickly with all of these operations
	EXPECT_NEAR(glm_pos.x, static_cast<RkFloat>(camera_position.x()), 1e-1f);
	EXPECT_NEAR(glm_pos.y, static_cast<RkFloat>(camera_position.y()), 1e-1f);
	EXPECT_NEAR(glm_pos.z, static_cast<RkFloat>(camera_position.z()), 1e-1f);
}

TEST(MatrixDeath, Access)
{
	auto death = [] {
		Matrix3x3 a {1, 2, 3, 4, 5, 6, 7, 8, 9};
		auto value = a[4, 3];
	};

	EXPECT_DEBUG_DEATH(death(), "");
}