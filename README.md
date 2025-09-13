# KLV C++11

Ce projet gère des paquets KLV en C++11 avec le patron de conception composite.
Les UL sont regroupées par norme MISB et chaque UL est associée à des lambdas
pour l'encodage et le décodage dans un registre.

## Tags pris en charge

Les exemples fournis couvrent quelques éléments de télémétrie et de détection :

- `PLATFORM_HEADING_ANGLE`
- `TARGET_LOCATION_ELEVATION`
- `TARGET_TRACK_GATE_WIDTH`
- `TARGET_TRACK_GATE_HEIGHT`
- `TARGET_TRACK_GATE_X`
- `TARGET_TRACK_GATE_Y`
- `SENSOR_LATITUDE`
- `SENSOR_LONGITUDE`
- `SENSOR_HORIZONTAL_FOV`
- `SENSOR_VERTICAL_FOV`
- `FRAME_CENTER_LATITUDE`
- `FRAME_CENTER_LONGITUDE`
- `FRAME_CENTER_ELEVATION`
- `OFFSET_CORNER_LAT_PT1`
- `OFFSET_CORNER_LON_PT1`
- `WIND_DIRECTION`
- `WIND_SPEED`
- `TARGET_LATITUDE`
- `TARGET_LONGITUDE`
- `WEAPON_LOAD`
- `PLATFORM_MAGNETIC_HEADING`
- `SENSOR_NORTH_VELOCITY`
- `CORNER_LAT_PT1_FULL`
- `CORNER_LON_PT1_FULL`
- `CLASSIFICATION` (ST0102)
- `CLASSIFICATION_SYSTEM` (ST0102)
- `VMTI_TARGET_ID` (ST0903)
- `VMTI_DETECTION_STATUS` (ST0903)

D'autres balises ST0601 numériques comme l'altitude/latitude de plate-forme
alternative, les hauteurs ellipsoïdales ou les angles d'attitude complets sont
également disponibles. Les champs nécessitant des ensembles imbriqués ou des
chaînes de caractères (Tag 66, 70, 73, 74, 81, 94, 95, etc.) restent hors du
cadre de cet exemple minimaliste.

## Couche STANAG 4609

La fonction `stanag::create_dataset` assemble automatiquement un `KLVSet`
à partir d'un vecteur de couples UL/valeur. Cela facilite la création d'un
jeu de données STANAG 4609 à partir des balises enregistrées des normes
ST0102, ST0601 et ST0903.

## Références

Pour la liste complète des balises et leurs définitions, se reporter à la
[norme MISB ST 0601](https://upload.wikimedia.org/wikipedia/commons/1/19/MISB_Standard_0601.pdf).

## Compilation

```
make
```

## Exécution

```
./main
```
