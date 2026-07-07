# HackPad

A compact 6-key programmable macropad built using the **Seeed Studio XIAO RP2040**.

HackPad started as a project to learn the complete hardware design workflow—from schematic capture and PCB design to enclosure design and firmware development. Instead of using an existing macropad, I wanted to build one from scratch and understand every part of the process.

The project combines mechanical switches, a rotary encoder, and a small OLED display into a compact device that can be customized for productivity, media control, programming shortcuts, CAD software, or everyday workflows.

---

## Features

- 6 Mechanical Keys
- Rotary Encoder with Push Button
- 0.91" SSD1306 OLED Display
- USB Type-C Connectivity
- Powered by Seeed Studio XIAO RP2040
- Custom PCB designed in KiCad
- 3D Printed Enclosure
- QMK Firmware 

---

## PCB

![PCB](assets/PCB1.png)

The PCB was designed entirely in **KiCad**, with a focus on keeping the layout compact while making routing clean and easy to manufacture.

---

## Schematic

![SCHEMATICS](assets/SCH1.png)

The schematic includes the XIAO RP2040, key matrix, rotary encoder, OLED display, USB connectivity, and all supporting components required for the macropad.

---

## 3D PCB Render

![3D PCB](assets/3DPCB1.png)

Before manufacturing, the board was verified using KiCad's 3D viewer to check component placement and overall fit.

---

## Enclosure Design

| Front | Rear |
|:------:|:----:|
| ![](assets/3DCase1.png) | ![](assets/3DCase2.png) |

<p align="center">
<img src="assets/3DCase3.png" width="70%">
</p>

The enclosure was designed to be simple, compact, and easy to 3D print while providing access to the USB-C port and keeping the overall footprint small.

---

## Tech Stack

- KiCad 9
- Seeed Studio XIAO RP2040
- QMK Firmware *(In Progress)*
- 3D Printing
- Git & GitHub

---

## Current Status

- ✅ Schematic Complete
- ✅ PCB Designed
- ✅ 3D PCB Verified
- ✅ Enclosure Designed
- ✅ Gerber Files Generated
- 🔄 Firmware Development
- ⏳ PCB Manufacturing
- ⏳ Assembly & Testing

---

## What I Learned

This project helped me gain hands-on experience with:

- PCB Design
- Schematic Design
- Component Selection
- PCB Routing
- 3D Enclosure Design
- Hardware Documentation
- GitHub Project Organization

---

## 🤝 Feedback

This project is still evolving, and I'd love to hear suggestions or ideas for improvements. If you spot something that could be done better, feel free to open an issue or reach out!

Thanks for checking out HackPad! 🚀