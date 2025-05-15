#ifndef TARGY_H
#define TARGY_H

#include "utils.h"
#include "texture.h"
#include <obj/model.h>


typedef struct Object
{
    Model model;
    Material material;
    GLuint texture_id; // Single texture ID
    vec3 pos;
    vec3 rot;
    vec3 scale;
} Object;

typedef struct Projectile {
    vec3 pos;       // Position of the projectile
    vec3 dir;       // Direction of the projectile
    float speed;    // Speed of the projectile
    int active;     // Whether the projectile is active
} Projectile;

void init_Object(Object* Object, char* obj, char* texture);

#endif /* TARGY_H */