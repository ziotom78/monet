#include "monet.h"
#include <iostream>

using namespace monet;

int main() {
  Color col1{rgb(1.0, 0.0, 0.5)};
  Color col2{hsl(0.6, 0.4, 0.7)};

  std::cout << "col1 = " << col1 << '\n';
  std::cout << "The HTML representation of col1 is '" << col1.toHTML() << "'\n";

  std::cout << "col2 = " << col2 << '\n';
  std::cout << "The HTML representation of col2 is '" << col2.toHTML() << "'\n";
}
