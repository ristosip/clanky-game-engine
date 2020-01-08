#include "demogame.h"
#include "gameobjectfactory.h"
#include "gameobject.h"
#include "player.h"
#include "controller.h"
#include "gameobjectdetector.h"
#include "wormhole.h"
#include "forcefield.h"
#include "gameobjecteater.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QUuid>

void DemoGame::createGame()
{
    // expand the scene

    GameObject *highx_lowy = m_factory->create(STEELBARHORIZONTAL);
    highx_lowy->setPos(10000, -10000);
    addGameObject(highx_lowy);

    GameObject *lowx_highy = m_factory->create(STEELBARHORIZONTAL);
    lowx_highy->setPos(-10000, 10000);
    addGameObject(lowx_highy);

    // background

    QGraphicsPixmapItem *background_0 = new QGraphicsPixmapItem(QPixmap(":/images/background/background_0.png"));
    QGraphicsPixmapItem *background_1 = new QGraphicsPixmapItem(QPixmap(":/images/background/background_1.png"));

    background_0->setScale(7);
    background_1->setScale(7);

    addBackgroundImageLayer(background_0, 0.95, -1000, -1000);
    addBackgroundImageLayer(background_1, 0.85, -1000, -1000);

    // game instructions
    QGraphicsTextItem *text = new QGraphicsTextItem("Press 'Esc' to exit fullscreen\n"
                                                    "Press 'r' to restart the map\n"
                                                    "Press 'q' to quit the game\n"
                                                    "\n"
                                                    "Controls:\n"
                                                    "Arrows to move\n"
                                                    "Use [shift] to sprint and jump higher\n"
                                                    "Hold and release [space] to throw grenades");
    text->setFont(QFont("Arial", 15));
    addBackgroundImageLayer(text, 1.0, -600, -300);

    QGraphicsTextItem *mission_text = new QGraphicsTextItem("FIND THE GOAL");
    mission_text->setFont(QFont("Arial", 50));
    qreal x = views().at(0)->mapToScene((views().at(0)->viewport()->width() - mission_text->boundingRect().width())/2, (views().at(0)->viewport()->height() - mission_text->boundingRect().height())/2).x();
    qreal y = views().at(0)->mapToScene((views().at(0)->viewport()->width() - mission_text->boundingRect().width())/2, (views().at(0)->viewport()->height() - mission_text->boundingRect().height())/2).y();
    addBackgroundImageLayer(mission_text, 0.5, x, y);

    // foreground

    qreal offset_x = 0;
    qreal offset_y = 0;

    // top floor
    {
        GameObject *steelbar = m_factory->create(STEELBARHORIZONTAL);
        steelbar->setPos(0 + offset_x, 0 + offset_y);
        addGameObject(steelbar);

        GameObject *steelbar2 = m_factory->create(STEELBARHORIZONTAL);
        steelbar2->setPos(200 + offset_x, 0 + offset_y);
        addGameObject(steelbar2);

        GameObject *steelbar3 = m_factory->create(STEELBARHORIZONTAL);
        steelbar3->setPos(400 + offset_x, 0 + offset_y);
        addGameObject(steelbar3);

        GameObject *steelbar4 = m_factory->create(STEELBARHORIZONTAL);
        steelbar4->setPos(600 + offset_x, 0 + offset_y);
        addGameObject(steelbar4);

        GameObject *steelbar5 = m_factory->create(STEELBARHORIZONTAL);
        steelbar5->setPos(800 + offset_x, 0 + offset_y);
        addGameObject(steelbar5);

        GameObject *steelbar6 = m_factory->create(STEELBARHORIZONTAL);
        steelbar6->setPos(1000 + offset_x, 0 + offset_y);
        addGameObject(steelbar6);

        GameObject *steelbar7 = m_factory->create(STEELBARHORIZONTAL);
        steelbar7->setPos(1200 + offset_x, 0 + offset_y);
        addGameObject(steelbar7);

        GameObject *steelbar8 = m_factory->create(STEELBARHORIZONTAL);
        steelbar8->setPos(1400 + offset_x, 0 + offset_y);
        addGameObject(steelbar8);

        GameObject *steelbar9 = m_factory->create(STEELBARHORIZONTAL);
        steelbar9->setPos(1600 + offset_x, 0 + offset_y);
        addGameObject(steelbar9);

        GameObject *steelbar10 = m_factory->create(STEELBARHORIZONTAL);
        steelbar10->setPos(1800 + offset_x, 0 + offset_y);
        addGameObject(steelbar10);

        GameObject *ice2 = m_factory->create(STEELBARHORIZONTAL);
        ice2->setPos(2000 + offset_x, 0 + offset_y);
        addGameObject(ice2);

        GameObject *ice3 = m_factory->create(STEELBARHORIZONTAL);
        ice3->setPos(2200 + offset_x, 0 + offset_y);
        addGameObject(ice3);

        GameObject *ice4 = m_factory->create(STEELBARHORIZONTAL);
        ice4->setPos(2400 + offset_x, 0 + offset_y);
        addGameObject(ice4);

        GameObject *ice5 = m_factory->create(STEELBARHORIZONTAL);
        ice5->setPos(2600 + offset_x, 0 + offset_y);
        addGameObject(ice5);

        GameObject *steelbar11 = m_factory->create(STEELBARHORIZONTAL);
        steelbar11->setPos(2800 + offset_x, 0 + offset_y);
        addGameObject(steelbar11);

        GameObject *ladder01 = m_factory->create(LADDER);
        ladder01->setPos(2600 + offset_x, -200 + offset_y);
        addGameObject(ladder01);

        GameObject *surrogate_symbol = m_factory->create(SURROGATE_SYMBOL_OBJECT);
        surrogate_symbol->setPos(2700 + offset_x, -40 + offset_y);
        addGameObject(surrogate_symbol);

        GameObject *steelbar12 = m_factory->create(STEELBARVERTICAL);
        steelbar12->setPos(3000 + offset_x, -180 + offset_y);
        addGameObject(steelbar12);

        GameObject *ladder = m_factory->create(LADDER);
        ladder->setPos(2980 + offset_x, -200 + offset_y);
        addGameObject(ladder);

        GameObject *ladder2 = m_factory->create(LADDER);
        ladder2->setPos(3160 + offset_x, -200 + offset_y);
        addGameObject(ladder2);

        GameObject *ladder3 = m_factory->create(LADDER);
        ladder3->setPos(3160 + offset_x, 0 + offset_y);
        addGameObject(ladder3);

        GameObject *wormhole = m_factory->create(WORMHOLE);
        wormhole->setPos(1500, -60);
        addGameObject(wormhole);
        static_cast<Wormhole*>(wormhole)->setExitPos(400, 300);
    }

    offset_x = 0;
    offset_y = 200;

    // floor top - 1
    {
        GameObject *steelbar = m_factory->create(STEELBARHORIZONTAL);
        steelbar->setPos(0 + offset_x, 0 + offset_y);
        addGameObject(steelbar);

        GameObject *steelbar2 = m_factory->create(ICEFIELD);
        steelbar2->setPos(200 + offset_x, 0 + offset_y);
        addGameObject(steelbar2);

        GameObject *steelbar3 = m_factory->create(STEELBARHORIZONTAL);
        steelbar3->setPos(400 + offset_x, 0 + offset_y);
        addGameObject(steelbar3);

        GameObject *steelbar4 = m_factory->create(ICEFIELD);
        steelbar4->setPos(600 + offset_x, 0 + offset_y);
        addGameObject(steelbar4);

        GameObject *steelbar5 = m_factory->create(ICEFIELD);
        steelbar5->setPos(800 + offset_x, 0 + offset_y);
        addGameObject(steelbar5);

        GameObject *steelbar6 = m_factory->create(ICEFIELD);
        steelbar6->setPos(1000 + offset_x, 0 + offset_y);
        addGameObject(steelbar6);

        GameObject *steelbar7 = m_factory->create(ICEFIELD);
        steelbar7->setPos(1200 + offset_x, 0 + offset_y);
        addGameObject(steelbar7);

        GameObject *steelbar8 = m_factory->create(ICEFIELD);
        steelbar8->setPos(1400 + offset_x, 0 + offset_y);
        addGameObject(steelbar8);

        GameObject *enemy = m_factory->create(ENEMY);
        enemy->setPos(1500 + offset_x, -50 + offset_y);
        addGameObject(enemy);

        GameObject *steelbar9 = m_factory->create(STEELBARHORIZONTAL);
        steelbar9->setPos(1600 + offset_x, 0 + offset_y);
        addGameObject(steelbar9);

        GameObject *steelbar10 = m_factory->create(STEELBARHORIZONTAL);
        steelbar10->setPos(1800 + offset_x, 0 + offset_y);
        addGameObject(steelbar10);

        GameObject *ice2 = m_factory->create(STEELBARHORIZONTAL);
        ice2->setPos(2000 + offset_x, 0 + offset_y);
        addGameObject(ice2);

        GameObject *ice3 = m_factory->create(STEELBARHORIZONTAL);
        ice3->setPos(2200 + offset_x, 0 + offset_y);
        addGameObject(ice3);

        GameObject *ice4 = m_factory->create(STEELBARHORIZONTAL);
        ice4->setPos(2400 + offset_x, 0 + offset_y);
        addGameObject(ice4);

        GameObject *ice5 = m_factory->create(STEELBARHORIZONTAL);
        ice5->setPos(2600 + offset_x, 0 + offset_y);
        addGameObject(ice5);

        GameObject *steelbar11 = m_factory->create(STEELBARHORIZONTAL);
        steelbar11->setPos(2800 + offset_x, 0 + offset_y);
        addGameObject(steelbar11);

        GameObject *steelbar12 = m_factory->create(STEELBARHORIZONTAL);
        steelbar12->setPos(3000 + offset_x, 0 + offset_y);
        addGameObject(steelbar12);
    }

    offset_x = 0;
    offset_y = 400;

    // floor top - 2
    {
        GameObject *steelbar = m_factory->create(STEELBARHORIZONTAL);
        steelbar->setPos(0 + offset_x, 0 + offset_y);
        addGameObject(steelbar);

        GameObject *steelbar2 = m_factory->create(STEELBARHORIZONTAL);
        steelbar2->setPos(200 + offset_x, 0 + offset_y);
        addGameObject(steelbar2);

        GameObject *steelbar3 = m_factory->create(STEELBARHORIZONTAL);
        steelbar3->setPos(400 + offset_x, 0 + offset_y);
        addGameObject(steelbar3);

        GameObject *steelbar4 = m_factory->create(ICEFIELD);
        steelbar4->setPos(600 + offset_x, 10 + offset_y);
        addGameObject(steelbar4);

        GameObject *steelbar5 = m_factory->create(ICEFIELD);
        steelbar5->setPos(800 + offset_x, 20 + offset_y);
        addGameObject(steelbar5);

        GameObject *steelbar6 = m_factory->create(ICEFIELD);
        steelbar6->setPos(1000 + offset_x, 30 + offset_y);
        addGameObject(steelbar6);

        GameObject *steelbar7 = m_factory->create(ICEFIELD);
        steelbar7->setPos(1200 + offset_x, 40 + offset_y);
        addGameObject(steelbar7);

        GameObject *steelbar8 = m_factory->create(ICEFIELD);
        steelbar8->setPos(1400 + offset_x, 50 + offset_y);
        addGameObject(steelbar8);

        GameObject *steelbar9 = m_factory->create(ICEFIELD);
        steelbar9->setPos(1600 + offset_x, 60 + offset_y);
        addGameObject(steelbar9);

        GameObject *steelbar10 = m_factory->create(ICEFIELD);
        steelbar10->setPos(1800 + offset_x, 70 + offset_y);
        addGameObject(steelbar10);

        GameObject *ice2 = m_factory->create(ICEFIELD);
        ice2->setPos(2000 + offset_x, 80 + offset_y);
        addGameObject(ice2);

        GameObject *ice3 = m_factory->create(ICEFIELD);
        ice3->setPos(2200 + offset_x, 90 + offset_y);
        addGameObject(ice3);

        GameObject *ice4 = m_factory->create(ICEFIELD);
        ice4->setPos(2400 + offset_x, 100 + offset_y);
        addGameObject(ice4);

        GameObject *ice5 = m_factory->create(ICEFIELD);
        ice5->setPos(2600 + offset_x, 110 + offset_y);
        addGameObject(ice5);

        GameObject *steelbar11 = m_factory->create(ICEFIELD);
        steelbar11->setPos(2800 + offset_x, 120 + offset_y);
        addGameObject(steelbar11);

        GameObject *steelbar12 = m_factory->create(STEELBARHORIZONTAL);
        steelbar12->setPos(3000 + offset_x, 130 + offset_y);
        addGameObject(steelbar12);

        GameObject *steelbar13 = m_factory->create(STEELBARHORIZONTAL);
        steelbar13->setPos(3300 + offset_x, 330 + offset_y);
        addGameObject(steelbar13);

        GameObject *steelbar14 = m_factory->create(STEELBARHORIZONTAL);
        steelbar14->setPos(3700 + offset_x, 330 + offset_y);
        addGameObject(steelbar14);

        GameObject *grenade_symbol = m_factory->create(GRENADE_SYMBOL_OBJECT);
        grenade_symbol->setPos(3400 + offset_x, 300 + offset_y);
        addGameObject(grenade_symbol);
    }


    offset_x = 4000;
    offset_y = 750;

    // floor top - 3
    {
        GameObject *steelbar = m_factory->create(STEELBARHORIZONTAL);
        steelbar->setPos(0 + offset_x, 0 + offset_y);
        addGameObject(steelbar);

        GameObject *steelbar2 = m_factory->create(STEELBARHORIZONTAL);
        steelbar2->setPos(200 + offset_x, 0 + offset_y);
        addGameObject(steelbar2);

        GameObject *steelbar3 = m_factory->create(STEELBARHORIZONTAL);
        steelbar3->setPos(400 + offset_x, 0 + offset_y);
        addGameObject(steelbar3);

        GameObject *steelbar4 = m_factory->create(STEELBARHORIZONTAL);
        steelbar4->setPos(600 + offset_x, 0 + offset_y);
        addGameObject(steelbar4);

        GameObject *steelbar5 = m_factory->create(STEELBARHORIZONTAL);
        steelbar5->setPos(800 + offset_x, 0 + offset_y);
        addGameObject(steelbar5);

        GameObject *steelbar6 = m_factory->create(STEELBARHORIZONTAL);
        steelbar6->setPos(1000 + offset_x, 0 + offset_y);
        addGameObject(steelbar6);

        GameObject *steelbar7 = m_factory->create(STEELBARHORIZONTAL);
        steelbar7->setPos(1200 + offset_x, 0 + offset_y);
        addGameObject(steelbar7);

        GameObject *steelbar8 = m_factory->create(STEELBARHORIZONTAL);
        steelbar8->setPos(1400 + offset_x, 0 + offset_y);
        addGameObject(steelbar8);

        GameObject *steelbar9 = m_factory->create(STEELBARHORIZONTAL);
        steelbar9->setPos(1600 + offset_x, 0 + offset_y);
        addGameObject(steelbar9);

        GameObject *steelbar10 = m_factory->create(STEELBARHORIZONTAL);
        steelbar10->setPos(1800 + offset_x, 0 + offset_y);
        addGameObject(steelbar10);

        GameObject *ice2 = m_factory->create(STEELBARHORIZONTAL);
        ice2->setPos(2000 + offset_x, 0 + offset_y);
        addGameObject(ice2);

        GameObject *ice3 = m_factory->create(STEELBARHORIZONTAL);
        ice3->setPos(2200 + offset_x, 0 + offset_y);
        addGameObject(ice3);

        GameObject *ice4 = m_factory->create(STEELBARHORIZONTAL);
        ice4->setPos(2400 + offset_x, 0 + offset_y);
        addGameObject(ice4);

        GameObject *ice5 = m_factory->create(STEELBARHORIZONTAL);
        ice5->setPos(2600 + offset_x, 0 + offset_y);
        addGameObject(ice5);

        GameObject *steelbar11 = m_factory->create(STEELBARHORIZONTAL);
        steelbar11->setPos(2800 + offset_x, 0 + offset_y);
        addGameObject(steelbar11);

        GameObject *steelbar12 = m_factory->create(STEELBARVERTICAL);
        steelbar12->setPos(3000 + offset_x, -200 + offset_y);
        addGameObject(steelbar12);

        GameObject *steelbar13 = m_factory->create(STEELBARVERTICAL);
        steelbar13->setPos(3000 + offset_x, -400 + offset_y);
        addGameObject(steelbar13);

        GameObject *steelbar14 = m_factory->create(STEELBARVERTICAL);
        steelbar14->setPos(3000 + offset_x, -600 + offset_y);
        addGameObject(steelbar14);

        GameObject *enemy = m_factory->create(ENEMY);
        enemy->setPos(1000 + offset_x, -50 + offset_y);
        addGameObject(enemy);

        ForceField *forcefield = static_cast<ForceField*>(m_factory->create(FORCEFIELD));
        forcefield->setForce(0, -3);
        forcefield->setPos(2800 + offset_x, -175 + offset_y);
        addGameObject(forcefield);

        ForceField *forcefield2 = static_cast<ForceField*>(m_factory->create(FORCEFIELD));
        forcefield2->setForce(0, -3);
        forcefield2->setPos(2800 + offset_x, -525 + offset_y);
        addGameObject(forcefield2);

        ForceField *forcefield3 = static_cast<ForceField*>(m_factory->create(FORCEFIELD));
        forcefield3->setForce(0, -3);
        forcefield3->setPos(2800 + offset_x, -875 + offset_y);
        addGameObject(forcefield3);
    }

    offset_x = 6900;
    offset_y = -1000;

    // final floor
    {
        GameObject *wall2 = m_factory->create(STEELBARVERTICAL);
        wall2->setPos(-500 + offset_x, -200 + offset_y);
        addGameObject(wall2);

        GameObject *bar = m_factory->create(STEELBARHORIZONTAL);
        bar->setPos(-300 + offset_x, 0 + offset_y);
        addGameObject(bar);

        GameObject *bar2 = m_factory->create(STEELBARHORIZONTAL);
        bar2->setPos(-500 + offset_x, 0 + offset_y);
        addGameObject(bar2);

        GameObject *steelbar = m_factory->create(STEELBARHORIZONTAL);
        steelbar->setPos(0 + offset_x, 0 + offset_y);
        addGameObject(steelbar);

        GameObject *steelbar2 = m_factory->create(STEELBARHORIZONTAL);
        steelbar2->setPos(200 + offset_x, 0 + offset_y);
        addGameObject(steelbar2);

        GameObject *steelbar3 = m_factory->create(STEELBARHORIZONTAL);
        steelbar3->setPos(400 + offset_x, 0 + offset_y);
        addGameObject(steelbar3);

        GameObject *enemy = m_factory->create(ENEMY);
        enemy->setPos(500 + offset_x, -50 + offset_y);
        addGameObject(enemy);

        GameObject *wall3 = m_factory->create(STEELBARVERTICAL);
        wall3 ->setPos(580 + offset_x, 20 + offset_y);
        addGameObject(wall3 );

        GameObject *steelbar4 = m_factory->create(ICEFIELD);
        steelbar4->setPos(600 + offset_x, 0 + offset_y);
        addGameObject(steelbar4);

        GameObject *steelbar5 = m_factory->create(STEELBARHORIZONTAL);
        steelbar5->setPos(800 + offset_x, 0 + offset_y);
        addGameObject(steelbar5);

        GameObject *wall = m_factory->create(STEELBARVERTICAL);
        wall->setPos(1000 + offset_x, -200 + offset_y);
        addGameObject(wall);

        GameObject *steelbar6 = m_factory->create(STEELBARHORIZONTAL);
        steelbar6->setPos(1000 + offset_x, 0 + offset_y);
        addGameObject(steelbar6);

        GameObject *steelbar7 = m_factory->create(ICEFIELD);
        steelbar7->setPos(1200 + offset_x, 0 + offset_y);
        addGameObject(steelbar7);

        GameObject *steelbar8 = m_factory->create(STEELBARHORIZONTAL);
        steelbar8->setPos(1400 + offset_x, 0 + offset_y);
        addGameObject(steelbar8);

        GameObject *steelbar9 = m_factory->create(STEELBARHORIZONTAL);
        steelbar9->setPos(1600 + offset_x, 0 + offset_y);
        addGameObject(steelbar9);

        GameObject *steelbar10 = m_factory->create(STEELBARHORIZONTAL);
        steelbar10->setPos(1800 + offset_x, 0 + offset_y);
        addGameObject(steelbar10);

        GameObject *ice2 = m_factory->create(STEELBARHORIZONTAL);
        ice2->setPos(2000 + offset_x, 0 + offset_y);
        addGameObject(ice2);

        GameObject *ice3 = m_factory->create(STEELBARHORIZONTAL);
        ice3->setPos(2200 + offset_x, 0 + offset_y);
        addGameObject(ice3);

        GameObject *ice4 = m_factory->create(STEELBARHORIZONTAL);
        ice4->setPos(2400 + offset_x, 0 + offset_y);
        addGameObject(ice4);

        GameObject *ice5 = m_factory->create(STEELBARHORIZONTAL);
        ice5->setPos(2600 + offset_x, 0 + offset_y);
        addGameObject(ice5);

        GameObject *steelbar11 = m_factory->create(STEELBARHORIZONTAL);
        steelbar11->setPos(2800 + offset_x, 0 + offset_y);
        addGameObject(steelbar11);

        GameObject *steelbar12 = m_factory->create(STEELBARVERTICAL);
        steelbar12->setPos(3000 + offset_x, -200 + offset_y);
        addGameObject(steelbar12);

        GameObject *steelbar17 = m_factory->create(STEELBARHORIZONTAL);
        steelbar17->setPos(800 + offset_x, 200 + offset_y);
        addGameObject(steelbar17);

        GameObject *steelbar13 = m_factory->create(STEELBARHORIZONTAL);
        steelbar13->setPos(1000 + offset_x, 200 + offset_y);
        addGameObject(steelbar13);

        GameObject *steelbar14 = m_factory->create(STEELBARHORIZONTAL);
        steelbar14->setPos(1200 + offset_x, 200 + offset_y);
        addGameObject(steelbar14);

        GameObject *steelbar15 = m_factory->create(STEELBARHORIZONTAL);
        steelbar15->setPos(1400 + offset_x, 200 + offset_y);
        addGameObject(steelbar15);

        GameObject *steelbar16 = m_factory->create(STEELBARHORIZONTAL);
        steelbar16->setPos(1600 + offset_x, 200 + offset_y);
        addGameObject(steelbar16);

        GameObject *wall4 = m_factory->create(STEELBARVERTICAL);
        wall4->setPos(1780 + offset_x, 0 + offset_y);
        addGameObject(wall4);

        GameObject *drone_symbol = m_factory->create(DRONE_SYMBOL_OBJECT);
        drone_symbol->setPos(- 300  + offset_x, -40 + offset_y);
        addGameObject(drone_symbol);

        GameObject *surrogate_symbol = m_factory->create(SURROGATE_SYMBOL_OBJECT);
        surrogate_symbol->setPos(1300 + offset_x, -40 + offset_y);
        addGameObject(surrogate_symbol);

        GameObject *ladder4 = m_factory->create(LADDER);
        ladder4->setPos(-50 + offset_x, 400 + offset_y);
        addGameObject(ladder4);

        GameObject *ladder5 = m_factory->create(LADDER);
        ladder5->setPos(-50 + offset_x, 200 + offset_y);
        addGameObject(ladder5);

        GameObject *ladder6 = m_factory->create(LADDER);
        ladder6->setPos(-50 + offset_x, 0 + offset_y);
        addGameObject(ladder6);

        GameObject *ladder7 = m_factory->create(LADDER);
        ladder7->setPos(-50 + offset_x, -200 + offset_y);
        addGameObject(ladder7);
    }

    offset_x = 0;
    offset_y = 1000;

    // bottom floor
    {
        GameObject *steelbar02 = m_factory->create(STEELBARHORIZONTAL);
        steelbar02->setPos(-400 + offset_x, 0 + offset_y);
        addGameObject(steelbar02);

        GameObject *steelbar01 = m_factory->create(STEELBARHORIZONTAL);
        steelbar01->setPos(-200 + offset_x, 0 + offset_y);
        addGameObject(steelbar01);

        GameObject *steelbar = m_factory->create(STEELBARHORIZONTAL);
        steelbar->setPos(0 + offset_x, 0 + offset_y);
        addGameObject(steelbar);

        GameObject *steelbar2 = m_factory->create(STEELBARHORIZONTAL);
        steelbar2->setPos(200 + offset_x, 0 + offset_y);
        addGameObject(steelbar2);

        GameObject *steelbar3 = m_factory->create(STEELBARHORIZONTAL);
        steelbar3->setPos(400 + offset_x, 0 + offset_y);
        addGameObject(steelbar3);

        GameObject *enemy = m_factory->create(ENEMY);
        enemy->setPos(500 + offset_x, -50 + offset_y);
        addGameObject(enemy);

        GameObject *steelbar4 = m_factory->create(STEELBARHORIZONTAL);
        steelbar4->setPos(600 + offset_x, 0 + offset_y);
        addGameObject(steelbar4);

        GameObject *steelbar5 = m_factory->create(STEELBARHORIZONTAL);
        steelbar5->setPos(800 + offset_x, 0 + offset_y);
        addGameObject(steelbar5);

        GameObject *steelbar6 = m_factory->create(STEELBARHORIZONTAL);
        steelbar6->setPos(1000 + offset_x, 0 + offset_y);
        addGameObject(steelbar6);

        GameObject *steelbar7 = m_factory->create(STEELBARHORIZONTAL);
        steelbar7->setPos(1200 + offset_x, 0 + offset_y);
        addGameObject(steelbar7);

        GameObject *steelbar8 = m_factory->create(STEELBARHORIZONTAL);
        steelbar8->setPos(1400 + offset_x, 0 + offset_y);
        addGameObject(steelbar8);

        GameObject *steelbar9 = m_factory->create(STEELBARHORIZONTAL);
        steelbar9->setPos(1600 + offset_x, 0 + offset_y);
        addGameObject(steelbar9);

        GameObject *steelbar10 = m_factory->create(STEELBARHORIZONTAL);
        steelbar10->setPos(1800 + offset_x, 0 + offset_y);
        addGameObject(steelbar10);

        GameObject *ice2 = m_factory->create(STEELBARHORIZONTAL);
        ice2->setPos(2000 + offset_x, 0 + offset_y);
        addGameObject(ice2);

        GameObject *ice3 = m_factory->create(STEELBARHORIZONTAL);
        ice3->setPos(2200 + offset_x, 0 + offset_y);
        addGameObject(ice3);

        GameObject *ice4 = m_factory->create(STEELBARHORIZONTAL);
        ice4->setPos(2400 + offset_x, 0 + offset_y);
        addGameObject(ice4);

        GameObject *ice5 = m_factory->create(STEELBARHORIZONTAL);
        ice5->setPos(2600 + offset_x, 0 + offset_y);
        addGameObject(ice5);

        GameObject *steelbar11 = m_factory->create(STEELBARHORIZONTAL);
        steelbar11->setPos(2800 + offset_x, 0 + offset_y);
        addGameObject(steelbar11);

        GameObject *steelbar12 = m_factory->create(STEELBARHORIZONTAL);
        steelbar12->setPos(3000 + offset_x, 0 + offset_y);
        addGameObject(steelbar12);

        GameObject *steelbar13 = m_factory->create(STEELBARHORIZONTAL);
        steelbar13->setPos(3200 + offset_x, 0 + offset_y);
        addGameObject(steelbar13);

        GameObject *steelbar14 = m_factory->create(STEELBARHORIZONTAL);
        steelbar14->setPos(3400 + offset_x, 0 + offset_y);
        addGameObject(steelbar14);

        GameObject *steelbar15 = m_factory->create(STEELBARHORIZONTAL);
        steelbar15->setPos(3600 + offset_x, 0 + offset_y);
        addGameObject(steelbar15);

        GameObject *steelbar16 = m_factory->create(STEELBARHORIZONTAL);
        steelbar16->setPos(3800 + offset_x, 0 + offset_y);
        addGameObject(steelbar16);

        GameObject *drone_symbol = m_factory->create(DRONE_SYMBOL_OBJECT);
        drone_symbol->setPos(4050  + offset_x, -40 + offset_y);
        addGameObject(drone_symbol);

        GameObject *steelbar17 = m_factory->create(STEELBARHORIZONTAL);
        steelbar17->setPos(4000 + offset_x, 0 + offset_y);
        addGameObject(steelbar17);

        GameObject *enemy2 = m_factory->create(ENEMY);
        enemy2->setPos(3400 + offset_x, -50 + offset_y);
        addGameObject(enemy2);

        GameObject *ladder2 = m_factory->create(LADDER);
        ladder2->setPos(3260 + offset_x, -200 + offset_y);
        addGameObject(ladder2);

        GameObject *ladder3 = m_factory->create(LADDER);
        ladder3->setPos(3260 + offset_x, -400 + offset_y);
        addGameObject(ladder3);
    }

    // player

    Controller *controller = new Controller();
    connect(this, &GameScene::command, controller, &Controller::setCommands);

    m_player = m_factory->create(PLAYER);
    m_player->setController(controller);
    m_player->setPos(0, -80);
    addGameObject(m_player, true);

    GameObjectDetector *detector = new GameObjectDetector(m_player);
    detector->setPos(8600, -840);
    addGameObject(static_cast<GameObject*>(detector));

    QGraphicsTextItem *goal_text = new QGraphicsTextItem("GOAL");
    goal_text->setFont(QFont("Arial", 30));
    goal_text->setPos(8550, -860);
    addItem(goal_text);

    // game world event mechanisms

    GameObjectEater *eater = new GameObjectEater(15000, 1000);
    eater->setPos(-5000, 5000);
    addGameObject(eater);

    connect(detector, &GameObjectDetector::targetDetected, this, &DemoGame::winEvent);
    connect(static_cast<Player*>(m_player), &Player::died, this, &DemoGame::loseEvent);
    connect(eater, &GameObjectEater::playerEaten, this, &DemoGame::loseEvent);

    // sounds

    // registers a shared sound bank of miscellaneous sound effects that many sources can use without registering themselves
    m_sound_manager->registerSource(QUuid(0x11111111, 0x1111, 0x1111, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11), ":/sounds/misc/");

    m_sound_manager->playSoundtrack(m_uuid, "demogame_soundtrack", 0.7, -1);
}

