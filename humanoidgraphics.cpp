#include "humanoidgraphics.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QAnimationGroup>
#include "pixmapitem.h"

HumanoidGraphics::HumanoidGraphics(const QString &fileFolder, qreal scale, qreal x, qreal y, QObject *parent) : ObjectFeature(parent)
{   
    m_state = 2; // initialized

    m_torso = new PixmapItem(QPixmap(fileFolder + "torso.png"));
    m_torso->setFlag(QGraphicsItem::ItemIgnoresParentOpacity, true);
    m_torso->setPos(x, y);
    m_torso->setTransformOriginPoint(m_torso->boundingRect().width()/2, m_torso->boundingRect().height()/2);

    // head and neck
    PixmapItem *neck = new PixmapItem(QPixmap(fileFolder + "neck.png"), m_torso);
    neck->setPos(m_torso->boundingRect().width()/2 - neck->boundingRect().width()/2, -neck->boundingRect().height() + 0.2 * neck->boundingRect().height());
    neck->setTransformOriginPoint(neck->boundingRect().width()/2, neck->boundingRect().height());

    PixmapItem *head = new PixmapItem(QPixmap(fileFolder + "head.png"), neck);
    head->setPos(neck->boundingRect().width()/2 - head->boundingRect().width()/2, -head->boundingRect().height() + 0.2 * neck->boundingRect().height());
    head->setTransformOriginPoint(head->boundingRect().width()/2, head->boundingRect().height()/2);

    // arms
    PixmapItem *leftUpperArm = new PixmapItem(QPixmap(fileFolder + "leftUpperArm.png"), m_torso);
    leftUpperArm->setPos(-leftUpperArm->boundingRect().width() + 0.2 * leftUpperArm->boundingRect().width(), 0 + 0.2 * m_torso->boundingRect().height());
    leftUpperArm->setTransformOriginPoint(leftUpperArm->boundingRect().width(), 0);

    PixmapItem *leftLowerArm = new PixmapItem(QPixmap(fileFolder + "leftLowerArm.png"), leftUpperArm);
    leftLowerArm->setPos(leftUpperArm->boundingRect().width()/2 - leftLowerArm->boundingRect().width()/2, leftUpperArm->boundingRect().height()  - 0.2 * leftLowerArm->boundingRect().height());
    leftLowerArm->setTransformOriginPoint(leftLowerArm->boundingRect().width()/2, 0);

    PixmapItem *rightUpperArm = new PixmapItem(QPixmap(fileFolder + "rightUpperArm.png"), m_torso);
    rightUpperArm->setPos(m_torso->boundingRect().width() - 0.2 * rightUpperArm->boundingRect().width(), 0 + 0.2 * m_torso->boundingRect().height());

    PixmapItem *rightLowerArm = new PixmapItem(QPixmap(fileFolder + "rightLowerArm.png"), rightUpperArm);
    rightLowerArm->setPos(rightUpperArm->boundingRect().width()/2 - rightLowerArm->boundingRect().width()/2, rightUpperArm->boundingRect().height()  - 0.2 * rightLowerArm->boundingRect().height());
    rightLowerArm->setTransformOriginPoint(rightLowerArm->boundingRect().width()/2, 0);

    // legs
    PixmapItem *leftUpperLeg = new PixmapItem(QPixmap(fileFolder + "leftUpperLeg.png"), m_torso);
    leftUpperLeg->setPos(0.2 * m_torso->boundingRect().width(), m_torso->boundingRect().height() - 0.2 * leftUpperLeg->boundingRect().height());
    leftUpperLeg->setTransformOriginPoint(leftUpperLeg->boundingRect().width()/2, 0);

    PixmapItem *leftLowerLeg = new PixmapItem(QPixmap(fileFolder + "leftLowerLeg.png"), leftUpperLeg);
    leftLowerLeg->setPos(leftUpperLeg->boundingRect().width()/2 - leftLowerLeg->boundingRect().width()/2, leftUpperLeg->boundingRect().height() - 0.2 * leftLowerLeg->boundingRect().height());

    PixmapItem *rightUpperLeg = new PixmapItem(QPixmap(fileFolder + "rightUpperLeg.png"), m_torso);
    rightUpperLeg->setPos(m_torso->boundingRect().width() - rightUpperLeg->boundingRect().width() - 0.2 * m_torso->boundingRect().width(), m_torso->boundingRect().height() - 0.2 * rightUpperLeg->boundingRect().height());
    leftUpperLeg->setTransformOriginPoint(rightUpperLeg->boundingRect().width()/2, 0);

    PixmapItem *rightLowerLeg = new PixmapItem(QPixmap(fileFolder + "rightLowerLeg.png"), rightUpperLeg);
    rightLowerLeg->setPos(rightUpperLeg->boundingRect().width()/2 - rightLowerLeg->boundingRect().width()/2, rightUpperLeg->boundingRect().height() - 0.2 * rightLowerLeg->boundingRect().height());

    setScale(scale);

    createAnimations();
}

