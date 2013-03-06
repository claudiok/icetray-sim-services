#ifndef I3PROPAGATORSERVICE_H
#define I3PROPAGATORSERVICE_H

#include <map>

#include <icetray/I3PointerTypedefs.h>
#include <icetray/I3Frame.h>
#include <dataclasses/physics/I3Particle.h>
#include <phys-services/I3RandomService.h>

class I3PropagatorService {
  public:
    virtual std::vector<I3Particle> Propagate(I3Particle& p, I3FramePtr frame) = 0;
    virtual void SetRandomNumberGenerator(I3RandomServicePtr random) = 0;

    I3PropagatorService();
    virtual ~I3PropagatorService();
};

I3_POINTER_TYPEDEFS(I3PropagatorService);

typedef std::map<I3Particle::ParticleType, I3PropagatorServicePtr> I3ParticleTypePropagatorServiceMap;

I3_POINTER_TYPEDEFS(I3ParticleTypePropagatorServiceMap);

#endif //I3PROPAGATORSERVICE_H
