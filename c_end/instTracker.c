//
// Created by congyu on 5/13/23.
//

#include "instTracker.h"

// Runtime logging functions
runtimeInstList instList = {NULL, NULL, 0};

void addRuntimeInstNode(runtimeInstNode *node) {
    if (instList.head == NULL) {
        instList.head = node;
        instList.tail = node;
    } else {
        // Add node to the end of the list
        instList.tail->next = node;
        instList.tail = node;
    }
    // Increment size
    instList.size++;
}

runtimeInstNode* createRuntimeInstNode(enum runtimeInstActionType type, int id, long long int instCount) {
    runtimeInstNode *node = (runtimeInstNode *) malloc(sizeof(runtimeInstNode));
    node->id = id;
    node->type = type;
    node->instCount = instCount;
    if (clock_gettime(CLOCK_REALTIME, &node->time) != 0) {
        fprintf(stderr, "Error getting time\n");
        exit(1);
    }
    node->next = NULL;
    return node;
}

void deleteRuntimeInstList() {
    runtimeInstNode *curr = instList.head;
    runtimeInstNode *next = NULL;
    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }
    instList.head = NULL;
    instList.tail = NULL;
    instList.size = 0;
}

void printRuntimeInstList() {
    runtimeInstNode *curr = instList.head;
    if (curr == NULL) {
        printf("List is empty\n");
        return;
    }
    int count = 0;
    while (curr != NULL) {
        printf("#%d ", count);
        if (curr->type == BASE_COUNT) {
            printf("BASE_COUNT [Instruction count: %lld, Time: %ld.%09ld]\n", curr->instCount, curr->time.tv_sec, curr->time.tv_nsec);
        } else {
            switch (curr->type) {
                case SCOPE_ENTER:
                    printf("SCOPE_ENTER");
                    break;
                case SCOPE_EXIT:
                    printf("SCOPE_EXIT");
                    break;
                case RETURN:
                    printf("RETURN");
                    break;
            }
            printf(" [id: %d, Instruction count: %lld, Time: %ld.%09ld]\n",
                   curr->id, curr->instCount, curr->time.tv_sec, curr->time.tv_nsec);
        }
        curr = curr->next;
        count++;
    }
}

void exportRuntimeInstList() {
    const char *filename = "runtimeInst.cymp";
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(1);
    }
    runtimeInstNode *curr = instList.head;
    while (curr != NULL) {
        if (curr->type == BASE_COUNT) {
            fprintf(fp, "BCNT, %lld, %ld.%09ld\n", curr->instCount, curr->time.tv_sec, curr->time.tv_nsec);
        } else {
            switch (curr->type) {
                case SCOPE_ENTER:
                    fprintf(fp, "SENT");
                    break;
                case SCOPE_EXIT:
                    fprintf(fp, "SEXT");
                    break;
                case RETURN:
                    fprintf(fp, "RETN");
                    break;
            }
            fprintf(fp, ", %d, %lld, %ld.%09ld\n",
                   curr->id, curr->instCount, curr->time.tv_sec, curr->time.tv_nsec);
        }
        curr = curr->next;
    }
    fclose(fp);
}

