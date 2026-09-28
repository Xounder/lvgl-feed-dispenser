INTRODUÇÃO
A pesagem e a dosagem de produtos estão presentes em diversas aplicações do cotidiano e em processos comerciais e industriais, nos quais é necessário disponibilizar uma quantidade determinada de um produto de maneira controlada. Entre essas aplicações, destaca-se a comercialização de ração, em que a quantidade fornecida pode ser definida de acordo com a massa desejada ou com o valor que o usuário pretende gastar. 
Nesse contexto, o projeto propõe o desenvolvimento de um protótipo de sistema embarcado para representar o armazenamento, a dosagem e a pesagem de ração. O sistema permitirá que o usuário defina a quantidade desejada, realizando a liberação do produto de forma controlada até atingir o valor solicitado. Também será possível interromper o processo e realizar a liberação manual. Dessa forma, o projeto busca aplicar conceitos de sistemas embarcados na representação de uma aplicação prática de controle e dosagem (CURTO CIRCUITO, 2019).

OBJETIVOS
Objetivo Geral: Desenvolver um protótipo físico e funcional de um sistema automatizado de pesagem e dosagem, controlado por um sistema embarcado capaz de realizar a liberação de um produto com base na massa ou valor monetário solicitado pelo usuário. 
Objetivos Específicos:
Representar fisicamente as etapas de armazenamento, liberação, recebimento e pesagem da ração.
Permitir ao usuário selecionar o modo de dosagem, por massa ou por valor monetário, e realizar automaticamente a liberação da ração de acordo com a solicitação.
Monitorar a massa de ração efetivamente recebida no recipiente durante o processo.
Apresentar visualmente ao usuário a massa medida e o estado da operação.
Permitir a interrupção da dosagem automática e a liberação manual da ração.
Permitir o encerramento de uma operação e a preparação do sistema para uma nova dosagem.
Avaliar o funcionamento e a repetibilidade do sistema por meio de testes e demonstrações.

DESCRIÇÃO GERAL DA SOLUÇÃO PROPOSTA
3.1 Visão Geral e Funcionamento do Sistema


A solução inicialmente proposta consiste no desenvolvimento de um protótipo físico de um sistema embarcado para pesagem e dosagem de ração. O protótipo será composto por um reservatório destinado ao armazenamento da ração, um mecanismo responsável por controlar sua liberação, um recipiente para recebimento e uma estrutura de suporte. Parte da estrutura e do mecanismo de liberação será produzida por impressão 3D, enquanto componentes comerciais, como o reservatório e o recipiente, poderão ser utilizados para facilitar a construção e a manutenção do protótipo.
O funcionamento do sistema terá início a partir da interação do usuário com uma tela touch, por meio da qual será possível selecionar o modo de dosagem por massa ou por valor monetário e informar a quantidade desejada. A interface também apresentará informações referentes ao processo, como a massa medida e o estado da operação.
O ESP32 será utilizado como unidade central de controle do sistema, realizando a comunicação entre a interface do usuário, o sistema de pesagem e o mecanismo de liberação (EMBARCADOS, 2026). Para a medição, será utilizada uma célula de carga instalada sob a plataforma que sustenta o recipiente de recebimento. O sinal proveniente da célula de carga será tratado pelo módulo HX711, permitindo que o microcontrolador obtenha a informação da massa de ração efetivamente recebida durante a dosagem (CURTO CIRCUITO, 2019; SEIDLE, 2014).
Mecanismo de abertura e controle da dosagem
A partir da massa medida, o ESP32 realizará o controle do mecanismo de liberação, acionado por um servo motor (TOWER PRO, 2026). O mecanismo terá duas formas principais de atuação durante a dosagem automática.
Inicialmente, quando a massa medida estiver distante da quantidade desejada, o servo permanecerá em uma posição de maior abertura, permitindo uma liberação mais rápida da ração. À medida que a massa se aproximar do valor solicitado, o servo será movimentado para uma posição de abertura reduzida, permitindo uma liberação mais lenta e controlada.
Quando a massa atingir a condição estabelecida para a dosagem, o servo será posicionado de forma a fechar o mecanismo e interromper a liberação da ração. Essa estratégia busca proporcionar maior controle sobre a quantidade final dosada, considerando que parte da ração pode continuar em movimento mesmo após o fechamento do mecanismo.
Figura 2: Estratégia de abertura do servo 
A configuração física também será projetada de modo que a ração efetivamente recebida pelo recipiente seja pesada durante o processo. Dessa forma, a informação fornecida pela célula de carga será utilizada como referência para o acompanhamento e controle da dosagem.

