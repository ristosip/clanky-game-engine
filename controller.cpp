#include "controller.h"

Controller::Controller(QObject *parent) : ObjectFeature(parent)
{
    m_movement_vector_given = false;
    m_disabled = false;
}

void Controller::setCommands(int movement, int actions, int modifiers)
{
    if(movement != 4 && movement != 22 && movement != 26) // reject conflicting commands
        m_movement = movement;

    m_actions = actions;
    m_modifiers = modifiers;
}

void Controller::setMovement(qreal dx, qreal dy)
{
    m_dx = dx;
    m_dy = dy;
    m_movement_vector_given = true;
}

void Controller::disable()
{
    m_disabled = true;
}

void Controller::enable()
{
    m_disabled = false;
}

void Controller::update()
{
    if(!m_disabled){
        m_databank->move_action = m_movement;
        m_databank->other_action = m_actions;
        m_databank->action_modifier = m_modifiers;

        if(m_movement_vector_given){
            m_databank->dx = m_dx;
            m_databank->dy = m_dy;
            m_movement_vector_given = false;
        }
    }
}
