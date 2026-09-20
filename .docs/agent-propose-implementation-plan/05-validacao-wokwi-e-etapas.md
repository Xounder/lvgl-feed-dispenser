## 05. Validacao, Wokwi e etapas

[voltar ao índice](../agent-propose-implementation-plan.md)

---

# E como validar de verdade?

Eu dividiria a validação em 3 níveis.

## 🟢 Nível 1 — Interface

Validar manualmente.

Exemplo:

```
```

```
[ TESTE ]

✓ Botão Massa abre tela correta

✓ Botão Valor abre tela correta

✓ Voltar funciona

✓ Cancelar funciona

✓ Tela de concluído aparece
```

---

## 🟡 Nível 2 — Lógica

Criar testes automáticos.

Por exemplo:

```
```

```
TEST(
    stop_dosing_when_target_weight_reached
) {

    // Peso atual
    fakeSensor.setWeight(500);

    // Meta
    controller.setTarget(500);

    controller.update();

    EXPECT_FALSE(
        fakeDispenser.isRunning()
    );
}
```

O ESP-IDF possui suporte a testes unitários e também mecanismos de mocking para reduzir dependências de hardware, embora o suporte host-based para componentes específicos ainda seja parcial. 

---

## 🟠 Nível 3 — Simulação completa

Rodar o programa.

Você faz:

```
```

```
Selecionar MASSA
```

↓

```
```

```
Escolher 500 g
```

↓

```
```

```
INICIAR
```

↓

Peso começa:

```
```

```
0 g
15 g
32 g
64 g
125 g
237 g
...
```

↓

```
```

```
500 g
```

↓

```
```

```
CONCLUÍDO
```

Tudo funcionando no computador.

---

# E o Wokwi?

Você também pode usar o **Wokwi** como complemento. Ele possui simulação para ESP32, incluindo ESP32-S3, segundo a documentação atual. 

Mas, **para o seu projeto especificamente, eu não começaria por ele**.

Eu faria:

### 🥇 Desenvolvimento principal

```
```

```
PC + LVGL Simulator
```

Para desenvolver:

-  Interface 
-  Navegação 
-  Botões 
-  Estados 
-  Fluxo da aplicação 

---

### 🥈 Simulação da lógica

```
```

```
Fake HX711
+
Fake Servo
```

Feito por nós mesmos no código.

---

### 🥉 Depois

```
```

```
Wokwi
```

Para testar partes do ESP32 que forem suportadas na simulação.

---

### 🏆 Finalmente

```
```

```
ESP32-S3 REAL
+
Tela REAL
+
HX711 REAL
+
Servo REAL
```

---

# O que eu faria no seu lugar, começando agora

## Etapa 1

Criar um projeto:

```
```

```
RationDispenser
```

Com:

```
```

```
LVGL Simulator
```

---

## Etapa 2

Fazer apenas esta tela inicialmente:

```
```

```
┌───────────────────────────────────┐
│                                   │
│       PESAGEM E DOSAGEM           │
│                                   │
│         AGUARDANDO                │
│                                   │
│ Escolha o modo de dosagem         │
│                                   │
│ ┌───────────────────────────────┐ │
│ │ ⚖ MASSA                      │ │
│ │ Dosar por quantidade em gramas│
│ └───────────────────────────────┘ │
│                                   │
│ ┌───────────────────────────────┐ │
│ │ 💰 VALOR                      │ │
│ │ Dosar por valor em R$         │ │
│ └───────────────────────────────┘ │
│                                   │
└───────────────────────────────────┘
```

---

## Etapa 3

Adicionar navegação:

```
```

```
MASSA
  ↓

Selecionar peso
  ↓

100 g
250 g
500 g
1000 g
Personalizado
```

---

## Etapa 4

Criar:

```
```

```
SimulatedWeightSensor
```

---

## Etapa 5

Criar:

```
```

```
SimulatedDispenser
```

---

## Etapa 6

Fazer o fluxo inteiro funcionar:

```
```

```
INTERFACE
     ↓
CONFIGURAÇÃO
     ↓
INICIAR
     ↓
SERVO SIMULADO
     ↓
PESO SIMULADO
     ↓
META ATINGIDA
     ↓
PARAR
     ↓
CONCLUÍDO
```

---

# Minha recomendação mais importante

**Não comece escrevendo código diretamente para o HX711 e Servo.**

Como você ainda não tem os equipamentos, o melhor é projetar primeiro uma camada de abstração:

```
```

```
Sua aplicação
      │
      ▼
 Interfaces
      │
 ┌────┴─────┐
 ▼          ▼

SIMULAÇÃO    HARDWARE
AGORA        DEPOIS
```

Assim você consegue fazer provavelmente **70–85% do software antes de comprar as peças**.

## Para o seu projeto, eu sugiro o próximo passo:

Montarmos um **projeto base real**, já organizado para rodar no seu computador, contendo:

```
```

```
✓ LVGL
✓ Tela 800×480 simulada
✓ HomeScreen
✓ Seleção por MASSA
✓ Seleção por VALOR
✓ DosingController
✓ Servo simulado
✓ HX711 simulado
✓ Máquina de estados
✓ Estrutura preparada para ESP32-S3 depois
```

E desenvolvermos em etapas, começando pela instalação/configuração e uma primeira tela funcionando no seu PC. Essa abordagem aproveita justamente a portabilidade do simulador oficial do LVGL. [Documentação oficial do simulador LVGL](https://lvgl.io/docs/open/9.2/integration/ide/pc-simulator?utm_source=chatgpt.com)

### Fontes visualizadas

| FonteConfiança                          |          |
| --------------------------------------- | -------- |
| Documentação oficial LVGL 9.6           | **HIGH** |
| Documentação oficial LVGL Simulator 9.2 | **HIGH** |
| Documentação oficial LVGL Simulator 9.1 | **HIGH** |
| Documentação LVGL Platforms             | **HIGH** |
| Documentação oficial ESP-IDF Unit Tests | **HIGH** |
| Documentação oficial Wokwi ESP32        | **HIGH** |

Se quiser, no próximo passo eu posso **svgmontar a estrutura inicial do projeto para o seu PC**, escolhendo com você a melhor stack entre **LVGL + CMake/SDL** ou uma abordagem com **PlatformIO**, já pensando na futura migração para o ESP32-S3.
