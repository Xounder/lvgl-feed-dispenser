## 1. Roadmap por fases

_Voltar ao índice: [`../12-roadmap.md`](../12-roadmap.md)._

---

### Fase 1 — Fundação

```text
✓ criar projeto
✓ configurar CMake
✓ configurar LVGL
✓ integrar SDL2
✓ executar simulador
```

**Status: concluída.**

---

### Fase 2 — UI

```text
✓ Home
✓ seleção de modo
✓ configuração
✓ dosagem
✓ conclusão
✓ navegação
```

**Status: concluída.**

---

### Fase 3 — Domínio

```text
✓ DosingConfig
✓ DosingController
✓ estados básicos
✓ start
✓ update
✓ cancel
```

**Status: concluída em sua forma inicial.**

---

### Fase 4 — Hardware simulado

```text
✓ WeightSensor
✓ Dispenser
✓ sensor simulado
✓ dispenser simulado
✓ reset
✓ aumento de peso
✓ parada
```

**Status: concluída em sua forma inicial.**

---

### Fase 5 — Robustez do simulador

```text
→ ERROR
→ timeout
→ sensor parado
→ overshoot
→ leituras inválidas
→ validação de configuração
→ testes automatizados
```

**Status: próximo ciclo de desenvolvimento.**

---

### Fase 6 — ESP32-S3

```text
○ ESP-IDF
○ build
○ flash
○ monitor serial
○ boot
```

**Status: planejada.**

---

### Fase 7 — Display e Touch

```text
○ display 800×480
○ LVGL
○ touch capacitivo
○ navegação completa
```

**Status: planejada.**

---

### Fase 8 — Pesagem

```text
○ HX711
○ load cell
○ tara
○ calibração
○ filtragem
○ estabilidade
```

**Status: planejada.**

---

### Fase 9 — Dispenser físico

```text
○ SG90
○ mecanismo
○ alimentação
○ posições
○ controle
```

**Status: planejada.**

---

### Fase 10 — Dosagem física

```text
○ integração WeightSensor real
○ integração Dispenser real
○ primeira dosagem
○ ajuste de vazão
○ ajuste de parada
○ overshoot
```

**Status: planejada.**

---

### Fase 11 — Robustez física

```text
○ timeout real
○ falha do sensor
○ falha do dispenser
○ reinicialização segura
○ testes de energia
○ repetibilidade
```

**Status: planejada.**

---

### Fase 12 — Produto final

```text
○ estrutura mecânica final
○ organização elétrica
○ alimentação definitiva
○ interface refinada
○ calibração final
○ testes completos
```

**Status: futura.**

---

## 2. Roadmap visual

```text
                    PROJETO
                       │
                       ▼
              ┌─────────────────┐
              │  Fundação PC    │
              │      ✓          │
              └────────┬────────┘
                       │
                       ▼
              ┌─────────────────┐
              │       UI        │
              │      ✓          │
              └────────┬────────┘
                       │
                       ▼
              ┌─────────────────┐
              │     Domínio     │
              │      ✓          │
              └────────┬────────┘
                       │
                       ▼
              ┌─────────────────┐
              │ Hardware Sim.   │
              │      ✓          │
              └────────┬────────┘
                       │
                       ▼
              ┌─────────────────┐
              │    Robustez     │
              │       →         │
              └────────┬────────┘
                       │
                       ▼
              ┌─────────────────┐
              │    ESP32-S3     │
              │       ○         │
              └────────┬────────┘
                       │
                       ▼
              ┌─────────────────┐
              │ Display/Touch   │
              │       ○         │
              └────────┬────────┘
                       │
                       ▼
              ┌─────────────────┐
              │ HX711 + Load    │
              │      Cell       │
              │       ○         │
              └────────┬────────┘
                       │
                       ▼
              ┌─────────────────┐
              │ SG90 + Mecânica │
              │       ○         │
              └────────┬────────┘
                       │
                       ▼
              ┌─────────────────┐
              │ Dosagem Física  │
              │       ○         │
              └────────┬────────┘
                       │
                       ▼
              ┌─────────────────┐
              │ Produto Final   │
              │       ○         │
              └─────────────────┘
```

Legenda:

```text
✓ concluído
→ próximo ciclo
○ planejado
```

---

## 3. Dependências entre fases

Algumas etapas dependem de outras.

Por exemplo:

```text
Display
   ↓
LVGL
   ↓
UI física
```

e:

```text
HX711
   ↓
Load Cell
   ↓
calibração
   ↓
WeightSensor real
```

e:

```text
SG90
   ↓
mecanismo
   ↓
Dispenser real
```

Por fim:

```text
WeightSensor real
       +
Dispenser real
       ↓
DosingController
       ↓
Dosagem física
```

---

## 4. O que pode acontecer em paralelo

Nem todas as tarefas precisam ser estritamente sequenciais.

Enquanto o hardware físico estiver sendo preparado, por exemplo, o simulador pode continuar evoluindo:

```text
Hardware físico
       │
       ├── montagem
       │
       └── testes elétricos

Simulador
       │
       ├── timeout
       ├── ERROR
       ├── testes
       └── melhorias de UI
```

Isso evita que o desenvolvimento fique completamente parado aguardando componentes.

---

## 5. Critério para avançar

Uma fase deve avançar quando o comportamento essencial daquela etapa estiver suficientemente compreendido.

Por exemplo, antes de integrar o dispenser físico:

```text
✓ servo responde
✓ posição conhecida
✓ mecanismo funciona
```

Antes da dosagem completa:

```text
✓ peso confiável
✓ dispenser confiável
✓ controller funcionando
```

O objetivo é evitar introduzir múltiplas fontes de falha ao mesmo tempo.