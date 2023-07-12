#pragma once
#include <Arduino.h>
#include <TinyGPSPlus.h>

class SafeGPS : public TinyGPSPlus
{
public:
    struct SafeZone
    {
        double lat;
        double lon;
        uint16_t dist;
    };

    enum GPS_STATE
    {
        GPS_RX_ERROR,
        GPS_NO_FIX_NO_TIME,
        GPS_NO_FIX,
        GPS_FIX,
        GPS_FIX_UNSAFE
    };

    SafeGPS(HardwareSerial &ser) : TinyGPSPlus(), serial(ser){};

    void loop()
    {
        if (serial.available())
        {
            encode(serial.read());
        };
    };

    void setSafeZones(struct SafeZone * sz, uint8_t len)
    {
        this->safeZones = sz;
        this->len = len;
    };

    bool isSafe()
    {
        // return false if no gps fix
        if (!this->location.isValid())
            return false;
        // return true if no zones are defined
        if (this->safeZones == nullptr)
            return true;
        for (uint8_t i = 0; i < len; i++)
        {
            if (distanceBetween(safeZones[i].lat, safeZones[i].lon, this->location.lat(), this->location.lng()) < safeZones[i].dist)
            {
                return false;
            }
        }
        return true;
    };

    GPS_STATE getState()
    {
        if (&serial == nullptr)
            return GPS_RX_ERROR;
        if (!location.isValid() && !time.isValid())
            return GPS_NO_FIX_NO_TIME;
        if (!location.isValid() || location.age()>2000u)
            return GPS_NO_FIX;
        if (isSafe())
        {
            return GPS_FIX;
        }
        else
        {
            return GPS_FIX_UNSAFE;
        }
    };

    String getStateString()
    {
        auto state = getState();
        switch (state)
        {
        case GPS_FIX:
            return "GPS_FIX";
        case GPS_FIX_UNSAFE:
            return "GPS_UNSAFE";
        case GPS_NO_FIX:
            return "GPS_NO_FIX";
        case GPS_NO_FIX_NO_TIME:
            return "GPS_NO_TIME";
        case GPS_RX_ERROR:
            return "GPS_RX_ERROR";

        default:
            return "N/A";
        }
    }

    String coord()
    {
        auto state = getState();
        if (state == GPS_NO_FIX || state == GPS_NO_FIX_NO_TIME || state == GPS_RX_ERROR)
            return "N/A";
        return String(location.lat()) + "/" + location.lng();
    };

    String alt()
    {
        auto state = getState();
        if (state == GPS_NO_FIX || state == GPS_NO_FIX_NO_TIME || state == GPS_RX_ERROR)
            return "N/A";
        return String(altitude.meters()) + "m";
    };

    String timeStr()
    {
        String timeStr = "";
        if (date.isValid())
        {
            timeStr += (date.day());
            timeStr += (F("/"));
            timeStr += (date.month());
            timeStr += (F("/"));
            timeStr += String(date.year()).substring(2);
        }
        else
        {
            timeStr += (F("N/A"));
        }

        if (time.isValid())
        {
            timeStr += "-";
            if (time.hour() < 10)
                timeStr += (F("0"));
            timeStr += (time.hour());
            // time += (F(":"));
            if (time.minute() < 10)
                timeStr += (F("0"));
            timeStr += (time.minute());
            timeStr += (F(":"));
            if (time.second() < 10)
                timeStr += (F("0"));
            timeStr += (time.second());
        }
        else
        {
            timeStr += (F(" N/A"));
        }
        return timeStr;
    }

    void formGPSMessage(uint8_t *payloadBuffer)
    {
        uint32_t LatitudeBinary, LongitudeBinary;
        uint16_t altitudeGps;
        uint8_t hdopGps;

        LatitudeBinary = ((location.lat() + 90) / 180.0) * 16777215;
        LongitudeBinary = ((location.lng() + 180) / 360.0) * 16777215;

        payloadBuffer[0] = (LatitudeBinary >> 16) & 0xFF;
        payloadBuffer[1] = (LatitudeBinary >> 8) & 0xFF;
        payloadBuffer[2] = LatitudeBinary & 0xFF;

        payloadBuffer[3] = (LongitudeBinary >> 16) & 0xFF;
        payloadBuffer[4] = (LongitudeBinary >> 8) & 0xFF;
        payloadBuffer[5] = LongitudeBinary & 0xFF;

        altitudeGps = altitude.meters();
        payloadBuffer[6] = (altitudeGps >> 8) & 0xFF;
        payloadBuffer[7] = altitudeGps & 0xFF;

        hdopGps = hdop.value() / 10;
        payloadBuffer[8] = hdopGps & 0xFF;
    }

private:
    HardwareSerial &serial;
    SafeZone * safeZones;
    uint8_t len = 0;
};
