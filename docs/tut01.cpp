#include "monet.hpp"

using namespace monet;

int main() {
  SVGCanvas canvas("tut01.svg", 500, 500);

  canvas.setfillcolor(yellow);
  canvas.rectangle(Point(0, 0), Point(500, 500), Action::Fill);
}
