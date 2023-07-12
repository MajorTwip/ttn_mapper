#pragma once
#include <Arduino.h>
#include <SD.h>

class Logger
{
public:
    static Logger &instance()
    {
        static Logger _instance;
        return _instance;
    };
    Logger(HardwareSerial &serial = Serial, unsigned long baudrate = 115200U);
    ~Logger();

    enum LEVEL
    {
        TRACE = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4
    };

    void setPrefixPrecessor(void (*prefixProcessorCallback)(String &toPrint)) { Logger::prefixProcessor = prefixProcessorCallback; };
    bool setLogfile(const char *logfileName = "log.txt", uint8_t ssPin = 18U, uint8_t sckPin = -1, uint8_t misoPin = -1, uint8_t mosiPin = -1);
    void setLevel(const LEVEL level) { Logger::level = level; };
    void error(const String &message) { Logger::formatLog(message, ERROR); };
    void warn(const String &message) { Logger::formatLog(message, WARN); };
    void info(const String &message) { Logger::formatLog(message, INFO); };
    void debug(const String &message) { Logger::formatLog(message, DEBUG); };
    void trace(const String &message) { Logger::formatLog(message, TRACE); };

    void logRaw(const String &message) {Logger::doLog(message);};

    static void hexArray2Str(const uint8_t *arr, uint8_t len, String &ret){
        for(uint8_t i = 0; i<len; i++){
            ret+=String(arr[i],HEX)+" ";
        };
    };

private:
    HardwareSerial &serial = Serial;
    LEVEL level{WARN};
    File logfile;

    void (*prefixProcessor)(String &toPrint) = nullptr;

    const char *levelname[5] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR"};

    void formatLog(const String &message, LEVEL messageLevel);
    void doLog(const String &toPrint);
};
