#include "thrower.h"
#include "attributesandstates.h"
#include "controller.h"
#include "gamescene.h"

Thrower::Thrower(int throwable_object_type, int throwable_object_count, GameObjectFactory *factory, QObject *parent) : ObjectFeature(parent)
{
    m_throwable_object_type = throwable_object_type;
    m_throwable_object_count = throwable_object_count;
    m_factory = factory;
    m_last_known_move_command = LEFT;
    m_launched = false;
    m_loaded = false;
    m_power = 0.0;
}

void Thrower::setThrowableObjectType(int object_type)
{
    m_throwable_object_type = object_type;
}

void Thrower::update()
{
    for(int i = 0; i < m_databank->collected_symbols.length(); i++){
        if(m_databank->collected_symbols.at(i) == GRENADE_SYMBOL)
            m_throwable_object_count += 10;
    }

    if(m_throwable_object_count > 0){
        if(m_databank->other_action == THROW || m_databank->other_action == THROW_AUTO_RELEASE){
            m_launched = false;
            m_loaded = true;
            if(m_power < 1.0){
                m_power += (1.0 / 1000.0) * m_databank->deltatime;
            }
            else{
                if(m_databank->other_action == THROW_AUTO_RELEASE)
                    m_databank->other_action = IDLE;
            }
        }

        if(m_databank->other_action != THROW && m_databank->other_action != THROW_AUTO_RELEASE && !m_launched && m_loaded){
            m_launched = true;
            m_loaded = false;
            m_throwable_object_count--;
            if(m_factory != nullptr){
                GameObject* object = m_factory->create(m_throwable_object_type);

                Controller *controller = new Controller();
                object->setController(controller);

                qreal dx = 100 * m_power / (object->physicsPacket()->at(MASS) * MASS_CONSTANT);
                qreal dy = 100 * m_power / (object->physicsPacket()->at(MASS) * MASS_CONSTANT);

                if(m_last_known_move_command == LEFT){
                    object->setPos(m_owner->pos().x() - object->boundingRect().width() - 2, m_owner->pos().y());
                    controller->setMovement(-dx, -dy/2.0);
                }
                else if(m_last_known_move_command == RIGHT){
                    object->setPos(m_owner->pos().x() + m_owner->boundingRect().width() + 2, m_owner->pos().y());
                    controller->setMovement(dx, -dy/2.0);
                }
                else if(m_last_known_move_command == UP){
                    object->setPos(m_owner->pos().x() - object->boundingRect().width() - 2, m_owner->pos().y());
                    controller->setMovement(0, -dy);
                }
                else if(m_last_known_move_command == DOWN){
                    object->setPos(m_owner->pos().x() - object->boundingRect().width() - 2, m_owner->pos().y());
                    controller->setMovement(0, dy);
                }
                else if(m_last_known_move_command == UP_LEFT){
                    object->setPos(m_owner->pos().x() - object->boundingRect().width() - 2, m_owner->pos().y());
                    controller->setMovement(-0.7 * dx, - 1.2 * dy);
                }
                else if(m_last_known_move_command == UP_RIGHT){
                    object->setPos(m_owner->pos().x() + m_owner->boundingRect().width() + 2, m_owner->pos().y());
                    controller->setMovement(0.7 * dx, -1.2 * dy);
                }
                else if(m_last_known_move_command == DOWN_LEFT){
                    object->setPos(m_owner->pos().x() - object->boundingRect().width() - 2, m_owner->pos().y());
                    controller->setMovement(-0.7 * dx, 0.7 * dy);
                }
                else if(m_last_known_move_command == DOWN_RIGHT){
                    object->setPos(m_owner->pos().x() + m_owner->boundingRect().width() + 2, m_owner->pos().y());
                    controller->setMovement(0.7 * dx, 0.7 * dy);
                }

                static_cast<GameScene*>(m_owner->scene())->addGameObject(object);

                m_power = 0.0;
            }
        }
    }
    if(m_databank->move_action != 0){
        m_last_known_move_command = m_databank->move_action;
    }
}
