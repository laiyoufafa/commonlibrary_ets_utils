/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef JS_CONCURRENT_MODULE_TASKPOOL_WORKER_H_
#define JS_CONCURRENT_MODULE_TASKPOOL_WORKER_H_

#include <list>
#include <memory>

#include "helper/error_helper.h"
#include "helper/napi_helper.h"
#include "helper/object_helper.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "native_engine/native_engine.h"
#include "task.h"
#include "task_runner.h"
#include "utils/log.h"

namespace Commonlibrary::ConcurrentModule {
class Worker {
public:
    ~Worker();

    static Worker* WorkerConstructor(napi_env env);

    void NotifyExecuteTask();

private:
    explicit Worker(napi_env env);

    uv_loop_t* GetHostLoop() const
    {
        if (hostEnv_ != nullptr) {
            return Helper::NapiHelper::GetLibUV(hostEnv_);
        }
        return nullptr;
    }

    uv_loop_t* GetWorkerLoop() const
    {
        if (workerEnv_ != nullptr) {
            return Helper::NapiHelper::GetLibUV(workerEnv_);
        }
        return nullptr;
    }

    void RunLoop() const
    {
        uv_loop_t* loop = GetWorkerLoop();
        if (loop != nullptr) {
            uv_run(loop, UV_RUN_DEFAULT);
        } else {
            HILOG_ERROR("taskpool:: Worker loop is nullptr when start worker loop");
            return;
        }
    }

    void StartExecuteInThread();
    static void ExecuteInThread(const void* data);
    bool PrepareForWorkerInstance();
    void ReleaseWorkerThreadContent();
    static void PerformTask(const uv_async_t* req);
    static void TaskResultCallback(NativeEngine* engine, NativeValue* value, NativeValue* data);
    static void HandleTaskResult(const uv_async_t* req);

    napi_env hostEnv_ {nullptr};
    napi_env workerEnv_ {nullptr};
    TaskInfo *taskInfo_ {nullptr};
    uv_async_t performTaskSignal_ {};
    uv_async_t notifyResultSignal_ {};
    std::unique_ptr<TaskRunner> runner_ {};
    std::recursive_mutex liveEnvLock_ {};
};
} // namespace Commonlibrary::ConcurrentModule
#endif // JS_CONCURRENT_MODULE_TASKPOOL_WORKER_H_