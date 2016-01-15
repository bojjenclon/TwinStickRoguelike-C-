#ifndef CONTACT_LISTENER_HPP
#define CONTACT_LISTENER_HPP

#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <collisions/CollisionData.hpp>

class ContactListener : public b2ContactListener
{
public:
  void BeginContact(b2Contact* p_contact) override;
  void EndContact(b2Contact* p_contact) override;

private:
  static void PlayerEnemyContactBegin(CollisionData* p_dataA, CollisionData* p_dataB);
  static void BulletContactBegin(CollisionData* p_dataA, CollisionData* p_dataB);
  static void PlayerExitContactBegin(CollisionData* p_dataA, CollisionData* p_dataB);
};

#endif
