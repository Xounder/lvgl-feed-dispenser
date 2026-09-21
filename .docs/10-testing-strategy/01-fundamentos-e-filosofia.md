# Fundamentos e filosofia de testes

> _Voltar ao índice: [`../10-testing-strategy.md`](../10-testing-strategy.md)._

---

## 1. Objetivo

Este documento define como o projeto deve ser testado durante sua evolução no simulador PC e, posteriormente, no ESP32-S3.

O objetivo não é apenas verificar se a interface "abre e funciona".

Os testes devem validar o comportamento completo do sistema:

```text
Entrada do usuário
       ↓
Configuração
       ↓
Início da dosagem
       ↓
Leitura do peso
       ↓
Controle do dispenser
       ↓
Atingimento da meta
       ↓
Parada
       ↓
Conclusão
```

Também devem ser testadas situações anormais:

```text
cancelamento
sensor sem progresso
peso acima da meta
timeout
falha de hardware
configuração inválida
```

A estratégia deve evoluir junto com o projeto.

---

## 2. Filosofia de testes

O projeto utiliza uma abordagem de desenvolvimento incremental.

A sequência recomendada é:

```text
Implementar
    ↓
Compilar
    ↓
Executar no simulador
    ↓
Testar comportamento normal
    ↓
Testar casos de erro
    ↓
Corrigir
    ↓
Repetir
```

Quando uma funcionalidade estiver suficientemente estável no PC:

```text
Simulador
    ↓
Hardware real
    ↓
Testes físicos
```

---

## 3. O que deve ser testado

Os testes podem ser divididos em cinco grupos:

```text
1. Build
2. UI e navegação
3. Domínio
4. Hardware simulado
5. Integração
```

Posteriormente será acrescentado:

```text
6. Hardware real
```

---

## 4. Testes de build

Antes de testar o comportamento, o projeto deve compilar.

O mínimo esperado é:

```text
CMake configura
        ↓
Compilação concluída
        ↓
main.exe gerado
```

Uma alteração não deve ser considerada concluída se introduzir erros de compilação.

---

## 5. Teste de inicialização

Ao executar:

```text
bin/Debug/main.exe
```

deve ocorrer:

```text
LVGL inicializa
      ↓
SDL2 inicializa
      ↓
UI inicializa
      ↓
Home aparece
```

### Resultado esperado

A janela do simulador deve abrir sem encerramento inesperado.

A tela inicial deve estar disponível.

---

## 6. Teste da tela Home

### Cenário

Iniciar o simulador.

### Esperado

A tela deve apresentar (Home / TELA 1):

```text
Pesagem e Dosagem

AGUARDANDO
Selecione o modo de dosagem

[ MASSA ]           → Dosar por peso (g)
[ VALOR MONETARIO ] → Dosar por valor (R$)

[ LIBERAR MANUALMENTE ] (adiantado por LED verde)

[ Inicio ] [ Dosagens ] [ Historico ] [ Config. ]
```

O botão `Iniciar` deve levar para a seleção de modo.

---

## 7. Teste de navegação

O fluxo principal deve ser:

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

Cada transição deve ocorrer sem:

* crash;
* tela vazia inesperada;
* objetos duplicados;
* estado incorreto;
* retorno para tela errada.

---

## 8. Teste de retorno

Nas telas que possuem navegação de retorno, deve ser possível voltar sem alterar indevidamente a configuração.

Exemplo:

```text
Home
 ↓
Mode
 ↓
Config
 ↓
Home
```

Depois:

```text
Home
 ↓
Mode
```

deve continuar sendo possível iniciar uma nova configuração.