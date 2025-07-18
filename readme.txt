Komponenten:
Waveshare ESP32‑C6‑WROOM‑1‑N8 Development Board

2× GY‑BME280 Sensoren (Innen und Außen)

Waveshare 0.49″ OLED‑Display SSD1306 (I²C)

Zigbee-Steckdose (z. B. Tuya) – eingelernt als Zigbee‑Endgerät

Verbindungsleitungen, 3.3 V Spannungsversorgung

Pin-Belegung:
I²C-Bus (Display + beide Sensoren)

SDA → I2C-SDA (z. B. GPIO?).

SCL → I2C-SCL.

Die Standard-LPI2C-Pins sind empfohlen (z. B. GPIO10 SDA, GPIO11 SCL auf DevKitC).

GND und 3.3 V verbinden.

Sensoren:

BME innen bei Adresse 0x76

BME außen bei Adresse 0x77

OLED:

I²C-Adresse 0x3C (überlappend I²C mit Sensoren)

Zigbee‑Radiofunktionen nutzen die eingebaute 802.15.4‑Funktion, keine externen Pins erforderlich.

Das ESP32‑C6‑DevKit hat integriertes Zigbee‑Funkmodul, somit sind keine weiteren Anschlussdrähte nötig – Zigbee läuft intern über das Funkmodul. 
Reddit
+9
Waveshare
+9
Waveshare
+9
Reddit

Stromversorgung:

Alle Module (BME280, OLED) mit 3.3 V versorgen

GND aller gemeinsam verbinden.





ESP starten → es öffnet Netzwerk (180 s)

Zigbee-Steckdose in Pairing-Modus

Im Log erscheint „Device Joined“, speichere
short_addr = join_data->short_address;

Danach wird die Steckdose bei passenden Bedingungen „ON/OFF“ geschaltet.
