#include "instProf.h"
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <asm/unistd.h>
#include <stdio.h>
#include "instTracker.h"

static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                            int cpu, int group_fd, unsigned long flags)
{
    int ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
    return ret;
}

static int fd;
static long long base_count; // Number of instructions for an empty profile
static long long count; // Number of instructions since last mark_change() call
static struct perf_event_attr pe;

void start_count() {
    memset(&pe, 0, sizeof(struct perf_event_attr));
    pe.type = PERF_TYPE_HARDWARE;
    pe.size = sizeof(struct perf_event_attr);
    pe.config = PERF_COUNT_HW_INSTRUCTIONS;
    pe.disabled = 1;
    pe.exclude_kernel = 1;
    pe.exclude_hv = 1;

    fd = perf_event_open(&pe, 0, -1, -1, 0);
    if (fd == -1) {
        fprintf(stderr, "Error opening leader %llx\n", pe.config);
        exit(EXIT_FAILURE);
    }

    ioctl(fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);
}

void stop_count() {
    ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
    read(fd, &count, sizeof(long long));

    close(fd);
}

void base_check() { // Checks the number of instructions for an empty profile
    start_count();
    stop_count();

    // Set Base Count
    base_count = count;

    // Create node
    runtimeInstNode* node = createRuntimeInstNode(BASE_COUNT, -1, base_count);
    addRuntimeInstNode(node);

    // Restart count
    start_count();
}

void logInstScopeEnter(int id) {
    stop_count();
    // Create node
    runtimeInstNode* node = createRuntimeInstNode(SCOPE_ENTER, id, count - base_count);
    addRuntimeInstNode(node);
    // Resume counting
    start_count();
}

void logInstScopeExit(int id) {
    stop_count();
    // Create node
    runtimeInstNode* node = createRuntimeInstNode(SCOPE_EXIT, id, count - base_count);
    addRuntimeInstNode(node);
    // Resume counting
    start_count();
}

void logInstReturn() {
    stop_count();
    // Create node
    runtimeInstNode* node = createRuntimeInstNode(RETURN, -1, count - base_count);
    addRuntimeInstNode(node);
    // Resume counting
    start_count();
}

void instAtExitFunc() {
    // Add to runtime list
    stop_count();

    // Export runtime list
    exportRuntimeInstList();

    // Print runtimeList
    printf("\nRuntime List:\n");
    printRuntimeInstList();

    // Delete runtime list
    deleteRuntimeInstList();
}
