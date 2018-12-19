#include <monet.hpp>

using namespace monet;

int main() {
  SVGCanvas canv("output.svg", 500, 500);
  canv.moveto(Point(0.0, 0.0));
  canv.lineto(Point(100.0, 0.0));
  canv.lineto(Point(100.0, 100.0));
  canv.closepath();
  canv.strokepath();

  canv.setfillcolor(Color{0.8, 0.7, 0.3});
  canv.setstrokecolor(brown);
  canv.setstrokewidth(3.0);
  canv.circle(Point(200, 200), 150, Action::FillAndStroke);

  canv.setstrokecolor(hsl(0.2, 0.5, 0.4));
  canv.setstrokewidth(8.0);
  canv.rectangle(Point(400, 100), Point(500, 150), Action::Stroke);

  canv.begingroup("textandpoint");
  Point textpoint = Point(300, 250);
  canv.setfillcolor(black);
  canv.circle(textpoint, 5, Action::Fill);

  canv.setfontsize(48);
  canv.setfontfamily(FontFamily::Monospaced);
  canv.text(textpoint, "Hello, world!", HorizontalAlignment::Center,
            VerticalAlignment::Bottom);
  canv.endgroup();
}
