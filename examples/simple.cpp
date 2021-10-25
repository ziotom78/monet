#include <monet.h>

using namespace monet;

int main() {
  SVGCanvas canv{"simple.svg", 100, 100};

  // Clear the image with a soft gray
  canv.setfillcolor(gray(0.9));
  canv.rectangle(Point{0, 0}, Point{canv.getwidth(), canv.getheight()},
                 Action::Fill);

  // Create a closed path
  canv.moveto(Point{0, 0});
  canv.lineto(Point{100, 0});
  canv.lineto(Point{100, 100});
  canv.closepath();
  canv.strokepath();
}
