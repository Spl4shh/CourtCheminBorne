CC = clang
CFLAGS = -Wall -Wextra -pedantic -O0 -g3 -fsanitize=address

projet: 
	$(CC) $(CFLAGS) -c GeographicDistances.c ouvrir_csv.c fonction.c -I ../include
	$(CC) $(CFLAGS) GeographicDistances.o ouvrir_csv.o fonction.o -o executable -I ../include

clean:
	rm -f *.o
	rm -f ./CSV_Ouverture/*.o
	rm -f ./CSV_Ouverture/ouvrir_csv
	rm -f ouvrir_csv
	rm -f ./CSV_Ouverture/Station.h.gch
	rm -f fonction
	rm -f GeographicDistances
	rm -f executable
	rm -f Main