#include "wormhole.h"
#include "attributesandstates.h"
#include "pixmapitem.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include "gameobjectfactory.h"
#include "gamescene.h"

Wormhole::Wormhole(const QString &image_path, qreal scale, GameObjectFactory *factory, bool this_is_exit, qreal width, qreal height, QGraphicsItem *graphics_parent, QObject *parent) : QObject(parent), GameObject(graphics_parent)
{
    m_factory = factory;
    m_this_is_exit = this_is_exit;
    m_scale = scale;

    setRect(0, 0, width, height);

    m_image = new PixmapItem(image_path);
    m_image->setScale(scale);
    m_image->setFlag(QGraphicsItem::ItemIgnoresParentOpacity, true);
    m_image->setParentItem(this);
    m_image->setTransformOriginPoint(m_image->boundingRect().width()/2.0, m_image->boundingRect().height()/2.0);
    m_image->setPos(-m_image->transformOriginPoint().x() + this->boundingRect().width()/2, -m_image->transformOriginPoint().y() + this->boundingRect().height()/2);

    m_animation = new QSequentialAnimationGroup(m_image);

    QParallelAnimationGroup *vortex_opening = new QParallelAnimationGroup();
    if(!m_this_is_exit){
        QPropertyAnimation *wobble = new QPropertyAnimation(m_image, "scale");
        wobble->setStartValue(0.001 * scale);
        wobble->setEndValue(1.0 * scale);
        wobble->setDuration(1000);

        QPropertyAnimation *rotation = new QPropertyAnimation(m_image, "rotation");
        rotation->setStartValue(0);
        rotation->setEndValue(360);
        rotation->setDuration(1000);

        QPropertyAnimation *opacity = new QPropertyAnimation(m_image, "opacity");
        opacity->setStartValue(0.01);
        opacity->setEndValue(1.0);
        opacity->setDuration(1000);

        vortex_opening->addAnimation(wobble);
        vortex_opening->addAnimation(rotation);
        vortex_opening->addAnimation(opacity);
    }
    QParallelAnimationGroup *vortex_open = new QParallelAnimationGroup();
    {
        QPropertyAnimation *wobble = new QPropertyAnimation(m_image, "scale");
        wobble->setStartValue(1.0 * scale);
        wobble->setEndValue(0.9 * scale);
        wobble->setLoopCount(-1);
        wobble->setEasingCurve(QEasingCurve::SineCurve);
        wobble->setDuration(750);

        QPropertyAnimation *rotation = new QPropertyAnimation(m_image, "rotation");
        rotation->setStartValue(0);
        rotation->setEndValue(360);
        rotation->setLoopCount(-1);
        rotation->setDuration(1500);

        QPropertyAnimation *opacity = new QPropertyAnimation(m_image, "opacity");
        opacity->setStartValue(1.0);
        opacity->setEndValue(0.7);
        opacity->setLoopCount(-1);
        opacity->setEasingCurve(QEasingCurve::SineCurve);
        opacity->setDuration(1000);

        vortex_open->addAnimation(wobble);
        vortex_open->addAnimation(rotation);
        vortex_open->addAnimation(opacity);
    }
    if(!m_this_is_exit)
        m_animation->addAnimation(vortex_opening);
    m_animation->addAnimation(vortex_open);
    m_animation->start();

    m_attribute_mask = 1<<MYSTERIOUS;
    m_databank.attributes = 0;
    m_databank.attributes = m_databank.attributes | m_attribute_mask;
}

void Wormhole::setExitPos(qreal x, qreal y)
{
    m_exit_x = x;
    m_exit_y = y;
}

void Wormhole::closeWormhole()
{
    m_animation->stop();

    QParallelAnimationGroup *vortex_closing = new QParallelAnimationGroup(m_image);

    QPropertyAnimation *wobble = new QPropertyAnimation(m_image, "scale");
    wobble->setStartValue(1.0 * m_scale);
    wobble->setEndValue(0.001 * m_scale);
    wobble->setDuration(1000);

    QPropertyAnimation *rotation = new QPropertyAnimation(m_image, "rotation");
    rotation->setStartValue(0);
    rotation->setEndValue(360);
    rotation->setDuration(1000);

    QPropertyAnimation *opacity = new QPropertyAnimation(m_image, "opacity");
    opacity->setStartValue(1.0);
    opacity->setEndValue(0.01);
    opacity->setDuration(1000);

    vortex_closing->addAnimation(wobble);
    vortex_closing->addAnimation(rotation);
    vortex_closing->addAnimation(opacity);

    vortex_closing->start();
    connect(vortex_closing, &QParallelAnimationGroup::finished, this, &Wormhole::removeWormHole);
}

void Wormhole::removeWormHole()
{
    static_cast<GameScene*>(scene())->removeGameObject(this);
}

void Wormhole::advanceObject(int delta_time, int phase)
{
    if(phase == 1){
        m_databank.attributes = m_databank.attributes | m_attribute_mask;

        if(!m_this_is_exit){
            QList<QGraphicsItem*> colliding_raw_items =  collidingItems();

            // the loop filters out all items other than the bounding rectangles
            for(int i = 0; i < colliding_raw_items.length(); i++)
            {
                if(dynamic_cast<GameObject*>(colliding_raw_items[i]) != nullptr){
                    m_databank.colliding_objects.append(dynamic_cast<GameObject*>(colliding_raw_items[i]));
                }
            }
            if(!m_databank.colliding_objects.isEmpty()){
                for(int i = 0; i < m_databank.colliding_objects.length(); i++)
                {
                    GameObject *exit = m_factory->create(WORMHOLE_EXIT);
                    exit->setPos(m_exit_x, m_exit_y);
                    static_cast<GameScene*>(scene())->addGameObject(exit);

                    m_databank.colliding_objects.at(i)->setPos(m_exit_x, m_exit_y);

                    static_cast<GameScene*>(scene())->removeGameObject(this);
                }
            }
        }
    }
    GameObject::advanceObject(delta_time, phase);
}
