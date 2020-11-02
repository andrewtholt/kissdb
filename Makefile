# http://creativecommons.org/publicdomain/zero/1.0/

CC=gcc
BINS=kissdb-test tst

all:	$(BINS)

kissdb-test:	kissdb.c kissdb.h
	$(CC) -Wall -O2 -DKISSDB_TEST -o kissdb-test kissdb.c

kissdb.o:	kissdb.c kissdb.h
	$(CC) -c -Wall -O2 -o kissdb.o kissdb.c

tst:	tst.c kissdb.o kissdb.h
	$(CC) -Wall -DEMPTY tst.c kissdb.o -o tst

clean:
	rm -f kissdb-test *.o test.db
