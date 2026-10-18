cmake_minimum_required(VERSION 3.14)

project(deriv
    VERSION 1.0.0
    DESCRIPTION "Derivatives pricing in C++: binomial trees, Black-Scholes, Monte Carlo"
    LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

option(DERIV_BUILD_EXAMPLES "Build the example programs" ON)
option(DERIV_BUILD_TESTS "Build the regression tests" ON)

# ---------------------------------------------------------------- library

add_library(deriv
    src/numerics/DefiniteIntegral.cpp
    src/numerics/Matrix.cpp
    src/numerics/NonlinearSolver.cpp
    src/numerics/Random.cpp
    src/binomial/BinomialPricer.cpp
    src/binomial/BinomialTreeModel.cpp
    src/binomial/CRRFormula.cpp
    src/binomial/DigitalOption.cpp
    src/binomial/Option.cpp
    src/binomial/SpreadOption.cpp
    src/analytic/BlackScholesCall.cpp
    src/analytic/GeneralizedBlackScholes.cpp
    src/fixedincome/BondYield.cpp
    src/montecarlo/AsianOptions.cpp
    src/montecarlo/GBMModel.cpp
    src/montecarlo/PathDepOption.cpp
    src/montecarlo/basket/GBMModel.cpp
    src/montecarlo/basket/PathDepOption.cpp
)
add_library(deriv::deriv ALIAS deriv)

target_include_directories(deriv PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
target_compile_features(deriv PUBLIC cxx_std_11)
set_target_properties(deriv PROPERTIES CXX_EXTENSIONS OFF)

function(deriv_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -pedantic)
    endif()
endfunction()

deriv_warnings(deriv)

# ---------------------------------------------------------------- examples

if(DERIV_BUILD_EXAMPLES)
    set(DERIV_EXAMPLES
        price_every_way
        european_binomial
        replicating_portfolio
        american_options
        binomial_convergence
        black_scholes
        implied_volatility
        bond_yield
        asian_monte_carlo
        basket_option
        numerical_integration
    )
    foreach(example IN LISTS DERIV_EXAMPLES)
        add_executable(${example} examples/${example}.cpp)
        target_link_libraries(${example} PRIVATE deriv::deriv)
        deriv_warnings(${example})
        set_target_properties(${example} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples)
    endforeach()
endif()

# ---------------------------------------------------------------- tests

if(DERIV_BUILD_TESTS)
    enable_testing()
    add_executable(deriv_tests tests/deriv_tests.cpp)
    target_link_libraries(deriv_tests PRIVATE deriv::deriv)
    deriv_warnings(deriv_tests)
    add_test(NAME regression COMMAND deriv_tests)

    # every example must run to completion
    if(DERIV_BUILD_EXAMPLES)
        foreach(example IN LISTS DERIV_EXAMPLES)
            add_test(NAME example.${example} COMMAND ${example})
        endforeach()
    endif()
endif()

# ---------------------------------------------------------------- install

include(GNUInstallDirs)
install(TARGETS deriv EXPORT derivTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT derivTargets NAMESPACE deriv:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/deriv)
