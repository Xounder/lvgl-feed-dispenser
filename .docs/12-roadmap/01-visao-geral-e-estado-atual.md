## 1. Objetivo

_Voltar ao índice: [`../12-roadmap.md`](../12-roadmap.md)._

---

Este documento apresenta a evolução planejada do projeto desde o início do simulador desktop até a integração com o hardware físico baseado em ESP32-S3.

O roadmap existe para manter uma visão clara de:

* o que já foi concluído;
* o que está sendo desenvolvido;
* qual é o próximo passo;
* quais funcionalidades ainda são planejadas;
* quais etapas dependem do hardware físico.

O roadmap não deve ser interpretado como um cronograma rígido.

A ordem pode mudar conforme:

* disponibilidade dos componentes;
* descobertas durante os testes;
* problemas de arquitetura;
* limitações do hardware;
* necessidades do projeto.

---

## 2. Visão geral

A evolução do projeto pode ser representada por:

```text
Ideia
  ↓
Simulador PC
  ↓
Arquitetura
  ↓
Fluxo de dosagem
  ↓
Simulação de hardware
  ↓
Tratamento de falhas
  ↓
ESP32-S3
  ↓
Display + Touch
  ↓
HX711 + Load Cell
  ↓
SG90 + mecanismo
  ↓
Dosagem física
  ↓
Calibração
  ↓
Produto funcional
```

A estratégia principal continua sendo:

> **validar primeiro no ambiente mais simples possível e adicionar complexidade apenas quando ela for necessária.**

---

## 3. Estado atual do projeto

Atualmente o projeto já possui um simulador funcional no PC.

O fluxo principal está implementado:

```text
Home
  ↓
Seleção de modo
  ↓
Configuração
  ↓
Dosagem
  ↓
Conclusão
```

A dosagem possui um peso simulado e um dispenser simulado.

O ciclo completo já pode ser executado sem o hardware físico.