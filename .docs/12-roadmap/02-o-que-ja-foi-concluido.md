## 1. O que já foi concluído

_Voltar ao índice: [`../12-roadmap.md`](../12-roadmap.md)._

---

### 1.1 Ambiente de desenvolvimento

Concluído:

```text
✓ Windows
✓ Visual Studio Build Tools
✓ MSVC
✓ CMake
✓ vcpkg
✓ SDL2
✓ LVGL
```

O projeto consegue ser configurado e compilado pelo CMake.

---

## 2. Repositório

Concluído:

```text
✓ Git inicializado
✓ histórico inicial criado
✓ projeto organizado
✓ .gitignore configurado
✓ estrutura de diretórios definida
```

O projeto possui seu próprio histórico Git.

---

## 3. Simulador desktop

Concluído:

```text
✓ aplicação C
✓ LVGL
✓ SDL2
✓ janela 800×480
✓ loop principal
✓ HAL do simulador
✓ execução do executável
```

O simulador já pode ser executado no PC.

---

## 4. Interface inicial

Concluído:

```text
✓ Home
✓ botão Iniciar
✓ status inicial
✓ navegação
```

A tela inicial serve como ponto de entrada da aplicação.

---

## 5. Seleção de modo

Concluído:

```text
✓ seleção de quantidade fixa
✓ seleção de porções
✓ retorno para Home
```

O usuário consegue escolher como deseja configurar a dosagem.

---

## 6. Configuração de quantidade fixa

Concluído:

```text
✓ valor em gramas
✓ botão +
✓ botão -
✓ incremento de 10 g
✓ limite mínimo de 10 g
✓ valor inicial de 100 g
```

O valor é armazenado em:

```c
DosingConfig
```

---

## 7. Configuração porções

Concluído:

```text
✓ quantidade de porções
✓ botão +
✓ botão -
✓ incremento de 1
✓ limite mínimo de 1
✓ valor inicial de 1
```

A configuração também é armazenada no domínio através de:

```text
DosingConfig
```

---

## 8. Controller de dosagem

Concluído:

```text
✓ DosingController
✓ inicialização
✓ start
✓ update
✓ cancel
✓ leitura do peso
✓ leitura do estado
```

O controller já coordena o sensor de peso e o dispenser simulado.

---

## 9. Abstração de hardware

Concluído:

```text
✓ WeightSensor
✓ Dispenser
✓ implementação simulada do WeightSensor
✓ implementação simulada do Dispenser
```

A arquitetura passou a distinguir:

```text
capacidade do hardware
```

de:

```text
implementação do hardware
```

---

## 10. Sensor de peso simulado

Concluído:

```text
✓ leitura em gramas
✓ incremento de peso
✓ reset
```

O peso atualmente aumenta de forma determinística durante a dosagem.

---

## 11. Dispenser simulado

Concluído:

```text
✓ start
✓ stop
✓ is_active
```

O dispenser possui um estado simples:

```text
ativo
ou
parado
```

---

## 12. Dosagem simulada

Concluído:

```text
✓ peso começa em 0
✓ dispenser inicia
✓ peso aumenta
✓ meta é verificada
✓ dispenser para
✓ estado muda para COMPLETED
```

Atualmente a simulação adiciona:

```text
2 g
```

a cada atualização de aproximadamente:

```text
300 ms
```

---

## 13. Cancelamento

Concluído:

```text
✓ botão Cancelar
✓ dispenser é parado
✓ controller retorna para IDLE
✓ aplicação retorna para Home
```

Esse comportamento será especialmente importante quando o dispenser real for integrado.

---

## 14. Reset

Concluído:

```text
✓ peso é resetado ao iniciar nova dosagem
```

Isso permite executar ciclos consecutivos sem carregar o peso da dosagem anterior.

---

## 15. Tela de conclusão

Concluído:

```text
✓ Completed
✓ indicação da conclusão
✓ Nova dosagem
✓ Voltar ao início
```

O ciclo completo pode ser repetido.

---

## 16. Testes manuais já realizados

Já foram verificados:

```text
✓ build
✓ execução
✓ navegação
✓ configuração +
✓ configuração -
✓ dosagem
✓ aumento do peso
✓ parada do dispenser
✓ reset
✓ cancelamento
✓ retorno para Home
✓ Completed
✓ Nova dosagem
✓ ciclo completo repetido
```

Esses testes representam o caminho principal atualmente implementado.

---

## 17. Documentação

A documentação arquitetural também já está sendo construída.

Documentos concluídos:

```text
00-project-story.md
01-project-overview.md
02-architecture.md
03-architecture-decisions.md
04-domain-and-state-machine.md
07-ui-and-navigation.md
08-target-hardware.md
09-pc-development-environment.md
10-testing-strategy.md
11-migration-pc-to-esp32.md
```

Esses documentos preservam não apenas o código atual, mas também o raciocínio por trás das decisões.

---

## 18. Referências canônicas

Os detalhes das etapas concluídas acima são tratados em seus documentos canônicos:

- [`09-pc-development-environment.md`](../09-pc-development-environment.md) — ambiente de desenvolvimento, simulador desktop, build e execução no PC.
- [`07-ui-and-navigation.md`](../07-ui-and-navigation.md) — telas e navegação (Home, seleção de modo, configuração, dosagem, conclusão).
- [`04-domain-and-state-machine.md`](../04-domain-and-state-machine.md) — controller, configuração e regras de dosagem e cancelamento.
- [`05-hardware-abstraction.md`](../05-hardware-abstraction.md) — `WeightSensor`, `Dispenser` e implementações simuladas.
- [`06-simulation-strategy.md`](../06-simulation-strategy.md) — simulação do peso (2 g a cada ~300 ms) e do dispenser.
- [`10-testing-strategy.md`](../10-testing-strategy.md) — testes manuais verificados e critérios do caminho principal.
- [`README.md`](../README.md) — índice geral da documentação.