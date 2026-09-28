## 1. O que ainda não está definido

_Voltar ao índice: [`../12-roadmap.md`](../12-roadmap.md)._

---

Alguns detalhes permanecem deliberadamente abertos:

```text
○ controlador exato do display
○ modelo exato da load cell
○ mecanismo definitivo do dispenser
○ posições do SG90
○ vazão real
○ tolerância de overshoot
○ timeout definitivo
○ filtragem do peso
○ arquitetura final de alimentação
○ estrutura mecânica definitiva
```

Removidos desta lista (definidos): **controlador do touch (GT911 em `0x14`)** e **pinagem de display/touch/backlight/indicador** — ver `src/hardware/esp32/board_config.h`.

Esses itens devem ser definidos conforme o hardware e os testes avançarem.

---

## 2. Roadmap não é contrato

O roadmap deve ser atualizado quando novas informações aparecerem.

Por exemplo:

```text
simulação
 ↓
descoberta
 ↓
mudança de arquitetura
 ↓
roadmap atualizado
```

Não é necessário manter uma decisão antiga apenas porque ela apareceu primeiro na documentação.

A documentação deve refletir o estado real do projeto.

---

## 3. Regra para atualização do roadmap

Sempre que uma etapa importante for concluída, atualizar:

```text
status
```

e registrar:

```text
o que foi feito
```

Se uma etapa revelar um novo requisito:

```text
novo requisito
 ↓
adicionar ao roadmap
```

Se uma decisão deixar de fazer sentido:

```text
decisão antiga
 ↓
documentar alteração
 ↓
atualizar roadmap
```

---

## 4. Visão de longo prazo

O estado final desejado é:

```text
┌─────────────────────────────────────────┐
│            Dosador de Ração             │
├─────────────────────────────────────────┤
│                                         │
│              Display 4.3"               │
│               800×480                   │
│                                         │
│          ┌──────────────────┐           │
│          │ Configuração     │           │
│          │ da dosagem       │           │
│          └──────────────────┘           │
│                                         │
│       Peso real ← Load Cell             │
│                                         │
│       Controle → Dispenser              │
│                                         │
└─────────────────────────────────────────┘
```

Internamente:

```text
ESP32-S3
    │
    ├── LVGL
    │
    ├── UI
    │
    ├── DosingController
    │
    ├── WeightSensor
    │      └── HX711 + Load Cell
    │
    └── Dispenser
           └── SG90 + mecanismo
```

---

## 5. Estado final esperado

O produto deverá ser capaz de:

```text
1. ligar
2. inicializar hardware
3. apresentar interface
4. receber interação
5. selecionar modo
6. configurar dosagem
7. iniciar operação
8. medir peso
9. controlar dispenser
10. detectar meta
11. interromper dispenser
12. informar conclusão
```

E também:

```text
13. permitir cancelamento
14. detectar falhas
15. aplicar timeout
16. entrar em estado seguro
17. informar erro
18. permitir nova operação
```

---

## 6. Princípio final

O roadmap inteiro segue uma ideia simples:

> **Primeiro provar o comportamento. Depois integrar o hardware. Depois calibrar a física.**

O projeto já saiu da fase puramente experimental e possui um simulador funcional com:

```text
UI
+
Domain
+
Hardware abstractions
+
Simulated hardware
```

O próximo grande ciclo é aumentar a robustez dessa base.

Depois disso, a migração deve ocorrer gradualmente:

```text
PC
 ↓
robustez
 ↓
ESP32-S3
 ↓
display
 ↓
touch
 ↓
peso real
 ↓
dispenser real
 ↓
mecânica
 ↓
calibração
 ↓
testes
 ↓
produto final
```

A principal regra para os próximos agentes é:

> **Não pular diretamente para o hardware apenas porque ele está disponível. Levar para o ESP32 uma arquitetura já compreendida e testada no simulador, e usar os testes físicos para descobrir e resolver aquilo que somente o hardware pode revelar.**