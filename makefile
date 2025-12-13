run:
	gcc ./code/fil_rouge.c -Wall ./code/graph.c ./code/ll.c ./code/stack.c -o ./fr

ex1: run
	./fr -i ./tests/graphes/numeros/ex1.txt -o out.txt

ex2: run
	./fr -i ./tests/graphes/numeros/ex2.txt -o out.txt

ex1t: run
	./fr -i ./tests/graphes/numeros/ex1.txt

ex2t: run
	./fr -i ./tests/graphes/numeros/ex2.txt