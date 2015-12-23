#ifndef HIT_ONCE_COMPONENT_HPP
#define HIT_ONCE_COMPONENT_HPP

#include <ecstasy/core/Component.h>

struct HitOnceComponent : public ECS::Component<HitOnceComponent>
{
  std::vector<uint64_t> alreadyHit;

  void reset() override
  {
    alreadyHit.clear();
  }
};

#endif
