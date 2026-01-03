#ifndef QUEST_H
#define QUEST_H

#include "dialog_handler.h"

typedef enum {
    CONDITION_NONE,
    CONDITION_GET_ITEM,
    CONDITION_GIVE_ITEM,
    CONDITION_KILL_ENEMIES,
    CONDITION_KILL_ENEMY,
    CONDITION_TALK_TO_NPC,
    CONDITION_GET_TO_LOCATION
} QuestCondition_t;

typedef enum {
    QUEST_STATUS_INACTIVE,
    QUEST_STATUS_ACTIVE,
    QUEST_STATUS_COMPLETE
} QuestStatus_t;

typedef struct {
    const char* name;
    const char* description;
    QuestStatus_t status;
    QuestCondition_t condition;
    unsigned int condition_param[2];
    Dialog_t* dialog_start;
    Dialog_t* dialog_end;
} Quest_t;

#endif