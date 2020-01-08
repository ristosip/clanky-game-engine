#ifndef FORCEFIELD_H
#define FORCEFIELD_H

#include "gameobject.h"
#include "pixmapitem.h"


class ForceField : public GameObject
{
public:
    ForceField(qreal width, qreal height, QGraphicsItem *parent = nullptr);
    void setImage(const QString &image_path, qreal scale = 1.0, qreal rotation = 0.0);
    void setForce(qreal force_x, qreal force_y);
    QPointF force();

private:
    PixmapItem *m_image;
    qreal m_force_x = 0.0;
    qreal m_force_y = 0.0;
};

#endif // FORCEFIELD_H