Operação da dosagem automática
Na operação automática, o sistema permanecerá inicialmente no estado Aguardando, aguardando a interação do usuário. O usuário selecionará o modo de dosagem, por massa ou por valor monetário, e informará a quantidade desejada por meio da tela touch. Após a confirmação da solicitação, o sistema entrará no estado Dosando. O ESP32 acionará o servo motor para permitir a passagem da ração, enquanto a célula de carga realizará continuamente a medição da massa presente no recipiente.
Durante o processo, o sistema acompanhará o aumento da massa medida. Quando a massa estiver próxima da quantidade desejada, o mecanismo de liberação será progressivamente fechado para reduzir a vazão. Ao atingir a condição estabelecida, a liberação será interrompida e o sistema passará para o estado Concluído.
O usuário também poderá interromper a operação antes de sua conclusão por meio do comando de parada disponível na interface ou pelo dispositivo físico de emergência. Após o encerramento da operação, o sistema poderá ser preparado para uma nova dosagem.
Figura 3: Fluxograma da dosagem automática 
Liberação manual
Além da dosagem automática, o protótipo contará com uma função de liberação manual direta da ração. Essa função permitirá que o usuário controle diretamente a abertura do mecanismo por meio de um botão físico.
A liberação manual estará disponível somente quando não houver uma dosagem automática em andamento. Ao pressionar o botão de liberação manual, o ESP32 acionará o servo motor para abrir o mecanismo e permitir a passagem da ração. Enquanto o botão permanecer pressionado, o mecanismo permanecerá aberto. Ao soltar o botão, o servo retornará à posição fechada, interrompendo a liberação. Durante essa operação, um LED indicador permanecerá aceso para sinalizar visualmente que o modo de liberação manual está ativo.

Figura 4: Fluxograma da liberação manual 
 Interrupção e segurança da operação
O protótipo também contará com um botão físico de emergência, destinado à interrupção da operação quando necessário. O comando de interrupção deverá possuir prioridade sobre a dosagem automática, permitindo interromper a liberação da ração mesmo durante uma operação em andamento. Além disso, será utilizada uma chave geral para ligar e desligar o sistema. Esses elementos fornecem ao protótipo formas adicionais de controle e interrupção, contribuindo para uma operação mais segura durante os testes e demonstrações.

3.2 Protótipo proposto
	A Figura  apresenta a representação do protótipo proposto para o sistema de pesagem e dosagem. O equipamento integra o reservatório de armazenamento, o mecanismo de liberação do produto, o recipiente para coleta, o sistema de pesagem e a interface de controle. A estrutura foi projetada de modo a reunir os componentes necessários para a realização da dosagem automática e permitir a interação do usuário com o sistema. 
Figura 5 – Representação do protótipo proposto para o sistema de pesagem e dosagem. 
3.3 Interface do usuário 
A interface do usuário será desenvolvida para permitir o acompanhamento e o controle do processo de dosagem de forma simples e intuitiva (LVGL, 2026). Conforme apresentado na Figura 6, o sistema possui diferentes estados de operação: Aguardando, Dosando, Concluído e Interrompido. No estado Aguardando, o usuário pode selecionar o tipo de dosagem, por massa ou valor monetário, e definir a quantidade desejada. Durante o estado Dosando, são apresentados a massa atual e o progresso da operação, sendo a liberação manual desabilitada enquanto o controle automático estiver ativo. Ao término da operação, o sistema passa para o estado Concluído, disponibilizando uma nova dosagem e a liberação manual. Caso a operação seja interrompida, a interface apresenta o estado Interrompido e a massa parcial dosada. 

Figura 6 – Interfaces do sistema nos diferentes estados de operação. 

ATENDIMENTO AOS REQUISITOS DO PROJETO

