#ifndef DRONE_H
#define DRONE_H

#include "gameobject.h"
#include "soundmanager.h"

class Drone : public QObject, public GameObject
{
    Q_OBJECT
public:
    Drone(const QString &image_path, qreal width, qreal height, qreal image_scale = 1.0, SoundManager *soundmanager = nullptr, QGraphicsItem *graphics_parent = nullptr);
signals:
    void crashed();
    void batteryDied();

private slots:
    void playBatteryDiesSound();
    void playCrashSound();

private:
    SoundManager *m_soundmanager;
};

#endif // DRONE_H
