#include "monet.h"

using namespace monet;

int main() {
  SVGCanvas canvas{"tut07.svg", 500, 150};

  Point pt1{0, 0};
  Point pt2{280, 50};
  for (double tr{}; tr < 1.0; tr += 0.2) {
    canvas.settransparency(tr);
    canvas.setfillcolor(hsl(tr, 1.0, 0.5));
    canvas.rectangle(pt1, pt2, Action::Fill);

    canvas.settransparency(0.0);
    canvas.setfillcolor(black);
    canvas.rectangle(pt1, pt2, Action::Stroke);

    pt1 += Point(50, 20);
    pt2 += Point(50, 20);
  }
}