Código
Requisito
Proposta
RP01
Possuir um reservatório para armazenamento do produto. 
Será utilizado um reservatório cilíndrico plástico transparente, adquirido comercialmente, instalado na parte superior da estrutura. A transparência permitirá visualizar a quantidade de produto armazenada.
RP02
Permitir abastecimento e reabastecimento.
O reservatório possuirá tampa removível, permitindo seu abastecimento e reposição do produto entre as demonstrações. 
RP03
Possuir mecanismo para promover e interromper fisicamente a liberação. 


Será desenvolvido um mecanismo de abertura e fechamento, produzido por impressão 3D e acionado por um servo motor. Durante a dosagem automática, serão utilizadas duas estratégias de controle: quando a massa medida estiver distante do valor desejado, o servo manterá uma abertura maior, permitindo a liberação mais rápida da ração; à medida que a massa se aproximar da quantidade solicitada, o servo reduzirá gradualmente sua abertura, diminuindo a vazão e permitindo maior controle da quantidade liberada. Ao atingir a massa desejada, o servo fechará completamente o mecanismo, interrompendo a liberação da ração.
RP04
Possuir região para posicionamento do recipiente.
Será construída uma plataforma de recebimento/pesagem localizada diretamente abaixo do mecanismo de liberação, onde será colocado o recipiente que receberá o produto. 
RP05
Permitir a pesagem do produto efetivamente recebido.
Será utilizada uma plataforma de pesagem independente, composta por quatro células de carga de 5 kg, distribuídas nos quatro pontos de apoio da plataforma. O recipiente destinado ao recebimento da ração será apoiado sobre essa estrutura, de modo que o peso do conjunto seja distribuído entre as quatro células de carga. Os sinais provenientes das células serão encaminhados ao módulo HX711, responsável pela amplificação e conversão dos sinais elétricos, permitindo que o ESP32 obtenha a informação correspondente à massa medida. Dessa forma, durante a dosagem, à medida que a ração for liberada do reservatório e acumulada no recipiente, o sistema acompanhará continuamente o aumento da massa recebida. A informação obtida pela plataforma de pesagem será utilizada pelo ESP32 para controlar a abertura do servo motor e determinar o momento de redução da vazão e de encerramento da dosagem.
RP06
Permitir visualizar e demonstrar o processo físico.
A estrutura será projetada de maneira aberta e utilizará um reservatório transparente, permitindo visualizar o armazenamento, a liberação, a queda do produto, o recebimento e a pesagem.
RP07
Permitir retirar o produto e reposicionar/substituir o recipiente.
O recipiente será removível e a plataforma de pesagem permanecerá acessível, permitindo retirar o produto e preparar o sistema para uma nova operação, evitando que a massa da operação anterior seja considerada.
RP08
Possuir dispositivo geral de liga/desliga. 
Será instalada uma chave geral física na estrutura do protótipo, responsável pelo controle da alimentação elétrica do sistema. Na posição ligada, a chave permitirá o fornecimento de energia ao ESP32, ao servo motor, ao sistema de pesagem e aos demais componentes elétricos. Na posição desligada, a alimentação será interrompida, desligando o sistema e os atuadores. A chave será instalada em local de fácil acesso, permitindo ligar ou desligar completamente o protótipo antes, durante ou após sua utilização. 
RS01
Selecionar dosagem por massa ou valor monetário.
Será utilizada uma tela touch com interface gráfica, apresentando as opções “Massa” e “Valor (R$)”. O usuário selecionará o modo antes de iniciar a operação.
RS02
Informar a massa desejada.
No modo massa, a tela apresentará um campo numérico para que o usuário informe a quantidade desejada em gramas. O valor será enviado ao ESP32 para ser utilizado como referência.
RS03
Informar o valor monetário desejado.
No modo monetário, a interface apresentará um campo para o usuário informar o valor desejado em reais.
RS04
Utilizar preço de referência por unidade de massa.
Será definido pela equipe um preço fixo por unidade de massa, armazenado no software do ESP32. Esse valor será utilizado para converter o valor monetário informado em uma massa correspondente.
RS05
Determinar a massa correspondente ao valor informado.
Ao selecionar o modo monetário, o ESP32 realizará a conversão entre valor e massa utilizando o preço de referência definido pela equipe. A massa calculada será utilizada como referência para a dosagem.
RS06
Iniciar automaticamente a liberação após o comando de início.
Após o usuário confirmar a quantidade e pressionar “Iniciar”, o ESP32 acionará o servo motor, iniciando automaticamente a liberação do produto.
RS07
Monitorar a massa recebida durante a dosagem.
Durante a liberação da ração, o recipiente de recebimento permanece apoiado sobre a plataforma de pesagem, fazendo com que o peso do recipiente e da ração seja transmitido às células de carga. À medida que a ração é acumulada no recipiente, a massa medida aumenta, provocando uma variação no sinal elétrico gerado pelas células de carga. Esse sinal é encaminhado ao módulo HX711, responsável por amplificá-lo e convertê-lo em um sinal digital que pode ser interpretado pelo ESP32. Dessa forma, o ESP32 recebe continuamente os valores de massa medidos e utiliza essas informações para acompanhar o andamento da dosagem e determinar os ajustes necessários na abertura do mecanismo de liberação.
RS08
Apresentar visualmente a massa atualizada.
A tela touch exibirá continuamente a massa medida, permitindo ao usuário acompanhar o aumento da quantidade durante a dosagem.
RS09
Controlar e interromper automaticamente a liberação.
O ESP32 utilizará a massa medida como referência para controlar o servo. Será utilizada uma abertura maior quando a massa estiver distante do alvo e uma abertura progressivamente menor quando estiver próxima, seguida do fechamento do mecanismo ao atingir a condição definida.
RS10
Apresentar os estados Aguardando, Dosando e Concluído.
A interface possuirá diferentes estados visuais: Aguardando, antes do início; Dosando, durante a liberação; e Concluído, após o encerramento automático.
RS11
Permitir a interrupção manual da dosagem.
Durante uma dosagem automática, o usuário poderá interromper a operação de duas formas: pelo botão “Parar” apresentado na tela touch ou pelo botão físico de emergência. Ao receber qualquer um desses comandos, o ESP32 deverá interromper imediatamente o processo de dosagem e enviar um comando ao servo motor para fechar o mecanismo de liberação, impedindo a continuidade da saída de ração. A massa já recebida permanecerá no recipiente e será mantida como informação da operação interrompida. O sistema deverá sinalizar na tela que a dosagem foi interrompida e permanecer em estado de parada até que o usuário realize o procedimento previsto para iniciar uma nova operação. O comando de interrupção terá prioridade sobre o controle automático do servo, de modo que o mecanismo não volte a abrir enquanto a interrupção estiver ativa.


