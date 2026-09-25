## 06. Build system, plataforma, memória e assets

[voltar ao índice](../11-migration-pc-to-esp32.md)

---

### 31. Build system

No PC:

```text
CMake
+
Visual Studio Build Tools
+
vcpkg
```

Detalhes do ambiente de build do PC: [09-pc-development-environment.md](../09-pc-development-environment.md).

No ESP32, a expectativa (padrão da equipe) é utilizar:

```text
Arduino IDE
+
core ESP32 (Espressif)
+
Arduino framework
```

A **Arduino IDE** gerencia build, flash e monitoramento do ESP32-S3, e o Arduino framework serve como camada de aplicação sobre o ESP-IDF/FreeRTOS.

O sketch vive em `arduino/`: `src/` e `libraries/lvgl` são junctions para o código compartilhado e o LVGL do repositório (não duplicam arquivos; recriar com `arduino/create_sketch_links.ps1`), e `libraries/lv_conf.h` encaminha para `config/lv_conf_esp32.h`.

Detalhes da decisão de linguagem: [03-architecture-decisions.md](../03-architecture-decisions.md).

---

### 32. Arduino IDE + core ESP32

A implementação final deverá utilizar o ambiente apropriado para ESP32-S3.

O Arduino framework (que roda sobre o ESP-IDF) fornecerá recursos como:

* drivers;
* GPIO;
* PWM (servo via bibliotecas como ESP32Servo);
* tarefas;
* timers;
* comunicação;
* configuração do chip;
* build;
* flash;
* monitoramento.

A linguagem de aplicação será **C++**, aproveitando as bibliotecas Arduino para os periféricos (HX711, servo, display).

Esses recursos devem permanecer concentrados nas camadas específicas da plataforma.

---

### 33. FreeRTOS

O Arduino framework (assim como o ESP-IDF) utiliza FreeRTOS em sua arquitetura.

O projeto não deve assumir que o loop desktop:

```c
while(1) {
    lv_timer_handler();
    Sleep(...);
}
```

será copiado literalmente para o firmware.

O loop principal do simulador é descrito em [09-pc-development-environment.md](../09-pc-development-environment.md).

No ESP32, a execução deverá ser adaptada ao modelo de tarefas/timers do ambiente escolhido.

O comportamento da aplicação, entretanto, deve permanecer equivalente.

---

### 34. LVGL no ESP32

No PC:

```text
LVGL
 ↓
SDL HAL
```

No ESP32:

```text
LVGL
 ↓
display driver
 ↓
LCD
```

O LVGL continua responsável pelos objetos e pela interface.

A camada abaixo dele muda.

A relação display/LVGL no hardware é detalhada em [08-target-hardware.md](../08-target-hardware.md); o papel do LVGL (e do SDL2 como driver de janela) no simulador em [09-pc-development-environment.md](../09-pc-development-environment.md).

---

### 35. Display

O display é um dos primeiros subsistemas a integrar.

No ESP32 será necessário configurar:

* controlador do display;
* interface de comunicação;
* resolução;
* orientação;
* buffers;
* frequência;
* touch;
* backlight.

Esses detalhes dependem do módulo adquirido e são canônicos em [08-target-hardware.md](../08-target-hardware.md).

---

### 36. Touch driver

A UI não deve conhecer o protocolo do touch controller.

A arquitetura desejada é:

```text
Touch Controller
       ↓
Driver
       ↓
LVGL input
       ↓
UI
```

Assim, trocar o controlador de touch não exige reescrever as telas.

Os drivers de touch são detalhados em [08-target-hardware.md](../08-target-hardware.md).

---

### 37. Buffers e memória

Essa é uma diferença importante entre PC e ESP32.

No PC:

```text
memória relativamente abundante
```

No ESP32:

```text
RAM limitada
PSRAM disponível na variante alvo
```

O projeto deverá revisar:

* buffers do display;
* alocações;
* fontes;
* imagens;
* objetos LVGL;
* uso de heap;
* PSRAM;
* tamanho das telas.

Não se deve assumir que uma solução confortável no PC terá exatamente o mesmo custo no ESP32.

---

### 38. Performance

O PC possui recursos muito superiores ao microcontrolador.

Portanto, durante a migração devem ser observados:

```text
tempo de renderização
uso de RAM
uso de CPU
frequência de atualização
tamanho dos assets
quantidade de objetos
complexidade das telas
```

Uma UI que funciona perfeitamente no PC pode precisar de otimização no ESP32.

---

### 39. Assets

Imagens, fontes e outros recursos visuais também precisarão ser avaliados.

No PC, um asset grande pode ser pouco problemático.

No ESP32, ele pode consumir uma quantidade significativa de:

```text
flash
RAM
PSRAM
tempo de renderização
```

Portanto, os assets devem ser escolhidos considerando o hardware final.