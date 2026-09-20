## 11. Exemplos e estado final

[voltar ao índice](../13-agent-guide.md)

---

## 89. Exemplo completo de raciocínio

Suponha que o requisito seja:

> "Parar automaticamente se o peso não aumentar."

O agente não deve começar adicionando um `if` na tela.

O raciocínio esperado é:

```text
Requisito
 ↓
é regra da operação
 ↓
pertence ao domínio
 ↓
precisa de noção de tempo/progresso
 ↓
simulação deve conseguir representar peso parado
 ↓
controller detecta ausência de progresso
 ↓
dispenser.stop()
 ↓
ERROR
 ↓
UI apresenta erro
```

Assim a mesma regra poderá futuramente funcionar no ESP32.

---

## 90. Outro exemplo: botão físico

Se surgir o requisito:

> "Adicionar botão físico de cancelar."

A arquitetura esperada não é:

```text
GPIO
 ↓
DosingController
```

diretamente.

Primeiro considerar:

```text
GPIO / driver
 ↓
evento de entrada
 ↓
camada de aplicação/UI
 ↓
dosing_controller_cancel()
```

O domínio deve continuar conhecendo apenas a ação:

```text
cancelar
```

e não o pino físico responsável por acioná-la.

---

## 91. Outro exemplo: HX711

Para integrar o HX711:

```text
HX711 driver
 ↓
WeightSensor implementation
 ↓
DosingController
```

O controller não deve conhecer:

```text
GPIO
HX711
ADC
ponte de Wheatstone
```

Ele deve continuar recebendo:

```text
peso
```

através da capacidade de `WeightSensor`.

---

## 92. Outro exemplo: SG90

Para integrar o servo:

```text
servo driver
 ↓
Dispenser implementation
 ↓
DosingController
```

O controller não deve precisar saber:

```text
PWM
GPIO
ângulo 35°
ângulo 90°
timer do servo
```

Esses detalhes pertencem à implementação física.

---

## 93. Critério de qualidade

Uma alteração é arquiteturalmente saudável quando, sempre que possível:

```text
o domínio continua compreensível
o simulador continua funcionando
o hardware continua substituível
a UI continua responsável apenas pela apresentação
os testes continuam reproduzíveis
a migração para ESP32 continua possível
```

---

## 94. Ordem recomendada para grandes alterações

Para uma alteração maior:

```text
1. estudar o requisito
2. ler documentação relacionada
3. mapear código existente
4. definir impacto
5. alterar domínio/interface se necessário
6. alterar implementação
7. alterar UI
8. compilar
9. testar
10. revisar regressões
11. atualizar documentação
```

Não necessariamente toda alteração precisa seguir literalmente essa ordem, mas essa deve ser a linha de raciocínio.

---

## 95. Estado esperado ao final do desenvolvimento

O resultado final deverá ser algo próximo de:

```text
                ┌─────────────────┐
                │      LVGL       │
                │       UI        │
                └────────┬────────┘
                         │
                         ▼
                ┌─────────────────┐
                │    Application   │
                │     / Domain     │
                └────────┬────────┘
                         │
                ┌────────┴────────┐
                ▼                 ▼
        ┌──────────────┐   ┌──────────────┐
        │ WeightSensor │   │  Dispenser   │
        └──────┬───────┘   └──────┬───────┘
               │                  │
       ┌───────┴───────┐   ┌──────┴───────┐
       │               │   │              │
       ▼               ▼   ▼              ▼
   Simulado          HX711 Simulado    SG90
                     + Load Cell
```

No PC:

```text
UI
 ↓
Domain
 ↓
Simulated Hardware
```

No hardware:

```text
UI
 ↓
Domain
 ↓
Real Hardware
```

O objetivo é que a mudança entre esses ambientes seja principalmente uma troca de implementação de plataforma, e não uma reescrita da lógica do produto.

---

## 96. Regra final para qualquer agente

Antes de terminar uma alteração, faça estas perguntas:

```text
1. O que estou modificando?
2. Por que essa mudança é necessária?
3. Em qual camada ela pertence?
4. Estou colocando uma responsabilidade na camada errada?
5. Estou assumindo alguma informação que ainda não foi confirmada?
6. Estou confundindo arquitetura planejada com código existente?
7. Isso continuará fazendo sentido no ESP32-S3?
8. Posso testar isso no simulador?
9. O comportamento já existente continua funcionando?
10. A documentação ainda representa a realidade?
```

Se essas respostas estiverem claras, a alteração provavelmente está alinhada com o projeto.

---

## 97. Princípio central

Toda evolução do projeto deve preservar esta ideia:

> **Desenvolver primeiro o comportamento, isolar as dependências específicas de plataforma, validar no simulador e substituir as implementações simuladas pelas reais quando o hardware estiver disponível.**

E, principalmente:

> **O hardware deve implementar as necessidades da aplicação; a aplicação não deve ser moldada desnecessariamente pelos detalhes do hardware.**

O papel de um agente neste projeto não é apenas fazer o código funcionar.

É fazer o código funcionar **sem destruir a capacidade de o projeto continuar evoluindo do simulador para o dispositivo físico**.