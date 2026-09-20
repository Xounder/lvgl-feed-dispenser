## 08. Etapas da migração incremental

[voltar ao índice](../11-migration-pc-to-esp32.md)

As etapas abaixo executam a sequência do documento de migração incremental. A validação física de cada subsistema é detalhada em [08-target-hardware.md](../08-target-hardware.md).

---

### 46. Etapa 1 — ESP32-S3

Primeiro validar:

```text
ESP32-S3
 ↓
boot
 ↓
firmware
 ↓
serial
```

Antes de integrar a aplicação completa.

---

### 47. Etapa 2 — Display

Depois:

```text
ESP32-S3
 ↓
display
```

Validar:

* inicialização;
* resolução;
* orientação;
* backlight;
* atualização.

---

### 48. Etapa 3 — LVGL

Depois:

```text
ESP32-S3
 ↓
LVGL
 ↓
display
```

Executar uma tela simples.

O objetivo é provar que a infraestrutura gráfica funciona antes de migrar toda a aplicação.

---

### 49. Etapa 4 — Touch

Depois:

```text
display
+
touch
```

Testar:

```text
toque
 ↓
evento LVGL
 ↓
botão
```

Somente depois levar toda a navegação para o hardware.

---

### 50. Etapa 5 — UI completa

Com display e touch funcionando:

```text
Home
 ↓
Mode
 ↓
Config
 ↓
Dosing
 ↓
Completed
```

deve ser portada e validada.

Nesse momento ainda pode ser utilizado um hardware de dosagem simulado.

---

### 51. Etapa 6 — HX711

Depois integrar:

```text
HX711
 ↓
load cell
 ↓
WeightSensor
```

Primeiro sem dispenser automático.

O objetivo é validar somente a leitura de peso.

---

### 52. Etapa 7 — Calibração

Com o sensor funcionando:

```text
tara
 ↓
peso conhecido
 ↓
calibração
 ↓
verificação
```

O valor exibido pela aplicação deve ser comparado com pesos conhecidos.

---

### 53. Etapa 8 — Dispenser

Depois:

```text
SG90
 ↓
Dispenser
```

Testar o mecanismo independentemente do fluxo completo.

Validar:

```text
start
stop
posição
tempo
segurança
```

---

### 54. Etapa 9 — Integração

Somente então:

```text
WeightSensor real
+
Dispenser real
+
DosingController
```

A arquitetura final passa a ser:

```text
UI
 ↓
DosingController
 ↓
┌─────────────────┐
│                 │
▼                 ▼
WeightSensor    Dispenser
   │               │
   ▼               ▼
 HX711             SG90
   │               │
   ▼               ▼
Load Cell       Mechanism
```

---

### 55. Etapa 10 — Primeira dosagem física

A primeira dosagem real deve ser feita com:

```text
quantidade pequena
```

e supervisão direta.

O objetivo inicial não é atingir precisão máxima.

É verificar:

```text
iniciar
 ↓
ração começa a sair
 ↓
peso aumenta
 ↓
controller detecta meta
 ↓
dispenser para
```

---

### 56. Ajuste da vazão

Depois da primeira integração, a vazão real poderá ser medida.

Exemplo:

```text
peso
tempo
```

permitindo estimar:

```text
g/s
```

A simulação atual utiliza aproximadamente:

```text
6,7 g/s
```

mas esse valor é apenas um modelo inicial (taxa de alimentação da simulação em [09-pc-development-environment.md](../09-pc-development-environment.md) e [06-simulation-strategy.md](../06-simulation-strategy.md)).

O valor real provavelmente será diferente.

---

### 57. Ajuste do ponto de parada

Se houver overshoot:

```text
target = 100 g
final = 110 g
```

pode ser necessário parar o mecanismo antes da meta.

Por exemplo, conceitualmente:

```text
meta = 100 g
stop_at = 92 g
```

para que a ração em trânsito leve o sistema próximo de 100 g.

O valor não deve ser inventado antecipadamente.

Ele deverá ser determinado através de testes físicos.