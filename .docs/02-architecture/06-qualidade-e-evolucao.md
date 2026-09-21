## 06. Qualidade, testabilidade e evolução

[voltar ao índice](../02-architecture.md)

---

### 1. Por que essa arquitetura é importante

Sem essa separação, seria fácil terminar com uma aplicação como:

```text
LVGL callback
    ↓
GPIO
    ↓
Servo
    ↓
HX711
    ↓
regra de negócio
    ↓
LVGL
```

Isso dificultaria:

* testar o domínio;
* executar a aplicação no PC;
* trocar sensores;
* trocar atuadores;
* alterar a UI;
* criar testes automatizados;
* reutilizar a lógica no ESP32;
* simular falhas.

A arquitetura atual busca evitar esse acoplamento.

---

### 2. Testabilidade

Uma das consequências desejadas da arquitetura é permitir testar a lógica sem hardware físico.

Por exemplo:

```text
DosingConfig:
    target = 100 g

Sensor simulado:
    0 g

Dispenser:
    ativo

Atualização:
    +20 g (fase rápida); +2 g a partir de ~30 g da meta (fase fina)

...

Sensor:
    100 g

Resultado esperado:
    dispenser parado
    state = COMPLETED
```

Isso pode posteriormente ser transformado em testes automatizados.

Também será possível simular situações como:

```text
sensor não responde
peso não aumenta
peso ultrapassa limite
dispenser permanece ativo
timeout
cancelamento
```

Estratégia de testes: [10-testing-strategy.md](../10-testing-strategy.md).

---

### 3. Evolução planejada da abstração

As interfaces atuais são deliberadamente simples.

Isso é aceitável durante a fase inicial do projeto.

Conforme o hardware for integrado, elas poderão evoluir.

Por exemplo, o sensor poderá precisar representar:

```text
init
tare
calibrate
read_raw
read_grams
is_valid
```

e o dispenser poderá precisar representar:

```text
init
start
stop
set_power
is_active
has_error
```

Essas alterações devem ser feitas somente quando requisitos reais justificarem sua existência.

Não se deve adicionar abstrações apenas por antecipação. Ver [05-hardware-abstraction.md](../05-hardware-abstraction.md).

---

### 4. Estado atual versus arquitetura final

É importante distinguir:

```text
ARQUITETURA DESEJADA
```

de:

```text
IMPLEMENTAÇÃO ATUAL
```

O projeto está em evolução.

Alguns pontos ainda são simplificados no código atual.

Por exemplo, o `DosingController` atualmente utiliza diretamente as implementações simuladas:

```text
simulated_weight_sensor
simulated_dispenser
```

A direção arquitetural futura é permitir uma composição mais limpa:

```text
DosingController
      ↓
WeightSensor
Dispenser
      ↓
implementação selecionada
```

Portanto, futuras melhorias devem aproximar a implementação da arquitetura definida sem introduzir complexidade desnecessária antes da necessidade.

---

### 5. Arquitetura resumida

A arquitetura pode ser resumida em quatro níveis:

```text
┌─────────────────────────────┐
│ UI                          │
│ LVGL / Screens / Navigation │
└──────────────┬──────────────┘
               │
               ▼
┌─────────────────────────────┐
│ DOMAIN                      │
│ Configuration / Controller  │
│ State / Business Rules      │
└──────────────┬──────────────┘
               │
               ▼
┌─────────────────────────────┐
│ HARDWARE INTERFACES         │
│ WeightSensor / Dispenser    │
└──────────────┬──────────────┘
          ┌────┴────┐
          ▼         ▼
┌──────────────┐ ┌──────────────┐
│ SIMULATED    │ │ REAL         │
│ PC           │ │ ESP32-S3     │
└──────────────┘ └──────────────┘
```

A regra fundamental permanece:

> **UI apresenta e recebe comandos; domínio decide; interfaces representam capacidades de hardware; implementações executam essas capacidades.**

Essa separação é a base para evoluir o projeto do simulador para o dispositivo físico sem transformar a migração em uma reescrita completa.