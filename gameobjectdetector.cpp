#include "gameobjectdetector.h"
#include "gamescene.h"

GameObjectDetector::GameObjectDetector(GameObject *target, qreal width, qreal heigth, QGraphicsItem *parent) :  QObject(), GameObject(0, 0, width, heigth, parent)
{
    m_target_object = target;
}

void GameObjectDetector::setTargetObject(GameObject *target)
{
    m_target_object = target;
}

void GameObjectDetector::advanceObject(int delta_time, int phase)
{
    if(m_target_object != nullptr){
        if(phase == 1){
            QList<QGraphicsItem*> colliding_raw_items =  this->collidingItems();

            // the loop filters out all items other than the bounding rectangles
            for(int i = 0; i < colliding_raw_items.length(); i++)
            {
                if(dynamic_cast<GameObject*>(colliding_raw_items[i]) == m_target_object){
                    emit targetDetected();
                    static_cast<GameScene*>(this->scene())->removeGameObject(this);
                }
            }
        }
    }
}


