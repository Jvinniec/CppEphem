
#------------------------------------------
# Define include directories
#------------------------------------------
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include
                    ${CMAKE_SOURCE_DIR}/test/include
                    )

# Define the tests
cppephem_test(test_CEBody        test_CEBody.cpp)
cppephem_test(test_CECoordinates test_CECoordinates.cpp)
cppephem_test(test_CEDate        test_CEDate.cpp)
cppephem_test(test_CEException   test_CEException.cpp)
cppephem_test(test_CENamespace   test_CENamespace.cpp)
cppephem_test(test_CEObservation test_CEObservation.cpp)
cppephem_test(test_CEObserver    test_CEObserver.cpp)
cppephem_test(test_CEPlanet      test_CEPlanet.cpp)
cppephem_test(test_CERunningDate test_CERunningDate.cpp)
cppephem_test(test_CETime        test_CETime.cpp)
