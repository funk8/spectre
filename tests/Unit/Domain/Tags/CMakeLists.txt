# Distributed under the MIT License.
# See LICENSE.txt for details.

set(LIBRARY "Test_DomainTags")

set(LIBRARY_SOURCES
  Test_BlockNamesAndGroups.cpp
  Test_Faces.cpp
  Test_SurfaceJacobian.cpp
  )

add_test_library(
  ${LIBRARY}
  "Domain/Tags"
  "${LIBRARY_SOURCES}"
  ""
  )

target_link_libraries(
  ${LIBRARY}
  PRIVATE
  DataStructures
  Domain
  DomainHelpers
  DomainStructure
  Utilities
  )
