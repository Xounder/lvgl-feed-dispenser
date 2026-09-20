## 04. Dispenser, servo e mecanismo

[voltar ao índice](../08-target-hardware.md)

---

### 17. SG90

O SG90 é o atuador considerado inicialmente para o mecanismo de dosagem.

A função do servo será movimentar um mecanismo responsável por controlar a saída da ração.

Conceitualmente:

```text
ESP32-S3
    ↓
PWM
    ↓
SG90
    ↓
mecanismo
    ↓
abertura/fechamento
    ↓
ração
```

---

### 18. O servo não é o dispenser inteiro

É importante distinguir:

```text
SG90
```

de:

```text
Dispenser
```

O SG90 é o atuador.

O `Dispenser` é a abstração de software que representa a capacidade:

```text
liberar ração
parar liberação
```

A implementação pode utilizar um SG90, mas o domínio não deve depender diretamente dele.

---

### 19. Possível mecanismo

Uma montagem inicial pode utilizar:

```text
reservatório
     ↓
abertura
     ↓
comporta / porta
     ↑
    SG90
```

O servo muda a posição da comporta.

Por exemplo:

```text
fechado
   │
   ▼
┌───────┐
│       │
│ração  │
│       │
└───┬───┘
    X
```

e:

```text
aberto
   │
   ▼
┌───────┐
│ração  │
│       │
└───┬───┘
    ↓
   ração
```

O mecanismo exato ainda pode mudar durante os testes.

---

### 20. Posição do servo

A posição do SG90 será definida experimentalmente.

Poderão existir pelo menos duas posições conceituais:

```text
SERVO_CLOSED
SERVO_OPEN
```

Os valores físicos de ângulo não devem ser tratados como regras de negócio.

Por exemplo, o domínio não deve dizer:

```text
mover para 70 graus
```

Ele deve dizer:

```text
start()
```

ou:

```text
stop()
```

A implementação do `Dispenser` converte isso em movimento físico.

---

### 21. Inércia e overshoot

O servo e o mecanismo podem introduzir atraso entre:

```text
comando de parada
```

e:

```text
fim efetivo da liberação
```

Além disso, ração que já está em movimento pode continuar caindo.

Portanto:

```text
comando stop
       ↓
mecanismo fecha
       ↓
ração ainda cai
       ↓
peso aumenta
```

Esse comportamento deverá ser observado durante os testes físicos.

---

[voltar ao índice](../08-target-hardware.md)