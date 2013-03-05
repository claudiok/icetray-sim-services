
#include "sim-services/I3CascadeMCServiceBase.h"
#include "dataclasses/physics/I3Particle.h"

void register_I3CascadeMCServiceBase()
{
	using namespace boost::python;
	
	class_<I3CascadeMCServiceBase, I3CascadeMCServiceBasePtr, boost::noncopyable>("I3CascadeMCServiceBase", no_init)
	    .def("Simulate", &I3CascadeMCServiceBase::Simulate)
	;
}
