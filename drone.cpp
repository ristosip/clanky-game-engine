#include "drone.h"
#include "pixmapitem.h"
#include "soundmanager.h"

Drone::Drone(const QString &image_path, qreal width, qreal height, qreal image_scale, SoundManager *soundmanager, QGraphicsItem *graphics_parent) : QObject(), GameObject(graphics_parent)
{
    m_soundmanager = soundmanager;

    connect(this, &Drone::batteryDied, this, &Drone::playBatteryDiesSound);
    connect(this, &Drone::crashed, this, &Drone::playCrashSound);

    setRect(0, 0, width, height);

    PixmapItem *image = new PixmapItem(image_path);
    image->setFlag(QGraphicsItem::ItemIgnoresParentOpacity, true);
    image->setParentItem(this);

    image->setTransformOriginPoint(image->boundingRect().width()/2.0, image->boundingRect().height()/2.0);
    image->setPos(-image->transformOriginPoint().x() + this->boundingRect().width()/2, -image->transformOriginPoint().y() + this->boundingRect().height()/2);

    image->setScale(image_scale);
}

void Drone::playCrashSound(){
    if(m_soundmanager != nullptr){
        m_soundmanager->playEffect(QUuid(0x11111111, 0x1111, 0x1111, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11), "device_crash", 0.15);
    }
}

void Drone::playBatteryDiesSound(){
    if(m_soundmanager != nullptr){
        m_soundmanager->playEffect(QUuid(0x11111111, 0x1111, 0x1111, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11), "battery_dies", 0.15);
    }
}
