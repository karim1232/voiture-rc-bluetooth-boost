# Voiture RC Bluetooth + boost 5 V → 12 V

Projet embarqué de 3A, Polytech Grenoble, réalisé par Karim Glelah et Mohamed Hashem (environ 30 h, matériel du labo uniquement).

Une voiture RC à 4 moteurs (skid-steer) est pilotée en Bluetooth par une manette maison. Il y a une **STM32 Nucleo F446RE** dans chaque carte, programmée en C avec accès direct aux registres (framework Polytech, sans HAL). Le projet comprend aussi un **convertisseur boost 5 V → 12 V** commandé par un oscillateur **NE555**.

📄 [Rapport final](docs/Rapport_final.pdf) · [Cahier des charges](docs/Cahier_des_charges.pdf) · [Composants](docs/composants.md) · [Brochage](docs/pinout.md)

## Arborescence

```
.
├── code/
│   ├── controlleur/            projet STM32CubeIDE de la manette (joystick I2C + HC-05 maître)
│   ├── robot/                  projet STM32CubeIDE de la voiture (Motor Shield + HC-05 esclave)
│   └── test_moteurs_arduino/   premier essai des moteurs sous Arduino IDE
├── simulation/
│   ├── oscillateur.asc         NE555 astable seul (LTspice)
│   └── oscillateur_boost.asc   NE555 + étage boost, charge 470 Ω (LTspice)
├── docs/
│   ├── Rapport_final.pdf / .docx
│   ├── Cahier_des_charges.pdf / .docx
│   ├── composants.md           tableau des composants
│   └── pinout.md               brochage des deux cartes
├── datasheets/                 fiches techniques (voir datasheets/README.md)
└── photos/
    ├── voiture_origine/    voiture et carte QC-029RX d'origine
    ├── composants/         HC-05, shield, joystick, FTDI, transistors du labo
    ├── montages/           câblage voiture et manette
    ├── boost/              boost sur plaque d'essai puis plaque perforée
    └── captures/           test de la liaison Bluetooth (Hercules)
```

## Fonctionnement

```
Joystick ──I2C──► Nucleo manette ──USART1──► HC-05 maître ))) HC-05 esclave ──USART1──► Nucleo voiture ──PWM/GPIO──► Motor Shield ──► 4 moteurs
```

1. La manette lit X et Y (adresse `0x20`, 10 bits), applique une zone morte de ±50 autour de 512, puis choisit l'axe dominant.
2. Elle envoie **un octet ASCII**, seulement quand la commande change : `F` avant, `B` arrière, `L` gauche, `R` droite, `S` stop.
3. La voiture exécute `motor_forward(70)`, `motor_back(70)`, `motor_left(50)`, `motor_right(50)` ou `motor_stop()`. Les autres octets sont ignorés.

| Fichier | Rôle |
|---|---|
| `code/controlleur/src/i2c.c` | I2C1 (PB8/PB9, AF4, 100 kHz) |
| `code/controlleur/src/joystick.c` | lecture des axes, zone morte, conversion en commande |
| `code/*/src/bluetooth.c` | USART1 (PA9/PA10, AF7, 9600 bauds, BRR = 9375) |
| `code/robot/src/mouvement.c` | TIM3 PWM 1 kHz (PB4, PC7) + sens (PB3, PB10) |
| `code/*/src/main.c` | boucle principale de chaque carte |

## Compiler et flasher

Dans STM32CubeIDE : *File → Import → Existing Projects into Workspace*, puis choisir `code/controlleur` ou `code/robot`. On peut aussi compiler avec `make` dans le dossier du projet (Makefile du framework Polytech). Le binaire produit est `bin/prog.elf`.

## Boost 5 V → 12 V

| Grandeur | Formule | Valeur |
|---|---|---|
| Fréquence | f = 1,44 / ((R1 + 2 R2) C) | 20 kHz (R1 = 12 kΩ, R2 = 30 kΩ, C = 1 nF) |
| Rapport cyclique | D = (R1 + R2) / (R1 + 2 R2) | 58,3 % |
| Sortie idéale | Vout = Vin / (1 − D) | 12 V pour 5 V |

Simulation LTspice de `oscillateur_boost.asc` (L = 1 mH, MBR745, Ce = Cs = 1 µF, shunt 0,1 Ω) :

| Charge | Vout moyenne simulée (8-10 ms) |
|---|---|
| 470 Ω | 12,6 V |
| 1 kΩ | 16,9 V (conduction discontinue) |

Écarts entre la simulation et la carte :

| Composant | Simulation | Carte |
|---|---|---|
| MOSFET | Si4982DY (bibliothèque `SP.lib` du TP) | IRF740N |
| Diode | MBR745 (Schottky, TO-220) | diode rapide fournie par l'enseignant, équivalente |

## Résultats

| Partie | État |
|---|---|
| Moteurs (PWM + sens) | ✅ |
| Liaison Bluetooth + protocole | ✅ |
| Joystick I2C | ✅ |
| Boost + NE555 sur plaque perforée | ✅ 12 V mesurés |
| Montage final intégré | ❌ soudures défectueuses (première expérience de soudure) |

## Limites connues

- Si une transaction I2C est interrompue, le joystick bloque SDA : il faut couper puis remettre son alimentation.
- La vitesse proportionnelle calculée par la manette n'est pas transmise : la vitesse est fixée côté voiture.
- Le boost n'a pas de régulation : Vout dépend fortement de la charge.
