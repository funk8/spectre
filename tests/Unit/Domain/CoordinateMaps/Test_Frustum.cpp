// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "Framework/TestingFramework.hpp"

#include <array>
#include <cmath>
#include <memory>
#include <optional>
#include <pup.h>
#include <random>

#include "DataStructures/Tensor/EagerMath/Determinant.hpp"
#include "DataStructures/Tensor/Tensor.hpp"
#include "Domain/CoordinateMaps/Frustum.hpp"
#include "Domain/Structure/OrientationMap.hpp"
#include "Framework/TestHelpers.hpp"
#include "Helpers/Domain/CoordinateMaps/TestMapHelpers.hpp"
#include "Utilities/ErrorHandling/Error.hpp"
#include "Utilities/TypeTraits.hpp"

namespace domain {
namespace {
void test_suite_for_frustum(const bool with_equiangular_map) {
  INFO("Suite for frustum");
  // Set up random number generator
  MAKE_GENERATOR(gen);
  std::uniform_real_distribution<> lower_bound_lower_base_dis(-9, -3);
  std::uniform_real_distribution<> upper_bound_lower_base_dis(3, 9);
  std::uniform_real_distribution<> lower_bound_upper_base_dis(-14, -9);
  std::uniform_real_distribution<> upper_bound_upper_base_dis(9, 14);

  const double lower_x_lower_base = lower_bound_lower_base_dis(gen);
  CAPTURE(lower_x_lower_base);
  const double lower_y_lower_base = lower_bound_lower_base_dis(gen);
  CAPTURE(lower_y_lower_base);
  const double upper_x_lower_base = upper_bound_lower_base_dis(gen);
  CAPTURE(upper_x_lower_base);
  const double upper_y_lower_base = upper_bound_lower_base_dis(gen);
  CAPTURE(upper_y_lower_base);
  const double lower_x_upper_base = lower_bound_upper_base_dis(gen);
  CAPTURE(lower_x_upper_base);
  const double lower_y_upper_base = lower_bound_upper_base_dis(gen);
  CAPTURE(lower_y_upper_base);
  const double upper_x_upper_base = upper_bound_upper_base_dis(gen);
  CAPTURE(upper_x_upper_base);
  const double upper_y_upper_base = upper_bound_upper_base_dis(gen);
  CAPTURE(upper_y_upper_base);

  for (OrientationMapIterator<3> map_i{}; map_i; ++map_i) {
    if (get(determinant(discrete_rotation_jacobian(*map_i))) < 0.0) {
      continue;
    }
    const std::array<std::array<double, 2>, 4> face_vertices{
        {{{lower_x_lower_base, lower_y_lower_base}},
         {{upper_x_lower_base, upper_y_lower_base}},
         {{lower_x_upper_base, lower_y_upper_base}},
         {{upper_x_upper_base, upper_y_upper_base}}}};
    const CoordinateMaps::Frustum frustum_map(face_vertices, -1.0, 2.0, map_i(),
                                              with_equiangular_map, 1.2, false,
                                              0.5);
    test_suite_for_map_on_unit_cube(frustum_map);
  }
}

void test_frustum_fail() {
  INFO("Frustum fail");
  const std::array<std::array<double, 2>, 4> face_vertices{
      {{{-2.0, -2.0}}, {{2.0, 2.0}}, {{-4.0, -4.0}}, {{4.0, 4.0}}}};
  const CoordinateMaps::Frustum map(face_vertices, 2.0, 5.0,
                                    OrientationMap<3>{});

  // For the choice of params above, any point with z<=-1 should fail.
  const std::array<double, 3> test_mapped_point1{{3.0, 3.0, -1.0}};
  const std::array<double, 3> test_mapped_point2{{6.0, -7.0, -1.0}};
  const std::array<double, 3> test_mapped_point3{{6.0, -7.0, -3.0}};

  // This is outside the mapped frustum, so inverse should either
  // return the correct inverse (which happens to be computable for
  // this point) or it should return nullopt.
  const std::array<double, 3> test_mapped_point4{{0.0, 0.0, 9.0}};

  CHECK_FALSE(map.inverse(test_mapped_point1).has_value());
  CHECK_FALSE(map.inverse(test_mapped_point2).has_value());
  CHECK_FALSE(map.inverse(test_mapped_point3).has_value());
  if (map.inverse(test_mapped_point4).has_value()) {
    CHECK_ITERABLE_APPROX(map(map.inverse(test_mapped_point4).value()),
                          test_mapped_point4);
  }
}

void test_alignment() {
  INFO("Alignment");
  // This test tests that the logical axes point along the expected directions
  // in physical space

  const std::array<std::array<double, 2>, 4> face_vertices{
      {{{-2.0, -2.0}}, {{2.0, 2.0}}, {{-4.0, -4.0}}, {{4.0, 4.0}}}};
  const double lower_bound = 2.0;
  const double upper_bound = 5.0;

  const auto wedge_directions = all_wedge_directions();
  const CoordinateMaps::Frustum map_upper_zeta(
      face_vertices, lower_bound, upper_bound,
      wedge_directions[0]);  // Upper Z frustum
  const CoordinateMaps::Frustum map_upper_eta(
      face_vertices, lower_bound, upper_bound,
      wedge_directions[2]);  // Upper Y frustum
  const CoordinateMaps::Frustum map_upper_xi(
      face_vertices, lower_bound, upper_bound,
      wedge_directions[4]);  // Upper X Frustum
  const CoordinateMaps::Frustum map_lower_zeta(
      face_vertices, lower_bound, upper_bound,
      wedge_directions[1]);  // Lower Z frustum
  const CoordinateMaps::Frustum map_lower_eta(
      face_vertices, lower_bound, upper_bound,
      wedge_directions[3]);  // Lower Y frustum
  const CoordinateMaps::Frustum map_lower_xi(
      face_vertices, lower_bound, upper_bound,
      wedge_directions[5]);  // Lower X frustum
  const std::array<double, 3> lowest_corner{{-1.0, -1.0, -1.0}};
  const std::array<double, 3> along_xi{{1.0, -1.0, -1.0}};
  const std::array<double, 3> along_eta{{-1.0, 1.0, -1.0}};
  const std::array<double, 3> along_zeta{{-1.0, -1.0, 1.0}};

  const std::array<double, 3> lowest_physical_corner_in_map_upper_zeta{
      {-2.0, -2.0, 2.0}};
  const std::array<double, 3> lowest_physical_corner_in_map_upper_eta{
      {-2.0, 2.0, -2.0}};
  const std::array<double, 3> lowest_physical_corner_in_map_upper_xi{
      {2.0, -2.0, -2.0}};
  const std::array<double, 3> lowest_physical_corner_in_map_lower_zeta{
      {-2.0, 2.0, -2.0}};
  const std::array<double, 3> lowest_physical_corner_in_map_lower_eta{
      {-2.0, -2.0, 2.0}};
  const std::array<double, 3> lowest_physical_corner_in_map_lower_xi{
      {-2.0, 2.0, -2.0}};

  // Test that this map's logical axes point along +X, +Y, +Z:
  CHECK(map_upper_zeta(along_xi)[0] == approx(2.0));
  CHECK(map_upper_zeta(along_eta)[1] == approx(2.0));
  CHECK(map_upper_zeta(along_zeta)[2] == approx(5.0));
  CHECK_ITERABLE_APPROX(map_upper_zeta(lowest_corner),
                        lowest_physical_corner_in_map_upper_zeta);

  // Test that this map's logical axes point along +Z, +X, +Y:
  CHECK(map_upper_eta(along_xi)[2] == approx(2.0));
  CHECK(map_upper_eta(along_eta)[0] == approx(2.0));
  CHECK(map_upper_eta(along_zeta)[1] == approx(5.0));
  CHECK_ITERABLE_APPROX(map_upper_eta(lowest_corner),
                        lowest_physical_corner_in_map_upper_eta);

  // Test that this map's logical axes point along +Y, +Z, +X:
  CHECK(map_upper_xi(along_xi)[1] == approx(2.0));
  CHECK(map_upper_xi(along_eta)[2] == approx(2.0));
  CHECK(map_upper_xi(along_zeta)[0] == approx(5.0));
  CHECK_ITERABLE_APPROX(map_upper_xi(lowest_corner),
                        lowest_physical_corner_in_map_upper_xi);

  // Test that this map's logical axes point along +X, -Y, -Z:
  CHECK(map_lower_zeta(along_xi)[0] == approx(2.0));
  CHECK(map_lower_zeta(along_eta)[1] == approx(-2.0));
  CHECK(map_lower_zeta(along_zeta)[2] == approx(-5.0));
  CHECK_ITERABLE_APPROX(map_lower_zeta(lowest_corner),
                        lowest_physical_corner_in_map_lower_zeta);

  // Test that this map's logical axes point along -Z, +X, -Y:
  CHECK(map_lower_eta(along_xi)[2] == approx(-2.0));
  CHECK(map_lower_eta(along_eta)[0] == approx(2.0));
  CHECK(map_lower_eta(along_zeta)[1] == approx(-5.0));
  CHECK_ITERABLE_APPROX(map_lower_eta(lowest_corner),
                        lowest_physical_corner_in_map_lower_eta);

  // Test that this map's logical axes point along -Y, +Z, -X:
  CHECK(map_lower_xi(along_xi)[1] == approx(-2.0));
  CHECK(map_lower_xi(along_eta)[2] == approx(2.0));
  CHECK(map_lower_xi(along_zeta)[0] == approx(-5.0));
  CHECK_ITERABLE_APPROX(map_lower_xi(lowest_corner),
                        lowest_physical_corner_in_map_lower_xi);
}

void test_auto_projective_scale_factor() {
  INFO("Auto projective scale factor");
  // This test tests that the correct suggested projective scale factor
  // is computed based on the side lengths of the frustum.

  // Set up random number generator
  MAKE_GENERATOR(gen);
  std::uniform_real_distribution<> lower_bound_dis(-14, -2);
  std::uniform_real_distribution<> upper_bound_dis(2, 14);
  std::uniform_real_distribution<> logical_dis(-1, 1);

  const double lower_x_lower_base = lower_bound_dis(gen);
  CAPTURE(lower_x_lower_base);
  const double lower_y_lower_base = lower_bound_dis(gen);
  CAPTURE(lower_y_lower_base);
  const double upper_x_lower_base = upper_bound_dis(gen);
  CAPTURE(upper_x_lower_base);
  const double upper_y_lower_base = upper_bound_dis(gen);
  CAPTURE(upper_y_lower_base);
  const double lower_x_upper_base = lower_bound_dis(gen);
  CAPTURE(lower_x_upper_base);
  const double lower_y_upper_base = lower_bound_dis(gen);
  CAPTURE(lower_y_upper_base);
  const double upper_x_upper_base = upper_bound_dis(gen);
  CAPTURE(upper_x_upper_base);
  const double upper_y_upper_base = upper_bound_dis(gen);
  CAPTURE(upper_y_upper_base);
  const double xi = logical_dis(gen);
  CAPTURE(xi);
  const double eta = logical_dis(gen);
  CAPTURE(eta);
  const double zeta = logical_dis(gen);
  CAPTURE(zeta);

  const std::array<double, 3> logical_coord{{0.0, 0.0, zeta}};
  const double lower_bound = 2.0;
  const double upper_bound = 5.0;
  const double sigma_z = 0.5 * (upper_bound + lower_bound);
  const double delta_z = 0.5 * (upper_bound - lower_bound);
  const double w_delta = sqrt(((upper_x_lower_base - lower_x_lower_base) *
                               (upper_y_lower_base - lower_y_lower_base)) /
                              ((upper_x_upper_base - lower_x_upper_base) *
                               (upper_y_upper_base - lower_y_upper_base)));
  const double projective_zeta = (w_delta - 1.0 + zeta * (w_delta + 1.0)) /
                                 (w_delta + 1.0 + zeta * (w_delta - 1.0));
  const double expected_physical_z = sigma_z + delta_z * (projective_zeta);
  const std::array<std::array<double, 2>, 4> face_vertices{
      {{{lower_x_lower_base, lower_y_lower_base}},
       {{upper_x_lower_base, upper_y_lower_base}},
       {{lower_x_upper_base, lower_y_upper_base}},
       {{upper_x_upper_base, upper_y_upper_base}}}};

  const CoordinateMaps::Frustum map(face_vertices, lower_bound, upper_bound, {},
                                    false, 1.0, true);  // Upper Z frustum
  CHECK(map(logical_coord)[2] == approx(expected_physical_z));
}

void test_is_identity() {
  INFO("Is identity");
  check_if_map_is_identity(CoordinateMaps::Frustum{
      std::array<std::array<double, 2>, 4>{
          {{{-1.0, -1.0}}, {{1.0, 1.0}}, {{-1.0, -1.0}}, {{1.0, 1.0}}}},
      -1.0, 1.0, OrientationMap<3>{}, false, 1.0});
  CHECK(not CoordinateMaps::Frustum{
      std::array<std::array<double, 2>, 4>{
          {{{-1.0, -1.0}}, {{2.0, 1.0}}, {{-1.0, -3.0}}, {{1.0, 1.0}}}},
      -1.0, 1.0, OrientationMap<3>{}, false, 1.5}
                .is_identity());
}

void test_bulged_frustum_jacobian() {
  INFO("Bulged frustum jacobian");
  const std::array<std::array<double, 2>, 4> face_vertices{
      {{{-2.0, -2.0}}, {{2.0, 2.0}}, {{-4.0, -4.0}}, {{4.0, 4.0}}}};
  const CoordinateMaps::Frustum map(
      face_vertices, 2.0, 5.0, OrientationMap<3>{}, false, 1.0, false, 1.0);

  const std::array<double, 3> test_point1{{-1.0, 0.25, 0.0}};
  const std::array<double, 3> test_point2{{1.0, 1.0, -0.5}};
  const std::array<double, 3> test_point3{{0.7, -0.2, 0.4}};
  const std::array<double, 3> test_point4{{0.0, 0.0, 0.0}};

  test_jacobian(map, test_point1);
  test_jacobian(map, test_point2);
  test_jacobian(map, test_point3);
  test_jacobian(map, test_point4);
}

void test_bulged_frustum_inv_jacobian() {
  INFO("Bulged frustum inverse jacobian");
  const std::array<std::array<double, 2>, 4> face_vertices{
      {{{-2.0, -2.0}}, {{2.0, 2.0}}, {{-4.0, -4.0}}, {{4.0, 4.0}}}};
  const CoordinateMaps::Frustum map(
      face_vertices, 2.0, 5.0, OrientationMap<3>{}, false, 1.0, false, 1.0);

  const std::array<double, 3> test_point1{{-1.0, 0.25, 0.0}};
  const std::array<double, 3> test_point2{{1.0, 1.0, -0.5}};
  const std::array<double, 3> test_point3{{0.7, -0.2, 0.4}};
  const std::array<double, 3> test_point4{{0.0, 0.0, 0.0}};

  test_inv_jacobian(map, test_point1);
  test_inv_jacobian(map, test_point2);
  test_inv_jacobian(map, test_point3);
  test_inv_jacobian(map, test_point4);
}

void test_bulged_frustum_inv_map() {
  INFO("Bulged frustum inverse map");
  const std::array<std::array<double, 2>, 4> face_vertices{
      {{{-2.0, -2.0}}, {{2.0, 2.0}}, {{-4.0, -4.0}}, {{4.0, 4.0}}}};
  const CoordinateMaps::Frustum map(
      face_vertices, 2.0, 5.0, OrientationMap<3>{}, false, 1.0, false, 1.0);

  const std::array<double, 3> test_point1{{-1.0, 0.25, 0.0}};
  const std::array<double, 3> test_point2{{1.0, 1.0, -0.5}};
  const std::array<double, 3> test_point3{{0.7, -0.2, 0.4}};
  const std::array<double, 3> test_point4{{0.0, 0.0, 0.01}};

  test_inverse_map(map, test_point1);
  test_inverse_map(map, test_point2);
  test_inverse_map(map, test_point3);
  test_inverse_map(map, test_point4);
}
}  // namespace

SPECTRE_TEST_CASE("Unit.Domain.CoordinateMaps.Frustum", "[Domain][Unit]") {
  test_frustum_fail();
  test_suite_for_frustum(false);  // Equidistant
  test_suite_for_frustum(true);   // Equiangular
  test_alignment();
  test_auto_projective_scale_factor();
  test_is_identity();
  test_bulged_frustum_jacobian();
  test_bulged_frustum_inv_jacobian();
  test_bulged_frustum_inv_map();
}

// [[OutputRegex, A projective scale factor of zero maps all coordinates to
// zero! Set projective_scale_factor to unity to turn off projective scaling.]]
[[noreturn]] SPECTRE_TEST_CASE("Unit.Domain.CoordinateMaps.Frustum.Assert0",
                               "[Domain][Unit]") {
  ASSERTION_TEST();
#ifdef SPECTRE_DEBUG
  const std::array<std::array<double, 2>, 4> face_vertices{
      {{{-2.0, -2.0}}, {{2.0, 2.0}}, {{-4.0, -4.0}}, {{4.0, 4.0}}}};
  const double lower_bound = 2.0;
  const double upper_bound = 5.0;
  const double projective_scale_factor = 0.0;
  const bool with_equiangular_map = false;

  auto failed_frustum = CoordinateMaps::Frustum(
      face_vertices, lower_bound, upper_bound, OrientationMap<3>{},
      with_equiangular_map, projective_scale_factor);
  static_cast<void>(failed_frustum);

  ERROR("Failed to trigger ASSERT in an assertion test");
#endif
}

// [[OutputRegex, The lower bound for a coordinate must be numerically less
// than the upper bound for that coordinate.]]
[[noreturn]] SPECTRE_TEST_CASE("Unit.Domain.CoordinateMaps.Frustum.Assert1",
                               "[Domain][Unit]") {
  ASSERTION_TEST();
#ifdef SPECTRE_DEBUG
  const std::array<std::array<double, 2>, 4> face_vertices{
      {{{-2.0, -2.0}}, {{-3.0, 2.0}}, {{-4.0, -4.0}}, {{4.0, 4.0}}}};
  const double lower_bound = 2.0;
  const double upper_bound = 5.0;

  auto failed_frustum = CoordinateMaps::Frustum(
      face_vertices, lower_bound, upper_bound, OrientationMap<3>{});
  static_cast<void>(failed_frustum);

  ERROR("Failed to trigger ASSERT in an assertion test");
#endif
}

// [[OutputRegex, The lower bound for a coordinate must be numerically less
// than the upper bound for that coordinate.]]
[[noreturn]] SPECTRE_TEST_CASE("Unit.Domain.CoordinateMaps.Frustum.Assert2",
                               "[Domain][Unit]") {
  ASSERTION_TEST();
#ifdef SPECTRE_DEBUG
  const std::array<std::array<double, 2>, 4> face_vertices{
      {{{-2.0, -2.0}}, {{2.0, -3.0}}, {{-4.0, -4.0}}, {{4.0, 4.0}}}};
  const double lower_bound = 2.0;
  const double upper_bound = 5.0;

  auto failed_frustum = CoordinateMaps::Frustum(
      face_vertices, lower_bound, upper_bound, OrientationMap<3>{});
  static_cast<void>(failed_frustum);

  ERROR("Failed to trigger ASSERT in an assertion test");
#endif
}

// [[OutputRegex, The lower bound for a coordinate must be numerically less
// than the upper bound for that coordinate.]]
[[noreturn]] SPECTRE_TEST_CASE("Unit.Domain.CoordinateMaps.Frustum.Assert3",
                               "[Domain][Unit]") {
  ASSERTION_TEST();
#ifdef SPECTRE_DEBUG
  const std::array<std::array<double, 2>, 4> face_vertices{
      {{{-2.0, -2.0}}, {{2.0, 2.0}}, {{-4.0, -4.0}}, {{-5.0, 4.0}}}};
  const double lower_bound = 2.0;
  const double upper_bound = 5.0;

  auto failed_frustum = CoordinateMaps::Frustum(
      face_vertices, lower_bound, upper_bound, OrientationMap<3>{});
  static_cast<void>(failed_frustum);

  ERROR("Failed to trigger ASSERT in an assertion test");
#endif
}

// [[OutputRegex, The lower bound for a coordinate must be numerically less
// than the upper bound for that coordinate.]]
[[noreturn]] SPECTRE_TEST_CASE("Unit.Domain.CoordinateMaps.Frustum.Assert4",
                               "[Domain][Unit]") {
  ASSERTION_TEST();
#ifdef SPECTRE_DEBUG
  const std::array<std::array<double, 2>, 4> face_vertices{
      {{{-2.0, -2.0}}, {{2.0, 2.0}}, {{-4.0, -4.0}}, {{4.0, -5.0}}}};
  const double lower_bound = 2.0;
  const double upper_bound = 5.0;

  auto failed_frustum = CoordinateMaps::Frustum(
      face_vertices, lower_bound, upper_bound, OrientationMap<3>{});
  static_cast<void>(failed_frustum);

  ERROR("Failed to trigger ASSERT in an assertion test");
#endif
}

// [[OutputRegex, The lower bound for a coordinate must be numerically less
// than the upper bound for that coordinate.]]
[[noreturn]] SPECTRE_TEST_CASE("Unit.Domain.CoordinateMaps.Frustum.Assert5",
                               "[Domain][Unit]") {
  ASSERTION_TEST();
#ifdef SPECTRE_DEBUG
  const std::array<std::array<double, 2>, 4> face_vertices{
      {{{-2.0, -2.0}}, {{2.0, 2.0}}, {{-4.0, -4.0}}, {{4.0, 4.0}}}};
  const double lower_bound = 2.0;
  const double upper_bound = -2.0;

  auto failed_frustum = CoordinateMaps::Frustum(
      face_vertices, lower_bound, upper_bound, OrientationMap<3>{});
  static_cast<void>(failed_frustum);

  ERROR("Failed to trigger ASSERT in an assertion test");
#endif
}

// [[OutputRegex, The sphericity must be set between 0.0, corresponding to a
// flat surface, and 1.0, corresponding to a spherical surface, inclusive. It is
// currently set to 1.3.]]
[[noreturn]] SPECTRE_TEST_CASE("Unit.Domain.CoordinateMaps.Frustum.Assert6",
                               "[Domain][Unit]") {
  ASSERTION_TEST();
#ifdef SPECTRE_DEBUG
  const std::array<std::array<double, 2>, 4> face_vertices{
      {{{-2.0, -2.0}}, {{2.0, 2.0}}, {{-4.0, -4.0}}, {{4.0, 4.0}}}};
  const double lower_bound = 2.0;
  const double upper_bound = 5.0;
  const double projective_scale_factor = 1.0;
  const bool with_equiangular_map = false;
  const double sphericity = 1.3;

  auto failed_frustum = CoordinateMaps::Frustum(
      face_vertices, lower_bound, upper_bound, OrientationMap<3>{},
      with_equiangular_map, projective_scale_factor, false, sphericity);
  static_cast<void>(failed_frustum);

  ERROR("Failed to trigger ASSERT in an assertion test");
#endif
}
}  // namespace domain
