#pragma once

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace monet {

const char *version = "0.0.2";

////////////////////////////////////////////////////////////////////////////////

struct Point {
  double x, y;

  Point(double ax, double ay) : x(ax), y(ay) {}
};

inline Point operator+(Point a, Point b) { return Point(a.x + b.x, a.y + b.y); }
inline Point operator-(Point a, Point b) { return Point(a.x - b.x, a.y - b.y); }
inline Point operator*(Point p, double f) { return Point(f * p.x, f * p.y); }
inline Point operator*(double f, Point p) { return p * f; }
inline Point operator/(Point p, double f) { return Point(p.x / f, p.y / f); }

////////////////////////////////////////////////////////////////////////////////

struct Color {
  double r, g, b;

  std::string toHTML() const {
    char buf[8];
    snprintf(buf, sizeof(buf) / sizeof(buf[0]), "#%02x%02x%02x", int(r * 255),
             int(g * 255), int(b * 255));
    return std::string(buf);
  }
};

// All the three values h, s, and l must be in the range [0, 1]
inline Color hsl(double h, double s, double l) {
  h = std::fmod(h, 1.0);
  double chroma = (1 - std::fabs(2 * l - 1)) * s;
  double sh = 6 * h;
  double x = chroma * (1 - std::fabs(std::fmod(sh, 2) - 1));

  Color scaled;
  if (sh <= 1)
    scaled = Color{chroma, x, 0};
  else if (sh <= 2)
    scaled = Color{x, chroma, 0};
  else if (sh <= 3)
    scaled = Color{0, chroma, x};
  else if (sh <= 4)
    scaled = Color{0, x, chroma};
  else if (sh <= 5)
    scaled = Color{x, 0, chroma};
  else
    scaled = Color{chroma, 0, x};

  double m = l - chroma / 2;
  return Color{scaled.r + m, scaled.g + m, scaled.b + m};
}

const Color black{0.0, 0.0, 0.0};
const Color darkred{0.5, 0.0, 0.0};
const Color darkgreen{0.0, 0.5, 0.0};
const Color darkblue{0.0, 0.0, 0.5};
const Color brown{0.5, 0.5, 0.0};
const Color darkpurple{0.5, 0.0, 0.5};
const Color darkcyan{0.0, 0.5, 0.5};
const Color lightred{1.0, 0.0, 0.0};
const Color lightgreen{0.0, 1.0, 0.0};
const Color lightblue{0.0, 0.0, 1.0};
const Color yellow{1.0, 1.0, 0.0};
const Color lightpurple{1.0, 0.0, 1.0};
const Color lightcyan{0.0, 1.0, 1.0};
const Color white{1.0, 1.0, 1.0};

const std::vector<Color> plotcolors{Color{1.0, 1.0, 1.0}};

////////////////////////////////////////////////////////////////////////////////

enum class Action { Stroke, Fill, FillAndStroke };

////////////////////////////////////////////////////////////////////////////////

enum class FontFamily { Serif, SansSerif, Monospaced };
enum class HorizontalAlignment { Left, Center, Right };
enum class VerticalAlignment { Top, Center, Middle, Bottom };

////////////////////////////////////////////////////////////////////////////////

class BaseCanvas {
private:
  Color strokecolor;
  Color fillcolor;
  double strokewidth;
  FontFamily fontfamily;
  double fontsize;

protected:
  virtual void canvas2devxy(double inx, double iny, double *outx,
                            double *outy) = 0;
  virtual void movetoxy(double x, double y) = 0;
  virtual void linetoxy(double x, double y) = 0;
  virtual void linexy(double x1, double y1, double x2, double y2) = 0;
  virtual void circlexy(double x, double y, double radius, Action act) = 0;
  virtual void rectanglexy(double x1, double y1, double x2, double y2,
                           Action act) = 0;
  virtual void textxy(double x, double y, const char *text,
                      HorizontalAlignment halign, VerticalAlignment valign) = 0;

  Point canvas2dev(Point p) {
    double devx, devy;
    canvas2devxy(p.x, p.y, &devx, &devy);
    return Point{devx, devy};
  }

public:
  BaseCanvas()
      : strokecolor(black), fillcolor(white), strokewidth(1.0),
        fontfamily(FontFamily::SansSerif), fontsize(12.0) {}
  void setstrokecolor(Color col) { strokecolor = col; }
  void setfillcolor(Color col) { fillcolor = col; }

  Color getstrokecolor() const { return strokecolor; }
  Color getfillcolor() const { return fillcolor; }

  void setstrokewidth(double width) { strokewidth = width; };
  double getstrokewidth() const { return strokewidth; }

  virtual void setfontfamily(FontFamily fam) { fontfamily = fam; }
  virtual void setfontsize(double size) { fontsize = size; }

  virtual FontFamily getfontfamily() const { return fontfamily; }
  virtual double getfontsize() const { return fontsize; }

  void moveto(Point p) {
    Point devpt = canvas2dev(p);
    movetoxy(devpt.x, devpt.y);
  }

  void lineto(Point p) {
    Point devpt = canvas2dev(p);
    linetoxy(devpt.x, devpt.y);
  }

  void line(Point p1, Point p2) {
    Point devpt1 = canvas2dev(p1);
    Point devpt2 = canvas2dev(p2);
    linexy(devpt1.x, devpt1.y, devpt2.x, devpt2.y);
  }

  void circle(Point p, double radius, Action act = Action::Stroke) {
    Point devpt = canvas2dev(p);
    Point devptborder = canvas2dev(p + Point(radius, 0));
    // Note: this assumes that the circle is still a circle when
    // converted from canvas to device coordinates!
    circlexy(devpt.x, devpt.y, std::fabs(devptborder.x - devpt.x), act);
  }

  void rectangle(Point p1, Point p2, Action act = Action::Stroke) {
    Point devpt1 = canvas2dev(p1);
    Point devpt2 = canvas2dev(p2);
    rectanglexy(devpt1.x, devpt1.y, devpt2.x, devpt2.y, act);
  }

  void text(Point p, const std::string &str,
            HorizontalAlignment halign = HorizontalAlignment::Right,
            VerticalAlignment valign = VerticalAlignment::Top) {
    Point devpt = canvas2dev(p);
    HorizontalAlignment dev_halign = halign;
    VerticalAlignment dev_valign = valign;

    // Decide where is "right" and "left"
    Point rightpt = canvas2dev(p + Point(1.0, 0.0));
    if (rightpt.x < devpt.x) {
      // We must flip the horizontal alignment, as this device flips
      // "right" and "left" in the conversion from canvas to device
      // coordinates.
      switch (halign) {
      case HorizontalAlignment::Right:
        dev_halign = HorizontalAlignment::Left;
        break;
      case HorizontalAlignment::Left:
        dev_halign = HorizontalAlignment::Right;
        break;
      default:
        // Useless, but it prevents a warning from the compiler
        dev_halign = halign;
      }
    }

    // Decide where is "up" and "down"
    Point upperpt = canvas2dev(p + Point(0.0, 1.0));
    if (upperpt.y < devpt.y) {
      // We must flip the vertical alignment, as this device flips
      // "top" and "bottom" in the conversion from canvas to device
      // coordinates.
      switch (valign) {
      case VerticalAlignment::Top:
        dev_valign = VerticalAlignment::Bottom;
        break;
      case VerticalAlignment::Bottom:
        dev_valign = VerticalAlignment::Top;
        break;
      default:
        // Useless, but it prevents a warning from the compiler
        dev_valign = valign;
      }
    }
    textxy(devpt.x, devpt.y, str.c_str(), dev_halign, dev_valign);
  }

  virtual void closepath() = 0;
  virtual void strokepath() = 0;
  virtual void fillpath() = 0;
  virtual void fillandstrokepath() {
    fillpath();
    strokepath();
  }

  virtual void begingroup(const std::string &name = "") = 0;
  virtual void endgroup() = 0;
};

////////////////////////////////////////////////////////////////////////////////

class SVGCanvas : public BaseCanvas {
private:
  const int tabwidth = 2;

  std::unique_ptr<std::ostream> stream;
  int indentlevel;
  double width, height;
  std::string pathspec;
  int grouplevel;

  void indent() {
    for (int i = 0; i < tabwidth * indentlevel; ++i) {
      *stream << ' ';
    }
  }

  std::string indentstr(int level) {
    return std::string(tabwidth * level, ' ');
  }

  std::string fontfamilyname() {
    switch (getfontfamily()) {
    case FontFamily::SansSerif:
      return "helvetica";
    case FontFamily::Serif:
      return "times";
    case FontFamily::Monospaced:
      return "courier";
    default:
      abort();
    }
  }

protected:
  void canvas2devxy(double inx, double iny, double *outx,
                    double *outy) override {
    assert(outx != NULL);
    assert(outy != NULL);

    *outx = inx;
    *outy = height - iny;
  }

  void movetoxy(double x, double y) override {
    if (!pathspec.empty())
      pathspec += ' ';

    std::stringstream buf;
    buf << "M" << x << ',' << y;
    pathspec += buf.str();
  }

  void linetoxy(double x, double y) override {
    if (!pathspec.empty())
      pathspec += ' ';

    std::stringstream buf;
    buf << x << ',' << y;
    pathspec += buf.str();
  }

  void linexy(double x1, double y1, double x2, double y2) override {
    assert(stream != nullptr);
    indent();

    *stream << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2
            << "\" y2=\"" << y2 << "\" stroke-width=\"" << getstrokewidth()
            << "\" stroke=\"" << getstrokecolor().toHTML() << "\"/>\n";
  }

  void circlexy(double x, double y, double radius,
                Action act = Action::Stroke) override {
    assert(stream != nullptr);

    indent();
    *stream << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"" << radius;

    std::stringstream buf;
    switch (act) {
    case Action::Stroke:
      buf << "\" stroke=\"" << getstrokecolor().toHTML() << "\" stroke-width=\""
          << getstrokewidth() << "\"/>";
      break;
    case Action::Fill:
      buf << "\" fill=\"" << getfillcolor().toHTML() << "\" stroke=\"none\"/>";
      break;
    case Action::FillAndStroke:
      buf << "\" fill=\"" << getfillcolor().toHTML() << "\" stroke=\""
          << getstrokecolor().toHTML() << "\" stroke-width=\""
          << getstrokewidth() << "\"/>";
      break;
    default:
      abort();
    }

    *stream << buf.rdbuf();
    *stream << '\n';
  }

  void rectanglexy(double x1, double y1, double x2, double y2,
                   Action act = Action::Stroke) override {
    assert(stream != nullptr);
    std::string spaces = indentstr(indentlevel + 1);

    indent();
    *stream << "<rect\n"
            << spaces << "x=\"" << x1 << "\" y=\"" << y1 << "\"\n"
            << spaces << "width=\"" << x2 - x1 << "\" height=\"" << y2 - y1
            << "\"\n"
            << spaces;

    std::stringstream buf;
    switch (act) {
    case Action::Stroke:
      buf << "stroke=\"" << getstrokecolor().toHTML() << "\" stroke-width=\""
          << getstrokewidth() << "\" fill=\"none\"/>";
      break;
    case Action::Fill:
      buf << "stroke=\"none\" fill=\"" << getfillcolor().toHTML() << "\"/>";
      break;
    case Action::FillAndStroke:
      buf << "stroke=\"" << getstrokecolor().toHTML() << "\" stroke-width=\""
          << getstrokewidth() << "\" fill=\"" << getfillcolor().toHTML()
          << "\"/>";
      break;
    default:
      abort();
    }

    *stream << buf.rdbuf();
    *stream << '\n';
  }

  void textxy(double x, double y, const char *text, HorizontalAlignment halign,
              VerticalAlignment valign) override {
    assert(stream != nullptr);

    std::string halign_def;
    switch (halign) {
    case HorizontalAlignment::Left:
      halign_def = "text-anchor=\"end\"";
      break;
    case HorizontalAlignment::Center:
      halign_def = "text-anchor=\"middle\"";
      break;
    case HorizontalAlignment::Right:
      halign_def = "text-anchor=\"start\"";
      break;
    default:
      abort();
    }

    std::string valign_def;
    switch (valign) {
    case VerticalAlignment::Top:
      valign_def = "dominant-baseline=\"text-top\"";
      break;
    case VerticalAlignment::Center:
      valign_def = "dominant-baseline=\"central\"";
      break;
    case VerticalAlignment::Middle:
      valign_def = "dominant-baseline=\"middle\"";
      break;
    case VerticalAlignment::Bottom:
      valign_def = "dominant-baseline=\"text-bottom\"";
      break;
    default:
      abort();
    }

    indent();
    const std::string spaces = indentstr(indentlevel + 1);
    *stream << "<text\n"
            << spaces << "x=\"" << x << "\" y=\"" << y << "\"\n"
            << spaces << halign_def << "\n"
            << spaces << valign_def << "\n"
            << spaces << "font-family=\"" << fontfamilyname()
            << "\" font-size=\"" << getfontsize() << "\" fill=\""
            << getfillcolor().toHTML() << "\">\n"
            << text << "\n";

    indent();
    *stream << "</text>\n";
  }

public:
  SVGCanvas(const std::string &filename, double awidth, double aheight)
      : BaseCanvas(), stream(new std::ofstream(filename.c_str())),
        indentlevel(0), width(awidth), height(aheight), pathspec(""),
        grouplevel(0) {
    if (stream == nullptr) {
      std::perror("Unable to create file");
      std::abort();
    }

    *stream << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
               "<!-- Created with Monet "
            << version
            << " (https://github.com/ziotom78/monet)-->\n"
               "\n"
               "<svg\n"
               "    width=\""
            << width
            << "mm\"\n"
               "    height=\""
            << height
            << "mm\"\n"
               "    viewBox=\"0 0 "
            << width << " " << height
            << "\"\n"
               "    version=\"1.1\"\n"
               "    xmlns=\"http://www.w3.org/2000/svg\">\n";
    indentlevel++;
  }
  void operator=(const SVGCanvas &canvas) = delete;
  ~SVGCanvas() {
    if (stream == nullptr) {
      return;
    }

    // Close any group that was not closed yet
    for (int i = grouplevel; i > 0; i--) {
      endgroup();
    }

    *stream << "</svg>\n";
  }

  bool isok() const { return stream->good(); }

  void closepath() override { pathspec += " z"; }
  void strokepath() override {
    assert(stream != nullptr);
    std::string spaces = indentstr(indentlevel + 1);

    indent();
    *stream << "<path\n"
            << spaces << "d=\"" << pathspec << "\"\n"
            << spaces << "fill=\"none\" stroke=\"" << getstrokecolor().toHTML()
            << "\" stroke-width=\"" << getstrokewidth() << "\"/>\n";
  }

  void fillpath() override {
    assert(stream != nullptr);
    std::string spaces = indentstr(indentlevel + 1);

    indent();
    *stream << "<path\n"
            << spaces << "d=\"" << pathspec << "\"\n"
            << spaces << "fill=\"" << getfillcolor().toHTML()
            << "\" stroke=\"none\"/>\n";
  }

  void fillandstrokepath() override {
    assert(stream != nullptr);
    std::string spaces = indentstr(indentlevel + 1);

    indent();
    *stream << "<path\n"
            << spaces << "d=\"" << pathspec << "\"\n"
            << spaces << "fill=\"" << getfillcolor().toHTML() << "\" stroke=\""
            << getstrokecolor().toHTML() << "\" stroke-width=\""
            << getstrokewidth() << "\"/>\n";
  }

  void begingroup(const std::string &name = "") override {
    assert(stream != nullptr);

    indent();

    if (name.empty()) {
      *stream << "<g>\n";
    } else {
      *stream << "<g name=\"" << name << "\">\n";
    }
    indentlevel++;
    grouplevel++;
  }

  void endgroup() override {
    assert(stream != nullptr);

    if (grouplevel <= 0)
      abort();

    if (indentlevel <= 0)
      abort();

    indentlevel--;
    indent();
    *stream << "</g>\n";

    grouplevel--;
  }
};

}; // namespace monet
