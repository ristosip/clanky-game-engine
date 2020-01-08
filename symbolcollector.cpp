#include "symbolcollector.h"
#include "attributesandstates.h"
#include "symbol.h"
#include "gamescene.h"

void SymbolCollector::update()
{
    for(int i = 0; i < m_databank->colliding_objects.length(); i++){
        if(checkAttribute(m_databank->colliding_objects.at(i)->attributes(), COLLECTIBLE)){
            if(dynamic_cast<Symbol*>(m_databank->colliding_objects.at(i)) != nullptr){
                m_databank->collected_symbols.append(dynamic_cast<Symbol*>(m_databank->colliding_objects.at(i))->symbolCode());
                static_cast<GameScene*>(m_owner->scene())->removeGameObject(m_databank->colliding_objects.at(i));
            }
        }
    }
}
