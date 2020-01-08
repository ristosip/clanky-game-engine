#ifndef GAMEOBJECTDETECTOR_H
#define GAMEOBJECTDETECTOR_H

#include "gameobject.h"
#include <QObject>

class GameObjectDetector : public QObject, public GameObject
{
   Q_OBJECT
public:
    GameObjectDetector(GameObject *target = nullptr, qreal width = 10, qreal heigth = 10, QGraphicsItem *parent = nullptr);
public:
    void setTargetObject(GameObject *target);
    void advanceObject(int delta_time, int phase) override;
signals:
    void targetDetected();
private:
    GameObject *m_target_object;
};

#endif // GAMEOBJECTDETECTOR_H
