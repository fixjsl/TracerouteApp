#pragma once
#include <functional>
#include <queue>
#include "qobject.h"

#include <qtimer.h>
class TaskQueue : public QObject {
    Q_OBJECT

private:
    std::queue<std::function<void()>> taskQueue;
    bool isProcessing = false;

public:
    void addTask(std::function<void()> task) {
        taskQueue.push(task);
        if (!isProcessing) {
            processNextTask();
        }
    }

private:
    void processNextTask() {
        if (taskQueue.empty()) {
            isProcessing = false;
            return;
        }

        isProcessing = true;
        auto task = taskQueue.front();
        taskQueue.pop();

        // 현재 작업 실행
        task();

        // 다음 작업 실행 예약
        QTimer::singleShot(0, this, &TaskQueue::processNextTask);
    }
};

