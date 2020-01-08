#include "dronesounds.h"
#include "soundmanager.h"
#include "attributesandstates.h"

DroneSounds::DroneSounds(const QString &fileFolder, SoundManager &sound_manager, QObject *parent) : ObjectFeature(parent)
{
    m_sound_manager = &sound_manager;
    m_uuid = QUuid::createUuid();
    m_registered = m_sound_manager->registerSource(m_uuid, fileFolder);
    m_previous_command = IDLE;
    m_previous_state = IDLE;
    m_time_switch_done = false;
    m_elapsed_time = 0;
}

DroneSounds::~DroneSounds()
{
    m_sound_manager->unRegisterSource(m_uuid);
}

void DroneSounds::update()
{
    if(m_registered){
        if(m_previous_command != m_databank->move_action){
            if(!(m_previous_command == UP || m_previous_command == UP_LEFT || m_previous_command == UP_RIGHT) && (m_databank->move_action == UP || m_databank->move_action == UP_LEFT || m_databank->move_action == UP_RIGHT)){
                m_sound_manager->playEffect(m_uuid, "acceleration", 0.6);
            }
            else if((m_previous_command == UP || m_previous_command == UP_LEFT || m_previous_command == UP_RIGHT) && !(m_databank->move_action == UP || m_databank->move_action == UP_LEFT || m_databank->move_action == UP_RIGHT)){
                if(m_owner->state() == FREE_FALLING){
                    m_sound_manager->playEffect(m_uuid, "idle", 0.6, -1);
                }
            }
            m_previous_command = m_databank->move_action;
            m_time_switch_done = false;
            m_elapsed_time = 0;
        }
        else if(!m_time_switch_done && (m_databank->move_action == UP || m_databank->move_action == UP_LEFT || m_databank->move_action == UP_RIGHT)){
            if(m_elapsed_time > 1800){
                m_sound_manager->playEffect(m_uuid, "full_power", 0.6, -1);
                m_time_switch_done = true;
            }
            m_elapsed_time += m_databank->deltatime;
        }
        if(m_previous_state != m_owner->state()){
            if(m_owner->state() == ON_A_SURFACE){
                m_sound_manager->playEffect(m_uuid, "landing", 0.6);
            }
            m_previous_state = m_owner->state();
        }
    }
}
