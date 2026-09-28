## 02. Arquitetura e camadas

[voltar ao índice](../13-agent-guide.md)

---

## 6. Arquitetura atual

A estrutura principal é:

```text
src/
├── main.c
├── hal/
│   ├── hal.c
│   └── hal.h
├── ui/
│   ├── ui.c
│   ├── ui.h
│   ├── screen_manager.c
│   ├── screen_manager.h
│   └── screens/
│       ├── screen_chrome.c
│       ├── screen_chrome.h
│       ├── home_screen.c
│       ├── home_screen.h
│       ├── config_screen.c
│       ├── config_screen.h
│       ├── dosing_screen.c
│       ├── dosing_screen.h
│       ├── completed_screen.c
│       ├── completed_screen.h
│       ├── interrupted_screen.c
│       ├── interrupted_screen.h
│       ├── manual_release_widget.c
│       └── manual_release_widget.h
├── domain/
│   ├── dosing_config.h
│   ├── dosing_controller.c
│   └── dosing_controller.h
└── hardware/
    ├── weight_sensor.h
    ├── dispenser.h
    ├── simulated/
    │   ├── simulated_weight_sensor.c
    │   └── simulated_dispenser.c
    └── esp32/
        ├── real_weight_sensor.cpp
        └── real_dispenser.cpp
```

---

## 7. Responsabilidade de cada camada

### UI

Responsável por:

* apresentar informações;
* criar telas;
* receber interação;
* solicitar ações ao domínio;
* atualizar informações visuais.

A UI não deve assumir responsabilidades de hardware.

---

### Screen Manager

Responsável por:

* controlar qual tela está ativa;
* realizar navegação;
* centralizar transições entre telas;
* manter a configuração de dosagem compartilhada.

Não transformar o `screen_manager` em um controller de negócio.

---

### Domain

Responsável por:

* regras de negócio;
* configuração da dosagem;
* estado da operação;
* início da dosagem;
* atualização da dosagem;
* cancelamento;
* conclusão;
* futuramente tratamento de erro.

O domínio deve depender o mínimo possível de:

```text
LVGL
SDL2
Windows
Arduino/ESP-IDF
GPIO
drivers físicos
```

---

### Hardware

Responsável por representar capacidades físicas.

Exemplos:

```text
WeightSensor
Dispenser
```

A interface deve representar a necessidade da aplicação, e não detalhes desnecessários do componente físico.

---

## 8. Regra de dependência

A direção conceitual deve ser:

```text
UI
 ↓
Domain
 ↓
Hardware Interface
 ↓
Hardware Implementation
```

Evitar:

```text
Domain
 ↓
LVGL
```

ou:

```text
Domain
 ↓
SDL2
```

ou:

```text
Domain
 ↓
GPIO
```

ou:

```text
UI
 ↓
GPIO diretamente
```

Sempre que uma nova dependência aparecer, pergunte:

> Essa dependência pertence realmente a esta camada?

---

## 9. UI não deve conter regra de negócio

A UI pode fazer:

```text
botão Continuar
 ↓
dosing_controller_start()
```

Mas não deveria implementar:

```text
se peso >= meta
    parar servo
    concluir
```

Essa regra pertence ao domínio.

O princípio é:

> **A UI apresenta e solicita; o domínio decide; o hardware executa.**

---

## 10. Não colocar LVGL no domínio sem necessidade

Evite adicionar:

```c
#include "lvgl.h"
```

em:

```text
src/domain/
```

Se o controller precisar informar algo para a UI, prefira expor:

* estado;
* peso;
* configuração;
* resultado;
* erro.

A UI pode transformar essas informações em componentes LVGL.

---

## 11. Não colocar SDL2 no domínio

SDL2 existe para o ambiente desktop.

Portanto, código como:

```c
SDL_Event
SDL_Renderer
SDL_Window
```

não deve entrar na lógica de negócio.

Isso dificultaria a futura migração para ESP32.

---

## 12. Não colocar Windows no domínio

Evitar dependências como:

```c
Sleep()
Windows.h
```

dentro de:

```text
src/domain/
```

Se uma funcionalidade depender de tempo, o mecanismo utilizado deve permitir sua evolução para o ambiente embarcado.

O loop atual em `main.c` é específico do simulador.

---

## 13. Hardware abstrato

As interfaces atuais são:

```text
WeightSensor
Dispenser
```

### WeightSensor

Representa a capacidade de:

```text
ler peso
adicionar peso na simulação
resetar
```

### Dispenser

Representa:

```text
start
stop
is_active
```

A implementação física futura deverá fornecer essas capacidades de acordo com o hardware real.

---

## 14. Não acople a interface ao componente físico

Evite criar uma abstração como:

```text
SG90Controller
```

se o domínio só precisa de:

```text
Dispenser
```

O domínio não precisa saber que o mecanismo é um SG90.

O domínio precisa saber:

```text
dispenser.start()
dispenser.stop()
dispenser.is_active()
```

A implementação física decide como isso será realizado.

---

## 15. Simulação deve continuar existindo

Quando o hardware real for implementado, não remover automaticamente as implementações simuladas:

```text
src_pc/hardware/simulated/
```

O simulador continuará sendo útil para:

* desenvolvimento da UI;
* testes do domínio;
* regressão;
* reprodução de falhas;
* desenvolvimento sem hardware conectado;
* validação rápida.

A existência do hardware físico não elimina a utilidade do simulador.

---

## 16. Não substituir a simulação por hardware cedo demais

Evite uma mudança do tipo:

```text
simulação
   ↓
apagar simulação
   ↓
hardware
```

Preferir:

```text
simulação
   ↓
interface estável
   ├── simulado
   └── real
```

Isso permite comparar comportamentos.