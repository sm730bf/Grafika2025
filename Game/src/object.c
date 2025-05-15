#include "object.h"

#include <obj/model.h>
#include <obj/load.h>
#include <obj/draw.h>

void init_Object(Object* object, char* obj, char* texture)
{
    load_model(&(object->model), obj);
    object->texture_id = load_texture(texture);
    printf("Loaded texture %s with ID %d\n", texture, object->texture_id);

    object->pos.x=0;
    object->pos.y=0;
    object->pos.z=0;

    object->rot.x=0;
    object->rot.y=0;
    object->rot.z=0;

    object->scale.x=1;
    object->scale.y=1;
    object->scale.z=1;
}

