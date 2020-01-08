#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QGraphicsRectItem>
#include "objectfeature.h"

class ObjectFeature;
class GameObject;

struct DataBank{
    qreal dx = 0;
    qreal dy = 0;
    int move_action = 0;
    int other_action = 0;
    int action_modifier = 0;
    int deltatime = 1;
    int health = 100;
    int state = 0;
    unsigned int attributes = 0;
    QList<GameObject*> colliding_objects;
    QList<GameObject*> detected_objects;
    QList<int> collected_symbols;
    QList<qreal> physics_packet;
};

class GameObject : public QGraphicsRectItem
{

public:
    GameObject(QGraphicsItem  *parent = nullptr);
    GameObject(qreal boundingrect_x, qreal boundingrect_y, qreal boundingrect_width, qreal boundingrect_height, QGraphicsItem  *parent = nullptr);

    ~GameObject();

    void addFeature(ObjectFeature *feature);
    void setController(ObjectFeature *controller);
    virtual void advanceObject(int delta_time, int phase);
    unsigned int attributes();
    const QList<qreal>* physicsPacket();
    int state();

protected:
    DataBank m_databank;

private:
    QList<ObjectFeature*> m_featureList;
};

#endif // GAMEOBJECT_H
