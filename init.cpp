/*
 * Copyright (c) 2024 OneTeste250376
 *
 * License: BSD-2, GPLv3
 */
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <cstdlib>

#define RESET   "\033[0m"
#define RED     "\033[31m"      
#define GREEN   "\033[32m"      
#define YELLOW  "\033[33m"      
#define BLUE    "\033[34m"      
#define MAGENTA "\033[35m"      
#define CYAN    "\033[36m"      
#define WHITE   "\033[37m"

static void
spawn(char *const argv[])
{
    switch (fork()) {
    case 0:
        sigprocmask(SIG_UNBLOCK, &set, NULL);
        setsid();
        execvp(argv[0], argv);
        perror("execvp");
        _exit(1);
    case -1:
        perror("fork");
    }
}

int main() {
    std::string rc_service_path = "/etc/rc/";
    std::string rc_service_conf = "/etc/rc/init.conf";
    static char *const rcinitcmd[] = { "/bin/rc.init", NULL };
    std::string version = "0.1.1a1";

    if (getpid() != 1) {
        return 1;
    }

    std::cout << YELLOW << "Welcome to Minit " << CYAN << version << RESET << std::endl;

    // Reading config file
    std::ifstream file(rc_service_conf);
    if (!file.is_open()) {
        std::cerr << "Error: config not found in " << rc_service_conf << std::endl;
        return 1;
    }

    std::string service_name, action;
    while (file >> service_name >> action) {
        if (action == "run") {
            std::cout << "Service '" << service_name << "' should be run." << std::endl;
            std::string full_path = rc_service_path + service_name + "/" + service_name + ".sh";
            char *const service_cmd[] = { const_cast<char*>(full_path.c_str()), nullptr };
            spawn(service_cmd); 
        } else if (action == "norun") {
            std::cout << "Service '" << service_name << "' should not be run." << std::endl;
        } else {
            std::cerr << "Error: Invalid action '" << action << "' for service '" << service_name << "'" << std::endl;
        }
    }
    
    // Running rc.init script
    spawn(rcinitcmd);
}
