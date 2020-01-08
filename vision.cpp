#include "vision.h"
#include <QGraphicsRectItem>

Vision::Vision(QObject *parent) : ObjectFeature(parent)
{
    m_field_of_view = new QGraphicsRectItem(0, 0, 600, 150);
}

void Vision::setOwner(GameObject *owner)
{
    m_owner = owner;
    m_field_of_view->setParentItem(m_owner);
    m_field_of_view->setPos(-300, -75);
}

void Vision::update()
{
    QList<QGraphicsItem*> detected_raw_items =  m_field_of_view->collidingItems();

    // the loop filters out all items other than the bounding rectangles
    for(int i = 0; i < detected_raw_items.length(); i++)
    {
        if(dynamic_cast<GameObject*>(detected_raw_items[i]) != nullptr){
            m_databank->detected_objects.append(dynamic_cast<GameObject*>(detected_raw_items[i]));
            //qDebug() << dynamic_cast<GameObject*>(detected_raw_items[i])->attributes();
        }
    }
}
