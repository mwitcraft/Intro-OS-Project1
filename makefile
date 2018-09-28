all:
	gcc project1.c -o project1

clean:
	# mv project1.c makefile README.txt ../
	mv project1.c makefile README.txt sources.txt ../
	rm -r *
	mv ../project1.c ../makefile ../README.txt ../sources.txt ./


