#ifndef MAIN_H
#define MAIN_H

#include <CoreUtils/log.hpp>
#include <QObject>

struct Runtime {static const char *name() {return "virgil";}; enum {use = _ENABLE_LOG};};
struct Debug {static const char *name() {return "virgil_debug";}; enum {use = _ENABLE_DEBUG};};

#endif //MAIN_H