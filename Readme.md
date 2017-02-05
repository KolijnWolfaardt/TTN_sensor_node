# TTN Sensor Board

This is a basic sensor board I'm designing to experiment with the LORAWAN protocol, and [The Things Network](https://www.thethingsnetwork.org/) infrastructure. It is by no means intended to be a good or complete example of all the capabilities of either.


The board uses a Microchip RN2483 to perform the RF communication and a PIC18F46J50 for the other work. The sensors were chosen based on what was available, cheap, and easy to solder.


## Datasheets

* Microcontroller [PIC18F26J50](http://ww1.microchip.com/downloads/en/DeviceDoc/39632e.pdf)
* Lora Interface module [RN2483](http://ww1.microchip.com/downloads/en/DeviceDoc/50002346A.pdf)
* Battery Management IC [MCP73831/2](http://ww1.microchip.com/downloads/en/DeviceDoc/20001984g.pdf)
* Low dropout Voltage Regulator [TPS736](http://www.ti.com/lit/ds/symlink/tps736.pdf)
* Ambient Light Sensor [BH1603FVC](http://rohmfs.rohm.com/en/products/databook/datasheet/ic/sensor/light/bh1603fvc-e.pdf)
* Thermistor [MCP9701/9701A](http://ww1.microchip.com/downloads/en/DeviceDoc/21942e.pdf)
* P-channel Mosfet [NXP BSS84](http://www.nxp.com/documents/data_sheet/BSS84.pdf)
* N-channel Mosfet [NXP 2N7002](https://www.nxp.com/documents/data_sheet/2N7002.pdf)
