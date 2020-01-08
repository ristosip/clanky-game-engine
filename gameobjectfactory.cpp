#include "gameobjectfactory.h"
#include "attributesandstates.h"
#include "gameobject.h"
#include "player.h"
#include "controller.h"
#include "collisiondetection.h"
#include "humanoidphysics.h"
#include "humanoidgraphics.h"
#include "surfacephysics.h"
#include "climbablephysics.h"
#include "projectilephysics.h"
#include "dronephysics.h"
#include "thrower.h"
#include "explosive.h"
#include "expiration.h"
#include "humanoidhealth.h"
#include "dronehealth.h"
#include "vision.h"
#include "enemyai.h"
#include "symbol.h"
#include "symbolcollector.h"
#include "wormhole.h"
#include "drone.h"
#include "remotecontrolling.h"
#include "surrogate.h"
#include "soundmanager.h"
#include "humanoidsounds.h"
#include "dronesounds.h"
#include "forcefield.h"
#include "bargraphics.h"

GameObjectFactory::GameObjectFactory(QObject *parent) : QObject(parent)
{
    m_sound_manager = new SoundManager();
    m_soundmanager_owner = true;
}

GameObjectFactory::~GameObjectFactory()
{
    if(m_soundmanager_owner){
        delete m_sound_manager;
    }
}

GameObject* GameObjectFactory::create(int object_type)
{
    switch (object_type) {
        case SOUNDMANAGER:
            return soundManager();
        case PLAYER:
            return  player();
        case SURROGATE:
            return  surrogate();
        case ENEMY:
            return enemy();
        case STEELBARHORIZONTAL:
            return  steelbarHorizontal();
        case STEELBARVERTICAL:
            return  steelbarVertical();
        case LADDER:
            return  ladder();
        case ICEFIELD:
            return  icefield();
        case GRENADE:
            return grenade();
        case SHRAPNEL:
            return shrapnel();
        case GRENADE_SYMBOL_OBJECT:
            return grenadeSymbolObject();
        case DRONE_SYMBOL_OBJECT:
            return droneSymbolObject();
        case SURROGATE_SYMBOL_OBJECT:
            return surrogateSymbolObject();
        case WORMHOLE:
            return wormhole();
        case WORMHOLE_EXIT:
            return wormholeExit();
        case DRONE:
            return drone();
        case FORCEFIELD:
            return forcefield();
        default:
            return nullptr;
    }
}

GameObject* GameObjectFactory::soundManager()
{
    m_soundmanager_owner = false;
    return m_sound_manager;
}

GameObject* GameObjectFactory::player()
{
    GameObject *player = new Player(0, 0, 30, 64);

    HumanoidHealth *health = new HumanoidHealth();
    Explosive *explosive = new Explosive(":/images/player/shrapnel.png", this, static_cast<SoundManager*>(m_sound_manager));
    connect(health, &HumanoidHealth::death, explosive, &Explosive::trigger);
    connect(health, &HumanoidHealth::death, static_cast<Player*>(player), &Player::died);

    // order matters: last one is run last
    player->addFeature(new CollisionDetection());
    player->addFeature(new SymbolCollector);
    player->addFeature(new RemoteControlling(this));
    player->addFeature(new Thrower(GRENADE, 0, this));
    player->addFeature(health);
    player->addFeature(explosive);
    player->addFeature(new HumanoidPhysics(1.0, 1.0, 1.0, PLAYER_CONTROLLED));
    player->addFeature(new HumanoidGraphics(":/images/player/", 0.5));
    player->addFeature(new HumanoidSounds(":/sounds/player/", *static_cast<SoundManager*>(m_sound_manager)));

    return player;
}

