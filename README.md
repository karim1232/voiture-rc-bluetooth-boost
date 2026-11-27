# Bluetooth RC car and 5 V to 12 V boost converter

3rd-year embedded systems project at Polytech Grenoble, by Karim Glelah and Mohamed Hashem, done in about thirty hours with lab equipment only.

We removed the stock receiver board from a 4-motor RC car (skid-steer) and drove it over Bluetooth from a homemade controller. The controller and the car each run on an STM32 Nucleo F446RE, programmed in C directly at register level (Polytech framework, no HAL). The electronics part is a 5 V to 12 V boost converter switched by an NE555 oscillator.

Documents (in French): [final report](docs/Rapport_final.pdf), [specifications](docs/Cahier_des_charges.pdf). In English: [components](docs/composants.md), [pinout](docs/pinout.md).

## Repository layout

```
.
├── code/
│   ├── controlleur/            STM32CubeIDE project for the controller (I2C joystick + master HC-05)
│   ├── robot/                  STM32CubeIDE project for the car (Motor Shield + slave HC-05)
│   └── test_moteurs_arduino/   first motor test with the Arduino IDE
├── simulation/
│   ├── oscillateur.asc         NE555 astable oscillator alone (LTspice)
│   └── oscillateur_boost.asc   NE555 + boost stage, 470 Ω load (LTspice)
├── docs/
│   ├── Rapport_final.pdf / .docx       final report (French)
│   ├── Cahier_des_charges.pdf / .docx  specifications (French)
│   ├── composants.md                   component list
│   └── pinout.md                       pinout of both boards
├── datasheets/                 datasheets (see datasheets/README.md)
└── photos/
    ├── voiture_origine/    the car and its original QC-029RX board
    ├── composants/         HC-05, shield, joystick, FTDI, lab transistors
    ├── montages/           car and controller wiring
    ├── boost/              boost on a breadboard, then on a perfboard
    └── captures/           Bluetooth link test (Hercules terminal)
```

## How it works

```
Joystick ──I2C──► controller Nucleo ──USART1──► master HC-05 ))) slave HC-05 ──USART1──► car Nucleo ──PWM/GPIO──► Motor Shield ──► 4 motors
```

The controller reads X and Y from the joystick (address `0x20`, 10-bit values). If both axes stay within 50 of the center (512), the joystick counts as released; otherwise the axis with the largest deflection decides the command.

It then sends a single ASCII byte, only when the command changes: `F` forward, `B` back, `L` left, `R` right, `S` stop.

The car calls `motor_forward(70)`, `motor_back(70)`, `motor_left(50)`, `motor_right(50)` or `motor_stop()` depending on the byte, and ignores anything else.

| File | Role |
|---|---|
| `code/controlleur/src/i2c.c` | I2C1 (PB8/PB9, AF4, 100 kHz) |
| `code/controlleur/src/joystick.c` | axis reading, dead zone, conversion to a command |
| `code/*/src/bluetooth.c` | USART1 (PA9/PA10, AF7, 9600 baud, BRR = 9375) |
| `code/robot/src/mouvement.c` | TIM3 PWM at 1 kHz (PB4, PC7) + direction (PB3, PB10) |
| `code/*/src/main.c` | main loop of each board |

## Build and flash

In STM32CubeIDE, use *File → Import → Existing Projects into Workspace* and pick `code/controlleur` or `code/robot`. You can also run `make` in the project folder (Polytech framework Makefile); the binary ends up in `bin/prog.elf`.

## The boost converter

| Quantity | Formula | Value |
|---|---|---|
| Frequency | f = 1.44 / ((R1 + 2 R2) C) | 20 kHz (R1 = 12 kΩ, R2 = 30 kΩ, C = 1 nF) |
| Duty cycle | D = (R1 + R2) / (R1 + 2 R2) | 58.3 % |
| Ideal output | Vout = Vin / (1 − D) | 12 V from 5 V |

The LTspice simulation `oscillateur_boost.asc` uses L = 1 mH, an MBR745, Ce = Cs = 1 µF and a 0.1 Ω shunt. Average output between 8 and 10 ms:

| Load | Simulated Vout |
|---|---|
| 470 Ω | 12.6 V |
| 1 kΩ | 16.9 V (discontinuous conduction) |

Two parts on the board differ from the simulation:

| Part | Simulation | Board |
|---|---|---|
| MOSFET | Si4982DY (from the course's `SP.lib` library) | IRF740N |
| Diode | MBR745 (Schottky, TO-220) | equivalent fast diode supplied by the teacher |

## Results

| Part | Status |
|---|---|
| Motors (PWM + direction) | works |
| Bluetooth link + protocol | works |
| I2C joystick | works |
| Boost + NE555 on perfboard | works, 12 V measured |
| Final integrated build | not finished: bad solder joints (our first time soldering) |

## Limitations

- If an I2C transaction is cut off halfway, the joystick holds the SDA line low. Its power has to be cycled.
- The controller computes a speed proportional to the joystick tilt but does not send it: the speed is fixed on the car side.
- The boost converter is not regulated, so its output voltage depends heavily on the load.
