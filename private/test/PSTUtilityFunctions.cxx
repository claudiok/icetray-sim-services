#include <I3Test.h>
#include <icetray/I3Units.h>
#include <mmc-icetray/I3PropagatorServiceMMC.h>
#include <sim-services/PropagatorServiceUtils.h>
#include <dataclasses/physics/I3MCTreeUtils.h>
#include <c2j-icetray/I3JavaVM.h>
#include <simclasses/I3MMCTrack.h>
#include <cmc/I3CascadeMCService.h>
#include <phys-services/I3GSLRandomService.h>
#include "PSTUtilityFunctions.h"
  
PSTUtils::Configuration
PSTUtils::Setup(I3Particle lepton){
  std::string I3_BUILD(getenv("I3_BUILD"));
  
  std::vector<std::string> jvmOpts;
  jvmOpts.push_back( std::string("-Djava.class.path="+I3_BUILD+"/lib/mmc.jar") );
  I3JavaVMPtr jvm(new I3JavaVM(jvmOpts)); 
  
  std::string mmcopts("-romb=5 -raw -user -sdec -time -lpm -bs=1 ");
  mmcopts += "-ph=3 -bb=2 -sh=2 -frho -cont ";
  mmcopts += "-tdir="+I3_BUILD+"/mmc-icetray/resources ";
  mmcopts += "-mediadef="+I3_BUILD+"/mmc-icetray/resources/mediadef ";
  mmcopts += "-seed=1 -radius=600 -length=1200 ";
  if( lepton.GetType() == I3Particle::TauPlus ||
      lepton.GetType() == I3Particle::TauMinus ){
    mmcopts += "-tau ";
  }
  I3PropagatorServiceBasePtr p( new I3PropagatorServiceMMC(*jvm,mmcopts) );
  
  I3GSLRandomServicePtr rand( new I3GSLRandomService(42) );
  shared_ptr<I3CascadeMCService> cmc( new I3CascadeMCService(rand) );
  
  I3MCTreePtr t( new I3MCTree );
  
  I3Particle nu;
  nu.SetShape( I3Particle::Primary );
  if(lepton.GetType() == I3Particle::MuMinus || lepton.GetType() == I3Particle::MuPlus){
    nu.SetType( I3Particle::NuMu );
  }else{
    if(lepton.GetType() == I3Particle::TauMinus || lepton.GetType() == I3Particle::TauPlus){
      nu.SetType( I3Particle::NuTau );
    }else{
      if(lepton.GetType() == I3Particle::EMinus || lepton.GetType() == I3Particle::EPlus){
	nu.SetType( I3Particle::NuE );
      }else{
	log_fatal("Flavor not recognized.");
      }
    }
  }
  
  lepton.SetPos(0,0,0);
  lepton.SetDir(0,0);
  lepton.SetLocationType( I3Particle::InIce );
  
  I3Particle cascade;
  cascade.SetType(I3Particle::Hadrons);
  cascade.SetPos(0,0,0);
  cascade.SetDir(0,0);
  cascade.SetEnergy( lepton.GetEnergy() );
  cascade.SetLocationType( I3Particle::InIce );
  
  I3MCTreeUtils::AddPrimary(t,nu);
  I3MCTreeUtils::AppendChild(t,nu,lepton);
  I3MCTreeUtils::AppendChild(t,nu,cascade);
  
  Configuration c;
  c.prop = p;
  c.mctree = t;
  c.cmc = cmc;
  c.jvm = jvm;
  return c;
}
  
void PSTUtils::BasicTrackTests(Configuration config){
  
  boost::shared_ptr< I3Vector<I3MMCTrack> >
    mmctracks = PropagatorServiceUtils::Propagate( config.mctree, config.prop );
  
  ENSURE( mmctracks->size() == 1 );
  
  // find the original particle and ensure it's propagated
  // the new tree should have the daughters as well
  ENSURE( config.mctree->size() > 2 );
  
  for( I3MCTree::iterator i = config.mctree->begin();
       i != config.mctree->end(); i++){
    std::cerr << *i << std::endl;
    if( i->GetType() == mmctracks->front().GetI3Particle().GetType() ){
      ENSURE( i->GetLength() > 0);      
    }
  }
  
}


std::pair<int,int>
PSTUtils::CalculateMuonsAndCascades( I3MCTreePtr mctree){
  int nmuons(0);
  int ncascades(0);
  for( I3MCTree::iterator i = mctree->begin();
       i != mctree->end(); i++){
    if( i->IsCascade() ){
      ncascades++;
    }
    if( i->GetType() == I3Particle::MuPlus ||
	i->GetType() == I3Particle::MuMinus ){
      nmuons++;
    }
  }
  return std::pair<int,int>(nmuons,ncascades);
}

void PSTUtils::BasicSecondPassTests(Configuration config){  
  
  boost::shared_ptr< I3Vector<I3MMCTrack> >
    mmctracklist = PropagatorServiceUtils::Propagate( config.mctree, config.prop );
  
  std::pair<int,int> mc_pre = CalculateMuonsAndCascades( config.mctree );
  
  mmctracklist = PropagatorServiceUtils::SecondPass( config.mctree, 
						     config.cmc,
						     config.prop );
  
  std::pair<int,int> mc_post = CalculateMuonsAndCascades( config.mctree );
  
  std::cerr<<"nmuons_pre = "<<mc_pre.first<<std::endl;
  std::cerr<<"ncascades_pre = "<<mc_pre.second<<std::endl;
  std::cerr<<"nmuons_post = "<<mc_post.first<<std::endl;
  std::cerr<<"ncascades_post = "<<mc_post.second<<std::endl;
  ENSURE( mc_pre.first < mc_post.first, 
	  "there should be more muons after running CMC");
  
  ENSURE( mc_pre.second < mc_post.second, 
	  "there should be more cascades after running CMC");
  
}
  
