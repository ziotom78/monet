#pragma once

/*
 * Monet - A C++11 header-only vector graphics library
 * https://github.com/ziotom78/monet
 *
 * Copyright 2018 Maurizio Tomasi
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace monet {

const char *version = "0.0.6";

////////////////////////////////////////////////////////////////////////////////

struct Point {
  double x, y;

  Point() : x(0), y(0) {}
  Point(double ax, double ay) : x(ax), y(ay) {}

  inline void operator+=(Point p) {
    x += p.x;
    y += p.y;
  }

  inline void operator-=(Point p) {
    x -= p.x;
    y -= p.y;
  }

  inline void operator*=(double f) {
    x *= f;
    y *= f;
  }

  inline void operator/=(double f) {
    x /= f;
    y /= f;
  }
};

inline Point operator+(Point a, Point b) { return Point(a.x + b.x, a.y + b.y); }
inline Point operator-(Point a, Point b) { return Point(a.x - b.x, a.y - b.y); }
inline Point operator*(Point p, double f) { return Point(f * p.x, f * p.y); }
inline Point operator*(double f, Point p) { return p * f; }
inline Point operator/(Point p, double f) { return Point(p.x / f, p.y / f); }
inline std::ostream &operator<<(std::ostream &out, Point p) {
  out << '(' << p.x << ", " << p.y << ')';
  return out;
}

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

inline std::ostream &operator<<(std::ostream &out, Color col) {
  out << "rgb[" << col.r << ", " << col.g << ", " << col.b << ']';
  return out;
}

// All the three values r, g, and b must be in the range [0, 1]
inline Color rgb(double r, double g, double b) { return Color{r, g, b}; }

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
const Color red{1.0, 0.0, 0.0};
const Color green{0.0, 1.0, 0.0};
const Color blue{0.0, 0.0, 1.0};
const Color yellow{1.0, 1.0, 0.0};
const Color purple{1.0, 0.0, 1.0};
const Color cyan{0.0, 1.0, 1.0};
const Color lightred{1.0, 0.5, 0.5};
const Color lightgreen{0.5, 1.0, 0.5};
const Color lightblue{0.5, 0.5, 1.0};
const Color lightyellow{1.0, 1.0, 0.5};
const Color lightpurple{1.0, 0.5, 1.0};
const Color lightcyan{0.5, 1.0, 1.0};
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
  double transparency;

protected:
  virtual void canvas2devxy(double inx, double iny, double *outx,
                            double *outy) = 0;
  virtual void movetoxy(double x, double y) = 0;
  virtual void linetoxy(double x, double y) = 0;
  virtual void quadratictoxy(double xdir, double ydir, double xend,
                             double yend) = 0;
  virtual void cubictoxy(double xc1, double yc1, double xc2, double yc2,
                         double xend, double yend) = 0;
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
        fontfamily(FontFamily::SansSerif), fontsize(12.0), transparency(0.0) {}
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

  virtual double getwidth() const = 0;
  virtual double getheight() const = 0;

  void settransparency(double tr) { transparency = tr; }
  double gettransparency() const { return transparency; }

  void moveto(Point p) {
    Point devpt = canvas2dev(p);
    movetoxy(devpt.x, devpt.y);
  }

  void lineto(Point p) {
    Point devpt = canvas2dev(p);
    linetoxy(devpt.x, devpt.y);
  }

  void quadraticto(Point dir, Point end) {
    Point devdir = canvas2dev(dir);
    Point devend = canvas2dev(end);
    quadratictoxy(devdir.x, devdir.y, devend.x, devend.y);
  }

  void cubicto(Point control1, Point control2, Point end) {
    Point devc1 = canvas2dev(control1);
    Point devc2 = canvas2dev(control2);
    Point devend = canvas2dev(end);
    cubictoxy(devc1.x, devc1.y, devc2.x, devc2.y, devend.x, devend.y);
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
            VerticalAlignment valign = VerticalAlignment::Top);

  virtual void closepath() = 0;
  virtual void strokepath() = 0;
  virtual void fillpath() = 0;
  virtual void fillandstrokepath() {
    fillpath();
    strokepath();
  }
  virtual void clearpath() = 0;

  virtual void begingroup(const std::string &name = "") = 0;
  virtual void endgroup() = 0;
};

inline void BaseCanvas::text(Point p, const std::string &str,
                             HorizontalAlignment halign,
                             VerticalAlignment valign) {
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

  std::string fontfamilyname() const;

protected:
  void canvas2devxy(double inx, double iny, double *outx,
                    double *outy) override;
  void movetoxy(double x, double y) override;
  void linetoxy(double x, double y) override;
  void quadratictoxy(double xdir, double ydir, double xend,
                     double yend) override;
  void cubictoxy(double xc1, double yc1, double xc2, double yc2, double xend,
                 double yend) override;
  void linexy(double x1, double y1, double x2, double y2) override;
  void circlexy(double x, double y, double radius,
                Action act = Action::Stroke) override;
  void rectanglexy(double x1, double y1, double x2, double y2,
                   Action act = Action::Stroke) override;
  void textxy(double x, double y, const char *text, HorizontalAlignment halign,
              VerticalAlignment valign) override;

public:
  SVGCanvas(const std::string &filename, double awidth, double aheight);
  void operator=(const SVGCanvas &canvas) = delete;
  virtual ~SVGCanvas();

  bool isok() const { return stream->good(); }

  void closepath() override { pathspec += " z"; }
  void strokepath() override;
  void fillpath() override;
  void fillandstrokepath() override;
  void clearpath() override { pathspec = ""; }

  void begingroup(const std::string &name = "") override;
  void endgroup() override;

  double getwidth() const override { return width; }
  double getheight() const override { return height; }
};

inline std::string SVGCanvas::fontfamilyname() const {
  switch (getfontfamily()) {
  case FontFamily::SansSerif:
    return "sans-serif";
  case FontFamily::Serif:
    return "serif";
  case FontFamily::Monospaced:
    return "monospace";
  default:
    abort();
  }
}

inline void SVGCanvas::canvas2devxy(double inx, double iny, double *outx,
                                    double *outy) {
  assert(outx != NULL);
  assert(outy != NULL);

  *outx = inx;
  *outy = height - iny;
}

inline void SVGCanvas::movetoxy(double x, double y) {
  if (!pathspec.empty())
    pathspec += ' ';

  std::stringstream buf;
  buf << "M " << x << ',' << y;
  pathspec += buf.str();
}

inline void SVGCanvas::linetoxy(double x, double y) {
  if (!pathspec.empty())
    pathspec += ' ';

  std::stringstream buf;
  buf << x << ',' << y;
  pathspec += buf.str();
}

inline void SVGCanvas::quadratictoxy(double xdir, double ydir, double xend,
                                     double yend) {
  if (!pathspec.empty())
    pathspec += ' ';

  std::stringstream buf;
  buf << "Q " << xdir << ',' << ydir << ' ' << xend << ',' << yend;
  pathspec += buf.str();
}

inline void SVGCanvas::cubictoxy(double xc1, double yc1, double xc2, double yc2,
                                 double xend, double yend) {
  if (!pathspec.empty())
    pathspec += ' ';

  std::stringstream buf;
  buf << "C " << xc1 << ',' << yc1 << ' ' << xc2 << ',' << yc2 << ' ' << xend
      << ',' << yend;
  pathspec += buf.str();
}

inline void SVGCanvas::linexy(double x1, double y1, double x2, double y2) {
  assert(stream != nullptr);
  indent();

  *stream << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2
          << "\" y2=\"" << y2 << "\" stroke-width=\"" << getstrokewidth()
          << "\" stroke=\"" << getstrokecolor().toHTML() << '\"';

  if (gettransparency() > 0)
    *stream << " opacity=\"" << 1 - gettransparency() << '\"';

  *stream << "/>\n";
}

inline void SVGCanvas::circlexy(double x, double y, double radius, Action act) {
  assert(stream != nullptr);

  indent();
  *stream << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"" << radius;

  std::stringstream buf;
  switch (act) {
  case Action::Stroke:
    buf << "\" stroke=\"" << getstrokecolor().toHTML() << "\" stroke-width=\""
        << getstrokewidth() << '\"';
    break;
  case Action::Fill:
    buf << "\" fill=\"" << getfillcolor().toHTML() << "\" stroke=\"none\"";
    break;
  case Action::FillAndStroke:
    buf << "\" fill=\"" << getfillcolor().toHTML() << "\" stroke=\""
        << getstrokecolor().toHTML() << "\" stroke-width=\"" << getstrokewidth()
        << '\"';
    break;
  default:
    abort();
  }

  *stream << buf.rdbuf();

  if (gettransparency() > 0)
    *stream << " opacity=\"" << 1 - gettransparency() << '\"';

  *stream << "/>\n";
}

inline void SVGCanvas::rectanglexy(double x1, double y1, double x2, double y2,
                                   Action act) {
  assert(stream != nullptr);
  std::string spaces = indentstr(indentlevel + 1);

  indent();
  *stream << "<rect\n"
          << spaces << "x=\"" << std::min(x1, x2) << "\" y=\""
          << std::min(y1, y2) << "\"\n"
          << spaces << "width=\"" << std::fabs(x2 - x1) << "\" height=\""
          << std::fabs(y2 - y1) << "\"\n"
          << spaces;

  std::stringstream buf;
  switch (act) {
  case Action::Stroke:
    buf << "stroke=\"" << getstrokecolor().toHTML() << "\" stroke-width=\""
        << getstrokewidth() << "\" fill=\"none\"";
    break;
  case Action::Fill:
    buf << "stroke=\"none\" fill=\"" << getfillcolor().toHTML() << '\"';
    break;
  case Action::FillAndStroke:
    buf << "stroke=\"" << getstrokecolor().toHTML() << "\" stroke-width=\""
        << getstrokewidth() << "\" fill=\"" << getfillcolor().toHTML() << '\"';
    break;
  default:
    abort();
  }

  *stream << buf.rdbuf();

  if (gettransparency() > 0)
    *stream << " opacity=\"" << 1 - gettransparency() << '\"';

  *stream << "/>\n";
}

inline void SVGCanvas::textxy(double x, double y, const char *text,
                              HorizontalAlignment halign,
                              VerticalAlignment valign) {
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
          << spaces << "font-family=\"" << fontfamilyname() << "\" font-size=\""
          << getfontsize() << "\"\n";

  if (gettransparency() > 0)
    *stream << spaces << "opacity=\"" << 1 - gettransparency() << "\"\n";

  *stream << spaces << "fill=\"" << getfillcolor().toHTML() << "\">\n"
          << text << "\n";

  indent();
  *stream << "</text>\n";
}

inline SVGCanvas::SVGCanvas(const std::string &filename, double awidth,
                            double aheight)
    : BaseCanvas(), stream(new std::ofstream(filename.c_str())), indentlevel(0),
      width(awidth), height(aheight), pathspec(""), grouplevel(0) {
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
          << width << ' ' << height
          << "\"\n"
             "    version=\"1.1\"\n"
             "    xmlns=\"http://www.w3.org/2000/svg\">\n";
  indentlevel++;
}

inline SVGCanvas::~SVGCanvas() {
  if (stream == nullptr) {
    return;
  }

  // Close any group that was not closed yet
  for (int i = grouplevel; i > 0; i--) {
    endgroup();
  }

  *stream << "</svg>\n";
}

inline void SVGCanvas::strokepath() {
  assert(stream != nullptr);
  std::string spaces = indentstr(indentlevel + 1);

  indent();
  *stream << "<path\n"
          << spaces << "d=\"" << pathspec << "\"\n"
          << spaces << "fill=\"none\" stroke=\"" << getstrokecolor().toHTML()
          << "\" stroke-width=\"" << getstrokewidth() << "\"/>\n";
}

inline void SVGCanvas::fillpath() {
  assert(stream != nullptr);
  std::string spaces = indentstr(indentlevel + 1);

  indent();
  *stream << "<path\n"
          << spaces << "d=\"" << pathspec << "\"\n"
          << spaces << "fill=\"" << getfillcolor().toHTML()
          << "\" stroke=\"none\"/>\n";
}

inline void SVGCanvas::fillandstrokepath() {
  assert(stream != nullptr);
  std::string spaces = indentstr(indentlevel + 1);

  indent();
  *stream << "<path\n"
          << spaces << "d=\"" << pathspec << "\"\n"
          << spaces << "fill=\"" << getfillcolor().toHTML() << "\" stroke=\""
          << getstrokecolor().toHTML() << "\" stroke-width=\""
          << getstrokewidth() << "\"/>\n";
}

inline void SVGCanvas::begingroup(const std::string &name) {
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

inline void SVGCanvas::endgroup() {
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

}; // namespace monet
