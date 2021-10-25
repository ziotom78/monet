#include "monet.h"
#include <cstdio>
#include <vector>

using namespace monet;

int main() {
  SVGCanvas canvas("tut05.svg", 400, 100);

  std::vector<Point> points{Point(10, 10), Point(200, 70), Point(300, 10)};

  // Quartic curve, shown using a thick black line
  canvas.setstrokewidth(3);
  canvas.setstrokecolor(black);

  canvas.moveto(points[0]);
  canvas.quadraticto(points[1], points[2]);
  canvas.strokepath();

  // Clear the current path and start a new one
  canvas.clearpath();

  // Draw a light red line that crosses the three points
  canvas.setstrokewidth(1);
  canvas.setstrokecolor(lightred);

  canvas.moveto(points[0]);
  canvas.lineto(points[1]);
  canvas.lineto(points[2]);
  canvas.strokepath();

  // Plot circles where each point stands
  canvas.setfillcolor(black);
  for (Point pt : points) {
    canvas.circle(pt, 5, Action::Fill);
  }

  // Finally, add some text
  canvas.setfontsize(10);
  canvas.text(points[1] + Point(0, 10), "Target point",
              HorizontalAlignment::Center);
  canvas.text(points[2] + Point(10, 0), "End point", HorizontalAlignment::Right,
              VerticalAlignment::Center);
}
