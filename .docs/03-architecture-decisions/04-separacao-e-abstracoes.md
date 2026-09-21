## 04. Separação do domínio e abstrações

[voltar ao índice](../03-architecture-decisions.md)

---

### 1. Decisão: separar domínio e hardware

**Decisão**

A lógica de dosagem não deve depender diretamente dos drivers físicos.

Foram criadas abstrações como:

```text
WeightSensor
Dispenser
```

**Motivo**

O controller precisa responder a conceitos como:

```text
"qual é o peso?"
"o dispenser está ativo?"
"inicie a liberação"
"pare a liberação"
```

Ele não precisa saber:

```text
"qual GPIO controla o servo?"
"qual registrador do HX711 devo ler?"
"qual biblioteca de PWM está sendo usada?"
```

Esses detalhes são específicos da implementação.

Status: **Atual**.

---

### 2. Decisão: utilizar interfaces de hardware

As interfaces atuais utilizam `structs` com ponteiros para funções.

Exemplo conceitual:

```c
typedef struct {
    int (*read_grams)(void);
    void (*reset)(void);
} WeightSensor;
```

Isso permite representar diferentes implementações através do mesmo contrato.

Por exemplo:

```text
              WeightSensor
                   │
          ┌────────┴────────┐
          ▼                 ▼
 simulated_weight_sensor  real_weight_sensor
 (simulada, PC)           (HX711, ESP32)
```

O mesmo princípio vale para o dispenser:

```text
                Dispenser
                    │
           ┌────────┴────────┐
           ▼                 ▼
 simulated_dispenser    real_dispenser
 (simulada, PC)         (SG90, ESP32)
```

Status: **Atual**.

Interfaces completas: [05-hardware-abstraction.md](../05-hardware-abstraction.md).

---

### 3. Por que não acessar o hardware diretamente?

Uma implementação como:

```c
read_hx711();
```

diretamente dentro do `DosingController` criaria uma dependência específica.

Isso faria com que:

```text
DosingController
        ↓
HX711
```

fosse obrigatório.

No simulador, seria necessário criar uma série de condicionais ou substituir o código.

Com a abstração:

```text
DosingController
        ↓
WeightSensor
        ↓
Simulated / Real
```

a troca acontece na implementação.

Status: **Atual**.

---

### 4. Decisão: simular o sensor de peso

**Decisão**

Antes de possuir a leitura real da célula de carga, o sistema utiliza um sensor de peso simulado.

Atualmente o peso pode ser incrementado artificialmente:

```text
0 g
2 g
4 g
6 g
...
```

**Motivo**

O objetivo inicial é validar a regra:

```text
se peso >= objetivo
    parar dispenser
    concluir dosagem
```

Não é necessário possuir uma célula de carga física para validar essa regra.

**Benefício**

É possível testar rapidamente:

```text
100 g
200 g
500 g
```

sem alterar hardware.

Também será possível posteriormente simular condições como:

```text
peso não aumenta
peso aumenta lentamente
peso aumenta rapidamente
peso ultrapassa o objetivo
leitura inválida
sensor travado
```

Status: **Atual**. Detalhes: [06-simulation-strategy.md](../06-simulation-strategy.md).

---

### 5. Decisão: simular o dispenser

**Decisão**

O dispenser também possui uma implementação simulada.

Atualmente ele possui essencialmente um estado:

```text
active
```

com operações:

```text
start()
stop()
is_active()
```

**Motivo**

O controller precisa ser capaz de executar:

```text
iniciar dispenser
        ↓
monitorar peso
        ↓
atingir objetivo
        ↓
parar dispenser
```

Isso pode ser validado sem servo físico.

Status: **Atual**. Detalhes: [05-hardware-abstraction.md](../05-hardware-abstraction.md).

---

### 6. Decisão: não antecipar abstrações sem necessidade

Embora a arquitetura utilize abstrações, isso não significa que tudo deva ser transformado em uma camada abstrata imediatamente.

Uma abstração deve existir quando houver uma razão concreta.

Por exemplo:

```text
WeightSensor
```

possui uma razão clara:

```text
Simulação
        vs
HX711 real
```

Da mesma forma:

```text
Dispenser
```

possui:

```text
Simulado
        vs
Servo/atuador real
```

Criar abstrações para conceitos que ainda não possuem variações reais pode apenas aumentar a complexidade.

Status: **Atual**.

---

### 7. Decisão: o hardware real não deve redefinir o domínio

Quando o HX711 e o servo forem integrados, a tendência natural pode ser adaptar toda a aplicação aos detalhes dos drivers.

Isso deve ser evitado.

A pergunta deve ser:

```text
Qual capacidade o domínio precisa?
```

e não:

```text
Como o HX711 funciona?
```

Por exemplo, o domínio precisa de:

```text
peso atual em gramas
```

A implementação do sensor pode internamente lidar com:

```text
ADC/raw value
tare
calibration factor
filter
HX711 protocol
```

Esses detalhes não precisam vazar para o controller.

Status: **Possível** (diretriz para a integração futura).

---

### 8. Decisão: manter o hardware substituível

A arquitetura deve permitir pelo menos:

```text
PC
 └── simulated hardware
```

e:

```text
ESP32-S3
 └── real hardware
```

Idealmente, também deve ser possível criar implementações alternativas para testes.

Exemplo:

```text
WeightSensor
├── SimulatedWeightSensor
├── HX711WeightSensor
└── TestWeightSensor
```

O terceiro caso não é uma obrigação imediata, mas demonstra a finalidade da abstração.

Status: **Possível** (implementações alternativas de teste).