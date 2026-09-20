## 03. Estratégia de desenvolvimento

[voltar ao índice](../03-architecture-decisions.md)

---

### 1. Decisão: desenvolver o simulador antes do ESP32

**Decisão**

O projeto será desenvolvido inicialmente no PC.

O ESP32-S3 será integrado posteriormente.

**Motivo principal**

O hardware físico introduz várias fontes de complexidade simultaneamente:

```text
Firmware
Display
Touch
HX711
Load Cell
Servo
Alimentação
Mecânica
GPIO
PWM
Ruído
Calibração
```

Se software e hardware fossem desenvolvidos simultaneamente desde o início, seria difícil determinar a origem de um problema.

Por exemplo:

```text
A dosagem parou incorretamente.
```

Isso poderia ser causado por:

* bug no controller;
* leitura errada do sensor;
* problema no HX711;
* calibração;
* servo;
* mecanismo;
* alimentação;
* temporização;
* interface.

O simulador permite eliminar grande parte dessas variáveis durante o desenvolvimento inicial.

Status: **Atual**.

Estratégia de simulação: [06-simulation-strategy.md](../06-simulation-strategy.md).

---

### 2. Decisão: o simulador não é descartável

Uma decisão importante do projeto é tratar o simulador como parte permanente do processo de desenvolvimento.

Ele não deve ser entendido como:

```text
"código provisório que será jogado fora"
```

mas como:

```text
ambiente de desenvolvimento e validação
```

Mesmo depois da existência do hardware físico, o simulador poderá continuar sendo utilizado para:

* testes de lógica;
* testes de UI;
* regressões;
* reprodução de bugs;
* simulação de falhas;
* desenvolvimento sem acesso ao hardware.

Status: **Atual**.

Estratégia de testes: [10-testing-strategy.md](../10-testing-strategy.md).

---

### 3. Decisão: manter o projeto orientado a migração

O simulador é desenvolvido levando em consideração a futura execução no ESP32-S3.

Isso não significa que o código deva ser artificialmente limitado.

Significa que decisões devem ser avaliadas considerando:

```text
Isso será reutilizável no ESP32?
```

e:

```text
Isso é específico apenas do PC?
```

Por exemplo:

**Código potencialmente reutilizável:**

```text
DosingConfig
DosingController
DosingState
regras de dosagem
interfaces de hardware
```

**Código específico do PC:**

```text
SDL2
Windows Sleep
configuração do executável desktop
```

**Código específico do ESP32:**

```text
GPIO
PWM
drivers HX711
driver do display
driver do touch
```

Status: **Atual**.

Plano de migração: [11-migration-pc-to-esp32.md](../11-migration-pc-to-esp32.md).

---

### 4. Decisão: evolução incremental

O projeto deve evoluir em etapas verificáveis.

A estratégia é:

```text
funcionalidade
     ↓
implementação
     ↓
build
     ↓
execução
     ↓
validação
     ↓
próxima funcionalidade
```

Em vez de tentar implementar simultaneamente:

```text
UI
+
HX711
+
servo
+
ESP32
+
touch
+
estados
+
tratamento de erros
```

Status: **Atual**.

Progressão planejada: [12-roadmap.md](../12-roadmap.md).

---

### 5. Decisão: validar o fluxo antes da física

Antes de tentar reproduzir fielmente o comportamento físico, deve-se validar o fluxo lógico.

Primeiro:

```text
configurar
   ↓
iniciar
   ↓
dosar
   ↓
atingir objetivo
   ↓
parar
   ↓
concluir
```

Depois:

```text
peso real
   ↓
calibração
   ↓
inércia da ração
   ↓
overshoot
   ↓
tempo de resposta
```

Isso evita tentar resolver problemas físicos antes de confirmar que a lógica básica funciona.

Status: **Atual**.

---

### 6. Decisão: a simulação deve evoluir com o projeto

A simulação inicial é deliberadamente simples.

Atualmente:

```text
dispenser ativo
      ↓
peso += 2 g
```

Isso é suficiente para validar o fluxo inicial.

No futuro, a simulação poderá representar:

```text
tempo
+
taxa de fluxo
+
variação
+
atraso
+
overshoot
+
falhas
```

Por exemplo:

```text
Dispenser
    ↓
fluxo estimado
    ↓
peso
    ↓
controller
```

O nível de realismo deve aumentar apenas quando houver necessidade de testar determinado comportamento.

Status: **Possível** (evolução futura). Detalhes: [06-simulation-strategy.md](../06-simulation-strategy.md).

---

### 7. Decisão: preparar tratamento de falhas

O sistema físico inevitavelmente poderá apresentar situações diferentes do fluxo ideal.

Exemplos:

```text
HX711 não responde
```

```text
peso não aumenta
```

```text
peso aumenta rápido demais
```

```text
dispenser não para
```

```text
meta não é atingida
```

```text
sensor apresenta leitura inválida
```

A arquitetura deve permitir representar esses casos sem transformar a UI em responsável pelo diagnóstico.

Status: **Possível** (planejado). Cenários em [06-simulation-strategy.md](../06-simulation-strategy.md) e [10-testing-strategy.md](../10-testing-strategy.md).