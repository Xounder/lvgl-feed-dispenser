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

---

## 6. Estado `SELECT_MODE`

### Significado

O usuário está escolhendo como deseja realizar a dosagem.

Atualmente existem dois modos:

```text
Quantidade fixa
Porções
```

---

### Transições

```text
SELECT_MODE
   │
   ├── Quantidade fixa ──→ CONFIGURING
   │
   └── Porções ──────────→ CONFIGURING
```

O modo selecionado deve ser armazenado para que a tela de configuração saiba quais parâmetros apresentar.

---

## 7. Estado `CONFIGURING`

### Significado

O usuário está configurando os parâmetros necessários para a dosagem.

A configuração atual é representada por:

```c
typedef struct {
    int target_grams;
    int portions;
} DosingConfig;
```

---

## 8. Configuração por quantidade fixa

Nesse modo, o parâmetro principal é:

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

## 9. Configuração por porções

Nesse modo, o parâmetro principal é:

```text
portions
```

Exemplo:

```text
3 porções
```

A interface atual permite:

```text
-1
+1
```

com mínimo de:

```text
1
```

---

### Regra futura

A relação entre:

```text
porções
```

e:

```text
gramas
```

ainda precisa ser definida de forma mais precisa pelo domínio do produto.

Não se deve assumir uma conversão arbitrária sem uma regra de negócio definida.

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