if(BUILD_PYBINDINGS)
 i3_add_pybindings(sim_services
   I3SumGenerator.cxx
   I3PropagatorServiceBase.cxx
   module.cxx
   USE_TOOLS boost python log4cplus
   USE_PROJECTS sim-services
   )
endif(BUILD_PYBINDINGS)
