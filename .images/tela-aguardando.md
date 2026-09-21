Com base no contexto do projeto do sistema de pesagem e dosagem e nas imagens fornecidas, aqui estão as representações em `#ASCII` das telas solicitadas.

O fluxo entre elas ocorre quando o sistema está no estado "Aguardando" e o usuário seleciona o modo "MASSA" na primeira tela, sendo direcionado para a segunda tela para definir os parâmetros da dosagem automática.

#ASCII
"""
TELA 1: AGUARDANDO (Seleção de Modo)

┌────────────────────────────────────────────┐
│ 10:30                              ıll 100%│
│ ≡      Pesagem e Dosagem                   │
│ ────────────────────────────────────────── │
│ ESTADO ATUAL                               │
│ AGUARDANDO                              ⏳ │
│                                            │
│ Selecione o modo de dosagem                │
│                                            │
│ ┌────────────────────────────────────────┐ │
│ │ ⚖️   MASSA                             │ │
│ │       Dosar por peso (g)               │ │
│ └────────────────────────────────────────┘ │
│                                            │
│ ┌────────────────────────────────────────┐ │
│ │ 💲   VALOR MONETÁRIO                   │ │
│ │       Dosar por valor (R$)             │ │
│ └────────────────────────────────────────┘ │
│                                            │
│                                            │
│    ✋ LIBERAÇÃO MANUAL DISPONÍVEL          │
│ ┌────────────────────────────────────────┐ │
│ │  ✋   LIBERAR MANUALMENTE              │ │
│ └────────────────────────────────────────┘ │
│ ────────────────────────────────────────── │
│ 🏠 Início  📄 Dosagens  🕒 Hist. ⚙️ Config │
└────────────────────────────────────────────┘
"""

#ASCII
"""
TELA 2: AGUARDANDO (Definição de Quantidade)
(Acessada ao clicar no botão "MASSA" da tela anterior)

┌────────────────────────────────────────────┐
│ 10:30                              ıll 100%│
│ ≡      Pesagem e Dosagem                   │
│ ────────────────────────────────────────── │
│ AGUARDANDO                                 │
│ Defina a quantidade                        │
│                                            │
│ ┌────────────────────┐┌──────────────────┐ │
│ │     MASSA (g)      ││    VALOR (R$)    │ │
│ └────────────────────┘└──────────────────┘ │
│                                            │
│ Quantidade desejada                        │
│ ┌─────┐                        ┌─────┐     │
│ │  -  │         500 g          │  +  │     │
│ └─────┘                        └─────┘     │
│                                            │
│ Valores rápidos                            │
│ ┌─────────┐  ┌─────────┐  ┌──────────────┐ │
│ │  100 g  │  │  250 g  │  │    500 g     │ │
│ └─────────┘  └─────────┘  └──────────────┘ │
│ ┌─────────┐  ┌─────────┐  ┌──────────────┐ │
│ │  750 g  │  │ 1.000 g │  │   2.000 g    │ │
│ └─────────┘  └─────────┘  └──────────────┘ │
│                                            │
│ ┌────────────────────────────────────────┐ │
│ │   ▶   INICIAR DOSAGEM AUTOMÁTICA       │ │
│ └────────────────────────────────────────┘ │
│ ┌────────────────────────────────────────┐ │
│ │                VOLTAR                  │ │
│ └────────────────────────────────────────┘ │
│ ────────────────────────────────────────── │
│ 🏠 Início  📄 Dosagens  🕒 Hist. ⚙️ Config │
└────────────────────────────────────────────┘
"""

### Resumo do Fluxo e Funcionamento:

1. **Tela 1:** O sistema inicia no estado "Aguardando". O usuário tem a opção de fazer uma **liberação manual** direta mantendo o botão verde inferior pressionado, ou pode escolher iniciar uma dosagem automática selecionando entre os botões **MASSA** ou **VALOR MONETÁRIO**.
2. **Tela 2:** Ao clicar em "MASSA", a tela de definição de quantidade é aberta. Nela, o usuário pode ajustar o valor desejado usando os botões de incremento/decremento `[ - ]` e `[ + ]`, ou selecionar rapidamente através da grade de **Valores rápidos**. Ao confirmar a quantidade, basta clicar em **INICIAR DOSAGEM AUTOMÁTICA** para que o ESP32 comece a atuar sobre o servo motor e ler a célula de carga, mudando o sistema para o estado "Dosando". Caso queira desistir, o botão **VOLTAR** retorna à tela inicial.

---

Aqui estão as representações em `#ASCII` atualizadas, agora detalhando as cores de fundo, cores dos textos, além da presença e das cores dos ícones de cada elemento da interface, com base nas imagens fornecidas.

O fundo geral do aplicativo em ambas as telas é **Escuro/Preto**.

