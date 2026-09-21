## 1. Próximo passo imediato

_Voltar ao índice: [`../12-roadmap.md`](../12-roadmap.md)._

---

O próximo passo deve ser consolidar o comportamento do simulador antes de iniciar a migração física.

A prioridade é transformar o fluxo atualmente funcional em uma base mais robusta para receber cenários de falha.

O próximo ciclo deve focar principalmente em:

```text
1. melhorar o comportamento do domínio
2. tratar estados de erro
3. adicionar timeout
4. simular sensor sem progresso
5. tratar excesso de peso
6. fortalecer os testes
```

---

## 2. Estado de erro

Atualmente a máquina de estados implementada é:

```text
IDLE
DOSING
COMPLETED
INTERRUPTED
```

A evolução planejada é:

```text
IDLE
 ↓
SELECT_MODE
 ↓
CONFIGURING
 ↓
DOSING
 ├──→ COMPLETED
 ├──→ INTERRUPTED  (já implementado)
 └──→ ERROR
```

O estado `ERROR` deverá representar uma falha da operação de dosagem.

---

## 3. Timeout

Uma das próximas funcionalidades importantes é impedir que o sistema fique indefinidamente em:

```text
DOSING
```

Exemplo:

```text
dispenser ativo
       ↓
peso não aumenta
       ↓
tempo limite
       ↓
dispenser.stop()
       ↓
ERROR
```

O valor do timeout deverá ser definido com base no comportamento esperado do mecanismo.

---

## 4. Sensor sem progresso

A simulação deverá futuramente permitir representar:

```text
dispenser ativo
peso parado
```

Por exemplo:

```text
0 g
0 g
0 g
0 g
...
```

O objetivo é testar o comportamento do controller sem precisar produzir fisicamente uma falha.

---

## 5. Overshoot

O simulador também deverá futuramente conseguir representar situações como:

```text
Meta: 100 g

98 g
100 g
104 g
```

Isso permitirá preparar a arquitetura para o comportamento real do dispenser.

O valor aceitável de overshoot não será definido arbitrariamente agora.

Ele deverá ser observado no hardware.

---

## 6. Validação de configuração

O domínio deverá gradualmente assumir maior responsabilidade pela validação.

A intenção é garantir que:

```text
modo Massa:      target_grams > 0
modo Valor (R$): target_money_cents > 0 e price_per_kg_cents > 0
```

e que valores acima da capacidade do sistema possam ser tratados adequadamente.

A UI não deve ser a única responsável pela validade dos dados.

---

## 7. Testes automatizados

Depois que o comportamento do domínio estiver mais estabilizado, poderá ser criada uma camada de testes automatizados.

O foco inicial deve ser:

```text
DosingController
```

e suas regras.

Casos importantes:

```text
✓ start
✓ update
✓ completion
✓ cancel
✓ timeout
✓ sensor parado
✓ overshoot
✓ configuração inválida
```

A UI não precisa estar aberta para validar essas regras.

---

## 8. Simulador de hardware mais realista

A implementação atual é deliberadamente simples.

Futuramente, o simulador poderá representar:

```text
normal
sensor parado
sensor ruidoso
overshoot
atraso
dispenser travado
leitura inválida
```

Isso permitirá testar cenários difíceis antes do hardware.

---

## 9. Organização futura das implementações

Uma evolução possível é organizar:

```text
hardware/
├── weight_sensor.h
├── dispenser.h
│
├── simulated/
│   ├── simulated_weight_sensor.c
│   └── simulated_dispenser.c
│
└── esp32/
    ├── real_weight_sensor.c
    └── real_dispenser.c
```

O objetivo é deixar claro quais implementações pertencem a cada plataforma.

---

## Referências canônicas

O detalhamento técnico dos itens desta fase é tratado em:

- [`04-domain-and-state-machine.md`](../04-domain-and-state-machine.md) — estados, `ERROR`, timeout, validação de configuração e evolução da máquina de estados.
- [`06-simulation-strategy.md`](../06-simulation-strategy.md) — sensor parado, falta de progresso, overshoot e simulador com falhas.
- [`10-testing-strategy.md`](../10-testing-strategy.md) — testes automatizados do `DosingController` e casos de erro.
- [`05-hardware-abstraction.md`](../05-hardware-abstraction.md) — organização das implementações simuladas/reais.
- [`11-migration-pc-to-esp32.md`](../11-migration-pc-to-esp32.md) — organização futura de `src/hardware/` na migração.