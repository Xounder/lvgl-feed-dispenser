## 1. Preparação para o ESP32-S3

_Voltar ao índice: [`../12-roadmap.md`](../12-roadmap.md)._

---

Depois de o simulador estar suficientemente estável, começa a preparação do ambiente físico.

Primeiro:

```text
✓ ESP-IDF
✓ compilação
✓ flash
✓ monitor serial
```

Depois:

```text
✓ ESP32-S3 inicializa
```

---

## 2. Integração do display

O próximo bloco físico será:

```text
ESP32-S3
   ↓
display 4.3"
   ↓
800×480
   ↓
LVGL
```

O objetivo é executar inicialmente uma interface mínima.

---

## 3. Integração do touch

Depois:

```text
display
+
touch capacitivo
```

A meta é reproduzir o comportamento de interação já validado no simulador.

O controlador exato do touch ainda depende do módulo físico utilizado.

---

## 4. Port da UI

Com display e touch funcionando:

```text
Home
 ↓
Mode
 ↓
Config
 ↓
Dosing
 ↓
Completed
```

deverá ser executado no ESP32-S3.

Nesse estágio, o hardware de dosagem ainda pode continuar simulado.

---

## 5. Integração do HX711

Depois da interface:

```text
HX711
 ↓
Load Cell
 ↓
ESP32-S3
```

O primeiro objetivo será obter uma leitura confiável.

Antes da dosagem automática deverão ser validados:

```text
tara
calibração
estabilidade
repetibilidade
```

---

## 6. Integração da célula de carga

A load cell deverá ser instalada mecanicamente de maneira estável.

Depois será necessário:

```text
peso conhecido
 ↓
leitura
 ↓
calibração
 ↓
comparação
```

A unidade utilizada pela aplicação deverá continuar sendo:

```text
gramas
```

---

## 7. Integração do SG90

Depois:

```text
SG90
 ↓
mecanismo
```

Primeiro devem ser testados:

```text
posição inicial
posição de acionamento
posição de parada
tempo de movimento
```

Sem executar imediatamente uma dosagem completa.

---

## 8. Integração mecânica

Depois da eletrônica:

```text
reservatório
+
mecanismo
+
recipiente de pesagem
```

serão integrados.

O objetivo será garantir que a ração caia de forma previsível sobre a célula de carga.

---

## 9. Primeiros testes físicos

A primeira dosagem deverá utilizar uma quantidade pequena.

Sequência:

```text
iniciar
 ↓
acionar dispenser
 ↓
ração começa a cair
 ↓
peso aumenta
 ↓
atingir meta
 ↓
parar
```

O comportamento deverá ser observado manualmente.

---

## 10. Ajuste da dosagem física

Depois dos primeiros testes, serão analisados:

```text
vazão
overshoot
tempo de resposta
estabilidade
precisão
```

O comportamento observado poderá exigir ajustes no:

* mecanismo;
* posição do servo;
* tempo de acionamento;
* estratégia de parada;
* filtragem do peso;
* lógica do controller.

---

## 11. Calibração do sistema completo

Depois de todos os componentes integrados:

```text
Load Cell
+
HX711
+
Dispenser
+
Controller
```

serão realizados testes com quantidades conhecidas.

Exemplo:

```text
Meta: 50 g
Meta: 100 g
Meta: 150 g
Meta: 200 g
```

O objetivo é observar a repetibilidade do sistema.

---

## 12. Testes de repetibilidade

Não basta uma dosagem funcionar uma única vez.

Devem ser realizados vários ciclos:

```text
50 g
50 g
50 g
50 g
50 g
```

e depois:

```text
100 g
100 g
100 g
100 g
100 g
```

Isso permite observar a variação entre ciclos.

---

## 13. Testes de falha no hardware

Depois do caminho normal, deverão ser testados:

```text
sensor sem progresso
sensor desconectado
peso instável
dispenser travado
timeout
cancelamento
reinicialização
```

O objetivo é garantir que falhas não deixem o atuador em estado perigoso.

---

## 14. Teste de energia

O sistema físico deverá ser testado considerando:

```text
ESP32-S3
display
servo
HX711
touch
LEDs
```

Também deverão ser observados:

```text
quedas de tensão
reinicializações
ruído
comportamento do servo
```

A arquitetura elétrica definitiva deverá ser validada separadamente da lógica de software.

---

## Referências canônicas

O detalhamento técnico desta fase é tratado em:

- [`08-target-hardware.md`](../08-target-hardware.md) — hardware físico alvo (ESP32-S3, display/touch, HX711/load cell, SG90/mecanismo, botões, indicadores, alimentação).
- [`11-migration-pc-to-esp32.md`](../11-migration-pc-to-esp32.md) — plano de migração incremental (etapas 1–10) e checklist de migração.
- [`10-testing-strategy.md`](../10-testing-strategy.md) — testes físicos, calibração, repetibilidade e falhas no hardware.