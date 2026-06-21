# Proxmark3GUI for RRG v4.21611

[中文说明](README_zh_CN.md)

This fork adapts Proxmark3GUI to the command formats used by [RfidResearchGroup/proxmark3](https://github.com/RfidResearchGroup/proxmark3).

## Features

- Matching Windows client for RRG/Iceman `v4.21611-145-g2bf48a02c`
- MIFARE Classic read, write, wipe, simulation, and access-bit tools
- Quick entries for `fchk`, `autopwn`, `isen`, and FM11RF08S recovery
- Gen2 CUID/FUID/UFUID write and wipe operations with confirmation prompts
- Permanent UFUID lock confirmation
- Raw command console for RRG commands without dedicated GUI controls

## Compatibility

| Component | Version |
| --- | --- |
| GUI | `Proxmark3GUI v0.2.16` |
| Windows client | `v4.21611-145-g2bf48a02c` |
| Bootrom / OS | `v4.21611-145-g2bf48a02c` |
| Source checksum | `c92e2128b` |

The firmware is built only for **Proxmark3 RDV4 with AT91SAM7S512 (512 KB)**. Do not flash it to a different model. The client and firmware must be updated together to avoid version mismatch warnings.

## Download

Download both ZIP files from [Release v0.2.16-rrg-v4.21611](https://github.com/ZhXllIN/Proxmark3GUI/releases/tag/v0.2.16-rrg-v4.21611):

- `Proxmark3GUI-v0.2.16-rrg-v4.21611-matched.zip`
- `proxmark3-firmware-rdv4-v4.21611-145-g2bf48a02c.zip`

SHA256:

```text
056c39269ebcfcdd612583e5c8ee72ee180e7c2bfe625edfc02405b2489a7b5d  Proxmark3GUI-v0.2.16-rrg-v4.21611-matched.zip
1b2b6eb0b02c10581dfe4f0e45383849a0528e068c9c503762c7ebdb20940063  proxmark3-firmware-rdv4-v4.21611-145-g2bf48a02c.zip
```

## Flash Firmware on Windows

1. Extract both ZIP files into the same parent folder.
2. Open the extracted firmware folder and double-click `flash-pm3-rdv4.bat`.
3. Enter the Proxmark3 COM port, confirm the hardware warning, and wait for verification to finish.

Expected layout:

```text
Downloads\
|-- Proxmark3GUI-v0.2.16-rrg-v4.21611-matched\
`-- proxmark3-firmware-rdv4-v4.21611-145-g2bf48a02c\
    `-- flash-pm3-rdv4.bat
```

The script locates the matching Windows client, flashes `bootrom.elf` and `fullimage.elf`, then runs `hw version`. Do not disconnect USB while flashing. If the device cannot enter bootloader mode, disconnect USB, hold the Proxmark3 button while reconnecting, and run the script again.

The script can also be run from Command Prompt with the port supplied directly:

```bat
flash-pm3-rdv4.bat COM5
```

Client, Bootrom, and OS should all report `v4.21611-145-g2bf48a02c` after flashing.

## Use the GUI

1. Fully extract `Proxmark3GUI-v0.2.16-rrg-v4.21611-matched.zip`.
2. Double-click `启动Proxmark3GUI.bat`.
3. Select the device COM port and connect.

The package contains the matching client, DLLs, resources, dictionaries, and RRG configuration. Keep the extracted directory structure intact. MIFARE Classic workflows are available from their GUI pages; other RRG commands remain available from the raw command console.

## Safety

- Only test cards and systems that you own or are explicitly authorized to assess.
- Back up a card before writing, wiping, or permanently locking it.
- A permanent UFUID lock is irreversible.

## Source

- Modified GUI: [ZhXllIN/Proxmark3GUI](https://github.com/ZhXllIN/Proxmark3GUI)
- Original GUI: [wh201906/Proxmark3GUI](https://github.com/wh201906/Proxmark3GUI)
- RRG client and firmware: [RfidResearchGroup/proxmark3](https://github.com/RfidResearchGroup/proxmark3)

See [LICENSE](LICENSE) for licensing information.
