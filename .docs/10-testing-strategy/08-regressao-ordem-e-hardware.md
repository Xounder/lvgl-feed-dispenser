# Regressão, ordem de testes e hardware físico

> _Voltar ao índice: [`../10-testing-strategy.md`](../10-testing-strategy.md)._

---

## 59. Testes de regressão

Toda mudança relevante deve preservar os cenários anteriormente aprovados.

Por exemplo, depois de implementar timeout, ainda devem funcionar:

```text
dosagem normal
cancelamento
nova dosagem
reset
completed
```

Adicionar uma proteção não deve quebrar o fluxo normal.

---

## 60. Ordem recomendada de testes

Para uma alteração de código, a sequência recomendada é:

### Nível 1 — Build

```text
compila?
```

### Nível 2 — Inicialização

```text
abre?
```

### Nível 3 — Fluxo básico

```text
Home → Config → Dosing
```

### Nível 4 — Cenário normal

```text
Dosing → Completed
```

### Nível 5 — Cancelamento

```text
Dosing → Interrupted → Home
```

### Nível 6 — Repetição

```text
Nova dosagem
```

### Nível 7 — Casos de erro

```text
sensor parado
timeout
overshoot
falha de leitura
```

---

## 61. Testes antes do hardware

Antes de migrar para o ESP32-S3, o simulador deve ser capaz de validar pelo menos:

```text
✓ inicialização
✓ navegação
✓ configuração
✓ dosagem normal
✓ reset
✓ cancelamento
✓ conclusão
✓ nova dosagem
✓ estado do dispenser
✓ atualização do peso
```

Idealmente também:

```text
✓ timeout
✓ sensor sem progresso
✓ leitura inválida
✓ overshoot
✓ configuração inválida
```

---

## 62. Testes no hardware real

Quando o hardware estiver disponível, os testes deverão ser repetidos considerando fatores físicos.

A sequência recomendada é gradual:

```text
ESP32-S3
 ↓
display
 ↓
touch
 ↓
HX711
 ↓
load cell
 ↓
calibração
 ↓
leitura de peso
 ↓
servo
 ↓
mecanismo
 ↓
ração
```

Não é recomendável conectar todos os componentes e executar a dosagem completa como primeiro teste.

---

## 63. Teste inicial do HX711

Antes da dosagem automática:

```text
HX711
 ↓
load cell
 ↓
leitura
```

deve ser validado isoladamente.

Testar:

```text
tara
peso conhecido
estabilidade
ruído
repetibilidade
```

---

## 64. Teste inicial do dispenser

O SG90/mecanismo deve ser testado sem depender imediatamente da lógica completa.

Validar:

```text
posição inicial
posição de abertura
posição de fechamento
tempo de movimento
```

Depois integrar ao controller.

---

## 65. Teste físico sem ração

Antes de colocar ração:

```text
dispenser
 ↓
mecanismo
 ↓
recipiente
```

deve ser testado sem material.

Isso reduz o risco de:

* travamento;
* excesso de material;
* movimentação inesperada;
* problemas mecânicos.

---

## 66. Teste físico com pequenas quantidades

Depois:

```text
pequena quantidade
```

pode ser utilizada para observar:

```text
vazão
overshoot
tempo
estabilidade
```

Somente depois devem ser executadas dosagens maiores.

---

## 67. Critério de segurança

Em qualquer teste físico, a prioridade deve ser:

```text
parar o mecanismo
```

antes de otimizar:

```text
precisão
velocidade
aparência
```

A lógica de erro deve sempre possuir um caminho para colocar o dispenser em estado seguro.