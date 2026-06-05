#include "names_monitor.h"

bool NamesMonitor::contains(const std::string& name) {
    std::unique_lock lock(mtx);
    return names.contains(name);
}

void NamesMonitor::insert(const std::string& name) {
    std::unique_lock lock(mtx);
    names.insert(name);
}

void NamesMonitor::erase(const std::string& name) {
    std::unique_lock lock(mtx);
    if (names.contains(name))
        names.erase(name);
}
