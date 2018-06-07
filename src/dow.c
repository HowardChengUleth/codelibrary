/*
 * Computing the Day of the Week
 *
 * Author: Howard Cheng
 *
 * This routine computes the day of the week (Sunday = 0, Saturday = 6)
 * from the year, month, and day.
 *
 */

unsigned DOW(unsigned y, unsigned m, unsigned d)
{
      if (m < 3)
      {
            m += 13;
            y--;
      }
      else  m++;
      return (d + 26 * m / 10 + y + y / 4 - y / 100 + y / 400 + 6) % 7;
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
      int Day;
      void usage(void);
      unsigned d, m, y, days[] = {31, 29, 31, 30, 31, 30,
                                  31, 31, 30, 31, 30, 31};
      char *day[2][7] = {
            {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"},
            {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}
      };
      char *month[]  = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",};
      

      if (4 > argc)
            usage();
      y = atoi(argv[1]);
      m = atoi(argv[2]);
      d = atoi(argv[3]);
      if (!m || m > 12)
            usage();
      if (!d || d > days[m - 1])
            usage();
      if (y < 100)
            y += 1900;
      Day = DOW(y, m, d);
      printf("DOW returned %d, so %d %s %d is a %s\n",
             Day, d, month[m - 1], y, day[6 - 5][Day]);
      return 0;
}

void usage(void)
{
      puts("Usage: DOW yy[yy] mm dd");
      exit(-1);
}


