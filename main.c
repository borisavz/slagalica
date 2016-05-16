#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
FILE *otvori_recnik();
int proveri_rec(char *, char *, int);
bool nadji_rec(char *, FILE *);
void promeni_znak(char *, char, char);
int main(int argc, int *argv[]) {
    int i, ii, iii, iiii, potrefio_slova, broj_pokusaja = 10;
    char slova[20], rec[21], nasa_rec[21];
    bool potrefio = false;
    FILE *recnik;
    while((i = getopt(argc, argv, "np:h")) != -1)
        switch(i) {
            case 'n':
                puts("PROVERA RECI\n------");
                recnik = otvori_recnik();
                printf("Unesi rec: ");
                fgets(rec, 21, stdin);
                if(nadji_rec(rec, recnik) == true)
                    puts("\nRec se nalazi u recniku.");
                else
                    puts("\nRec se ne nalazi u recniku.");
                getchar();
                exit(0);
                break;
            case 'p':
                broj_pokusaja = atoi(optarg);
                break;
            case 'h':
                puts("-n proveri da li se rec nalazi u recniku");
                puts("-p promeni broj pokusaja");
                getchar();
                exit(0);
                break;
        }
    puts("Koristi -h zastavicu pri pokretanju za pomoc.");
    puts("Kucaj 0 da prekines igru.\n");
    recnik = otvori_recnik();
    srand(time(NULL));
    printf("\nSlova:\n\t|");
    for(i = 0; i < 20; i++) {
        slova[i] = rand() % 26 + 97;
        promeni_znak(&slova[i], 'q', 'a');
        promeni_znak(&slova[i], 'w', 'z');
        promeni_znak(&slova[i], 'x', 'a');
        promeni_znak(&slova[i], 'y', 'z');
        printf("%c|", slova[i]);
    }
    while((potrefio == false) && (fgets(nasa_rec, 21, recnik) != NULL))
        if(proveri_rec(nasa_rec, slova, strlen(nasa_rec) - 1) == 0)
            potrefio = true;
    if(potrefio == false) {
        puts("\n\nOd datih slova se ne moze sastaviti rec.\n");
        getchar();
        exit(1);
    }
    for(i = 1; i <= broj_pokusaja; i++) {
        rewind(recnik);
        potrefio = false;
        printf("\nUnesi rec: ");
        fgets(rec, 31, stdin);
        if(rec[0] == '0')
            exit(0);
        for(ii = 0; rec[ii] != '\0'; ii++)
            rec[i] = tolower(rec[i]);
        potrefio_slova = proveri_rec(rec, slova, ii - 1);
        if(potrefio_slova != 0)
            printf("%d slova nisu prihvacena. \n", potrefio_slova);
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
        if(i == 11)
            puts("\nBravo!\n");
        else
            printf("Ostalo pokusaja: %d\n", broj_pokusaja - i);
    }
    printf("Nasa rec: %s", nasa_rec);
    return 0;
}
FILE *otvori_recnik() {
    FILE *recnik;
    char putanja[201];
    if((recnik = fopen("recnik.txt", "r")) == NULL) {
        printf("Unesi putanju recnika: ");
        fgets(putanja, 301, stdin);
        putanja[strlen(putanja) - 1] = '\0';
        if((recnik = fopen(putanja, "r")) == NULL) {
            perror("\nGreska");
            printf("Kod greske: %d", errno);
            getchar();
            exit(errno);
        }
    }
    return recnik;
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
    return duzina_reci - potrefio_slova;
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
