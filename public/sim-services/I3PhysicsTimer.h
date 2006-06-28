#ifndef ICETRAY_I3PHYSICSTIMER_H_INCLUDED
#define ICETRAY_I3PHYSICSTIMER_H_INCLUDED

#include <sys/time.h>
#include <sys/resource.h>

#include <string>
#include <icetray/I3Frame.h>
#include <icetray/I3PointerTypedefs.h>

struct I3RUsage : public I3FrameObject 
{
  // in microseconds, though timer resoution may vary with hardware
  double systemtime, usertime, wallclocktime;

  template <typename Archive>
  void 
  serialize(Archive &ar, unsigned version);

};
  
I3_POINTER_TYPEDEFS(I3RUsage);


class I3PhysicsTimer
{
  struct rusage start_rusage_, end_rusage_;
  struct timeval start_wallclock_, end_wallclock_;

  I3FramePtr frame_;
  std::string module_name_;

  double 
  delta(const struct timeval &starttime, const struct timeval& stoptime);

public:

  I3PhysicsTimer(I3FramePtr frame, const std::string& module_name);
  ~I3PhysicsTimer();

};
  


#endif
