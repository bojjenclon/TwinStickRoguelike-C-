#ifndef UI_VALUES_HPP
#define UI_VALUES_HPP

class UIValues
{
public:
  struct PlayerValues
  {
    int currentHealth = 5;
    int maxHealth = 5;
    bool healthChanged = false;
  };

  struct EnemyValues
  {
    bool display = false;
    bool displayChanged = false;

    int currentHealth = 5;
    int maxHealth = 5;
    bool healthChanged = false;
  };

  UIValues();

  PlayerValues player;
  EnemyValues enemy;
};

#endif
