#include "surfacephysics.h"
#include "attributesandstates.h"
#include "forcefield.h"
#include <math.h>

SurfacePhysics::SurfacePhysics(qreal relative_friction, qreal relative_attachment_strength, qreal relative_mass, QObject *parent): ObjectFeature(parent)
{
    if(relative_mass > 0 && relative_mass <= 1.0)
        m_mass = relative_mass;
    else if(relative_mass <= 0)
        m_mass = 0.1;
    else if(relative_mass > 1.0)
        m_mass = 1.0;

    if(relative_friction > 0 && relative_friction <= 1.0)
        m_friction = relative_friction;
    else if(relative_friction <= 0)
        m_friction = 0.1;
    else if(relative_friction > 1.0)
        m_friction = 1.0;

    if(relative_attachment_strength > 0 && relative_attachment_strength <= 1.0)
        m_attachment_strength = relative_attachment_strength;
    else if(relative_attachment_strength <= 0)
        m_attachment_strength = 0.1;
    else if(relative_attachment_strength > 1.0)
        m_attachment_strength = 1.0;

    m_velocity_x = 0.0;
    m_velocity_y = 0.0;
    m_velocity_for_next_cycle_x = 0.0;
    m_velocity_for_next_cycle_y = 0.0;

    m_attributes = 0;
    setAttribute(WALKABLE);
    setAttribute(ATTACHED);
    setAttribute(COLLIDEABLE);
    setAttribute(BOUNCY);

    m_state = FREE_FALLING;

    emit internalsChanged();
}

