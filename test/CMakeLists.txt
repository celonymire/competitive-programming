find_package(Python COMPONENTS Interpreter REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("../include")

function(add_library_test name)
    add_executable(${name}_program "program/${name}.cpp")
    add_executable(${name}_solution "solution/${name}.cpp")

    add_test(NAME ${name}
            COMMAND ${Python_EXECUTABLE} -m test.checker.${name} -p $<TARGET_FILE:${name}_program> -s $<TARGET_FILE:${name}_solution>
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )
endfunction()

# graph
add_library_test("bridges")
add_library_test("lowest_common_ancestor")
add_library_test("strongly_connected_components")
add_library_test("union_find")

# math
add_library_test("combinatorics")
add_library_test("matrix")

# range_query
add_library_test("fenwick")
add_library_test("heavy_light_decomposition")
add_library_test("mo_array")
add_library_test("segment_tree")
add_library_test("segment_tree_lazy")
add_library_test("sparse_table")
add_library_test("treap")

# string
add_library_test("aho_corasick")
add_library_test("rolling_hash")
