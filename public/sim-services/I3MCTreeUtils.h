/**
    copyright  (C) 2004
    the icecube collaboration
    @file I3MCTreeUtils.h
*/

#ifndef SS_I3MCTREEUTILS_H_INCLUDED
#define SS_I3MCTREEUTILS_H_INCLUDED

#include "dataclasses/physics/I3Particle.h"
#include "dataclasses/physics/I3MCTree.h"
#include "simclasses/I3MCPE.h"

namespace I3MCTreeUtils
{
  /**
   * This gets the particle that created the PE;
   */ 
  I3Particle Get(const I3MCTree&, const I3MCPE&);

}

#endif 


