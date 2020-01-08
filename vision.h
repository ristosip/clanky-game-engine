#ifndef VISION_H
#define VISION_H

#include "objectfeature.h"
#include <QGraphicsRectItem>

class Vision : public ObjectFeature
{
    Q_OBJECT
public:
    Vision(QObject *parent = nullptr);
    void update() override;
protected:
    void setOwner(GameObject *owner) override;
private:
    QGraphicsRectItem *m_field_of_view;
};

#endif // VISION_H
