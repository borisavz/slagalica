#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
void promeni_znak(char *, char, char);
int main() {
    int i, ii, iii, iiii, potrefio_slova;
    char slova[20], rec[21], rec_iz_recnika[20], *putanja;
    bool preskoci_slovo[20], preskoci_rec[20], potrefio;
    FILE *recnik;
    if((recnik = fopen("recnik.txt", "r")) == NULL) {
        printf("Unesi putanju recnika: ");
        putanja = malloc(300 * sizeof(char));
        fgets(putanja, 300, stdin);
        *(putanja + strlen(putanja) - 1) = '\0';
        if((recnik = fopen(putanja, "r")) == NULL) {
            perror("\nGreska");
            printf("Kod greske: %d", errno);
            getchar();
            exit(errno);
        }
        free(putanja);
    }
    srand(time(NULL));
    printf("\nSlova:\n\t|");
    for(i = 0; i < 20; i++) {
        slova[i] = rand() % 25 + 98;
        promeni_znak(&slova[i], 'q', 'a');
        promeni_znak(&slova[i], 'w', 'c');
        promeni_znak(&slova[i], 'x', 'a');
        promeni_znak(&slova[i], 'y', 'u');
        printf("%c|", slova[i]);
    }
    for(i = 0; i < 10; i++) {
        rewind(recnik);
        potrefio = false;
        potrefio_slova = 0;
        for(ii = 0; ii < 20; ii++) {
            preskoci_slovo[ii] = false;
            preskoci_rec[ii] = false;
        }
        printf("\n\nUnesi rec: ");
        fgets(rec, 31, stdin);
        iiii = strlen(rec) - 1;
        for(ii = 0; ii < iiii; ii++)
            rec[i] = tolower(rec[i]);
        for(ii = 0; ii < 20; ii++)
            for(iii = 0; iii < iiii; iii++)
                if((slova[ii] == rec[iii]) && (preskoci_slovo[ii] == false) &&
                    (preskoci_rec[iii] == false)) {
                    preskoci_slovo[ii] = true;
                    preskoci_rec[iii] = true;
                    potrefio_slova++;
                }
        if(potrefio_slova != iiii)
            printf("%d slova nisu prihvacena. \n", iiii - potrefio_slova);
        else
            while(((fgets(rec_iz_recnika, 21, recnik)) != NULL) && (potrefio == false))
                if(strcmp(rec, rec_iz_recnika) == 0)
                    potrefio = true;
        if(potrefio == true) {
            puts("Mozemo da prihvatimo rec. Da li ste sigurni? y/n");
            scanf("%c", &ii);
            getchar();
            if(tolower(ii) == 'y')
                i = 11;
        } else
            puts("Ne mozemo da prihvatimo rec.\n");
        printf("Ostalo pokusaja: %d", 9 - i);
    }
    return 0;
}
void promeni_znak(char *a, char b, char c) {
    if(*a == b)
        *a = c;
}
