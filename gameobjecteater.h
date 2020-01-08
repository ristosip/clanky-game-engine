#ifndef GAMEOBJECTEATER_H
#define GAMEOBJECTEATER_H

#include "gameobject.h"

class GameObjectEater : public QObject, public GameObject{
    Q_OBJECT
public:
    GameObjectEater(qreal width, qreal height, QGraphicsObject *parent = nullptr);
    void advanceObject(int delta_time, int phase);

signals:
    void playerEaten();
};

#endif // GAMEOBJECTEATER_H
