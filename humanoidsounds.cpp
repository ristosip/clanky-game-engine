#include "humanoidsounds.h"
#include "soundmanager.h"
#include "attributesandstates.h"

HumanoidSounds::HumanoidSounds(const QString &fileFolder, SoundManager &sound_manager, QObject *parent) : ObjectFeature(parent)
{
    m_sound_manager = &sound_manager;
    m_uuid = QUuid::createUuid();
    m_registered = m_sound_manager->registerSource(m_uuid, fileFolder);
    m_previous_state = IDLE;
}

HumanoidSounds::~HumanoidSounds()
{
    m_sound_manager->unRegisterSource(m_uuid);
}

void HumanoidSounds::update()
{
    if(m_registered){
        if(m_previous_state != m_owner->state()){
            m_previous_state = m_owner->state();
            if(m_owner->state() == FREE_FALLING){
                m_sound_manager->playEffect(m_uuid, "jump", 0.1);
            }           
        }
        for(int i = 0; i < m_databank->colliding_objects.length(); i++){
            if(checkAttribute(m_databank->colliding_objects.at(i)->attributes(), MYSTERIOUS)){
                m_sound_manager->playEffect(m_uuid, "mysterious", 0.9);
            }
            else if(checkAttribute(m_databank->colliding_objects.at(i)->attributes(), COLLECTIBLE)){
                m_sound_manager->playEffect(m_uuid, "mysterious", 0.9);
            }
        }
    }
}
