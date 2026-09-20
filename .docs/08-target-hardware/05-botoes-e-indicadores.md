## 05. Botões e indicadores

[voltar ao índice](../08-target-hardware.md)

---

### 22. Botões físicos

Além do touchscreen, o projeto prevê botões físicos.

Os componentes inicialmente considerados incluem:

```text
2 botões físicos
```

Eles podem ser utilizados para funções específicas da operação.

A função exata ainda pode evoluir conforme o uso do dispositivo.

Possíveis funções incluem:

```text
iniciar
cancelar
confirmar
```

Mas a definição final deve considerar a interface touchscreen e a experiência real de utilização.

---

### 23. Botões e debounce

Botões físicos podem apresentar bouncing.

O software deverá considerar debounce, seja:

```text
por hardware
```

ou:

```text
por software
```

O objetivo é evitar que uma única pressão seja interpretada como múltiplas.

Conceitualmente:

```text
botão
  ↓
debounce
  ↓
evento válido
  ↓
aplicação
```

---

### 24. Chave liga/desliga

O projeto também prevê uma:

```text
chave liga/desliga
```

A função principal é controlar a alimentação do dispositivo ou permitir o acionamento geral do sistema, dependendo da arquitetura elétrica escolhida.

A implementação elétrica final deverá garantir que a chave seja compatível com a corrente e tensão utilizadas.

---

### 25. Indicador / LED

Também existe um indicador visual simples, inicialmente considerado como:

```text
LED
```

Ele pode ser utilizado para representar condições como:

```text
dispositivo ligado
dosagem em andamento
concluído
erro
```

A semântica final ainda pode ser definida durante a integração.

O LED não deve substituir as informações principais da interface gráfica.

---

[voltar ao índice](../08-target-hardware.md)