//
//   Copyright (c) 2009
//   the icecube collaboration
//   $Id$
//                                                                                                       
//   @version $Revision$                                                                                                    
//   @author Henrike Wissing 
//

#include <icetray/I3FrameObject.h>
#include <icetray/load_project.h>

using namespace boost::python;
namespace bp = boost::python;
#include <sim-services/I3GeoShifter.h>

void register_I3SumGenerator();

void
shiftTreeToCenter(I3FramePtr frame , I3MCTreePtr tree, 
		  I3OMGeo::OMType omType = I3OMGeo::IceCube)
{
  I3GeoShifter::ShiftTree(frame, tree, omType);
}


BOOST_PYTHON_MODULE(sim_services)
{
  load_project("libsim-services", false);
  def("shiftTreeToCenter", &shiftTreeToCenter);

  register_I3SumGenerator();
}

