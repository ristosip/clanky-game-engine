#include "surrogate.h"
#include "soundmanager.h"

Surrogate::Surrogate(qreal boundingrect_x, qreal boundingrect_y, qreal boundingrect_width, qreal boundingrect_height, SoundManager *soundmanager,  QGraphicsItem  *parent) : QObject(), GameObject(boundingrect_x, boundingrect_y, boundingrect_width, boundingrect_height, parent)
{
    m_soundmanager = soundmanager;

    connect(this, &Surrogate::batteryDied, this, &Surrogate::playBatteryDiesSound);
    connect(this, &Surrogate::crashed, this, &Surrogate::playCrashSound);
}

void Surrogate::playCrashSound(){
    if(m_soundmanager != nullptr){
        m_soundmanager->playEffect(QUuid(0x11111111, 0x1111, 0x1111, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11), "device_crash", 0.15);
    }
}

void Surrogate::playBatteryDiesSound(){
    if(m_soundmanager != nullptr){
        m_soundmanager->playEffect(QUuid(0x11111111, 0x1111, 0x1111, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11), "battery_dies", 0.15);
    }
}
