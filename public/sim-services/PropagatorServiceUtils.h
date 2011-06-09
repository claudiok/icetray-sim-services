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
#include <cmc/I3CascadeMCService.h>
#include <mmc-icetray/I3PropagatorService.h>
#include <simclasses/I3MMCTrack.h>

namespace PropagatorServiceUtils{

  shared_ptr<I3Vector<I3MMCTrack> > 
    Propagate(shared_ptr<I3MCTree> mctree_ptr, shared_ptr<I3PropagatorService> propagator);

  shared_ptr<I3Vector<I3MMCTrack> > SecondPass(shared_ptr<I3MCTree>& mctree_ptr, 
				    shared_ptr<I3CascadeMCService> cmc ,
				    shared_ptr<I3PropagatorService> propagator);
    
}

#endif //PropagatorServiceUtils
