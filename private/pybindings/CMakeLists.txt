if(BUILD_PYBINDINGS)
 i3_add_pybindings(sim_services
   module.cxx		
   USE_TOOLS boost python log4cplus root
   USE_PROJECTS sim-services
   )
endif(BUILD_PYBINDINGS)
