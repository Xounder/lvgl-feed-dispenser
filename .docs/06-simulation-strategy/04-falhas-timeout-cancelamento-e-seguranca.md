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

## 27. Cancelamento

O cancelamento já faz parte do comportamento atual.

Ao cancelar:

```text
dosing_controller_cancel()
```

o dispenser deve parar:

```text
dispenser.stop()
```

e o controller retorna para:

```text
IDLE
```

A simulação deve preservar esse comportamento em qualquer cenário.

Mesmo se:

```text
peso = 10 g
```

ou:

```text
peso = 90 g
```

o cancelamento deve interromper a dosagem.

---

## 28. Segurança em caso de erro

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