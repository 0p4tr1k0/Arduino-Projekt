# Arduino-Projekt

# Projekt
- Tento kód slouží čistě pro LEDku na ESP32


# Projekt_rele
- Tento kód je potřeba pro ovládání relé, které musíš zapojit k ESP32
- Vezmi si tři káblíky
- 1. Káblík spoj s DC+ (RELÉ) s 3V3 (ESP 32)
  2. Káblík spoj s DC- (RELÉ) s GND (ESP 32)
  3. Káblík spoj s IN (RELÉ) s D14 (ESP 32)

- DC + je pro to, aby do Relé proudil proud.
- DC - je pro uzemnění Relé s deskou.
- IN je vstup, přes který budeš relé ovládat.

# Stažení
- Kódy normálně stáhní a otevři je v arduino IDE.

# Připojení na web server
- V Arduino IDE klikni na "Verify" a pak "Upload"
- V arduino IDE si rozklikni v pravém horním rohu "serial monitor". Tam následně bys po spuštění měla vidět lokální ip (192.168.xxx.xx).
- To zadej do prohlížeče (Zařízení, kde to chceš otevřít, musí být na stejné Wi-Fi, kterou jsi zadala v kódu.
