/**
 * Definition of I3MCRawDOMStatusFiller class
 *
 * copyright  (C) 2004
 * the IceCube collaboration
 * $Id: I3MCRawDOMStatusFiller.h 6917 2005-04-26 02:56:37Z tmccauley $
 * 
 * @file I3MCRawDOMStatusFiller.h
 * @version $Revision: 1.14 $
 * @date $Date: 2005-04-25 22:56:37 -0400 (Mon , 25 Apr 2005) $
 * @author pretz
 * @author tmccauley
 */

#ifndef I3MCCALIBRATIONSERVICE_H
#define I3MCCALIBRATIONSERVICE_H

#include "phys-services/I3CalibrationService.h"
#include "phys-services/I3GeometryService.h"

class I3Geometry;
I3_POINTER_TYPEDEFS(I3Geometry);
/**
 *
 * @brief This module allow you to fill the RawDOMStatus data 
 *	contained on the DetectorStatus stream by hand. 
 *	The information gets cloned for all of the IceCube
 * 	DOMs contained in the geometry.
 */
class I3MCCalibrationService : public I3CalibrationService
{
public:

  I3MCCalibrationService(I3GeometryServicePtr g){
    geo_service_ = g;
  }
  virtual I3CalibrationConstPtr GetCalibration(I3Time time);
  virtual ~I3MCCalibrationService() { };

  SET_LOGGER("I3MCCalibrationService");

 private:
  I3MCCalibrationService();
  I3GeometryServicePtr geo_service_;
};

#endif
