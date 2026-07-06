#ifndef INC_IoT_HPP
#define INC_IoT_HPP

#if defined(ARDUINO) && defined(ESP32)
#if defined(ERA_USE_SSL)
// #include <ERaSimpleMBEsp32SSL.hpp>
#else
#include <ESP32_simple.hpp>
#endif

#endif
#endif /*INC_IoT_HPP*/