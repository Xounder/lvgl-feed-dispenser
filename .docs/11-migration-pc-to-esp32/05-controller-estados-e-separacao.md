## 05. DosingController, estados e separação

[voltar ao índice](../11-migration-pc-to-esp32.md)

---

### 26. DosingController

O `DosingController` é uma das partes que devem permanecer conceitualmente iguais.

Atualmente ele coordena:

```text
WeightSensor
Dispenser
```

e decide quando:

```text
iniciar
continuar
parar
concluir
cancelar
```

Essa responsabilidade não pertence ao SDL2 nem ao ESP32.

As responsabilidades do controller são canônicas em [04-domain-and-state-machine.md](../04-domain-and-state-machine.md).

---

### 27. Exemplo da migração

No PC:

```text
dosing_controller
       │
       ├── weight_sensor (simulada)
       └── dispenser (simulada)
```

No ESP32:

```text
dosing_controller
       │
       ├── real_weight_sensor
       └── real_dispenser
```

O objetivo é que a regra:

```text
peso >= meta → parar → concluir
```

continue existindo independentemente da plataforma.

---

### 28. Estados

A máquina de estados deve permanecer conceitualmente independente da plataforma.

A arquitetura planejada é:

```text
IDLE
 ↓
SELECT_MODE
 ↓
CONFIGURING
 ↓
DOSING
 ├──→ COMPLETED
 └──→ ERROR
```

Atualmente a implementação possui:

```text
IDLE
DOSING
COMPLETED
```

A migração para o ESP32 não deve ser usada como motivo para misturar estado de hardware com estado de UI.

A máquina de estados e suas regras são canônicas em [04-domain-and-state-machine.md](../04-domain-and-state-machine.md).

---

### 29. Exemplo do que não fazer

Evitar algo como:

```c
if (esp32) {
    ...
}

if (hx711) {
    ...
}

if (servo) {
    ...
}
```

espalhado pelo domínio.

Isso faria o controller depender diretamente da plataforma.

A intenção é:

```text
Domain
  ↓
Hardware abstraction
  ↓
Real implementation
```

---

### 30. Exemplo de separação desejada

Conceitualmente:

```text
DosingController
      │
      ▼
WeightSensor
      │
      └── real_weight_sensor
                │
                └── HX711
```

e:

```text
DosingController
      │
      ▼
Dispenser
      │
      └── real_dispenser
                │
                └── SG90
```