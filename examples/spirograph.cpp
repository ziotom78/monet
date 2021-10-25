#include <cmath>
#include <monet.h>

using namespace std;
using namespace monet;

// For an explanation of the equation, see
// https://en.wikipedia.org/wiki/Spirograph
Point spirograph(double small_r, double large_r, double a, double t) {
  double r_diff{large_r - small_r};
  double r_ratio{small_r / large_r};
  return Point{r_diff * cos(r_ratio * t) + a * cos((1 - r_ratio) * t),
               r_diff * sin(r_ratio * t) - a * sin((1 - r_ratio) * t)};
}

int main() {
  SVGCanvas canv{"spirograph.svg", 500, 500};

  // Clear the image with a soft gray
  canv.setfillcolor(gray(0.9));
  canv.rectangle(Point{0, 0}, Point{canv.getwidth(), canv.getheight()},
                 Action::Fill);

  Point center{canv.getwidth() / 2, canv.getheight() / 2};

  // Draw the spirograph

  const double small_r{42};
  const double large_r{188};
  const double a{75};
  const double tmax{1000};
  const double delta_t{0.2};

  Point old_point{center + spirograph(small_r, large_r, a, 0)};
  for (double t{delta_t}; t < tmax; t += delta_t) {
    Point new_point{center + spirograph(small_r, large_r, a, t)};
    // By smoothly increasing the hue in the call to `hsl`, we're
    // iterating over all the colors of the rainbow
    canv.setstrokecolor(hsl(t / tmax, 1.0, 0.4));
    canv.line(old_point, new_point);
    canv.strokepath();

    old_point = new_point;
  }
}
