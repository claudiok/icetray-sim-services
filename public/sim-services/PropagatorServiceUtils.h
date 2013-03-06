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

#include <icetray/I3Frame.h>
#include <dataclasses/physics/I3Particle.h>
#include <dataclasses/physics/I3MCTree.h>
#include <sim-services/I3PropagatorService.h>

namespace PropagatorServiceUtils{

  void Propagate(shared_ptr<I3MCTree>& mctree_ptr,
  					I3PropagatorServicePtr propagator,
  					I3FramePtr frame);

  void SecondPass(shared_ptr<I3MCTree>& mctree_ptr, 
				    I3PropagatorServicePtr cmc ,
				    I3PropagatorServicePtr propagator,
				    I3FramePtr frame);
    
}

#endif //PropagatorServiceUtils
