#include "monet.h"

using namespace monet;

int main() {
  SVGCanvas canvas("tut01.svg", 500, 300);

  canvas.setfillcolor(yellow);
  canvas.rectangle(Point(0, 0), Point(500, 300), Action::Fill);

  canvas.setstrokecolor(black);
  canvas.line(Point(100, 100), Point(400, 200));
}
