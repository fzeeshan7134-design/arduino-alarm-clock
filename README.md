# 🔔 Wireless Arduino Alarm System (Arduino / NRF24L01)

Ever hit snooze one too many times? This project forces you to get out of bed by hiding the only button capable of turning the alarm off somewhere else in the room.

Instead of simply pressing a button beside your bed, the alarm can only be disabled by a wireless remote powered by another Arduino, making it much harder to fall back asleep.

---

## 🚧 Status

**Work in Progress**

The core wireless communication system is functional. Future updates will focus on improving reliability, adding scheduling features, and expanding functionality.

---

## 🖼️ Gallery

### Final Prototype

*Image coming soon*

### Receiver (Alarm Unit)

*Image coming soon*

### Transmitter (Hidden Button)

*Image coming soon*

### Wiring Diagram

*Image coming soon*

---

# 🧠 How It Works

🔊 The **receiver Arduino** powers the alarm buzzer and continuously waits for incoming wireless messages.

📡 An **NRF24L01 transceiver** listens for signals sent by the remote Arduino.

🔘 The **transmitter Arduino** contains a hidden push button located somewhere away from the bed.

📨 When the button is pressed, a wireless message is transmitted using another NRF24L01 module.

✅ The receiver verifies the message and immediately turns the alarm off.

⚡ Both devices communicate wirelessly over the 2.4 GHz RF band using the RF24 library.

---

# ✨ Features

* 📡 Long-range wireless communication
* 🔔 Loud alarm buzzer
* 🔘 Hidden remote shutoff button
* ⚡ Low-cost hardware
* 🔧 Modular design for future upgrades
* 💻 Fully programmable using the Arduino IDE

---

# 🧰 Components

## Receiver

* Arduino Uno
* NRF24L01 Wireless Module
* Active Piezo Buzzer
* Breadboard
* Jumper Wires

## Transmitter

* Arduino Uno
* NRF24L01 Wireless Module
* Push Button
* 10 kΩ Pull-down Resistor
* Breadboard
* Jumper Wires

---

# 📂 Repository Structure

```text
Wireless-Arduino-Alarm/
│
├── receiver/
│   └── receiver.ino
│
├── transmitter/
│   └── transmitter.ino
│
├── images/
│   ├── prototype.jpg
│   ├── receiver.jpg
│   ├── transmitter.jpg
│   └── wiring.png
│
└── README.md
```

---

# ⚙️ Wiring

### NRF24L01 Connections

| NRF24L01 | Arduino Uno |
| -------- | ----------- |
| VCC      | 3.3V        |
| GND      | GND         |
| CE       | D7          |
| CSN      | D8          |
| MOSI     | D11         |
| MISO     | D12         |
| SCK      | D13         |

> **Note:** The NRF24L01 must be powered using the Arduino's **3.3V** pin. Supplying 5V may permanently damage the module.

---

# 🚀 Future Improvements

* ⏰ Real-Time Clock (RTC) for scheduled alarms
* 📺 LCD or OLED display
* 🔋 Rechargeable battery power
* 📱 Smartphone companion app
* 🌐 Wi-Fi or Bluetooth connectivity
* 🎵 Multiple alarm sounds
* 📊 Sleep tracking and alarm statistics

---

# 📚 Libraries Used

* RF24
* SPI

Developed using the Arduino IDE.

---

# 📈 Development Timeline

**July 2026** — Initial project idea

**July 2026** — Implemented wireless communication using NRF24L01 modules

**July 2026** — Added remote button functionality to disable the alarm

**Current** — Improving reliability, refining hardware design, and documenting the project

---

# ▶️ How to Run

1. Upload `receiver.ino` to the receiver Arduino.
2. Upload `transmitter.ino` to the transmitter Arduino.
3. Connect the NRF24L01 modules according to the wiring table above.
4. Power both Arduinos.
5. When the alarm is active, press the hidden remote button to transmit the stop signal.

---

# 🤝 Contributing

Suggestions and improvements are always welcome! Feel free to fork the repository, submit pull requests, or open an issue if you discover a bug or have an idea for a new feature.

---

# 📄 License

This project is licensed under the MIT License.

---

## 👨‍💻 Author

Created by **Faaris Zeeshan** as an Arduino project exploring wireless communication with NRF24L01 modules and embedded systems design.
