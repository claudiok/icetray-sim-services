/**
 * copyright  (C) 2004
 * the IceCube collaboration
 *  $Id: I3Selection.h 2601 2005-02-08 20:05:42Z olivas $
  *
 * @file I3Selection.h
 * @version $Revision: 1.3 $
 * @date $Date: 2005-02-08 21:05:42 +0100 (Tue, 08 Feb 2005) $
 * @author olivas
 */

#ifndef I3GEOSHIFTER_H
#define I3GEOSHIFTER_H

/**
 * @brief Utilities for parsing the input parameters for I3GeometrySelector.
 */

#include <vector>
#include <string>
#include "dataclasses/geometry/I3Geometry.h"
#include "dataclasses/physics/I3MCTree.h"

namespace I3GeoShifter{

  /**
   * Calculates the center of the detector in X-Y.
   * Returns a std::pair<double,double> where first
   * is x and second is y.
   */
  std::pair<double,double> 
    DetectorCenter(I3FramePtr frame, 
		    I3OMGeo::OMType omType = I3OMGeo::IceCube);

  void ShiftTree(const std::pair<double,double>&, I3MCTreePtr);

  void ShiftTree(I3FramePtr frame, I3MCTreePtr, 
		 I3OMGeo::OMType omType = I3OMGeo::IceCube);
					   
}

#endif
