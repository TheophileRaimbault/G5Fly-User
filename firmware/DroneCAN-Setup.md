### How to flash G5Fly. 

Build with Ap_Periph

Tool : build can be do with a raspberry and Openocd??  

after 


Voici un **Markdown prêt à copier/coller** pour documenter ce que tu as fait et garder une procédure de debug exploitable.

***

````markdown
# Bring-up AP_Periph SeptentrioG5fly — Mosaic-G5 → STM32G473 → DroneCAN

## 1. Objectif

L’objectif est de transformer un récepteur Septentrio Mosaic-G5 connecté en UART à un STM32G4 en périphérique DroneCAN compatible ArduPilot.

Architecture finale :

```text
Septentrio Mosaic-G5
    ↓ SBF / UART
STM32G473 / AP_Periph
    ↓ DroneCAN
Transceiver CAN
    ↓ CANH / CANL
Cube Orange / ArduPilot
````

Résultat obtenu :

* AP\_Periph démarre correctement sur le STM32.
* Le nœud DroneCAN apparaît dans Mission Planner.
* Les messages GNSS DroneCAN sont publiés.
* Le Cube Orange peut utiliser le récepteur comme GPS DroneCAN.
* Le flux Mosaic-G5 le plus fiable est le **SBF**, pas le NMEA.

AP\_Periph utilise le système `hwdef.dat` de ChibiOS/ArduPilot pour définir le MCU, les pins, les interfaces et les drivers embarqués. Les STM32G4 sont supportés par AP\_Periph.

***

# 2. Fichiers de configuration utilisés

Les fichiers de configuration sont placés dans :

```bash
~/ardupilot/libraries/AP_HAL_ChibiOS/hwdef/SeptentrioG5fly/
```

Fichiers principaux :

```text
hwdef.dat
defaults.parm
```

Un `hwdef-bl.dat` a été créé pour générer un bootloader, mais la version fonctionnelle actuelle est flashée directement sans bootloader ArduPilot.

***

# 3. Configuration hardware STM32 / AP\_Periph

## 3.1 MCU

Le hardware utilise un STM32G473RCT6, mais la build ArduPilot a été faite avec le module STM32G474xx car le support ArduPilot/ChibiOS accepte cette famille pour ce bring-up.

```text
MCU STM32G4xx STM32G474xx
FLASH_SIZE_KB 256
```

***

## 3.2 Flash et stockage paramètres

La version finale fonctionnelle est sans bootloader :

```text
FLASH_RESERVE_START_KB 0
define AP_BOOTLOADER_FLASHING_ENABLED 0
```

Le stockage paramètres est placé en fin de première banque flash :

```text
STORAGE_FLASH_PAGE 63
define HAL_STORAGE_SIZE 2048
```

Cette configuration évite le conflit entre l’application AP\_Periph et la zone de paramètres.

***

## 3.3 UARTs

### USART1 — lien GPS avec le Mosaic-G5

```text
PA10 USART1_RX USART1
PA9  USART1_TX USART1
```

Connexion :

```text
Mosaic TX → PA10 / USART1_RX
Mosaic RX ← PA9  / USART1_TX
GND       ↔ GND
```

### USART2 — port de debug temporaire

```text
PA3 USART2_RX USART2
PA2 USART2_TX USART2
```

Connexion possible pour debug PC/Raspberry :

```text
USB-TTL TX → PA3
USB-TTL RX ← PA2
GND        ↔ GND
```

***

## 3.4 Ordre des ports série

```text
SERIAL_ORDER USART1 USART2
```

Le GPS AP\_Periph est associé au port logique 0 :

```text
define HAL_PERIPH_GPS_PORT_DEFAULT 0
```

Donc :

```text
GPS_PORT = 0 → USART1 → Mosaic-G5
```

***

## 3.5 CAN

```text
PA11 CAN1_RX CAN1
PA12 CAN1_TX CAN1
```

Le STM32 ne doit pas être connecté directement au bus CAN. Il faut obligatoirement un transceiver CAN entre PA11/PA12 et CANH/CANL.

Bus CAN :

```text
STM32 PA11/PA12
    ↓
Transceiver CAN
    ↓
CANH / CANL
    ↓
Cube Orange
```

La terminaison CAN standard est une résistance de 120 Ω entre CANH et CANL à chaque extrémité du bus. Avec deux terminaisons, la mesure hors tension entre CANH et CANL doit être proche de 60 Ω. [\[docs.ncnynl.com\]](https://docs.ncnynl.com/en/px4/en/can/)

***

# 4. Configuration AP\_Periph importante

Extraits essentiels du `hwdef.dat` :

```text
env AP_PERIPH 1

