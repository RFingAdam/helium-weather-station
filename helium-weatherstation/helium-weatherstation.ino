#include <LoRaWan_APP.h>
#include <Arduino.h>
#include <Seeed_BME280.h>
#include <Wire.h>

bool ENABLE_SERIAL = false; // enable serial debug output here if required
uint32_t appTxDutyCycle = 300000; // the frequency of readings, in milliseconds(default set 300s)
/* OTAA para*/

//**Important** FIll IN DEVEUI, APPEUI, AND APPKEY BELOW FOR EACH DEVICE BEFORE PROGRAMMING
uint8_t devEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/* ABP para*/
uint8_t nwkSKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appSKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint32_t devAddr =  ( uint32_t )0x00000000;

/*LoraWan channelsmask, default channels 0-7*/
uint16_t userChannelsMask[6] = { 0xFF00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t  loraWanClass = LORAWAN_CLASS;

/*the application data transmission duty cycle.  value in [ms].*/
//uint32_t appTxDutyCycle;

/*OTAA or ABP*/
bool overTheAirActivation = LORAWAN_NETMODE;

/*ADR enable*/
bool loraWanAdr = LORAWAN_ADR;

/* set LORAWAN_Net_Reserve ON, the node could save the network info to flash, when node reset not need to join again */
bool keepNet = LORAWAN_NET_RESERVE;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = LORAWAN_UPLINKMODE;

/* Application port */
uint8_t appPort = 2;

uint8_t confirmedNbTrials = 4;

int temperature, humidity, batteryVoltage, batteryLevel;
long pressure;

BME280 bme280;

static void prepareTxFrame( uint8_t port )
{
  // This enables the output to power the sensor
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
  delay(500);

  if(!bme280.init()){
    if(ENABLE_SERIAL){
      Serial.println("Device error!");
    }
  }

  // This delay is required to allow the sensor time to init
  delay(500);
  
  temperature = bme280.getTemperature() * 100;
  humidity = bme280.getHumidity();
  pressure = bme280.getPressure();
  
  Wire.end();
  
  // Turn the power to the sensor off again
  digitalWrite(Vext, HIGH);
  
  batteryVoltage = getBatteryVoltage();
  batteryLevel = (BoardGetBatteryLevel() / 254) * 100; 

  appDataSize = 12;
  appData[0] = highByte(temperature);
  appData[1] = lowByte(temperature);

  appData[2] = highByte(humidity);
  appData[3] = lowByte(humidity);

  appData[4] = (byte) ((pressure & 0xFF000000) >> 24 );
  appData[5] = (byte) ((pressure & 0x00FF0000) >> 16 );
  appData[6] = (byte) ((pressure & 0x0000FF00) >> 8  );
  appData[7] = (byte) ((pressure & 0X000000FF)       );

  appData[8] = highByte(batteryVoltage);
  appData[9] = lowByte(batteryVoltage);

  appData[10] = highByte(batteryLevel);
  appData[11] = lowByte(batteryLevel);

  if(ENABLE_SERIAL){
    Serial.print("Temperature: ");
    Serial.print(temperature / 100);
    Serial.print("C, Humidity: ");
    Serial.print(humidity);
    Serial.print("%, Pressure: ");
    Serial.print(pressure / 100);
    Serial.print(" mbar, Battery Voltage: ");
    Serial.print(batteryVoltage);
    Serial.print(" mV, Battery Level: ");
    Serial.print(batteryLevel);
    Serial.println(" %");
  }
}

void setup()
{
  
  boardInitMcu();
  if(ENABLE_SERIAL){
    Serial.begin(115200); 
  }
  deviceState = DEVICE_STATE_INIT;
  LoRaWAN.ifskipjoin();

}

void loop()
{
  switch( deviceState )
  {
    case DEVICE_STATE_INIT:
    {
      printDevParam();
      LoRaWAN.init(loraWanClass,loraWanRegion);
      deviceState = DEVICE_STATE_JOIN;
      break;
    }
    case DEVICE_STATE_JOIN:
    {
      LoRaWAN.join();
      break;
    }
    case DEVICE_STATE_SEND:
    {
      prepareTxFrame( appPort );
      LoRaWAN.send();
      deviceState = DEVICE_STATE_CYCLE;
      break;
    }
    case DEVICE_STATE_CYCLE:
    {
      // Schedule next packet transmission
      txDutyCycleTime = appTxDutyCycle + randr( 0, APP_TX_DUTYCYCLE_RND );
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      break;
    }
    case DEVICE_STATE_SLEEP:
    {
      LoRaWAN.sleep();
      break;
    }
    default:
    {
      deviceState = DEVICE_STATE_INIT;
      break;
    }
  }
}
