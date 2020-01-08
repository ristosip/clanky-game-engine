#include "forcefield.h"
#include "pixmapitem.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include "attributesandstates.h"

ForceField::ForceField(qreal width, qreal height, QGraphicsItem *parent) : GameObject(parent)
{
    setRect(0, 0, width, height);

    unsigned int mask = 1<<FORCING;
    m_databank.attributes = m_databank.attributes | mask;

    mask = 1<<MYSTERIOUS;
    m_databank.attributes = m_databank.attributes | mask;
}

void ForceField::setImage(const QString &image_path, qreal scale, qreal rotation)
{
    m_image = new PixmapItem(QPixmap(image_path));
    m_image->setFlag(QGraphicsItem::ItemIgnoresParentOpacity, true);
    m_image->setParentItem(this);
    m_image->setRotation(rotation);
    m_image->setScale(scale);
    m_image->setTransformOriginPoint(m_image->boundingRect().width()/2.0, m_image->boundingRect().height()/2.0);
    m_image->setPos(-m_image->transformOriginPoint().x() + this->boundingRect().width()/2.0, -m_image->transformOriginPoint().y() + this->boundingRect().height()/2.0);
    m_image->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);

    PixmapItem *image_layer = new PixmapItem(QPixmap(image_path));
    image_layer->setParentItem(m_image);
    image_layer->setFlag(QGraphicsItem::ItemIgnoresParentOpacity, true);
    image_layer->setRotation(rotation + 90.0);
    image_layer->setScale(.9);
    image_layer->setTransformOriginPoint(image_layer->boundingRect().width()/2.0, image_layer->boundingRect().height()/2.0);
    image_layer->setPos(-image_layer->transformOriginPoint().x() + m_image->boundingRect().width()/2.0, -image_layer->transformOriginPoint().y() + m_image->boundingRect().height()/2.0);

    // animation 1
    {
        QParallelAnimationGroup *group = new QParallelAnimationGroup(m_image);

        QPropertyAnimation *opac_anim = new QPropertyAnimation(m_image, "opacity");
        opac_anim->setStartValue(0.6);
        opac_anim->setEndValue(0.4);
        opac_anim->setEasingCurve(QEasingCurve::SineCurve);
        opac_anim->setDuration(700);

        QPropertyAnimation *rot_anim = new QPropertyAnimation(m_image, "rotation");
        rot_anim->setStartValue(5);
        rot_anim->setEndValue(-5);
        rot_anim->setEasingCurve(QEasingCurve::SineCurve);
        rot_anim->setDuration(2000);

        group->addAnimation(opac_anim);
        group->addAnimation(rot_anim);
        group->setLoopCount(-1);
        group->start();
    }

    // animation 2
    {
        QParallelAnimationGroup *group = new QParallelAnimationGroup(image_layer);

        QPropertyAnimation *opac_anim = new QPropertyAnimation(image_layer, "opacity");
        opac_anim->setStartValue(0.4);
        opac_anim->setEndValue(0.3);
        opac_anim->setEasingCurve(QEasingCurve::SineCurve);
        opac_anim->setDuration(900);

        QPropertyAnimation *rot_anim = new QPropertyAnimation(image_layer, "rotation");
        rot_anim->setStartValue(7);
        rot_anim->setEndValue(-7);
        rot_anim->setEasingCurve(QEasingCurve::SineCurve);
        rot_anim->setDuration(3000);

        group->addAnimation(opac_anim);
        group->addAnimation(rot_anim);
        group->setLoopCount(-1);
        group->start();
    }
}

void ForceField::setForce(qreal force_x, qreal force_y)
{
    m_force_x = force_x * GRAVITY;
    m_force_y = force_y * GRAVITY;
}

QPointF ForceField::force()
{
    return  QPointF(m_force_x, m_force_y);
}
