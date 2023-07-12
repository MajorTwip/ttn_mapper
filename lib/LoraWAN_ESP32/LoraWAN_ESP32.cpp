#include <LoraWAN_ESP32.hpp>

void LoraWAN_ESP32::setup(lmic_pinmap pinMap, otaa_credentials oc){
        this->Super::begin();
        this->otaa_creds=oc;
        bool res = false;
        Serial.println(res);
};

//$ this method is called when the LMIC needs OTAA info.
// return false to indicate "no provisioning", otherwise
// fill in the data and return true.
bool LoraWAN_ESP32::GetOtaaProvisioningInfo(
    OtaaProvisioningInfo *pInfo
    ) {
    // these are the same constants used in the LMIC compliance test script; eases testing
    // with the RedwoodComm RWC5020B/RWC5020M testers.
    Serial.println("HE" + String(otaa_creds.deveui[0]));
    // initialize info
    memcpy(pInfo->DevEUI, otaa_creds.deveui, sizeof(pInfo->DevEUI));
    memcpy(pInfo->AppEUI, otaa_creds.appeui, sizeof(pInfo->AppEUI));
    memcpy(pInfo->AppKey, otaa_creds.appkey, sizeof(pInfo->AppKey));

    return true;
};