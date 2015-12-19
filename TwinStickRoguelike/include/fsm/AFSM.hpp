#ifndef A_FSM_HPP
#define A_FSM_HPP

#include <string>
#include <map>

#include <fsm/State.hpp>

class AFSM
{
public:
  virtual ~AFSM();

  void addState(std::string p_name, State* p_state);
  void act(float p_dt) const;
  void reset();

  virtual void enterState(std::string p_name) = 0;
  virtual void exitState(std::string p_name) = 0;

protected:
  std::map<std::string, State*> m_states;
  State* m_currentState = nullptr;
};

#endif
