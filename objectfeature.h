#ifndef OBJECTFEATURE_H
#define OBJECTFEATURE_H

#include <QObject>
#include "gameobject.h"

struct DataBank;
class GameObject;

class ObjectFeature : public QObject
{
    Q_OBJECT
public:
    ObjectFeature(QObject *parent = nullptr);
    virtual void update();

signals:
    void internalsChanged();

protected:
    virtual void setOwner(GameObject *owner);
    virtual void setDataBank(DataBank &databank);
    bool checkAttribute(unsigned int attributes, int attribute_index);
    void setAttribute(int attribute_index, bool attribute_on = true);
    GameObject *m_owner;
    DataBank *m_databank;
    unsigned int m_attributes;
    unsigned int m_removed_attributes_mask;

protected slots:
    virtual void handleInternalsChange();

    friend class GameObject;
};

#endif // OBJECTFEATURE_H
