// Great Circle computations ////////////////////////////////////////////
// lat [-90,90], long [-180,180]
double greatcircle(double lat1, double long1, double lat2, double long2,
		   double radius) {
  lat1 *= PI/180.0; lat2 *= PI/180.0; long1 *= PI/180.0; long2 *= PI/180.0;
  double dlong = long2 - long1, dlat = lat2 - lat1;
  double a = sin(dlat/2)*sin(dlat/2) + cos(lat1)*cos(lat2)*sin(dlong/2)*sin(dlong/2);
  return radius * 2 * atan2(sqrt(a), sqrt(1-a));
}

void longlat2cart(double lat, double lon, double radius,
		  double &x, double &y, double &z) {
  lat *= PI/180.0; lon *= PI/180.0;  x = radius * cos(lat) * cos(lon);
  y = radius * cos(lat) * sin(lon);  z = radius * sin(lat);
}

void cart2longlat(double x, double y, double z,
		  double &lat, double &lon, double &radius) {
  radius = sqrt(x*x + y*y + z*z);
  lat = (PI/2 - acos(z / radius)) * 180.0 / PI;  lon = atan2(y, x) * 180.0 / PI;
}
