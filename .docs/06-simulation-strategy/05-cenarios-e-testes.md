# Cenários e testes

## 29. Cenários determinísticos

Uma característica importante do simulador deve ser a possibilidade de reproduzir exatamente um cenário.

Por exemplo:

```text
Scenario:
target = 100 g
flow_rate = 5 g/s
noise = 0
overshoot = 0
sensor_failure = false
dispenser_failure = false
```

O resultado deve ser previsível.

Isso facilita:

* depuração;
* testes;
* comparação entre versões;
* reprodução de bugs.

---

## 30. Cenários aleatórios

Depois que os cenários determinísticos estiverem funcionando, a simulação poderá oferecer variação aleatória.

Por exemplo:

```text
fluxo:
5–8 g/s

ruído:
±0,5 g

overshoot:
0–3 g
```

Isso pode ajudar a encontrar comportamentos que não aparecem em um único cenário fixo.

Entretanto, o uso de aleatoriedade deve permitir definir uma `seed`.

Assim, um problema encontrado pode ser reproduzido.

---

## 31. Seed de simulação

Conceitualmente:

```text
seed = 12345
```

produziria sempre a mesma sequência pseudoaleatória.

Isso permite:

```text
executar cenário
      ↓
encontrar bug
      ↓
salvar seed
      ↓
executar novamente
      ↓
reproduzir bug
```

Essa característica é especialmente útil para testes de comportamento.

---

## 32. Cenários recomendados

A estratégia futura pode incluir pelo menos:

| Cenário              | Objetivo                         |
| -------------------- | -------------------------------- |
| Normal               | Dosagem padrão                   |
| Rápido               | Fluxo alto                       |
| Lento                | Fluxo baixo                      |
| Overshoot pequeno    | Simular atraso mecânico          |
| Overshoot grande     | Testar limite                    |
| Sem fluxo            | Detectar ausência de progresso   |
| Sensor travado       | Testar falha do sensor           |
| Sensor inválido      | Testar erro de leitura           |
| Dispenser não inicia | Testar falha de acionamento      |
| Dispenser não para   | Testar falha crítica             |
| Cancelamento         | Verificar interrupção            |
| Timeout              | Verificar encerramento por tempo |
| Ruído                | Testar estabilidade              |
| Ruído + overshoot    | Cenário mais próximo do real     |

---

## 33. Simulação da quantidade de porções

O modo de porções também deve ser testável no simulador.

Por exemplo:

```text
porções = 3
```

Se cada porção tiver:

```text
100 g
```

o comportamento esperado poderá ser:

```text
porção 1 → 100 g
porção 2 → 100 g
porção 3 → 100 g
```

A forma exata como as porções serão definidas e acumuladas pertence ao domínio.

A simulação deve apenas fornecer o comportamento físico necessário.