#include "core/Loop.h"

#include "./strategies/MinimalLoopStrategy.h"

int main() {
  MinimalLoopStrategy strategy;

  auto loop = Core::Loop({&strategy});
  loop.Run();
}
