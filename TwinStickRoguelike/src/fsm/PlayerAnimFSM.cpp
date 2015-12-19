#include <fsm/PlayerAnimFSM.hpp>

PlayerAnimFSM::PlayerAnimFSM()
{
  
}

void PlayerAnimFSM::enterState(std::string p_name)
{
  m_currentState = m_states[p_name];
}

void PlayerAnimFSM::exitState(std::string p_name)
{
  m_currentState = nullptr;
}
