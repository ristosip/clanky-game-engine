#ifndef HUMANOIDGRAPHICS_H
#define HUMANOIDGRAPHICS_H

#include "objectfeature.h"
#include "pixmapitem.h"
#include <QSequentialAnimationGroup>

class HumanoidGraphics : public ObjectFeature
{
    Q_OBJECT
public:
    HumanoidGraphics(const QString &fileFolder = ":/", qreal scale = 1, qreal x = 0, qreal y = 0, QObject *parent = nullptr);
    virtual void update();
    qreal scale();
    void setScale(qreal scale);

protected:
    void setOwner(GameObject *owner) override;
    virtual void createAnimations();

private:
    PixmapItem *m_torso;
    QSequentialAnimationGroup *m_animationGroup;
    int m_state;
};

#endif // HUMANOIDGRAPHICS_H
