#include "gameobjecteater.h"
#include "gamescene.h"
#include "collisiondetection.h"
#include "player.h"
#include "surrogate.h"
#include "attributesandstates.h"

GameObjectEater::GameObjectEater(qreal width, qreal height, QGraphicsObject *parent) : QObject(), GameObject(0, 0, width, height, parent)
{
    addFeature(new CollisionDetection);
}

void GameObjectEater::advanceObject(int delta_time, int phase)
{
    GameObject::advanceObject(delta_time, phase);
    if(phase == 1){
        for(int i = 0; i < m_databank.colliding_objects.length(); i++){

            unsigned int temp_mask = 1<<PLAYER_CONTROLLED;

            if(!(m_databank.colliding_objects.at(i)->attributes() & temp_mask)){
                static_cast<GameScene*>(scene())->removeGameObject(m_databank.colliding_objects.at(i));
            }

            if(dynamic_cast<Player*>(m_databank.colliding_objects.at(i)) != nullptr){
                emit playerEaten();
            }
        }
    }
}
