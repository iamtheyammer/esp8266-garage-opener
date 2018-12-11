# esp8266-garage-opener

Open your garage with some help from an ESP8266 and a relay. This script is easy to use, and works with Siri using shortcuts.

## Installation

Grab an ESP, open Arduino, set the board, etc.

1. Download and open src/esp8266/esp8266-garage-opener.ino
2. Set the settings at the top
3. Flash
4. Connect up your ESP to the relay

Done! To control the garage, either go to the IP of the device or `garage.local` and press the button!

(this only works on your home wifi.)

## Control the garage with Siri

Grab your iPhone, and scan the below QR code. Import the shortcut, then tap on the 3 dots next to it, press on the settings, then tap `Siri Phrase` and then you can set a phrase (I use 'open the garage') so when you tell Siri that, she'll open the garage.

![qr code for shortcut](img/shortcutqrcode.png)

Alternatively, you can visit [this link](https://www.icloud.com/shortcuts/402457f6455e411a943b3ec86ddf624f): `https://www.icloud.com/shortcuts/402457f6455e411a943b3ec86ddf624f`.

## License

GNU GPLv2
