#ifndef FACTORY_OPTIONS_HPP
#define FACTORY_OPTIONS_HPP

struct FactoryOptions
{
  bool isActive = true;

  FactoryOptions(bool p_isActive = true)
    : isActive(p_isActive)
  {
    
  }
};

#endif
