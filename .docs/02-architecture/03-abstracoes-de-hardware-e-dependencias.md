## 03. Abstrações de hardware

[voltar ao índice](../02-architecture.md)

---

### 1. Camada de abstração de hardware

Localização:

```text
src/hardware/
```

Essa camada define contratos para os dispositivos utilizados pela aplicação.

Atualmente existem duas abstrações principais:

```text
WeightSensor
Dispenser
```

Os contratos completos, as implementações simuladas, as implementações reais e a evolução recomendada estão documentados em [05-hardware-abstraction.md](../05-hardware-abstraction.md). Este documento registra apenas o papel de cada abstração sob o ponto de vista da arquitetura.

---

### 2. `WeightSensor`

O `WeightSensor` representa qualquer fonte capaz de fornecer o peso atual.

A função principal para o domínio é:

```text
read_grams()
```

que representa:

> "Qual é o peso atual?"

#### 2.1 Implementação simulada

No PC:

```text
WeightSensor
      ↓
weight_sensor (simulada)
```

Essa implementação mantém um valor interno de peso e o incrementa durante a simulação, por exemplo de `0 g` até `100 g`.

Detalhes: [05-hardware-abstraction.md](../05-hardware-abstraction.md).

#### 2.2 Implementação futura

No hardware:

```text
WeightSensor
      ↓
HX711 implementation
      ↓
Load Cell
```

O restante do domínio não precisa conhecer os detalhes da comunicação com o HX711.

---

### 3. `Dispenser`

O `Dispenser` representa o mecanismo responsável por liberar a ração.

O domínio pode trabalhar conceitualmente com:

```text
start()
stop()
is_active()
```

sem conhecer como o mecanismo é implementado.

#### 3.1 Implementação simulada

No PC:

```text
Dispenser
    ↓
dispenser (simulada)
```

A implementação mantém apenas um estado:

```text
active = 0
```

ou:

```text
active = 1
```

Assim é possível simular:

```text
start()
   ↓
ativo
   ↓
peso aumenta
   ↓
meta atingida
   ↓
stop()
   ↓
inativo
```

Detalhes: [05-hardware-abstraction.md](../05-hardware-abstraction.md).

#### 3.2 Implementação real futura

No dispositivo:

```text
Dispenser
    ↓
Servo / Atuador
    ↓
mecanismo físico
    ↓
ração
```

A implementação poderá utilizar:

* GPIO;
* PWM;
* temporização;
* controle de posição;
* lógica específica do mecanismo.

Esses detalhes devem permanecer fora do domínio.

---

### 4. Hardware simulado e hardware real

O projeto deve manter a seguinte ideia:

```text
                  Interface
                     │
          ┌──────────┴──────────┐
          │                     │
     Simulated               Real
          │                     │
          ▼                     ▼
   PC / SDL2             ESP32-S3
```

Exemplo:

```text
               WeightSensor
                    │
             ┌──────┴──────┐
             ▼             ▼
 simulated_weight     hx711_weight
```

O domínio deve trabalhar com o conceito de sensor, não com a implementação específica.

A substituição das implementações durante a migração é tratada em [05-hardware-abstraction.md](../05-hardware-abstraction.md) e em [11-migration-pc-to-esp32.md](../11-migration-pc-to-esp32.md).