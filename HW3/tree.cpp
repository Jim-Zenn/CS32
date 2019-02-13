/*
 * tree.cpp
 * Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
 *
 * Distributed under terms of the MIT license.
 */

// #include <iostream>
// #include <cassert>

// using namespace std;

int countIncludes(const double a1[], int n1, const double a2[], int n2);
// Return the number of ways that all n2 elements of a2 appear
// in the n1 element array a1 in the same order (though not
// necessarily consecutively).  The empty sequence appears in a
// sequence of length n1 in 1 way, even if n1 is 0.

void exchange(double &x, double &y);
// Exchange two doubles

void divide(double a[], int n, double divider,
            int &firstNotGreater, int &firstLess);
// Rearrange the elements of the array so that all the elements
// whose value is > divider come before all the other elements,
// and all the elements whose value is < divider come after all
// the other elements.  Upon return, firstNotGreater is set to the
// index of the first element in the rearranged array that is
// <= divider, or n if there is no such element; and firstLess is
// set to the index of the first element that is < divider, or n
// if there is no such element.
// In other words, upon return from the function, the array is a
// permutation of its original value such that
//   * for 0 <= i < firstNotGreater, a[i] > divider
//   * for firstNotGreater <= i < firstLess, a[i] == divider
//   * for firstLess <= i < n, a[i] < divider
// All the elements > divider end up in no particular order.
// All the elements < divider end up in no particular order.

void order(double a[], int n);
// Rearrange the elements of the array in decreasing order, i.e.
// a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
// If n <= 1, do nothing.

int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
  if (n1 < 0) n1 = 0;  // spec required, though it's kinda stupid
  if (n2 < 0) n2 = 0;  // spec required, though it's kinda stupid
  if (n2 == 0) return 1;
  if (n1 == 0) return 0;
  if (a1[0] == a2[0]){
    return countIncludes(a1 + 1, n1 - 1, a2 + 1, n2 - 1) +
           countIncludes(a1 + 1, n1 - 1, a2, n2);
  }
  else {
    return countIncludes(a1 + 1, n1 - 1, a2, n2);
  }
}

void exchange(double &x, double &y)
{
  double t = x;
  x = y;
  y = t;
}

void divide(double a[], int n, double divider,
            int &firstNotGreater, int &firstLess)
{
  if (n < 0) n = 0;

  // It will always be the case that just before evaluating the loop
  // condition:
  //  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
  //  Every element earlier than position firstNotGreater is > divider
  //  Every element from position firstNotGreater to firstUnknown-1 is
  //    == divider
  //  Every element from firstUnknown to firstLess-1 is not known yet
  //  Every element at position firstLess or later is < divider

  firstNotGreater = 0;
  firstLess = n;
  int firstUnknown = 0;
  while (firstUnknown < firstLess)
  {
    if (a[firstUnknown] < divider)
    {
      firstLess--;
      exchange(a[firstUnknown], a[firstLess]);
    }
    else
    {
      if (a[firstUnknown] > divider)
      {
        exchange(a[firstNotGreater], a[firstUnknown]);
        firstNotGreater++;
      }
      firstUnknown++;
    }
  }
}

void order(double a[], int n)
{
  if (n < 0) n = 0;  // spec required, though it's kinda stupid
  if (n == 0 || n == 1) return;  // no need to sort
  int firstNotGreater, firstLess;
  divide(a, n, a[0], firstNotGreater, firstLess);
  order(a, firstNotGreater);
  order(a + firstLess, n - firstLess);
}

// int main()
// {
//   double a1[7] = {10, 50, 40, 20, 50, 40, 30};
//   double a21[3] = {10, 20, 40};
//   double a22[3] = {10, 40, 30};
//   double a23[3] = {20, 10, 40};
//   double a24[3] = {50, 40, 30};

//   assert(countIncludes(a1, 7, a21, 3) == 1);
//   assert(countIncludes(a1, 7, a22, 3) == 2);
//   assert(countIncludes(a1, 7, a23, 3) == 0);
//   assert(countIncludes(a1, 7, a24, 3) == 3);

//   order(a1, 7);

//   assert(a1[0] == 50);
//   assert(a1[1] == 50);
//   assert(a1[2] == 40);
//   assert(a1[3] == 40);
//   assert(a1[4] == 30);
//   assert(a1[5] == 20);
//   assert(a1[6] == 10);

//   cout << "passed all tests" << endl;
//   return 0;
// }
