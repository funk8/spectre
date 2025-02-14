// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "Framework/TestingFramework.hpp"

#include <algorithm>
#include <string>

#include "DataStructures/DataBox/DataBox.hpp"
#include "DataStructures/DataBox/Tag.hpp"
#include "ParallelAlgorithms/Initialization/MergeIntoDataBox.hpp"
#include "Utilities/Gsl.hpp"
#include "Utilities/TMPL.hpp"
#include "Utilities/TaggedTuple.hpp"

namespace TestAddToBox_detail {
struct NoEquivalence {
  int value = 0;
};

namespace Tags {
struct Int : db::SimpleTag {
  using type = int;
};

struct NoEquivalence : db::SimpleTag {
  using type = TestAddToBox_detail::NoEquivalence;
};

template <typename Tag>
struct MultiplyByTwo : db::SimpleTag {
  using type = int;
};

template <typename Tag>
struct MultiplyByTwoCompute : MultiplyByTwo<Tag>, db::ComputeTag {
  static void function(const gsl::not_null<int*> result, const int& t) {
    *result = t * 2;
  }
  static void function(const gsl::not_null<int*> result,
                       const TestAddToBox_detail::NoEquivalence& t) {
    *result = t.value * 2;
  }
  using argument_tags = tmpl::list<Tag>;
  using return_type = int;
  using base = MultiplyByTwo<Tag>;
};
}  // namespace Tags

struct FakeAction {};

void test() {
  auto box0 = Initialization::merge_into_databox<FakeAction,
                                                 db::AddSimpleTags<Tags::Int>>(
      db::create<db::AddSimpleTags<>>(), 2);
  CHECK(db::get<Tags::Int>(box0) == 2);
  auto box1 = Initialization::merge_into_databox<FakeAction,
                                                 db::AddSimpleTags<Tags::Int>>(
      std::move(box0), 2);
  CHECK(db::get<Tags::Int>(box1) == 2);
  auto box2 = Initialization::merge_into_databox<
      FakeAction, db::AddSimpleTags<Tags::NoEquivalence>>(std::move(box1),
                                                          NoEquivalence{5});
  CHECK(db::get<Tags::NoEquivalence>(box2).value == 5);

  // Make sure that for tags that don't have an equivalence operator we don't
  // overwrite the value.
  auto box3 = Initialization::merge_into_databox<
      FakeAction, db::AddSimpleTags<Tags::NoEquivalence>, db::AddComputeTags<>,
      Initialization::MergePolicy::IgnoreIncomparable>(std::move(box2),
                                                       NoEquivalence{7});
  CHECK(db::get<Tags::NoEquivalence>(box3).value == 5);

  // Check that we can overwrite tags that have an equivalence operator
  auto box4 = Initialization::merge_into_databox<
      FakeAction, db::AddSimpleTags<Tags::Int>, db::AddComputeTags<>,
      Initialization::MergePolicy::Overwrite>(std::move(box3), 4);
  CHECK(db::get<Tags::Int>(box4) == 4);

  auto box5 = Initialization::merge_into_databox<
      FakeAction, db::AddSimpleTags<Tags::NoEquivalence>, db::AddComputeTags<>,
      Initialization::MergePolicy::Overwrite>(
      std::move(box4), TestAddToBox_detail::NoEquivalence{7});
  CHECK(db::get<Tags::NoEquivalence>(box5).value == 7);

  // Check that adding nothing works
  auto box6 =
      Initialization::merge_into_databox<FakeAction, db::AddSimpleTags<>>(
          std::move(box5));
  CHECK(db::get<Tags::Int>(box6) == 4);
  CHECK(db::get<Tags::NoEquivalence>(box6).value == 7);

  // Now test that compute items are not re-added, and that they are properly
  // reset if a simple tag is mutated.
  auto box7 = Initialization::merge_into_databox<
      FakeAction, db::AddSimpleTags<>,
      db::AddComputeTags<Tags::MultiplyByTwoCompute<Tags::Int>>>(
      std::move(box6));
  CHECK(db::get<Tags::MultiplyByTwo<Tags::Int>>(box7) == 8);

  auto box8 = Initialization::merge_into_databox<
      FakeAction, db::AddSimpleTags<>,
      db::AddComputeTags<Tags::MultiplyByTwoCompute<Tags::Int>,
                         Tags::MultiplyByTwoCompute<Tags::NoEquivalence>>>(
      std::move(box7));
  CHECK(db::get<Tags::MultiplyByTwo<Tags::NoEquivalence>>(box8) == 14);

  // Now swap out the value of Tags::Int
  auto box9 = Initialization::merge_into_databox<
      FakeAction, db::AddSimpleTags<Tags::Int>,
      db::AddComputeTags<Tags::MultiplyByTwoCompute<Tags::Int>,
                         Tags::MultiplyByTwoCompute<Tags::NoEquivalence>>,
      Initialization::MergePolicy::Overwrite>(std::move(box8), 10);
  CHECK(db::get<Tags::MultiplyByTwo<Tags::Int>>(box9) == 20);
}

void test_failure_value() {
  auto box0 = Initialization::merge_into_databox<FakeAction,
                                                 db::AddSimpleTags<Tags::Int>>(
      db::create<db::AddSimpleTags<>>(), 2);
  CHECK(db::get<Tags::Int>(box0) == 2);
  // This merge should give an error since they're different values
  auto box1 = Initialization::merge_into_databox<FakeAction,
                                                 db::AddSimpleTags<Tags::Int>>(
      std::move(box0), 3);
  (void)box1;
}
}  // namespace TestAddToBox_detail

SPECTRE_TEST_CASE("Unit.ParallelAlgorithms.Initialization.AddToDataBox",
                  "[Unit][ParallelAlgorithms]") {
  TestAddToBox_detail::test();
}

// [[OutputRegex, While adding the simple tag Int that is already in the DataBox
// we found that the value being set by the action
// TestAddToBox_detail::FakeAction is not the same as what is already in the
// DataBox. The value in the DataBox is: 2 while the value being added is 3]]
SPECTRE_TEST_CASE("Unit.ParallelAlgorithms.Initialization.AddToDataBox.Error",
                  "[Unit][ParallelAlgorithms]") {
  ERROR_TEST();
  TestAddToBox_detail::test_failure_value();
}
