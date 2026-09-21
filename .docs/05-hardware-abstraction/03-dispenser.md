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

typedef struct {
    void (*start)(void);
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
PC       → dispenser (simulada, em src/hardware/simulated/)
ESP32    → dispenser (real, em src/hardware/esp32/)
```

---

## 20. Responsabilidades do `Dispenser`

O domínio precisa basicamente de três operações:

```text
start()
stop()
is_active()
```

### `start()`

Solicita que o mecanismo comece a liberar ração.

### `stop()`

Solicita que o mecanismo interrompa a liberação.

### `is_active()`

Informa se o mecanismo está considerado ativo.

---

## 21. Implementação simulada do dispenser

Arquivo:

```text
src/hardware/simulated/simulated_dispenser.c
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

## 23. Futuro `Dispenser` baseado em SG90

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

A implementação real poderá cuidar de:

* inicialização do PWM;
* configuração do pino;
* posição inicial;
* posição de abertura;
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
start()
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