void DemoGame::winEvent()
{
    QGraphicsTextItem *win_text = new QGraphicsTextItem("YOU WIN");
    win_text->setFont(QFont("Arial", 100));
    qreal x = views().at(0)->mapToScene((views().at(0)->viewport()->width() - win_text->boundingRect().width())/2, (views().at(0)->viewport()->height() - win_text->boundingRect().height())/2).x();
    qreal y = views().at(0)->mapToScene((views().at(0)->viewport()->width() - win_text->boundingRect().width())/2, (views().at(0)->viewport()->height() - win_text->boundingRect().height())/2).y();
    addBackgroundImageLayer(win_text, 1.0, x, y);
}

void DemoGame::loseEvent()
{
    QGraphicsTextItem *lose_text = new QGraphicsTextItem("GAME OVER");
    lose_text->setFont(QFont("Arial", 100));
    lose_text->setPos(views().at(0)->mapToScene((views().at(0)->viewport()->width() - lose_text->boundingRect().width())/2, (views().at(0)->viewport()->height() - lose_text->boundingRect().height())/2));
    qreal x = views().at(0)->mapToScene((views().at(0)->viewport()->width() - lose_text->boundingRect().width())/2, (views().at(0)->viewport()->height() - lose_text->boundingRect().height())/2).x();
    qreal y = views().at(0)->mapToScene((views().at(0)->viewport()->width() - lose_text->boundingRect().width())/2, (views().at(0)->viewport()->height() - lose_text->boundingRect().height())/2).y();
    addBackgroundImageLayer(lose_text, 1.0, x, y);
}
