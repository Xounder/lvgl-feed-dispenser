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
Pronto

[ Iniciar ]
```

O texto utiliza `Racao` em vez de `Ração` porque a configuração atual da fonte padrão do LVGL não possui todos os caracteres acentuados necessários.

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
Quantidade fixa
Porcoes
```

---

## 12. Modo Quantidade Fixa

Quando o usuário escolhe:

```text
Quantidade fixa
```

a UI solicita:

```text
screen_manager_show_config(CONFIG_MODE_FIXED_AMOUNT);
```

O modo é armazenado pelo gerenciamento da UI para que a tela de configuração saiba qual interface apresentar.

---

## 13. Modo Porções

Quando o usuário escolhe:

```text
Porcoes
```

a UI solicita:

```text
screen_manager_show_config(CONFIG_MODE_PORTIONS);
```

O princípio é o mesmo:

```text
usuário escolhe modo
        ↓
UI informa modo selecionado
        ↓
ConfigScreen apresenta controles apropriados
```

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