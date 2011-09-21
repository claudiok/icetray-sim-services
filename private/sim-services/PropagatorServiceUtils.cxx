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

using namespace std;

I3MMCTrackListPtr PropagatorServiceUtils::Propagate(I3MCTreePtr mctree_ptr, I3PropagatorServicePtr propagator){
  //find all the muons and taus and propagate them
  //the propagator updates the length and fills a
  //vector with the children
  I3MMCTrackListPtr mmcTrackList( new I3MMCTrackList);
  for(I3MCTree::iterator t_iter = mctree_ptr->begin();
      t_iter != mctree_ptr->end(); t_iter++){
    if( (t_iter->GetType() == I3Particle::MuMinus ||
	 t_iter->GetType() == I3Particle::MuPlus ||
	 t_iter->GetType() == I3Particle::TauMinus ||
	 t_iter->GetType() == I3Particle::TauPlus ) ) {
      //only propagate taus and muons 
      vector<I3Particle> children;
      I3MMCTrackPtr mmcTrack = propagator->Propagate(*t_iter, children);
      if(mmcTrack) mmcTrackList->push_back(*mmcTrack);
      BOOST_FOREACH(I3Particle& c, children)
	mctree_ptr->append_child(t_iter, c);
    }	 
  }
  return mmcTrackList;
}

I3MMCTrackListPtr PropagatorServiceUtils::SecondPass(I3MCTreePtr& mctree_ptr, 
				 shared_ptr<I3CascadeMCService> cmc ,
				 I3PropagatorServicePtr propagator){
  I3MMCTrackListPtr mmcTrackList( new I3MMCTrackList);
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
    vector<I3Particle> cascadeChildren;
    cmc->Simulate(*t_iter, cascadeChildren);
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
	t_iter->GetMajorID() != copy_iter->GetMajorID() ) log_fatal("lost track of the particle");

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
	  vector<I3Particle> muonChildren;
	  I3MMCTrackPtr mmcTrack = propagator->Propagate(*c_iter, muonChildren);
	  if(mmcTrack) mmcTrackList->push_back(*mmcTrack);
	  BOOST_FOREACH(I3Particle& s, muonChildren)
	    tree_copy->append_child(c_iter, s);
	}	  
      } // end loop over children of the cascade 
    } // end if cmc actually did something
  } // end iteration over all the particles in the tree
  //just before returning we need to swap the pointers
  mctree_ptr = tree_copy;
  return mmcTrackList;
} // end of function 
