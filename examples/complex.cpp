#include <monet.h>

using namespace monet;

int main() {
  SVGCanvas canv{"complex.svg", 500, 500};

  // Create a closed path
  canv.moveto(Point{});
  canv.lineto(Point{100, 0});
  canv.lineto(Point{100, 100});
  canv.closepath();
  canv.strokepath();

  // Paths can be stroked and filled at the same time
  canv.setfillcolor(Color{0.8, 0.7, 0.3});
  canv.setstrokecolor(brown);
  canv.setstrokewidth(3.0);
  canv.circle(Point{200, 200}, 150, Action::FillAndStroke);

  // You can specify colors using HSL
  canv.setstrokecolor(hsl(0.2, 0.5, 0.4));
  canv.setstrokewidth(8.0);
  canv.rectangle(Point{400, 100}, Point{500, 150}, Action::Stroke);

  // You can group graphical elements (useful if you plan to
  // open the SVG in a vector graphics program like Inkscape)
  canv.begingroup(TransformSequence{translate(Point{300, 250})});
  {
    Point textpoint{Point{0, 0}};
    canv.setfontsize(48);
    canv.setfontfamily(FontFamily::Monospaced);
    canv.setfillcolor(black);
    canv.text(textpoint, "Hello, world!", HorizontalAlignment::Center,
              VerticalAlignment::Bottom);

    // Highlight the pivot of the text element by
    // drawing a small circle around it
    canv.setfillcolor(red);
    canv.circle(textpoint, 5, Action::Fill);
  }
  canv.endgroup();
}
