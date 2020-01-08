#include "objectfeature.h"
#include "gameobject.h"

ObjectFeature::ObjectFeature(QObject *parent) : QObject(parent)
{
    m_removed_attributes_mask = 0;
    m_attributes = 0;
    m_owner = nullptr;
    m_databank = nullptr;
}

void ObjectFeature::setOwner(GameObject *owner)
{
    m_owner = owner;
}

void ObjectFeature::setDataBank(DataBank &databank)
{
    m_databank = &databank;

    // connection made for updating internal changes of the feature to the object databank
    connect(this, &ObjectFeature::internalsChanged, this, &ObjectFeature::handleInternalsChange);
    emit internalsChanged();
}

bool ObjectFeature::checkAttribute(unsigned int attributes, int attribute_index)
{
    unsigned int temp_mask = 1<<attribute_index;
    return (temp_mask & attributes);
}

void ObjectFeature::setAttribute(int attribute_index, bool attribute_on)
{
    if(attribute_on){
        unsigned int mask = 1<<attribute_index;
        m_attributes = m_attributes | mask;
    }
    else{
        unsigned int all_ones = 0;
        all_ones = ~all_ones;
        unsigned int mask = 1<<attribute_index;
        mask = mask^all_ones;
        m_attributes = m_attributes & mask;
        m_removed_attributes_mask = mask;
    }

    emit internalsChanged();
}

void ObjectFeature::handleInternalsChange()
{
    if(m_owner != nullptr){
        m_databank->attributes = m_databank->attributes | m_attributes;
        m_databank->attributes = m_databank->attributes & m_removed_attributes_mask;
        m_removed_attributes_mask = 0;
        m_removed_attributes_mask = ~m_removed_attributes_mask;
    }
}

void ObjectFeature::update()
{

}
