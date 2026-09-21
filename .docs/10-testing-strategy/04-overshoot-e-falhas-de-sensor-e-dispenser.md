# Overshoot e falhas de sensor e dispenser

> _Voltar ao índice: [`../10-testing-strategy.md`](../10-testing-strategy.md)._

---

## 28. Excesso de peso

Outro cenário importante é:

```text
target = 100 g
current = 105 g
```

A condição:

```text
current_weight >= target_grams
```

já considera a operação concluída.

Porém, fisicamente, ultrapassar a meta pode ser relevante.

---

## 29. Overshoot

Em um sistema físico, o peso pode não parar exatamente na meta.

Por exemplo:

```text
Meta: 100 g

98 g
99 g
100 g
102 g
```

Isso pode acontecer devido a:

* inércia;
* ração já em queda;
* atraso do sensor;
* tempo de resposta do servo;
* mecânica do dispenser.

Por isso, o teste deve registrar o peso final.

---

## 30. Critério futuro para excesso

Ainda não existe um limite definitivo no projeto.

Uma futura política poderá distinguir:

```text
target = 100 g
final = 101 g
```

de:

```text
target = 100 g
final = 140 g
```

O segundo caso pode representar uma falha significativa.

O limite deverá ser definido após observar o comportamento real do mecanismo.

---

## 31. Sensor com leitura inválida

O hardware real também poderá produzir leituras inválidas.

Exemplos:

```text
erro de leitura
valor impossível
desconexão
sensor indisponível
```

O domínio não deve assumir que toda leitura recebida é necessariamente válida.

Uma futura abstração poderá representar:

```text
leitura válida
leitura inválida
falha do sensor
```

---

## 32. Peso negativo

Dependendo da implementação do sensor e da calibração, podem surgir valores abaixo de zero.

Exemplo:

```text
-5 g
```

O comportamento esperado deve ser definido explicitamente.

Durante a operação normal, uma leitura física inválida não deve fazer o sistema concluir uma dosagem.

---

## 33. Falha do dispenser

Outro cenário futuro:

```text
controller.start()
      ↓
dispenser.start()
      ↓
mecanismo não funciona
```

A aplicação precisa eventualmente distinguir:

```text
comando enviado
```

de:

```text
hardware realmente funcionando
```

A abstração atual não possui feedback físico suficiente para detectar todas essas situações.

Isso deverá ser tratado conforme a eletrônica e o mecanismo forem definidos.

---

## 34. Dispenser ativo após conclusão

Teste de segurança importante:

```text
Meta atingida
      ↓
Completed
```

O dispenser deve estar:

```text
OFF
```

O sistema não pode permitir que a UI esteja em `Completed` enquanto o mecanismo continua ativo.

---

## 35. Dispenser ativo após interrupção ou conclusão

Da mesma forma:

```text
INTERROMPER DOSAGEM / Emergencia fisico
      ↓
Interrupted
```

ou:

```text
Meta atingida
      ↓
Completed
```

deve resultar em:

```text
dispenser = OFF
```

Esse teste deve permanecer mesmo depois da implementação do hardware real.

---

## 36. Interrupção durante dosagem

Deve ser considerado o encerramento inesperado da aplicação durante:

```text
DOSING
```

No PC:

```text
fechar janela
```

No hardware:

```text
reset
queda de energia
watchdog
```

O comportamento seguro esperado para o hardware real deve ser:

```text
reinicialização
      ↓
atuadores em estado seguro
```

A implementação física deve considerar isso explicitamente.