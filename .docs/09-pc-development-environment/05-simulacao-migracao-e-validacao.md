# Simulação, migração e validação

[voltar ao índice](../09-pc-development-environment.md)

Esta parte reúne as seções que fazem a ponte entre o ambiente PC e os temas canônicos de simulação ([06-simulation-strategy.md](../06-simulation-strategy.md)), abstração de hardware ([05-hardware-abstraction.md](../05-hardware-abstraction.md)), migração ([11-migration-pc-to-esp32.md](../11-migration-pc-to-esp32.md)) e testes ([10-testing-strategy.md](../10-testing-strategy.md)).

---

## 48. Simulação de hardware

O PC não possui:

```text
HX711
Load Cell
SG90
```

Por isso, esses componentes são simulados.

A estrutura `hardware/` e as interfaces `WeightSensor`/`Dispenser` são detalhadas em [05-hardware-abstraction.md](../05-hardware-abstraction.md).

A estratégia de simulação (o que simular e como) é o tema de [06-simulation-strategy.md](../06-simulation-strategy.md).

---

## 49. Simulated Weight Sensor

A implementação simulada mantém internamente um peso:

```c
static int simulated_weight = 0;
```

Ela fornece operações como:

```c
read_grams()
add_grams()
reset()
```

Assim, o controller pode trabalhar com uma abstração semelhante à que utilizará no hardware real.

Interface e implementação simulada: [05-hardware-abstraction.md](../05-hardware-abstraction.md).

---

## 50. Simulated Dispenser

O dispenser simulado mantém um estado:

```c
static int active = 0;
```

e oferece:

```c
start()
stop()
is_active()
```

Isso representa conceitualmente:

```text
SG90/mecanismo
```

sem precisar de hardware.

Interface e implementação simulada: [05-hardware-abstraction.md](../05-hardware-abstraction.md).

---

## 51. Comportamento atual da simulação

A dosagem simulada funciona aproximadamente assim:

```text
controller.start()
      ↓
peso = 0
      ↓
dispenser = ativo
      ↓
controller.update() (≈ a cada 300 ms) → fase rápida (+20 g)
      ↓
... a partir de ~30 g da meta → fase fina (+2 g por update)
      ↓
... até peso >= objetivo
      ↓
dispenser.stop()
      ↓
COMPLETED
```

Detalhes do comportamento simulado: [06-simulation-strategy.md](../06-simulation-strategy.md).

---

## 52. Taxa aproximada da simulação

A simulação usa duas etapas por atualização, aproximadamente a cada `300 ms`:

```text
fase rápida: +20 g  → ≈ 66,7 g/s
fase fina:   +2 g   → ≈ 6,7 g/s
```

Essas taxas são apenas um modelo inicial e não devem ser interpretadas como a vazão real do futuro mecanismo.

Detalhes: [06-simulation-strategy.md](../06-simulation-strategy.md).

---

## 53. Por que a simulação é simples

O objetivo inicial não é reproduzir perfeitamente a física.

É validar:

```text
início
 ↓
aumento de peso
 ↓
atingimento da meta
 ↓
parada
 ↓
conclusão
```

Depois que o hardware físico produzir dados reais, o modelo poderá ser ajustado.

Detalhes: [06-simulation-strategy.md](../06-simulation-strategy.md).

---

## 54. Fluxo completo do simulador

O fluxo completo (`main.c` → `lv_init()`/`sdl_hal_init()`/`ui_init()` → Screen Manager → Home (seleção de modo) → Config → DosingController (com WeightSensor e Dispenser simulados) → Dosing → Completed) é apresentado de forma integrada em [07-ui-and-navigation.md](../07-ui-and-navigation.md) e [02-architecture.md](../02-architecture.md).

---

## 56. O que será mantido na migração

A migração para ESP32-S3 deverá preservar principalmente:

```text
domain/
ui/
conceitos de hardware abstraction/
fluxo da aplicação/
DosingConfig/
DosingController/
```

