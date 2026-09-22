***

# Library Management

## Overview

All project components are centralized in a single library to ensure portability, reproducibility, and ease of maintenance.

```
hardware/common/libraries/
├── symbols/
│   └── G5FLY.kicad_sym
└── footprints/
    └── G5FLY.pretty/
```

All symbols and footprints used in the project must be defined in these libraries.  
External or global KiCad libraries must not be used.

***

## Adding a New Component

When introducing a new component (e.g., from JLCPCB, SnapEDA, UltraLibrarian), follow the process below.

### 1. Import and Validate

Import the symbol and footprint using the preferred method.  
Verify correctness before integration:

* Pin mapping
* Orientation
* Footprint dimensions
* Datasheet consistency

***

### 2. Add Symbol to Project Library

1. Open the Symbol Editor
2. Open the project library:
   ```
   G5FLY.kicad_sym
   ```
3. Open the imported symbol
4. Copy the symbol and paste it into `G5FLY.kicad_sym`

The library already exists; new components must be added to it, not recreated.

Apply a consistent naming convention:

```
R_10k_0603
C_100nF_0603
STM32G0_QFN32
USB_C_Receptacle
```

***

### 3. Add Footprint to Project Library

1. Open the Footprint Editor
2. Open the project footprint library:
   ```
   G5FLY.pretty
   ```
3. Copy the footprint and paste it into this library

Ensure consistency between symbol and footprint naming.

***

### 4. Add and Link 3D Model

All 3D models must be stored in:

```
hardware/common/3d_models/
```

Link the 3D model in the footprint using a relative path:

```
${KIPRJMOD}/../../../common/3d_models/component.step
```

Absolute paths must not be used.

***

### 5. Link Symbol, Footprint, and Fields

Ensure that:

* The symbol references the correct footprint
* The footprint references the correct 3D model
* Manufacturer or supplier fields are properly defined (e.g., LCSC ID)

***

### 6. Verify Library Consistency

In the schematic editor:

```
Tools → Edit Symbol Library Links
```

All components must reference:

```
G5FLY_lib
```

In the PCB editor:

```
Tools → Edit Footprint Library Links
```

Only the project footprint library should be used.

***

## Design Rules

The following rules apply to all components:

* Do not use global KiCad libraries (e.g., Device, Connector)
* Do not use absolute file paths
* Do not reference external plugin libraries directly
* Always copy components into the project libraries before use

***

# Version Management

## Directory Structure

Each hardware revision is stored independently:

```
hardware/versions/
├── v1/
├── v2/
└── v3/
```

Each version contains:

* KiCad project files
* Fabrication outputs
* Assembly notes
* Visual outputs

***

## Versioning Policy

### Immutable Releases

Released versions must not be modified after fabrication or validation.

Any change must result in a new version.

***

### Creating a New Version

To create a new revision:

```
cp -r v1 v2
```

All design modifications must be performed in the new version directory.

***

### Git Versioning

Each hardware revision must be tagged in Git:

```
v1.0
v2.0
```

Tags must correspond to manufacturable PCB versions.

***

### Recommended Workflow

* `main`: stable and released versions
* `dev`: ongoing development

Example:

```
git checkout -b dev
git commit -m "fix: correct pull-up resistor value"
git commit -m "feat: add ESD protection"
```

When ready:

```
git checkout main
git merge dev
git tag v2.0
```

***

## Fabrication Outputs

Each version must include complete manufacturing data:

```
fabrication/
├── gerbers/
├── bom/
├── pick_place/
```

This ensures that any version can be reproduced without ambiguity.

***

## Objective

The repository must guarantee that any user can:

```
git clone
open project in KiCad
generate outputs
manufacture the board
```

and obtain identical hardware.

***

## Summary

* Single, centralized project library
* No external dependencies
* Fully versioned hardware revisions
* Reproducible and traceable fabrication data
* Consistent and maintainable structure for long-term development

***