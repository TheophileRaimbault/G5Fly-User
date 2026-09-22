# G5Fly Test Plan

Version: Rev 2  
Date: 30-07-2026  
Scope: Functional verification of the G5Fly unit through a structured test sequence.

Test file: [G5Fly_Test plan-REV2.xlsx](G5Fly_Test%20plan-REV2.xlsx)

> Note: The values shown in the result cells below are examples only. They must be replaced with the actual measured or observed results during execution.



## 1. Test Execution Order

The tests must be executed in the following order:

1. Smoke Test
   - Power verification
   - Global functional check
2. Interface and Devices Test
   - Port-by-port interface validation
   - Device validation only after all relevant ports are globally validated and no over-current condition is observed
3. GNSS Test
   - GNSS Part 1: basic acquisition and fix validation
   - GNSS Part 2: performance, robustness, and recovery validation

---

## 2. Smoke Test

Objective: Confirm that the unit powers correctly and behaves normally at system level before any deeper interface or GNSS validation.

### Test Procedure

| ID | Test Step / Task | Procedure | Expected Result | Result (example only) | Status |
|---|---|---|---|---|---|
| ST-01 | Initial visual inspection | Check board assembly, connectors, antenna connections, and mechanical mounting. | No visible damage, no loose connector, no missing component. | Example: PASS |  |
| ST-02 | Power-on with nominal supply | Apply the nominal input voltage and power the unit. | Unit powers on without abnormal behavior. | Example: PASS |  |
| ST-03 | Supply current check | Measure input current at startup and steady state. | Current remains within expected range and no abnormal surge is observed. | Example: 0.42 A |  |
| ST-04 | Boot sequence verification | Observe boot indicators, status LEDs, and/or console output. | Unit boots correctly and reaches the expected operating state. | Example: PASS |  |
| ST-05 | Basic system functionality | Confirm the main functions are alive: CPU/SoC activity, communication readiness, and default state. | System is responsive and enters the expected default operational mode. | Example: PASS |  |
| ST-06 | Reset and recovery check | Perform a reset or reboot and confirm the system returns to normal operation. | Unit recovers normally after reset. | Example: PASS |  |
| ST-07 | Thermal and stability check | Monitor temperature during operation for a short period. | Temperature remains within acceptable limits and no instability appears. | Example: 42°C |  |
| ST-08 | Global smoke status | Review all smoke-test observations together. | Smoke test is successful and the unit is ready for interface testing. | Example: PASS |  |

---

## 3. Interface and Devices Test

Objective: Validate all relevant interfaces and connected devices in a structured order. A device is considered validated only when all associated ports are validated globally and no over-current consumption is observed.

### Test Procedure

| ID | Test Step / Task | Procedure | Expected Result | Result (example only) | Status |
|---|---|---|---|---|---|
| ID-01 | Port inventory review | Identify all interfaces and ports to be tested. | All target ports are listed and available for test. | Example: PASS |  |
| ID-02 | Power rail validation on interfaces | Verify the interface power rails and related supply lines. | Correct voltage and no unexpected drop or instability. | Example: 3.3 V nominal |  |
| ID-03 | UART / serial interface test | Validate serial communication on the relevant UART ports. | Data exchange works correctly and no error is observed. | Example: PASS |  |
| ID-04 | USB interface test | Validate USB data path and host/device behavior where applicable. | USB communication is functional and stable. | Example: PASS |  |
| ID-05 | Bus interface test (I2C/SPI/CAN) | Validate communication on the bus interfaces used by the system. | All tested bus transactions succeed without retries or failures. | Example: PASS |  |
| ID-06 | GPIO / control line test | Check control and status lines for proper logic levels and switching behavior. | GPIO lines behave as expected. | Example: PASS |  |
| ID-07 | Port-level over-current check | Monitor current draw during each port validation. | No over-current condition is detected. | Example: PASS |  |
| ID-08 | Global port validation | Confirm that all required ports have passed their individual validation. | All relevant ports are validated globally. | Example: PASS |  |
| ID-09 | Device validation | Validate each connected device only after the related ports pass and no over-current condition is present. | Devices operate normally and are considered validated. | Example: PASS |  |
| ID-10 | Interface test completion | Review all interface and device results. | Interface and device test is complete and ready for GNSS validation. | Example: PASS |  |

---

## 4. GNSS Test

Objective: Validate GNSS functionality in two stages: basic acquisition and then performance/robustness behavior.

### 4.1 GNSS Part 1 – Basic Acquisition and Fix Validation

| ID | Test Step / Task | Procedure | Expected Result | Result (example only) | Status |
|---|---|---|---|---|---|
| GNSS-01 | Antenna and receiver readiness | Check GNSS antenna connection and receiver power state. | Antenna is connected correctly and receiver is powered. | Example: PASS |  |
| GNSS-02 | Receiver startup | Start the GNSS receiver and wait for initialization. | Receiver starts successfully and reports valid status. | Example: PASS |  |
| GNSS-03 | Satellite acquisition | Observe satellite detection and acquisition process. | Satellites are detected and the receiver begins tracking. | Example: 10 sats |  |
| GNSS-04 | Fix acquisition | Wait for the receiver to achieve a valid position fix. | A valid fix is acquired within the expected time. | Example: Fix acquired in 35 s |  |
| GNSS-05 | Navigation data validity | Check position, time, and velocity output. | Output data is valid and consistent. | Example: PASS |  |
| GNSS-06 | GNSS Part 1 completion | Review all Part 1 observations. | GNSS basic acquisition is validated. | Example: PASS |  |

### 4.2 GNSS Part 2 – Performance, Robustness, and Recovery

| ID | Test Step / Task | Procedure | Expected Result | Result (example only) | Status |
|---|---|---|---|---|---|
| GNSS-07 | Reacquisition after reset | Reset or restart the receiver and observe reacquisition. | Receiver reacquires correctly after reset. | Example: PASS |  |
| GNSS-08 | Recovery after temporary signal loss | Temporarily interrupt or weaken the GNSS signal and observe recovery. | Receiver recovers gracefully and reacquires the fix. | Example: PASS |  |
| GNSS-09 | Stability over time | Monitor GNSS operation during an extended test period. | Stable output and no unexpected dropouts. | Example: PASS |  |
| GNSS-10 | Data output consistency | Confirm that the GNSS output remains consistent and usable over time. | Output remains valid and stable. | Example: PASS |  |
| GNSS-11 | GNSS Part 2 completion | Review all Part 2 observations. | GNSS performance and robustness are validated. | Example: PASS |  |

---

## 5. Final Acceptance Summary

Use the following checklist to conclude the full test campaign:

- [ ] Smoke test completed successfully
- [ ] Interface and device tests completed successfully
- [ ] GNSS Part 1 completed successfully
- [ ] GNSS Part 2 completed successfully
- [ ] No over-current condition observed
- [ ] All required results recorded in the test table

This document is intended to be used as a structured test checklist and record sheet for the G5Fly validation campaign.

