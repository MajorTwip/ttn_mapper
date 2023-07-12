
//--- Select LoRaWAN Network ---
// The Things Network
#define ARDUINO_LMIC_CFG_NETWORK_TTN 1 //could be defined in platformio.ini

//-----------------------------------------------------------------------------
//
// User Configuration
//

// Enable debug mode (debug messages via serial port)
//#define _DEBUG_MODE_ //could be defined in platformio.ini

// Intervall to upload GPS to TTN-Mapper in seconds
#define GPS_UPLOAD_INTERVALL 60

//-----------------------------------------------------------------------------


// Debug printing
#define DEBUG_PORT Serial
#if defined(_DEBUG_MODE_)
    #define DEBUG_PRINTF(...) { DEBUG_PORT.printf(__VA_ARGS__); }
    #define DEBUG_PRINTF_TS(...) { DEBUG_PORT.printf("%d ms: ", osticks2ms(os_getTime())); \
                                   DEBUG_PORT.printf(__VA_ARGS__); }
#else
  #define DEBUG_PRINTF(...) {}
  #define DEBUG_PRINTF_TS(...) {}
#endif