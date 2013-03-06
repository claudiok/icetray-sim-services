/**
 *  copyright  (C) 2005
 *  the IceCube collaboration
 *  $Id: $
 *
 *  @version $Revision: $
 *  @date    $Date: $
 *  @author  Alex Olivas <olivas@icecube.umd.edu>
 *
 *  @brief   PropagatorServiceUtils IMPLEMENTATION FILE
 */

#include <sim-services/PropagatorServiceUtils.h>
#include <boost/foreach.hpp>


void
PropagatorServiceUtils::Propagate(I3MCTreePtr& mctree_ptr, 
                                  I3PropagatorServicePtr propagator,
                                  I3FramePtr frame)
{
    // Iteratively apply propagator->Propagate() to all muons and taus
    // in the tree. In case any tau should generate another muon,
    // propagate that muon as well. 

    // Extract a list of particles to work on
    std::deque<I3MCTree::iterator> particlesToPropagate;
    
    for(I3MCTree::iterator t_iter = mctree_ptr->begin();
        t_iter != mctree_ptr->end(); t_iter++)
    {
        if ((t_iter->GetType() != I3Particle::MuMinus) &&
            (t_iter->GetType() != I3Particle::MuPlus) &&
            (t_iter->GetType() != I3Particle::TauMinus) &&
            (t_iter->GetType() != I3Particle::TauPlus))
             continue;

        // it's either a tau or a muon. propagate it later.
        particlesToPropagate.push_back(t_iter);
    }
    
    
    // The main propagation loop.
    while (!particlesToPropagate.empty())
    {
        // retrieve the first entry
        const I3MCTree::iterator &currentParticle_it =
            particlesToPropagate.front();
        particlesToPropagate.pop_front();

        // propagate it!
        std::vector<I3Particle> children =
        propagator->Propagate(*currentParticle_it, frame);
        log_trace("number of children: %zu", children.size());


        // Insert each of the children into the tree. While at it,
        // check to see if any of them is something we should propagate.
        BOOST_FOREACH(const I3Particle& child, children)
        {
            const I3MCTree::iterator child_it =
                mctree_ptr->append_child(currentParticle_it, child);
            
            if ((child_it->GetType() == I3Particle::MuMinus) ||
                (child_it->GetType() == I3Particle::MuPlus) ||
                (child_it->GetType() == I3Particle::TauMinus) ||
                (child_it->GetType() == I3Particle::TauPlus))
            {
                // it's either a tau or a muon. propagate it later.
                particlesToPropagate.push_back(child_it);
            }
        }

    }
}

void PropagatorServiceUtils::SecondPass(I3MCTreePtr& mctree_ptr, 
                                 I3PropagatorServicePtr cmc ,
                                 I3PropagatorServicePtr propagator,
                                 I3FramePtr frame){

  // copy the tree and fill that as we go.
  // this keeps us from recursively splitting cascades
  // may not be a bad idea, but this may unneccesarily inflate the tree size. 
  I3MCTreePtr tree_copy( new I3MCTree(*mctree_ptr) );

  //run over the tree and simulate everything and anything cmc wants to
  I3MCTree::iterator t_iter = mctree_ptr->begin();
  I3MCTree::iterator copy_iter = tree_copy->begin();
  // going to assume new particles are added to the end of the tree
  // don't worry, we'll check later to be sure
  for(;t_iter != mctree_ptr->end(); t_iter++, copy_iter++){
   
    //simulate everything CMC can
    std::vector<I3Particle> cascadeChildren =
    cmc->Propagate(*t_iter, frame);
    // we want hit-maker to ignore this and generate hits
    // for the children only

    // from this point on we modify the copy
    if( t_iter->GetMinorID() != copy_iter->GetMinorID() ||
        t_iter->GetMajorID() != copy_iter->GetMajorID() ){
      // these are not copies of each other
      // somehow we got out of sync.  
      // need to match them the hard way.
      for( copy_iter = tree_copy->begin();
           copy_iter != tree_copy->end();
           copy_iter++){
        if( t_iter->GetMinorID() == copy_iter->GetMinorID() &&
            t_iter->GetMajorID() == copy_iter->GetMajorID() )
          break;
      }
    }    
    // just one last sanity check
    if( copy_iter == tree_copy->end() ||
        ! tree_copy->is_valid( copy_iter ) ||
       t_iter->GetMinorID() != copy_iter->GetMinorID() ||
        t_iter->GetMajorID() != copy_iter->GetMajorID() ) 
      log_fatal("lost track of the particle");

    // ok we're free and clear
    if(cascadeChildren.size() > 0 ){
      copy_iter->SetShape(I3Particle::Dark);
    
      //now append the children to the tree and propagate muons
      BOOST_FOREACH(I3Particle& c, cascadeChildren){
        // we need to keep the iterator of the cascade we're append
        // this will make it easier to add the muon children
        I3MCTree::iterator c_iter = tree_copy->append_child(copy_iter, c);
      
        if(c_iter->GetType() == I3Particle::MuMinus ||
           c_iter->GetType() == I3Particle::MuPlus ){
          std::vector<I3Particle> muonChildren =
          propagator->Propagate(*c_iter, frame);
          BOOST_FOREACH(I3Particle& s, muonChildren)
            tree_copy->append_child(c_iter, s);
        }         
      } // end loop over children of the cascade 
    } // end if cmc actually did something
  } // end iteration over all the particles in the tree
  //just before returning we need to swap the pointers
  mctree_ptr = tree_copy;
} // end of function 
