## 12. Critério de sucesso e roadmap

[voltar ao índice](../08-target-hardware.md)

---

### 57. Critério de sucesso da integração

A integração física estará em um estágio funcional quando for possível:

```text
ligar dispositivo
    ↓
interagir com touchscreen
    ↓
selecionar modo
    ↓
configurar quantidade
    ↓
tarar
    ↓
iniciar dosagem
    ↓
acionar mecanismo
    ↓
medir peso
    ↓
atingir objetivo
    ↓
parar mecanismo
    ↓
mostrar conclusão
```

Esse é o equivalente físico do fluxo já validado no simulador.

---

### 58. Relação com o projeto atual

O hardware físico não deve alterar o objetivo principal do projeto.

A arquitetura continua sendo:

```text
UI
 ↓
DosingController
 ↓
WeightSensor + Dispenser
 ↓
hardware físico
```

O que muda é a implementação abaixo das abstrações.

Atualmente:

```text
SimulatedWeightSensor
SimulatedDispenser
```

Futuramente:

```text
HX711WeightSensor
ServoDispenser
```

---

### 59. Estado atual do hardware

Neste momento, o hardware físico é uma especificação alvo.

A parte já desenvolvida e validada está no simulador desktop.

O comportamento de software já possui:

```text
✓ Home
✓ seleção de modo
✓ configuração
✓ dosagem simulada
✓ leitura de peso simulada
✓ dispenser simulado
✓ cancelamento
✓ conclusão
✓ nova dosagem
```

A integração física ainda representa a próxima grande etapa.

---

### 60. Roadmap de hardware

A evolução prevista é:

```text
ESP32-S3
   ↓
Display + Touch
   ↓
LVGL
   ↓
HX711
   ↓
Load Cell
   ↓
calibração
   ↓
SG90
   ↓
mecanismo
   ↓
alimentação definitiva
   ↓
testes de dosagem
   ↓
ajustes
   ↓
protótipo funcional
```

---

### 61. Princípio final

O hardware alvo deve ser entendido como a implementação física do sistema que já está sendo desenvolvido no simulador.

A arquitetura desejada é:

```text
                    APLICAÇÃO
                        │
                        ▼
                       UI
                        │
                        ▼
                DosingController
                        │
              ┌─────────┴─────────┐
              ▼                   ▼
        WeightSensor          Dispenser
              │                   │
              ▼                   ▼
        HX711 + Load Cell      SG90
              │                   │
              └─────────┬─────────┘
                        ▼
                  MECÂNICA FÍSICA
                        │
                        ▼
                      RAÇÃO
```

O princípio fundamental é:

> **O hardware deve implementar as necessidades da aplicação; a aplicação não deve ser moldada desnecessariamente pelos detalhes do hardware.**

O ESP32-S3, o display, o touch, o HX711, a célula de carga, o SG90 e os demais componentes devem formar a plataforma física sobre a qual a arquitetura já validada no simulador poderá ser executada.

A migração deve, portanto, ser incremental: **primeiro validar cada componente isoladamente, depois integrar os subsistemas e finalmente validar o ciclo completo de dosagem.**

---

[voltar ao índice](../08-target-hardware.md)