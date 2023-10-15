# Simbolo UFABC
## Nome: Bruno Valem Aranha
## RA: 11039116
Projeto para a disciplina de Computação Gráfica na UFABC.
O projeto foi baseado no triângulo de sierpinski mostrado em aula.

# Etapas de Funcionamento
## 1.
No cabeçalho é criado um vetor que armazena os pontos do triângulo onde o simbolo será desenhado.  
Também é criada a variável `m_P`, um vetor de duas posições que vai armazenar as coordenadas do ponto a ser desenhado quando o programa rodar.  
Obs: O triângulo indicado no vetor não é desenhado! Ele serve apenas como referência para desenhar os três circulos posteriormente.  
![image](https://github.com/BrunoVAranha/UfabcOpenGL/assets/49883183/2db199de-3e2f-49b5-ad5f-f6f9b5ca5706)

## 2.
### Desenho dos três círculos:
No triângulo de sierpinski, a cada frame, calculamos um triângulo e o desenhamos sobre um ponto médio entre o triângulo principal e o triângulo gerado.  
Para desenhar os círculos, mantemos um raio fixo (0.9f), e a cada frame geramos um ângulo aleatório, que será multiplicado seu pelo seno e cosseno para gerarmos as coordenadas do ponto de cada círculo.  
Por fim, somamos `m_P` a um dos pontos do triângulo principal para que haja a distribuição dos pontos entre três círculos, e dividimos por quatro para que eles fiquem visíveis.  
Quando 1500 pontos forem distribuidos entre os três círculos, sinalizamos que o círculo foi desenhado, e o contador de pontos reseta.  
![image](https://github.com/BrunoVAranha/UfabcOpenGL/assets/49883183/d8f5bf28-c850-4899-8b24-5a9b987c0f17)  

## 3.
### Desenho das duas retas:
Obs: a explicação abaixo se refere as sub-retas que compôem cada uma das duas retas do símbolo
Começamos escolhendo os dois pontos que serão o limite inferior e superior da reta.    
`num_points_reta` é a variável que diz quantas posições diferentes cada ponto pode ocupar, e `offset` é o deslocamento (no caso, sempre para a esquerda).  
Quando 500 pontos estiverem distribuídos aleatoriamente entre as 120 posições possíveis, o valor de `offset` é incrementado, e a próxima sub-reta passa a ser desenhada.  
Isso é feito um certo número de vezes, até que a segunda reta comece a ser desenhada em um ponto de partida mais distante, para que haja o espaço entre as duas retass do símbolo.
        RETA1              | RETA2
:-------------------------:|:-------------------------:
![image](https://github.com/BrunoVAranha/UfabcOpenGL/assets/49883183/d5da9bdd-7e44-44a1-8d38-bfa1e63cb2d0)   |  ![image](https://github.com/BrunoVAranha/UfabcOpenGL/assets/49883183/d4422f60-d218-4be1-b90d-9b79a0e996af)