void HumanoidGraphics::update()
{
    int prev_state = m_state;

    // changes animation state based on the states of the game object
    if(m_databank->action_modifier == 1 && prev_state != 2)
        m_state = 2;
    else if(m_databank->action_modifier == 0 && prev_state != 1)
        m_state = 1;

    if(prev_state != m_state){
        switch (m_state) {
            case 0: // no state change
                break;
            case -1: // stops animations
                    m_animationGroup->currentAnimation()->stop();
                    break;
            default: // other than 0 or -1 changes the animation
                     m_animationGroup->currentAnimation()->stop();
                     if(m_state > 0 && m_state <= m_animationGroup->animationCount() && m_animationGroup->animationCount() > 0)
                        m_animationGroup->animationAt(m_state -1)->start();
                    break;
        }
    }
}

void HumanoidGraphics::setOwner(GameObject *owner)
{
    m_owner = owner;
    m_torso->setParentItem(owner);
    // reposition the drawing to the parent
    m_torso->setPos(-m_torso->transformOriginPoint().x() + m_owner->pos().x() + m_owner->boundingRect().width()/2, -m_torso->transformOriginPoint().y() + m_owner->pos().y() + 1.1 * m_owner->boundingRect().height()/2);
}

qreal HumanoidGraphics::scale()
{
    return m_torso->scale();
}

void HumanoidGraphics::setScale(qreal scale)
{
    m_torso->setScale(scale);
}