Enquanto elementos específicos do PC poderão ser substituídos:

```text
SDL2
SDL HAL
MSVC
Windows
```

Detalhes de migração: [11-migration-pc-to-esp32.md](../11-migration-pc-to-esp32.md).

---

## 57. O que será substituído

Na plataforma futura, `SDL2`/`SDL HAL` serão substituídos por display driver, touch driver e `ESP32 HAL`.

O `DosingController` deverá continuar conceitualmente o mesmo, e o hardware abstraído deverá passar de `simulated` para `real`.

Detalhes de migração: [11-migration-pc-to-esp32.md](../11-migration-pc-to-esp32.md).

---

## 58. PC como ambiente de validação

O simulador deve ser utilizado para validar mudanças antes de colocá-las no hardware:

```text
alteração no controller
        ↓
build
        ↓
simulação
        ↓
testes
        ↓
comportamento aprovado
        ↓
integração física
```

Isso reduz a quantidade de ciclos de tentativa diretamente no dispositivo.

---

## 59. Testes rápidos

Antes de considerar uma alteração concluída, o fluxo mínimo no PC deve verificar:

```text
1. CMake configura
2. Projeto compila
3. Executável inicia
4. Home aparece
5. Navegação funciona
6. Configuração funciona
7. Dosagem inicia
8. Peso simulado aumenta
9. Dispenser simulado para
10. Completed aparece
```

Os procedimentos de teste correspondentes estão detalhados em [10-testing-strategy.md](../10-testing-strategy.md).

---

## 62. Dependências externas não devem virar código da aplicação

O projeto depende de:

```text
LVGL
SDL2
vcpkg
```

mas isso não significa que a aplicação deve espalhar conhecimento dessas ferramentas por todo o código.

Por exemplo:

```text
SDL2
```

deve ficar concentrado principalmente na integração da plataforma.

Enquanto:

```text
DosingController
```

não deve depender de SDL2.

---

## 63. Fronteiras importantes

As principais fronteiras mantidas são: Plataforma PC (Windows/SDL2/HAL) → UI (LVGL) → Domain (`DosingController`) → Hardware Abstraction (`WeightSensor`/`Dispenser`) → Implementações Simuladas.

As camadas, a direção das dependências e o papel dessas fronteiras na estratégia de migração são detalhados em [02-architecture.md](../02-architecture.md).

---

## 64. Por que não desenvolver diretamente no ESP32

Desenvolver primeiro no PC oferece algumas vantagens:

* feedback visual rápido;
* debugging mais simples;
* compilação mais rápida;
* testes de navegação mais fáceis;
* possibilidade de alterar UI sem reflashear firmware;
* simulação de hardware;
* validação do controller antes da eletrônica;
* menor dependência de componentes físicos durante a fase inicial.

O hardware continua sendo necessário para validar aspectos que só existem fisicamente.

---

## 65. O que o simulador não consegue validar

O simulador não consegue reproduzir perfeitamente:

```text
ruído do HX711
vibração
fluxo real da ração
overshoot físico
atrito do mecanismo
limitações do SG90
picos de corrente
interferência elétrica
tempo real de estabilização
problemas mecânicos
```

Por isso, o simulador é uma etapa de desenvolvimento, não um substituto completo do protótipo físico.

As limitações e riscos da migração são detalhados em [11-migration-pc-to-esp32.md](../11-migration-pc-to-esp32.md).

---

## 66. Relação entre simulação e hardware

A estratégia é usar o PC para validar lógica, UI, arquitetura e comportamento básico; depois o hardware valida a física real, o modelo é ajustado e se volta ao simulador quando útil.

Essa abordagem permite que os dois ambientes se complementem.

Detalhes: [06-simulation-strategy.md](../06-simulation-strategy.md) e [11-migration-pc-to-esp32.md](../11-migration-pc-to-esp32.md).