#include "monet.h"
#include <cstdio>
#include <string>
#include <vector>

using namespace monet;

struct ColorDescription {
  std::string name;
  Color col;
  Color textcol;
};

const std::vector<ColorDescription> colors{
    ColorDescription{"black", black, white},
    ColorDescription{"darkred", darkred, white},
    ColorDescription{"darkgreen", darkgreen, white},
    ColorDescription{"darkblue", darkblue, white},
    ColorDescription{"brown", brown, white},
    ColorDescription{"darkpurple", darkpurple, white},
    ColorDescription{"darkcyan", darkcyan, white},
    ColorDescription{"red", red, black},
    ColorDescription{"green", green, black},
    ColorDescription{"blue", blue, white},
    ColorDescription{"yellow", yellow, black},
    ColorDescription{"purple", purple, black},
    ColorDescription{"cyan", cyan, black},
    ColorDescription{"lightred", lightred, black},
    ColorDescription{"lightgreen", lightgreen, black},
    ColorDescription{"lightblue", lightblue, black},
    ColorDescription{"lightyellow", yellow, black},
    ColorDescription{"lightpurple", lightpurple, black},
    ColorDescription{"lightcyan", lightcyan, black},
    ColorDescription{"white", white, black},
};

int main() {
  const double barheight = 16;
  SVGCanvas canvas("ref-color-table.svg", 300, barheight * colors.size());

  canvas.setfontsize(8);
  canvas.setfontfamily(FontFamily::Monospaced);

  for (size_t i = 0; i < colors.size(); ++i) {
    Point pt1(0, barheight * i);
    Point pt2(canvas.getwidth(), barheight * (i + 1));

    canvas.setfillcolor(colors[i].col);
    canvas.setstrokecolor(black);
    canvas.rectangle(pt1, pt2, Action::FillAndStroke);

    canvas.setfillcolor(colors[i].textcol);
    canvas.text((pt1 + pt2) / 2, colors[i].name, HorizontalAlignment::Center,
                VerticalAlignment::Center);
  }
}