void HumanoidGraphics::createAnimations()
{
    m_animationGroup = new QSequentialAnimationGroup(this);

    ///////////// WALK ///////////////
    {
        QParallelAnimationGroup *walk = new QParallelAnimationGroup(m_torso);

        // torso

        QPropertyAnimation *torsoSwing = new QPropertyAnimation(m_torso, "rotation");
        torsoSwing->setStartValue(15);
        torsoSwing->setEndValue(-15);
        walk->addAnimation(torsoSwing);

        // walk: headswing

        QPropertyAnimation *neckSwing = new QPropertyAnimation(static_cast<PixmapItem *>(m_torso->childItems().at(0)), "rotation");
        neckSwing->setStartValue(15);
        neckSwing->setEndValue(-15);
        walk->addAnimation(neckSwing);

        QPropertyAnimation *headSwing = new QPropertyAnimation(static_cast<PixmapItem *>(m_torso->childItems().at(0)->childItems().at(0)), "rotation");
        headSwing->setStartValue(15);
        headSwing->setEndValue(-15);
        walk->addAnimation(headSwing);

        // walk: left arm

        QPropertyAnimation *lArmSwing = new QPropertyAnimation(static_cast<PixmapItem *>(m_torso->childItems().at(1)), "rotation");
        lArmSwing->setStartValue(15);
        lArmSwing->setEndValue(-15);
        walk->addAnimation(lArmSwing);

        QPropertyAnimation *lLowerArmSwing = new QPropertyAnimation(static_cast<PixmapItem *>(m_torso->childItems().at(1)->childItems().at(0)), "rotation");
        lLowerArmSwing->setStartValue(15);
        lLowerArmSwing->setEndValue(-15);
        walk->addAnimation(lLowerArmSwing);

        // walk: right arm

        QPropertyAnimation *rArmSwing = new QPropertyAnimation(static_cast<PixmapItem *>(m_torso->childItems().at(2)), "rotation");
        rArmSwing->setStartValue(15);
        rArmSwing->setEndValue(-15);
        walk->addAnimation(rArmSwing);

        QPropertyAnimation *rLowerArmSwing = new QPropertyAnimation(static_cast<PixmapItem *>(m_torso->childItems().at(2)->childItems().at(0)), "rotation");
        rLowerArmSwing->setStartValue(15);
        rLowerArmSwing->setEndValue(-15);
        walk->addAnimation(rLowerArmSwing);

        // walk: left leg

        QPropertyAnimation *lLegSwing = new QPropertyAnimation(static_cast<PixmapItem *>(m_torso->childItems().at(3)), "rotation");
        lLegSwing->setStartValue(50);
        lLegSwing->setEndValue(0);
        walk->addAnimation(lLegSwing);

        QPropertyAnimation *lLowerLegSwing = new QPropertyAnimation(static_cast<PixmapItem *>(m_torso->childItems().at(3)->childItems().at(0)), "rotation");
        lLowerLegSwing->setStartValue(30);
        lLowerLegSwing->setEndValue(-30);
        walk->addAnimation(lLowerLegSwing);

        // walk: left leg

        QPropertyAnimation *rLegSwing = new QPropertyAnimation(static_cast<PixmapItem *>(m_torso->childItems().at(4)), "rotation");
        rLegSwing->setStartValue(-50);
        rLegSwing->setEndValue(0);
        walk->addAnimation(rLegSwing);

        QPropertyAnimation *rLowerLegSwing = new QPropertyAnimation(static_cast<PixmapItem *>(m_torso->childItems().at(4)->childItems().at(0)), "rotation");
        rLowerLegSwing->setStartValue(30);
        rLowerLegSwing->setEndValue(-30);
        walk->addAnimation(rLowerLegSwing);

        // walk: swing duration and looping

        for (int i = 0; i < walk->animationCount(); ++i) {
            QPropertyAnimation *animation = qobject_cast<QPropertyAnimation *>(walk->animationAt(i));
            animation->setEasingCurve(QEasingCurve::SineCurve);
            animation->setDuration(1500);
        }

        walk->setLoopCount(-1);

        // walk: add to master group
        m_animationGroup->addAnimation(walk);
    }

    ///////////// RUN ///////////////
    {
        QParallelAnimationGroup *run = new QParallelAnimationGroup(m_torso);

        // run: torsoswing

        QPropertyAnimation *torsoSwing = new QPropertyAnimation(m_torso, "rotation");
        torsoSwing->setStartValue(15);
        torsoSwing->setEndValue(-15);
        run->addAnimation(torsoSwing);

        // run: headswing

        QPropertyAnimation *neckSwing = new QPropertyAnimation(static_cast<PixmapItem *>(m_torso->childItems().at(0)), "rotation");
        neckSwing->setStartValue(15);
        neckSwing->setEndValue(-15);
        run->addAnimation(neckSwing);

        QPropertyAnimation *headSwing = new QPropertyAnimation(static_cast<PixmapItem *>(m_torso->childItems().at(0)->childItems().at(0)), "rotation");
        headSwing->setStartValue(15);
        headSwing->setEndValue(-15);
        run->addAnimation(headSwing);

        // run: left arm

        QPropertyAnimation *lArmSwing = new QPropertyAnimation(static_cast<PixmapItem *>(m_torso->childItems().at(1)), "rotation");
        lArmSwing->setStartValue(15);
        lArmSwing->setEndValue(-15);
        run->addAnimation(lArmSwing);

        QPropertyAnimation *lLowerArmSwing = new QPropertyAnimation(static_cast<PixmapItem *>(m_torso->childItems().at(1)->childItems().at(0)), "rotation");
        lLowerArmSwing->setStartValue(15);
        lLowerArmSwing->setEndValue(-15);
        run->addAnimation(lLowerArmSwing);

        // run: right arm

        QPropertyAnimation *rArmSwing = new QPropertyAnimation(static_cast<PixmapItem *>(m_torso->childItems().at(2)), "rotation");
        rArmSwing->setStartValue(15);
        rArmSwing->setEndValue(-15);
        run->addAnimation(rArmSwing);

        QPropertyAnimation *rLowerArmSwing = new QPropertyAnimation(static_cast<PixmapItem *>(m_torso->childItems().at(2)->childItems().at(0)), "rotation");
        rLowerArmSwing->setStartValue(15);
        rLowerArmSwing->setEndValue(-15);
        run->addAnimation(rLowerArmSwing);

        // run: left leg

        QPropertyAnimation *lLegSwing = new QPropertyAnimation(static_cast<PixmapItem *>(m_torso->childItems().at(3)), "rotation");
        lLegSwing->setStartValue(50);
        lLegSwing->setEndValue(0);
        run->addAnimation(lLegSwing);

        QPropertyAnimation *lLowerLegSwing = new QPropertyAnimation(static_cast<PixmapItem *>(m_torso->childItems().at(3)->childItems().at(0)), "rotation");
        lLowerLegSwing->setStartValue(30);
        lLowerLegSwing->setEndValue(-30);
        run->addAnimation(lLowerLegSwing);

        // run: left leg

        QPropertyAnimation *rLegSwing = new QPropertyAnimation(static_cast<PixmapItem *>(m_torso->childItems().at(4)), "rotation");
        rLegSwing->setStartValue(-50);
        rLegSwing->setEndValue(0);
        run->addAnimation(rLegSwing);

        QPropertyAnimation *rLowerLegSwing = new QPropertyAnimation(static_cast<PixmapItem *>(m_torso->childItems().at(4)->childItems().at(0)), "rotation");
        rLowerLegSwing->setStartValue(30);
        rLowerLegSwing->setEndValue(-30);
        run->addAnimation(rLowerLegSwing);

        // run: swing duration and looping

        for (int i = 0; i < run->animationCount(); ++i) {
            QPropertyAnimation *animation = qobject_cast<QPropertyAnimation *>(run->animationAt(i));
            animation->setEasingCurve(QEasingCurve::SineCurve);
            animation->setDuration(500);
        }

        run->setLoopCount(-1);

        // run: add to master group
        m_animationGroup->addAnimation(run);
    }
}
