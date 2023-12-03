#include "core/Loop.h"
#include "strategies/ECSStrategy.h"

int main() {
  MinimalLoopStrategy strategy;

  auto loop = Core::Loop({&strategy});
  loop.Run();
}
