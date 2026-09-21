# Interrupção, timeout e falta de progresso

> _Voltar ao índice: [`../10-testing-strategy.md`](../10-testing-strategy.md)._

---

## 20. Teste de interrupção

O usuário deve conseguir interromper uma dosagem em andamento.

### Cenário

```text
Dosing
peso = 40 g
dispenser = ativo
```

Pressionar:

```text
Parar
```

ou:

```text
Emergencia
```

(RS11/RS12)

### Esperado

```text
dispenser.stop()
         ↓
state = INTERRUPTED
         ↓
InterruptedScreen (massa parcial preservada)
```

---

## 21. Critério de segurança da interrupção

A interrupção não deve apenas trocar de tela.

É necessário garantir:

```text
Parar / Emergencia
      ↓
dispenser.stop()
```

Isso é especialmente importante quando o simulador for substituído pelo SG90 ou outro mecanismo físico.

A troca de tela sozinha não pode representar a parada física.

A interrupção tem **prioridade** sobre o controle automático: mesmo no meio de uma etapa rápida, o dispenser deve parar (RS12).

---

## 22. Teste de interrupção repetida

Também deve ser considerado:

```text
Dosing
 ↓
Parar / Emergencia
 ↓
Interrupted
 ↓
(repetir comando de interrupção)
```

A aplicação não deve apresentar crash ou comportamento inesperado.

Eventos de interrupção devem ser tratados de forma segura mesmo quando o sistema já estiver parado (o controller ignora o comando quando não está em `DOSING`).

---

## 23. Sensor parado

Esse é um dos principais cenários futuros.

### Situação

O dispenser está ativo:

```text
dispenser = ON
```

mas o sensor não registra aumento de peso:

```text
0 g
0 g
0 g
0 g
...
```

Isso pode representar situações como:

* reservatório vazio;
* mecanismo travado;
* ração não caindo;
* load cell com problema;
* HX711 sem atualização;
* conexão física defeituosa.

---

## 24. Problema do sensor parado

A implementação atual ainda não possui uma política completa para esse caso.

Hoje o controller espera:

```text
peso >= target
```

Se o peso nunca aumentar:

```text
peso < target
```

para sempre.

Isso significa que uma política de timeout/progresso precisa ser adicionada antes da integração física definitiva.

---

## 25. Estratégia de falta de progresso

Uma futura implementação deve acompanhar algo semelhante a:

```text
último peso
último instante de mudança
```

Exemplo conceitual:

```text
peso = 20 g
      ↓
sensor continua em 20 g
      ↓
tempo passa
      ↓
limite de inatividade atingido
      ↓
ERROR
```

---

## 26. Timeout

O timeout deve impedir que a aplicação permaneça indefinidamente em:

```text
DOSING
```

Exemplo:

```text
Início
 ↓
dispenser ON
 ↓
peso não chega à meta
 ↓
timeout
 ↓
dispenser OFF
 ↓
ERROR
```

O tempo exato ainda deve ser definido de acordo com o comportamento do mecanismo físico.

---

## 27. Timeout global versus falta de progresso

São conceitos diferentes.

### Timeout global

Limita o tempo total da operação.

```text
Início
 ↓
tempo máximo excedido
 ↓
ERROR
```

### Timeout de progresso

Verifica se o peso está evoluindo.

```text
peso = 40 g
 ↓
nenhuma mudança durante período limite
 ↓
ERROR
```

Idealmente, o sistema pode utilizar ambos.