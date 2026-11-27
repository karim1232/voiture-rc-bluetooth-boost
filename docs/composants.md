# Components

Everything came from the Polytech lab; nothing was bought. The datasheets listed here are in `datasheets/`.

## Car and controller

| Component | Qty | Role | Datasheet |
|---|---|---|---|
| STM32 Nucleo F446RE | 2 | MCU of the controller and of the car | `STM32F446xC_E_datasheet_st.pdf`, `STM32F446_RM0390_reference_manual_st.pdf`, `Nucleo64_MB1136_schematic_st.pdf` |
| HC-05 (Bluetooth 2.0 SPP) | 2 | 9600 baud serial link, master / slave | `HC-05_components101.pdf` |
| SparkFun Qwiic Joystick (COM-15168) | 1 | I2C joystick, address 0x20, on-board ATtiny85 | `Qwiic_Joystick_sparkfun_hookup_guide.pdf`, `ATtiny85_microchip.pdf` |
| Microbot Dual DC Motor Shield V2 (MR007-001.2) | 1 | 2 H-bridges, 2 A per channel, 4.8 to 46 V motors | `microbot_dual_dc_motor_shield_v2_MR007-001-2.pdf` |
| Allegro A4953 (on the shield) | 2 | DMOS motor driver | `A4952_A4953_allegro.pdf` |
| TCN75A (on the shield) | 1 | I2C temperature sensor, not used | `TCN75A_microchip.pdf` |
| Car DC motors | 4 | measured at 5 V: ~2.8 Ω, ~1.8 A, ~3 A peak | |
| YP-05 (FTDI USB-serial) | 1 | AT configuration of the HC-05s, printf console (USART2) | |
| FTDI UM232H | 1 | not used | `UM232H_ftdi.pdf` |

## 5 V to 12 V boost converter

| Component | Value / part | Role | Datasheet |
|---|---|---|---|
| NE555 | astable | switching signal (20 kHz, 58 %) | `NE555_ti.pdf` |
| R1 / R2 | 12 kΩ / 30 kΩ | set f and D | |
| C1 | 1 nF | timing capacitor | |
| C (CV pin) | 10 nF | pin 5 decoupling | |
| MOSFET | IRF740N (Si4982DY in simulation) | boost switch | `IRF740_vishay.pdf` |
| Inductor L1 | 1 mH | energy storage | |
| Diode D1 | fast diode supplied by the teacher (MBR745 in simulation) | output diode | `MBR745_onsemi.pdf` (the simulated part) |
| Ce / Cs | 1 µF / 1 µF | input decoupling / output smoothing | |
