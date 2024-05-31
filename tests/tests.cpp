#include "gtest/gtest.h"
#include "TTaskScheduler.hpp"

TEST(TTaskTest, TaskWithArgumentsTest) {
    auto func = [](int a, int b) { return a + b; };

    TTaskScheduler scheduler;
    auto task = scheduler.add(func, 10, 20);

    EXPECT_EQ(scheduler.getResult(task), 30);
}

TEST(TTaskSchedulerTest, DifferentReturnTypesTest) {
    TTaskScheduler scheduler;

    auto task1 = scheduler.add([]() { return 10; });
    auto task2 = scheduler.add([]() { return std::string("hello"); });
    auto task3 = scheduler.add([]() { return std::vector<int>{1, 2, 3}; });

    EXPECT_EQ(scheduler.getResult(task1), 10);
    EXPECT_EQ(scheduler.getResult(task2), "hello");
    EXPECT_EQ(scheduler.getResult(task3), (std::vector<int>{1, 2, 3}));
}

TEST(TTaskSchedulerTest, TaskDependenciesTest) {
    TTaskScheduler scheduler;

    auto task1 = scheduler.add([]() { return 10; });
    auto task2 = scheduler.add([](int x) { return x * 2; }, scheduler.getFutureResult(task1));
    auto task3 = scheduler.add([](int x) { return x * 3 + 5; }, scheduler.getFutureResult(task2));

    EXPECT_EQ(scheduler.getResult(task3), 65);
}

TEST(TTaskSchedulerTest, TaskDependenciesTest2) {
    TTaskScheduler scheduler;

    auto self = [](int x) { return x; };

    auto task1 = scheduler.add(self, 78);
    auto task2 = scheduler.add(self, scheduler.getFutureResult(task1));
    auto task3 = scheduler.add(self, scheduler.getFutureResult(task2));
    auto task4 = scheduler.add(self, scheduler.getFutureResult(task3));
    auto task5 = scheduler.add(self, scheduler.getFutureResult(task4));
    auto task6 = scheduler.add(self, scheduler.getFutureResult(task5));

    EXPECT_EQ(scheduler.getResult(task6), 78);
}

TEST(TTaskSchedulerTest, MultipleTasksTest) {
    TTaskScheduler scheduler;

    auto task1 = scheduler.add([]() { return 30; });
    auto task2 = scheduler.add([]() { return 20; });
    auto task3 = scheduler.add([](int x, int y) { return x - y; }, scheduler.getFutureResult(task1), scheduler.getFutureResult(task2));

    scheduler.executeAll();

    EXPECT_EQ(scheduler.getResult(task3), 10);
}

TEST(TTaskSchedulerTest, VoidReturnTypeTest) {
    TTaskScheduler scheduler;

    bool flag = false;
    auto task = scheduler.add([&flag]() { return flag = true; });

    scheduler.executeAll();

    EXPECT_EQ(scheduler.getResult(task), true);
}