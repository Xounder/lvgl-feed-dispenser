# Telas de conclusão, interrupção e mapa das telas

## 32. Tela de conclusão

Arquivo:

```text
src/ui/screens/completed_screen.c
```

A tela apresenta o resultado de uma dosagem concluída e corresponde à
**TELA 3** (`Concluido`) do mockup `.images/tela-concluido.md`:

```text
CONCLUIDO ✓
Dosagem concluida com sucesso

Massa final
│                                                      │
│                       500 g                         │
│                                                      │
┌──────────────────────────┐
│ NOVA DOSAGEM             │
└──────────────────────────┘

┌──────────────────────────────────────┐
│ LIBERAR MANUALMENTE                  │
└──────────────────────────────────────┘
```

A massa final vem do domínio:

```text
dosing_controller_get_weight()
```

Além da liberação manual (mesmo widget da Home), a tela oferece:

```text
NOVA DOSAGEM
```

O widget de liberação manual fica na **parte inferior da tela**, logo
acima da barra de navegação (como na Home).

---

## 33. Nova dosagem

Ao selecionar:

```text
NOVA DOSAGEM
```

o usuário retorna para:

```text
SCREEN_HOME
```

Antes de voltar, o controller é reiniciado com tara:

```text
dosing_controller_new_dosing();
```

(zera a balança e volta para `IDLE`).

O fluxo é:

```text
COMPLETED / INTERRUPTED
    ↓
Nova dosagem
    ↓
HOME
```

---

## 34. (removido) Voltar ao início

A antiga ação:

```text
Voltar ao inicio
```

foi **removida** das telas de conclusão e interrupção: os mockups
`.images/tela-concluido.md` e `.images/tela-interrompido.md` apresentam
somente **NOVA DOSAGEM** (que já retorna à Home) e a liberação manual.

---

## 34a. Tela de interrupção

Arquivo:

```text
src/ui/screens/interrupted_screen.c
```

É exibida quando a dosagem é interrompida pelo comando:

```text
INTERROMPER DOSAGEM
```

durante a execução (RS11/RS12) ou pelo botão físico de emergência no
alvo embarcado.

A tela corresponde à **TELA 4** (`Interrompido`) do mockup
`.images/tela-interrompido.md`:

```text
INTERROMPIDO ⚠
Dosagem interrompida

Massa parcial: 240 g de 500 g
┌────────────────────────────────────────┐
│ ████████░░░░░░░░░░░░░░░░░░    48%      │
└────────────────────────────────────────┘

┌──────────────────────────┐
│ NOVA DOSAGEM             │
└──────────────────────────┘

┌──────────────────────────────────────┐
│ LIBERAR MANUALMENTE                  │
└──────────────────────────────────────┘
```

**NOVA DOSAGEM** retorna a `SCREEN_HOME` (com tara via
`dosing_controller_new_dosing()`).

---

## 35. Mapa das telas

| Tela        | Função                     | Próximas telas             |
| ----------- | -------------------------- | -------------------------- |
| Home        | Início, seleção de modo e liberação manual | Config           |
| Config      | Definir parâmetros         | Dosing, Home               |
| Dosing      | Acompanhar execução        | Completed, Interrupted     |
| Completed   | Informar conclusão         | Home                       |
| Interrupted | Informar interrupção       | Home                       |