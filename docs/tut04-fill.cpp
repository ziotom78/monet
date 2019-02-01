#include "monet.hpp"
#include <cstdio>
#include <vector>

using namespace monet;

int main() {
  SVGCanvas canvas("tut04-fill.svg", 500, 200);

  std::vector<Point> points{Point(100, 50), Point(250, 140), Point(350, 175),
                            Point(350, 50)};

  for (size_t i = 0; i < points.size(); ++i) {
    if (i == 0)
      canvas.moveto(points[i]);
    else
      canvas.lineto(points[i]);
  }

// @@@START_CODE_SNIPPET@@@
#include "tut04-fill.inc.cpp"
  // @@@END_CODE_SNIPPET@@@

  int pointidx = 1;
  for (const Point &point : points) {
    canvas.setfillcolor(lightred);
    canvas.setstrokecolor(black);
    canvas.circle(point, 5, Action::FillAndStroke);

    char buf[8];
    std::sprintf(buf, "#%d", pointidx++);
    canvas.setfillcolor(black);
    canvas.text(point + Point(canvas.getfontsize(), 0), buf,
                HorizontalAlignment::Right, VerticalAlignment::Center);
  }
}
