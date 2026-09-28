## 11. Limitações e segurança

[voltar ao índice](../08-target-hardware.md)

---

### 52. Limitações conhecidas

Neste estágio, alguns detalhes ainda não estão definitivamente definidos:

* modelo exato da célula de carga;
* capacidade da célula;
* pinagem definitiva dos periféricos externos (HX711, servo, botões, LED);
* mecanismo mecânico final;
* posições exatas do SG90;
* estratégia final de alimentação;
* necessidade de reguladores adicionais;
* comportamento final dos botões;
* método de calibração;
* filtro de peso;
* estratégia final contra overshoot.

O controlador do touch (**GT911**) e os pinos de display/touch/backlight já estão definidos em `src/hardware/esp32/board_config.h`; os demais pontos devem ser definidos com base nos componentes efetivamente disponíveis e nos testes físicos.

---

### 53. Não assumir a pinagem antecipadamente

A pinagem do ESP32-S3 deve ser definida somente depois de confirmar:

* modelo da placa;
* periféricos;
* display;
* touch controller;
* HX711;
* servo;
* botões;
* conflitos entre interfaces.

Não registrar GPIOs arbitrários na aplicação apenas para preencher a documentação.

A pinagem deve ser uma decisão documentada quando os componentes reais estiverem confirmados.

---

### 54. Segurança elétrica

Antes da montagem final, devem ser verificados:

* tensão de cada componente;
* corrente máxima;
* corrente de pico;
* polaridade;
* aterramento comum;
* reguladores;
* conexões do servo;
* proteção da fonte;
* capacidade dos cabos;
* conexões mecânicas.

O fato de um componente funcionar individualmente não significa que a combinação inteira tenha alimentação adequada.

---

### 55. Segurança mecânica

Também devem ser considerados:

* partes móveis do mecanismo;
* pontos de esmagamento;
* fixação do servo;
* estabilidade do reservatório;
* suporte da célula de carga;
* queda da ração;
* facilidade de limpeza;
* acesso aos componentes.

O mecanismo deve evitar que uma falha de software resulte facilmente em uma condição mecânica perigosa.

---

### 56. Separação entre protótipo e produto final

Durante o desenvolvimento:

```text
protoboard
cabos
MB102
componentes temporários
```

são aceitáveis.

No produto final, poderá ser necessário utilizar:

```text
placa mais organizada
conectores
fixação mecânica
caixa
proteções
alimentação dedicada
```

Portanto, o protótipo não deve ser confundido com a arquitetura física definitiva.

---

[voltar ao índice](../08-target-hardware.md)