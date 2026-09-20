## 08. Erros e checklists

[voltar ao índice](../13-agent-guide.md)

---

## 65. Ao encontrar um bug

Primeiro reproduzir.

Depois identificar:

```text
UI?
Screen Manager?
Domain?
Hardware abstraction?
Simulation?
HAL?
Build?
```

Só então alterar.

Evitar corrigir o sintoma na camada errada.

---

## 66. Exemplo de bug de arquitetura

Se o peso mostrado estiver errado:

Não começar imediatamente alterando:

```text
label LVGL
```

Verificar:

```text
sensor
 ↓
controller
 ↓
valor retornado
 ↓
UI
```

A UI pode estar correta e apenas exibindo um valor errado vindo do domínio.

---

## 67. Preserve comportamento já validado

Os seguintes comportamentos já foram testados:

```text
✓ configuração +
✓ configuração -
✓ iniciar dosagem
✓ peso aumentar
✓ atingir meta
✓ dispenser parar
✓ concluir
✓ cancelar
✓ resetar
✓ voltar para Home
✓ iniciar nova dosagem
```

Alterações futuras devem evitar regressões nesses caminhos.

---

## 68. Checklist antes de modificar

Antes de escrever código:

```text
[ ] Entendi o objetivo da alteração?
[ ] Sei em qual camada ela pertence?
[ ] Verifiquei a implementação atual?
[ ] Verifiquei a documentação relevante?
[ ] Sei se é funcionalidade atual ou futura?
[ ] Existe alguma implementação semelhante?
[ ] A mudança afeta o hardware futuro?
```

---

## 69. Checklist depois de modificar

```text
[ ] Código compila?
[ ] Aplicação inicia?
[ ] Fluxo afetado funciona?
[ ] Fluxo completo continua funcionando?
[ ] Cancelamento continua funcionando?
[ ] Reset continua funcionando?
[ ] Não introduzi dependência desnecessária?
[ ] Não quebrei a separação de camadas?
[ ] Documentação precisa ser atualizada?
```

---

## 70. Checklist específico para domínio

Se modificar `src/domain/`:

```text
[ ] Evitei LVGL?
[ ] Evitei SDL2?
[ ] Evitei Windows?
[ ] Evitei detalhes de GPIO?
[ ] A regra pertence realmente ao domínio?
[ ] O comportamento pode ser testado no PC?
[ ] A mudança continua válida no ESP32?
```

---

## 71. Checklist específico para hardware

Se modificar `src/hardware/`:

```text
[ ] A interface representa uma capacidade?
[ ] A implementação pode ser trocada?
[ ] A simulação continua funcionando?
[ ] A implementação física está isolada?
[ ] O comportamento de parada é seguro?
[ ] Há necessidade de calibração?
[ ] Há possibilidade de erro?
```

---

## 72. Checklist específico para UI

Se modificar `src/ui/`:

```text
[ ] A tela está apenas apresentando/interagindo?
[ ] A regra de negócio continua no domínio?
[ ] A navegação existente foi preservada?
[ ] O timer foi tratado corretamente?
[ ] O objeto/tela possui ciclo de vida adequado?
[ ] Os textos são suportados pela fonte?
```

---

## 73. Checklist específico para CMake

Se adicionar/remover arquivos:

```text
[ ] MAIN_SOURCES atualizado?
[ ] includes corretos?
[ ] build limpo funciona?
[ ] build incremental funciona?
[ ] nenhuma dependência externa foi adicionada sem necessidade?
```