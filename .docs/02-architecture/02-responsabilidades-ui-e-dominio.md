## 02. Responsabilidades da UI e do domínio

[voltar ao índice](../02-architecture.md)

---

### 1. Camada de UI

Localização:

```text
src/ui/
```

Responsabilidade:

> Apresentar o sistema ao usuário e transformar interações do usuário em comandos para a aplicação.

A UI utiliza:

```text
LVGL
SDL2
```

no ambiente desktop.

No hardware final, o SDL2 será substituído pela camada correspondente ao display/input do ESP32-S3.

A estrutura atual de arquivos e a navegação entre telas estão detalhadas em [07-ui-and-navigation.md](../07-ui-and-navigation.md).

---

### 1.1 `ui.c`

É o ponto de inicialização da interface.

Atualmente sua responsabilidade é essencialmente:

```text
ui_init()
    ↓
screen_manager_init()
```

Ele não deve conter regras de negócio.

---

### 1.2 `screen_manager`

O `screen_manager` é responsável por controlar qual tela está sendo apresentada.

Conceitualmente:

```text
SCREEN_HOME
SCREEN_MODE
SCREEN_CONFIG
SCREEN_DOSING
SCREEN_COMPLETED
```

Ele atua como uma ponte entre o fluxo da aplicação e a apresentação visual.

Por exemplo:

```text
Usuário pressiona "Iniciar"
            ↓
home_screen
            ↓
screen_manager_show(SCREEN_MODE)
            ↓
mode_screen
```

O `screen_manager` pode conhecer:

* telas;
* LVGL;
* estado de navegação;
* configuração necessária para construir telas.

Ele não deve implementar a lógica física da dosagem.

---

### 2. Camada de domínio

Localização:

```text
src/domain/
```

Essa é a camada que representa o comportamento da aplicação.

Atualmente contém:

```text
src/domain/
├── dosing_config.h
├── dosing_controller.c
└── dosing_controller.h
```

As regras de dosagem, os estados e a máquina de estados são detalhados em [04-domain-and-state-machine.md](../04-domain-and-state-machine.md).

---

### 3. `DosingConfig`

`DosingConfig` representa a configuração necessária para executar uma dosagem.

Atualmente:

```c
typedef struct {
    int target_grams;
    int portions;
} DosingConfig;
```

Seu papel é representar **dados da configuração**, e não controlar a interface.

Exemplo:

```text
target_grams = 100
portions     = 1
```

A UI pode alterar esses valores, mas não deve assumir o controle da execução da dosagem.

---

### 4. `DosingController`

O `DosingController` é o componente central da lógica de dosagem.

Sua responsabilidade é coordenar o processo:

```text
iniciar
   ↓
ler peso
   ↓
controlar dispenser
   ↓
verificar objetivo
   ↓
finalizar ou cancelar
```

Conceitualmente:

```text
             DosingController
                    │
          ┌─────────┴─────────┐
          ▼                   ▼
    WeightSensor          Dispenser
          │                   │
       leitura             start/stop
          │                   │
          └─────────┬─────────┘
                    ▼
             decisão do estado
```

As transições de estado e as regras de cada estado estão em [04-domain-and-state-machine.md](../04-domain-and-state-machine.md).

---

### 5. O que o domínio deve conhecer

O domínio deve conhecer conceitos relacionados ao problema:

```text
Dosagem
Peso
Quantidade alvo
Porções
Estado
Dispenser
Sensor de peso
Erro
Conclusão
Cancelamento
```

---

### 6. O que o domínio não deve conhecer

O domínio não deve depender diretamente de detalhes como:

```text
LVGL
SDL2
SDL_Window
SDL_Renderer
GPIO
PWM
driver HX711
driver de display
touch controller
registradores do ESP32
```

Por exemplo, uma regra como:

```c
if (weight >= target) {
    stop_dispenser();
}
```

pertence ao domínio.

Já algo como:

```c
gpio_set_level(SERVO_PIN, 0);
```

pertence à implementação de hardware.

---

### 7. Exemplo de separação correta

**Domínio:**

```c
if (current_weight >= config->target_grams) {
    dispenser.stop();
    state = DOSING_STATE_COMPLETED;
}
```

**UI:**

```c
lv_label_set_text_fmt(
    weight_label,
    "%d g",
    current_weight
);
```

Cada camada resolve um problema diferente.

A separação de decisões entre tela e controller também é detalhada em [04-domain-and-state-machine.md](../04-domain-and-state-machine.md) e em [07-ui-and-navigation.md](../07-ui-and-navigation.md).