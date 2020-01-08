#include "gamescene.h"
#include "attributesandstates.h"
#include <QGraphicsScene>
#include <chrono>

GameScene::GameScene(QObject* parent) :  QGraphicsScene(parent)
{
    m_timer = new QTimer(this);
    m_objectlist_changed = false;
    m_paused = false;
    m_running = false;
    m_uuid = QUuid::createUuid();
    m_factory = nullptr;
    m_sound_manager = nullptr;
    m_active_area_margins = 500;
}

GameScene::~GameScene()
{
    stopGameScene();
}

void GameScene::createGame()
{

}

void GameScene::addBackgroundImageLayer(QGraphicsItem *background_item, qreal parallax_factor, qreal initial_pos_x, qreal initial_pos_y)
{
    if(background_item != nullptr){
        if(parallax_factor > 1.0)
            m_background_layer_parallax_factors.append(1.0);
        else if(parallax_factor < 0.0)
            m_background_layer_parallax_factors.append(0.0);
        else
            m_background_layer_parallax_factors.append(parallax_factor);

        m_background_image_layers.append(background_item);
        addItem(background_item);
        background_item->setPos(initial_pos_x, initial_pos_y);
    }
}

void GameScene::updateBackgroundParallax(qreal viewpoint_shift_dx, qreal viewpoint_shift_dy)
{
    for(int i= 0; i < m_background_image_layers.length(); i++){
        m_background_image_layers.at(i)->moveBy(viewpoint_shift_dx * m_background_layer_parallax_factors.at(i), viewpoint_shift_dy * m_background_layer_parallax_factors.at(i));
    }
}

void GameScene::updateActiveGameArea(QRectF visible_area){
    m_active_area = QRectF(visible_area.x() - m_active_area_margins, visible_area.y() - m_active_area_margins, visible_area.width() + 2 * m_active_area_margins, visible_area.height() + 2 * m_active_area_margins);
}

void GameScene::setActiveAreaMargins(qreal margin_size){
    m_active_area_margins = margin_size;
}

void GameScene::addGameObject(GameObject *gameobject, bool tracked)
{
    if(gameobject != nullptr){
        if(tracked){
            m_tracked_objects_to_add.append(gameobject);
        }

        m_objects_to_add.append(gameobject);

        m_objectlist_changed = true;
    }
}

void GameScene::removeGameObject(GameObject *gameobject)
{
    if(gameobject != nullptr){

        bool object_already_on_the_list = false;

        for(int i = 0; i < m_object_to_delete.length(); i++){
            if(m_object_to_delete.at(i) == gameobject)
                object_already_on_the_list = true;
        }

        if(!object_already_on_the_list){
            m_object_to_delete.append(gameobject);
            m_objectlist_changed = true;
        }
    }
}

void GameScene::makeObjectListChanges()
{
    // delete
    if(!m_object_to_delete.isEmpty()){
        for(int i = 0; i < m_object_to_delete.length(); i++){
            GameObject *object = m_object_to_delete.at(i);
            if(object != nullptr){
                for(int i = 0; i < m_tracked_objects.length(); i++){
                    if(object  == m_tracked_objects.at(i)){
                        m_tracked_objects.removeAt(i);
                        emit trackedObjectChanged();
                    }
                }

                removeItem(object);
                m_gameobject_list.removeOne(object);
                delete object;
            }
        }
        m_object_to_delete.clear();
    }

    // add
    if(!m_objects_to_add.isEmpty()){
        for(int i = 0; i < m_objects_to_add.length(); i++){
            GameObject *object = m_objects_to_add.at(i);
            if(object != nullptr){
                for(int i = 0; i < m_tracked_objects_to_add.length(); i++){
                    if(object == m_tracked_objects_to_add.at(i)){
                        m_tracked_objects.append(object);
                        emit trackedObjectChanged();
                    }
                }

                m_gameobject_list.append(object);
                addItem(object);
            }
        }
        m_tracked_objects_to_add.clear();
        m_objects_to_add.clear();
    }
    m_objectlist_changed = false;
}

const QList<GameObject*>* GameScene::trackedObjects()
{
    return static_cast<const QList<GameObject*>*>(&m_tracked_objects);
}

bool GameScene::isPaused()
{
    return m_paused;
}

bool GameScene::isRunning()
{
    return m_running;
}

void GameScene::startGameScene()
{
    if(m_paused || m_running)
        stopGameScene();

    m_factory = new GameObjectFactory();
    m_sound_manager = static_cast<SoundManager*>(m_factory->create(0));
    m_sound_manager->registerSource(m_uuid, ":/sounds/soundtrack/");
    createGame();
    makeObjectListChanges();
    connect(m_timer, &QTimer::timeout, this, &GameScene::advanceGame);
    m_phase = 1;
    m_timer->start(10);
    m_first_cycle = true;
    m_running = true;
    m_active_area = sceneRect();

    emit trackedObjectChanged();
    emit started();
}

void GameScene::pauseGameScene()
{
    if(m_running){
        m_timer->stop();
        m_paused = true;
        m_running = false;
        emit paused();
    }
}

void GameScene::continueGameScene()
{
    if(m_paused){
        m_timer->start(10);
        m_first_cycle = true;
        m_paused = false;
        m_running = true;
        emit trackedObjectChanged();
    }
}

void GameScene::stopGameScene()
{
    m_timer->stop();
    if(!m_gameobject_list.isEmpty()){
        for(int i = 0; i < m_gameobject_list.length(); i++){
            removeGameObject(m_gameobject_list.at(i));
        }
        makeObjectListChanges();
    }
    if(!m_background_image_layers.isEmpty()){
        for(int i= 0; i < m_background_image_layers.length(); i++){
            QGraphicsItem *item = m_background_image_layers.at(i);
            removeItem(item);
            m_background_image_layers.removeOne(item);
            delete item;
        }
        m_background_image_layers.clear();
        m_background_layer_parallax_factors.clear();
    }
    m_paused = false;
    m_running = false;

    delete m_sound_manager;
    delete m_factory;

    emit stopped();
}

void GameScene::advanceGame()
{
    if(m_first_cycle){
        m_previous_time_point = std::chrono::steady_clock::now();
        m_delta_time = 0;
        m_first_cycle = false;
    }

    if(m_phase == 2){
        std::chrono::steady_clock::time_point time_point = std::chrono::steady_clock::now();
        m_delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(time_point - m_previous_time_point).count();
        m_previous_time_point = time_point;

        if(m_delta_time > 30)
            m_delta_time = 30;
    }

    for(int i = 0; i < m_gameobject_list.length(); i++){
        GameObject *obj = m_gameobject_list.at(i);
        if(m_active_area.intersects(QRectF(obj->pos().x(), obj->pos().y(), obj->boundingRect().width(), obj->boundingRect().height())) || (obj->attributes() & (unsigned int) 1<<PLAYER_CONTROLLED)){
                obj->advanceObject((int)m_delta_time, m_phase);
        }
    }

    if(m_phase == 1) { m_phase = 2; } // change the phase
    else {
        m_phase = 1;
        if(m_objectlist_changed){
            makeObjectListChanges();
        }
        emit gameCycleFinished();       
    }
}
