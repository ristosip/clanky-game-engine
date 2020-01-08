#ifndef SURROGATE_H
#define SURROGATE_H

#include "gameobject.h"
#include "soundmanager.h"

class Surrogate : public QObject, public GameObject
{
    Q_OBJECT
public:
    Surrogate(qreal boundingrect_x, qreal boundingrect_y, qreal boundingrect_width, qreal boundingrect_height, SoundManager *soundmanager = nullptr, QGraphicsItem  *parent = nullptr);

signals:
    void crashed();
    void batteryDied();

private slots:
    void playBatteryDiesSound();
    void playCrashSound();

private:
    SoundManager *m_soundmanager;
};

#endif // SURROGATE_H
