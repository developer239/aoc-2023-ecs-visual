#pragma once

namespace ECS {

const unsigned int MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;

}  // namespace ECS