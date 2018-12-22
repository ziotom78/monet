#include "monet.hpp"
#include <iostream>

using namespace monet;

int main() {
  // @@@START_CODE_SNIPPET@@@
  Point pt1(300, 400);
  std::cout << "pt1 is " << pt1 << '\n';
  pt1 /= 10;
  std::cout << "pt1 is now " << pt1 << '\n';
  std::cout << "pt1 scaled by 5 is " << pt1 / 5 << '\n';

  Point pt2(100, 200);
  std::cout << "The sum of pt1 and pt2 is " << pt1 + pt2 << '\n';
  std::cout << "The X component of pt1 + pt2 is " << (pt1 + pt2).x << '\n';
  // @@@END_CODE_SNIPPET@@@
}
