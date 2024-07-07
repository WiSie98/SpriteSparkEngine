#pragma once

#include "SparkCore/HeaderFiles/ThreadPool.h"

namespace SpriteSpark {

    class GlobalThreadPool {

    public:

        inline static SpriteSpark::ThreadPool& Get() {
            static SpriteSpark::ThreadPool threadPool;
            return threadPool;
        }

    };

}