# Falhas, timeout, cancelamento e segurança

## 22. Timeout

O timeout impede que o sistema fique indefinidamente em:

```text
DOSING
```

Um exemplo:

```text
objetivo = 100 g

tempo máximo = 30 s
```

Se após 30 segundos o objetivo não for atingido:

```text
stop dispenser
      ↓
ERROR
```

O valor real do timeout deve ser definido após conhecer o comportamento do mecanismo.

---

## 23. Sensor travado

Um cenário de falha pode ser:

```text
peso inicial = 20 g
```

e o sensor continuar retornando:

```text
20 g
20 g
20 g
20 g
```

mesmo com o dispenser ativo.

Isso permite testar a detecção de ausência de progresso.

---

## 24. Sensor com erro

Uma implementação futura poderá simular:

```text
WeightSensor
    ↓
erro
```

ou:

```text
leitura inválida
```

Nesse caso, o comportamento esperado poderá ser:

```text
parar dispenser
      ↓
ERROR
```

A forma exata de representar esse erro ainda deve ser definida quando a interface do sensor evoluir.

---

## 25. Dispenser travado

Outro cenário:

```text
controller.stop()
```

mas a simulação continua indicando:

```text
is_active() == true
```

Isso permite testar uma situação em que o comando foi enviado, mas o atuador não obedeceu.

No hardware real, esse tipo de falha pode ser difícil de reproduzir deliberadamente.

No simulador, pode ser criado sob demanda.

---

## 26. Dispenser que não inicia

Também pode ser simulado:

```text
start()
```

mas:

```text
is_active() == false
```

Isso representa uma falha de acionamento.

O controller poderá futuramente detectar:

```text
comando de início enviado
+
dispenser não ficou ativo
```

e entrar em erro.

---

## 27. Cancelamento, interrupção e emergência

A interrupção já faz parte do comportamento atual, via:

```text
dosing_controller_cancel()
```

dois comandos na tela de Dosagem a acionam:

```text
[ Parar ]          → comando na tela
[ Emergencia ]     → simulação do botão físico de emergência
```

Ao interromper:

```text
dispenser.stop()
```

e o controller passa para:

```text
INTERRUPTED
```

A massa já dosada é preservada:

```text
massa parcial = peso no momento da interrupção
```

e é exibida na tela `Interrupted`.

### Prioridade da interrupção (RS12)

O comando de interrupção possui prioridade sobre o controle automático:

```text
DOSING (etapa rápida ou fina)
    ↓
Parar / Emergencia
    ↓
dispenser.stop()
    ↓
INTERRUPTED
```

Nenhum tick posterior de `dosing_controller_update()` pode voltar a abrir o dispenser enquanto a interrupção estiver ativa, pois o update só age quando o estado é `DOSING`.

### Relação com o cancelamento anterior

Em versões anteriores, o cancelamento retornava diretamente para `IDLE`. No modelo atual, a interrupção é um estado próprio (`INTERRUPTED`) que preserva a massa parcial e exige um comando explícito para preparar uma nova dosagem (`dosing_controller_new_dosing()`).

A simulação deve preservar esse comportamento em qualquer cenário.

Mesmo se:

```text
peso = 10 g
```

ou:

```text
peso = 90 g
```

a interrupção deve funcionar na etapa rápida e na etapa fina.

---

## 29. Segurança em caso de erro

Todo cenário de erro deve considerar primeiro o atuador.

A sequência desejada é:

```text
detectar erro
    ↓
parar dispenser
    ↓
registrar estado de erro
    ↓
informar UI
```

Evitar:

```text
erro
 ↓
continuar dispenser
```

porque isso poderia representar um comportamento perigoso no hardware real.