#include "real_command_runner.h"

#include "gtest/gtest.h"

struct MockSystemInfo {
  int num_subprocs = 10;
  double available_memory = 0.0f;
  double load_average = 0.0f;
};

struct MockRealCommandRunner : public RealCommandRunner {
  MockRealCommandRunner(const BuildConfig& config, const MockSystemInfo info)
      : RealCommandRunner(config), load_average_(info.load_average),
        num_subprocs_(info.num_subprocs),
        available_memory_(info.available_memory) {
    for (size_t i = 0; i < info.num_subprocs; ++i) {
      subprocs_.Add("", "");
    }
  }
  double AvailableMemory() const override { return available_memory_; }
  double LoadAverage() const override { return load_average_; }

  int num_subprocs_;
  double load_average_;
  double available_memory_;
};

struct CanRunMoreTest : public testing::Test {
  // const int num_subprocs = 10;
  void CheckCanRunMore(const BuildConfig config, const MockSystemInfo& info,
                       int capacity) {
    MockRealCommandRunner runner(config, info);
    ASSERT_EQ(runner.subprocs_.running_.size(), runner.num_subprocs_);
    ASSERT_EQ(runner.subprocs_.finished_.size(), 0);
    EXPECT_EQ(capacity, runner.CanRunMore());
  }
};

TEST_F(CanRunMoreTest, ParallelismGreaterThanSubprocNumber) {
  BuildConfig config;
  MockSystemInfo info;
  config.parallelism = 10;
  info.num_subprocs = 4;
  CheckCanRunMore(config, info, 6);
}

TEST_F(CanRunMoreTest, ParallelismEqualSubprocNumber) {
  BuildConfig config;
  MockSystemInfo info;
  config.parallelism = 4;
  info.num_subprocs = 4;
  CheckCanRunMore(config, info, 0);
}

TEST_F(CanRunMoreTest, ParallelismLessThanSubprocNumber) {
  BuildConfig config;
  MockSystemInfo info;
  config.parallelism = 2;
  info.num_subprocs = 4;
  CheckCanRunMore(config, info, 0);
}

TEST_F(CanRunMoreTest, LoadCapacityLessThanCapacity) {
  BuildConfig config;
  MockSystemInfo info;
  config.parallelism = 10;
  config.max_load_average = 2.0f;
  info.num_subprocs = 4;
  CheckCanRunMore(config, info, 2);
}

TEST_F(CanRunMoreTest, NotEnoughAvailableMemory) {
  BuildConfig config;
  MockSystemInfo info;
  config.parallelism = 10;
  config.min_available_memory = 2.0f;
  info.num_subprocs = 4;
  info.available_memory = 1.0f;
  CheckCanRunMore(config, info, 0);
}