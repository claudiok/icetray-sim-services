#include <sim-services/I3PropagatorServiceBase.h>

namespace bp = boost::python;

struct I3PropagatorServiceBaseWrapper : I3PropagatorServiceBase, bp::wrapper<I3PropagatorServiceBase>
{
    // pure virtual
    virtual shared_ptr<I3MMCTrack> Propagate(I3Particle& p, std::vector<I3Particle>& daughters) {
        // python needs to be able to change these, so provide it with pointers
        I3ParticlePtr p_(new I3Particle(p));
        I3ParticleVectPtr daughters_(new I3ParticleVect());
        daughters_->assign(daughters.begin(), daughters.end());
        
        shared_ptr<I3MMCTrack> retval = this->get_override("Propagate")(p_, daughters_);
        
        p = *p_;
        daughters = *daughters_;
        
        return retval;
    }
};

void register_I3PropagatorServiceBase()
{
    {
        bp::scope I3PropagatorServiceBase_scope = 
        bp::class_<I3PropagatorServiceBaseWrapper, boost::shared_ptr<I3PropagatorServiceBaseWrapper>, boost::noncopyable>("I3PropagatorServiceBase")
        .def("Propagate", bp::pure_virtual(&I3PropagatorServiceBase::Propagate))
        ;
    }
    
    bp::implicitly_convertible<shared_ptr<I3PropagatorServiceBaseWrapper>, shared_ptr<const I3PropagatorServiceBase> >();
    bp::implicitly_convertible<shared_ptr<I3PropagatorServiceBaseWrapper>, shared_ptr<I3PropagatorServiceBase> >();
    bp::implicitly_convertible<shared_ptr<I3PropagatorServiceBaseWrapper>, shared_ptr<const I3PropagatorServiceBaseWrapper> >();
}
