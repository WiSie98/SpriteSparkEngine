#pragma once

#include "SparkCore/HeaderFiles/ThreadPool.h"

namespace SpriteSpark {

    class GlobalThreadPool {

    public:

        inline static ThreadPool& Get() {
            static ThreadPool threadPool;
            return threadPool;
        }

    };

}