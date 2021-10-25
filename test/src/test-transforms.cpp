#include <cassert>
#include <monet.h>

using namespace monet;

int main() {
  TransformSequence seq{rotate(Point{1.0, 2.0}, 3.0) |
                        translate(Point{4.0, 5.0})};

  assert(seq.size() == 2);

  assert(seq.at(0).type == TransformType::Translation);
  assert(seq.at(0).translation.x == 4.0);
  assert(seq.at(0).translation.y == 5.0);

  assert(seq.at(1).type == TransformType::Rotation);
  assert(seq.at(1).rotation.pivot.x == 1.0);
  assert(seq.at(1).rotation.pivot.y == 2.0);
  assert(seq.at(1).rotation.angle == 3.0);

  seq = seq | scalex(6.0);

  assert(seq.size() == 3);
  assert(seq.at(0).type == TransformType::Scale);
  assert(seq.at(0).scale_factor.x == 6.0);
  assert(seq.at(0).scale_factor.y == 1.0);
}