RS12
Dar prioridade à interrupção manual.
O software será desenvolvido de modo que o comando de parada tenha prioridade sobre a operação automática. Quando acionado, o ESP32 deverá interromper o acionamento do mecanismo de liberação.
RS13
Encerrar a operação e preparar uma nova dosagem.
Após a conclusão ou interrupção de uma operação, a interface disponibilizará a opção “Nova dosagem”. Ao selecionar essa opção, o sistema encerrará a operação anterior e realizará a tara da plataforma de pesagem, utilizando a leitura do recipiente vazio como referência de zero. Em seguida, o usuário poderá informar uma nova quantidade e iniciar outra dosagem, sem a necessidade de desligar o protótipo.
RS14
Permitir liberação manual direta.
Será utilizado um botão físico de liberação manual. Enquanto o botão estiver acionado, o ESP32 manterá o mecanismo aberto. Ao soltar o botão, o mecanismo será fechado. Nesse modo, a massa solicitada não será utilizada como condição automática de parada.
RS15
Impedir liberação manual durante dosagem automática.
O software será estruturado com diferentes estados de operação, como “Aguardando”, “Dosando”, “Concluído” e “Interrompido”. No estado “Aguardando”, o comando de liberação manual estará disponível ao usuário. Quando uma dosagem automática for iniciada, o sistema passará para o estado “Dosando”, no qual o comando de liberação manual será desabilitado, impedindo que o usuário acione o servo de forma manual enquanto o controle automático estiver em andamento. Durante esse estado, o servo será controlado exclusivamente pelo ESP32 de acordo com a massa medida. Após a conclusão ou interrupção da dosagem, o sistema alterará seu estado e somente permitirá a liberação manual quando estiver novamente em uma condição segura para essa operação.
RS16
Indicar visualmente o modo de liberação manual.
Durante a liberação manual, o ESP32 acionará simultaneamente o servo motor e o LED indicador. Enquanto o comando de liberação manual permanecer ativo, o servo manterá o mecanismo de dosagem aberto e o LED permanecerá aceso, indicando visualmente que o sistema está operando em modo manual. Quando o comando for encerrado, o ESP32 determinará o fechamento do mecanismo e desligará o LED, sinalizando o término da liberação manual


