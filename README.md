# STM32F407 Bare-Metal Drivers (GPIO & SPI)

This repository contains **bare-metal peripheral drivers** written in **C** for the **STM32F407 (Cortex-M4)** microcontroller.  
All drivers are written **from scratch at register level**, without using HAL or CMSIS drivers.

The project demonstrates low-level understanding of MCU architecture, peripheral registers, and embedded debugging.

---

## 📌 Features

### GPIO Driver
- Pin mode configuration (Input / Output / Alternate Function / Analog)
- Output type (Push-Pull / Open-Drain)
- Speed configuration
- Pull-up / Pull-down control
- Alternate function selection (AFR)

### SPI Driver
- Master mode configuration
- Full-duplex communication
- Clock configuration (CPOL, CPHA, prescaler)
- Software Slave Management (SSM / SSI)
- Blocking (polling-based) transmit API using TXE flag
- Supports 8-bit and 16-bit data frame formats

---

## 🧪 Example Application

**SPI2 Transmit Test**

- Transmits the string **"Hello World"** over SPI2
- Transmission verified using a **logic analyzer**
- Demonstrates polling-based SPI communication

---

## 🧰 Hardware & Tools

- **Microcontroller**: STM32F407
- **Board**: STM32F407 Discovery / STM32F4 series
- **IDE**: STM32CubeIDE
- **Language**: C (bare-metal)
- **Debugging**: ST-LINK, Logic Analyzer (PulseView)

---

## 📂 Repository Structure

