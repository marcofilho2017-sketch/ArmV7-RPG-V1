#include "item.h"

Item_t empty_item = {
    .type = ITEM_TYPE_NONE,
    .name = "--------",
    .description = "",
    .value = 0,
    .data.effect.attack = 0,
    .data.effect.defense = 0,
    .data.effect.duration = 0,
    .data.effect.life = 0
};