#include <gtest/gtest.h>

#include <Core/Maths/Quaternion/Quaternion.hpp>

#include <glm/glm.hpp>
#include "glm/gtc/quaternion.hpp"

USING_RUKEN_NAMESPACE

static Vector3m GlmVecToRkVec(glm::vec3 const& in_vec)
{ return Vector3m {static_cast<Meters>(in_vec.x), static_cast<Meters>(in_vec.y), static_cast<Meters>(in_vec.z)}; }

TEST(Quaternion, RotationX)
{
	Quaternion const rk_euler  {{90_deg, 0_deg, 0_deg}};
	Quaternion const rk_axis   {Constants<Vector3m>::right, 90_deg};
	glm::quat  const glm_axis  {glm::angleAxis(glm::radians(90.0f), glm::vec3 {1, 0, 0})};
	glm::quat  const glm_euler {{glm::radians(90.0f), .0f, .0f}};

	glm::vec3  const glm_value     {0, 0, 1};
	glm::vec3  const glm_axis_rot  {glm_axis  * glm_value};
	glm::vec3  const glm_euler_rot {glm_euler * glm_value};

	Vector3m   const rk_value     {Constants<Vector3m>::forward};
	Vector3m   const rk_axis_rot  {rk_axis  * rk_value};
	Vector3m   const rk_euler_rot {rk_euler * rk_value};

	EXPECT_EQ(GlmVecToRkVec(glm_axis_rot) , rk_axis_rot);
	EXPECT_EQ(GlmVecToRkVec(glm_euler_rot), rk_euler_rot);
	EXPECT_EQ(rk_axis_rot, rk_euler_rot);
}

TEST(Quaternion, RotationY)
{
	Quaternion const rk_euler  {{0_deg, 90_deg, 0_deg}};
	Quaternion const rk_axis   {Constants<Vector3m>::up, 90_deg};
	glm::quat  const glm_axis  {glm::angleAxis(glm::radians(90.0f), glm::vec3 {0, 1, 0})};
	glm::quat  const glm_euler {{.0f, glm::radians(90.0f), .0f}};

	glm::vec3  const glm_value     {0, 0, 1};
	glm::vec3  const glm_axis_rot  {glm_axis  * glm_value};
	glm::vec3  const glm_euler_rot {glm_euler * glm_value};

	Vector3m   const rk_value     {Constants<Vector3m>::forward};
	Vector3m   const rk_axis_rot  {rk_axis  * rk_value};
	Vector3m   const rk_euler_rot {rk_euler * rk_value};

	EXPECT_EQ(GlmVecToRkVec(glm_axis_rot) , rk_axis_rot);
	EXPECT_EQ(GlmVecToRkVec(glm_euler_rot), rk_euler_rot);
	EXPECT_EQ(rk_axis_rot, rk_euler_rot);
}

TEST(Quaternion, RotationZ)
{
	Quaternion const rk_euler  {{0_deg, 0_deg, 90_deg}};
	Quaternion const rk_axis   {Constants<Vector3m>::forward, 90_deg};
	glm::quat  const glm_axis  {glm::angleAxis(glm::radians(90.0f), glm::vec3 {0, 0, 1})};
	glm::quat  const glm_euler {{.0f, .0f, glm::radians(90.0f)}};

	glm::vec3  const glm_value     {0, 1, 0};
	glm::vec3  const glm_axis_rot  {glm_axis  * glm_value};
	glm::vec3  const glm_euler_rot {glm_euler * glm_value};

	Vector3m   const rk_value     {Constants<Vector3m>::up};
	Vector3m   const rk_axis_rot  {rk_axis  * rk_value};
	Vector3m   const rk_euler_rot {rk_euler * rk_value};

	EXPECT_EQ(GlmVecToRkVec(glm_axis_rot) , rk_axis_rot);
	EXPECT_EQ(GlmVecToRkVec(glm_euler_rot), rk_euler_rot);
	EXPECT_EQ(rk_axis_rot, rk_euler_rot);
}
TEST(Quaternion, RotationOrder)
{
	Quaternion const rk_euler  {{45_deg, 45_deg, 45_deg}};
	glm::quat  const glm_euler {glm::vec3{glm::radians(45.0f)}};

	Vector3m   const rk_euler_rot  {rk_euler * Constants<Vector3m>::one};
	glm::vec3  const glm_euler_rot {glm_euler * glm::vec3 {1}};

	EXPECT_EQ(GlmVecToRkVec(glm_euler_rot), rk_euler_rot);
}