/****************************************************************************\
|
|	The LoRaWAN object
|
\****************************************************************************/
#define ARDUINO_LMIC_CFG_NETWORK_TTN 1

#pragma once

#include <Arduino_LoRaWAN_network.h>
#include <Arduino_LoRaWAN_EventLog.h>
#include <arduino_lmic.h>

//Not automaticly loaded in PlatformIO
#include <SPI.h>



class LoraWAN_ESP32 : public Arduino_LoRaWAN_network {


public:
    struct otaa_credentials {
        // deveui, little-endian
        std::uint8_t deveui[8];
        // appeui, little-endian
        std::uint8_t appeui[8];
        // appkey: just a string of bytes, sometimes referred to as "big endian".
        std::uint8_t appkey[16];
    };

    LoraWAN_ESP32() {};
    using Super = Arduino_LoRaWAN_network;
    void setup(lmic_pinmap, otaa_credentials);

    // if you have persistent storage, you can provide implementations for these:
    virtual void NetSaveSessionInfo(const SessionInfo &Info, const uint8_t *pExtraInfo, size_t nExtraInfo) override{};
    virtual void NetSaveSessionState(const SessionState &State) override {};
    virtual bool NetGetSessionState(SessionState &State) override {return false;};


protected:
    // you'll need to provide implementation for this.
    virtual bool GetOtaaProvisioningInfo(Arduino_LoRaWAN::OtaaProvisioningInfo*) override;
    otaa_credentials otaa_creds;
};