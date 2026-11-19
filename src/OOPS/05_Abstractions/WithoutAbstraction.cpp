#include <iostream>
#include <string>
#include <ctime>

// Without Abstraction
class LogginService
{

    // Class is doing several things
    // Service is coupled to every logging mechnaism

    void log(const std::string &destination, const std::string &message)
    {
        if (destination.find("console") != std::string::npos)
        {
            std::cout << "[LOG]:" << message << std::endl;
        }
        else if (destination.find("file") != std::string::npos)
        {
            // file open , write and close
        }
        else if (destination.find("remote") != std::string::npos)
        {
            // http connection, send payload, handle response
        }
    }
};

// With Abstarction

class Logger
{
private:
    std::string _level;

protected:
    std::string formatMessage(const std::string &message)
    {
        std::time_t now = std::time(nullptr);
        char timestamp[20];
        std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        return "[" + std::string(timestamp) + "] [" + _level + " ] " + message;
    };

public:
    Logger(const std::string &level) : _level{level} {}

    virtual ~Logger() = default;

    virtual void log(const std::string &message) = 0; // pure virtual
};

class ConsoleLogger : public Logger
{
private:
public:
    ConsoleLogger(const std::string &level) : Logger(level) {}

    void log(const std::string &message) override
    {
        std::cout << formatMessage(message) << std::endl;
    }
};

class FileLogger : public Logger
{
private:
public:
    FileLogger(const std::string &level) : Logger(level) {}

    void log(const std::string &message) override
    {
    }
};

void log(Logger &logger, const std::string &message)
{
    logger.log(message);
}

int main()
{
    ConsoleLogger cl("INFO");
    log(cl, "Printing logs");

    return 0;
}