MATERIAIS E COMPONENTES
Item
Especificação
Quantidade
Função no projeto
Microcontrolador ESP32
Microcontrolador com conectividade Wi-Fi/Bluetooth e entradas/saídas digitais
1
Unidade central de controle, responsável pelo processamento das informações e pela comunicação entre a tela, sistema de pesagem, servo motor e comandos físicos.
Tela Touch
Display TFT Touch 4.3" ou 5"
1
Permitir a interação do usuário com o sistema, seleção do modo de dosagem, entrada da quantidade e acompanhamento da operação.
Célula de Carga
Tipo barra, capacidade a definir conforme a faixa operacional do protótipo
4
Realizar a medição da massa da ração recebida no recipiente.
Amplificador HX711
Amplificador/conversor para célula de carga
1
Amplificar e converter o sinal proveniente da célula de carga para permitir sua leitura pelo ESP32.
Atuador Mecânico
Servo motor SG90
1
Acionar o mecanismo de abertura e fechamento da passagem da ração.
Reservatório
Cilíndrico, plástico transparente, com tampa
1
Armazenar a ração durante o processo de dosagem.
Chave Geral
Interruptor gangorra (tic-tac) 10A
1
Desligamento geral e corte de alimentação.
Botões Físicos
Botões pulsadores (Push button)
2
Comandos físicos de interrupção (emergência) e liberação manual.
Indicador Visual
LED genérico 5mm com resistor
1
Indicar visualmente o acionamento do modo de liberação manual.
Fonte de alimentação
Fonte DC compatível com ESP32, tela e atuadores
1
Fornecer energia elétrica aos componentes do sistema, garantindo alimentação adequada para o funcionamento do ESP32, tela e atuadores.
Fonte Ajustável Para Protoboard Mb102
Módulo de alimentação para protoboard MB102, com saídas reguladas de 3,3 V e 5 V
1
Fornecer alimentação regulada para os componentes eletrônicos conectados à protoboard durante a montagem e os testes do circuito.
Cabos e fios
Fios para alimentação e sinais, bitolas adequadas
1 conjunto
Realizar as conexões elétricas entre os componentes.
Protoboard/placa de montagem
Compatível com os componentes eletrônicos
1
Facilitar a montagem e os testes do circuito eletrônico durante o desenvolvimento.
Parafusos, porcas e elementos de fixação
Dimensões definidas conforme o projeto mecânico
-
Fixar e montar as partes da estrutura e dos componentes.
Ração
Produto granulado/seco para utilização no protótipo
1 Kg
Material utilizado nas demonstrações do processo de armazenamento, dosagem e pesagem.
Estrutura Base
Peças impressas em 3D
Vários
Suporte principal, plataforma de pesagem, porta dosadora e painel.
Filamento para impressão 3D
PLA 
1 rolos de filamento PLA 
Produzir a estrutura, suportes, plataforma e mecanismo de liberação.
Recipiente
Vasilha plástica comercial
1
Recebimento do produto dosado.

