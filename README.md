## sIOT_solar

Sistema de Monitoramento com LDR, ESP32 e Arduino
Este projeto consiste em um sistema de monitoramento que utiliza um sensor LDR (Light Dependent Resistor) para medir a luminosidade em uma área externa. Os dados coletados são enviados via Wi-Fi para um ESP32 no laboratório, que os repassa para um Arduino via comunicação serial. Uma aplicação desktop consome esses dados, exibe gráficos em tempo real e salva os dados em arquivos .txt. Além disso, os dados são enviados para a plataforma IoT Blynk, com fallback para conexão Ethernet em caso de falha na Wi-Fi.

## Componentes Utilizados

#### Área Externa
* Sensor LDR: Mede a luminosidade ambiente.

* ESP32: Coleta os dados do sensor e os envia via Wi-Fi para o laboratório.

* Painel Solar: Fornece energia para os componentes na área externa.

### Laboratório

* ESP32: Recebe os dados da área externa e os envia para o Arduino via comunicação serial.

* Arduino: Recebe os dados do ESP32 e os repassa para a aplicação desktop.

* Shield Ethernet: Fornece conexão de fallback para envio de dados à plataforma IoT em caso de falha na Wi-Fi.

### Aplicação Desktop

* Desenvolvida em Python apresentando gráficos em tempo real com matplotlib e drawnow.

* Exibe gráficos em tempo real usando matplotlib.

* Salva os dados em arquivos .txt.

#### Plataforma IoT Blynk

* Utilizada para plotar gráficos com os dados monitorados.

* Fallback para envio de dados via Ethernet em caso de falha na Wi-Fi.

### Funcionalidades

* #### Coleta de Dados:

  * O sensor LDR mede a luminosidade na área externa
  * O ESP32 na área externa envia os dados via Wi-Fi para o ESP32 no laboratório


* #### Comunicação Serial:

   * O ESP32 no laboratório envia os dados para o Arduino via comunicação serial.

   * O Arduino repassa os dados para a aplicação desktop.

* #### Aplicação Desktop:

   * Exibe gráficos em tempo real da luminosidade.

   * Salva os dados em arquivos .txt.

   * Permite visualizar e analisar os dados coletados.

* #### Plataforma IoT Blynk:

   * Os dados são enviados para a plataforma Blynk para monitoramento remoto.

   * Em caso de falha na Wi-Fi, o sistema utiliza a conexão Ethernet para enviar os dados.

### Pré-requisitos
* #### Hardware
   * Sensor LDR.

   *  3x ESP32.

   * Arduino com Shield Ethernet.

   * Painel solar (para alimentação na área externa).

   * Cabos e componentes eletrônicos necessários.

* #### Software
   * Python 3.x.

   * Bibliotecas Python:

      ``` 
         pyserial 
         drawnow
         matplotlib 
      ```

* IDE Arduino (para programação do ESP32 e Arduino).

* Plataforma Blynk (para monitoramento na nuvem).