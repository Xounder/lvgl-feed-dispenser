## 07. Tempos, alimentação, GPIOs, board_config e incremental

[voltar ao índice](../11-migration-pc-to-esp32.md)

---

### 40. Tempo e timers

No simulador, a tela de dosagem utiliza atualmente um timer de aproximadamente:

```text
300 ms
```

No ESP32, esse timer poderá continuar conceitualmente existindo, mas sua implementação deverá utilizar o mecanismo adequado ao ambiente.

O importante é preservar o comportamento:

```text
periodicamente
    ↓
atualizar controller
    ↓
obter peso
    ↓
atualizar UI
```

---

### 41. Tempo real

O hardware introduz requisitos que o simulador não possui.

Por exemplo:

```text
tempo de leitura do HX711
tempo do servo
tempo de atualização da UI
tempo de estabilização do peso
```

Esses tempos deverão ser medidos no dispositivo real.

O valor de:

```text
300 ms
```

não deve ser considerado definitivo apenas porque funciona no simulador.

---

### 42. Alimentação

No PC, alimentação não faz parte da aplicação.

No dispositivo físico, passa a ser crítica.

Devem ser avaliados:

```text
ESP32-S3
display
HX711
SG90
LEDs
touch
```

Especialmente o servo pode gerar demandas de corrente que precisam ser consideradas no projeto de alimentação.

A arquitetura de software deve assumir que o hardware físico pode apresentar limitações que não existem no PC.

Os detalhes físicos de alimentação (fonte, picos do servo, separação e MB102) são tratados em [08-target-hardware.md](../08-target-hardware.md).

---

### 43. GPIOs

No simulador:

```text
GPIO = inexistente
```

No ESP32:

```text
GPIO
```

será necessário para componentes como:

* HX711;
* servo/PWM;
* botões;
* switch;
* indicador.

Os números dos GPIOs não devem ser espalhados pelo domínio.

Idealmente, ficam em uma camada/configuração específica do hardware.

O hardware físico que consome esses GPIOs está detalhado em [08-target-hardware.md](../08-target-hardware.md).

---

### 44. Configuração de pinos

A definição futura poderá seguir conceitualmente:

```text
hardware/
    board_config
        display
        touch
        hx711
        servo
        buttons
        led
```

O formato exato dependerá da implementação na plataforma ESP32 (Arduino/PlatformIO).

A intenção é evitar que o restante da aplicação precise saber:

```text
HX711 = GPIO X
servo = GPIO Y
button = GPIO Z
```

---

### 45. Migração incremental

A migração não deve ocorrer de uma vez.

Uma sequência recomendada é:

```text
1. ESP32-S3 inicializa
2. Display funciona
3. LVGL funciona
4. UI básica funciona
5. Touch funciona
6. UI completa funciona
7. HX711 funciona
8. Load cell funciona
9. Peso calibrado
10. Dispenser funciona
11. Controller integrado
12. Dosagem física
```