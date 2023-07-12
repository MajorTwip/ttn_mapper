#include <logger.hpp>

Logger::Logger(HardwareSerial &serial, unsigned long baudrate)
{
    // Initialize Serial for Output
    Logger::serial = serial;
    serial.begin(baudrate);
}

Logger::~Logger()
{
    serial.end();
    if (logfile)
    {
        logfile.close();
        SD.end();
    }
}

bool Logger::setLogfile(const char *logfileName, uint8_t ssPin, uint8_t sckPin, uint8_t misoPin, uint8_t mosiPin)
{
    bool sdStartSuccess = false;
    if (sckPin != -1 && misoPin != -1 && mosiPin != -1) // if SPI-Pins explicitely set define new SPI-Port
    {

        //SPIClass spi2 = SPIClass();
        SPIClass * spi2 = new SPIClass(HSPI);
        (*spi2).begin(sckPin, misoPin, mosiPin,ssPin);
        sdStartSuccess = SD.begin(ssPin, *spi2);
    }
    else
    {
        sdStartSuccess = SD.begin(ssPin);
    }

    if (sdStartSuccess)
    {
        logfile = SD.open(logfileName, FILE_APPEND);
        if (logfile)
        {
            return true;
        }
    }
    return false;
}

void Logger::formatLog(const String &message, Logger::LEVEL messageLevel)
{
    if (Logger::level <= messageLevel)
    {
        String toPrint;
        if (prefixProcessor != nullptr)
            prefixProcessor(toPrint);
        toPrint += levelname[messageLevel];
        toPrint += ": ";
        toPrint += message;
        doLog(toPrint);
    }
}

void Logger::doLog(const String &toPrint)
{
    serial.println(toPrint);
    if (logfile)
        logfile.println(toPrint);
        logfile.flush();
}