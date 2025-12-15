add_includedirs("../include")
set_languages("c++17", {extensions = false})

function library_target(name)
    target(name .. "_program")
        set_kind("binary")
        set_default(false)
        add_files("program/" .. name .. ".cpp")
    target(name .. "_solution")
        set_kind("binary")
        set_default(false)
        add_files("solution/" .. name .. ".cpp")
    target(name)
        set_kind("phony")
        add_deps(name .. "_program", name .. "_solution")
        add_tests("default")
        on_test(function(target, opts)
            local program_target = target:dep(name .. "_program")
            local solution_target = target:dep(name .. "_solution")
            os.cd(os.projectdir())
            os.execv("python", {"-m", "test.checker." .. name, "-p", program_target:targetfile(), "-s", solution_target:targetfile()})
            return true
        end)
end

-- graph
library_target("bridges")
library_target("lowest_common_ancestor")
library_target("strongly_connected_components")
library_target("union_find")

-- math
library_target("combinatorics")
library_target("matrix")

-- range_query
library_target("fenwick")
library_target("heavy_light_decomposition")
library_target("mo_array")
library_target("segment_tree")
library_target("segment_tree_lazy")
library_target("sparse_table")
library_target("treap")

-- string
library_target("aho_corasick")
library_target("rolling_hash")

