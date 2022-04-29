#include "logging.hpp"
#include "static-defines.h"
#include <map>

std::map<std::string, LoggerContextObject> contextLoggers;
namespace EnhancedPlay
{
    Logger& Logging::getLogger()
    {
        static Logger* logger = new Logger({ID, VERSION}, LoggerOptions(false, true));
        return *logger;
    }
    
    LoggerContextObject& Logging::getContextLogger(const char* func, const char* file, int line)
    {
        std::string contextString(string_format("%s:%i", file, line));
        auto it = contextLoggers.find(contextString);
        if (it != contextLoggers.end())
        {
            return it->second;
        }
        contextLoggers.emplace(contextString, getLogger().WithContext(contextString));
        return contextLoggers.find(contextString)->second;
    }
}