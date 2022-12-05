set(MODULE_NAME start)

message("compile ${MODULE_NAME}")

set(INTERNAL_FILES
  Start.cpp
)

set(PUBLIC_HEADERS
  Start.hpp
)

add_library(
  ${MODULE_NAME}
  ${INTERNAL_FILES}
  ${PUBLIC_HEADERS}
)

target_include_directories(
  ${MODULE_NAME}
  PRIVATE 
    ${MODULE_NAME}
    
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}
)
target_link_libraries(
  ${MODULE_NAME}
  PRIVATE
    lib::matrix
    lib::branchandbound
    lib::bruteforce
    lib::dynamicprogramming
)

add_library(lib::${MODULE_NAME} ALIAS ${MODULE_NAME})