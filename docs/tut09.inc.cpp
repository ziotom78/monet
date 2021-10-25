const double width{500};
const double height{150};
SVGCanvas canvas{"tut09.svg", width, height};

{
  canvas.defineclip();

  std::vector<Point> points{Point{100, 30}, Point{250, 110}, Point{330, 135},
                            Point{260, 30}};

  canvas.drawpath(points);
  canvas.strokepath(); // canvas.fillpath() would have been ok as well

  canvas.endclip();
}

canvas.useclip();

// Etc.
