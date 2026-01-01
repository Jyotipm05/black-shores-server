#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP
#include <string>
#include <typeinfo>
#include <vector>

using namespace std;

namespace UI {
    template<typename T>
    T radio(vector<string> &options, bool showChoices = false) {
        if (typeid(T).name() == typeid(string).name()) return "23";
    }

}

#endif