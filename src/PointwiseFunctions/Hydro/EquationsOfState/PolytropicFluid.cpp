// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "PointwiseFunctions/Hydro/EquationsOfState/PolytropicFluid.hpp"

#include <cmath>
#include <limits>

#include "DataStructures/DataVector.hpp"  // IWYU pragma: keep
#include "DataStructures/Tensor/Tensor.hpp"
#include "Utilities/MakeWithValue.hpp"

// IWYU pragma: no_forward_declare Tensor

namespace EquationsOfState {
template <bool IsRelativistic>
PolytropicFluid<IsRelativistic>::PolytropicFluid(
    const double polytropic_constant, const double polytropic_exponent)
    : polytropic_constant_(polytropic_constant),
      polytropic_exponent_(polytropic_exponent) {}

EQUATION_OF_STATE_MEMBER_DEFINITIONS(template <bool IsRelativistic>,
                                     PolytropicFluid<IsRelativistic>, double, 1)
EQUATION_OF_STATE_MEMBER_DEFINITIONS(template <bool IsRelativistic>,
                                     PolytropicFluid<IsRelativistic>,
                                     DataVector, 1)

template <bool IsRelativistic>
PolytropicFluid<IsRelativistic>::PolytropicFluid(CkMigrateMessage* /*unused*/) {
}

template <bool IsRelativistic>
void PolytropicFluid<IsRelativistic>::pup(PUP::er& p) {
  EquationOfState<IsRelativistic, 1>::pup(p);
  p | polytropic_constant_;
  p | polytropic_exponent_;
}

template <bool IsRelativistic>
template <class DataType>
Scalar<DataType> PolytropicFluid<IsRelativistic>::pressure_from_density_impl(
    const Scalar<DataType>& rest_mass_density) const {
  return Scalar<DataType>{polytropic_constant_ *
                          pow(get(rest_mass_density), polytropic_exponent_)};
}

template <>
template <class DataType>
Scalar<DataType> PolytropicFluid<true>::rest_mass_density_from_enthalpy_impl(
    const Scalar<DataType>& specific_enthalpy) const {
  return Scalar<DataType>{pow(((polytropic_exponent_ - 1.0) /
                               (polytropic_constant_ * polytropic_exponent_)) *
                                  (get(specific_enthalpy) - 1.0),
                              1.0 / (polytropic_exponent_ - 1.0))};
}

template <>
template <class DataType>
Scalar<DataType> PolytropicFluid<false>::rest_mass_density_from_enthalpy_impl(
    const Scalar<DataType>& specific_enthalpy) const {
  return Scalar<DataType>{pow(((polytropic_exponent_ - 1.0) /
                               (polytropic_constant_ * polytropic_exponent_)) *
                                  get(specific_enthalpy),
                              1.0 / (polytropic_exponent_ - 1.0))};
}

template <>
template <class DataType>
Scalar<DataType> PolytropicFluid<true>::specific_enthalpy_from_density_impl(
    const Scalar<DataType>& rest_mass_density) const {
  return Scalar<DataType>{
      1.0 + polytropic_exponent_ / (polytropic_exponent_ - 1.0) *
                polytropic_constant_ *
                pow(get(rest_mass_density), polytropic_exponent_ - 1.0)};
}

template <>
template <class DataType>
Scalar<DataType> PolytropicFluid<false>::specific_enthalpy_from_density_impl(
    const Scalar<DataType>& rest_mass_density) const {
  return Scalar<DataType>{
      polytropic_exponent_ / (polytropic_exponent_ - 1.0) *
      polytropic_constant_ *
      pow(get(rest_mass_density), polytropic_exponent_ - 1.0)};
}

template <bool IsRelativistic>
template <class DataType>
Scalar<DataType>
PolytropicFluid<IsRelativistic>::specific_internal_energy_from_density_impl(
    const Scalar<DataType>& rest_mass_density) const {
  return Scalar<DataType>{
      polytropic_constant_ / (polytropic_exponent_ - 1.0) *
      pow(get(rest_mass_density), polytropic_exponent_ - 1.0)};
}

template <bool IsRelativistic>
template <class DataType>
Scalar<DataType> PolytropicFluid<IsRelativistic>::chi_from_density_impl(
    const Scalar<DataType>& rest_mass_density) const {
  return Scalar<DataType>{
      polytropic_constant_ * polytropic_exponent_ *
      pow(get(rest_mass_density), polytropic_exponent_ - 1.0)};
}

template <bool IsRelativistic>
template <class DataType>
Scalar<DataType> PolytropicFluid<IsRelativistic>::
    kappa_times_p_over_rho_squared_from_density_impl(
        const Scalar<DataType>& rest_mass_density) const {
  return make_with_value<Scalar<DataType>>(get(rest_mass_density), 0.0);
}

template <bool IsRelativistic>
double PolytropicFluid<IsRelativistic>::rest_mass_density_upper_bound() const {
  // this bound comes from the dominant energy condition which implies
  // that the pressure is bounded by the total energy density,
  // i.e. p < e = rho * (1 + eps)
  if (IsRelativistic and polytropic_exponent_ > 2.0) {
    return pow((polytropic_exponent_ - 1.0) /
                   (polytropic_constant_ * (polytropic_exponent_ - 2.0)),
               1.0 / (polytropic_exponent_ - 1.0));
  }
  return std::numeric_limits<double>::max();
}
}  // namespace EquationsOfState

template class EquationsOfState::PolytropicFluid<true>;
template class EquationsOfState::PolytropicFluid<false>;
