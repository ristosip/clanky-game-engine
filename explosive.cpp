#include "explosive.h"
#include "gameobjectfactory.h"
#include "controller.h"
#include "gamescene.h"
#include "bargraphics.h"
#include "soundmanager.h"

Explosive::Explosive(const QString &shrapnel_graphics_path, GameObjectFactory *factory, SoundManager *soundmanager, qreal image_scale, QObject *parent) : ObjectFeature(parent)
{
    m_shrapnel_graphics_path = shrapnel_graphics_path;
    m_factory = factory;
    m_image_scale = image_scale;
    m_triggered = false;
    m_soundmanager = soundmanager;
}

void Explosive::trigger()
{
    m_triggered = true;
}

void Explosive::update()
{
    if(m_triggered){
        if(m_factory != nullptr){
            GameScene *scene = static_cast<GameScene*>(m_owner->scene());
            GameObject *object1 = m_factory->create(SHRAPNEL);
            qreal epicentre_x = m_owner->pos().x() + m_owner->boundingRect().width()/2.0;
            qreal epicentre_y = m_owner->pos().y() + m_owner->boundingRect().height()/2.0;
            qreal shrp_width = object1->boundingRect().width();
            qreal shrp_height = object1->boundingRect().height();
            object1->setPos(epicentre_x - 2 * shrp_width - 4, epicentre_y - 2 * shrp_height - 4);
            Controller *c1 = new Controller();
            c1->setMovement(-7, -7);
            object1->setController(c1);
            object1->addFeature(new BarGraphics(m_shrapnel_graphics_path, m_image_scale));
            scene->addGameObject(object1);

            GameObject *object2 = m_factory->create(SHRAPNEL);
            object2->setPos(epicentre_x - 1 * shrp_width - 2, epicentre_y - 2 * shrp_height - 4);
            Controller *c2 = new Controller();
            c2->setMovement(-2, -10);
            object2->setController(c2);
            object2->addFeature(new BarGraphics(m_shrapnel_graphics_path, m_image_scale));
            scene->addGameObject(object2);

            GameObject *object3 = m_factory->create(SHRAPNEL);
            object3->setPos(epicentre_x - 2, epicentre_y - 2 * shrp_height - 4);
            Controller *c3 = new Controller();
            c3->setMovement(2, -10);
            object3->setController(c3);
            object3->addFeature(new BarGraphics(m_shrapnel_graphics_path, m_image_scale));
            scene->addGameObject(object3);

            GameObject *object4 = m_factory->create(SHRAPNEL);
            object4->setPos(epicentre_x + 1 * shrp_width + 2, epicentre_y - 2 * shrp_height - 4);
            Controller *c4 = new Controller();
            c4->setMovement(7, -7);
            object4->setController(c4);
            object4->addFeature(new BarGraphics(m_shrapnel_graphics_path, m_image_scale));
            scene->addGameObject(object4);

            GameObject *object5 = m_factory->create(SHRAPNEL);
            object5->setPos(epicentre_x - 2 * shrp_width - 4, epicentre_y - 1 * shrp_height - 2);
            Controller *c5 = new Controller();
            c5->setMovement(-10, 0);
            object5->setController(c5);
            object5->addFeature(new BarGraphics(m_shrapnel_graphics_path, m_image_scale));
            scene->addGameObject(object5);

            GameObject *object6 = m_factory->create(SHRAPNEL);
            object6->setPos(epicentre_x - 1 * shrp_width - 2, epicentre_y - 1 * shrp_height - 2);
            Controller *c6 = new Controller();
            c6->setMovement(-2, 10);
            object6->setController(c6);
            object6->addFeature(new BarGraphics(m_shrapnel_graphics_path, m_image_scale));
            scene->addGameObject(object6);

            GameObject *object7 = m_factory->create(SHRAPNEL);
            object7->setPos(epicentre_x - 2, epicentre_y - 1 * shrp_height - 2);
            Controller *c7 = new Controller();
            c7->setMovement(2, 10);
            object7->setController(c7);
            object7->addFeature(new BarGraphics(m_shrapnel_graphics_path, m_image_scale));
            scene->addGameObject(object7);

            GameObject *object8 = m_factory->create(SHRAPNEL);
            object8->setPos(epicentre_x + 1 * shrp_width + 2, epicentre_y - 1 * shrp_height - 2);
            Controller *c8 = new Controller();
            c8->setMovement(7, 7);
            object8->setController(c8);
            object8->addFeature(new BarGraphics(m_shrapnel_graphics_path, m_image_scale));
            scene->addGameObject(object8);

            if(m_soundmanager != nullptr)
                m_soundmanager->playEffect(QUuid(0x11111111, 0x1111, 0x1111, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11), "explosion", 0.6);

            scene->removeGameObject(m_owner);
        }
    }
}