GameObject* GameObjectFactory::surrogate()
{
    GameObject *surrogate = new Surrogate(0, 0, 30, 64, static_cast<SoundManager*>(m_sound_manager));

    HumanoidHealth *health = new HumanoidHealth();
    Explosive *explosive = new Explosive(":/images/surrogate/shrapnel.png", this, static_cast<SoundManager*>(m_sound_manager));
    connect(health, &HumanoidHealth::death, explosive, &Explosive::trigger);
    connect(health, &HumanoidHealth::death, static_cast<Surrogate*>(surrogate), &Surrogate::crashed);

    Expiration *expiration = new Expiration(30000, true);
    connect(expiration, &Expiration::expired, static_cast<Surrogate*>(surrogate), &Surrogate::batteryDied);

    Expiration *expiration2 = new Expiration(33000, true);
    connect(expiration2, &Expiration::expired, explosive, &Explosive::trigger);
    connect(expiration2, &Expiration::expired, static_cast<Surrogate*>(surrogate), &Surrogate::crashed);

    // order matters: last one is run last
    surrogate->addFeature(new CollisionDetection());
    surrogate->addFeature(new SymbolCollector);
    surrogate->addFeature(new RemoteControlling(this));
    surrogate->addFeature(new Thrower(GRENADE, 0, this));
    surrogate->addFeature(health);
    surrogate->addFeature(explosive);
    surrogate->addFeature(expiration);
    surrogate->addFeature(expiration2);
    surrogate->addFeature(new HumanoidPhysics(1.0, 1.0, 1.0, PLAYER_CONTROLLED));
    surrogate->addFeature(new HumanoidGraphics(":/images/surrogate/", 0.5));
    surrogate->addFeature(new HumanoidSounds(":/sounds/player/", *static_cast<SoundManager*>(m_sound_manager)));

    return surrogate;
}

GameObject* GameObjectFactory::enemy()
{
    GameObject *enemy = new GameObject(0, 0, 30, 64);

    HumanoidHealth *health = new HumanoidHealth();
    Explosive *explosive = new Explosive(":/images/enemy/shrapnel.png", this);
    connect(health, &HumanoidHealth::death, explosive, &Explosive::trigger);

    // order matters: last one is run last
    enemy->addFeature(new CollisionDetection());
    enemy->addFeature(new Vision());
    enemy->addFeature(new EnemyAI());
    enemy->addFeature(new Thrower(GRENADE, 100, this));
    enemy->addFeature(health);
    enemy->addFeature(explosive);
    enemy->addFeature(new HumanoidPhysics());
    enemy->addFeature(new HumanoidGraphics(":/images/enemy/", 0.5));

    return enemy;
}

GameObject* GameObjectFactory::steelbarHorizontal()
{
    GameObject *steelbar = new GameObject(0, 0, 200, 20);

    // order matters: last one is run last
    steelbar->addFeature(new CollisionDetection());
    steelbar->addFeature(new SurfacePhysics());
    steelbar->addFeature(new BarGraphics(":/images/misc/steelbar_h.png", 0.5));

    return steelbar;
}

GameObject* GameObjectFactory::steelbarVertical()
{
    GameObject *steelbar = new GameObject(0, 0, 20, 200);

    // order matters: last one is run last
    steelbar->addFeature(new CollisionDetection());
    steelbar->addFeature(new SurfacePhysics());
    steelbar->addFeature(new BarGraphics(":/images/misc/steelbar_v.png", 0.5));

    return steelbar;
}

GameObject* GameObjectFactory::ladder()
{
    GameObject *ladder = new GameObject(0, 0, 5, 200);

    // order matters: last one is run last
    ladder->addFeature(new CollisionDetection());
    ladder->addFeature(new ClimbablePhysics());
    ladder->addFeature(new BarGraphics(":/images/misc/ladder.png", 0.5, -7.5));

    return ladder;
}

GameObject* GameObjectFactory::icefield()
{
    GameObject *icefield = new GameObject(0, 0, 200, 20);

    // order matters: last one is run last
    icefield->addFeature(new CollisionDetection());
    icefield->addFeature(new SurfacePhysics(0.01, 0.125));
    icefield->addFeature(new BarGraphics(":/images/misc/icefield.png", 0.5));

    return icefield;
}

GameObject* GameObjectFactory::grenade()
{
    GameObject *grenade = new GameObject(0, 0, 10, 10);

    Expiration *expiration = new Expiration(2500);
    Explosive *explosive = new Explosive(":/images/misc/shrapnel.png", this, static_cast<SoundManager*>(m_sound_manager), 0.5);
    connect(expiration, &Expiration::expired, explosive, &Explosive::trigger);

    // order matters: last one is run last
    grenade->addFeature(new CollisionDetection());
    grenade->addFeature(expiration);
    grenade->addFeature(explosive);
    grenade->addFeature(new ProjectilePhysics(.2, 1.0));
    grenade->addFeature(new BarGraphics(":/images/misc/grenade.png", 0.5));

    return grenade;
}

