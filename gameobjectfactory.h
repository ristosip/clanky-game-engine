#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H

#include <QObject>
#include "gameobject.h"
#include "soundmanager.h"

#define SOUNDMANAGER 0
#define PLAYER 1
#define STEELBARHORIZONTAL 2
#define STEELBARVERTICAL 3
#define LADDER 4
#define ICEFIELD 5
#define GRENADE 6
#define SHRAPNEL 7
#define ENEMY 8
#define GRENADE_SYMBOL_OBJECT 9
#define WORMHOLE 10
#define WORMHOLE_EXIT 11
#define DRONE 12
#define DRONE_SYMBOL_OBJECT 13
#define SURROGATE 14
#define SURROGATE_SYMBOL_OBJECT 15
#define FORCEFIELD 16

class GameObjectFactory : public QObject
{
    Q_OBJECT
public:
    GameObjectFactory(QObject *parent = nullptr);
    ~GameObjectFactory();
    GameObject* create(int object_type);
private:
    GameObject *m_sound_manager;
    bool m_soundmanager_owner;

    GameObject* soundManager();
    GameObject* player();
    GameObject* enemy();
    GameObject* steelbarHorizontal();
    GameObject* steelbarVertical();
    GameObject* ladder();
    GameObject* icefield();
    GameObject* grenade();
    GameObject* shrapnel();
    GameObject* grenadeSymbolObject();
    GameObject* droneSymbolObject();
    GameObject* surrogateSymbolObject();
    GameObject* wormhole();
    GameObject* wormholeExit();
    GameObject* drone();
    GameObject* surrogate();
    GameObject* forcefield();
};

#endif // GAMEOBJECTFACTORY_H
