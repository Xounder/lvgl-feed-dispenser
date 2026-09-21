## 02. O que permanece, o que será substituído e comparação

[voltar ao índice](../11-migration-pc-to-esp32.md)

---

### 4. O que permanece

A intenção é manter:

```text
src/domain/
src/ui/
interfaces de hardware
DosingConfig
DosingController
fluxo da aplicação
conceitos de estados
```

Especialmente o comportamento:

```text
configurar
   ↓
iniciar
   ↓
dosar
   ↓
atingir meta
   ↓
parar
   ↓
concluir
```

---

### 5. O que será substituído

Os principais elementos específicos do simulador são:

```text
SDL2
SDL HAL
simulated_weight_sensor
simulated_dispenser
Windows-specific code
MSVC-specific build environment
```

Eles serão substituídos por:

```text
ESP32-S3 HAL
display/touch drivers
real WeightSensor (HX711)
real Dispenser (SG90 / mecanismo)
ESP32 toolchain (PlatformIO + Arduino)
hardware-specific build system
```

---

### 6. Comparação PC × ESP32-S3

| Área       | PC                     | ESP32-S3                                    |
| ---------- | ---------------------- | ------------------------------------------- |
| Plataforma | Windows                | ESP32-S3                                    |
| CPU        | x64                    | Xtensa/RISC-V conforme variante/SDK adotado |
| Linguagem  | C++ (domínio atual em C)| C++                                        |
| Build      | CMake + Visual Studio  | PlatformIO + Arduino framework (sobre ESP-IDF) |
| Gráficos   | LVGL + SDL2            | LVGL + driver de display                    |
| Touch      | mouse/SDL2             | touch controller real                       |
| Peso       | sensor simulado        | HX711 + load cell                           |
| Dispenser  | implementação simulada | SG90/mecanismo real                         |
| Energia    | PC                     | fonte/alimentação do dispositivo            |
| Tempo      | loop desktop           | execução no firmware/RTOS                   |
| Debug      | debugger/console       | serial/JTAG/debugger                        |
| Interface  | janela                 | display físico                              |

A tabela representa a separação conceitual. Detalhes do ESP32-S3, drivers, touch controller e pinagem ainda precisam ser definidos conforme o hardware adquirido — ver [08-target-hardware.md](../08-target-hardware.md).

---

### 7. O que não deve ser migrado

Alguns componentes existem exclusivamente para o simulador.

Por exemplo:

```text
SDL2
SDL2d.dll
Windows Sleep()
SDL HAL
```

Esses componentes não devem ser levados para o firmware apenas porque funcionam no PC.

O objetivo é remover a dependência deles da aplicação quando a plataforma mudar.