void SurfacePhysics::update()
{
    m_velocity_x = m_velocity_for_next_cycle_x;
    m_velocity_y = m_velocity_for_next_cycle_y;

    ////////////// INERTIA ///////////////
    m_databank->dx = m_velocity_x * m_databank->deltatime;
    m_databank->dy = m_velocity_y * m_databank->deltatime;

    ////////////// GRAVITY ///////////////
    if(!checkAttribute(m_attributes, ATTACHED)){
        m_databank->dy += (GRAVITY * pow(m_databank->deltatime, 2.0)/2);
    }

    qreal corrective_x = 0.0; // these are used to correct for intersecting bounding boxes
    qreal corrective_y = 0.0;

    //////// INTERACTIONS WITH OBJECTS ////////
    if(!m_databank->colliding_objects.isEmpty()){

        // forcefields
        for(int i = 0; i < m_databank->colliding_objects.length(); i++){
            if(checkAttribute(m_databank->colliding_objects.at(i)->attributes(), FORCING)){
                m_databank->dx += static_cast<ForceField*>(m_databank->colliding_objects.at(i))->force().x() * pow(m_databank->deltatime, 2.0)/2.0;
                m_databank->dy += static_cast<ForceField*>(m_databank->colliding_objects.at(i))->force().y() * pow(m_databank->deltatime, 2.0)/2.0;
            }
        }

        int index_of_the_surface_object_underneath = 0;
        bool colliding = false;

        for(int i = 0; i < m_databank->colliding_objects.length(); i++){
            if(checkAttribute(m_databank->colliding_objects.at(i)->attributes(), COLLIDEABLE)){

                colliding = true;

                bool colliding_object_is_on_top = false;
                bool colliding_object_is_the_surface_underneath = false;
                bool colliding_object_is_on_the_left = false;
                bool colliding_object_is_on_the_right = false;
                qreal col_obj_vel_x = m_databank->colliding_objects.at(i)->physicsPacket()->at(VELOCITY_X);
                qreal col_obj_vel_y = m_databank->colliding_objects.at(i)->physicsPacket()->at(VELOCITY_Y);
                int col_obj_state = m_databank->colliding_objects.at(i)->state();
                bool col_obj_is_attached = checkAttribute(m_databank->colliding_objects.at(i)->attributes(), ATTACHED);

                // stops objects from intersecting

                QPointF d_opposite_corners_1 = (m_owner->pos())    -    (m_databank->colliding_objects.at(i)->pos() + QPointF(m_databank->colliding_objects.at(i)->boundingRect().width(), m_databank->colliding_objects.at(i)->boundingRect().height()));
                QPointF d_opposite_corners_2 = (m_owner->pos() + QPointF(m_owner->boundingRect().width(), 0))    -     (m_databank->colliding_objects.at(i)->pos() + QPointF(0, m_databank->colliding_objects.at(i)->boundingRect().height()));
                QPointF d_opposite_corners_3 = (m_owner->pos() + QPointF(0, m_owner->boundingRect().height()))    -    (m_databank->colliding_objects.at(i)->pos() + QPointF(m_databank->colliding_objects.at(i)->boundingRect().width(), 0));
                QPointF d_opposite_corners_4 = (m_owner->pos() + QPointF(m_owner->boundingRect().width(), m_owner->boundingRect().height())   -   (m_databank->colliding_objects.at(i)->pos()));

                QList<qreal> distances;

                distances.append((sqrt(pow(d_opposite_corners_1.x(), 2.0) + pow(d_opposite_corners_1.y(), 2.0))));
                distances.append((sqrt(pow(d_opposite_corners_2.x(), 2.0) + pow(d_opposite_corners_2.y(), 2.0))));
                distances.append((sqrt(pow(d_opposite_corners_3.x(), 2.0) + pow(d_opposite_corners_3.y(), 2.0))));
                distances.append((sqrt(pow(d_opposite_corners_4.x(), 2.0) + pow(d_opposite_corners_4.y(), 2.0))));

                int number_of_the_selected_opposite_corners = 0; // this variable tells which corners are closest to each other
                qreal shortest_distance = 10000000.0;

                for(int i = 0; i < distances.length(); i++){
                    if(distances.at(i) < shortest_distance){
                        shortest_distance = distances.at(i);
                        number_of_the_selected_opposite_corners = i + 1;
                    }
                }

                if(number_of_the_selected_opposite_corners == 1){ // upper-left corner of the owner object
                    if(abs(d_opposite_corners_1.x()) > abs(d_opposite_corners_1.y())){
                        if(m_databank->dy < 0){
                            if(checkAttribute(m_databank->colliding_objects.at(i)->attributes(), ATTACHED))
                                corrective_y = + 1.0 + abs(m_owner->pos().y() - (m_databank->colliding_objects.at(i)->y() + m_databank->colliding_objects.at(i)->boundingRect().height()));
                            }
                            colliding_object_is_on_top = true;
                    }
                    else{
                        if(m_databank->dx < 0){
                            if(checkAttribute(m_databank->colliding_objects.at(i)->attributes(), ATTACHED)){
                                corrective_x = 1.0 - (m_owner->pos().x() - (m_databank->colliding_objects.at(i)->pos().x() + m_databank->colliding_objects.at(i)->boundingRect().width()));
                            }
                            else{
                                corrective_x += (1.0 - (m_owner->pos().x() - (m_databank->colliding_objects.at(i)->pos().x() + m_databank->colliding_objects.at(i)->boundingRect().width())));
                            }
                            colliding_object_is_on_the_left = true;
                        }
                    }
                }
                else if(number_of_the_selected_opposite_corners == 2){ // upper-right corner of the owner object
                    if(abs(d_opposite_corners_2.x()) > abs(d_opposite_corners_2.y())){
                        if(m_databank->dy < 0){
                            if(checkAttribute(m_databank->colliding_objects.at(i)->attributes(), ATTACHED))
                                corrective_y = + 1.0 + abs(m_owner->pos().y() - (m_databank->colliding_objects.at(i)->y() + m_databank->colliding_objects.at(i)->boundingRect().height()));
                        }
                        colliding_object_is_on_top = true;
                    }
                    else{
                        if(m_databank->dx > 0){
                            if(checkAttribute(m_databank->colliding_objects.at(i)->attributes(), ATTACHED)){
                                corrective_x = -(1.0 + (m_owner->pos().x() + m_owner->boundingRect().width()) - m_databank->colliding_objects.at(i)->pos().x());
                            }
                            else{
                                corrective_x += -(1.0 + (m_owner->pos().x() + m_owner->boundingRect().width()) - m_databank->colliding_objects.at(i)->pos().x());
                            }
                            colliding_object_is_on_the_right = true;
                        }
                    }
                }
                else if(number_of_the_selected_opposite_corners == 3){ // lower-left corner of the owner object
                    if(abs(d_opposite_corners_3.x()) > abs(d_opposite_corners_3.y())){
                        if(m_databank->dy > 0 && !checkAttribute(m_databank->colliding_objects.at(i)->attributes(), BOUNCY) && col_obj_vel_y >= 0.0 && (col_obj_state == ON_A_SURFACE || col_obj_is_attached)){
                            corrective_y = 0.5 + m_databank->colliding_objects.at(i)->pos().y() - (m_owner->pos().y() + m_owner->boundingRect().height());
                            m_state = ON_A_SURFACE;
                            index_of_the_surface_object_underneath = i;
                            colliding_object_is_the_surface_underneath = true;
                        }
                        else if(m_databank->dy > 0 && checkAttribute(m_databank->colliding_objects.at(i)->attributes(), BOUNCY)/* && col_obj_vel_y >= 0.0 && (col_obj_state == ON_A_SURFACE || col_obj_is_attached)*/){
                            qreal temp_dy = -m_databank->dy * BOUNCE_COEFFICIENT;
                            if(temp_dy > -1.0){
                                corrective_y = 0.5 + m_databank->colliding_objects.at(i)->pos().y() - (m_owner->pos().y() + m_owner->boundingRect().height());
                                m_state = ON_A_SURFACE;
                                index_of_the_surface_object_underneath = i;
                                colliding_object_is_the_surface_underneath = true;
                            }
                            else{
                                m_databank->dy = temp_dy;
                            }
                        }
                        else if(m_databank->dy == 0.0 && m_state == ON_A_SURFACE && col_obj_vel_y >= 0.0  && (col_obj_state == ON_A_SURFACE || col_obj_is_attached)){
                            // problem!
                            index_of_the_surface_object_underneath = i;
                            colliding_object_is_the_surface_underneath = true;
                        }
                        else{
                            m_state = FREE_FALLING;
                        }
                    }
                    else{
                        if(m_databank->dx < 0){
                            if(checkAttribute(m_databank->colliding_objects.at(i)->attributes(), ATTACHED)){
                                corrective_x = 1.0 - (m_owner->pos().x() - (m_databank->colliding_objects.at(i)->pos().x() + m_databank->colliding_objects.at(i)->boundingRect().width()));
                            }
                            else{
                                corrective_x += (1.0 - (m_owner->pos().x() - (m_databank->colliding_objects.at(i)->pos().x() + m_databank->colliding_objects.at(i)->boundingRect().width())));
                            }
                            colliding_object_is_on_the_left = true;
                        }
                    }
                }
                else if(number_of_the_selected_opposite_corners == 4){ // lower-right corner of the owner object
                    if(abs(d_opposite_corners_4.x()) > abs(d_opposite_corners_4.y())){
                        if(m_databank->dy > 0 && !checkAttribute(m_databank->colliding_objects.at(i)->attributes(), BOUNCY) && col_obj_vel_y >= 0.0  && (col_obj_state == ON_A_SURFACE || col_obj_is_attached)){
                            corrective_y = 0.5 + m_databank->colliding_objects.at(i)->pos().y() - (m_owner->pos().y() + m_owner->boundingRect().height());
                            m_state = ON_A_SURFACE;
                            index_of_the_surface_object_underneath = i;
                            colliding_object_is_the_surface_underneath = true;
                        }
                        else if(m_databank->dy > 0 && checkAttribute(m_databank->colliding_objects.at(i)->attributes(), BOUNCY)/* && col_obj_vel_y >= 0.0  && (col_obj_state == ON_A_SURFACE || col_obj_is_attached)*/){
                            qreal temp_dy = -m_databank->dy * BOUNCE_COEFFICIENT;
                            if(temp_dy > -1.0){
                                corrective_y = 0.5 + m_databank->colliding_objects.at(i)->pos().y() - (m_owner->pos().y() + m_owner->boundingRect().height());
                                m_state = ON_A_SURFACE;
                                index_of_the_surface_object_underneath = i;
                                colliding_object_is_the_surface_underneath = true;
                            }
                            else{
                                m_databank->dy = temp_dy;
                            }
                        }
                        else if(m_databank->dy == 0.0 && m_state == ON_A_SURFACE && col_obj_vel_y >= 0.0 && (col_obj_state == ON_A_SURFACE || col_obj_is_attached)){
                            // problem!
                            index_of_the_surface_object_underneath = i;
                            colliding_object_is_the_surface_underneath = true;
                        }
                        else{
                            m_state = FREE_FALLING;
                        }
                    }
                    else{
                        if(m_databank->dx > 0){
                            if(checkAttribute(m_databank->colliding_objects.at(i)->attributes(), ATTACHED)){
                                corrective_x = -(1.0 + (m_owner->pos().x() + m_owner->boundingRect().width()) - m_databank->colliding_objects.at(i)->pos().x());
                            }
                            else{
                                corrective_x += -(1.0 + (m_owner->pos().x() + m_owner->boundingRect().width()) - m_databank->colliding_objects.at(i)->pos().x());
                            }
                            colliding_object_is_on_the_right = true;
                        }
                    }
                }
                // momentum exchange between loose objects
                if(!checkAttribute(m_databank->colliding_objects.at(i)->attributes(), ATTACHED) && !checkAttribute(m_attributes, ATTACHED)){
                    qreal mv_sum_x = m_velocity_x * m_mass + col_obj_vel_x * m_databank->colliding_objects.at(i)->physicsPacket()->at(MASS);
                    qreal mv_sum_y = m_velocity_y * m_mass + col_obj_vel_y * m_databank->colliding_objects.at(i)->physicsPacket()->at(MASS);
                    if(!colliding_object_is_on_top && !colliding_object_is_the_surface_underneath){
                        m_databank->dx += (-m_velocity_x + mv_sum_x / (m_mass + m_databank->colliding_objects.at(i)->physicsPacket()->at(MASS))) * m_databank->deltatime;
                    }
                    if(!(colliding_object_is_on_top && col_obj_state == ON_A_SURFACE)){
                        if(m_state != ON_A_SURFACE)
                            m_databank->dy += (-m_velocity_y + mv_sum_y / (m_mass + m_databank->colliding_objects.at(i)->physicsPacket()->at(MASS))) * m_databank->deltatime;
                        else
                            m_databank->dy = 0;
                    }
                }
                else if(checkAttribute(m_databank->colliding_objects.at(i)->attributes(), ATTACHED)){
                    if(colliding_object_is_on_top){
                        if(m_databank->dy < 0.0)
                            m_databank->dy = 0.0; // stopped by an attached object                            
                    }
                    if(colliding_object_is_the_surface_underneath){
                        if(m_databank->dy > 0.0)
                            m_databank->dy = 0.0; // stopped by an attached object
                    }
                    if(colliding_object_is_on_the_left){
                        if(m_databank->dx < 0.0)
                            m_databank->dx = 0.0;
                    }
                    if(colliding_object_is_on_the_right){
                        if(m_databank->dx > 0.0)
                            m_databank->dx = 0.0;
                    }
                }
                // the surface is being impacted from above
                if(checkAttribute(m_attributes, ATTACHED) && m_databank->colliding_objects.at(i)->physicsPacket()->at(VELOCITY_Y) >= 0.0 && !checkAttribute(m_databank->colliding_objects.at(i)->attributes(), ATTACHED)){
                    qreal impulse_object = m_databank->colliding_objects.at(i)->physicsPacket()->at(VELOCITY_Y) * m_databank->deltatime * m_databank->colliding_objects.at(i)->physicsPacket()->at(MASS) / m_databank->deltatime;
                    qreal mg_object = m_databank->colliding_objects.at(i)->physicsPacket()->at(MASS) * MASS_CONSTANT * GRAVITY;
                    if((impulse_object + mg_object) > m_attachment_strength * ATTACHMENT_STRENGTH_CONSTANT){
                        setAttribute(ATTACHED, false);
                    }
                }
            }
        }

        // friction on a surface
        if(m_state == ON_A_SURFACE  && colliding){
            if (m_databank->dx < 0){ // kinetic friction while slowing down or idling -- sliding
                m_databank->dx += GRAVITY * KINETIC_FRICTION_COEFFICIENT * m_databank->colliding_objects.at(index_of_the_surface_object_underneath)->physicsPacket()->at(SURFACE_FRICTION) * pow(m_databank->deltatime, 2.0)/2.0;
                if(m_databank->dx > 0)
                    m_databank->dx = 0;
            }
            if (m_databank->dx > 0){ // kinetic friction while slowing down or idling -- sliding
                m_databank->dx -= GRAVITY * KINETIC_FRICTION_COEFFICIENT * m_databank->colliding_objects.at(index_of_the_surface_object_underneath)->physicsPacket()->at(SURFACE_FRICTION) * pow(m_databank->deltatime, 2.0)/2.0;
                if(m_databank->dx < 0)
                    m_databank->dx = 0;
            }
        }
    }
    else{
        m_state = FREE_FALLING;
    }

    m_velocity_for_next_cycle_x = m_databank->dx / m_databank->deltatime;
    m_velocity_for_next_cycle_y = m_databank->dy / m_databank->deltatime;

    m_databank->dx += corrective_x; // correcting for intersecting bounding rectangles
    m_databank->dy += corrective_y;   

    emit internalsChanged();
}

void SurfacePhysics::handleInternalsChange()
{
    if(m_owner != nullptr){
        m_databank->physics_packet.clear();
        m_databank->physics_packet.append(m_mass);
        m_databank->physics_packet.append(m_velocity_x);
        m_databank->physics_packet.append(m_velocity_y);
        m_databank->physics_packet.append(m_friction);
        m_databank->state = m_state;
        ObjectFeature::handleInternalsChange();
    }
}
