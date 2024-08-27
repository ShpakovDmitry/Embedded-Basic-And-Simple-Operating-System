/* -*-c++-*-
 * @file: Task.hpp
 * @author: Dmitry Shpakov
 * @date: 2024/08/27
 * @licence: LGPL3 (see text at the end of a file)
 * @description: Task class definition
 */

#pragma once

#include <cstdint>
#include <functional>
#include <list>
#include <string>

class Task {
public:
  using TaskFunction = std::function<int(std::list<std::string>&)>;
  using Hook = std::function<void(void)>;
  enum class State {
    READY,
    RUNNING,
    BLOCKED,
    SUSPENDED,
    TERMINATED
  };

  Task(const std::string taskName, TaskFunction taskFunction,
            std::list<std::string> parameters, uint32_t stackSize, uint8_t priority);
  ~Task();

  // Task management methods
  void Start(void);
  void Terminate(void);
  void Suspend(void);
  void Resume(void);

  // Context management
  void SaveContext(void);
  void LoadContext(void);

  // Stack and state management
  bool CheckStackOverflow(void) const;
  State GetState(void) const;
  void SetState(State newState);

  // Getters abd setters
  uint32_t GetPID(void) const;
  std::string getName(void) const;
  void SetName(const std::string &taskName);
  uint8_t GetPriority(void) const;
  void SetPriority(uint8_t taskPriority);
  uint32_t GetExecutionTime(void) const;
  void IncrementExecutionTime(uint32_t time);
  void SetExitCode(int32_t code);
  int32_t GetExitCode(void) const;

  // Event handling
  void SetEventFlag(uint32_t flag);
  void ClearEventFlag(uint32_t flag);
  bool IsEventFlagSet(uint32_t flag) const;

  // Hooks for task events
  void SetOnStartHook(Hook hook);
  void SetOnSuspendHook(Hook hook);
  void SetOnResumeHook(Hook hook);
  void SetOnTerminateHook(Hook hook);

private:
  uint32_t pid;                       // process ID
  std::string name;                   // task name
  TaskFunction entryPoint;            // task entry function
  std::list<std::string> parameters;  // parameters for the task function
  uint32_t *stack;                    // pointer to dynamically allocated stack
  uint32_t *stackPointer;             // pointer to the top of the tasks stack
  uint32_t stackSize;                 // size of the tasks stack
  uint8_t priority;                   // task priority 0 - highest 255 - lowest
  State state;                        // current state of the task
  uint32_t executionTime;             // accumulated execution time
  uint32_t wakeUpTime;                // time when the task should wake up
  uint32_t eventFlags;                // event flags for inter-task commnication
  int32_t exitCode;                   // exit code for the task
  Hook onStartHook;                   // hook for start event
  Hook onSuspendHook;                 // hook for suspend event
  Hook onResumeHook;                  // hook for resume event
  Hook onTerminateHook;               // hook for terminate event

  static uint32_t nextPID;            // static member to generate unique PIDs
};

/*

Copyright © 2024 Dmitry Shpakov

This library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, see <http://www.gnu.org/licenses/>.

*/

