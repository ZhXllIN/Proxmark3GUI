# Proxmark3GUI RRG v4.21611 修改版

[English](README.md)

这是针对 [RfidResearchGroup/proxmark3](https://github.com/RfidResearchGroup/proxmark3) 客户端命令格式适配的 Proxmark3GUI 修改版。

## 主要功能

- 配套 RRG/Iceman `v4.21611-145-g2bf48a02c` Windows 客户端
- MIFARE Classic 读取、写入、擦除、模拟和访问位工具
- `fchk`、`autopwn`、`isen` 和 FM11RF08S 恢复入口
- Gen2 CUID/FUID/UFUID 写入和擦除确认
- UFUID 永久锁卡确认
- 原始命令控制台，可使用没有独立 GUI 按钮的 RRG 命令

## 兼容版本

| 组件 | 版本 |
| --- | --- |
| GUI | `Proxmark3GUI v0.2.16` |
| Windows 客户端 | `v4.21611-145-g2bf48a02c` |
| Bootrom / OS | `v4.21611-145-g2bf48a02c` |
| 源码校验值 | `c92e2128b` |

固件仅适用于 **Proxmark3 RDV4、AT91SAM7S512（512 KB）**。不要刷入其他型号。客户端与固件必须配套更新，否则会出现版本不匹配警告。

## 下载

从 [Release v0.2.16-rrg-v4.21611](https://github.com/ZhXllIN/Proxmark3GUI/releases/tag/v0.2.16-rrg-v4.21611) 下载两个 ZIP：

- `Proxmark3GUI-v0.2.16-rrg-v4.21611-matched.zip`
- `proxmark3-firmware-rdv4-v4.21611-145-g2bf48a02c.zip`

SHA256：

```text
056c39269ebcfcdd612583e5c8ee72ee180e7c2bfe625edfc02405b2489a7b5d  Proxmark3GUI-v0.2.16-rrg-v4.21611-matched.zip
1b2b6eb0b02c10581dfe4f0e45383849a0528e068c9c503762c7ebdb20940063  proxmark3-firmware-rdv4-v4.21611-145-g2bf48a02c.zip
```

## Windows 刷入固件

1. 把两个 ZIP 解压到同一个父目录。
2. 打开解压后的固件文件夹，双击 `flash-pm3-rdv4.bat`。
3. 输入 Proxmark3 的 COM 端口，确认硬件警告，等待脚本完成刷写和版本验证。

目录结构应类似：

```text
下载目录\
|-- Proxmark3GUI-v0.2.16-rrg-v4.21611-matched\
`-- proxmark3-firmware-rdv4-v4.21611-145-g2bf48a02c\
    `-- flash-pm3-rdv4.bat
```

脚本会自动找到匹配客户端，刷入 `bootrom.elf` 和 `fullimage.elf`，最后执行 `hw version`。刷写时不要断开 USB。如果设备无法进入 Bootloader，请断开 USB，按住 Proxmark3 按钮重新连接，再运行脚本。

也可以在命令提示符中直接指定端口：

```bat
flash-pm3-rdv4.bat COM5
```

刷写完成后，客户端、Bootrom 和 OS 都应显示 `v4.21611-145-g2bf48a02c`。

## 使用 GUI

1. 完整解压 `Proxmark3GUI-v0.2.16-rrg-v4.21611-matched.zip`。
2. 双击 `启动Proxmark3GUI.bat`。
3. 选择设备 COM 端口并连接。

发布包已经包含匹配客户端、DLL、资源、字典和 RRG 配置，请保持解压后的目录结构。MIFARE Classic 常用流程可直接使用 GUI 页面，其他 RRG 命令可在原始命令控制台运行。

## 安全提醒

- 仅测试你拥有或明确获授权测试的卡片和系统。
- 写入、擦除或永久锁卡前先备份卡片。
- UFUID 永久锁卡不可逆。

## 源码

- 修改版：[ZhXllIN/Proxmark3GUI](https://github.com/ZhXllIN/Proxmark3GUI)
- 原 GUI：[wh201906/Proxmark3GUI](https://github.com/wh201906/Proxmark3GUI)
- RRG 客户端和固件：[RfidResearchGroup/proxmark3](https://github.com/RfidResearchGroup/proxmark3)

许可证见 [LICENSE](LICENSE)。
