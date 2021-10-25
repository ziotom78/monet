#include "monet.h"

using namespace monet;

int main() {
// @@@START_CODE_SNIPPET@@@
#include "tut09.inc.cpp"
  // @@@END_CODE_SNIPPET@@@

  canvas.setfontsize(10);

  // Width of the colored rectangle bar on the right
  const double leftside{width * 0.8};
  const double rightside{width - leftside};
  const int hue_nsteps{10};
  const double square_height{height / 3};

  // Use a fixed saturation for all the squares
  const double saturation{1.0};

  // Draw three rows of squares
  const std::vector<double> lightness_levels{0.33, 0.50, 0.67};
  for (size_t row_num{}; row_num < lightness_levels.size(); ++row_num) {
    double lightness = lightness_levels[row_num];

    // Inner loop: draw one row
    for (double hue_step{}; hue_step < hue_nsteps; ++hue_step) {
      double hue = (hue_step + 0.5) / hue_nsteps;
      canvas.setfillcolor(hsl(hue, saturation, lightness));

      // Plot the colored rectangle
      Point p1{hue_step * leftside / hue_nsteps, row_num * square_height};
      Point p2{(hue_step + 1) * leftside / hue_nsteps,
               (row_num + 1) * square_height};
      canvas.rectangle(p1, p2, Action::FillAndStroke);

      // Print the hue level over the rectangle
      char text[64];
      std::sprintf(text, "%.0f%%", hue * 100);
      Point textcenter{(p1 + p2) / 2};
      canvas.setfillcolor(black);
      canvas.text(textcenter, text, HorizontalAlignment::Center,
                  VerticalAlignment::Center);

      // Print the lightness level on the right side of the plot
      std::sprintf(text, "L: %.0f%%", lightness * 100);
      canvas.text(Point(width - rightside / 2, textcenter.y), text,
                  HorizontalAlignment::Center, VerticalAlignment::Center);
    }
  }
}
