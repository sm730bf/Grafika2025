#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"
#include "object.h"
#include <obj/model.h>

typedef struct Scene
{
    //Model cube;
    Object tank;
    Object tank2;
    Object projectile_model;
    Projectile projectiles[10];
    int tank_hp;
    int tank2_hp;
    Object ground;
    Material material;
    GLuint texture_id;
    float tank_speed;      // Speed of the tank
    float tank_rotation;   // Rotation angle of the tank
} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting();

/**
 * Set the current material.
 */
void set_material(const Material* material);

/**
 * Update the scene.
 */
void update_scene(Scene* scene);

/**
 * Render the scene objects.
 */
void render_scene(const Scene* scene);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin();

void draw_string(const Scene* scene, const char* s);

void render_obj();

void destroy_scene();

void keeper_movement(Object* keeper);

void render_menu_texture(GLuint texture);

void draw_score(const Scene* scene);
#endif /* SCENE_H */
