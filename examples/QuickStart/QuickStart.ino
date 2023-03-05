#include <Wire.h>
#include <HUSB238.h>

HUSB238 USB_PD;

void setup()
{
    Serial.begin(9600);
    // Will initialize I2C and query the HUSB for PSU capabilities
    USB_PD.begin();
    
    Serial.println(F("PSU capablities:"));
    Serial.print(F("\t5V:\t")); Serial.print(USB_PD.capability.ma_5V); Serial.println(F("mA"));
    Serial.print(F("\t9V:\t")); Serial.print(USB_PD.capability.ma_9V); Serial.println(F("mA"));
    Serial.print(F("\t12V:\t")); Serial.print(USB_PD.capability.ma_12V); Serial.println(F("mA"));
    Serial.print(F("\t15V:\t")); Serial.print(USB_PD.capability.ma_15V); Serial.println(F("mA"));
    Serial.print(F("\t18V:\t")); Serial.print(USB_PD.capability.ma_18V); Serial.println(F("mA"));
    Serial.print(F("\t20V:\t")); Serial.print(USB_PD.capability.ma_20V); Serial.println(F("mA"));

    // Try and set a supported voltage
    USB_PD.set_voltage(USB_PD.PDO_5V);

    // Check if the set was successful
    USB_PD.update_pd_status();
    
    switch (USB_PD.status.PD_RESPONSE)
    {
      case USB_PD.PD_SUCCESS:
        Serial.println(F("PD contract successfully set to 5V"));
        break;

      case USB_PD.PD_NO_RESPONSE:
        Serial.println(F("Got no response from USB-PD supply"));
        break;
      
      case USB_PD.PD_INVALID:
        // Not really sure how this would happen
        Serial.println(F("Sent an invalid command to the USB-PD supply"));
        break;
      
      case USB_PD.PD_CMD_NOT_SUPPORTED:
        // Not really sure how this would happen
        Serial.println(F("Command not supported by USB-PD supply"));
        break;
      
      case USB_PD.PD_FAIL:
        // Not really sure how this would happen
        Serial.println(F("PD communication failed. Something bad CRC"));
        break;
    }

    // Get the currently configured voltage and current capability
    Serial.print(F("Negotiated voltage: ")); Serial.print(USB_PD.get_pd_voltage()); Serial.println(F("V"));
    Serial.print(F("Negotiated current: ")); Serial.print(USB_PD.get_pd_current()); Serial.println(F("mA"));
}

void loop()
{
}