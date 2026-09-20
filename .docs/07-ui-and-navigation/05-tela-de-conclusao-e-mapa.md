# Tela de conclusão e mapa das telas

## 32. Tela de conclusão

Arquivo:

```text
src/ui/screens/completed_screen.c
```

A tela apresenta:

```text
Dosagem concluida!
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
Quantidade fixa
```

ou:

```text
Porções
```

O fluxo é:

```text
COMPLETED
    ↓
Nova dosagem
    ↓
MODE
```

---

## 34. Voltar ao início após conclusão

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

## 35. Mapa das telas

| Tela      | Função                     | Próximas telas  |
| --------- | -------------------------- | --------------- |
| Home      | Entrada e início           | Mode            |
| Mode      | Selecionar tipo de dosagem | Config, Home    |
| Config    | Definir parâmetros         | Dosing, Home    |
| Dosing    | Acompanhar execução        | Completed, Home |
| Completed | Informar conclusão         | Mode, Home      |