#ifndef WORMHOLE_H
#define WORMHOLE_H

#include "gameobject.h"
#include "gameobjectfactory.h"
#include "pixmapitem.h"
#include <QSequentialAnimationGroup>

class Wormhole : public QObject, public GameObject
{
    Q_OBJECT
public:
    Wormhole(const QString &image_path, qreal scale, GameObjectFactory *factory, bool this_is_exit = false, qreal width = 50, qreal height = 50, QGraphicsItem *graphics_parent = nullptr, QObject *parent = nullptr);
    void setExitPos(qreal x, qreal y);
    void advanceObject(int delta_time, int phase) override;
public slots:
    void closeWormhole();
private slots:
    void removeWormHole();
private:
    unsigned int m_attribute_mask;
    GameObjectFactory *m_factory;
    QSequentialAnimationGroup *m_animation;
    PixmapItem *m_image;
    qreal m_scale;
    qreal m_exit_x;
    qreal m_exit_y;
    bool m_this_is_exit;
};

#endif // WORMHOLE_H
