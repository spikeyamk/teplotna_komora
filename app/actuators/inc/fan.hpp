/**
 * @file fan.hpp
 * @brief This file contains the declarations for the fan actuator control functions.
 */

 /**
    * @namespace actuators::fan
    * @brief Namespace for fan actuator control functions.
*/

#ifndef FAN_HPP
#define FAN_HPP

namespace actuators {
    namespace fan {
        void startMoving();
        void stopMoving();
    }
}

#endif