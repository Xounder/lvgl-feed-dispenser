# Telas Home e seleção de modo

## 9. Tela Home

Arquivo:

```text
src/ui/screens/home_screen.c
```

A Home é a porta de entrada da aplicação.

Atualmente apresenta:

```text
Dosador de Racao
Aguardando dosagem
Peso atual: 0 g

[ LED: modo manual ]   (indicador verde quando liberação manual ativa)

[ Iniciar ]
[ Liberacao manual ]   (segurar para liberar manualmente)
```

O texto utiliza `Racao` em vez de `Ração` porque a configuração atual da fonte padrão do LVGL não possui todos os caracteres acentuados necessários.

---

## 9a. Liberação manual na Home

A Home também hospeda a **liberação manual** (RS14-RS16 do Trabalho.md):

* o botão **Liberacao manual** deve ser mantido pressionado;
* enquanto pressionado, o controller adiciona ração manualmente e o **LED** (indicador circular) fica verde;
* ao soltar, a liberação manual para e o LED volta a ficar cinza;
* a liberação manual só é permitida quando o estado do domínio é `IDLE` (bloqueada durante dosagem automática — RS15).

A tela exibe o peso atual em tempo real, permitindo observar o efeito da liberação manual.

---

## 10. Responsabilidades da Home

A Home deve:

* apresentar o estado inicial;
* permitir iniciar uma nova operação;
* servir como ponto de retorno;
* evitar expor detalhes internos do hardware.

O botão:

```text
Iniciar
```

leva para:

```text
SCREEN_MODE
```

A Home não deve iniciar diretamente o motor/servo ou manipular o sensor.

---

## 11. Tela de seleção de modo

Arquivo:

```text
src/ui/screens/mode_screen.c
```

A tela apresenta:

```text
Selecione o modo
```

com duas opções:

```text
Massa
Valor (R$)
```

---

## 12. Modo Massa

Quando o usuário escolhe:

```text
Massa
```

a UI solicita:

```text
screen_manager_show_config(DOSING_MODE_GRAMS);
```

O modo é armazenado pelo gerenciamento da UI para que a tela de configuração saiba qual interface apresentar.

---

## 13. Modo Valor (R$)

Quando o usuário escolhe:

```text
Valor (R$)
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

No modo Valor (R$), a quantidade desejada é informada em reais; a conversão para gramas usa o preço de referência definido no domínio (ver [04-domain-and-state-machine.md](../04-domain-and-state-machine.md)).

---

## 14. Voltar para o início

A tela de seleção de modo possui:

```text
Voltar ao inicio
```

Esse botão retorna para:

```text
SCREEN_HOME
```

Esse caminho não altera a configuração de dosagem.