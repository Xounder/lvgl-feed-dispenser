## 03. Domínio e regras de dosagem

[voltar ao índice](../13-agent-guide.md)

---

## 17. Estado de negócio ≠ tela

Não assumir que:

```text
SCREEN_DOSING
```

é equivalente a:

```text
DOSING_STATE_DOSING
```

São conceitos diferentes.

Uma tela representa a apresentação.

Um estado representa o comportamento da operação.

A máquina de estados desejada é conceitualmente:

```text
IDLE
 ↓
SELECT_MODE
 ↓
CONFIGURING
 ↓
DOSING
 ├──→ COMPLETED
 ├──→ INTERRUPTED
 └──→ ERROR
```

A enumeração atual é mais simples.

---

## 18. Não aumentar a máquina de estados sem necessidade

Se uma alteração não precisa de um novo estado, não criar um estado apenas para deixar a arquitetura "mais completa".

A evolução deve ser guiada pelo comportamento real.

Adicionar:

```text
ERROR
```

faz sentido quando houver comportamento de erro que precise ser representado.

Adicionar estados apenas por antecipação pode aumentar a complexidade sem benefício imediato.

---

## 19. DosingConfig

A configuração atual é:

```c
typedef enum {
    DOSING_MODE_GRAMS,
    DOSING_MODE_CURRENCY
} DosingMode;

typedef struct {
    DosingMode mode;
    int target_grams;          /* modo massa (g) */
    int target_money_cents;    /* modo valor (R$ em centavos) */
    int price_per_kg_cents;    /* preco de referencia por kg */
} DosingConfig;
```

Ela representa dados da operação.

Não transformar `DosingConfig` em um objeto responsável por:

* criar telas;
* controlar servo;
* atualizar LVGL;
* executar a dosagem.

A configuração deve permanecer simples.

---

## 20. Regras atuais de dosagem

A regra principal atualmente é:

```text
peso atual >= peso alvo
        ↓
parar dispenser
        ↓
COMPLETED
```

Durante a simulação:

```text
dispenser ativo
        ↓
nova leitura (duas etapas)
```

O crescimento por atualização depende da fase:

```text
etapa rápida: +20 g (faltando mais de 30 g)
etapa fina:   +2 g  (faltando 30 g ou menos)
```

A atualização ocorre aproximadamente a cada:

```text
300 ms
```

Portanto, nas etapas rápidas a taxa simulada atual é aproximadamente:

```text
20 g / 0,3 s
≈ 66,7 g/s
```

e na etapa fina:

```text
2 g / 0,3 s
≈ 6,7 g/s
```

Não assumir que essa taxa representa o hardware real.

Ela é apenas um comportamento de simulação.

---

## 21. Não usar a taxa simulada como parâmetro físico

Não concluir que o dispenser real deverá fornecer:

```text
6,7 g/s
```

O mecanismo físico ainda precisará ser medido.

A taxa real dependerá de:

* ração;
* geometria;
* reservatório;
* abertura;
* mecanismo;
* servo;
* gravidade;
* vibração;
* posição.

---

## 22. Interrupção deve ser segura

Atualmente:

```text
Parar / Emergencia
   ↓
dispenser.stop()
   ↓
INTERRUPTED (massa parcial preservada)
   ↓
InterruptedScreen
```

Qualquer alteração na interrupção deve preservar a ideia de:

> **ao interromper uma operação, o atuador deve ser colocado em estado seguro.**

A interrupção tem prioridade sobre o controle automático (RS12).

Isso será ainda mais importante no hardware físico.

---

## 23. Não ignorar o comportamento físico

Ao implementar o hardware real, não assumir que:

```text
stop()
```

significa:

```text
peso para imediatamente.
```

Pode existir:

* ração já em queda;
* inércia;
* atraso mecânico;
* ração acumulada;
* vibração;
* atraso da leitura.

Portanto:

```text
comando de parada
```

e:

```text
estabilização do peso
```

podem ser fenômenos diferentes.

---

## 24. Overshoot é esperado como possibilidade

Exemplo:

```text
Meta: 100 g

98 g
 ↓
acionamento
 ↓
102 g
```

Não corrigir isso artificialmente apenas no simulador.

Primeiro entender o comportamento real.

Depois decidir se será necessário:

* antecipar a parada;
* reduzir vazão;
* criar duas velocidades;
* usar tolerância;
* estabilizar a leitura;
* implementar uma estratégia de aproximação.

---

## 25. Tratamento de erros é uma evolução planejada

O sistema deverá futuramente lidar com situações como:

```text
sensor sem progresso
sensor inválido
dispenser travado
timeout
peso excessivo
```

Mas não inventar uma implementação complexa de erro sem que a tarefa exija.

Quando implementado, o caminho deve ser seguro:

```text
falha
 ↓
parar dispenser
 ↓
ERROR
 ↓
informar usuário
```

---

## 26. Não esconder falhas

Evitar código que transforme uma falha em um estado aparentemente normal.

Por exemplo, não fazer:

```text
sensor inválido
 ↓
usar 0 g silenciosamente
```

se isso puder mascarar um problema real.

É melhor definir explicitamente como a aplicação deve representar:

```text
leitura inválida
```

---

## 27. Validação deve existir além da UI

A UI atualmente limita alguns valores:

```text
modo Massa:      gramas >= 10
modo Valor (R$): valor >= R$ 0,50 e preço de referência > 0
```

Mas essas validações não devem ser consideradas exclusivamente responsabilidade da interface.

O domínio futuramente deverá proteger suas próprias regras.

Isso permite que a mesma regra funcione:

```text
na UI
no simulador
em testes
no ESP32
```

---

## 28. Cuidado com memória

O projeto utiliza LVGL e C.

Antes de adicionar alocações dinâmicas, avaliar:

```text
quem aloca?
quem libera?
qual é o ciclo de vida?
```

Existe atualmente um contexto de configuração criado com:

```c
lv_malloc(...)
```

que ainda não possui um ciclo explícito de liberação.

Não aumentar esse padrão indiscriminadamente.

Em código novo, preferir ciclos de vida claros.