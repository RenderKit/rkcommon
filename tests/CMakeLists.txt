## Copyright 2009 Intel Corporation
## SPDX-License-Identifier: Apache-2.0

add_executable(rkcommon_test_suite
  ${RKCOMMON_RESOURCE}

  catch_main.cpp

  array3D/test_Array3D.cpp
  array3D/test_for_each.cpp

  math/test_AffineSpace.cpp
  math/test_box.cpp
  math/test_constants.cpp
  math/test_LinearSpace.cpp
  math/test_rkmath.cpp
  math/test_Quaternion.cpp
  math/test_range.cpp
  math/test_vec.cpp

  memory/test_DeletedUniquePtr.cpp
  memory/test_malloc.cpp
  memory/test_RefCount.cpp

  os/test_FileName.cpp
  os/test_library.cpp

  containers/test_AlignedVector.cpp
  containers/test_FlatMap.cpp
  containers/test_TransactionalBuffer.cpp

  tasking/test_async.cpp
  tasking/test_AsyncLoop.cpp
  tasking/test_AsyncTask.cpp
  tasking/test_parallel_for.cpp
  tasking/test_parallel_foreach.cpp
  tasking/test_schedule.cpp

  traits/test_traits.cpp

  utility/test_AbstractArray.cpp
  utility/test_Any.cpp
  utility/test_ArgumentList.cpp
  utility/test_ArrayView.cpp
  utility/test_CodeTimer.cpp
  utility/test_DataView.cpp
  utility/test_demangle.cpp
  utility/test_DoubleBufferedValue.cpp
  utility/test_getEnvVar.cpp
  utility/test_multidim_index_sequence.cpp
  utility/test_Observers.cpp
  utility/test_OnScopeExit.cpp
  utility/test_Optional.cpp
  utility/test_OwnedArray.cpp
  utility/test_ParameterizedObject.cpp
  utility/test_PseudoURL.cpp
  utility/test_random.cpp
  utility/test_SaveImage.cpp
  utility/test_StringManip.cpp
  utility/test_TimeStamp.cpp
  utility/test_TransactionalValue.cpp
)

target_link_libraries(rkcommon_test_suite PRIVATE rkcommon)

add_test(NAME ArgumentList          COMMAND rkcommon_test_suite "[ArgumentList]")
add_test(NAME ArrayView             COMMAND rkcommon_test_suite "[ArrayView]")
add_test(NAME OnScopeExit           COMMAND rkcommon_test_suite "[OnScopeExit]")
add_test(NAME Optional              COMMAND rkcommon_test_suite "[Optional]")
add_test(NAME FlatMap               COMMAND rkcommon_test_suite "[FlatMap]")
add_test(NAME TransactionalBuffer   COMMAND rkcommon_test_suite "[TransactionalBuffer]")
add_test(NAME StringManip           COMMAND rkcommon_test_suite "[StringManip]")
add_test(NAME random                COMMAND rkcommon_test_suite "[random]")
if(NOT WIN32) # Tests which are broken on Windows with unknown fixes (for now)
  add_test(NAME Any                 COMMAND rkcommon_test_suite "[Any]")
  add_test(NAME AlignedVector       COMMAND rkcommon_test_suite "[AlignedVector]")
  add_test(NAME Observers           COMMAND rkcommon_test_suite "[Observers]")
  add_test(NAME ParameterizedObject COMMAND rkcommon_test_suite "[ParameterizedObject]")
  add_test(NAME async               COMMAND rkcommon_test_suite "[async]")
  add_test(NAME parallel_for        COMMAND rkcommon_test_suite "[parallel_for]")
  add_test(NAME parallel_foreach    COMMAND rkcommon_test_suite "[parallel_foreach]")
  add_test(NAME schedule            COMMAND rkcommon_test_suite "[schedule]")
endif()

install(TARGETS rkcommon_test_suite
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
