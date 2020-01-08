#include "gameobject.h"
#include "objectfeature.h"
#include <QGraphicsRectItem>

GameObject::GameObject(QGraphicsItem  *parent) : QGraphicsRectItem(parent)
{
    setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
    setOpacity(0);
}

GameObject::GameObject(qreal boundingrect_x, qreal boundingrect_y, qreal boundingrect_width, qreal boundingrect_height, QGraphicsItem  *parent) : QGraphicsRectItem(boundingrect_x, boundingrect_y, boundingrect_width, boundingrect_height, parent)
{
    setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
    setOpacity(0);
}

GameObject::~GameObject()
{
    //destroy features
    while (!m_featureList.isEmpty())
        delete m_featureList.takeFirst();
}

void GameObject::addFeature(ObjectFeature *feature)
{
    if(feature != nullptr){
        feature->setOwner(this);
        feature->setDataBank(m_databank);
        m_featureList.append(feature);
    }
}

void GameObject::setController(ObjectFeature *controller)
{
    if(controller != nullptr){
        controller->setOwner(this);
        controller->setDataBank(m_databank);
        if(!m_featureList.isEmpty())
            m_featureList.insert(0, controller);
        else
            m_featureList.append(controller);
    }
}

unsigned int GameObject::attributes()
{
    return m_databank.attributes;
}

const QList<qreal>* GameObject::physicsPacket()
{
    return &m_databank.physics_packet;
}

int GameObject::state()
{
    return m_databank.state;
}

void GameObject::advanceObject(int delta_time, int phase)
{
    if(phase == 1){
        m_databank.deltatime += delta_time; // deltatime is the sum of both phases

        for (int i = 0; i < m_featureList.size(); ++i) {
        m_featureList.at(i)->update();
        }
    }
    else{
        m_databank.deltatime = delta_time;
        if(!(m_databank.dx == 0.0 && m_databank.dy == 0.0))
            this->moveBy(m_databank.dx, m_databank.dy);

        m_databank.dx = 0.0;
        m_databank.dy = 0.0;
        m_databank.move_action = 0;
        m_databank.other_action = 0;
        m_databank.action_modifier = 0;
        m_databank.colliding_objects.clear();
        m_databank.detected_objects.clear();
        m_databank.collected_symbols.clear();
    }
}
