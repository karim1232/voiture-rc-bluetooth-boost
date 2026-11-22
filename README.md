# Voiture RC Bluetooth et boost 5 V vers 12 V

Projet embarqué de 3A à Polytech Grenoble, fait par Karim Glelah et Mohamed Hashem en une trentaine d'heures, avec uniquement le matériel du labo.

On a retiré la carte d'origine d'une voiture RC à 4 moteurs (direction skid-steer) pour la piloter en Bluetooth depuis une manette faite maison. La manette et la voiture ont chacune une STM32 Nucleo F446RE, programmée en C directement sur les registres (framework Polytech, sans HAL). La partie électronique est un convertisseur boost 5 V vers 12 V commandé par un NE555.

Documents : [rapport final](docs/Rapport_final.pdf), [cahier des charges](docs/Cahier_des_charges.pdf), [composants](docs/composants.md), [brochage](docs/pinout.md).    

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

La manette lit X et Y sur le joystick (adresse `0x20`, valeurs sur 10 bits). Si les deux axes restent à moins de 50 du centre (512), elle considère que le joystick est au repos ; sinon c'est l'axe le plus incliné qui décide de la commande.

Elle envoie alors un seul octet ASCII, et seulement quand la commande change : `F` avant, `B` arrière, `L` gauche, `R` droite, `S` stop.

La voiture appelle `motor_forward(70)`, `motor_back(70)`, `motor_left(50)`, `motor_right(50)` ou `motor_stop()` selon l'octet reçu, et ignore tout le reste.

| Fichier | Rôle |
|---|---|
| `code/controlleur/src/i2c.c` | I2C1 (PB8/PB9, AF4, 100 kHz) |
| `code/controlleur/src/joystick.c` | lecture des axes, zone morte, conversion en commande |
| `code/*/src/bluetooth.c` | USART1 (PA9/PA10, AF7, 9600 bauds, BRR = 9375) |
| `code/robot/src/mouvement.c` | TIM3 PWM 1 kHz (PB4, PC7) + sens (PB3, PB10) |
| `code/*/src/main.c` | boucle principale de chaque carte |

## Compiler et flasher

Dans STM32CubeIDE, passer par *File → Import → Existing Projects into Workspace* et choisir `code/controlleur` ou `code/robot`. On peut aussi lancer `make` dans le dossier du projet (Makefile du framework Polytech) ; le binaire sort dans `bin/prog.elf`.

## Le boost

| Grandeur | Formule | Valeur |
|---|---|---|
| Fréquence | f = 1,44 / ((R1 + 2 R2) C) | 20 kHz (R1 = 12 kΩ, R2 = 30 kΩ, C = 1 nF) |
| Rapport cyclique | D = (R1 + R2) / (R1 + 2 R2) | 58,3 % |
| Sortie idéale | Vout = Vin / (1 − D) | 12 V pour 5 V |

La simulation LTspice `oscillateur_boost.asc` utilise L = 1 mH, une MBR745, Ce = Cs = 1 µF et un shunt de 0,1 Ω. Moyenne de la sortie entre 8 et 10 ms :

| Charge | Vout simulée |
|---|---|
| 470 Ω | 12,6 V |
| 1 kΩ | 16,9 V (conduction discontinue) |

Deux composants de la carte diffèrent de la simulation :

| Composant | Simulation | Carte |
|---|---|---|
| MOSFET | Si4982DY (bibliothèque `SP.lib` du TP) | IRF740N |
| Diode | MBR745 (Schottky, TO-220) | diode rapide fournie par l'enseignant, équivalente |

## Résultats

| Partie | État |
|---|---|
| Moteurs (PWM + sens) | fonctionne |
| Liaison Bluetooth + protocole | fonctionne |
| Joystick I2C | fonctionne |
| Boost + NE555 sur plaque perforée | fonctionne, 12 V mesurés |
| Montage final intégré | non abouti : soudures ratées (c'était notre première fois) |

## Limites

- Si une transaction I2C est coupée en plein milieu, le joystick bloque la ligne SDA. Il faut couper puis remettre son alimentation.
- La manette calcule une vitesse proportionnelle à l'inclinaison mais ne l'envoie pas : la vitesse est fixée côté voiture.
- Le boost n'est pas régulé, donc sa tension de sortie dépend beaucoup de la charge.
