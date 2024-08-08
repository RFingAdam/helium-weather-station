# Mini Helium LoRaWAN Weather Station

**This is a battery powered LoRa weather station based around a Bosch BME280 and Heltec CubeCell development board inspired by this project on The Things Network(TTN): https://github.com/chrisys/mini-lora-weatherstation. 

Data is sent via The Helium Network to a Tago.io dashboard (or any other location you chose)**

Thanks to the Helium Network and THE PEOPLE, we're finally able to put LoRa-connected sensors almost anywhere we want! 

**For a deeper dive into the original build, check out his blog posts:**
* [Part 1: Build a simple solar-powered weather station with LoRa & The Things Network](https://www.balena.io/blog/build-a-simple-solar-powered-weather-station-with-lora-the-things-network/)
* [Part 2: Receive and store LoRa sensor data from TTN using a Raspberry Pi](https://www.balena.io/blog/receive-and-store-lora-sensor-data-from-ttn-using-a-raspberry-pi/)

## Hardware required
* Bosch BME280 breakout board 
* Heltec CubeCell HTCC-AB01 (I used the htcc-ab02s, but not the best board for this application) - be sure to select the correct LoRa frequency for your country
* 3.7V LiPo cell with a micro JST connector
* **Optional:** 6V 110x60mm solar panel 

## Other requirements
* [Arduino IDE](https://www.arduino.cc/en/main/software)
* SiLabs CP2104 Driver ([installation instructions](https://heltec-automation-docs.readthedocs.io/en/latest/general/establish_serial_connection.html))
* CubeCell framework added to Arduino IDE ([installation instructions](https://heltec-automation-docs.readthedocs.io/en/latest/cubecell/quick_start.html))
* A free Helium Console account ([register here](https://console.helium.com/))
* A free Tago.io account ([register here](https://tago.io/))
* You need to be in range of a Helium gateway ([here's a map](https://explorer.helium.com/)) or [buy your own gateway](https://www.helium.com/mine#hotspots).

## Helium Console configuration
* ([YouTube Helium Console Integration Tutorial](https://www.youtube.com/watch?v=5vmkxZVknhs))
* Add new device in the Helium Console under the 'Devices' tab. Name the device and attach an optional label for grouping multiple devices later on
* Copy Dev EUI, App EUI, and App Key in msb hex format (0x00, 0x00,....) into the Aruino code where called out like below:

        //**Important** FIll IN DEVEUI, APPEUI, AND APPKEY BELOW FOR EACH DEVICE BEFORE PROGRAMMING
        uint8_t devEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        uint8_t appKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        
* To decode the data in the Helium console, a custom payload format decoder function is required. This is included in `payload-decode.txt`. Simply paste the contents of this file into a new `Decoder`  under `Functions` within the Helium console.
* Add new Tago.io integration under 'Integrations' in the Helium console
* Connect your nodes under the `Flows` tab device in the Helium console. Weather_Station -> Weather Station Decoder -> Dashboard Integration

## Tago.io Dashboard
Dashboard Template: https://admin.tago.io/template/631a64017d5ecb00184f3071

## Enclsure Assembly (3D-Printed)
I used an Anycubic Mega Pro loaded with white PETG filament to print the enclosure for best UV/weather resistance. Open to other ideas. ~some initial prototypes were done in black ABS

* 1x Base (mini-lora-ws-base.stl)
* 6x Open layers (mini-lora-ws-openlayer.stl)
* 1x Closed layer (mini-lora-ws-closedlayer.stl)
* 1x Top layer (mini-lora-ws-top.stl)
* **Optional:** 1x Solar panel bracket
* M4 tap
* M4 stainless steel machine screws (for securing the top layer)
* M2 tap
* M2 stainless steel machine screws (for securing the CubeCell board to the base)

The holes in the top of the posts and in the upright stand for the CubeCell board on the base are tapping size for M4 and M2 respectively. This means that after it is printed you can run a standard tap into the hole and get some decent quality threads.

Once you've printed the parts they should fit together - tolerances should be large enough to cater for minor variations in prints but you may need to clean up the edges of the holes in the layers if your first layer is a bit squashed.

![parts](https://raw.githubusercontent.com/maritimeblockchain/helium-weather-station/main/assets/parts.png)

![assembly](https://raw.githubusercontent.com/maritimeblockchain/helium-weather-station/main/assets/assembly.png)

If you're adding a solar panel that layer can be placed between the top layer and the bracket.

![finished](https://raw.githubusercontent.com/maritimeblockchain/helium-weather-station/main/assets/install.jpg)
![dashboard](https://raw.githubusercontent.com/maritimeblockchain/helium-weather-station/main/assets/dashboard.png) 
