std::vector<Point> points{Point{10, 10}, Point{10, 70}, Point{250, 50},
                          Point{300, 10}};

// Cubic curve, shown using a thick black line
canvas.setstrokewidth(3);
canvas.setstrokecolor(black);

canvas.moveto(points[0]);
canvas.cubicto(points[1], points[2], points[3]);
canvas.strokepath();
