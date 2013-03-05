#ifndef I3PROPAGATORSERVICEBASE_H
#define I3PROPAGATORSERVICEBASE_H
/**
* class: I3PropagatorServiceBASE
*
* (c) 2012 IceCube Collaboration
*/

#include <vector>
#include <simclasses/I3MMCTrack.h>
#include <dataclasses/physics/I3Particle.h>
#include <boost/utility.hpp>
#include <icetray/I3PointerTypedefs.h>

#include <sim-services/I3PropagatorServiceBase.h>

class I3PropagatorServiceBase {
  public:
    virtual boost::shared_ptr<I3MMCTrack> Propagate(I3Particle& p, std::vector<I3Particle>& daughters) = 0;

    I3PropagatorServiceBase();
    virtual ~I3PropagatorServiceBase();
};

I3_POINTER_TYPEDEFS(I3PropagatorServiceBase);

#endif //I3PROPAGATORSERVICEBASE_H
