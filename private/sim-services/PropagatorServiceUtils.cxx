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

void PropagatorServiceUtils::Propagate(I3MCTreePtr mctree_ptr, I3PropagatorServicePtr propagator){
  //find all the muons and taus and propagate them
  //the propagator updates the length and fills a
  //vector with the children
  for(I3MCTree::iterator t_iter = mctree_ptr->begin();
      t_iter != mctree_ptr->end(); t_iter++){
    if( (t_iter->GetType() == I3Particle::MuMinus ||
	 t_iter->GetType() == I3Particle::MuPlus ||
	 t_iter->GetType() == I3Particle::TauMinus ||
	 t_iter->GetType() == I3Particle::TauPlus ) ) {
      //only propagate taus and muons 
      vector<I3Particle> children;
      propagator->Propagate(*t_iter, children);
      BOOST_FOREACH(I3Particle& c, children)
	mctree_ptr->append_child(t_iter, c);
    }
	 
  }
}

void PropagatorServiceUtils::SecondPass(I3MCTreePtr mctree_ptr, 
				 shared_ptr<I3CascadeMCService> cmc ,
				 I3PropagatorServicePtr propagator){
  //run over the tree and simulate everything and anything cmc wants to
  for(I3MCTree::iterator t_iter = mctree_ptr->begin();
      t_iter != mctree_ptr->end(); t_iter++){
    //simulate everything CMC can
    vector<I3Particle> cascadeChildren;
    cmc->Simulate(*t_iter, cascadeChildren);
    // we want hit-maker to ignore this and generate hits
    // for the children only
    if(cascadeChildren.size() > 0 ){
      t_iter->SetShape(I3Particle::Dark);
    
      //now append the children to the tree and propagate muons
      BOOST_FOREACH(I3Particle& c, cascadeChildren){
	// we need to keep the iterator of the cascade we're append
	// this will make it easier to add the muon children
	I3MCTree::iterator c_iter = mctree_ptr->append_child(t_iter, c);
      
	if(c_iter->GetType() == I3Particle::MuMinus ||
	   c_iter->GetType() == I3Particle::MuPlus ){
	  vector<I3Particle> muonChildren;
	  propagator->Propagate(*c_iter, muonChildren);
	  BOOST_FOREACH(I3Particle& s, muonChildren)
	    mctree_ptr->append_child(c_iter, s);
	}	  
      } // end loop over children of the cascade 
    } // end if cmc actually did something
  } // end iteration over all the particles in the tree
} // end of function 
