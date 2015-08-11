i3_project(sim-services
  PYTHON_DIR python
  PYTHON_DEST icecube/sim_services
  DOCS_DIR resources/docs
)

# define the lists used to build the basic library
SET(LIB_${PROJECT_NAME}_SOURCEFILES
  private/sim-services/I3SumGenerator.cxx
  private/sim-services/I3MCTreeHybridSimulationSplitter.cxx
  private/sim-services/I3PropagatorService.cxx
  private/sim-services/I3InIceCORSIKATrimmer.cxx
  private/sim-services/I3MCEventHeaderGenerator.cxx
  private/sim-services/I3ModifyStartTime.cxx
  private/sim-services/I3PropagatorModule.cxx
  private/sim-services/I3CombineMCPE.cxx
  private/sim-services/I3DownsampleMCPE.cxx
  private/sim-services/I3MCPEtoI3MCHitConverter.cxx
  private/sim-services/I3RemoveLargeDT.cxx
  private/sim-services/I3ModifyEventID.cxx # deprecated
)
SET(LIB_${PROJECT_NAME}_TOOLS python boost )

i3_add_library(sim-services
  ${LIB_${PROJECT_NAME}_SOURCEFILES}

  USE_TOOLS ${LIB_${PROJECT_NAME}_TOOLS} scipy

  USE_PROJECTS icetray
  phys-services
  dataclasses
  simclasses
)

add_subdirectory(private/pybindings)

# The following tests need matplotlib installed :
#
#  * all_your_tests_are_belong_to_us.py
#  * sanity_checker_module_tests.py
#  * sanity_checker_histogram_tests.py
#
# Now we could disable them if matplotlib isn't
# installed, but these test things that are going
# to be run in production.  It's better to have
# them fail on the bots than bring production to
# a screaching halt.
i3_test_scripts(resources/tests/propagator_state_storage.py
  #resources/tests/all_your_tests_are_belong_to_us.py
  #resources/tests/nugen_full_chain.py
  resources/tests/sanity_checker_module_tests.py
  resources/tests/sanity_checker_histogram_tests.py
  resources/tests/test_modify_start_time.py
  resources/tests/test_combine_PEs.py
  resources/tests/test_down_sample_mcpe.py
  resources/tests/test_mcpe_to_mchit_converter.py
  resources/tests/test_inice_corsika_trimmer.py
)
