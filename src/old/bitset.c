/*
 * Set Operations Implemented by Bitmap
 *
 * Author: Howard Cheng
 *
 * Note that we should be using unsigned integers for our bitmap type.
 * The i-th element is in the set iff the i-th bit is set to 1.
 *
 */

#include <stdio.h>
#include <assert.h>

typedef unsigned int Set;

Set s_intersect(Set A, Set B)
{
   return A & B;
}

Set s_union(Set A, Set B)
{
   return A | B;
}

Set s_diff(Set A, Set B)
{
   return A & ~B;
}

Set s_comp(Set A)
{
   return ~A;
}

Set s_equal(Set A, Set B)
{
   return A == B;
}

int s_subset(Set A, Set B)
{
   return !(A & ~B);
}

int s_propsubset(Set A, Set B)
{
   return (s_subset(A, B) && !s_equal(A, B));
}

int s_empty(Set A)
{
   return !A;
}

int s_element_of(Set A, int n)
{
   return (1 << n) & A;
}

void s_add(Set *A, int n)
{
   *A |= (1 << n);
}

void s_remove(Set *A, int n)
{
   *A &= ~(1 << n);
}

int s_num_elements(Set A)
{
   int count;
   for (count = 0; A != 0; A >>= 1) {
      if (A & 1) {
         count++;
      }
   }
   return count;
}

void s_print(Set A)
{
   int i;
   for (i = 0; i < 32; i++) {
      putchar((A & 1) ? '1' : '0');
      A >>= 1;
   }
   printf("\n");
}

int main(void)
{
   Set A, B, C, D;
   int i;

   A = 0;
   B = 0;

   assert(s_empty(A) && s_empty(B));

   for (i = 0; i < 32; i += 6) {
      s_add(&A, i);
   }
   for (i = 0; i < 32; i += 4) {
      s_add(&B, i);
   }
   for (i = 0; i < 32; i++) {
      if (i % 6 == 0) {
         assert(s_element_of(A, i));
      } else {
         assert(!s_element_of(A, i));
      }
      if (i % 4 == 0) {
         assert(s_element_of(B, i));
      } else {
         assert(!s_element_of(B, i));
      }
   }
   assert(!s_empty(A) && !s_empty(B));
   printf("A     = "); s_print(A);
   printf("B     = "); s_print(B);
   printf("A U B = "); s_print(s_union(A, B));
   printf("A ^ B = "); s_print(s_intersect(A, B));
   printf("A \\ B = "); s_print(s_diff(A, B));
   printf("~A    = "); s_print(s_comp(A));
   printf("|A|   = %d\n", s_num_elements(A));
   printf("|B|   = %d\n", s_num_elements(B));
   assert(s_num_elements(s_union(A, B)) ==
          s_num_elements(A) + s_num_elements(B) -
          s_num_elements(s_intersect(A, B)));
   assert(s_subset(A,A));
   assert(!s_propsubset(A,A));
   assert(s_subset(A, s_union(A, B)) && s_propsubset(A, s_union(A,B)));

   for (i = 0; i < 32; i += 4) {
      s_remove(&A, i);
   }
   printf("new A = "); s_print(A);
   return 0;
}
