#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

struct AppConfig {
    unsigned int width = 800;
    unsigned int height = 600;
    std::string title = "Hello World!";
    bool fullScreenEnabled = false;
    bool vsyncEnabled = true;
};

#endif
