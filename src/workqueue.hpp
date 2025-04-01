#ifndef _MANDELBROTWORKQUEUE
#define _MANDELBROTWORKQUEUE

#include <atomic>
#include <mutex>
#include <tuple>

// Distributes work for concurrent execution.
// Doesn't actually store a queue of tasks, just iterates from zero up to
// taskCount and any thread can fetch a task, which is done under mutex lock.
class WorkQueue {
public:
    WorkQueue();
    ~WorkQueue();

    // Set total number of tasks per iteration.
    void setTaskCount(unsigned int count);

    // Set task length.
    void setTaskLength(unsigned int length);

    // Get a tast from the queue.
    // If there are no tasks, returns -1.
    std::tuple<int, unsigned int> getTask();

    // Abort current iteration.
    void abortIteration();
    // Check if current iteration is aborted.
    const std::atomic_bool &isAborted() const;

private:
    unsigned int nextTask;
    unsigned int taskCount;
    unsigned int taskLength;

    std::atomic_bool abort;

    // Just one mutex and lock_guards for thread safety.
    std::mutex accessMutex;
};

#endif
