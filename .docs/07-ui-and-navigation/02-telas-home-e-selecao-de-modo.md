# Telas Home e seleção de modo

## 9. Tela Home

Arquivo:

```text
src/ui/screens/home_screen.c
```

A Home é a porta de entrada da aplicação e corresponde à **TELA 1**
(`Aguardando - Seleção de Modo`) do mockup em
`.images/tela-aguardando.md`.

O layout segue o chrome compartilhado (`screen_chrome.c`):

```text
Pesagem e Dosagem
──────────────────────────────────────────
ESTADO ATUAL
AGUARDANDO
Selecione o modo de dosagem

┌──────────────────────────────────────┐
│ MASSA                                │
│ Dosar por peso (g)                   │
└──────────────────────────────────────┘
┌──────────────────────────────────────┐
│ VALOR MONETARIO                      │
│ Dosar por valor (R$)                 │
└──────────────────────────────────────┘

┌──────────────────────────────────────┐
│ LIBERAR MANUALMENTE                  │
└──────────────────────────────────────┘
────────────────────────────────────────
🏠 Inicio   Dosagens   Historico   Config.
```

Os textos aparecem sem acentuação (`MONETARIO`, `Historicos`) porque a
fonte padrão do LVGL (Montserrat) só cobre ASCII básico + símbolos (ver
nota na seção 9b).

---

## 9a. Liberação manual na Home

A Home hospeda a **liberação manual** (RS14-RS16 do Trabalho.md):

* o botão **LIBERAR MANUALMENTE** deve ser mantido pressionado;
* enquanto pressionado, o controller adiciona ração manualmente;
* ao soltar, a liberação manual para;
* a liberação manual só é permitida quando o estado do domínio é
  `IDLE` (bloqueada durante dosagem automática — RS15).

O widget usado é `manual_release_widget.c` (apenas o botão),
compartilhado com as telas de conclusão e interrupção. Na Home, o widget
fica na **parte inferior da tela**, logo acima da barra de navegação.

---

## 9b. Nota sobre acentuação e ícones

A fonte enviada com o LVGL (`lv_font_montserrat_*`) contém apenas os
glifos `U+0020-U+007F`, `U+00B0` e `U+2022`, além dos símbolos
FontAwesome embutidos. Por isso:

* textos são escritos sem acento: `CONCLUIDO`, `INTERROMPER`, `Racao`,
  etc.;
* ícones que não existem na fonte (ampulheta, balança, cifrão, mão,
  cadeado) são representados pelos símbolos FontAwesome disponíveis ou
  omitidos;
* a barra inferior usa `LV_SYMBOL_HOME` (Inicio), `LV_SYMBOL_LIST`
  (Dosagens), `LV_SYMBOL_REFRESH` (Historico) e
  `LV_SYMBOL_SETTINGS` (Config.).

---

## 10. Responsabilidades da Home

A Home deve:

* apresentar o estado inicial (`AGUARDANDO`);
* permitir selecionar o modo de dosagem (cards MASSA / VALOR
  MONETARIO);
* permitir liberação manual;
* servir como ponto de retorno após `NOVA DOSAGEM`;
* evitar expor detalhes internos do hardware.

Clicar no card:

```text
MASSA
```

chama:

```text
screen_manager_show_config(DOSING_MODE_GRAMS);
```

Clicar no card:

```text
VALOR MONETARIO
```

chama:

```text
screen_manager_show_config(DOSING_MODE_CURRENCY);
```

A Home não deve iniciar diretamente o motor/servo ou manipular o sensor.

---

## 11. Seleção de modo incorporada à Home

A antiga `SCREEN_MODE` / `mode_screen.c` foi **removida**.

A seleção de modo agora é parte da própria Home (TELA 1), eliminando uma
tela intermediária. O fluxo passou a ser:

```text
Home (cards) -> Config -> Dosing -> Completed / Interrupted -> Home
```

---

## 12. Modo Massa

Quando o usuário escolhe:

```text
MASSA
```

a UI solicita:

```text
screen_manager_show_config(DOSING_MODE_GRAMS);
```

O modo é armazenado pelo gerenciamento da UI para que a tela de
configuração saiba qual interface apresentar.

---

## 13. Modo Valor (R$)

Quando o usuário escolhe:

```text
VALOR MONETARIO
```

a UI solicita:

```text
screen_manager_show_config(DOSING_MODE_CURRENCY);
```

O princípio é o mesmo:

```text
usuário escolhe modo
        ↓
UI informa modo selecionado
        ↓
ConfigScreen apresenta controles apropriados
```

No modo Valor (R$), a quantidade desejada é informada em reais; a
conversão para gramas usa o preço de referência definido no domínio (ver
[04-domain-and-state-machine.md](../04-domain-and-state-machine.md)).

---

## 14. Voltar para o início

A tela de configuração (TELA 2) possui o botão:

```text
VOLTAR
```

Esse botão retorna para:

```text
SCREEN_HOME
```

Esse caminho não altera a configuração de dosagem.