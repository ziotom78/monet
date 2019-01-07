#include "monet.hpp"

using namespace monet;

int main() {
  // @@@START_CODE_SNIPPET@@@
  SVGCanvas canvas("tut08.svg", 500, 150);

  Point pivot(250, 75);

  for (double angle = 0; angle < 360; angle += 30) {
    // First rotate, then translate
    canvas.begingroup(rotate(angle) | translate(pivot));
    canvas.setfillcolor(hsl(angle / 360, 1.0, 0.4));

    // Note that the position of the text is (0, 0)
    canvas.text(Point(0, 0), "Hello, world!");
    canvas.endgroup();
  }

  // Paint the pivot point as a black circle
  canvas.setfillcolor(black);
  canvas.circle(pivot, 5, Action::Fill);
  // @@@END_CODE_SNIPPET@@@
}
