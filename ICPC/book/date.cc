// Date:    dates from Jan 1, 1753 to after
using namespace std;
using namespace std::rel_ops;

struct Date {
  int yyyy, mm, dd;
  static int const BASE_YEAR = 1753;

  enum dayName {SUN,MON,TUE,WED,THU,FRI,SAT};

  static bool validDate(int yr, int mon, int day) {
    return yr >= BASE_YEAR && mon >= 1 && mon <= 12 && day > 0 && day <= daysIn(mon, yr);
  }    
  bool isValid() const { return validDate(yyyy, mm, dd); }
  
  Date(int yr = 1970, int mon = 1, int day = 1)   // assume valid date
    : yyyy(yr), mm(mon), dd(day) { }
  
  dayName dayOfWeek() const {
    int a = (14 - mm) / 12, y = yyyy - a, m = mm + 12 * a - 2;
    return (dayName)((dd + y + y/4 - y/100 + y/400 + 31 * m / 12) % 7);
  }
  
  bool operator==(const Date &d) const {
    return dd == d.dd && mm == d.mm && yyyy == d.yyyy;
  }

  bool operator<(const Date &d) const {
    return yyyy < d.yyyy || (yyyy == d.yyyy && mm < d.mm) ||
      (yyyy == d.yyyy && mm == d.mm && dd < d.dd);
  }

  static bool leapYear(int y) {
    return (y % 400 ==0 || (y % 4 == 0 && y % 100 != 0));
  }

  static int daysIn(int m, int y) {
    switch (m) {
    case 4:  case 6:  case 9: case 11: return 30;
    case 2:  return leapYear(y) ? 29 : 28;
    default: return 31;
    }
  }
  
  // increment by day, month, or year, can use negative
  // if result is invalid date, the result is rounded down to the last valid date
  void addDay(int n = 1) {    // about n/30 iterations
    dd += n;
    while (dd > daysIn(mm,yyyy)) {
      dd -= daysIn(mm,yyyy);
      if (++mm > 12) { mm = 1;   yyyy++;    }
    }
    
    while (dd < 1) {
      if (--mm < 1) {  mm = 12;  yyyy--;    }
      dd += daysIn(mm,yyyy); 
    }
  }

  void addMonth(int n = 1) {   // about n/12 iterations
    mm += n;
    while (mm > 12) {  mm -= 12;   yyyy++;  }
    while (mm < 1)  {  mm += 12;   yyyy--;  }
    if (dd > daysIn(mm,yyyy)) dd = daysIn(mm,yyyy);
  }

  void addYear(int n = 1) {
    yyyy += n;
    if (!leapYear(yyyy) && mm == 2 && dd == 29) dd = 28;
  }

  int daysFromStart() const {  // number of days since 1753/01/01, incl. current date
    int c = 0;
    Date d(BASE_YEAR, 1, 1);
    Date d2(d);

    d2.addYear(1);
    while (d2 < *this) {
      c += leapYear(d.yyyy) ? 366 : 365;
      d = d2;    d2.addYear(1);
    }

    d2 = d;      d2.addMonth(1);
    while (d2 < *this) {
      c += daysIn(d.mm, d.yyyy);
      d = d2;    d2.addMonth(1);
    }
    while (d <= *this) { d.addDay();  c++; }
    return c;
  }
};
