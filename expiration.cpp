#include "expiration.h"
#include "gamescene.h"

Expiration::Expiration(int time_ms, bool start, bool loop, bool remove_object_when_expired, QObject *parent) : ObjectFeature(parent)
{
    m_time_ms = time_ms;
    m_time_passed = 0;
    m_done = false;
    m_running = start;
    m_loop = loop;
    m_remove_object_when_expired = remove_object_when_expired;
}

void Expiration::start()
{
    m_time_passed = 0;
    m_running = true;
    m_done = false;
}

void Expiration::update()
{
    if(m_running){
        m_time_passed += m_databank->deltatime;

        if(m_time_passed >= m_time_ms && !m_done){
            emit expired();
            m_done = true;
            if(m_remove_object_when_expired){
                static_cast<GameScene*>(m_owner->scene())->removeGameObject(m_owner);
            }

            if(m_loop)
                start();
        }
    }
}
