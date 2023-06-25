//
// Created by congyu on 5/13/23.
//

#ifndef TEST_INSTTRACKER_H
#define TEST_INSTTRACKER_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

enum runtimeInstActionType {
    SCOPE_ENTER, // Scope enter
    SCOPE_EXIT,  // Scope exit
    RETURN,      // Return
    BASE_COUNT // Base Instruction Count
};

// LL for tracking runtime instructions
typedef struct runtimeInstNode {
    int id;
    enum runtimeInstActionType type;
    long long int instCount;
    struct timespec time;
    struct runtimeInstNode *next;
} runtimeInstNode;

typedef struct runtimeInstList {
    runtimeInstNode *head;
    runtimeInstNode *tail;
    int size;
} runtimeInstList;

runtimeInstNode* createRuntimeInstNode(enum runtimeInstActionType type, int id, long long int instCount);

void addRuntimeInstNode(runtimeInstNode *node);

void deleteRuntimeInstList();

void printRuntimeInstList();

void exportRuntimeInstList();

#endif //TEST_INSTTRACKER_H
