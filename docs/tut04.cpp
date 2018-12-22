#include "monet.hpp"
#include <cstdio>
#include <vector>

using namespace monet;

int main() {
  SVGCanvas canvas("tut04.svg", 500, 200);

  std::vector<Point> points{Point(100, 50), Point(250, 140), Point(350, 175),
                            Point(350, 50)};

  // Draw the path
  for (size_t i = 0; i < points.size(); ++i) {
    if (i == 0)
      canvas.moveto(points[i]);
    else
      canvas.lineto(points[i]);
  }
  canvas.strokepath();

  // Draw circles and some text on the points along the path
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