GameObject* GameObjectFactory::shrapnel()
{
    GameObject *shrapnel = new GameObject(0, 0, 7, 7);

    // order matters: last one is run last
    shrapnel->addFeature(new CollisionDetection());
    shrapnel->addFeature(new Expiration(1500, true, false, true));
    shrapnel->addFeature(new ProjectilePhysics(.02, 1.0, PIERCING));

    return shrapnel;
}

GameObject* GameObjectFactory::grenadeSymbolObject()
{
    GameObject *grenade_symbol_object = new Symbol(GRENADE_SYMBOL, ":/images/misc/grenade.png");
    grenade_symbol_object->addFeature(new Expiration(5000, true));

    return grenade_symbol_object;
}

GameObject* GameObjectFactory::droneSymbolObject()
{
    GameObject *drone_symbol_object = new Symbol(DRONE_SYMBOL, ":/images/misc/drone.png", 0.7);
    drone_symbol_object->addFeature(new Expiration(50000, true));

    return drone_symbol_object;
}

GameObject* GameObjectFactory::surrogateSymbolObject()
{
    GameObject *drone_symbol_object = new Symbol(SURROGATE_SYMBOL, ":/images/surrogate/head.png", 0.5);
    drone_symbol_object->addFeature(new Expiration(50000, true));

    return drone_symbol_object;
}

GameObject* GameObjectFactory::wormhole()
{
    GameObject *wormhole = new Wormhole(":/images/misc/wormhole.png", 0.5, this);
    Expiration *expiration = new Expiration(3500);
    connect(expiration, &Expiration::expired, static_cast<Wormhole*>(wormhole), &Wormhole::closeWormhole);
    wormhole->addFeature(expiration);

    return wormhole;
}

GameObject* GameObjectFactory::wormholeExit()
{
    GameObject *wormhole_exit = new Wormhole(":/images/misc/wormhole.png", 0.5, this, true);
    Expiration *expiration = new Expiration(400);
    connect(expiration, &Expiration::expired, static_cast<Wormhole*>(wormhole_exit), &Wormhole::closeWormhole);
    wormhole_exit->addFeature(expiration);

    return wormhole_exit;
}

GameObject* GameObjectFactory::drone()
{
    GameObject *drone = new Drone(":/images/misc/drone.png", 20, 20, 1.0, static_cast<SoundManager*>(m_sound_manager));
    Expiration *expiration = new Expiration(60000);
    connect(expiration, &Expiration::expired, static_cast<Drone*>(drone), &Drone::batteryDied);
    Expiration *expiration2 = new Expiration(63000);
    DroneHealth *health = new DroneHealth();
    Explosive *explosive = new Explosive(":/images/misc/shrapnel.png", this, static_cast<SoundManager*>(m_sound_manager));
    connect(expiration2, &Expiration::expired, explosive, &Explosive::trigger);
    connect(expiration2, &Expiration::expired, static_cast<Drone*>(drone), &Drone::crashed);
    connect(health, &DroneHealth::death, explosive, &Explosive::trigger);
    connect(health, &DroneHealth::death, static_cast<Drone*>(drone), &Drone::crashed);

    // order matters: last one is run last
    drone->addFeature(new CollisionDetection());
    drone->addFeature(new SymbolCollector());
    drone->addFeature(new RemoteControlling(this));
    drone->addFeature(expiration);
    drone->addFeature(expiration2);
    drone->addFeature(health);
    drone->addFeature(explosive);
    drone->addFeature(new DronePhysics(0.5, 0.2, 0.5, PLAYER_CONTROLLED));
    drone->addFeature(new DroneSounds(":/sounds/drone/", *static_cast<SoundManager*>(m_sound_manager)));

    return drone;
}

GameObject* GameObjectFactory::forcefield()
{
    GameObject *forcefield = new ForceField(100, 100);
    static_cast<ForceField*>(forcefield)->setImage(":/images/misc/forcefield.png", 0.5);

    return forcefield;
}