define HAL_USE_SERIAL TRUE
define HAL_USE_EMPTY_IO TRUE

define STM32_SERIAL_USE_USART1 TRUE
define STM32_SERIAL_USE_USART2 TRUE
define STM32_SERIAL_USE_USART3 FALSE

define HAL_USE_ADC FALSE
define DMA_RESERVE_SIZE 0

define AP_PERIPH_GPS_ENABLED 1

define GPS_MAX_RECEIVERS 1
define GPS_MAX_INSTANCES 1
define HAL_COMPASS_MAX_SENSORS 0

define AP_PERIPH_IMU_ENABLED 0
define AP_PERIPH_BARO_ENABLED 0
define AP_PERIPH_NOTIFY_ENABLED 0
define AP_PERIPH_MAG_ENABLED 0
```

***

# 5. Paramètres par défaut AP\_Periph

Le fichier suivant est utilisé :

```bash
libraries/AP_HAL_ChibiOS/hwdef/SeptentrioG5fly/defaults.parm
```

Contenu recommandé :

```text
# Default parameters for SeptentrioG5fly AP_Periph

CAN_BAUDRATE 1000000
CAN_NODE 0

GPS_PORT 0
GPS1_TYPE 10
GPS_AUTO_CONFIG 0
GPS1_RATE_MS 100
GPS_SAVE_CFG 0

DEBUG 0
```

Notes :

* `GPS1_TYPE = 10` correspond au driver SBF Septentrio côté AP\_Periph. ArduPilot documente l’utilisation de `GPS1_TYPE = 10` pour les GPS Septentrio/SBF. [\[github.com\]](https://github.com/ArduPilot/ardupilot/blob/master/libraries/AP_GPS/AP_GPS_SBF.h)
* `GPS_PORT = 0` correspond à USART1 dans notre `SERIAL_ORDER`.
* `GPS_AUTO_CONFIG = 0` est utilisé car la configuration Mosaic-G5 est faite manuellement.
* `CAN_NODE = 0` permet l’allocation dynamique du Node ID DroneCAN.

Lors du build, vérifier que le fichier est bien embarqué :

```text
Embedding file defaults.parm:.../processed_defaults.parm
```

***

# 6. Build AP\_Periph

Depuis le dossier ArduPilot :

```bash
cd ~/ardupilot
rm -rf build/SeptentrioG5fly
./waf configure --board SeptentrioG5fly
./waf AP_Periph
```

Vérifier que le binaire existe :

```bash
ls -lh build/SeptentrioG5fly/bin/
```

Le fichier principal est :

```text
build/SeptentrioG5fly/bin/AP_Periph
```

Vérifier que le firmware ne chevauche pas la zone de stockage :

```bash
arm-none-eabi-objdump -h build/SeptentrioG5fly/bin/AP_Periph | grep -E "text|data"
```

La fin du firmware doit rester avant la zone de stockage, ici proche de :

```text
0x0801F800
```

***

# 7. Flash via Raspberry Pi en SWD

La Raspberry Pi a été utilisée comme programmateur SWD via OpenOCD.

OpenOCD détectait correctement le STM32 :

```text
SWD DPIDR 0x2ba01477
Cortex-M4 detected
Examination succeed
```

Commande de flash :

```bash
sudo openocd -f ~/rpi_stm32g473_flash.cfg \
  -c "init" \
  -c "reset halt" \
  -c "program /home/septentriorpi/ardupilot/build/SeptentrioG5fly/bin/AP_Periph verify" \
  -c "reset run" \
  -c "shutdown"
