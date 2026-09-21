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

## 33. Modo Valor (R$): conversão monetária como cenário

O modo de dosagem por valor monetário deve ser testável no simulador.

Por exemplo:

```text
modo = Valor (R$)
preço de referência = R$12,00/kg
valor informado = R$3,00
```

o comportamento esperado é:

```text
meta em gramas = (300 × 1000) / 1200 = 250 g
    ↓
etapa rápida inicia (faltam 250 g)
    ↓
etapa fina quando faltam ≤ 30 g
    ↓
peso >= 250 g → COMPLETED
```

Cenários adicionais para o modo Valor:

- **Conversão conferida manualmente:** usar um valor conhecido e verificar a massa equivalente pelo preço de referência.
- **Preço de referência padrão:** confirmar que o padrão é R$12,00/kg.
- **Conversão com frações:** por exemplo R$1,50 → 125 g, verificando o arredondamento inteiro do resultado.

A forma exata de conversão pertence ao domínio; a simulação deve apenas receber a meta convertida e reproduzir a dosagem correspondente.

---

## 34. Liberação manual como cenário

A liberação manual simulada também é um cenário do simulador:

```text
estado = IDLE
    ↓
botão "Liberacao manual" pressionado
    ↓
dispenser.start()
    ↓
+5 g por tick (~200 ms)
    ↓
LED verde aceso (modo manual)
    ↓
botão solto
    ↓
dispenser.stop() e LED apaga
```

Cenário de bloqueio:

```text
estado = DOSING
    ↓
tentar acionar a liberação manual
    ↓
comando ignorado (RS15)
```

A simulação deve garantir que o passo manual só seja adicionado quando:

```text
manual_release_active
+
estado == IDLE
```