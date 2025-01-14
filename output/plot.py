import os
import matplotlib.pyplot as plt

# Função para ler os dados do arquivo
def ler_arquivo(nome_arquivo):
    x = []  # Lista para armazenar os valores de movimentos
    y = []  # Lista para armazenar os valores de tempo

    # Abrir e ler o arquivo
    with open(nome_arquivo, 'r') as arquivo:
        for linha in arquivo:
            # Divide a linha em dois valores: número de movimentos e tempo
            valores = linha.split()
            x.append(int(valores[0]))  # Movimento
            y.append(float(valores[1]))  # Tempo

    return x, y

# Função para plotar o gráfico
def plotar_grafico(x, y):
    plt.figure(figsize=(10, 6))  # Define o tamanho da figura
    plt.plot(x, y, marker='o', linestyle='-', color='b', label='Tempo vs Área')
    plt.xlabel('Tamanho da caverna (m²)')  # Rótulo do eixo X
    plt.ylabel('Tempo (segundos)')  # Rótulo do eixo Y
    plt.title('Gráfico de Área x Tempo')  # Título do gráfico
    plt.legend()  # Exibe a legenda
    plt.grid(True)  # Exibe a grade
    plt.show()  # Exibe o gráfico

# Caminho do arquivo
nome_arquivo = os.path.join(os.getcwd(), './output/saida.txt')

# Verifica se o arquivo existe no diretório atual
if not os.path.isfile(nome_arquivo):
    print(f"Erro: o arquivo '{nome_arquivo}' não foi encontrado no diretório atual.")
else:
    # Ler os dados do arquivo
    x, y = ler_arquivo(nome_arquivo)
    
    # Plotar o gráfico
    plotar_grafico(x, y)
