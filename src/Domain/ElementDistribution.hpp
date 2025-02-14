// Distributed under the MIT License.
// See LICENSE.txt for details.

#pragma once

#include <array>
#include <cstddef>
#include <utility>
#include <vector>

#include "Domain/Structure/ElementId.hpp"

namespace domain {

/*!
 * \brief Distribution strategy for assigning elements to CPUs using a
 * Morton ('Z-order') space-filling curve to determine placement within each
 * block.
 *
 * \details The element distribution assigns a balanced number of elements to
 * each processor. This distribution is computed by first greedily assigning to
 * each processor an allowance of
 * [total number of elements]/[number of processors] elements from one or more
 * blocks, starting with the lowest number block that still has elements to
 * contribute to an allowance.
 * Then, once those allowances are determined, a separate Z-order curve is
 * established for each block and the elements are assigned to processors within
 * each block by greedily filling each processors' allowance by contiguous
 * intervals along the Z-order curve.
 * Some examples:
 * - If there are 8 blocks, 16 elements per block, and 16 cores: each core gets
 * an allowance of 128 / 16 = 8 elements, so each core gets half of a block, and
 * the 8 elements for each core within the block are chosen via Z-order curve
 * for the respective blocks.
 * - If there are 3 blocks, 4 elements per block, and 4 cores: each core gets
 * an allowance of 12 / 4 = 3 elements. The first core gets three elements from
 * the first block, the second gets one element from the first block and two
 * elements from the second block, the third gets two elements from the second
 * block and one from the third, and the fourth core gets the remaining three
 * elements from the third block. Each collection of elements within the blocks
 * are then assigned using intervals along the Z-order curve for each block.
 *
 * Morton curves are a simple and easily-computed space-filling curve that
 * (unlike Hilbert curves) permit diagonal traversal. See, for instance,
 * \cite Borrell2018 for a discussion of mesh partitioning using space-filling
 * curves.
 * A concrete example of the use of a Morton curve in 2d is given below.
 *
 * A sketch of a 2D block with 4x2 elements, with each element labeled according
 * to the order on the Morton curve:
 * ```
 *          x-->
 *          0   1   2   3
 *        ----------------
 *  y  0 |  0   2   4   6
 *  |    |  | / | / | / |
 *  v  1 |  1   3   5   7
 * ```
 * (forming a zig-zag path, that under some rotation/reflection has a 'Z'
 * shape).
 *
 * The Morton curve method is a quick way of getting acceptable spatial locality
 * -- usually, for approximately even distributions, it will ensure that
 * elements are assigned in large volume chunks, and the structure of the Morton
 * curve ensures that for a given processor and block, the elements will be
 * assigned in no more than two orthogonally connected clusters. In principle, a
 * Hilbert curve could potentially improve upon the gains obtained by this class
 * by guaranteeing that all elements within each block form a single orthognally
 * connected cluster.
 *
 * The assignment of portions of blocks to processors may use partial blocks,
 * and/or multiple blocks to ensure an even distribution of elements to
 * processors.
 * We currently make no distinction between dividing elements between processors
 * within a node and dividing elements between processors across nodes. The
 * current technique aims to have a simple method of reducing communication
 * globally, though it would likely be more efficient to prioritize minimization
 * of inter-node communication, because communication across interconnects is
 * the primary cost of communication in charm++ runs.
 *
 * \warning The use of the Morton curve to generate a well-clustered element
 * distribution currently assumes that the refinement is uniform over each
 * block, with no internal structure that would be generated by, for instance
 * AMR.
 * This distribution method will need alteration to perform well for blocks with
 * internal structure from h-refinement. Morton curves can be defined
 * recursively, so a generalization of the present method is possible for blocks
 * with internal refinement
 */
template <size_t Dim>
struct BlockZCurveProcDistribution {
  BlockZCurveProcDistribution(
      size_t number_of_procs,
      const std::vector<std::array<size_t, Dim>>& refinements_by_block);

  /// Gets the suggested processor number for a particular element,
  /// determined by the greedy block assignment and Morton curve element
  /// assignment described in detail in the parent class documentation.
  size_t get_proc_for_element(const ElementId<Dim>& element_id) const;

 private:
  // in this nested data structure:
  // - The block id is the first index
  // - There is an arbitrary number of CPUs per block, each with an element
  //   allowance
  // - Each element allowance is represented by a pair of proc number, number of
  //   elements in the allowance
  std::vector<std::vector<std::pair<size_t, size_t>>>
      block_element_distribution_;
};
}  // namespace domain
