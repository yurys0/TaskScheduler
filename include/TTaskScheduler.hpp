#ifndef LABWORK10_YURYS0_TTASKSCHEDULER_HPP
#define LABWORK10_YURYS0_TTASKSCHEDULER_HPP

#include <vector>
#include <any>
#include "TTask.hpp"

class TTaskScheduler {
public:
    template <typename FuncObj>
    auto add(FuncObj funcObj) {
        using ReturnType = decltype(declval_helper<FuncObj>()());
        auto* new_task = new TTask<ReturnType>(funcObj);
        tasks_.push_back(new_task);
        return *new_task;
    }

    template <typename FuncObj, typename Arg1>
    auto add(FuncObj funcObj, Arg1 arg1) {
        using ReturnType = decltype(declval_helper<FuncObj>()(Dummy<typename ArgumentWrapper<Arg1>::ReturnType>::value));
        auto* new_task = new TTask<ReturnType, Arg1>(funcObj, arg1);
        tasks_.push_back(new_task);
        return *new_task;
    }

    template <typename FuncObj, typename Arg1, typename Arg2>
    auto add(FuncObj funcObj, Arg1 arg1, Arg2 arg2) {
        using ReturnType = decltype(declval_helper<FuncObj>()(Dummy<typename ArgumentWrapper<Arg1>::ReturnType>::value, Dummy<typename ArgumentWrapper<Arg2>::ReturnType>::value));
        auto* new_task = new TTask<ReturnType, Arg1, Arg2>(funcObj, arg1, arg2);
        tasks_.push_back(new_task);
        return *new_task;
    }

    template <typename T, typename Arg1, typename Arg2>
    auto getFutureResult(TTask<T, Arg1, Arg2>& task) {
        return task;
    }

    template <typename T, typename Arg1, typename Arg2>
    T getResult(TTask<T, Arg1, Arg2>& task) {
        return static_cast<T>(task());
    }

    void executeAll() {
        for (auto& task : tasks_) {
            task->execute();
        }
    }

    ~TTaskScheduler() {
        for (auto& task : tasks_) {
            delete task;
        }
    }

private:
    std::vector<TTaskBase*> tasks_;
};

#endif //LABWORK10_YURYS0_TTASKSCHEDULER_HPP
