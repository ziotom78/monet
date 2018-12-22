# Color

A `Color` structure represents a color to be used for painting the
canvas. It contains the following fields:

Field | Type     | Meaning
----- | -------- | ------------------
`r`   | `double` | Red level, in the range [0, 1] (inclusive)
`g`   | `double` | Green level, in the range [0, 1] (inclusive)
`b`   | `double` | Blue level, in the range [0, 1] (inclusive)

The preferred way to create a color is via one of the following
functions (remember, they are not constructors):

- `rgb(r, g, b)` is equivalent to `Color{r, g, b}`, but it is clearer
  in stating the color space;
- `hsl(h, s, l)` initializes the color using the hue, saturation, and
  luminosity values. In this case too, all the parameters must be in
  the range [0, 1] (yes, even the hue).

The only method provided by `Color` is `toHTML`, which returns a
string containing the HTML representation of the color. A `Color`
object can be printed to a stream using the `<<` operator, as shown in
the following example:

Here is an example showing these operators:

{{snippet_from_file("ref-color.cpp", "c++")}}

And here is the output:

{{code_from_file("ref-color.txt", "text")}}


## Standard colors

The library provides a number of predefined colors:

![color table](./ref-color-table.svg)


# Fonts

Fonts are specified through the `FontFamily` enumeration:

Name | Examples
---- | -----------
`Serif` | Garamond, Times New Roman
`SansSerif` | Arial, Helvetica
`Monospaced` | Courier

![font table](./ref-fonts.svg)

# Point

A `Point` structure represents a 2D point on the canvas. It contains
the following fields:

Field | Type     | Meaning
----- | -------- | ------------------
`x`   | `double` | First coordinate
`y`   | `double` | Second coordinate

To initialize a point, you can use one of the following constructs:

- `Point p` initializes `p` to the coordinates `(0, 0)`;
- `Point p(1, 2)` initializes `p` to the coordinates `(1, 2)`;

The `Point` structure implements a number of operators:

Operator | Meaning
-------- | ------------------------------
`+`      | Sum two points together
`-`      | Subtract one point from amother
`*`      | Scale a point by a number (both `f * p` and `p * f` are supported)
`/`      | Divide the coordinates of a point by a number
`+=`     | Increment a point using another point
`-=`     | Subtract one point from another
`*=`     | Scale a point by a number in-place
`/=`     | Divide the coordinates of a point by a number in-place
`<<`     | Print the coordinates of the point to a `std::ostream`

Here is an example showing these operators:

{{snippet_from_file("ref-point.cpp", "c++")}}

And here is the output:

{{code_from_file("ref-point.txt", "text")}}
