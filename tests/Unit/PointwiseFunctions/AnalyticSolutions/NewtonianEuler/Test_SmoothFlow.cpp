// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "Framework/TestingFramework.hpp"

#include <array>
#include <cstddef>
#include <limits>
#include <string>
#include <tuple>
#include <utility>

#include "DataStructures/DataBox/Prefixes.hpp"
#include "DataStructures/DataVector.hpp"
#include "DataStructures/Tensor/TypeAliases.hpp"
#include "Evolution/Systems/NewtonianEuler/Tags.hpp"
#include "Framework/CheckWithRandomValues.hpp"
#include "Framework/SetupLocalPythonEnvironment.hpp"
#include "Framework/TestCreation.hpp"
#include "Framework/TestHelpers.hpp"
#include "Options/ParseOptions.hpp"
#include "PointwiseFunctions/AnalyticSolutions/NewtonianEuler/SmoothFlow.hpp"
#include "PointwiseFunctions/Hydro/Tags.hpp"
#include "Utilities/MakeWithValue.hpp"
#include "Utilities/StdArrayHelpers.hpp"
#include "Utilities/TMPL.hpp"
#include "Utilities/TaggedTuple.hpp"

namespace {

template <size_t Dim>
struct SmoothFlowProxy : NewtonianEuler::Solutions::SmoothFlow<Dim> {
  using NewtonianEuler::Solutions::SmoothFlow<Dim>::SmoothFlow;

  template <typename DataType>
  using variables_tags =
      tmpl::list<hydro::Tags::RestMassDensity<DataType>,
                 hydro::Tags::SpatialVelocity<DataType, Dim>,
                 hydro::Tags::SpecificInternalEnergy<DataType>,
                 hydro::Tags::Pressure<DataType>,
                 hydro::Tags::SpecificEnthalpy<DataType>>;

  template <typename DataType>
  using ne_variables_tags =
      tmpl::list<NewtonianEuler::Tags::MassDensity<DataType>,
                 NewtonianEuler::Tags::Velocity<DataType, Dim>,
                 NewtonianEuler::Tags::SpecificInternalEnergy<DataType>,
                 NewtonianEuler::Tags::Pressure<DataType>>;

  template <typename DataType>
  tuples::tagged_tuple_from_typelist<variables_tags<DataType>>
  primitive_variables(const tnsr::I<DataType, Dim>& x, const double t) const {
    return this->variables(x, t, variables_tags<DataType>{});
  }

  template <typename DataType>
  tuples::tagged_tuple_from_typelist<ne_variables_tags<DataType>>
  ne_primitive_variables(const tnsr::I<DataType, Dim>& x,
                         const double t) const {
    return this->variables(x, t, ne_variables_tags<DataType>{});
  }
};

template <size_t Dim, typename DataType>
void test_solution(const DataType& used_for_size,
                   const std::array<double, Dim>& mean_velocity,
                   const std::string& mean_velocity_opt,
                   const std::array<double, Dim>& wave_vector,
                   const std::string& wave_vector_opt) {
  const double pressure = 1.23;
  const double adiabatic_index = 1.3334;
  const double perturbation_size = 0.78;

  SmoothFlowProxy<Dim> solution(mean_velocity, wave_vector, pressure,
                                adiabatic_index, perturbation_size);
  pypp::check_with_random_values<1>(
      &SmoothFlowProxy<Dim>::template primitive_variables<DataType>, solution,
      "Hydro.SmoothFlow",
      {"rest_mass_density", "spatial_velocity", "specific_internal_energy",
       "pressure", "specific_enthalpy"},
      {{{-15., 15.}}},
      std::make_tuple(mean_velocity, wave_vector, pressure, adiabatic_index,
                      perturbation_size),
      used_for_size);
  pypp::check_with_random_values<1>(
      &SmoothFlowProxy<Dim>::template ne_primitive_variables<DataType>,
      solution, "Hydro.SmoothFlow",
      {"rest_mass_density", "spatial_velocity", "specific_internal_energy",
       "pressure"},
      {{{-15., 15.}}},
      std::make_tuple(mean_velocity, wave_vector, pressure, adiabatic_index,
                      perturbation_size),
      used_for_size);

  const auto solution_from_options =
      TestHelpers::test_creation<NewtonianEuler::Solutions::SmoothFlow<Dim>>(
          "MeanVelocity: " + mean_velocity_opt +
          "\n"
          "WaveVector: " +
          wave_vector_opt +
          "\n"
          "Pressure: 1.23\n"
          "AdiabaticIndex: 1.3334\n"
          "PerturbationSize: 0.78");
  CHECK(solution == solution_from_options);

  NewtonianEuler::Solutions::SmoothFlow<Dim> solution_to_move(
      mean_velocity, wave_vector, pressure, adiabatic_index, perturbation_size);

  test_move_semantics(std::move(solution_to_move),
                      solution_from_options);  //  NOLINT
  test_serialization(solution);
  test_copy_semantics(solution);
}
}  // namespace

SPECTRE_TEST_CASE(
    "Unit.PointwiseFunctions.AnalyticSolutions.NewtonianEuler.SmoothFlow",
    "[Unit][PointwiseFunctions]") {
  pypp::SetupLocalPythonEnvironment local_python_env{
      "PointwiseFunctions/AnalyticSolutions/"};
  test_solution<1>(std::numeric_limits<double>::signaling_NaN(), {{-0.3}},
                   "[-0.3]", {{0.4}}, "[0.4]");
  test_solution<1>(DataVector(5), {{-0.3}}, "[-0.3]", {{0.4}}, "[0.4]");
  test_solution<2>(std::numeric_limits<double>::signaling_NaN(), {{-0.3, 0.1}},
                   "[-0.3, 0.1]", {{0.4, -0.24}}, "[0.4, -0.24]");
  test_solution<2>(DataVector(5), {{-0.3, 0.1}}, "[-0.3, 0.1]", {{0.4, -0.24}},
                   "[0.4, -0.24]");
  test_solution<3>(std::numeric_limits<double>::signaling_NaN(),
                   {{-0.3, 0.1, -0.002}}, "[-0.3, 0.1, -0.002]",
                   {{0.4, -0.24, 0.054}}, "[0.4, -0.24, 0.054]");
  test_solution<3>(DataVector(5), {{-0.3, 0.1, -0.002}}, "[-0.3, 0.1, -0.002]",
                   {{0.4, -0.24, 0.054}}, "[0.4, -0.24, 0.054]");
}
