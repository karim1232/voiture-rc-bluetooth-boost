# Composants

Tout vient du labo Polytech, on n'a rien acheté. Les fiches techniques citées sont dans `datasheets/`.

## Voiture et manette

| Composant | Qté | Rôle | Datasheet |
|---|---|---|---|
| STM32 Nucleo F446RE | 2 | MCU de la manette et de la voiture | `STM32F446xC_E_datasheet_st.pdf`, `STM32F446_RM0390_reference_manual_st.pdf`, `Nucleo64_MB1136_schematic_st.pdf` |
| HC-05 (Bluetooth 2.0 SPP) | 2 | liaison série 9600 bauds, maître / esclave | `HC-05_components101.pdf` |
| SparkFun Qwiic Joystick (COM-15168) | 1 | joystick I2C, adresse 0x20, ATtiny85 embarqué | `Qwiic_Joystick_sparkfun_hookup_guide.pdf`, `ATtiny85_microchip.pdf` |
| Microbot Dual DC Motor Shield V2 (MR007-001.2) | 1 | 2 ponts en H, 2 A par canal, moteurs de 4,8 à 46 V | `microbot_dual_dc_motor_shield_v2_MR007-001-2.pdf` |
| Allegro A4953 (sur le shield) | 2 | driver moteur DMOS | `A4952_A4953_allegro.pdf` |
| TCN75A (sur le shield) | 1 | capteur de température I2C, pas utilisé | `TCN75A_microchip.pdf` |
| Moteurs DC de la voiture | 4 | mesurés à 5 V : ~2,8 Ω, ~1,8 A, pic ~3 A | |
| YP-05 (USB-série FTDI) | 1 | configuration AT des HC-05, console printf (USART2) | |
| FTDI UM232H | 1 | pas utilisé | `UM232H_ftdi.pdf` |

## Boost 5 V vers 12 V

| Composant | Valeur / référence | Rôle | Datasheet |
|---|---|---|---|
| NE555 | astable | signal de commutation (20 kHz, 58 %) | `NE555_ti.pdf` |
| R1 / R2 | 12 kΩ / 30 kΩ | fixent f et D | |
| C1 | 1 nF | condensateur de temporisation | |
| C (broche CV) | 10 nF | découplage de la broche 5 | |
| MOSFET | IRF740N (Si4982DY en simulation) | interrupteur du boost | `IRF740_vishay.pdf` |
| Bobine L1 | 1 mH | stockage d'énergie | |
| Diode D1 | diode rapide fournie par l'enseignant (MBR745 en simulation) | diode de sortie | `MBR745_onsemi.pdf` (celle de la simulation) |
| Ce / Cs | 1 µF / 1 µF | découplage d'entrée / lissage de sortie | |
