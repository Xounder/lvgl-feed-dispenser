## 36. Evolução recomendada do `WeightSensor`

_Voltar ao índice: [`../05-hardware-abstraction.md`](../05-hardware-abstraction.md)._

---

A interface atual é suficiente para o simulador inicial.

Uma possível evolução é separar:

```text
operações comuns
```

de:

```text
operações específicas da simulação
```

Por exemplo, conceitualmente:

```text
WeightSensor
├── read_grams()
└── tare()
```

enquanto a simulação poderia possuir recursos adicionais internamente:

```text
SimulatedWeightSensor
├── add_grams()
├── set_weight()
├── set_failure()
└── reset()
```

Isso deixa o contrato comum mais próximo do que o hardware real realmente oferece.

---

## 37. Evolução recomendada do `Dispenser`

A interface inicial:

```text
start()
stop()
is_active()
```

é suficiente para validar o fluxo.

Posteriormente, dependendo do mecanismo, poderá ser necessário representar:

```text
posição
velocidade
modo
falha
estado de movimento
```

Mas essas capacidades não devem ser adicionadas até que o mecanismo físico realmente exija isso.

---

## 38. Composição das implementações

O objetivo futuro é permitir escolher a implementação adequada para cada ambiente.

No simulador:

```text
WeightSensor → simulated_weight_sensor
Dispenser    → simulated_dispenser
```

No ESP32:

```text
WeightSensor → hx711_weight_sensor
Dispenser    → servo_dispenser
```

O restante da aplicação deve permanecer o mais estável possível.

---

## 39. Regra de seleção da implementação

A seleção da implementação deve acontecer em uma camada de composição/inicialização, e não dentro das regras do domínio.

Evitar:

```c
if (running_on_pc) {
    ...
} else {
    ...
}
```

espalhado pelo controller.

Preferir conceitualmente:

```text
inicialização da plataforma
        ↓
seleciona implementação
        ↓
controller recebe/utiliza abstrações
```

---

## 40. Hardware abstraction e migração

A abstração existe principalmente para facilitar a transição:

```text
PC
 ↓
SimulatedWeightSensor
SimulatedDispenser
```

para:

```text
ESP32-S3
 ↓
HX711WeightSensor
ServoDispenser
```

Sem a abstração, a migração exigiria alterar diretamente o controller.

Com a abstração, a mudança fica concentrada nas implementações de hardware e na composição da aplicação.

---

## 41. Testes independentes

A arquitetura também permite testar componentes separadamente.

### Teste do sensor simulado

```text
reset
 ↓
read = 0
 ↓
add 10
 ↓
read = 10
```

### Teste do dispenser simulado

```text
stop
 ↓
is_active = false

start
 ↓
is_active = true
```

### Teste do controller

```text
target = 100
weight = 0
dispenser = ativo
 ↓
updates
 ↓
weight >= 100
 ↓
COMPLETED
```

Cada camada pode ser validada de maneira independente.

---

## 42. Segurança do estado físico

Uma preocupação importante na integração real é que o estado do software e o estado físico não fiquem inconsistentes.

Por exemplo:

```text
software:
DOSING

hardware:
servo parado
```

ou:

```text
software:
COMPLETED

hardware:
servo ainda aberto
```

A arquitetura deve evoluir para reduzir essas possibilidades.

Uma regra fundamental é:

```text
Ao finalizar, interromper ou entrar em erro:
        ↓
garantir dispenser em estado seguro
```

Os estados `DOSING`/`COMPLETED` são definidos na máquina de estados do domínio (ver [`../04-domain-and-state-machine.md`](../04-domain-and-state-machine.md)).

---

## 43. Inicialização

A inicialização futura poderá seguir aproximadamente:

```text
platform init
      ↓
sensor init
      ↓
dispenser init
      ↓
domain init
      ↓
UI init
      ↓
application loop
```

No simulador atual, essa sequência é simplificada.

No hardware real, a ordem poderá ser importante.

---

## 44. Falha durante inicialização

No futuro, uma implementação física poderá falhar antes mesmo de uma dosagem começar.

Exemplos:

```text
HX711 não detectado
servo não inicializado
display indisponível
touch indisponível
```

A arquitetura deverá permitir representar essas falhas separadamente de erros que acontecem durante a dosagem.

Não é necessário implementar essa distinção imediatamente.

---

## 45. Relação com o domínio

O domínio deve poder operar conceitualmente assim:

```text
controller.start()
       ↓
sensor.tare()
       ↓
dispenser.start()
       ↓
controller.update()
       ↓
sensor.read_grams()
       ↓
comparar objetivo
       ↓
dispenser.stop()
```

O domínio não precisa saber como:

```text
tare()
```

é implementado.

---

## 46. Regra para novas abstrações

Antes de criar uma nova abstração de hardware, verificar:

1. Existe mais de uma implementação?
2. Existe uma implementação simulada e uma real?
3. O domínio realmente precisa dessa capacidade?
4. O detalhe pode permanecer encapsulado dentro de outra implementação?
5. A nova abstração reduz acoplamento ou apenas adiciona complexidade?

Exemplos de abstrações que possuem justificativa clara:

```text
WeightSensor
Dispenser
```

Novas abstrações devem seguir o mesmo critério.

---

## 47. Estado atual

Atualmente o projeto possui:

```text
WeightSensor
    └── simulated_weight_sensor

Dispenser
    └── simulated_dispenser
```

O controller utiliza essas implementações para validar o fluxo da dosagem.

O hardware real ainda não foi integrado.

---

## 48. Próximas evoluções

A evolução planejada da camada de hardware é:

```text
1. Melhorar simulação
        ↓
2. Definir comportamento de falhas
        ↓
3. Integrar HX711
        ↓
4. Integrar Load Cell
        ↓
5. Calibrar peso
        ↓
6. Integrar SG90/atuador
        ↓
7. Validar mecanismo físico
        ↓
8. Ajustar controller conforme comportamento real
```

---

## 49. Princípio final

A abstração de hardware pode ser resumida em:

```text
O domínio conhece capacidades.
A implementação conhece dispositivos.
```

O `DosingController` deve pensar em:

```text
"qual é o peso?"
"comece a liberar"
"pare de liberar"
```

e não em:

```text
"leia o HX711 no GPIO X"
"gere PWM no timer Y"
"mova o SG90 para 70 graus"
```

A implementação física é responsável por transformar essas intenções em operações reais.

Assim, o projeto mantém a separação:

```text
              DOMÍNIO
                 │
        ┌────────┴────────┐
        ▼                 ▼
  WeightSensor         Dispenser
        │                 │
        ▼                 ▼
      HX711             SG90
        │                 │
        ▼                 ▼
   peso físico       mecanismo físico
```

Enquanto o simulador fornece:

```text
              DOMÍNIO
                 │
        ┌────────┴────────┐
        ▼                 ▼
  WeightSensor         Dispenser
        │                 │
        ▼                 ▼
   Simulated          Simulated
        │                 │
        └────────┬────────┘
                 ▼
             ambiente PC
```

Essa separação é uma das bases da estratégia **simulator-first** do projeto e deve ser preservada durante a migração para o ESP32-S3.