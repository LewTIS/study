c_07:c_07.o
	gcc $^ -o $@ 
c_07.o:c_07.s
	gcc -c $^ -o $@
c_07.s:c_07.i
	gcc -S $^ -o $@
c_07.i:c_07.c
	gcc -E $^ -o $@
clean:
	rm -f c_07.i c_07.s c_07.o c_07


	
