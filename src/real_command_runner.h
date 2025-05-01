// Copyright 2011 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef NINJA_REAL_COMMAND_RUNNER_H_
#define NINJA_REAL_COMMAND_RUNNER_H_

#include "build.h"
#include "subprocess.h"

struct RealCommandRunner : public CommandRunner {
  explicit RealCommandRunner(const BuildConfig& config) : config_(config) {}
  size_t CanRunMore() const override;
  bool StartCommand(Edge* edge) override;
  bool WaitForCommand(Result* result) override;
  std::vector<Edge*> GetActiveEdges() override;
  void Abort() override;
  virtual double LoadAverage() const;
  virtual double AvailableMemory() const;

  const BuildConfig& config_;
  SubprocessSet subprocs_;
  std::map<const Subprocess*, Edge*> subproc_to_edge_;
};

#endif  // NINJA_REAL_COMMAND_RUNNER_H_
