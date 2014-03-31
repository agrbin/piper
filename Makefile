.PHONY: run compile clean

run: compile
	cp test test_a
	echo "test 1"
	./piper ./test ./test_a
	echo "test 2"
	./piper "./test2 1 2 3" "./test2 arrrrrrg"
	
piper: piper.c
	gcc -o piper piper.c

clean:
	rm -f piper test test_a test2

compile: piper test test2

test: test.c
	gcc -o test test.c

test2: test2.c
	gcc -o test2 test2.c

