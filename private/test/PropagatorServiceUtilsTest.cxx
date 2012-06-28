#include <I3Test.h>
#include <sim-services/PropagatorServiceUtils.h>
#include <dataclasses/physics/I3MCTreeUtils.h>
#include <c2j-icetray/I3JavaVM.h>
#include <simclasses/I3MMCTrack.h>
#include <icetray/I3Units.h>
#include <cmc/I3CascadeMCService.h>
#include <phys-services/I3GSLRandomService.h>

#include "PSTUtilityFunctions.h"

TEST_GROUP(PropagatorServiceUtilsTest);

TEST(propagate_muon){
  I3Particle l;
  l.SetType( I3Particle::MuMinus );
  l.SetEnergy( 1 * I3Units::PeV );
  PSTUtils::Configuration config( PSTUtils::Setup( l ) );
  BasicTrackTests(config);
}

TEST(propagate_tau){
  I3Particle l;
  l.SetType( I3Particle::TauMinus );
  l.SetEnergy( 1 * I3Units::PeV );
  PSTUtils::Configuration config( PSTUtils::Setup( l ) );
  BasicTrackTests(config);
}

// electrons are special snowflakes
// MMC should skip over them and return
// an empty list
TEST(propagate_electron){

  I3Particle l;
  l.SetType( I3Particle::EMinus );
  l.SetEnergy( 1 * I3Units::PeV );
  PSTUtils::Configuration config( PSTUtils::Setup( l ) );

  boost::shared_ptr< I3Vector<I3MMCTrack> >
    mmctracklist = PropagatorServiceUtils::Propagate( config.mctree, config.prop );
  
  ENSURE( mmctracklist->empty() );
}

TEST(second_pass_tau){
  I3Particle l;
  l.SetType( I3Particle::TauMinus );
  l.SetEnergy( 1 * I3Units::PeV );
  PSTUtils::Configuration config( PSTUtils::Setup( l ) );
  BasicSecondPassTests( config );  
}

TEST(second_pass_muon){
  I3Particle l;
  l.SetType( I3Particle::MuMinus );
  l.SetEnergy( 1 * I3Units::PeV );
  PSTUtils::Configuration config( PSTUtils::Setup( l ) );
  BasicSecondPassTests( config );  
}

TEST(second_pass_electron){
  I3Particle l;
  l.SetType( I3Particle::EMinus );
  l.SetEnergy( 1 * I3Units::PeV );
  PSTUtils::Configuration config( PSTUtils::Setup( l ) );
  BasicSecondPassTests( config );  
}
