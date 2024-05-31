#ifndef LABWORK10_YURYS0_TTASK_HPP
#define LABWORK10_YURYS0_TTASK_HPP

#include "TFunction.hpp"

template<typename T>
struct Dummy {
    static T value;
};

template<typename T>
T& declval_helper();

class TTaskBase {
public:
    virtual void execute() = 0;
};

template <typename ReturnType, typename Arg1 = void, typename Arg2 = void>
class TTask;

template <typename T>
class ArgumentWrapper {
public:
    using ReturnType = T;

    explicit ArgumentWrapper(T& value) : value_(value) {}

    T getValue() const {
        return value_;
    }

private:
    T value_;
};

template <typename T>
class ArgumentWrapper<TTask<T>> {
public:
    using ReturnType = T;

    explicit ArgumentWrapper(TTask<ReturnType>& task) : task_(task) {}

    ReturnType getValue() {
        return (task_)();
    }

private:
    TTask<ReturnType> task_;
};

template <typename T, typename Arg>
class ArgumentWrapper<TTask<T, Arg>> {
public:
    using ReturnType = T;

    explicit ArgumentWrapper(TTask<ReturnType, Arg>& task) : task_(task) {}

    ReturnType getValue() {
        return (task_)();
    }

private:
    TTask<ReturnType, Arg> task_;
};

template <typename T, typename Arg1, typename Arg2>
class ArgumentWrapper<TTask<T, Arg1, Arg2>> {
public:
    using ReturnType = T;

    explicit ArgumentWrapper(TTask<ReturnType, Arg1, Arg2>& task) : task_(task) {}

    ReturnType getValue() {
        return (task_)();
    }

private:
    TTask<ReturnType, Arg1, Arg2> task_;
};

template<typename Result>
class TTask<Result> : public TTaskBase {
public:
    template<typename Func>
    explicit TTask(const Func& func) : _f(my_std::function<Result()>(func)) {}

    Result operator()() {
        if (is_completed) {
            return _result;
        }
        _result = (_f)();
        is_completed = true;
        return _result;
    }

    void execute() override {
        operator()();
    }

    ~TTask() = default;

private:
    my_std::function<Result()> _f;
    Result _result;
    bool is_completed = false;
};

template<typename Result, typename Arg1>
class TTask<Result, Arg1> : public TTaskBase {
public:
    template<typename Func>
    explicit TTask(const Func& func, Arg1 arg) : _f(my_std::function<Result(decltype(typename ArgumentWrapper<Arg1>::ReturnType()))>(func)), _arg1(new ArgumentWrapper<Arg1>(arg)) {}

    Result operator()() {
        if (is_completed) {
            return _result;
        }
         _result = (_f)(_arg1->getValue());
        is_completed = true;
        return _result;
    }

    void execute() override {
        operator()();
    }

    ~TTask() = default;

private:
    my_std::function<Result(decltype(typename ArgumentWrapper<Arg1>::ReturnType()))> _f;
    ArgumentWrapper<Arg1>* _arg1;
    Result _result;
    bool is_completed = false;
};

template<typename Result, typename Arg1, typename Arg2>
class TTask : public TTaskBase {
public:
    template<typename Func>
    explicit TTask(const Func& func, Arg1 arg1, Arg2 arg2) : _f(my_std::function<Result(decltype(typename ArgumentWrapper<Arg1>::ReturnType()), decltype(typename ArgumentWrapper<Arg2>::ReturnType()))>(func)), _arg1(new ArgumentWrapper<Arg1>(arg1)), _arg2(new ArgumentWrapper<Arg2>(arg2)) {}

    Result operator()() {
        if (is_completed) {
            return _result;
        }
        _result = (_f)(_arg1->getValue(), _arg2->getValue());
        is_completed = true;
        return _result;
    }

    void execute() override {
        operator()();
    }

    ~TTask() = default;

private:
    my_std::function<Result(decltype(typename ArgumentWrapper<Arg1>::ReturnType()), decltype(typename ArgumentWrapper<Arg2>::ReturnType()))> _f;
    ArgumentWrapper<Arg1>* _arg1;
    ArgumentWrapper<Arg2>* _arg2;
    Result _result;
    bool is_completed = false;
};

#endif //LABWORK10_YURYS0_TTASK_HPP
