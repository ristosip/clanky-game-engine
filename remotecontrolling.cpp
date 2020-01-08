#include "remotecontrolling.h"
#include "attributesandstates.h"
#include "controller.h"
#include "gamescene.h"
#include "controller.h"
#include "drone.h"

RemoteControlling::RemoteControlling(GameObjectFactory *factory, QObject *parent) : ObjectFeature(parent)
{
    m_factory = factory;
    m_piloting = false;
    m_drone_set_up = false;
    m_device_type = 0;
}

void RemoteControlling::stopControlling()
{
    m_piloting = false;
    m_drone_set_up = false;
    m_device_type = 0;
}

void RemoteControlling::update()
{
    for(int i = 0; i < m_databank->collected_symbols.length(); i++){
        if(m_databank->collected_symbols.at(i) == DRONE_SYMBOL || m_databank->collected_symbols.at(i) == SURROGATE_SYMBOL){
            m_piloting = true;
            m_device_type = m_databank->collected_symbols.at(i);
        }
    }

    if(m_piloting){
        if(!m_drone_set_up){

            GameObject *device;

            if(m_device_type == DRONE_SYMBOL){
                device = m_factory->create(DRONE);
                connect(static_cast<Drone*>(device), &Drone::crashed, this, &RemoteControlling::stopControlling);

                Controller *controller = new Controller();
                device->setController(controller);
                connect(this, &RemoteControlling::sendCommand, controller, &Controller::setCommands);
                connect(static_cast<Drone*>(device), &Drone::batteryDied, controller, &Controller::disable);
            }
            else if(m_device_type == SURROGATE_SYMBOL){
                device = m_factory->create(SURROGATE);
                connect(static_cast<Drone*>(device), &Drone::crashed, this, &RemoteControlling::stopControlling);

                Controller *controller = new Controller();
                device->setController(controller);
                connect(this, &RemoteControlling::sendCommand, controller, &Controller::setCommands);
                connect(static_cast<Drone*>(device), &Drone::batteryDied, controller, &Controller::disable);
            }
            else{
                device = nullptr;
            }
            if(device != nullptr){
                if(m_databank->move_action == LEFT || m_databank->move_action == IDLE || m_databank->move_action == UP_LEFT || m_databank->move_action == DOWN_LEFT || m_databank->move_action == UP || m_databank->move_action == DOWN)
                    device->setPos(m_owner->pos().x() - 100 - device->boundingRect().width(), m_owner->pos().y() + m_owner->boundingRect().height() - 5 - device->boundingRect().height());
                if(m_databank->move_action == RIGHT || m_databank->move_action == UP_RIGHT || m_databank->move_action == DOWN_RIGHT)
                    device->setPos(m_owner->pos().x() + 100 + m_owner->boundingRect().width() + device->boundingRect().width(), m_owner->pos().y() + m_owner->boundingRect().height() - 5 - device->boundingRect().height());

                static_cast<GameScene*>(m_owner->scene())->addGameObject(device, true);
                m_drone_set_up = true;
            }
        }
        else{
            //intercepts the commands and relays them to the drone
            sendCommand(m_databank->move_action, m_databank->other_action, m_databank->action_modifier);
            m_databank->move_action = 0;
            m_databank->other_action = 0;
            m_databank->action_modifier = 0;
        }
    }
}