ESTRATÉGIA DE TESTES E DEMONSTRAÇÃO
Funcionalidade/situação de teste
Estratégia de teste e demonstração
Resultado esperado
Inicialização do sistema
Ligar o protótipo pela chave geral e observar a inicialização do ESP32, da tela, do sistema de pesagem e do servo. 
O sistema deverá inicializar corretamente, apresentar a interface principal e permanecer no estado “Aguardando”, com o mecanismo de liberação fechado.
Estado inicial do mecanismo. 
Verificar fisicamente a posição do mecanismo após a inicialização e após o encerramento de uma operação. 
O mecanismo deverá permanecer fechado quando não houver uma liberação ativa. 
Seleção do modo de dosagem. 
Selecionar na tela touch os modos massa e valor monetário e informar uma quantidade válida para cada caso. 
O sistema deverá aceitar os dois modos e direcionar a operação de acordo com a opção selecionada. 
Entrada de dados. 
Informar diferentes valores de massa e de valor monetário pela tela, incluindo valores dentro da faixa operacional definida. 
O sistema deverá registrar corretamente os valores informados e permitir o início da operação somente quando os dados forem válidos. 
Entrada de valor inválido. 
Tentar iniciar uma operação sem informar quantidade, com valor igual a zero ou com valor fora da faixa operacional definida. 
O sistema deverá impedir o início da dosagem e apresentar uma indicação de que o valor informado é inválido. 
Tara da plataforma. 
Colocar o recipiente vazio sobre a plataforma e selecionar “Nova dosagem” ou a função de tara. 
O sistema deverá utilizar a massa do recipiente como referência de zero, passando a considerar apenas a massa de ração adicionada durante a nova operação. 
Verificação da tara. 
Realizar a tara com o recipiente vazio e observar a massa apresentada antes da liberação da ração. 
A interface deverá apresentar valor próximo de 0 g, dentro da tolerância definida para o sistema.
Dosagem automática por massa. 
Informar uma massa desejada, realizar a tara, iniciar a operação e acompanhar a liberação da ração. 
O sistema deverá liberar a ração automaticamente e encerrar a operação ao atingir a massa solicitada, respeitando a margem de erro estabelecida.
Dosagem automática por valor monetário. 
Informar um valor financeiro em reais e iniciar a operação. O valor será convertido em massa utilizando o preço de referência definido no código. 
O sistema deverá converter corretamente o valor monetário em massa e realizar a dosagem correspondente. 
Conversão de valor para massa. 
Utilizar valores monetários conhecidos e conferir manualmente a massa esperada a partir do preço de referência utilizado no código. 
A massa calculada pelo sistema deverá corresponder ao valor monetário informado, considerando a precisão definida para o projeto. 
Monitoramento da massa. 
Realizar uma dosagem observando continuamente o valor apresentado na tela enquanto a ração é liberada. 
A massa exibida deverá ser atualizada durante a operação e aumentar conforme a ração for adicionada ao recipiente. 
Controle da etapa rápida. 
Iniciar uma dosagem com a massa medida significativamente abaixo da massa desejada e observar a abertura do mecanismo. 
O servo deverá manter uma abertura maior, permitindo uma liberação mais rápida da ração durante a etapa inicial. 
Controle da etapa fina. 
Durante uma dosagem, observar o comportamento do mecanismo quando a massa estiver próxima do valor solicitado. 
O servo deverá reduzir a abertura, diminuindo a vazão para permitir maior controle da quantidade liberada. 
Encerramento automático. 
Realizar uma dosagem até atingir a quantidade solicitada e observar o comportamento do servo e da interface. 
O mecanismo deverá fechar, a liberação deverá ser interrompida e a interface deverá indicar que a dosagem foi concluída. 
Excesso de massa / ultrapassagem do alvo. 
Realizar testes próximos ao valor solicitado e observar quanto a massa ultrapassa o alvo após o fechamento do mecanismo. 
A quantidade final deverá permanecer dentro da margem de erro definida para o protótipo.
Interrupção pela tela. 
Iniciar uma dosagem automática e pressionar o botão “Parar” durante a liberação. 
O ESP32 deverá interromper a dosagem, fechar o mecanismo e sinalizar na interface que a operação foi interrompida. 
Interrupção pelo botão de emergência. 
Iniciar uma dosagem automática e acionar o botão físico de emergência. 
A liberação deverá ser interrompida e o mecanismo deverá permanecer fechado, com prioridade sobre o controle automático.
Prioridade da interrupção. 
Acionar o comando de parada ou emergência em diferentes momentos da dosagem, inclusive durante a etapa rápida e durante a etapa fina. 
Em qualquer momento, o comando de interrupção deverá ter prioridade e impedir a continuidade da liberação automática. 
Liberação manual direta. 
Com o sistema no estado “Aguardando”, pressionar e manter pressionado o botão de liberação manual. 
O servo deverá abrir o mecanismo enquanto o comando estiver ativo e o LED indicador deverá permanecer aceso. 
Encerramento da liberação manual. 
Soltar o botão de liberação manual durante a operação. 
O servo deverá retornar à posição fechada e o LED indicador deverá ser desligado. 
Bloqueio da liberação manual durante dosagem. 
Iniciar uma dosagem automática e tentar acionar o comando de liberação manual. 
O comando manual deverá permanecer bloqueado e não deverá alterar a posição do servo durante a dosagem automática.
Indicação do modo manual. 
Acionar a liberação manual e observar o LED indicador e a interface. 
O LED deverá indicar visualmente que a liberação manual está ativa e a interface deverá apresentar o estado correspondente, se previsto. 
Nova dosagem após conclusão. 
Finalizar uma dosagem, selecionar “Nova dosagem”, preparar o recipiente e iniciar uma nova solicitação sem desligar o protótipo. 
O sistema deverá encerrar a operação anterior, permitir a tara e possibilitar uma nova dosagem normalmente. 
Nova dosagem após interrupção. 
Interromper uma dosagem, selecionar “Nova dosagem”, realizar a preparação e iniciar uma nova operação. 
O sistema deverá sair do estado de interrupção, realizar a preparação necessária e permitir uma nova dosagem. 
Calibração da plataforma de pesagem. 
Posicionar massas conhecidas, como 100 g, 500 g e 1 kg, sobre a plataforma e comparar os valores medidos com as massas padrão. 
Os valores apresentados deverão permanecer dentro da margem de erro definida para o sistema, permitindo avaliar a precisão da pesagem.
Teste sem recipiente. 
Verificar a leitura da plataforma sem o recipiente e após a realização da tara. 
Após a tara, a leitura deverá permanecer próxima de zero, dentro da tolerância estabelecida. 
Teste com diferentes massas. 
Realizar medições com diferentes massas dentro da faixa operacional definida para o protótipo. 
O sistema deverá acompanhar o aumento da massa de forma coerente em toda a faixa de operação escolhida. 
Comportamento após desligamento. 
Desligar a chave geral durante uma condição de teste e verificar o comportamento do protótipo após o religamento. 
A alimentação deverá ser interrompida e, após o religamento, o sistema deverá realizar uma nova inicialização segura, sem acionar involuntariamente o mecanismo. 
Teste de diferentes condições de início. 
Realizar dosagens com diferentes quantidades solicitadas dentro da faixa operacional estabelecida. 
O sistema deverá executar corretamente as etapas rápida e fina e atingir os valores solicitados dentro da margem de erro definida.
Demonstração completa. 
Executar uma operação completa: ligar o sistema, preparar e tarar a plataforma, selecionar o modo, informar a quantidade, iniciar, acompanhar a massa, concluir a dosagem e iniciar uma nova operação. 
Todas as etapas deverão ocorrer na sequência prevista, demonstrando a integração entre interface, ESP32, HX711, células de carga e servo motor. 


