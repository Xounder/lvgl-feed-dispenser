## 05. Botões e indicadores

[voltar ao índice](../08-target-hardware.md)

---

### 22. Botões físicos

Além do touchscreen, o projeto prevê **2 botões físicos** (conforme Trabalho.md), com funções definidas:

```text
Botão de emergência    → interrompe imediatamente a dosagem (RS11/RS12)
Botão de liberação     → liberação manual direta, mantido pressionado (RS14)
```

O **botão de emergência** tem prioridade sobre o controle automático: acionado em qualquer momento da dosagem, o dispenser é parado imediatamente — equivale funcionalmente ao comando `INTERROMPER DOSAGEM` da tela de Dosagem no simulador.

O **botão de liberação** permite liberar ração manualmente e só deve atuar quando o sistema não estiver em dosagem automática (RS15); no simulador corresponde ao botão "Liberacao manual" da Home.

As funções físicas devem espelhar os comandos já validados no simulador para que o comportamento seja idêntico na migração.

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

Existe um **LED indicador** cuja função definida é sinalizar o **modo de liberação manual** (RS16): aceso quando a liberação manual está ativa, apagado caso contrário.

No simulador, esse mesmo comportamento é representado pelo indicador verde da Home.

A semântica final pode ser complementada durante a integração (ex.: outros estados), mas a indicação do modo manual já é um requisito fixo (RS16).

O LED não deve substituir as informações principais da interface gráfica.

Os pinos físicos de botões (emergência e liberação) e do LED estão definidos em `src/hardware/esp32/board_config.h`: botões em GPIO10 e GPIO18, LED em GPIO17. Implicações da placa VIEWE — ver [`11-limitacoes-e-seguranca.md`](11-limitacoes-e-seguranca.md):

* GPIO10–13 são o slot microSD (SPI) → os periféricos de domínio sacrificam o SD.
* GPIO17 é o trilho do WS2812B on-board (header 1,5 mm, removível) → confirmar físico.
* GPIO18 é o INT do GT911 (não usado pelo firmware) → validar que o sensor não aciona o pino.
* A integração no firmware (leitura dos botões, acionamento do LED) ainda não foi feita.

---

[voltar ao índice](../08-target-hardware.md)