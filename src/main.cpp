#include <Arduino.h>
#include <LoraWAN_ESP32.hpp>
#include <SPI.h> //not automaticly loaded in PlatformIO
#include <logger.hpp>
#include <SafeGPS.hpp>
#include <U8x8_Page.hpp>

//---------------------------------------------------------------------
// Load global config
#include <config.h>
// Prepare Lora - Structs
//  load credentials oota_creds
#include <secrets.h>
// load pinout pinMap
#include <boards.h>
// Pagedefinition
#include <page1.h>
//---------------------------------------------------------------------

LoraWAN_ESP32 lorawan;
Logger logger;
Logger::LEVEL loglevel = Logger::LEVEL::DEBUG;

HardwareSerial serialGPS(2);
SafeGPS gps(serialGPS);

unsigned long lastsec, secs = GPS_UPLOAD_INTERVALL - 10;
uint8_t msg[9];

void setup()
{
    logger.info("Setup");
    logger.setLevel(loglevel);

    logger.debug(String("Setup Lora (NSS:") + pinMap.nss + " RST:" + pinMap.rst + " DI0:" + pinMap.dio[0] + " DI1:" + pinMap.dio[1] + " DI2:" + pinMap.dio[2] + ")");
    logger.debug(String("Setup LoraWAN:"));
    String tmp;
    Logger::hexArray2Str(otaa_creds.deveui, 8u, tmp);
    logger.debug(String("deveui: ") + tmp);
    tmp.clear();
    Logger::hexArray2Str(otaa_creds.appeui, 8u, tmp);
    logger.debug(String("appeui: ") + tmp);
    tmp.clear();
    Logger::hexArray2Str(otaa_creds.appkey, 16u, tmp);
    logger.debug(String("appkey: ") + tmp);

    lorawan.setup(pinMap, otaa_creds);

    logger.info("Lora started");

    logger.info("Setup GPS");
    serialGPS.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
    gps.setSafeZones(safeZones, sizeof(safeZones) / sizeof(SafeGPS::SafeZone));

    logger.info("Setup Display");
    display.begin();
    display.setFont(u8x8_font_amstrad_cpc_extended_r);
    page1.render();

    lorawan.RegisterListener(
        // use a lambda so we're "inside" the cMyLoRaWAN from public/private perspective
        [](void *pClientInfo, uint32_t event) -> void
        {
            auto const pThis = (LoraWAN_ESP32 *)pClientInfo;
            switch (event)
            {
            case EV_JOINING:
                info_LORA.update("JOINING");
                break;
            case EV_JOIN_TXCOMPLETE:
                info_LORA.update("JOIN SENT");
                break;
            case EV_JOINED:
                info_LORA.update("JOINED OK");
                break;
            case EV_JOIN_FAILED:
                info_LORA.update("JOIN FAIL");
                break;

            case EV_TXSTART:
                info_LORA.update("TX START");
                break;
            case EV_TXCOMPLETE:
                info_LORA.update("TX OK");
                break;

            case EV_RXSTART:
                info_LORA.update("RX START");
                break;
            case EV_RXCOMPLETE:
                info_LORA.update("RX OK");
                break;

            default:
                logger.debug(String(event));
            }
        },
        (void *)nullptr // in case we need it.
    );
    lorawan.begin();
}

void loop()
{
    if (lastsec + 1000u < millis())
    {
        lastsec = millis();
        secs++;
        info_GPS.update(gps.getStateString());
        info_Time.update(gps.timeStr());
        info_Pos.update(gps.coord());
        info_Alt.update(gps.alt());
        if (secs % GPS_UPLOAD_INTERVALL == 0)
        {
            logger.info("Update Cycle");
            if (gps.isSafe())
            {
                // if LMIC is busy, just skip
                if (LMIC.opmode & (OP_JOINING))
                    info_LORA.update("BUSY");
                    return;
                gps.formGPSMessage(msg);
                lorawan.SendBuffer(msg, 9);
            }
        }
    }

    lorawan.loop();
    gps.loop();
}