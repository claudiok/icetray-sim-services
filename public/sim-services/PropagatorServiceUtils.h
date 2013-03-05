#ifndef PROPAGATORSERVICEUTILS_H
#define PROPAGATORSERVICEUTILS_H
/**
 *  copyright  (C) 2005
 *  the IceCube collaboration
 *  $Id: $
 *
 *  @version $Revision: $
 *  @date    $Date: $
 *  @author  Alex Olivas <olivas@icecube.umd.edu>
 *
 * @brief PropagatorServiceUtils header file
 * Utilities for converting I3NuGParticle lists to I3MCTrees
 * 
 */

#include <dataclasses/physics/I3Particle.h>
#include <dataclasses/physics/I3MCTree.h>
#include <sim-services/I3CascadeMCServiceBase.h>
#include <sim-services/I3PropagatorServiceBase.h>
#include <simclasses/I3MMCTrack.h>

namespace PropagatorServiceUtils{

  shared_ptr<I3Vector<I3MMCTrack> > 
    Propagate(shared_ptr<I3MCTree>& mctree_ptr, shared_ptr<I3PropagatorServiceBase> propagator);

  shared_ptr<I3Vector<I3MMCTrack> > SecondPass(shared_ptr<I3MCTree>& mctree_ptr, 
				    shared_ptr<I3CascadeMCServiceBase> cmc ,
				    shared_ptr<I3PropagatorServiceBase> propagator);
    
}

#endif //PropagatorServiceUtils
