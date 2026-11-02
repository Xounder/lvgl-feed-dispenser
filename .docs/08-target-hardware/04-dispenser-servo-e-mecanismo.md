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

O firmware usa posições configuráveis em
`src/hardware/esp32/board_config.h`:

| Parâmetro | Inicial | Comando |
| --- | --- | --- |
| `BOARD_SERVO_CLOSED_ANGLE` | 180° | Parada, conclusão e inicialização |
| `BOARD_SERVO_FAST_ANGLE` | 0° | Dosagem distante da meta |
| `BOARD_SERVO_FINE_ANGLE` | 150° | Dosagem próxima da meta |
| `BOARD_SERVO_MANUAL_ANGLE` | 0° | Liberação manual enquanto pressionado |

**Os valores são pontos de partida de configuração, não posições calibradas
nem uma vazão em g/s.** Para a orientação atual, 150° deixa uma abertura
menor que 0°, pois o fechamento está em 180°. O driver exige que a posição
fina esteja estritamente entre a fechada e a rápida; admite o sentido inverso.
Cada perfil gera um comando quando iniciado ou alterado, sem `delay()`.
O fechamento é solicitado diretamente, inclusive ao interromper.

#### Validação dos ângulos com `static_assert`

Em `src/hardware/esp32/real_dispenser.cpp`, os `static_assert` verificam as
constantes de `board_config.h` **durante a compilação**. Uma condição falsa
interrompe a compilação com a mensagem correspondente. Essas verificações
não são executadas no `loop()` nem acrescentam processamento em operação.

As três primeiras verificações exigem que os ângulos fechado, rápido e
manual estejam no intervalo inclusivo de **0° a 180°**. Por exemplo, fechado
em 200° provoca o erro `Angulo fechado fora da faixa do servo`.

A última verifica se o ângulo fino está **estritamente entre** fechado e
rápido, independentemente do sentido de abertura:

```text
(fino - fechado) * (fino - rapido) < 0
```

Quando fino está entre os extremos, uma diferença é negativa e a outra é
positiva, tornando o produto negativo. Com fechado = 180°, rápido = 0° e
fino = 150°, o resultado é `(-30) * 150 = -4500`: configuração aceita.
No sentido inverso, fechado = 0°, rápido = 180° e fino = 30° também é válido:
`30 * (-150) = -4500`.

Se fino coincidir com um extremo, o produto será zero; se estiver fora do
intervalo, será positivo. Ambos os casos impedem a compilação com a mensagem
`Angulo fino deve ficar entre fechado e rapido`. Essa condição também limita
o ângulo fino à faixa dos extremos já validados.

Os `static_assert` protegem contra configurações numéricas incoerentes.
**Não verificam batentes mecânicos, posição física ou vazão**: a abertura
fina e os limites de movimento ainda precisam de calibração no mecanismo.

### Calibração das aberturas e do limiar

1. Confirmar o fechamento físico sem forçar o batente e ajustar o ângulo fechado.
2. Ajustar a posição rápida para permitir passagem regular sem prender os grãos.
3. Testar a posição fina, inicialmente 150°, e ajustar até manter passagem
   menor, mas contínua. Uma abertura pequena demais pode obstruir a ração.
4. Manter a posição manual independente da fina para não herdar o perfil anterior.
5. Com pesagem funcional, verificar a transição quando faltarem 30 g e medir
   a massa que ainda cai depois do comando de fechamento.
6. Ajustar `FINE_THRESHOLD_GRAMS` em `src/domain/dosing_controller.c` se
   a vazão, o atraso da leitura ou o produto exigirem mais antecedência.

Não se pressupõe proporcionalidade entre ângulo e vazão. O controle em duas
aberturas é a implementação inicial: não utiliza PID nem estima vazão sem
medições. Uma vez na fina, a operação permanece nela até fechar, evitando
reabertura por ruído. Metas pequenas começam na fina. A tara é separada do início.

Logs esperados: `[SERVO] ABRIR/rapida`, `REDUZIR/fina`, `ABRIR/manual`
e `FECHAR/parar`, acompanhados por ângulo/pulso. Esses registros confirmam
comandos emitidos, não torque, fluxo ou precisão. Validar fisicamente no ESP32.

Os valores físicos de ângulo não devem ser tratados como regras de negócio.

Por exemplo, o domínio não deve dizer:

```text
mover para 70 graus
```

Ele deve dizer:

```text
start(DISPENSER_FLOW_FAST)
start(DISPENSER_FLOW_FINE)
start(DISPENSER_FLOW_MANUAL)
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
