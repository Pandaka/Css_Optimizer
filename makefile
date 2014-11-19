MAN = /usr/share/man/man1
all : css_optimizer.o
	gcc css_optimizer.o -o css_optimizer
css_optimizer.o : main.c css_optimizer.c css_optimizer.h
	gcc -c main.c -o css_optimizer.o
install :
	cp css_optimizer /usr/local/bin
	echo " commande installé"
	cp man $(MAN)/css_optimizer.1
	gzip /usr/share/man/man1/css_optimizer.1
	echo "man installé"
clean :
	rm -rf *.o
mrproper : clean
	rm -rf css_optimizer
