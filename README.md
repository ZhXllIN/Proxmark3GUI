# Proxmark3GUI for RRG v4.21611

这是针对 [RfidResearchGroup/proxmark3](https://github.com/RfidResearchGroup/proxmark3) 固件和客户端命令格式适配的 Proxmark3GUI 修改版。

本版本提供：

- 与 RRG/Iceman `v4.21611-145-g2bf48a02c` 固件匹配的 Windows 客户端
- MIFARE Classic 常用读取、写入、擦除和模拟操作
- `fchk`、`autopwn`、`isen` 和 FM11RF08S 恢复入口
- Gen2 CUID/FUID/UFUID 写入、擦除和 UFUID 永久锁卡确认
- 原始命令界面，可使用未制作成独立按钮的 RRG 客户端命令

## 兼容版本

配套组件必须保持一致：

| 组件 | 版本 |
| --- | --- |
| GUI | `Proxmark3GUI v0.2.16` |
| Windows 客户端 | `v4.21611-145-g2bf48a02c` |
| Bootrom / OS | `v4.21611-145-g2bf48a02c` |
| 源码校验值 | `c92e2128b` |

固件包是为 **Proxmark3 RDV4、AT91SAM7S512（512 KB）** 编译的。不要刷入其他硬件型号。RRG 上游更新后，需要同时重新编译 Windows 客户端和固件；仅升级其中一项会再次出现版本不匹配警告。

## 下载

从 [Releases](https://github.com/ZhXllIN/Proxmark3GUI/releases/tag/v0.2.16-rrg-v4.21611) 下载：

- `Proxmark3GUI-v0.2.16-rrg-v4.21611-matched.zip`
- `proxmark3-firmware-rdv4-v4.21611-145-g2bf48a02c.zip`

文件校验值：

```text
056c39269ebcfcdd612583e5c8ee72ee180e7c2bfe625edfc02405b2489a7b5d  Proxmark3GUI-v0.2.16-rrg-v4.21611-matched.zip
d7be757cac71c6a21b8b56ddec40304314f8649ae34b4137d586b212104cc87b  proxmark3-firmware-rdv4-v4.21611-145-g2bf48a02c.zip
```

## 刷入固件

以下步骤适用于 Windows PowerShell。

1. 确认设备确实是 Proxmark3 RDV4、AT91SAM7S512。
2. 完整解压 GUI ZIP 和固件 ZIP，不要直接在压缩包内运行文件。
3. 连接 Proxmark3，关闭正在使用设备串口的 GUI 或其他客户端。
4. 在设备管理器中确认串口号，例如 `COM5`。
5. 在两个解压目录的共同父目录打开 PowerShell，执行下列命令。把 `COM5` 换成实际串口号。

```powershell
$gui = (Resolve-Path '.\Proxmark3GUI-v0.2.16-rrg-v4.21611-matched').Path
$firmware = (Resolve-Path '.\proxmark3-firmware-rdv4-v4.21611-145-g2bf48a02c').Path
$env:PATH = "$gui\client\libs;$env:PATH"

& "$gui\client\proxmark3.exe" COM5 --flash --unlock-bootloader `
  --image "$firmware\bootrom.elf" `
  --image "$firmware\fullimage.elf"
```

刷写期间不要断开 USB。`--unlock-bootloader` 允许写入 Bootrom，只能用于确认匹配的 RDV4 固件。如果客户端无法让设备进入 Bootloader，请断开 USB，按住设备按钮重新连接，再执行同一命令。

刷写完成后重新连接设备并验证版本：

```powershell
& "$gui\client\proxmark3.exe" COM5 -c 'hw version'
```

客户端、Bootrom 和 OS 应全部显示 `v4.21611-145-g2bf48a02c`，且不应出现版本不匹配警告。

## 使用 GUI

1. 完整解压 `Proxmark3GUI-v0.2.16-rrg-v4.21611-matched.zip`。
2. 双击 `启动Proxmark3GUI.bat`。
3. 在 GUI 中选择设备串口并连接。
4. MIFARE Classic 常用操作可直接使用对应页面；其他 RRG 命令可在原始命令页面执行。

发布包使用相对路径，已经包含匹配的 `proxmark3.exe`、运行库、资源、字典和 RRG v4.21611 配置。请保持压缩包内的目录结构，不要只复制 `Proxmark3GUI.exe`。

## 操作提醒

- 仅操作你拥有或明确获授权测试的卡片和系统。
- 写入、擦除和永久锁卡前先保存卡片备份。
- UFUID 永久锁卡不可逆，确认数据正确并验证可用后再执行。
- 固件更新不会替代卡片数据备份。

## 源码

- 本修改版：[ZhXllIN/Proxmark3GUI](https://github.com/ZhXllIN/Proxmark3GUI)
- 原 GUI 项目：[wh201906/Proxmark3GUI](https://github.com/wh201906/Proxmark3GUI)
- RRG 固件和客户端：[RfidResearchGroup/proxmark3](https://github.com/RfidResearchGroup/proxmark3)

项目许可证见 [LICENSE](LICENSE)。
