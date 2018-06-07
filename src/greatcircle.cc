// Great Circle distance between two points using Heaverside formula
//
// Author: Howard Cheng
// Reference: http://mathforum.org/library/drmath/view/51879.html
//
// Given two points specified by their latitudes and longitudes, as well
// as the radius of the sphere, return the shortest distance between the
// two points along the surface of the sphere.
//
// latitude should be between -90 to 90 degrees (inclusive), and
// longitude should be between -180 to 180 degrees (inclusive)
//
// There are also routines that will convert between cartesian coordinates
// (x,y,z) and spherical coordinates (latitude, longitude, radius).
//

#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

const double PI = acos(-1.0);

double greatcircle(double lat1, double long1, double lat2, double long2,
		   double radius)
{
  lat1 *= PI/180.0;
  lat2 *= PI/180.0;
  long1 *= PI/180.0;
  long2 *= PI/180.0;
  
  double dlong = long2 - long1;
  double dlat = lat2 - lat1;
  double a = sin(dlat/2)*sin(dlat/2) +
    cos(lat1)*cos(lat2)*sin(dlong/2)*sin(dlong/2);
  return radius * 2 * atan2(sqrt(a), sqrt(1-a));
}

void longlat2cart(double lat, double lon, double radius,
		  double &x, double &y, double &z)
{
  lat *= PI/180.0;
  lon *= PI/180.0;
  x = radius * cos(lat) * cos(lon);
  y = radius * cos(lat) * sin(lon);
  z = radius * sin(lat);
}

void cart2longlat(double x, double y, double z,
		  double &lat, double &lon, double &radius)
{
  radius = sqrt(x*x + y*y + z*z);
  lat = (PI/2 - acos(z / radius)) * 180.0 / PI;
  lon = atan2(y, x) * 180.0 / PI;
}

int main(void)
{
  int T;
  cin >> T;
  while (T-- > 0) {
    const double radius = 6371009;
    double lat1, long1, lat2, long2;
    
    cin >> lat1 >> long1 >> lat2 >> long2;

    double x1, y1, z1, x2, y2, z2;
    
    longlat2cart(lat1, long1, radius, x1, y1, z1);
    longlat2cart(lat2, long2, radius, x2, y2, z2);

    double d1 = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
    double d2 = greatcircle(lat1, long1, lat2, long2, radius);

    cout << fixed << setprecision(0) << d2 - d1 << endl;

    double radius1;
    cart2longlat(x1, y1, z1, lat1, long1, radius1);
    cout << lat1 << ' ' << long1 << ' ' << radius1 << endl;
  }
  return 0;
}
