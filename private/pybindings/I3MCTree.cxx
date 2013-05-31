#include <dataclasses/physics/I3MCTree.h>
#include <simclasses/I3MCPE.h>
#include <sim-services/I3MCTreeUtils.h>

struct mctreeutils{
  I3Particle get(const I3MCTree& t, const I3MCPE& pe){
    return I3MCTreeUtils::Get(t,pe);
  }
};

namespace bp = boost::python;

void register_I3MCTree()
{
  bp::scope outer =    
    bp::class_<mctreeutils>("I3MCTreeUtils")
    .def("get", &mctreeutils::get);
}
