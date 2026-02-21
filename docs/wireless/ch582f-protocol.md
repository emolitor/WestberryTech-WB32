# CH582F Wireless Module UART Protocol

Protocol reference for the WCH CH582F wireless module as used in USB/Bluetooth/2.4GHz tri-mode
mechanical keyboards.

---

## Introduction

The WCH CH582F is a BLE 5.0 / 2.4 GHz wireless SoC from Nanjing Qinheng Microelectronics. In
tri-mode keyboards, it handles Bluetooth and 2.4 GHz RF connectivity while the main MCU
(WB32, STM32, or similar) handles the keyboard matrix scanning, RGB lighting, and USB HID.
Communication between the main MCU and the CH582F module uses a simple UART serial protocol.

This protocol is not specific to any one keyboard. It is used across products from multiple
manufacturers including GMMK, Akko, MonsGeek, Shortcut Studio, and others. The wireless module
firmware is provided by WCH as a binary blob; the keyboard manufacturer implements the host
side of this protocol in their main MCU firmware.

This document is derived from the Shortcut Studio Bridge75 keyboard firmware, which uses the
protocol on a WB32FQ95xx main MCU. The protocol itself is MCU-agnostic. An open-source QMK
port implementing this protocol is available at
[emolitor/qmk_firmware](https://github.com/emolitor/qmk_firmware) (branch `wireless-2.0`).

---

## UART Transport

| Parameter | Value |
|-----------|-------|
| Baud rate | 115200 (configurable via `MD_BAUD_RATE`) |
| Data bits | 8 |
| Parity | None |
| Stop bits | 1 |

On WB32FQ95xx: UART1, PA9 = TX, PA10 = RX, alternate function AF7.

The transport is half-duplex at the protocol level: the host sends a message, then waits for
an ACK before sending the next message. The module may also send unsolicited messages (LED
state changes, connection events, battery reports) at any time.

---

## Message Frame Format

All messages follow the same frame structure. There is no start-of-frame delimiter or length
field -- the receiver identifies messages by their command byte and uses a fixed or
command-dependent payload length.

### Host to Module (Tx)

```
[CMD] [PAYLOAD...] [CHECKSUM]
```

### Module to Host (Rx)

```
[CMD] [PAYLOAD...] [CHECKSUM]
```

### ACK

The module sends a 3-byte ACK sequence after receiving any valid message:

```
0x61 0x0D 0x0A
```

The host also sends this same ACK after receiving any valid message from the module.

### Checksum

The checksum is a simple 8-bit sum of all preceding bytes in the message (command byte plus
all payload bytes), masked to the low 8 bits:

```c
uint8_t checksum = 0;
for (int i = 0; i < payload_length; i++) {
    checksum += data[i];
}
// checksum is appended as the final byte
```

---

## Send Commands (Host to Module)

### HID Report Commands

These commands forward HID report data from the main MCU to the wireless module for
transmission to the connected host.

| Command | Byte | Payload Length | Description |
|---------|------|----------------|-------------|
| Keyboard (6KRO) | `0xA1` | 8 bytes | Standard 6-key rollover: `[mods, reserved, key1..key6]` |
| NKRO | `0xA2` | 14 bytes | N-key rollover bitmap (overflow keys beyond 6KRO) |
| Consumer Control | `0xA3` | 2 bytes | 16-bit consumer usage code (little-endian) |
| System Control | `0xA4` | 1 byte | System usage bitmask (power=0x01, sleep=0x02, wake=0x04) |
| Fn Key | `0xA5` | 1 byte | Fn key state |
| Mouse | `0xA8` | 5 bytes | `[buttons, x, y, v_wheel, h_wheel]` (signed int8) |

Total frame size for each command is payload length + 2 (command byte + checksum byte).

### Device Configuration Commands

| Command | Byte | Payload | Description |
|---------|------|---------|-------------|
| Device Info | `0xA9` | Up to 18 bytes | BLE device name (length-prefixed): `[len, name...]` |
| Manufacturer String | `0xAB` | Up to 46 bytes | USB manufacturer string for dongle. Length-prefixed: `[len, string...]`. ASCII for module firmware < v48, UTF-16LE for >= v48. |
| Product String | `0xAC` | Up to 46 bytes | USB product string for dongle. Same encoding as manufacturer. |
| VID/PID | `0xAD` | 4 bytes | `[VID_LO, VID_HI, PID_LO, PID_HI]` -- sets USB identifiers on the 2.4G dongle |
| Raw Pass-through | `0x91` | 32 bytes | Opaque data forwarded to/from the host via vendor HID |

### Device Control Command (0xA6)

The `0xA6` command uses a sub-command byte as its payload. The full frame is:

```
[0xA6] [SUB_CMD] [CHECKSUM]
```

#### Mode Selection Sub-commands

| Sub-command | Byte | Description |
|-------------|------|-------------|
| USB mode | `0x11` | Switch to wired USB mode |
| 2.4G mode | `0x30` | Switch to 2.4 GHz RF mode |
| BT1 | `0x31` | Switch to Bluetooth slot 1 |
| BT2 | `0x32` | Switch to Bluetooth slot 2 |
| BT3 | `0x33` | Switch to Bluetooth slot 3 |
| BT4 | `0x34` | Switch to Bluetooth slot 4 |
| BT5 | `0x35` | Switch to Bluetooth slot 5 |

#### Pairing and Connection Sub-commands

| Sub-command | Byte | Description |
|-------------|------|-------------|
| Pair | `0x51` | Enter pairing mode for the currently selected device |
| Unpair | `0x52` | Clear pairing data for the currently selected device |
| Query battery | `0x53` | Request battery percentage (module responds with `0x5C`) |
| Sleep | `0x54` | Immediate sleep (reserved) |

#### Sleep Timeout Sub-commands

| Sub-command | Byte | Description |
|-------------|------|-------------|
| BT sleep timeout enable | `0x55` | Enable 30-minute auto-sleep in Bluetooth mode |
| BT sleep timeout disable | `0x56` | Disable auto-sleep in Bluetooth mode |
| 2.4G sleep timeout enable | `0x57` | Enable 30-minute auto-sleep in 2.4G mode |
| 2.4G sleep timeout disable | `0x58` | Disable auto-sleep in 2.4G mode |

#### Factory and Charging Sub-commands

| Sub-command | Byte | Description |
|-------------|------|-------------|
| Factory pair BT | `0x62` | Forced Bluetooth pairing (factory environment) |
| Factory pair 2.4G | `0x63` | Forced 2.4G pairing (factory environment) |
| Charging | `0x64` | Battery charging active |
| Charging stop | `0x65` | Battery charging stopped |
| Charging done | `0x66` | Battery charging complete |

#### Query Sub-commands

| Sub-command | Byte | Description |
|-------------|------|-------------|
| Firmware version | `0x70` | Query module firmware version (module responds with `0x5D`) |

---

## Receive Commands (Module to Host)

The module sends these messages to the host MCU. The host must respond with the ACK sequence
(`0x61 0x0D 0x0A`) after receiving each valid message.

| Command | Byte | Payload | Description |
|---------|------|---------|-------------|
| LED Indicator | `0x5A` | 1 byte | HID LED state bitmask (Num Lock, Caps Lock, etc.) |
| Device Control Status | `0x5B` | 1 byte | Connection state change (see sub-codes below) |
| Battery Percentage | `0x5C` | 1 byte | Battery level, 0-100 |
| Firmware Version | `0x5D` | 1 byte | Module firmware version number |
| Host State | `0x60` | 1 byte | `0x00` = host suspended, `0x01` = host resumed |
| Raw Pass-through | `0x81` | 32 bytes | Opaque data from host via vendor HID |

### Device Control Status Sub-codes (0x5B)

The `0x5B` command carries a sub-code indicating the connection event:

| Sub-code | Byte | Description |
|----------|------|-------------|
| Pairing | `0x31` | Module entered pairing mode |
| Connected | `0x32` | Connection established |
| Disconnected | `0x33` | Connection lost |
| Rejected | `0x36` | Connection rejected by remote host |

---

## Connection State Machine

The host firmware tracks the module's connection state using these states:

| State | Value | Description |
|-------|-------|-------------|
| `MD_STATE_NONE` | 0 | Initial state, no connection activity |
| `MD_STATE_PAIRING` | 1 | Pairing mode active, discoverable |
| `MD_STATE_CONNECTED` | 2 | Connected to a host |
| `MD_STATE_DISCONNECTED` | 3 | Disconnected, not discoverable |
| `MD_STATE_REJECT` | 4 | Connection rejected by remote host |

State transitions are driven by incoming `0x5B` messages from the module. HID reports are
only transmitted when the state is `MD_STATE_CONNECTED`. If the host firmware attempts to send
a report in any other state, it triggers a reconnection attempt by re-sending the device mode
command.

### Device Indices

| Device | Index |
|--------|-------|
| `DEVS_USB` | 0 |
| `DEVS_BT1` | 1 |
| `DEVS_BT2` | 2 |
| `DEVS_BT3` | 3 |
| `DEVS_BT4` | 4 |
| `DEVS_BT5` | 5 |
| `DEVS_2G4` | 6 |

---

## Message Queue

The host firmware uses a FIFO message queue to serialize outgoing messages. This prevents
message interleaving when multiple subsystems (keyboard, mouse, consumer control) generate
reports concurrently.

| Parameter | Value |
|-----------|-------|
| Queue depth | 40 messages (`SMSG_NUM`) |
| Max payload | 50 bytes (`SMSG_PAYLOAD_LEN`) |
| Buffer size | 2000 bytes (40 x 50) |

### ACK-Based Flow Control

The protocol uses stop-and-wait flow control:

1. Dequeue the next message from the FIFO
2. Transmit the message over UART
3. Wait for the ACK sequence (`0x61 0x0D 0x0A`) from the module
4. If ACK is received within the timeout, pop the message and send the next one
5. If no ACK is received, retry the same message

| Parameter | Value |
|-----------|-------|
| ACK timeout | 10 ms (`MD_SNED_PKT_TIMEOUT`) |
| Max retries | 40 (`MD_SEND_PKT_RETRY`) |

After 40 failed retries, the message is dropped and the next message in the queue is sent.

---

## Device Switching Protocol

### Switching from USB to Bluetooth

To switch from USB to Bluetooth slot 1 with re-pairing:

```
Tx: [0xA6] [0x31] [checksum]     # Select BT1
    wait ACK
Tx: [0xA6] [0x52] [checksum]     # Unpair (clear stored pairing)
    wait ACK
Tx: [0xA9] [len] [name...] [checksum]  # Set BLE device name
    wait ACK
Tx: [0xA6] [0x51] [checksum]     # Enter pairing mode
    wait ACK

Rx: [0x5B] [0x31] [checksum]     # Module confirms: pairing mode
    send ACK

    ... user pairs from host ...

Rx: [0x5B] [0x32] [checksum]     # Module confirms: connected
    send ACK
```

To switch to BT1 without re-pairing (reconnect to previously paired host):

```
Tx: [0xA6] [0x31] [checksum]     # Select BT1
    wait ACK

Rx: [0x5B] [0x32] [checksum]     # Module confirms: connected
    send ACK
```

### Switching to 2.4G with Dongle Configuration

When switching to 2.4G mode with a fresh pairing, the host configures the dongle's USB
descriptors so the dongle enumerates with the keyboard's own VID/PID and product name:

```
Tx: [0xA6] [0x30] [checksum]              # Select 2.4G mode
    wait ACK
Tx: [0xAB] [len] [manufacturer...] [checksum]  # Set dongle manufacturer string
    wait ACK
Tx: [0xAC] [len] [product...] [checksum]       # Set dongle product string
    wait ACK
Tx: [0xAD] [VID_LO VID_HI PID_LO PID_HI] [checksum]  # Set dongle VID/PID
    wait ACK
Tx: [0xA6] [0x52] [checksum]              # Unpair
    wait ACK
Tx: [0xA6] [0x51] [checksum]              # Pair with dongle
    wait ACK
```

Module firmware version >= 48 supports UTF-16LE encoding for manufacturer and product strings.
Earlier versions accept ASCII only.

---

## Battery Reporting

Battery percentage is polled by the host MCU. The module does not send battery reports
unsolicited.

### Query Sequence

```
Tx: [0xA6] [0x53] [checksum]     # Query battery
    wait ACK

Rx: [0x5C] [percentage] [checksum]   # Battery level (0-100)
    send ACK
```

The host polls every 3000 ms (`WLS_INQUIRY_BAT_TIME`) while in wireless mode. Battery queries
are suppressed if the message queue is busy (i.e., if there are pending HID reports waiting
for ACK).

---

## Sleep and Wake

The keyboard enters low-power sleep after a configurable inactivity timeout (default 5
minutes). The sleep sequence involves both the MCU and the wireless module.

### Sleep Sequence

1. Inactivity timeout expires (`LPWR_TIMEOUT`, default 300000 ms)
2. MCU enters pre-sleep: disables RGB LEDs, calls `suspend_power_down()`
3. After a brief delay (`LPWR_PRESLEEP_DELAY`, 200 ms), MCU enables the module's sleep timeout
   by sending the appropriate sub-command (`0x55` for BT, `0x57` for 2.4G)
4. MCU configures GPIO EXTI wake sources (key matrix lines, UART RX, USB cable detect)
5. MCU enters stop mode (LP4 on WB32, STOP on STM32)
6. CPU halted, SRAM retained, peripherals off

### Wake Sequence

1. GPIO EXTI fires (keypress, UART activity from module, or USB cable insertion)
2. MCU restores clocks and peripheral configuration
3. MCU sends device mode command to reset the module connection
4. MCU re-enables RGB LEDs if they were previously enabled
5. MCU calls `suspend_wakeup_init()` and returns to normal operation

### Host Suspend/Resume (2.4G Mode)

In 2.4G mode, the remote host can signal suspend/resume via the dongle. The module forwards
this as a `0x60` command:

- `0x60 0x00`: Host suspended -- MCU sets `manual_timeout = true` to trigger immediate sleep
- `0x60 0x01`: Host resumed -- if MCU is in sleep, it transitions to the wake state

---

## Timing Constants

| Parameter | Value | Define |
|-----------|-------|--------|
| UART baud rate | 115200 | `MD_BAUD_RATE` |
| ACK timeout | 10 ms | `MD_SNED_PKT_TIMEOUT` |
| Send retry count | 40 | `MD_SEND_PKT_RETRY` |
| Message queue depth | 40 | `SMSG_NUM` |
| Max payload size | 50 bytes | `SMSG_PAYLOAD_LEN` |
| Battery poll interval | 3000 ms | `WLS_INQUIRY_BAT_TIME` |
| Inactivity timeout | 300000 ms (5 min) | `LPWR_TIMEOUT` |
| Pre-sleep delay | 200 ms | `LPWR_PRESLEEP_DELAY` |
| Stop entry delay | 200 ms | `LPWR_STOP_DELAY` |
| Wakeup delay | 200 ms | `LPWR_WAKEUP_DELAY` |
| Raw HID payload | 32 bytes | `MD_RAW_SIZE` |

---

## Example Implementation

This protocol is implemented in the Shortcut Studio Bridge75 keyboard firmware, built on QMK
with a WB32FQ95xx main MCU. An open-source port is available at
[emolitor/qmk_firmware](https://github.com/emolitor/qmk_firmware/tree/wireless-2.0) (branch
`wireless-2.0`), which supports the Bridge75 along with several other WB32/CH582F keyboards.
The relevant source files are in the wireless driver module:

- [`module.h`](https://github.com/emolitor/qmk_firmware/blob/wireless-2.0/keyboards/shortcut/wireless/module.h) -- Command definitions, state enums, and API declarations
- [`module.c`](https://github.com/emolitor/qmk_firmware/blob/wireless-2.0/keyboards/shortcut/wireless/module.c) -- UART framing, checksum, send/receive, and device switching logic
- [`smsg.h`](https://github.com/emolitor/qmk_firmware/blob/wireless-2.0/keyboards/shortcut/wireless/smsg.h) / [`smsg.c`](https://github.com/emolitor/qmk_firmware/blob/wireless-2.0/keyboards/shortcut/wireless/smsg.c) -- FIFO message queue implementation
- [`wireless.h`](https://github.com/emolitor/qmk_firmware/blob/wireless-2.0/keyboards/shortcut/wireless/wireless.h) / [`wireless.c`](https://github.com/emolitor/qmk_firmware/blob/wireless-2.0/keyboards/shortcut/wireless/wireless.c) -- QMK host driver integration (HID report forwarding)
- [`lowpower.h`](https://github.com/emolitor/qmk_firmware/blob/wireless-2.0/keyboards/shortcut/wireless/lowpower.h) / [`lowpower.c`](https://github.com/emolitor/qmk_firmware/blob/wireless-2.0/keyboards/shortcut/wireless/lowpower.c) -- Sleep/wake state machine
- [`transport.h`](https://github.com/emolitor/qmk_firmware/blob/wireless-2.0/keyboards/shortcut/wireless/transport.h) / [`transport.c`](https://github.com/emolitor/qmk_firmware/blob/wireless-2.0/keyboards/shortcut/wireless/transport.c) -- USB/wireless transport layer switching

The same protocol implementation (with minor variations in defines and pin assignments) is
shared across keyboards from multiple manufacturers, as they all use the same WCH CH582F
module firmware.
