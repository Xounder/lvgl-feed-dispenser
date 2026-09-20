## 08. Integração e fluxos

[voltar ao índice](../08-target-hardware.md)

---

### 34. Fluxo físico completo

O comportamento físico esperado é:

```text
usuário configura
       ↓
ESP32-S3 inicia dosagem
       ↓
SG90 abre mecanismo
       ↓
ração sai do reservatório
       ↓
ração cai no recipiente
       ↓
Load Cell detecta aumento
       ↓
HX711 mede
       ↓
ESP32-S3 obtém peso
       ↓
controller compara com objetivo
       ↓
objetivo atingido
       ↓
SG90 fecha mecanismo
       ↓
dosagem concluída
```

---

### 35. Integração do sensor

O fluxo lógico do sensor será:

```text
Load Cell
   ↓
HX711
   ↓
driver
   ↓
calibração
   ↓
filtragem
   ↓
WeightSensor
   ↓
DosingController
```

Essa camada deve esconder os detalhes específicos do hardware.

---

### 36. Integração do dispenser

O fluxo lógico do dispenser será:

```text
DosingController
       ↓
Dispenser
       ↓
Servo implementation
       ↓
PWM
       ↓
SG90
       ↓
mecanismo
```

O controller não deve manipular o PWM diretamente.

---

### 37. Integração dos botões

O fluxo será aproximadamente:

```text
botão físico
    ↓
GPIO
    ↓
debounce
    ↓
evento
    ↓
UI / aplicação
```

A implementação exata dependerá de como os botões serão utilizados em conjunto com o touchscreen.

---

### 38. Indicadores

Uma possível organização será:

```text
LVGL display
    ↓
informação detalhada

LED
    ↓
indicação rápida de estado
```

Por exemplo:

```text
Display → "Dosando"
LED     → ligado
```

ou:

```text
Display → "Erro"
LED     → padrão de erro
```

As regras exatas ainda não estão definidas.

---

### 39. Comunicação entre componentes

O sistema físico pode ser dividido em:

```text
                    ESP32-S3
                       │
      ┌────────────────┼────────────────┐
      │                │                │
      ▼                ▼                ▼
   Display          HX711            Servo
      │                │                │
    Touch          Load Cell       mecanismo
      │                                 │
      └─────────── UI                   │
                                       ▼
                                     Ração
```

Os botões e indicadores também se conectam ao ESP32-S3.

---

[voltar ao índice](../08-target-hardware.md)