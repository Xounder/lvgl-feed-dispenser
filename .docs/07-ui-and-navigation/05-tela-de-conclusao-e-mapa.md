# Telas de conclusão, interrupção e mapa das telas

## 32. Tela de conclusão

Arquivo:

```text
src/ui/screens/completed_screen.c
```

A tela apresenta:

```text
Dosagem concluida!
Peso final: X g
```

e informações sobre o objetivo atingido.

Ela oferece:

```text
Nova dosagem
```

e:

```text
Voltar ao inicio
```

---

## 33. Nova dosagem

Ao selecionar:

```text
Nova dosagem
```

o usuário retorna para:

```text
SCREEN_MODE
```

Isso permite selecionar novamente:

```text
Massa
```

ou:

```text
Valor (R$)
```

Antes de voltar ao modo, o controller é reiniciado com tara:

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
MODE
```

---

## 34. Voltar ao início após conclusão ou interrupção

Ao selecionar:

```text
Voltar ao inicio
```

o usuário retorna para:

```text
SCREEN_HOME
```

Esse caminho representa o encerramento do ciclo de uso.

---

## 34a. Tela de interrupção

Arquivo:

```text
src/ui/screens/interrupted_screen.c
```

É exibida quando a dosagem é interrompida por:

```text
Parar
```

ou:

```text
Emergencia
```

durante a execução (RS11/RS12).

A tela apresenta:

```text
Dosagem interrompida
Massa parcial: X g (de Y g)
```

e oferece:

```text
Nova dosagem
Voltar ao inicio
```

**Nova dosagem** retorna a `SCREEN_MODE` (com tara); **Voltar ao inicio** retorna a `SCREEN_HOME`.

---

## 35. Mapa das telas

| Tela        | Função                     | Próximas telas        |
| ----------- | -------------------------- | --------------------- |
| Home        | Entrada, início e liberação manual | Mode            |
| Mode        | Selecionar modo (Massa / Valor R$) | Config, Home  |
| Config      | Definir parâmetros         | Dosing, Home          |
| Dosing      | Acompanhar execução        | Completed, Interrupted |
| Completed   | Informar conclusão         | Mode, Home            |
| Interrupted | Informar interrupção       | Mode, Home            |