/* (Keep It) Simple Stupid Database
 *
 * Written by Adam Ierymenko <adam.ierymenko@zerotier.com>
 * KISSDB is in the public domain and is distributed with NO WARRANTY.
 *
 * http://creativecommons.org/publicdomain/zero/1.0/ */

/* Compile with KISSDB_TEST to build as a test program. */

/* Note: big-endian systems will need changes to implement byte swapping
 * on hash table file I/O. Or you could just use it as-is if you don't care
 * that your database files will be unreadable on little-endian systems. */

#define _FILE_OFFSET_BITS 64

#include "kissdb.h"

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#include <inttypes.h>

int main(int argc,char *argv[]) {
    uint64_t i,j;
    uint64_t v[8];
    KISSDB db;
    KISSDB_Iterator dbi;
    char got_all_values[10000];
    int q;

    int opt;

    bool openOnly = false;
    int count = 0;

    while ((opt = getopt(argc, argv, "hon:")) != -1) {
        switch(opt) {
        case 'h':
            printf("Help\n");
                break;
        case 'o':
                printf("Open and close\n");
                openOnly = true;
                break;

        case 'n':
                openOnly = false;
                count = atoi(optarg);
                printf("Write %d test values\n", count);
                break;
        }
    }

    printf("Opening new empty database test.db...\n");

    if (KISSDB_open(&db,"test.db",KISSDB_OPEN_MODE_RWREPLACE,1024,8,sizeof(v))) {
        printf("KISSDB_open failed\n");
        return 1;
    }

    if(!openOnly) {
        printf("Adding and then re-getting %d 64-byte values...\n", count);

        for(i=0;i< count ;++i) {
            for(j=0;j<8;++j)
                v[j] = i;
            if (KISSDB_put(&db,&i,v)) {
                printf("KISSDB_put failed (%"PRIu64")\n",i);
                return 1;
            }
            memset(v,0,sizeof(v));
            if ((q = KISSDB_get(&db,&i,v))) {
                printf("KISSDB_get (1) failed (%"PRIu64") (%d)\n",i,q);
                return 1;
            }
            for(j=0;j<8;++j) {
                if (v[j] != i) {
                    printf("KISSDB_get (1) failed, bad data (%"PRIu64")\n",i);
                    return 1;
                }
            }
        }

        printf("Getting %d 64-byte values...\n",count);

        for(i=0;i< count ;++i) {
            if ((q = KISSDB_get(&db,&i,v))) {
                printf("KISSDB_get (2) failed (%"PRIu64") (%d)\n",i,q);
                return 1;
            }
            for(j=0;j<8;++j) {
                if (v[j] != i) {
                    printf("KISSDB_get (2) failed, bad data (%"PRIu64")\n",i);
                    return 1;
                }
            }
        }

        printf("Closing and re-opening database in read-only mode...\n");

        KISSDB_close(&db);

        if (KISSDB_open(&db,"test.db",KISSDB_OPEN_MODE_RDONLY,1024,8,sizeof(v))) {
            printf("KISSDB_open failed\n");
            return 1;
        }

        printf("Getting %d 64-byte values...\n",count);

        for(i=0;i< count;++i) {
            if ((q = KISSDB_get(&db,&i,v))) {
                printf("KISSDB_get (3) failed (%"PRIu64") (%d)\n",i,q);
                return 1;
            }
            for(j=0;j<8;++j) {
                if (v[j] != i) {
                    printf("KISSDB_get (3) failed, bad data (%"PRIu64")\n",i);
                    return 1;
                }
            }
        }

        printf("Iterator test...\n");

        KISSDB_Iterator_init(&db,&dbi);
        i = 0xdeadbeef;
        memset(got_all_values,0,sizeof(got_all_values));
        while (KISSDB_Iterator_next(&dbi,&i,&v) > 0) {
            if (i < count)
                got_all_values[i] = 1;
            else {
                printf("KISSDB_Iterator_next failed, bad data (%"PRIu64")\n",i);
                return 1;
            }
        }
        for(i=0;i< count ;++i) {
            if (!got_all_values[i]) {
                printf("KISSDB_Iterator failed, missing value index %"PRIu64"\n",i);
                return 1;
            }
        }
    }
    KISSDB_close(&db);

    printf("All tests OK!\n");

    return 0;
}

