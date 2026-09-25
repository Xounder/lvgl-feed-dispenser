## 10. Checklist, critério de conclusão e evolução contínua

[voltar ao índice](../11-migration-pc-to-esp32.md)

---

### 68. Checklist de migração

#### Ambiente

```text
[ ] Arduino IDE + core ESP32 configurado
[ ] ESP32-S3 reconhecido
[ ] build funcionando
[ ] flash funcionando
[ ] monitor serial funcionando
```

#### Display

```text
[ ] display inicializa
[ ] 800×480 configurado
[ ] orientação correta
[ ] backlight funcionando
[ ] LVGL renderizando
```

#### Touch

```text
[ ] controlador identificado
[ ] driver funcionando
[ ] toque detectado
[ ] eventos LVGL funcionando
```

#### UI

```text
[ ] Home
[ ] Mode
[ ] Config
[ ] Dosing
[ ] Completed
```

#### Peso

```text
[ ] HX711
[ ] load cell
[ ] tara
[ ] calibração
[ ] leitura estável
```

#### Dispenser

```text
[ ] SG90
[ ] posição inicial
[ ] posição de acionamento
[ ] stop
[ ] mecanismo
```

#### Integração

```text
[ ] dosagem pequena
[ ] dosagem normal
[ ] cancelamento
[ ] overshoot
[ ] timeout
[ ] falha de sensor
[ ] repetibilidade
```

---

### 69. Critério para considerar a migração concluída

A migração não estará concluída simplesmente porque:

```text
"o firmware compilou"
```

O sistema deverá demonstrar:

```text
ESP32-S3
 ↓
display
 ↓
touch
 ↓
UI
 ↓
peso real
 ↓
dispenser real
 ↓
dosagem
 ↓
parada
 ↓
resultado
```

Além disso, deverão ser validados cenários de falha.

---

### 70. O simulador depois da migração

O simulador continuará tendo valor.

Ele poderá ser utilizado para:

* desenvolvimento de UI;
* testes rápidos;
* regressão;
* desenvolvimento do domínio;
* reprodução de falhas;
* testes de timeout;
* testes de sensor;
* testes de overshoot;
* validação de novas funcionalidades.

Portanto:

```text
ESP32 disponível
```

não significa:

```text
simulador descartado
```

O papel contínuo do simulador é detalhado em [06-simulation-strategy.md](../06-simulation-strategy.md).

---

### 71. Relação entre os dois ambientes

O ideal é manter:

```text
             Código compartilhado
                    │
          ┌─────────┴─────────┐
          │                   │
         PC                 ESP32
          │                   │
      Simulação             Física
```

O PC permite velocidade.

O ESP32 permite realidade.

Os dois possuem funções diferentes.

---

### 72. Estratégia de desenvolvimento após a migração

Mesmo depois que o hardware estiver funcionando, alterações grandes podem continuar sendo desenvolvidas primeiro no simulador:

```text
Nova funcionalidade
      ↓
Implementar
      ↓
Simulador
      ↓
Testes
      ↓
ESP32
      ↓
Validação física
```

Quando uma alteração depender essencialmente de hardware:

```text
Alteração física
      ↓
ESP32
      ↓
Teste real
```

---

### 73. Principais riscos da migração

Os maiores riscos esperados são:

```text
1. Diferença de memória
2. Diferença de performance
3. Driver do display
4. Driver do touch
5. Leitura ruidosa do HX711
6. Calibração da load cell
7. Overshoot da dosagem
8. Controle do servo
9. Alimentação
10. Diferenças de temporização
```

Esses riscos não devem ser tratados como problemas da arquitetura por si só.

São diferenças naturais entre simulação e hardware físico.

---

### 74. O que o simulador não promete

O simulador não garante:

```text
precisão física
```

nem:

```text
funcionamento elétrico
```

nem:

```text
comportamento mecânico
```

Ele garante apenas aquilo que seu modelo consegue representar.

Por isso, os testes físicos continuam obrigatórios.

---

### 75. Estratégia geral

A estratégia completa pode ser resumida em:

```text
                 DESENVOLVIMENTO
                       │
            ┌──────────┴──────────┐
            │                     │
            ▼                     ▼
      SIMULADOR PC             HARDWARE
            │                     │
     rápido/barato          real/físico
            │                     │
            └──────────┬──────────┘
                       │
                       ▼
                 mesma aplicação
```

O projeto deve aproveitar cada ambiente para aquilo que ele faz melhor.

---

### 76. Princípio final

A migração PC → ESP32-S3 deve ser uma **substituição progressiva de plataformas e implementações**, e não uma segunda aplicação completamente independente.

A arquitetura desejada é:

```text
                    APLICAÇÃO
                        │
        ┌───────────────┼───────────────┐
        │               │               │
        ▼               ▼               ▼
       UI             Domain        Interfaces
        │               │               │
        └───────────────┴───────────────┘
                        │
              ┌─────────┴─────────┐
              │                   │
              ▼                   ▼
          Plataforma PC       Plataforma ESP32
              │                   │
          SDL2/HAL             Arduino/ESP-IDF HAL
              │                   │
          Simulação              Real
              │                   │
       Weight/Dispenser     HX711/SG90
```

A regra mais importante para futuras implementações é:

> **Se uma funcionalidade puder ser desenvolvida e validada sem depender do hardware físico, ela deve continuar sendo desenvolvida de forma independente da plataforma.**

E, quando o hardware finalmente entrar no projeto:

> **Substituir a implementação física correspondente, preservando o máximo possível do comportamento já validado no simulador.**

Assim, o simulador PC deixa de ser apenas uma etapa inicial e passa a funcionar como uma segunda plataforma de desenvolvimento e regressão, enquanto o ESP32-S3 se torna a plataforma física final do produto.