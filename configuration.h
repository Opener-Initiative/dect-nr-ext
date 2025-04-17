#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <map>

/* Type definition of a map with keys and values
as strings */
typedef
std::map<std::string, std::string> StringMap; 

/* Type definition of a map with keys as strings
and values as StringMaps */
typedef
std::map<std::string, StringMap> ConfigMap;

// Configuration class for Simulation
class Configuration
{
    protected:
        // Default constructor for Configuration class
        Configuration();

        // Default destructor for Configuration class
        ~Configuration();
   
        // A ConfigMap that contains the actual configuration values
        ConfigMap Config;

    public:

        void PrintConfigMap();
};

#endif // CONFIGURATION_H