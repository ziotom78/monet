#include "monet.h"
#include <cstdio>
#include <vector>

using namespace monet;

int main() {
  SVGCanvas canvas("tut02.svg", 500, 500);
  std::vector<Point> points{
      Point(100, 100), Point(400, 100), Point(100, 400),
      Point(400, 400), Point(250, 250),
  };

  // Draw a border
  canvas.setstrokecolor(black);
  canvas.rectangle(Point(0, 0), Point(500, 500), Action::Stroke);

  // Draw a point and some text for every element in "points"
  canvas.setfillcolor(black);
  for (auto pt : points) {
    canvas.circle(pt, 5, Action::Fill);

    char text[64];
    sprintf(text, "(%d, %d)", static_cast<int>(pt.x), static_cast<int>(pt.y));
    canvas.text(pt + Point(0, canvas.getfontsize()), text,
                HorizontalAlignment::Center, VerticalAlignment::Top);
  }
}
