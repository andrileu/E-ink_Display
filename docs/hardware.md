# 02 · Hardware and verified wiring

**Panel:** Good Display GDEY075T7, 7.5-inch monochrome, 800 × 480  
**Adapter:** Good Display DESPI-C02, 24-pin FPC to SPI header  
**Controller board:** ESP32-S3 **N16R8** (16 MB flash / 8 MB PSRAM). PCB make/revision **not yet identified**.

![Tested logical SPI wiring](images/wiring-map.svg)

## Confirmed GPIO-to-signal connections

| Adapter pin | ESP32 GPIO / power | What it does |
| --- | --- | --- |
| `BUSY` | **GPIO 5** | Controller ready/busy status |
| `RES` / `RST` | **GPIO 6** | Reset |
| `D/C` | **GPIO 7** | Data vs command |
| `CS` | **GPIO 10** | SPI chip select |
| `SDI` / `DIN` | **GPIO 11** | SPI MOSI (ESP32 sends pixels) |
| `SCK` | **GPIO 12** | SPI clock |
| `GND` | **GND** | Shared ground |
| `3.3V` | **3V3** | Adapter power; never 5 V |

**Evidence:** This logical mapping produced both V6 images and all V7 partial updates; see [tests](tests.md). It is **NOT** a top/bottom/left/right pin-location drawing. Match the exact silkscreen of your physical board. The first attempts failed because the reference pinout was different from your board.

## Power / handling checklist

- [x] Display refresh and partial updates visibly work with the current wiring.
- [ ] Photograph and identify your exact ESP32-S3 PCB; save picture in `docs/images/`.
- [ ] Record the DESPI-C02's actual RESE switch setting from the working unit. **0.47 Ω** is the intended setting for this panel; do not move a working switch blindly.
- [ ] Record power supply and confirm adequate 3.3 V regulation if changing away from the current USB setup.

**Always disconnect USB before changing wires, the FPC ribbon, or the RESE switch.** The DESPI-C02's power input and SPI signals are 3.3 V. No MISO wire is needed. Do not connect the 24-pin FPC to MCU GPIOs.

## Unknown until you verify

- Physical pin locations and which USB connector belongs to your specific third-party ESP32 PCB.
- Exact onboard regulator capacity and all current Arduino Tools menu selections.
- Independent voltage measurement of the DESPI-C02 power rail.

## References

- https://www.good-display.com/product/396.html
- https://www.good-display.com/product/516.html
- https://github.com/ZinggJM/GxEPD2
