/**
 * copyright  (C) 2004
 * the IceCube collaboration
 *  $Id: I3GeoShiftTestModule.h 2601 2005-02-08 20:05:42Z olivas $
  *
 * @file I3GeoShiftTestModule.h
 * @version $Revision: 1.3 $
 * @date $Date: 2005-02-08 21:05:42 +0100 (Tue, 08 Feb 2005) $
 * @author olivas
 */

#ifndef GEOSHIFTTESTMODULE_H
#define GEOSHIFTTESTMODULE_H

// header files
#include "icetray/I3Module.h"

/**
 * @brief IceTray module to test whether I3GeometrySelector is working properly.
 *The two parameters stringsToUse_ and stringsToExclude_ should be set to the same 
 *values as the I3GeometrySelector.  This test passes if the geometry contains only OMs
 *that are in stringsToUse_ and does not contain any DOMs that are in stringsToExclude_.
 */

//This needs to be turned into a service that gets the geometry from another service
//and not the frame.  The muxer can then be pointed to this service to get the trimmed geometry.

class I3GeoShiftTestModule : public I3Module
{
public:

  /*
   * Constructor:  builds an instance of the module, with the
   * context provided by IceTray. 
   */ 
  I3GeoShiftTestModule(const I3Context& ctx);
  
  /*
   * Destructor: deletes the module
   */ 
  ~I3GeoShiftTestModule();
  
  /**
   * This module takes a configuration parameter and so it must be configured
   */
  void Configure();
  void DAQ(I3FramePtr frame);

private:
  
  /**
   *default constructor, assignment operator, and copy constructor
   * declared private to prevent use
   */
  I3GeoShiftTestModule();
  I3GeoShiftTestModule(const I3GeoShiftTestModule& source);
  I3GeoShiftTestModule& operator=(const I3GeoShiftTestModule& source);

  SET_LOGGER("I3GeoShiftTestModule");
  
};  // end of class I3GeoShiftTestModule

#endif //I3GEOSHIFTTESTMODULE_H
