build:
	gcc ./code/fil_rouge.c -Wall ./code/graph.c ./code/ll.c ./code/stack.c -o ./fr

ex1dot: build
	./fr dot -i ./tests/graphes/numeros/ex1.txt -o out.txt

ex2dot: build
	./fr dot -i ./tests/graphes/numeros/ex2.txt -o out.txt

ex1tdot: build
	./fr dot -i ./tests/graphes/numeros/ex1.txt

ex2tdot: build
	./fr dot -i ./tests/graphes/numeros/ex2.txt

ex1find: build
	./fr find -i ./tests/graphes/numeros/ex1.txt -start 0 -goal 3

ex2find: build
	./fr find -i ./tests/graphes/numeros/ex2.txt -start 2 -goal 9 -ll -rec

ex2cfct: build
	./fr cfc -i ./tests/graphes/numeros/ex2.txt -cfct

ex3cfc: build
	./fr cfc -i ./tests/graphes/numeros/ex3.txt -ll

ex1cfct: build
	./fr cfc -i ./tests/graphes/numeros/ex1.txt -ll -cfct