/*
 * linear.cpp
 * Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
 *
 * Distributed under terms of the MIT license.
 */

#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

bool somePredicate(double x);

bool anyTrue(const double a[], int n);
// Return true if the somePredicate function returns true for at
// least one of the array elements, false otherwise.

int countTrue(const double a[], int n);
// Return the number of elements in the array for which the
// somePredicate function returns true.

int firstTrue(const double a[], int n);
// Return the subscript of the first element in the array for which
// the somePredicate function returns true.  If there is no such
// element, return -1.

int positionOfMin(const double a[], int n);
// Return the subscript of the smallest element in the array (i.e.,
// return the smallest subscript m such that a[m] <= a[k] for all
// k from 0 to n-1).  If the function is told to examine no
// elements, return -1.

bool includes(const double a1[], int n1, const double a2[], int n2);
// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)

bool somePredicate(double x)
{
    // return sin(exp(x)) > cos(x); // include <cmath> for std::sin, etc.
    return x > 0;
}

bool anyTrue(const double a[], int n)
{
  if (n == 0) return false;
  return somePredicate(a[0]) || anyTrue(a + 1, n - 1);
}

int countTrue(const double a[], int n)
{
  if (n == 0) return 0;
  return somePredicate(a[0]) + countTrue(a + 1, n - 1);
}

int firstTrue(const double a[], int n)
{
  if (n == 0) return -1;
  if (somePredicate(a[0])) return 0;
  int r = firstTrue(a + 1, n - 1);
  return r == -1 ? -1 : 1 + r;
}

int positionOfMin(const double a[], int n)
{
  if (n == 0) return -1;
  if (n == 1) return 0;
  size_t minIdxExceptFirst = positionOfMin(a + 1, n - 1) + 1;
  return a[0] < a[minIdxExceptFirst] ? 0 : minIdxExceptFirst;
}

bool includes(const double a1[], int n1, const double a2[], int n2)
{
  if (n2 == 0) return true;
  if (n1 == 0) return false;
  if (a1[0] == a2[0])
    return includes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
  else
    return includes(a1 + 1, n1 - 1, a2, n2);
}

int main()
{
  double a0[0] = {};
  double a1[1] = {1};
  double a2[1] = {-1};
  double a3[5] = {-1, -2, -3, -4, -5};
  double a4[5] = {3, 5, 2, 1, 4};
  double a5[5] = {-1, -2, -3, -4, 5};
  double a6[6] = {1, 2, 3, 4, -5, 6};
  double s6[3] = {1, 3, -5};

  // bool anyTrue(const double a[], int n)
  assert(anyTrue(a0, 0) == false);
  assert(anyTrue(a1, 1) == true);
  assert(anyTrue(a2, 1) == false);
  assert(anyTrue(a3, 5) == false);
  assert(anyTrue(a4, 5) == true);
  assert(anyTrue(a5, 5) == true);
  assert(anyTrue(a6, 5) == true);

  // int countTrue(const double a[], int n)
  assert(countTrue(a0, 0) == 0);
  assert(countTrue(a1, 1) == 1);
  assert(countTrue(a2, 1) == 0);
  assert(countTrue(a3, 5) == 0);
  assert(countTrue(a4, 5) == 5);
  assert(countTrue(a5, 5) == 1);
  assert(countTrue(a6, 5) == 4);

  // int firstTrue(const double a[], int n)
  assert(firstTrue(a0, 0) == -1);
  assert(firstTrue(a1, 1) == 0);
  assert(firstTrue(a2, 1) == -1);
  assert(firstTrue(a3, 5) == -1);
  assert(firstTrue(a4, 5) == 0);
  assert(firstTrue(a5, 5) == 4);
  assert(firstTrue(a6, 5) == 0);

  // int positionOfMin(const double a[], int n)
  assert(positionOfMin(a0, 0) == -1);
  assert(positionOfMin(a1, 1) == 0);
  assert(positionOfMin(a2, 1) == 0);
  assert(positionOfMin(a3, 5) == 4);
  assert(positionOfMin(a4, 5) == 3);
  assert(positionOfMin(a5, 5) == 3);
  assert(positionOfMin(a6, 5) == 4);

  // bool includes(const double a1[], int n1, const double a2[], int n2)
  assert(includes(a0, 0, a0, 0) == true);
  assert(includes(a1, 1, a1, 1) == true);
  assert(includes(a1, 1, a0, 0) == true);
  assert(includes(a0, 0, a1, 1) == false);
  assert(includes(a3, 5, a2, 1) == true);
  assert(includes(a2, 1, a3, 5) == false);
  assert(includes(a6, 5, s6, 3) == true);
  assert(includes(a6, 6, s6, 3) == true);
  assert(includes(a6, 3, s6, 3) == false);

  cout << "passed all tests" << endl;

  return 0;
}