REFERÊNCIAS
CURTO CIRCUITO. Balança com célula de carga e HX711. 2019. Disponível em: https://curtocircuito.com.br/blog/arduino/balanca-com-celula-de-carga-e-hx711. Acesso em: 30 ago. 2026.
EMBARCADOS. ESP32: a tecnologia que impulsiona a IoT inteligente. Disponível em: https://embarcados.com.br/esp32-a-tecnologia-que-impulsiona-a-iot-inteligente/. Acesso em: 30 ago. 2026. 
LVGL. Add LVGL to an ESP32 IDF project. Disponível em: https://docs.lvgl.io/master/integration/chip_vendors/espressif/add_lvgl_to_esp32_idf_project.html. Acesso em: 1 set. 2026.
SEIDLE, Nathan. SparkFun_HX711_Calibration.ino. GitHub, 2014. Disponível em: https://github.com/sparkfun/HX711-Load-Cell-Amplifier/blob/master/firmware/SparkFun_HX711_Calibration/SparkFun_HX711_Calibration.ino. Acesso em: 30 ago. 2026. 
SPARKFUN ELECTRONICS. Getting started with load cells. Disponível em: https://learn.sparkfun.com/tutorials/getting-started-with-load-cells. Acesso em: 1 set. 2026.
SPARKFUN ELECTRONICS. Load cell amplifier HX711 breakout hookup guide. Disponível em: https://learn.sparkfun.com/tutorials/load-cell-amplifier-hx711-breakout-hookup-guide. Acesso em: 1 set. 2026.
TOWER PRO. SG90 9g Micro Servo. Disponível em: https://www.eletrogate.com/micro-servo-9g-sg90-towerpro. Acesso em: 30 ago. 2026.