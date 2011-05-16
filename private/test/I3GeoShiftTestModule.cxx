/**
 * copyright  (C) 2004
 * the icecube collaboration
 * $Id: I3GeoShiftTestModule.cxx 2598 2005-02-04 17:18:56Z olivas $
 *
 * @file I3GeoShiftTestModule.cxx
 * @version $Revision: 1.8 $
 * @date $Date: 2005-02-04 18:18:56 +0100 (Fri, 04 Feb 2005) $
 * @author olivas
 *
*/

// headers for this selection
#include <I3Test.h>
#include "I3GeoShiftTestModule.h"
#include "interfaces/I3GeometryService.h"

// other headers
//  global header for all the IceTray stuff
#include "icetray/I3TrayHeaders.h"
/* headers for the dataclasses stuff */
#include "dataclasses/geometry/I3Geometry.h"
#include "dataclasses/physics/I3MCTree.h"
#include "dataclasses/physics/I3MCTreeUtils.h"
#include "sim-services/I3GeoShifter.h"

using namespace std;

I3_MODULE(I3GeoShiftTestModule);

I3GeoShiftTestModule::I3GeoShiftTestModule(const I3Context& ctx) : 
  I3Module(ctx)
{
  AddOutBox("OutBox");
}

I3GeoShiftTestModule::~I3GeoShiftTestModule() {
}

// transitions
void I3GeoShiftTestModule::Configure() {
}

void I3GeoShiftTestModule::Physics(I3FramePtr frame) {
  
  const double X_0(0.*I3Units::m);
  const double Y_0(0.*I3Units::m);
  const double Z_0(0.*I3Units::m);

  I3Particle cascade;
  cascade.SetPos(X_0,Y_0,Z_0);

  I3MCTreePtr mctree(new I3MCTree); 
  I3MCTreeUtils::AddPrimary(mctree,cascade); 

  I3GeoShifter::ShiftTree(frame,mctree); 

  std::vector<I3Particle> primaries = I3MCTreeUtils::GetPrimaries(mctree);
  
  ENSURE(mctree->size() == 1);
  
  I3Particle shifted_cascade = primaries.front();

  std::pair<double,double> dcent = I3GeoShifter::DetectorCenter(frame);

  ENSURE(shifted_cascade.GetX() == X_0 + dcent.first);
  ENSURE(shifted_cascade.GetY() == Y_0 + dcent.second);
  ENSURE(shifted_cascade.GetZ() == Z_0);

}


