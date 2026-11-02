## 19. `Dispenser`

_Voltar ao índice: [`../05-hardware-abstraction.md`](../05-hardware-abstraction.md)._

---

O `Dispenser` representa a capacidade de liberar ou interromper a ração.

Interface atual:

```c
#ifndef DISPENSER_H
#define DISPENSER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DISPENSER_FLOW_FAST,
    DISPENSER_FLOW_FINE,
    DISPENSER_FLOW_MANUAL
} DispenserFlow;

typedef struct {
    void (*start)(DispenserFlow flow);
    void (*stop)(void);
    int (*is_active)(void);
} Dispenser;

extern Dispenser dispenser;

#ifdef __cplusplus
}
#endif

#endif
```

A instância exposta pela plataforma (`dispenser`) é compartilhada entre domínio e UI:

```text
PC       → dispenser (simulada, em src_pc/hardware/simulated/)
ESP32    → dispenser (real, em src/hardware/esp32/)
```

---

## 20. Responsabilidades do `Dispenser`

O domínio precisa basicamente de três operações:

```text
start(flow)
stop()
is_active()
```

### `start(flow)`

Solicita abertura no perfil `FAST`, `FINE` ou `MANUAL`. Pode também ajustar
um dispenser já ativo para outro perfil, sem precisar fechar antes.
O domínio informa a intenção de vazão; ângulos permanecem no driver.
A implementação real evita reescrever a mesma posição para o mesmo perfil
enquanto ativo. Após `stop()`, um novo `start(flow)` volta a comandar abertura.

### `stop()`

Solicita que o mecanismo interrompa a liberação.

### `is_active()`

Informa se o mecanismo está considerado ativo.

---

## 21. Implementação simulada do dispenser

Arquivo:

```text
src_pc/hardware/simulated/simulated_dispenser.c
```

O simulador mantém:

```c
static int active = 0;
```

Ao iniciar:

```c
active = 1;
```

Ao parar:

```c
active = 0;
```

A consulta simplesmente retorna o estado.

---

## 22. Relação entre dispenser e sensor simulado

Na simulação atual, o controller usa o estado do dispenser para determinar quando o peso deve aumentar.

Conceitualmente:

```text
Dispenser ativo
      ↓
ração sendo liberada
      ↓
peso aumenta
```

Isso é uma simplificação deliberada.

No mundo real, o dispenser não controla diretamente o valor do sensor.

A relação física real será:

```text
Dispenser
   ↓
ração cai
   ↓
célula de carga detecta aumento
   ↓
HX711
   ↓
WeightSensor
```

Essa diferença deve ser preservada conceitualmente.

---

## 23. `Dispenser` real baseado em servo

O primeiro atuador considerado é o:

```text
SG90
```

ou outro mecanismo/atuador equivalente, caso os testes mecânicos indiquem necessidade.

A cadeia será aproximadamente:

```text
DosingController
       │
       ▼
Dispenser
       │
       ▼
Servo implementation
       │
       ▼
PWM
       │
       ▼
SG90
       │
       ▼
mecanismo mecânico
       │
       ▼
ração
```

---

## 24. Responsabilidades da implementação do servo

A implementação real cuida de:

* inicialização do PWM;
* configuração do pino;
* posição inicial;
* posições de abertura rápida, fina e manual;
* posição de fechamento;
* movimentação;
* temporização;
* limites;
* estado do atuador.

O controller não deve precisar saber:

```text
qual GPIO
qual duty cycle
qual timer
qual frequência PWM
qual biblioteca do ESP32
```

Ele apenas solicita:

```text
start(DISPENSER_FLOW_FAST)
start(DISPENSER_FLOW_FINE)
start(DISPENSER_FLOW_MANUAL)
stop()
```

---

## 25. Mecanismo físico

O SG90 é apenas o atuador.

Ele não define sozinho como a ração será liberada.

O mecanismo poderá envolver:

```text
servo
   ↓
comporta / porta / mecanismo
   ↓
reservatório
   ↓
ração
```

A geometria mecânica ainda pode mudar durante os testes.

Por isso, o software deve evitar assumir detalhes mecânicos prematuramente.

---

## 26. `start()` não significa necessariamente "servo girando continuamente"

No simulador atual, `start()` significa:

```text
dispenser ativo
```

No hardware real, a implementação poderá interpretar isso de maneiras diferentes.

Por exemplo:

```text
start()
    ↓
mover servo para posição aberta
```

ou:

```text
start()
    ↓
iniciar ciclo de abertura/fechamento
```

dependendo do mecanismo escolhido.

A abstração representa a intenção:

> iniciar a liberação de ração.

A implementação define como isso acontece.

---

## 27. `stop()` e estado seguro

O `stop()` deve colocar o mecanismo em uma condição segura.

No mínimo:

```text
não liberar ração intencionalmente
```

No hardware, isso poderá significar:

```text
servo → posição fechada
```

O comportamento exato dependerá do mecanismo.

Os ângulos são definidos em `src/hardware/esp32/board_config.h`: fechado
180°, rápido 0°, fino 150° e manual 0°. O valor fino é inicial, não calibrado.
O driver verifica em compilação que ele está entre fechado e rápido,
inclusive para montagens cujo sentido de abertura seja invertido.
Parada/conclusão solicitam fechamento diretamente, sem rampa nem espera.
`is_active()` representa estado comandado, não confirmação física de fluxo.
