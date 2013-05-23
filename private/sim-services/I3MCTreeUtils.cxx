#include "sim-services/I3MCTreeUtils.h"

I3Particle
I3MCTreeUtils::Get(const I3MCTree& t, const I3MCPE& mcpe)
{
  I3MCTree::iterator iter;
  for (iter=t.begin(); iter!=t.end(); ++iter) 
    if (iter->GetMajorID() == mcpe.major_ID && 
	iter->GetMinorID() == mcpe.minor_ID) 
      return *iter;
  log_debug("Could not find I3Particle associated with I3MCPE");
  return I3Particle();
}
