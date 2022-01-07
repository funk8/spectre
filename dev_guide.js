var dev_guide =
[
    [ "Build System", "spectre_build_system.html", [
      [ "Developing and Improving Executables", "dev_guide.html#autotoc_md64", null ],
      [ "Having your Contributions Merged into SpECTRE", "dev_guide.html#autotoc_md65", null ],
      [ "General SpECTRE Terminology", "dev_guide.html#autotoc_md66", null ],
      [ "Charm++ Interface", "dev_guide.html#autotoc_md67", null ],
      [ "Template Metaprogramming (TMP)", "dev_guide.html#autotoc_md68", null ],
      [ "Foundational Concepts in SpECTRE", "dev_guide.html#autotoc_md69", null ],
      [ "Technical Documentation for Fluent Developers", "dev_guide.html#autotoc_md70", null ],
      [ "CoordinateMap Guide", "dev_guide.html#autotoc_md71", null ],
      [ "Continuous Integration", "dev_guide.html#autotoc_md72", null ],
      [ "CMake", "spectre_build_system.html#cmake", [
        [ "Adding Source Files", "spectre_build_system.html#adding_source_files", [
          [ "Adding Libraries", "spectre_build_system.html#adding_libraries", null ]
        ] ],
        [ "Adding Unit Tests", "spectre_build_system.html#adding_unit_tests", null ],
        [ "Adding Executables", "spectre_build_system.html#adding_executables", null ],
        [ "Adding External Dependencies", "spectre_build_system.html#adding_external_dependencies", null ],
        [ "Commonly Used CMake flags", "spectre_build_system.html#common_cmake_flags", null ],
        [ "Checking Dependencies", "spectre_build_system.html#autotoc_md57", null ],
        [ "Formaline", "spectre_build_system.html#autotoc_md58", null ]
      ] ]
    ] ],
    [ "Creating Executables", "dev_guide_creating_executables.html", null ],
    [ "Writing SpECTRE executables", "tutorials_parallel.html", "tutorials_parallel" ],
    [ "Option Parsing", "dev_guide_option_parsing.html", [
      [ "General option format", "dev_guide_option_parsing.html#autotoc_md91", null ],
      [ "Constructible classes", "dev_guide_option_parsing.html#autotoc_md92", null ],
      [ "Factory", "dev_guide_option_parsing.html#autotoc_md93", null ],
      [ "Custom parsing", "dev_guide_option_parsing.html#autotoc_md94", null ]
    ] ],
    [ "Importing data", "dev_guide_importing.html", [
      [ "Importing volume data", "dev_guide_importing.html#autotoc_md80", null ]
    ] ],
    [ "Profiling With Charm++ Projections", "profiling_with_projections.html", [
      [ "Basic Setup and Compilation", "profiling_with_projections.html#autotoc_md112", null ],
      [ "Using PAPI With Projections", "profiling_with_projections.html#autotoc_md113", null ],
      [ "Running SpECTRE With Trace Output", "profiling_with_projections.html#autotoc_md114", null ],
      [ "Visualizing Trace %Data In Projections", "profiling_with_projections.html#autotoc_md115", null ]
    ] ],
    [ "Writing Python Bindings", "spectre_writing_python_bindings.html", [
      [ "CMake and Directory Layout", "spectre_writing_python_bindings.html#autotoc_md116", null ],
      [ "Writing Bindings", "spectre_writing_python_bindings.html#autotoc_md117", null ],
      [ "Testing Python Bindings and Code", "spectre_writing_python_bindings.html#autotoc_md118", null ],
      [ "Using The Bindings", "spectre_writing_python_bindings.html#autotoc_md119", null ],
      [ "Notes:", "spectre_writing_python_bindings.html#autotoc_md120", null ]
    ] ],
    [ "Implementing SpECTRE vectors", "implementing_vectors.html", [
      [ "Overview of SpECTRE Vectors", "implementing_vectors.html#general_structure", null ],
      [ "The class definition", "implementing_vectors.html#class_definition", null ],
      [ "Allowed operator specification", "implementing_vectors.html#blaze_definitions", null ],
      [ "Supporting operations for <tt>std::array</tt>s of vectors", "implementing_vectors.html#array_vector_definitions", null ],
      [ "Equivalence operators", "implementing_vectors.html#Vector_type_equivalence", null ],
      [ "MakeWithValueImpl", "implementing_vectors.html#Vector_MakeWithValueImpl", null ],
      [ "Interoperability with other data types", "implementing_vectors.html#Vector_tensor_and_variables", null ],
      [ "Writing tests", "implementing_vectors.html#Vector_tests", [
        [ "Utility check functions", "implementing_vectors.html#autotoc_md73", [
          [ "<tt>TestHelpers::VectorImpl::vector_test_construct_and_assign()</tt>", "implementing_vectors.html#autotoc_md74", null ],
          [ "<tt>TestHelpers::VectorImpl::vector_test_serialize()</tt>", "implementing_vectors.html#autotoc_md75", null ],
          [ "<tt>TestHelpers::VectorImpl::vector_test_ref()</tt>", "implementing_vectors.html#autotoc_md76", null ],
          [ "<tt>TestHelpers::VectorImpl::vector_test_math_after_move()</tt>", "implementing_vectors.html#autotoc_md77", null ],
          [ "<tt>TestHelpers::VectorImpl::vector_ref_test_size_error()</tt>", "implementing_vectors.html#autotoc_md78", null ]
        ] ],
        [ "<tt>TestHelpers::VectorImpl::test_functions_with_vector_arguments()</tt>", "implementing_vectors.html#autotoc_md79", null ]
      ] ],
      [ "Vector storage nuts and bolts", "implementing_vectors.html#Vector_storage", null ]
    ] ],
    [ "Understanding Compiler and Linker Errors", "compiler_and_linker_errors.html", [
      [ "Linker Errors", "compiler_and_linker_errors.html#understanding_linker_errors", null ]
    ] ],
    [ "Static Analysis Tools", "static_analysis_tools.html", null ],
    [ "Build Profiling and Optimization", "build_profiling_and_optimization.html", [
      [ "Why is our build so expensive?", "build_profiling_and_optimization.html#autotoc_md51", null ],
      [ "Understanding template expenses", "build_profiling_and_optimization.html#autotoc_md52", null ],
      [ "Profiling the build", "build_profiling_and_optimization.html#autotoc_md53", [
        [ "Specialized tests and feature exploration", "build_profiling_and_optimization.html#autotoc_md54", null ],
        [ "Templight++", "build_profiling_and_optimization.html#autotoc_md55", null ],
        [ "Clang AST syntax generation", "build_profiling_and_optimization.html#autotoc_md56", null ]
      ] ]
    ] ],
    [ "Writing Good Documentation", "writing_good_dox.html", [
      [ "Tutorials, Instructions, and Dev Guide", "writing_good_dox.html#writing_dox_writing_help", null ],
      [ "C++ Documentation", "writing_good_dox.html#writing_dox_cpp_dox_help", [
        [ "Add your object to an existing Module:", "writing_good_dox.html#autotoc_md138", null ],
        [ "Add a new Module:", "writing_good_dox.html#autotoc_md139", null ],
        [ "Add a new namespace:", "writing_good_dox.html#autotoc_md140", null ],
        [ "Put any mathematical expressions in your documentation into LaTeX form:", "writing_good_dox.html#autotoc_md141", null ],
        [ "Cite publications in your documentation", "writing_good_dox.html#writing_dox_citations", null ],
        [ "Include any pictures that aid in the understanding of your documentation:", "writing_good_dox.html#autotoc_md142", null ]
      ] ],
      [ "Python Documentation", "writing_good_dox.html#writing_dox_python_dox_help", null ]
    ] ],
    [ "Writing Unit Tests", "writing_unit_tests.html", null ],
    [ "Travis CI", "travis_guide.html", [
      [ "Testing SpECTRE with Travis CI", "travis_guide.html#travis_ci", [
        [ "What is tested", "travis_guide.html#what-is-tested", null ],
        [ "How to perform the checks locally", "travis_guide.html#perform-checks-locally", null ],
        [ "Travis setup", "travis_guide.html#travis-setup", null ],
        [ "Troubleshooting", "travis_guide.html#travis-troubleshooting", null ],
        [ "Precompiled Headers and ccache", "travis_guide.html#precompiled-headers-ccache", null ],
        [ "Build Stages", "travis_guide.html#travis-build-stages", null ],
        [ "Caching Dependencies on macOS Builds", "travis_guide.html#caching-mac-os", null ]
      ] ]
    ] ],
    [ "Code Review Guide", "code_review_guide.html", null ],
    [ "Domain Concepts", "domain_concepts.html", null ],
    [ "Notes on SpECTRE load-balancing using Charm++'s built-in load balancers", "load_balancing_notes.html", null ],
    [ "SFINAE", "sfinae.html", null ],
    [ "Motivation for SpECTRE's DataBox", "databox_foundations.html", [
      [ "Introduction", "databox_foundations.html#databox_introduction", null ],
      [ "Towards SpECTRE's DataBox", "databox_foundations.html#databox_towards_spectres_databox", [
        [ "Working without DataBoxes", "databox_foundations.html#databox_working_without_databoxes", null ],
        [ "A std::map DataBox", "databox_foundations.html#databox_a_std_map_databox", null ],
        [ "A std::tuple DataBox", "databox_foundations.html#databox_a_std_tuple_databox", null ],
        [ "A TaggedTuple DataBox", "databox_foundations.html#databox_a_taggedtuple_databox", null ]
      ] ],
      [ "SpECTRE's DataBox", "databox_foundations.html#databox_a_proper_databox", [
        [ "SimpleTags", "databox_foundations.html#databox_documentation_for_simple_tags", null ],
        [ "ComputeTags", "databox_foundations.html#databox_documentation_for_compute_tags", null ],
        [ "Mutating DataBox items", "databox_foundations.html#databox_documentation_for_mutate_tags", null ]
      ] ],
      [ "Toward SpECTRE's Actions", "databox_foundations.html#databox_towards_actions", [
        [ "Mutators", "databox_foundations.html#databox_documentation_for_mutators", null ]
      ] ]
    ] ],
    [ "Protocols", "protocols.html", [
      [ "Overview of protocols", "protocols.html#protocols_overview", null ],
      [ "Protocol users: Conforming to a protocol", "protocols.html#protocols_conforming", null ],
      [ "Protocol authors: Writing a protocol", "protocols.html#protocols_author", null ],
      [ "Protocol authors: Testing a protocol", "protocols.html#protocols_testing", null ],
      [ "Protocols and C++20 \"Constraints and concepts\"", "protocols.html#protocols_and_constraints", null ]
    ] ],
    [ "Observers Infrastructure", "observers_infrastructure_dev_guide.html", null ],
    [ "Parallelization, Charm++, and Core Concepts", "dev_guide_parallelization_foundations.html", [
      [ "Introduction", "dev_guide_parallelization_foundations.html#dev_guide_parallelization_introduction", null ],
      [ "The Metavariables Class", "dev_guide_parallelization_foundations.html#dev_guide_parallelization_metavariables_class", null ],
      [ "Phases of an Execution", "dev_guide_parallelization_foundations.html#dev_guide_parallelization_phases_of_execution", null ],
      [ "The Algorithm", "dev_guide_parallelization_foundations.html#dev_guide_parallelization_core_algorithm", null ],
      [ "Parallel Components", "dev_guide_parallelization_foundations.html#dev_guide_parallelization_parallel_components", null ],
      [ "Actions", "dev_guide_parallelization_foundations.html#dev_guide_parallelization_actions", [
        [ "1. Simple Actions", "dev_guide_parallelization_foundations.html#dev_guide_parallelization_simple_actions", null ],
        [ "2. Iterable Actions", "dev_guide_parallelization_foundations.html#dev_guide_parallelization_iterable_actions", null ],
        [ "3. Reduction Actions", "dev_guide_parallelization_foundations.html#dev_guide_parallelization_reduction_actions", null ],
        [ "4. Threaded Actions", "dev_guide_parallelization_foundations.html#dev_guide_parallelization_threaded_actions", null ],
        [ "5. Local Synchronous Actions", "dev_guide_parallelization_foundations.html#dev_guide_parallelization_local_synchronous_actions", null ]
      ] ],
      [ "Mutable items in the GlobalCache", "dev_guide_parallelization_foundations.html#autotoc_md106", [
        [ "1. Specification of mutable GlobalCache items", "dev_guide_parallelization_foundations.html#autotoc_md107", null ],
        [ "2. Use of mutable GlobalCache items", "dev_guide_parallelization_foundations.html#autotoc_md108", [
          [ "1. Checking if the item is up-to-date", "dev_guide_parallelization_foundations.html#autotoc_md109", null ],
          [ "2. Retrieving the item", "dev_guide_parallelization_foundations.html#autotoc_md110", null ]
        ] ],
        [ "3. Modifying a mutable GlobalCache item", "dev_guide_parallelization_foundations.html#autotoc_md111", null ]
      ] ],
      [ "Charm++ Node and Processor Level Initialization Functions", "dev_guide_parallelization_foundations.html#dev_guide_parallelization_charm_node_processor_level_initialization", null ]
    ] ],
    [ "Redistributing Gridpoints", "redistributing_gridpoints.html", [
      [ "Introduction", "redistributing_gridpoints.html#autotoc_md134", null ],
      [ "Generalized Logical Coordinates", "redistributing_gridpoints.html#autotoc_md135", null ],
      [ "Equiangular Maps", "redistributing_gridpoints.html#autotoc_md136", null ],
      [ "Projective Maps", "redistributing_gridpoints.html#autotoc_md137", null ]
    ] ],
    [ "Automatic versioning", "dev_guide_automatic_versioning.html", [
      [ "Creating releases", "dev_guide_automatic_versioning.html#autotoc_md49", null ],
      [ "Release notes", "dev_guide_automatic_versioning.html#autotoc_md50", null ]
    ] ]
];