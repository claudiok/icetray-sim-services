
#ifndef SIM_SERVICES_I3CASCADEMCSERVICEBASE_H_INCLUDED
#define SIM_SERVICES_I3CASCADEMCSERVICEBASE_H_INCLUDED

#include "icetray/I3PointerTypedefs.h"
#include <vector>

class I3Particle;

class I3CascadeMCServiceBase {
public:
	virtual void Simulate( const I3Particle& c, std::vector<I3Particle>& d) = 0;
	virtual ~I3CascadeMCServiceBase();
};

I3_POINTER_TYPEDEFS(I3CascadeMCServiceBase);

#endif // SIM_SERVICES_I3CASCADEMCSERVICEBASE_H_INCLUDED
