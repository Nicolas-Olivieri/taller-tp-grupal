#ifndef NAMES_MONITOR_H
#define NAMES_MONITOR_H


#include <mutex>
#include <string>
#include <unordered_set>

class NamesMonitor {
    std::mutex mtx;
    std::unordered_set<std::string> names;

public:
    bool contains(const std::string& name);

    void insert(const std::string& name);

    void erase(const std::string& name);
};


#endif  // NAMES_MONITOR_H
