if(BUILD_PYBINDINGS)
 i3_add_pybindings(sim_services
   I3SumGenerator.cxx
   I3PropagatorServiceBase.cxx
   I3CascadeMCServiceBase.cxx
   module.cxx
   USE_TOOLS boost python
   USE_PROJECTS sim-services
   )
endif(BUILD_PYBINDINGS)