#ASCII
"""
TELA 1: AGUARDANDO (Seleção de Modo)

┌──────────────────────────────────────────────────────────────┐
│ [Ícone: ≡ Menu / Cor: Branco]    [Texto: Branco]             │
│                                  Pesagem e Dosagem           │
│ ──────────────────────────────────────────────────────────── │
│ [Texto: Azul Escuro]                                         │
│ ESTADO ATUAL                                                 │
│                                                              │
│ [Texto: Azul Claro]              [Ícone: ⏳ Ampulheta]      │
│ AGUARDANDO                       [Cor do Ícone: Azul Claro]  │
│                                                              │
│ [Texto: Branco]                                              │
│ Selecione o modo de dosagem                                  │
│                                                              │
│ ┌──────────────────────────────────────────────────────────┐ │
│ │ [Fundo: Cinza Escuro]                                    │ │
│ │ [Ícone: ⚖️ Balança]             [Texto: Branco]           │ │
│ │ [Cor do Ícone: Azul Claro]     MASSA                     │ │
│ │                                [Texto: Cinza Claro]      │ │
│ │                                Dosar por peso (g)        │ │
│ └──────────────────────────────────────────────────────────┘ │
│                                                              │
│ ┌──────────────────────────────────────────────────────────┐ │
│ │ [Fundo: Cinza Escuro]                                    │ │
│ │ [Ícone: 💲 Cifrão ($)]          [Texto: Branco]          │ │
│ │ [Cor do Ícone: Azul Claro]     VALOR MONETÁRIO           │ │
│ │                                [Texto: Cinza Claro]      │ │
│ │                                Dosar por valor (R$)      │ │
│ └──────────────────────────────────────────────────────────┘ │
│                                                              │
│                                                              │
│ [Ícone: ✋ Mão] [Cor: Verde]     [Texto: Verde]              │
│                                  LIBERAÇÃO MANUAL DISPONÍVEL │
│ ┌──────────────────────────────────────────────────────────┐ │
│ │ [Fundo: Verde Escuro]                                    │ │
│ │ [Ícone: ✋ Mão]                [Texto: Branco]           │ │
│ │ [Cor do Ícone: Branco]         LIBERAR MANUALMENTE       │ │
│ └──────────────────────────────────────────────────────────┘ │
│ ──────────────────────────────────────────────────────────── │
│ [Ícone: 🏠 Casa]  [Ícone: 📄 Doc] [Ícone: 🕒 Rel.] [Ícone: ⚙️]│
│ [Cor: Azul]       [Cor: Cinza]    [Cor: Cinza]     [Cor: Cinz]│
│ Início (Azul)     Dosagens        Histórico        Config.    │
└──────────────────────────────────────────────────────────────┘
"""
(Referência visual obtida da tela "Aguardando" com o menu de seleção)

---

#ASCII
"""
TELA 2: AGUARDANDO (Definição de Quantidade)

┌──────────────────────────────────────────────────────────────┐
│ [Ícone: ≡ Menu / Cor: Branco]    [Texto: Branco]             │
│                                  Pesagem e Dosagem           │
│ ──────────────────────────────────────────────────────────── │
│ [Texto: Azul Claro]                                          │
│ AGUARDANDO                                                   │
│                                                              │
│ [Texto: Branco]                                              │
│ Defina a quantidade                                          │
│                                                              │
│ ┌────────────────────────┐      ┌────────────────────────┐   │
│ │ [Fundo: Azul Claro]    │      │ [Fundo: Transparente]  │   │
│ │ [Texto: Branco]        │      │ [Texto: Cinza Claro]   │   │
│ │ MASSA (g)              │      │ VALOR (R$)             │   │
│ └────────────────────────┘      └────────────────────────┘   │
│                                                              │
│ [Texto: Branco]                                              │
│ Quantidade desejada                                          │
│ ┌───────┐                        ┌───────┐                   │
│ │ [ - ] │    [Texto: Branco]     │ [ + ] │                   │
│ │ Fundo:│    500 g               │ Fundo:│                   │
│ │ Cinza │                        │ Cinza │                   │
│ └───────┘                        └───────┘                   │
│                                                              │
│ [Texto: Cinza Claro] Valores rápidos                         │
│ ┌───────────┐  ┌───────────┐  ┌───────────┐                  │
│ │ 100 g     │  │ 250 g     │  │ 500 g     │ <-[Fundo: Azul]  │
│ │ [Fundo:   │  │ [Fundo:   │  │ [Texto:   │                  │
│ │  Cinza]   │  │  Cinza]   │  │  Branco]  │                  │
│ └───────────┘  └───────────┘  └───────────┘                  │
│ ┌───────────┐  ┌───────────┐  ┌───────────┐                  │
│ │ 750 g     │  │ 1.000 g   │  │ 2.000 g   │                  │
│ │ [Fundo:   │  │ [Fundo:   │  │ [Fundo:   │                  │
│ │  Cinza]   │  │  Cinza]   │  │  Cinza]   │                  │
│ └───────────┘  └───────────┘  └───────────┘                  │
│                                                              │
│ ┌──────────────────────────────────────────────────────────┐ │
│ │ [Fundo: Verde Escuro]                                    │ │
│ │ [Ícone: ▶ Play/Seta]           [Texto: Branco]           │ │
│ │ [Cor do Ícone: Branco]         INICIAR DOSAGEM AUTOMÁTICA│ │
│ └──────────────────────────────────────────────────────────┘ │
│ ┌──────────────────────────────────────────────────────────┐ │
│ │ [Fundo: Cinza Escuro]                                    │ │
│ │ [Ícone: Sem Ícone]             [Texto: Cinza Claro]      │ │
│ │                                VOLTAR                    │ │
│ └──────────────────────────────────────────────────────────┘ │
│ ──────────────────────────────────────────────────────────── │
│ [Ícone: 🏠 Casa]  [Ícone: 📄 Doc] [Ícone: 🕒 Rel.] [Ícone: ⚙️]│
│ [Cor: Azul]       [Cor: Cinza]    [Cor: Cinza]     [Cor: Cinz]│
│ Início (Azul)     Dosagens        Histórico        Config.    │
└──────────────────────────────────────────────────────────────┘
"""
(Referência visual obtida da tela "Aguardando" após selecionar o modo massa)