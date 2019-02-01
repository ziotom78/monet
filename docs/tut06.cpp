#include "monet.hpp"
#include <cstdio>
#include <vector>

using namespace monet;

int main() {
  SVGCanvas canvas("tut06.svg", 400, 100);

// @@@START_CODE_SNIPPET@@@
#include "tut06.inc.cpp"
  // @@@END_CODE_SNIPPET@@@

  // Clear the current path and start a new one
  canvas.clearpath();

  // Draw a light red line that crosses the three points
  canvas.setstrokewidth(1);
  canvas.setstrokecolor(lightred);

  canvas.moveto(points[0]);
  canvas.lineto(points[1]);
  canvas.lineto(points[2]);
  canvas.lineto(points[3]);
  canvas.strokepath();

  // Plot circles where each point stands
  canvas.setfillcolor(black);
  for (Point pt : points) {
    canvas.circle(pt, 5, Action::Fill);
  }

  // Finally, add some text
  canvas.setfontsize(10);
  canvas.text(points[1] + Point(0, 10), "Control point #1",
              HorizontalAlignment::Right);
  canvas.text(points[2] + Point(0, 10), "Control point #2",
              HorizontalAlignment::Center);
  canvas.text(points[3] + Point(10, 0), "End point", HorizontalAlignment::Right,
              VerticalAlignment::Center);
}
