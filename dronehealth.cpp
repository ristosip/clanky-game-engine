#include "dronehealth.h"
#include "attributesandstates.h"

DroneHealth::DroneHealth(int initial_health, QObject* parent) : ObjectFeature(parent)
{
    m_health = initial_health;
    if(initial_health > 100)
        initial_health = 100;
    if(initial_health <= 0)
        initial_health = 1;

    m_previous_velocity_x = 0.0;
    m_previous_velocity_y = 0.0;
}

void DroneHealth::update()
{
    for(int i = 0; i < m_databank->colliding_objects.length(); i++){
        if(checkAttribute(m_databank->colliding_objects.at(i)->attributes(), COLLIDEABLE) && m_databank->state != ON_A_SURFACE){
            m_health -= 5;
            if(m_health <= 0)
                emit death();
        }
        else if (checkAttribute(m_databank->colliding_objects.at(i)->attributes(), PIERCING)){
            m_health -= 100;
            if(m_health <= 0)
                emit death();
        }
    }
    if(abs(m_databank->physics_packet.at(VELOCITY_X) - m_previous_velocity_x) > 0.5)
        m_health -= 25;

    if(abs(m_databank->physics_packet.at(VELOCITY_Y) - m_previous_velocity_y) > 0.5)
        m_health -= 25;

    if(m_health <= 0)
        emit death();

    m_previous_velocity_x = m_databank->physics_packet.at(VELOCITY_X);
    m_previous_velocity_y = m_databank->physics_packet.at(VELOCITY_Y);

    m_databank->health = m_health;
}
