## 09. Abstração e dependências

[voltar ao índice](../08-target-hardware.md)

---

### 40. Dependências físicas

Os componentes podem ser classificados em:

| Componente   | Função                       |
| ------------ | ---------------------------- |
| ESP32-S3     | processamento e controle     |
| Display      | interface visual             |
| Touch        | interação                    |
| HX711        | aquisição da célula de carga |
| Load Cell    | medição de peso              |
| SG90         | acionamento mecânico         |
| Botões       | interação física             |
| Chave        | alimentação/controle geral   |
| LED          | indicação                    |
| Fonte        | alimentação                  |
| MB102        | prototipagem/distribuição    |
| Reservatório | armazenamento                |
| Recipiente   | recebimento/pesagem          |
| Estrutura    | suporte mecânico             |

---

### 41. Componentes simulados atualmente

No simulador, os componentes físicos mais importantes são representados por implementações de software.

```text
Hardware real             Simulação

Load Cell + HX711    →    SimulatedWeightSensor

SG90 + mecanismo     →    SimulatedDispenser

Display              →    SDL2 + LVGL

Touch                →    input do SDL2
```

Isso permite desenvolver o comportamento antes da montagem.

---

### 42. O que não deve ser levado diretamente para o domínio

O domínio não deve conhecer:

```text
GPIO
PWM
HX711
Load Cell
SG90
I²C
SPI
ADC
ângulo do servo
duty cycle
pino físico
```

Esses detalhes pertencem às implementações de hardware.

O domínio deve trabalhar com abstrações como:

```text
WeightSensor
Dispenser
```

---

### 43. Hardware e abstração

A relação final esperada é:

```text
                    DosingController
                           │
              ┌────────────┴────────────┐
              ▼                         ▼
        WeightSensor                 Dispenser
              │                         │
              ▼                         ▼
       HX711 implementation       Servo implementation
              │                         │
              ▼                         ▼
          Load Cell                    SG90
```

Isso permite substituir componentes físicos no futuro sem reescrever a lógica principal.

---

[voltar ao índice](../08-target-hardware.md)