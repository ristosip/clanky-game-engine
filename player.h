#ifndef PLAYER_H
#define PLAYER_H

#include "gameobject.h"
#include <QObject>

class Player : public QObject, public GameObject
{
    Q_OBJECT
public:
    Player(qreal boundingrect_x, qreal boundingrect_y, qreal boundingrect_width, qreal boundingrect_height, QGraphicsItem  *parent = nullptr);

signals:
    void died();
};

#endif // PLAYER_H
