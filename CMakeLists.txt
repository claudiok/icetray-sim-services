i3_project(sim-services
        PYTHON_DEST icecube/sim_services
	PYTHON_DIR python
)

# define the lists used to build the basic library
SET(LIB_${PROJECT_NAME}_SOURCEFILES
	private/sim-services/I3SumGenerator.cxx 
	private/sim-services/I3MCTreeHybridSimulationSplitter.cxx
	private/sim-services/I3PropagatorService.cxx
	private/sim-services/I3InIceCORSIKATrimmer.cxx
	private/sim-services/time-generator/I3MCEventHeaderGenerator.cxx
	private/sim-services/time-generator/I3ModifyEventID.cxx
	private/sim-services/I3PropagatorModule.cxx
	private/sim-services/I3CombineMCPE.cxx
	private/sim-services/I3DownsampleMCPE.cxx
	private/sim-services/I3MCPEtoI3MCHitConverter.cxx
)
SET(LIB_${PROJECT_NAME}_TOOLS python boost )

i3_add_library(sim-services
	${LIB_${PROJECT_NAME}_SOURCEFILES}
	
	USE_TOOLS ${LIB_${PROJECT_NAME}_TOOLS} 

	USE_PROJECTS icetray
	phys-services 
	dataclasses 
	simclasses 
  
)

# TODO: make the tests use PROPOSAL instead of MMC
IF (IS_DIRECTORY ${CMAKE_SOURCE_DIR}/c2j-icetray AND IS_DIRECTORY ${CMAKE_SOURCE_DIR}/mmc-icetray)
i3_test_executable(test 
	private/test/*.cxx
	USE_TOOLS boost python jni gsl
	USE_PROJECTS dataclasses phys-services sim-services trigger-sim dataio interfaces icetray
	#c2j-icetray mmc-icetray
)
ENDIF (IS_DIRECTORY ${CMAKE_SOURCE_DIR}/c2j-icetray AND IS_DIRECTORY ${CMAKE_SOURCE_DIR}/mmc-icetray)


add_subdirectory(private/pybindings)

i3_test_scripts(resources/tests/propagator_state_storage.py
  #resources/tests/all_your_tests_are_belong_to_us.py
  resources/tests/sanity_checker_module_tests.py
  resources/tests/sanity_checker_histogram_tests.py
)
