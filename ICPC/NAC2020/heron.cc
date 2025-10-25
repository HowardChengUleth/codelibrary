double area_heron(double a, double b, double c) { // assumes triangle valid
  return sqrt((a+b+c)*(c-a+b)*(c+a-b)*(a+b-c))/4.0;
}
