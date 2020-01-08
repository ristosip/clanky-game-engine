#include "enemyai.h"
#include "attributesandstates.h"

EnemyAI::EnemyAI(QObject *parent) : ObjectFeature(parent)
{

}

void EnemyAI::update()
{
    for(int i = 0; i < m_databank->detected_objects.length(); i++){
        if(checkAttribute(m_databank->detected_objects.at(i)->attributes(), PLAYER_CONTROLLED)){

            qreal dx = (m_owner->pos().x() + m_owner->boundingRect().width()/2.0) - (m_databank->detected_objects.at(i)->pos().x() + m_databank->detected_objects.at(i)->boundingRect().width()/2.0);

            if(dx < 0){
                m_databank->move_action = RIGHT;
                m_databank->other_action = THROW_AUTO_RELEASE;
            }
            if(dx > 0){
                m_databank->move_action = LEFT;
                m_databank->other_action = THROW_AUTO_RELEASE;
            }
            break;
        }
    }
}
