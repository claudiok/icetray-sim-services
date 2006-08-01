#ifndef I3ICETOPUTILS_H
#define I3ICETOPUTILS_H
 
#include "dataclasses/physics/I3Particle.h"
#include "dataclasses/I3Units.h"
#include "dataclasses/I3Constants.h"

using namespace I3Units;

namespace I3IceTopUtils
{

  double Intersect(const I3Particle& track,
                   const I3Position& tank_pos);

};

#endif
