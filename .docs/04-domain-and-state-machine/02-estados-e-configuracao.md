## 5. Estado `IDLE`

_Voltar ao índice: [`../04-domain-and-state-machine.md`](../04-domain-and-state-machine.md)._

---

### Significado

O sistema está parado e nenhuma dosagem está sendo executada.

Características:

```text
dispenser = parado
dosagem ativa = não
peso = sem processo ativo
```

O sistema pode receber uma solicitação para iniciar o fluxo de configuração.

---

### Transições possíveis

```text
IDLE
  │
  │ iniciar
  ▼
SELECT_MODE
```

No modelo simplificado atual, parte desse fluxo é representada diretamente pela navegação da UI.

Na implementação atual, `dosing_controller_start()` parte diretamente de `IDLE` para `DOSING` (executando tara/reset do sensor e iniciando o dispenser na fase `FAST`).

---

## 6. Estado `SELECT_MODE`

### Significado

O usuário está escolhendo como deseja realizar a dosagem.

Os modos previstos são:

```text
Massa (gramas)
Valor (R$)
```

Esse estado ainda é evolução prevista: na implementação atual, a escolha do modo é feita diretamente pela UI e refletida em `DosingConfig.mode`.

---

### Transições

```text
SELECT_MODE
   │
   ├── Massa ──────→ CONFIGURING
   │
   └── Valor (R$) ─→ CONFIGURING
```

O modo selecionado deve ser armazenado para que a tela de configuração saiba quais parâmetros apresentar.

---

## 7. Estado `CONFIGURING`

### Significado

O usuário está configurando os parâmetros necessários para a dosagem.

A configuração atual é representada por:

```c
typedef enum {
    DOSING_MODE_GRAMS,
    DOSING_MODE_CURRENCY
} DosingMode;

typedef struct {
    DosingMode mode;
    int target_grams;          /* modo massa (g) */
    int target_money_cents;    /* modo valor (R$ em centavos) */
    int price_per_kg_cents;    /* preco de referencia por kg (centavos) */
} DosingConfig;
```

---

## 8. Configuração por massa (gramas)

Nesse modo (`DOSING_MODE_GRAMS`), o parâmetro principal é:

```text
target_grams
```

Exemplo:

```text
100 g
```

A interface atual permite:

```text
-10 g
+10 g
```

com valor mínimo de:

```text
10 g
```

---

### Regra

O valor alvo deve ser positivo.

No comportamento atual:

```text
target_grams >= 10
```

A regra exata poderá ser alterada caso os requisitos físicos do produto mudem.

---

## 9. Configuração por valor (R$)

Nesse modo (`DOSING_MODE_CURRENCY`), os parâmetros principais são:

```text
target_money_cents     valor desejado (R$ em centavos)
price_per_kg_cents     preço de referência (R$ por kg em centavos)
```

Exemplo:

```text
valor desejado = R$ 3,00   (300 centavos)
preço          = R$ 12,00/kg (1200 centavos)
```

O preço de referência padrão é:

```text
R$ 12,00/kg
```

---

### Regra de conversão

O valor desejado é convertido em gramas pelo controller:

```text
target_grams = (target_money_cents * 1000) / price_per_kg_cents
```

Exemplo com os valores acima:

```text
(300 * 1000) / 1200 = 250 g
```

Se `price_per_kg_cents <= 0`, não existe preço de referência válido e o alvo convertido é tratado como inválido (`0 g`), impedindo o início da dosagem.

A conversão é centralizada no controller (ver [`04-tabelas-eventos-e-invariantes.md`](04-tabelas-eventos-e-invariantes.md) e [`05-regras-de-borda.md`](05-regras-de-borda.md)).

---

## 10. Início da dosagem

Quando o usuário confirma a configuração:

```text
CONFIGURING
      │
      │ continuar
      ▼
   DOSING
```

Antes de iniciar efetivamente o processo, o controller deve preparar o sistema.

Fluxo conceitual:

```text
receber configuração
        ↓
validar configuração
        ↓
resetar/tarar sensor
        ↓
iniciar dispenser
        ↓
estado = DOSING
```

Na implementação atual, esse fluxo resume-se a `dosing_controller_start()`:

```text
reset/tara do sensor (weight_sensor.reset)
        ↓
estado = DOSING, fase = FAST
        ↓
dispenser ativo
```

---

## 11. Estado `DOSING`

### Significado

O sistema está efetivamente tentando atingir o peso configurado.

Nesse estado:

```text
sensor de peso = sendo monitorado
dispenser = potencialmente ativo
controller = executando controle
```

O processo é iterativo.

Conceitualmente:

```text
ler peso
   ↓
comparar com objetivo
   ↓
objetivo atingido?
   ├── não → continuar
   └── sim → concluir
```