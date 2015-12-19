#include <fsm/AFSM.hpp>

AFSM::~AFSM()
{
  m_states.clear();
  m_currentState = nullptr;
}

void AFSM::addState(std::string p_name, State* p_state)
{
  m_states[p_name] = p_state;
}

void AFSM::act(float p_dt) const
{
  m_currentState->act(p_dt);
}

void AFSM::reset()
{
  m_currentState = nullptr;
}
