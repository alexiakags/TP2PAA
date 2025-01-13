all:
	gcc -o programa src/main.c src/caverna.c src/saidaArquivo.c

run:
	programa 

plot:
	python output/plot.py