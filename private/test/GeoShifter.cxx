#include <I3Test.h>

#include "sim-services/I3GeoShifter.cxx"
#include "dataclasses/physics/I3MCTree.h"
#include "dataclasses/physics/I3MCTreeUtils.h"

using namespace std;

// Test some of Calibrate Module functionality
TEST_GROUP(GeoShifter);

TEST(detector_center){

  I3Tray tray;

  std::string strings_to_use("21,29,39,38,30,40,49");
  std::string stations_to_use("21,29,30,38,39,40,47,48,49,50,57,58,59,66,67,74");

  std::string strings_to_exclude("-1:20,22:28,31:37,41:48,50:80");
  std::string stations_to_exclude("-1:20,22:28,31:37,41:46,51:56,60:65,68:73,75:80");

  string icecube_geo(getenv("I3_SRC"));
  icecube_geo += "/phys-services/resources/icecube.geo";
  string amanda_geo(getenv("I3_SRC"));
  amanda_geo += "/phys-services/resources/amanda.geo";

  tray.AddService("I3TextFileGeometryServiceFactory","geoservice")
    ("IceCubeGeoFile",icecube_geo.c_str())
    ("AmandaGeoFile",amanda_geo.c_str());
  tray.AddService("I3EmptyStreamsFactory","empty_streams")
    ("NFrames",4)
    ("InstallGeometry",false);
  tray.AddService("I3GeometrySelectorServiceFactory","geo_selector")
    ("StringsToUse",strings_to_use.c_str())
    ("StationsToUse",stations_to_use.c_str())
    ("GeoSelectorName","I3GeometrySelectorService")
    ;

  tray.AddModule("I3Muxer","muxer")
    ("GeometryService","I3GeometrySelectorService");
  //I3GeoShiftTestModule contains ENSURE statements
  tray.AddModule("I3GeoShiftTestModule","geo_test");

  tray.AddModule("TrashCan","trash");

  tray.Execute();
  tray.Finish();


}

TEST(shift_tree){
  const double X_0(0.*I3Units::m);
  const double Y_0(0.*I3Units::m);
  const double Z_0(0.*I3Units::m);

  I3Particle cascade;
  cascade.SetPos(X_0,Y_0,Z_0);

  I3MCTreePtr mctree(new I3MCTree); 
  I3MCTreeUtils::AddPrimary(mctree,cascade); 

  const double DX(50.*I3Units::m);
  const double DY(50.*I3Units::m);
  pair<double,double> dcent(DX,DY);

  I3GeoShifter::ShiftTree(dcent,mctree);
  
  vector<I3Particle> primaries = I3MCTreeUtils::GetPrimaries(mctree);
  
  //make sure the size of the tree doesn't change
  ENSURE(mctree->size() == 1);
  
  I3Particle shifted_cascade = primaries.front();

  ENSURE(shifted_cascade.GetX() == X_0 + dcent.first);
  ENSURE(shifted_cascade.GetY() == Y_0 + dcent.second);
  ENSURE(shifted_cascade.GetZ() == Z_0);
}
