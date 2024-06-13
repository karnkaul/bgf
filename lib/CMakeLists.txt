add_library(${PROJECT_NAME})
add_library(bave::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
  PUBLIC bave::bave
  PRIVATE bave::bave-compile-options
)

target_include_directories(${PROJECT_NAME} PUBLIC
  include
)

file(GLOB_RECURSE sources LIST_DIRECTORIES false CONFIGURE_DEPENDS "include/*.hpp" "src/*.[hc]pp")
target_sources(${PROJECT_NAME} PRIVATE
  ${sources}
)

if(BAVE_USE_PCH)
  target_precompile_headers(${PROJECT_NAME} REUSE_FROM bave)
endif()
