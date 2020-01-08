#include "collisiondetection.h"
#include <QGraphicsItem>

void CollisionDetection::update()
{
    QList<QGraphicsItem*> colliding_raw_items =  m_owner->collidingItems();

    // the loop filters out all items other than the bounding rectangles
    for(int i = 0; i < colliding_raw_items.length(); i++)
    {
        if(dynamic_cast<GameObject*>(colliding_raw_items[i]) != nullptr){
            m_databank->colliding_objects.append(dynamic_cast<GameObject*>(colliding_raw_items[i]));
        }
    }
}
