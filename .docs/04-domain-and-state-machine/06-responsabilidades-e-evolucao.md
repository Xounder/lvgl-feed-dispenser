## 34. Responsabilidade da tela de dosagem

_Voltar ao índice: [`../04-domain-and-state-machine.md`](../04-domain-and-state-machine.md)._

---

A tela de dosagem deve:

* exibir o peso;
* exibir o objetivo;
* atualizar o progresso;
* apresentar o status;
* permitir cancelamento;
* reagir à conclusão;
* reagir a erros.

Ela não deve decidir como a dosagem funciona.

Fluxo:

```text
Timer da UI
    ↓
controller.update()
    ↓
controller.get_weight()
    ↓
controller.get_state()
    ↓
UI atualiza apresentação
```

---

## 35. Responsabilidade do controller

O controller deve:

* iniciar a dosagem;
* parar a dosagem;
* cancelar;
* consultar peso;
* controlar o dispenser;
* determinar conclusão;
* determinar erros;
* manter o estado do processo.

Não deve:

* criar objetos LVGL;
* atualizar labels;
* desenhar progress bars;
* mudar telas;
* manipular SDL2.

---

## 36. Responsabilidade da UI

A UI deve:

* receber entrada do usuário;
* editar configuração;
* chamar operações da aplicação;
* exibir estado;
* exibir peso;
* exibir progresso;
* apresentar mensagens.

Não deve:

* implementar regras físicas;
* controlar GPIO;
* ler diretamente HX711;
* decidir quando uma dosagem terminou.

---

## 37. Responsabilidade do hardware

A implementação de hardware deve:

* comunicar-se com dispositivos físicos;
* converter leituras para formatos utilizáveis;
* executar comandos recebidos;
* detectar falhas específicas do dispositivo;
* manter detalhes de drivers encapsulados.

Ela não deve:

* decidir a quantidade de ração desejada;
* criar telas;
* decidir quando uma dosagem está concluída.

Os detalhes concretos que devem permanecer encapsulados estão listados na abstração de hardware (ver [`../05-hardware-abstraction.md`](../05-hardware-abstraction.md)).

---

## 38. Estado atual da implementação

A implementação existente é uma versão simplificada da máquina planejada.

Atualmente:

```text
IDLE
  ↓
DOSING
  ↓
COMPLETED
```

e o fluxo de:

```text
SELECT_MODE
CONFIGURING
```

é representado principalmente pela navegação da UI.

O estado:

```text
ERROR
```

ainda deve ser implementado.

Isso é intencional.

A máquina de estados deve evoluir conforme os requisitos reais forem definidos, e não através da criação antecipada de estados sem comportamento associado.

---

## 39. Evolução planejada

A evolução recomendada é:

### Etapa 1 — atual

```text
IDLE
DOSING
COMPLETED
```

### Etapa 2

Adicionar explicitamente:

```text
SELECT_MODE
CONFIGURING
```

ao modelo de domínio caso isso traga benefício real.

### Etapa 3

Adicionar:

```text
ERROR
```

com causas de erro bem definidas.

### Etapa 4

Adicionar:

```text
timeout
detecção de falta de progresso
validação de sensor
```

### Etapa 5

Ajustar as regras utilizando dados do hardware real.

---

## 40. Princípio para futuras mudanças

Antes de adicionar um novo estado, responder:

1. Existe um comportamento diferente que justifica o estado?
2. Existe uma transição clara para entrar nele?
3. Existe uma transição clara para sair dele?
4. Quais operações são permitidas nesse estado?
5. O hardware precisa estar em qual condição?
6. A UI precisa representar esse estado?
7. É realmente um estado ou apenas uma condição/evento?

Isso evita transformar a máquina de estados em uma coleção arbitrária de telas.

---

## 41. Resumo da máquina

A ideia central pode ser resumida em:

```text
                    ┌─────────┐
                    │  IDLE   │
                    └────┬────┘
                         │
                      iniciar
                         ▼
                  ┌──────────────┐
                  │ SELECT_MODE  │
                  └──────┬───────┘
                         │
                    modo escolhido
                         ▼
                  ┌──────────────┐
                  │ CONFIGURING  │
                  └──────┬───────┘
                         │
                    configuração
                       válida
                         ▼
                  ┌──────────────┐
                  │    DOSING    │
                  └──┬────┬───┬──┘
                     │    │   │
                cancelar │   │ meta
                     │   │   │
                     │ erro  │
                     │   │   │
                     ▼   ▼   ▼
                   IDLE ERROR COMPLETED
                                  │
                              nova dosagem
                                  │
                                  ▼
                             SELECT_MODE
```

A regra mais importante do processo é:

```text
DOSING
  ↓
monitorar peso
  ↓
peso >= objetivo?
  ├── não → continuar
  └── sim → parar dispenser → COMPLETED
```

E a regra de segurança fundamental é:

```text
qualquer cancelamento ou erro durante DOSING
        ↓
parar dispenser
        ↓
mudar para o estado apropriado
```

---

## 42. Princípio final

A máquina de estados existe para representar o comportamento do **produto**, não para reproduzir a estrutura da interface.

O objetivo final é que o domínio consiga responder claramente:

```text
Onde estou?
O que estou fazendo?
O que posso fazer agora?
O que acontece se algo der errado?
Qual condição permite avançar?
Qual condição encerra o processo?
```

A UI apenas apresenta essas informações e oferece as interações permitidas.

O hardware executa as capacidades físicas solicitadas pelo domínio.

Assim, o fluxo permanece:

```text
Usuário
   ↓
UI
   ↓
Domínio / Estado
   ↓
Regras
   ↓
Hardware
   ↓
Resultado
   ↓
Domínio
   ↓
UI
```

Essa separação deve permanecer como referência durante a evolução do simulador e durante a futura migração para o ESP32-S3.