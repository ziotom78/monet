#include "monet.hpp"
#include <iostream>

using namespace monet;

int main() {
  SVGCanvas canvas("ref-fonts.svg", 500, 90);

  canvas.setfillcolor(black);

  // @@@START_CODE_SNIPPET@@@
  canvas.setfontfamily(FontFamily::Serif);
  canvas.text(Point(250, 75), "FontFamily::Serif", HorizontalAlignment::Center,
              VerticalAlignment::Center);

  canvas.setfontfamily(FontFamily::SansSerif);
  canvas.text(Point(250, 45), "FontFamily::SansSerif",
              HorizontalAlignment::Center, VerticalAlignment::Center);

  canvas.setfontfamily(FontFamily::Monospaced);
  canvas.text(Point(250, 15), "FontFamily::Monospaced",
              HorizontalAlignment::Center, VerticalAlignment::Center);
  // @@@END_CODE_SNIPPET@@@
}
