build:
	gcc ./code/main.c -Wall ./code/graph.c ./code/ll.c ./code/stack.c -o ./fr


### TEST COMPLET ###

ex1: build
	@echo "Test DOT:"
	./fr dot -i ./tests/graphes/numeros/ex1.txt

	@echo "Test chemin:"
	./fr find -i ./tests/graphes/numeros/ex1-lr-scrambled.txt -start 0 -goal 3

	@echo "Test CFCT:"
	./fr cfc -i ./tests/graphes/numeros/ex1-tabs-lr.txt -ll -cfct

ex1nom: build
	@echo "Test DOT:"
	./fr dot -i ./tests/graphes/noms/ex1.txt -ll

	@echo "Test chemin:"
	./fr find -i ./tests/graphes/noms/ex1-lr-scrambled.txt -start 0 -goal 3 -ll -rec

	@echo "Test CFC:"
	./fr cfc -i ./tests/graphes/noms/ex1-tabs-lr.txt

ex2: build
	@echo "Test DOT:"
	./fr dot -i ./tests/graphes/numeros/ex2.txt

	@echo "Test chemin:"
	./fr find -i ./tests/graphes/numeros/ex2.txt -start 2 -goal 9 -ll -rec

	@echo "Test CFCT:"
	./fr cfc -i ./tests/graphes/numeros/ex2.txt -cfct

ex2nom: build
	@echo "Test DOT:"
	./fr dot -i ./tests/graphes/noms/ex2.txt

	@echo "Test chemin:"
	./fr find -i ./tests/graphes/noms/ex2.txt -start 0 -goal 6

	@echo "Test CFC:"
	./fr cfc -i ./tests/graphes/noms/ex2.txt

automaton: build
	@echo "Test DOT:"
	./fr dot -i ./tests/graphes/numeros/automaton.txt -ll

	@echo "Test chemin:"
	./fr find -i ./tests/graphes/numeros/automaton.txt -start 8 -goal 4

	@echo "Test CFCT:"
	./fr cfc -i ./tests/graphes/numeros/automaton.txt -cfct -ll

onlyonescc: build
	@echo "Test DOT:"
	./fr dot -i ./tests/graphes/noms/only-one-scc.txt

	@echo "Test chemin:"
	./fr find -start 0 -goal 7 -i ./tests/graphes/noms/only-one-scc.txt -rec

	@echo "Test CFC:"
	./fr cfc -i ./tests/graphes/noms/only-one-scc.txt

pcbind: build
	@echo "Test DOT:"
	./fr dot -i ./tests/graphes/noms/PC-bind.txt -o ./out.txt

	@echo "Test chemin:"
	./fr find -start 0 -goal 56 -i ./tests/graphes/noms/PC-bind.txt -rec -ll

javadeps: build
	@echo "Test DOT:"
	./fr dot -i ./tests/graphes/noms/GD06-Java.txt -o ./out.txt

	@echo "Test chemin:"
	./fr find -start 34 -goal 560 -i ./tests/graphes/noms/GD06-Java.txt -rec -ll

	@echo "Test CFC:"
	./fr cfc -i ./tests/graphes/noms/GD06-Java.txt -ll

### BENCHMARK TEST ###

benchmark: build
	@echo "benchmark dot"
	time -p ./fr dot -i ./tests/graphes/numeros/PC-biogrid.txt -o /dev/null
	time -p ./fr dot -i ./tests/graphes/numeros/PC-biogrid.txt -o /dev/null -ll

	@echo "benchmark cfc"
	time -p ./fr cfc -i ./tests/graphes/numeros/PC-biogrid.txt -o /dev/null
	time -p ./fr cfc -i ./tests/graphes/numeros/PC-biogrid.txt -o /dev/null -ll

### TEST PARTIELLE ###

# TEST CONVERT TO DOT

ex1dot: build
	./fr dot -i ./tests/graphes/numeros/ex1.txt -o out.txt

ex2tdot: build
	./fr dot -i ./tests/graphes/numeros/ex2.txt

ex1nomdot: build
	./fr dot -i ./tests/graphes/noms/ex1.txt

# TEST FIND

ex1find: build
	./fr find -i ./tests/graphes/numeros/ex1.txt -start 0 -goal 3

ex2find: build
	./fr find -i ./tests/graphes/numeros/ex2.txt -start 2 -goal 9 -ll -rec

ex1nomfind: build
	./fr find -start 0 -goal 3 -i ./tests/graphes/noms/ex1.txt -ll -rec

# TEST CFC & CFC Terminal

ex2cfct: build
	./fr cfc -i ./tests/graphes/numeros/ex2.txt -cfct

ex3cfc: build
	./fr cfc -i ./tests/graphes/numeros/ex3.txt -ll

ex1cfct: build
	./fr cfc -i ./tests/graphes/numeros/ex1.txt -ll -cfct

ex1nomcfc: build
	./fr cfc -i ./tests/graphes/noms/ex1.txt