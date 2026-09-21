# Fluxos do usuário e princípios

## 57. Fluxo completo do usuário

O fluxo normal é:

```text
┌──────────────┐
│    HOME      │
│              │
│ Massa        │  (seleção de modo na própria Home — TELA 1)
│ Valor (R$)   │
└──────┬───────┘
       │
       ▼
┌──────────────┐
│    CONFIG    │
│              │
│      - +     │
│              │
│   Iniciar    │
└──────┬───────┘
       │
       ▼
┌──────────────┐
│    DOSING    │
│              │
│  Peso: 75 g  │
│  ███████░░░  │
│              │
│ Interromper  │
└──────┬───┬───┘
       │   │ objetivo
       │   │ atingido
       ▼   ▼
┌──────────────┐ ┌──────────────┐
│  COMPLETED   │ │ INTERRUPTED  │
│              │ │              │
│   concluído  │ │  interrompida│
│              │ │              │
│ Nova dosagem │ │ Nova dosagem │
└──────────────┘ └──────────────┘
```

`Nova dosagem` retorna a `HOME` com tara (`dosing_controller_new_dosing()`).

---

## 58. Fluxos alternativos

### Interrupção

```text
DOSING
   ↓
INTERROMPER DOSAGEM (tela) / Emergencia (botao fisico)
   ↓
INTERRUPTED
```

A tela de interrupção oferece:

```text
Nova dosagem → HOME (com tara)
```

### Voltar durante seleção

Como a seleção de modo ocorre na própria Home, não existe mais a tela
intermediária `MODE`.

### Voltar durante configuração

```text
CONFIG
   ↓
Início
   ↓
HOME
```

### Nova dosagem

```text
COMPLETED / INTERRUPTED
   ↓
Nova dosagem
   ↓
HOME (com tara)
```

---

## 59. Estado de erro futuro

Ainda não existe uma tela:

```text
ERROR
```

na implementação atual.

Porém, a arquitetura de estados prevê essa possibilidade.

Uma futura tela de erro poderá apresentar:

```text
Erro na dosagem
```

e informações como:

```text
Sensor sem leitura
Fluxo interrompido
Tempo excedido
Falha no dispenser
```

O importante é que a UI não diagnostique essas situações sozinha.

O domínio/hardware deverá fornecer a informação.

---

## 60. Possível fluxo futuro de erro

```text
DOSING
   ↓
falha detectada
   ↓
DosingController
   ↓
stop dispenser
   ↓
ERROR
   ↓
ErrorScreen
```

A tela poderia então oferecer:

```text
Tentar novamente
```

ou:

```text
Voltar ao início
```

A definição final desse fluxo ainda pertence à evolução futura do projeto.

---

## 61. Regra para futuras telas

Uma nova tela deve ser criada quando existir uma necessidade clara de apresentação ou interação.

Antes de adicionar uma nova tela, perguntar:

1. A informação realmente precisa de uma tela separada?
2. A tela possui uma responsabilidade clara?
3. A navegação faz sentido para o usuário?
4. O estado representado pertence à UI ou ao domínio?
5. A nova tela está adicionando comportamento de negócio indevidamente?

Isso evita transformar a UI em uma máquina de estados paralela ao domínio.

---

## 62. Princípio de navegação

A navegação deve representar o fluxo de utilização do produto:

```text
entrar
  ↓
configurar
  ↓
executar
  ↓
acompanhar
  ↓
concluir
```

Ela não deve ser criada simplesmente para refletir a organização interna do código.

---

## 63. Princípio de independência

A UI deve poder mudar visualmente sem exigir mudanças no controller.

Por exemplo, seria possível trocar:

```text
barra de progresso
```

por:

```text
círculo de progresso
```

sem modificar a regra:

```text
peso >= objetivo
```

Da mesma forma, uma tela de dosagem diferente poderia continuar utilizando:

```text
dosing_controller_get_weight()
```

e:

```text
dosing_controller_get_state()
```

---

## 64. Princípio final

A arquitetura de UI deste projeto pode ser resumida assim:

```text
                    USUÁRIO
                       │
                       ▼
                     UI
                       │
              ┌────────┴────────┐
              │                 │
          interação         informação
              │                 ▲
              ▼                 │
       Screen Manager           │
              │                 │
              ▼                 │
       DosingController ────────┘
              │
              ▼
       Hardware abstraction
```

A regra fundamental é:

> **A UI apresenta e solicita; o domínio decide; o hardware executa.**

As telas devem permanecer responsáveis pela experiência de interação, enquanto o `DosingController` permanece responsável pelo comportamento da dosagem.

Assim, a interface pode evoluir visualmente, o simulador pode evoluir em fidelidade e o hardware pode ser substituído sem transformar a navegação em uma dependência da lógica física do produto.