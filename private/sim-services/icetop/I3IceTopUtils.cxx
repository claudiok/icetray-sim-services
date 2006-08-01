#include "sim-services/icetop/I3IceTopUtils.h"
                                                                                                 
using namespace std;

/*
  return intersection length in meter but
  return 0 if the tank was not hit by this particle
*/

double I3IceTopUtils::Intersect (const I3Particle &track,
			       const I3Position &tank_pos) {
  
  // read the tank's geometry (should get this from db later
  const double tankRadius = .93 * I3Units::m; // tank.GetRadius();
  const double tankHeight = 1.* I3Units::m; // tank.GetHeight();
  
  // get tank position. Add a safety margin so that the tank is below
  // the particles
  static const double z_safety = 5. * I3Units::m;
  const double tankX = tank_pos.GetX ();
  const double tankY = tank_pos.GetY ();
  const double tankZ = tank_pos.GetZ () - z_safety;
  
  // todo check tank 0,0,0
  const double zTop = 0.5 * tankHeight;
  const double zBottom = - 0.5 * tankHeight;

  log_debug ("Checking intersection between %d at %f/%f/%f",
             track.GetType (),
             track.GetPos ().GetX (),
             track.GetPos ().GetY (),
             track.GetPos ().GetZ ());
  log_debug ("and tank at %f/%f/%f, h+/- %f/%f",
             tankX, tankY, tankZ, zTop, zBottom);

  //
  // the speed of everything is just c in units of m/ns
  const double speed = 3.0e8 * (I3Units::m/I3Units::s);

  //
  // the particle's starting point
  I3Position track_pos = track.GetPos ();
  const double x  = track_pos.GetX () - tankX;
  const double y  = track_pos.GetY () - tankY;
  const double z  = track_pos.GetZ () - tankZ;
  const double time = track.GetTime ();

  const double phi = track.GetAzimuth();
  const double theta = track.GetZenith();
  
// TODO check for consistent coordinate system !!!!!!!!!!!!!!!!!!!!!!!!!!
  // downward going particle has positiv nz here !!
  const double nx = cos( phi ) * sin( theta );
  const double ny = sin( phi ) * sin( theta );
  const double nz = cos( theta );

  log_debug ("Particle starts at x/y/z (w.r.t. tank): %f/%f/%f",
             x, y, z);
  log_debug ("directrion th/ph %f/%f",
             theta, phi);
  log_debug ("direction of travel is %f/%f/%f",
             nx, ny, nz);

  /*
    Calculate the entry AND exit point of particle in tank
  */

  // position of the intersections found (entry and exit points)
  // theoretically two points are enough, but algorithmically ... who knows
  vector<double> IntersectionPointsX;
  vector<double> IntersectionPointsY;
  vector<double> IntersectionPointsZ;
  vector<double> IntersectionPointsT; // time

  // for the test of interscections with the top/bottom of the tank
  // take care of particles moving parallel to the ground
  if (nz != 0.) {

    log_debug ("particle is _not_ horizontal");

    double prop, dist;
    double intersection_x, intersection_y, intersection_z, intersection_t;

    // check for intersection with the tank's TOP SURFACE
    prop           = (zTop-z)/nz;
    intersection_x = x + prop*nx;
    intersection_y = y + prop*ny;

    // distance from x=y=0 (center of tank) of projection of that point on z=0
    dist = sqrt( intersection_x*intersection_x +
                 intersection_y*intersection_y );

    log_debug ("particle propagates in z direction for %f,"
               "distance from tank centre is %f, tankradius is %f",
               prop/I3Units::m, dist/I3Units::m, tankRadius/I3Units::m);

    // see if intersection is found
    if (dist<=tankRadius) {
      intersection_z = z + prop*nz;
      intersection_t = time + prop/speed;

      // add this point to the list of intersections
      IntersectionPointsT.push_back( intersection_t );
      IntersectionPointsX.push_back( intersection_x );
      IntersectionPointsY.push_back( intersection_y );
      IntersectionPointsZ.push_back( intersection_z );
    }

    // check for intersection with the tank's BOTTOM SURFACE
    prop           = (zBottom-z)/nz;
    intersection_x = x + prop*nx;
    intersection_y = y + prop*ny;

    dist = sqrt( intersection_x*intersection_x +
                 intersection_y*intersection_y );

    // see if intersection is valid
    if(dist <= tankRadius) {
      intersection_z = z + prop*nz;
      intersection_t = time + prop/speed;
      IntersectionPointsT.push_back( intersection_t );
      IntersectionPointsX.push_back( intersection_x );
      IntersectionPointsY.push_back( intersection_y );
      IntersectionPointsZ.push_back( intersection_z );
    }
  }

  // for the test of intersection the tank walls
  // check if particle is NOT moving perpendicular to surface
  if ( theta!=0 &&
       IntersectionPointsT.size()<2 ) {
    // Particle could intersect side surface
                                                                                                                                                             
    // sqr of length of nx,ny projected on the z=0 plane
    double nr2_proj = nx*nx + ny*ny;

    // solve now:   x1/2 = -p/2 +- sqrt( (p/2)^2 - q )
    double p = 2.*(x*nx + y*ny)/nr2_proj;
    double q = (x*x + y*y - tankRadius*tankRadius)/nr2_proj;

    // helper variables
    double tmp2 =-p/2;
    double tmp1 = tmp2*tmp2-q;

    if(tmp1>0) {
      // quadratic equation has non-complex solutions
      tmp1   = sqrt(tmp1);

      // check if first solution is on the tank surface
      if (zBottom<z+(tmp2+tmp1)*nz  &&  z+(tmp2+tmp1)*nz<zTop) {
        double prop = tmp2+tmp1;
        double intersection_x = x + prop*nx;
        double intersection_y = y + prop*ny;
        double intersection_z = z + prop*nz;
        double intersection_t = time + prop/speed;

        IntersectionPointsT.push_back( intersection_t );
        IntersectionPointsX.push_back( intersection_x );
        IntersectionPointsY.push_back( intersection_y );
        IntersectionPointsZ.push_back( intersection_z );
      }

      // for execution speed check again if not both intersection points
      // have been found already
      if ( IntersectionPointsT.size()<2 ) {

        // check if second solution is on the tank surface
        if (zBottom<z+(tmp2-tmp1)*nz  &&  z+(tmp2-tmp1)*nz<zTop) {
          // second intersection with detector

          double prop = tmp2-tmp1;
          double intersection_x = x + prop*nx;
          double intersection_y = y + prop*ny;
          double intersection_z = z + prop*nz;
          double intersection_t = time + prop/speed;

          IntersectionPointsT.push_back( intersection_t );
          IntersectionPointsX.push_back( intersection_x );
          IntersectionPointsY.push_back( intersection_y );
          IntersectionPointsZ.push_back( intersection_z );
        }
      }
    }
  }

  log_debug ("Number of intersection points is %d",
             IntersectionPointsT.size ());

  // only if 2 intersecting points were found up to here, we have a valid hit
  if ( 2==IntersectionPointsT.size() ) {

    double length = sqrt( (IntersectionPointsX[0]-IntersectionPointsX[1])*
                          (IntersectionPointsX[0]-IntersectionPointsX[1]) +
                          (IntersectionPointsY[0]-IntersectionPointsY[1])*
                          (IntersectionPointsY[0]-IntersectionPointsY[1]) +
                          (IntersectionPointsZ[0]-IntersectionPointsZ[1])*
                          (IntersectionPointsZ[0]-IntersectionPointsZ[1]) );
    log_debug ("track length is %f", length / I3Units::m);
    return length;
  }

  return 0;
  
}

