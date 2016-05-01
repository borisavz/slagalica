#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
int proveri_rec(char *, char *, int);
bool nadji_rec(char *, FILE *);
void promeni_znak(char *, char, char);
int main() {
    int i, ii, iii, iiii, potrefio_slova;
    char slova[20], rec[21], nasa_rec[21], *putanja;
    bool potrefio = false;
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
        slova[i] = rand() % 26 + 97;
        promeni_znak(&slova[i], 'q', 'a');
        promeni_znak(&slova[i], 'w', 'z');
        promeni_znak(&slova[i], 'x', 'a');
        promeni_znak(&slova[i], 'y', 'u');
        printf("%c|", slova[i]);
    }
    while((potrefio == false) && (fgets(nasa_rec, 21, recnik) != NULL)) {
        i = strlen(nasa_rec) - 1;
        if(proveri_rec(nasa_rec, slova, i) == i)
            potrefio = true;
    }
    if(potrefio == false) {
        puts("\nOd datih slova se ne moze sastaviti rec.\n");
        getchar();
        exit(1);
    }
    for(i = 0; i < 10; i++) {
        rewind(recnik);
        potrefio = false;
        printf("\n\nUnesi rec: ");
        fgets(rec, 31, stdin);
        iiii = strlen(rec) - 1;
        for(ii = 0; ii < iiii; ii++)
            rec[i] = tolower(rec[i]);
        potrefio_slova = proveri_rec(rec, slova, iiii);
        if(potrefio_slova != iiii)
            printf("%d slova nisu prihvacena. \n", iiii - potrefio_slova);
        else if(nadji_rec(rec, recnik) == true)
            potrefio = true;
        if(potrefio == true) {
            puts("Mozemo da prihvatimo rec. Da li ste sigurni? y/n");
            scanf("%c", &ii);
            getchar();
            if(tolower(ii) == 'y')
                i = 11;
        } else
            puts("Ne mozemo da prihvatimo rec.\n");
        printf("Ostalo pokusaja: %d\n", 9 - i);
    }
    printf("Nasa rec: %s", nasa_rec);
    return 0;
}
int proveri_rec(char *rec, char *slova, int duzina_reci) {
    int i, ii, potrefio_slova = 0;
    bool preskoci_slovo[20], preskoci_rec[20];
    for(i = 0; i < 20; i++) {
        *(preskoci_slovo + i) = false;
        *(preskoci_rec + i) = false;
    }
    for(i = 0; i < 20; i++)
        for(ii = 0; ii < duzina_reci; ii++)
            if((*(slova + i) == *(rec + ii)) && (preskoci_slovo[i] == false) &&
                (preskoci_rec[ii] == false)) {
                preskoci_slovo[i] = true;
                preskoci_rec[ii] = true;
                potrefio_slova++;
            }
    return potrefio_slova;
}
bool nadji_rec(char *rec, FILE *recnik) {
    char rec_iz_recnika[21];
    while(fgets(rec_iz_recnika, 21, recnik) != NULL)
        if(strcmp(rec, rec_iz_recnika) == 0)
            return true;
    return false;
}
void promeni_znak(char *a, char b, char c) {
    if(*a == b)
        *a = c;
}