```

Succès attendu :

```text
** Programming Finished **
** Verify Started **
** Verified OK **
```

***

# 8. Configuration Mosaic-G5

La solution fonctionnelle utilise **SBF**, pas NMEA.

## 8.1 Configuration UART Mosaic-G5

Sur le port Mosaic connecté au STM32 :

```text
Baudrate : 115200
Protocol : SBF
NMEA     : désactivé sur ce port
```

## 8.2 Blocs SBF nécessaires

Configurer le Mosaic pour émettre au minimum :

```text
PVTGeodetic
DOP
ReceiverStatus
VelCovGeodetic
```

Fréquence recommandée au début :

```text
1 Hz ou 5 Hz
```

Une fois validé :

```text
10 Hz possible si le port n’est pas saturé
```

Le driver SBF ArduPilot utilise notamment les blocs `PVTGeodetic`, `DOP`, `ReceiverStatus` et `VelCovGeodetic`. [\[firmware.a...upilot.org\]](https://firmware.ardupilot.org/coverage/AP_GPS/AP_GPS_SBF.h.gcov.html), [\[deepwiki.com\]](https://deepwiki.com/ArduPilot/ardupilot/2.5-gps-and-sensor-integration)

***

# 9. Configuration Cube Orange / ArduPilot

## 9.1 Configuration CAN

Si le périphérique est branché sur CAN2 du Cube :

```text
CAN_P2_DRIVER   = 1
CAN_D2_PROTOCOL = 1
CAN_P2_BITRATE  = 1000000
```

Si le périphérique est branché sur CAN1 :

```text
CAN_P1_DRIVER   = 1
CAN_D1_PROTOCOL = 1
CAN_P1_BITRATE  = 1000000
```

`CAN_Dx_PROTOCOL = 1` correspond à DroneCAN dans ArduPilot. [\[github.com\]](https://github.com/ArduPilot/ardupilot/issues/30315)

Après changement :

```text
Write Params
Reboot Autopilot
```

***

## 9.2 Configuration GPS côté Cube

Important : côté Cube, le GPS arrive via DroneCAN.

Donc :

```text
GPS1_TYPE = 9
```

Ne pas mettre `GPS1_TYPE = 10` sur le Cube.  
`10` est le type SBF pour un GPS directement connecté en série, alors que le Cube reçoit ici un périphérique DroneCAN. Pour un GPS DroneCAN, ArduPilot demande `GPSx_TYPE = 9`. [\[github.com\]](https://github.com/ArduPilot/ardupilot/issues/30315)

Optionnel :

```text
GPS1_CAN_NODEID = <Node ID du AP_Periph>
GPS1_CAN_OVRIDE = 1
```

Pour une sélection automatique :

```text
GPS1_CAN_OVRIDE = 0
```

***

# 10. Vérification dans Mission Planner

Ouvrir :

```text
CTRL-F → DroneCAN
```

Choisir :

```text
MAVLinkCAN2
```

si le périphérique est sur CAN2.

Ou :

```text
MAVLinkCAN1
```

si le périphérique est sur CAN1.

Ne pas utiliser `SLCAN` sauf si un adaptateur USB-CAN est branché directement au PC.

***

## 10.1 Messages attendus dans l’inspecteur DroneCAN

Quand tout fonctionne, le nœud AP\_Periph doit publier :

```text
uavcan_equipment_gnss_Fix2
uavcan_equipment_gnss_Auxiliary
ardupilot_gnss_Status
uavcan_protocol_NodeStatus
```

Exemple observé :

```text
ID 124 - org.ardupilot.SeptentrioG5fly
  ardupilot_gnss_Status
  uavcan_equipment_gnss_Auxiliary
  uavcan_equipment_gnss_Fix2
  uavcan_protocol_NodeStatus
```

Si seulement `NodeStatus` apparaît, le nœud fonctionne mais le GPS n’est pas décodé.

***

# 11. Procédure de debug

## 11.1 Le nœud DroneCAN n’apparaît pas

Vérifier :

```text
CANH ↔ CANH
CANL ↔ CANL
GND  ↔ GND
```

Vérifier hors tension :

```text
≈60 Ω entre CANH et CANL
```

Vérifier dans Mission Planner :

```text
CAN_Px_DRIVER = 1
CAN_Dx_PROTOCOL = 1
CAN_Px_BITRATE = 1000000
```

Vérifier que le bon canal est sélectionné dans DroneCAN Inspector :

```text
MAVLinkCAN1 ou MAVLinkCAN2
```

***

## 11.2 Le nœud apparaît mais aucun message GNSS

Symptôme :

```text
uavcan_protocol_NodeStatus uniquement
```

Causes probables :

```text
GPS1_TYPE incorrect dans AP_Periph
GPS_PORT incorrect
Mosaic encore en NMEA
SBF non envoyé sur le bon port
Flux SBF incomplet
Baudrate incorrect
```

Vérifier les paramètres du nœud AP\_Periph :

```text
GPS_PORT = 0
GPS1_TYPE = 10
GPS_AUTO_CONFIG = 0
GPS1_RATE_MS = 100
```

Vérifier le Mosaic :

```text
SBF activé
NMEA désactivé sur ce port
PVTGeodetic + DOP + ReceiverStatus + VelCovGeodetic activés
Baudrate 115200
```

***

## 11.3 Messages GNSS présents mais valeurs à zéro

Symptôme :

```text
Fix2 présent
latitude = 0
longitude = 0
sats_used = 0
status = 0
```

Causes possibles :

```text
GPS en intérieur sans fix
SBF PVTGeodetic incomplet
pas assez de blocs SBF
Mosaic pas encore initialisé
```

Tester dehors ou près d’une fenêtre.

Même sans fix, la présence des messages `Fix2`, `Auxiliary` et `ardupilot_gnss_Status` prouve que le chemin AP\_Periph → DroneCAN fonctionne.

***

## 11.4 Erreur SBF 0x40

Message observé :

```text
GPS 1: SBF error changed (0x00000000/0x00000040)
GPS 1: probing for SBF at 115200 baud
```

`0x40` est associé à une congestion de port dans des contextes Septentrio/ArduPilot. [\[ardupilot.org\]](https://ardupilot.org/copter/docs/common-gps-septentrio.html)

Actions :

```text
Réduire la fréquence SBF à 1 Hz ou 5 Hz
Désactiver NMEA sur le même port
Limiter les blocs aux blocs nécessaires
Vérifier que le baudrate est bien 115200
```

***

## 11.5 Les paramètres du nœud ne persistent pas

Vérifier que le build embarque bien `defaults.parm` :

```text
Embedding file defaults.parm:.../processed_defaults.parm
```

Vérifier le stockage flash :

```text
STORAGE_FLASH_PAGE 63
define HAL_STORAGE_SIZE 2048
```

Tester :

```text
Modifier DEBUG = 1
Commit Params
Power cycle
Vérifier que DEBUG reste à 1
```

Si les paramètres ne persistent toujours pas, faire un mass erase puis reflasher :

```bash
sudo openocd -f ~/rpi_stm32g473_flash.cfg \
  -c "init" \
  -c "reset halt" \
  -c "stm32l4x mass_erase 0" \
  -c "shutdown"
