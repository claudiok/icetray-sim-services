class I3Particle;
class I3CascadeMCService;
class I3PropagatorService;
#include <dataclasses/I3Tree.h>
typedef I3Tree<I3Particle> I3MCTree;

namespace PSTUtils{
  
  struct Configuration{
    shared_ptr<I3PropagatorService> prop;
    shared_ptr<I3MCTree> mctree;
    shared_ptr<I3CascadeMCService> cmc;
  };

  void BasicTrackTests( Configuration );      
  
  void BasicSecondPassTests( Configuration );
  
  Configuration Setup( I3Particle );

  std::pair<int,int> CalculateMuonsAndCascades( shared_ptr<I3MCTree> );
  
}
