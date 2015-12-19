#ifndef PLAYER_ANIM_FSM_HPP
#define PLAYER_ANIM_FSM_HPP

#include <fsm/AFSM.hpp>

class PlayerAnimFSM : AFSM
{
public:
  PlayerAnimFSM();

  void enterState(std::string p_name) override;
  void exitState(std::string p_name) override;
};

#endif