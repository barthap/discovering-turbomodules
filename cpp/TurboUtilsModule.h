#ifndef TURBOUTILSMODULE_H
#define TURBOUTILSMODULE_H

#include <jsi/jsi.h>

namespace turboutils {
  double sumSquares(double a, double b);

  void installJsi(facebook::jsi::Runtime& rt);
}

#endif /* TURBOUTILSMODULE_H */
