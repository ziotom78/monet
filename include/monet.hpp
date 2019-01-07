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

const char *version = "0.0.7";

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

// The gray level must be in the range [0, 1]
inline Color gray(double level) { return hsl(0.0, 0.0, level); }

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

enum class TransformType { Identity, Translation, Rotation, Scale };

// All the stuff below would have been far clearer, had C++ support
// for algebraic types like Haskell or Rust...

struct Transform {
  TransformType type;
  union {
    Point translation;
    struct {
      Point pivot;
      double angle;
    } rotation;
    Point scale_factor;
  };

  Transform() : type(TransformType::Identity), translation(Point(0.0, 0.0)) {}
};

Transform translate(Point pt) {
  Transform result;
  result.type = TransformType::Translation;
  result.translation = pt;

  return result;
}

Transform rotate(Point pivot, double angle) {
  Transform result;
  result.type = TransformType::Rotation;
  result.rotation.pivot = pivot;
  result.rotation.angle = angle;

  return result;
}

Transform rotate(double angle) {
  Transform result;
  result.type = TransformType::Rotation;
  result.rotation.pivot = Point{0.0, 0.0};
  result.rotation.angle = angle;

  return result;
}

Transform scale(double factorx, double factory) {
  Transform result;
  result.type = TransformType::Scale;
  result.scale_factor = Point{factorx, factory};

  return result;
}

Transform scale(Point factor) { return scale(factor.x, factor.y); }
Transform scale(double factor) { return scale(factor, factor); }
Transform scalex(double factor) { return scale(factor, 1.0); }
Transform scaley(double factor) { return scale(1.0, factor); }

typedef std::vector<Transform> TransformSequence;

const TransformSequence identity{Transform()};

TransformSequence operator|(Transform tr1, Transform tr2) {
  TransformSequence result(2);
  result[0] = tr2;
  result[1] = tr1;

  return result;
}

TransformSequence operator|(TransformSequence seq, Transform tr) {
  size_t input_size = seq.size();
  TransformSequence result(input_size + 1);
  result[0] = tr;
  std::copy(seq.begin(), seq.end(), result.begin() + 1);

  return result;
}

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

  void moveto(Point p) { movetoxy(p.x, p.y); }

  void lineto(Point p) { linetoxy(p.x, p.y); }

  void quadraticto(Point dir, Point end) {
    quadratictoxy(dir.x, dir.y, end.x, end.y);
  }

  void cubicto(Point control1, Point control2, Point end) {
    cubictoxy(control1.x, control1.y, control2.x, control2.y, end.x, end.y);
  }

  void line(Point p1, Point p2) { linexy(p1.x, p1.y, p2.x, p2.y); }

  void circle(Point pt, double radius, Action act = Action::Stroke) {
    circlexy(pt.x, pt.y, radius, act);
  }

  void rectangle(Point p1, Point p2, Action act = Action::Stroke) {
    rectanglexy(p1.x, p1.y, p2.x, p2.y, act);
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

  virtual void begingroup(const TransformSequence &transforms = identity,
                          const std::string &name = "") = 0;
  virtual void endgroup() = 0;
  virtual int grouplevel() const = 0;
};

inline void BaseCanvas::text(Point p, const std::string &str,
                             HorizontalAlignment halign,
                             VerticalAlignment valign) {
  textxy(p.x, p.y, str.c_str(), halign, valign);
}

////////////////////////////////////////////////////////////////////////////////

class SVGCanvas : public BaseCanvas {
private:
  const int tabwidth = 2;

  std::unique_ptr<std::ostream> stream;
  int indentlevel;
  double width, height;
  std::string pathspec;
  int m_grouplevel;

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

  void begingroup(const TransformSequence &transforms = identity,
                  const std::string &name = "") override;
  void endgroup() override;
  int grouplevel() const override { return m_grouplevel; }

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

  // We place the text to (0, 0) and then translate it after reversing the
  // Y axis; otherwise, the text would be flipped vertically (remember that
  // we are using a different coordinate system than SVG's default).
  *stream << "<text\n"
          << spaces << "x=\"0\" y=\"0\"\n"
          << spaces << halign_def << "\n"
          << spaces << valign_def << "\n"
          << spaces << "font-family=\"" << fontfamilyname() << "\" font-size=\""
          << getfontsize() << "\"\n"
          << spaces << "transform=\"translate(" << x << " " << y
          << ") scale(1 -1)\"\n";

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
      width(awidth), height(aheight), pathspec(""), m_grouplevel(0) {
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
  begingroup(scaley(-1) | translate(Point(0, height)), "canvas");
}

inline SVGCanvas::~SVGCanvas() {
  if (stream == nullptr) {
    return;
  }

  // Close any group that was not closed yet
  for (int i = m_grouplevel; i > 0; i--) {
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

inline void SVGCanvas::begingroup(const TransformSequence &transforms,
                                  const std::string &name) {
  assert(stream != nullptr);

  indent();

  *stream << "<g";

  if (!name.empty()) {
    *stream << " name=\"" << name << '\"';
  }

  if (transforms.size() > 1 ||
      (transforms.size() == 1 &&
       transforms[0].type != TransformType::Identity)) {
    *stream << " transform=\"";
    for (const auto &transf : transforms) {
      switch (transf.type) {
      case TransformType::Identity:
        break;

      case TransformType::Translation:
        *stream << "translate(" << transf.translation.x << ' '
                << transf.translation.y << ") ";
        break;

      case TransformType::Rotation:
        *stream << "rotate(" << transf.rotation.angle << ' '
                << transf.rotation.pivot.x << ' ' << transf.rotation.pivot.y
                << ") ";
        break;

      case TransformType::Scale:
        *stream << "scale(" << transf.scale_factor.x << ' '
                << transf.scale_factor.y << ") ";
        break;

      default:
        abort();
      }
    }
    *stream << '\"';
  }

  *stream << ">\n";
  indentlevel++;
  m_grouplevel++;
}

inline void SVGCanvas::endgroup() {
  assert(stream != nullptr);

  if (m_grouplevel <= 0)
    abort();

  if (indentlevel <= 0)
    abort();

  indentlevel--;
  indent();
  *stream << "</g>\n";

  m_grouplevel--;
}

}; // namespace monet
