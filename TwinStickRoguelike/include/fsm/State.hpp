#ifndef STATE_HPP
#define STATE_HPP
#include <string>

class State
{
public:
  State(std::string p_name);
  virtual ~State() {};

  std::string getName() const;

  virtual void act(float p_dt) = 0;

protected:
  std::string m_name;
};

#endif
