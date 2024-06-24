#pragma once

#include "SparkEvents/Event.h"

#include "SparkEvents/ApplicationEvent.h"
#include "SparkEvents/KeyEvent.h"
#include "SparkEvents/MouseEvent.h"

namespace SpriteSpark {

    class GlobalEventDispatcher {

    public:

        inline static SpriteSpark::EventDispatcher& Get() {
            static SpriteSpark::EventDispatcher dispatcher;
            return dispatcher;
        }

    };

}