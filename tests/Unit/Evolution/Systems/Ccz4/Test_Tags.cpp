// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "Framework/TestingFramework.hpp"

#include <cstddef>
#include <string>

#include "DataStructures/DataVector.hpp"
#include "Evolution/Systems/Ccz4/Tags.hpp"
#include "Helpers/DataStructures/DataBox/TestHelpers.hpp"

namespace {
struct ArbitraryFrame;
}  // namespace

template <size_t Dim, typename Frame, typename DataType>
void test_simple_tags() {
  TestHelpers::db::test_simple_tag<Ccz4::Tags::ConformalFactor<DataType>>(
      "ConformalFactor");
  TestHelpers::db::test_simple_tag<
      Ccz4::Tags::ConformalFactorSquared<DataType>>("ConformalFactorSquared");
  TestHelpers::db::test_simple_tag<
      Ccz4::Tags::ConformalMetric<Dim, Frame, DataType>>(
      "Conformal(SpatialMetric)");
  TestHelpers::db::test_simple_tag<
      Ccz4::Tags::InverseConformalMetric<Dim, Frame, DataType>>(
      "Conformal(InverseSpatialMetric)");
  TestHelpers::db::test_simple_tag<Ccz4::Tags::ATilde<Dim, Frame, DataType>>(
      "ATilde");
  TestHelpers::db::test_simple_tag<Ccz4::Tags::TraceATilde<DataType>>(
      "TraceATilde");
  TestHelpers::db::test_simple_tag<Ccz4::Tags::LogLapse<DataType>>("LogLapse");
  TestHelpers::db::test_simple_tag<Ccz4::Tags::FieldA<Dim, Frame, DataType>>(
      "FieldA");
  TestHelpers::db::test_simple_tag<
      Ccz4::Tags::FieldB<Dim, Frame, DataType>>("FieldB");
  TestHelpers::db::test_simple_tag<Ccz4::Tags::FieldD<Dim, Frame, DataType>>(
      "FieldD");
  TestHelpers::db::test_simple_tag<Ccz4::Tags::LogConformalFactor<DataType>>(
      "LogConformalFactor");
  TestHelpers::db::test_simple_tag<Ccz4::Tags::FieldP<Dim, Frame, DataType>>(
      "FieldP");
  TestHelpers::db::test_simple_tag<Ccz4::Tags::FieldDUp<Dim, Frame, DataType>>(
      "FieldDUp");
  TestHelpers::db::test_simple_tag<
      Ccz4::Tags::ConformalChristoffelSecondKind<Dim, Frame, DataType>>(
      "ConformalChristoffelSecondKind");
  TestHelpers::db::test_simple_tag<
      Ccz4::Tags::DerivConformalChristoffelSecondKind<Dim, Frame, DataType>>(
      "DerivConformalChristoffelSecondKind");
  TestHelpers::db::test_simple_tag<
      Ccz4::Tags::ChristoffelSecondKind<Dim, Frame, DataType>>(
      "ChristoffelSecondKind");
  TestHelpers::db::test_simple_tag<Ccz4::Tags::Ricci<Dim, Frame, DataType>>(
      "Ricci");
  TestHelpers::db::test_simple_tag<
      Ccz4::Tags::GradGradLapse<Dim, Frame, DataType>>("GradGradLapse");
  TestHelpers::db::test_simple_tag<Ccz4::Tags::DivergenceLapse<DataType>>(
      "DivergenceLapse");
  TestHelpers::db::test_simple_tag<
      Ccz4::Tags::ContractedConformalChristoffelSecondKind<Dim, Frame,
                                                           DataType>>(
      "ContractedConformalChristoffelSecondKind");
  TestHelpers::db::test_simple_tag<
      Ccz4::Tags::DerivContractedConformalChristoffelSecondKind<Dim, Frame,
                                                                DataType>>(
      "DerivContractedConformalChristoffelSecondKind");
  TestHelpers::db::test_simple_tag<Ccz4::Tags::GammaHat<Dim, Frame, DataType>>(
      "GammaHat");
  TestHelpers::db::test_simple_tag<
      Ccz4::Tags::SpatialZ4Constraint<Dim, Frame, DataType>>(
      "SpatialZ4Constraint");
  TestHelpers::db::test_simple_tag<
      Ccz4::Tags::SpatialZ4ConstraintUp<Dim, Frame, DataType>>(
      "SpatialZ4ConstraintUp");
  TestHelpers::db::test_simple_tag<
      Ccz4::Tags::GradSpatialZ4Constraint<Dim, Frame, DataType>>(
      "GradSpatialZ4Constraint");
  TestHelpers::db::test_simple_tag<
      Ccz4::Tags::RicciScalarPlusDivergenceZ4Constraint<DataType>>(
      "RicciScalarPlusDivergenceZ4Constraint");
}

SPECTRE_TEST_CASE("Unit.Evolution.Systems.Ccz4.Tags", "[Unit][Evolution]") {
  test_simple_tags<1, ArbitraryFrame, double>();
  test_simple_tags<1, ArbitraryFrame, DataVector>();
  test_simple_tags<2, ArbitraryFrame, double>();
  test_simple_tags<2, ArbitraryFrame, DataVector>();
  test_simple_tags<3, ArbitraryFrame, double>();
  test_simple_tags<3, ArbitraryFrame, DataVector>();
}