```

Puis reflasher AP\_Periph.

***

# 12. État final validé

La chaîne complète validée est :

```text
Mosaic-G5
  ↓ SBF 115200
STM32G473 AP_Periph
  ↓ DroneCAN
CAN Transceiver
  ↓ CANH/CANL
Cube Orange
  ↓ MAVLink
Mission Planner
```

Messages CAN attendus :

```text
uavcan_equipment_gnss_Fix2
uavcan_equipment_gnss_Auxiliary
ardupilot_gnss_Status
```

Paramètres AP\_Periph essentiels :

```text
GPS_PORT = 0
GPS1_TYPE = 10
GPS_AUTO_CONFIG = 0
CAN_BAUDRATE = 1000000
```

Paramètres Cube essentiels :

```text
CAN_Px_DRIVER = 1
CAN_Dx_PROTOCOL = 1
GPS1_TYPE = 9
```

***

# 13. Note rapide pour PX4

Pour PX4, le principe matériel est le même : le périphérique doit publier des messages DroneCAN GNSS valides sur le bus CAN.

Cependant, PX4 n’active pas DroneCAN par défaut. La documentation PX4 indique que DroneCAN doit être explicitement activé, que PX4 utilise encore les paramètres UAVCAN pour cette fonction, et qu’une carte SD est requise pour l’allocation dynamique des Node ID et les mises à jour firmware. [\[docs.px4.io\]](https://docs.px4.io/v1.14/en/dronecan/), [\[docs.px4.io\]](https://docs.px4.io/main/en/dronecan/)

À vérifier côté PX4/QGroundControl :

```text
UAVCAN_ENABLE = 2
```

pour activer les capteurs DroneCAN avec allocation dynamique de Node ID. Une documentation d’exemple PX4 indique que `UAVCAN_ENABLE = 2` active les capteurs UAVCAN/DroneCAN avec allocation dynamique et mise à jour firmware. [\[mathworks.com\]](https://www.mathworks.com/help/uav/px4/ref/read-gps-uavcan-example.html)

Si le nœud AP\_Periph publie déjà :

```text
uavcan_equipment_gnss_Fix2
uavcan_equipment_gnss_Auxiliary
```

alors il devrait être compatible côté protocole avec un système PX4 configuré pour recevoir des GNSS DroneCAN/UAVCAN v0, sous réserve de la version PX4 et du support exact du message GNSS utilisé. PX4 documente le support de périphériques GNSS DroneCAN/UAVCAN v0. [\[docs.px4.io\]](https://docs.px4.io/v1.14/en/dronecan/), [\[docs.px4.io\]](https://docs.px4.io/main/en/dronecan/)

```

---

Tu es arrivé au bout du bring-up : le plus important est que **SBF → AP_Periph → DroneCAN → ArduPilot** fonctionne. Pour PX4, la suite serait surtout de tester la détection du nœud avec `UAVCAN_ENABLE = 2` dans QGroundControl.
```
