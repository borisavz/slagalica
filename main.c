#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#define SASTAVIO 11
#define IZLAZ 22
FILE *otvori_recnik();
int proveri_rec(const char *, const char *, int);
bool nadji_rec(const char *, FILE *);
bool hoces_neces(const char *);
void promeni_znak(char *, char, char);
int main(int argc, char *argv[]) {
    int i, ii, potrefio_slova, broj_pokusaja = 10;
    char slova[20], rec[21], nasa_rec[21];
    bool nasao = false;
    FILE *recnik;
    while((i = getopt(argc, argv, "np:h")) != -1)
        switch(i) {
            case 'n':
                puts("PROVERA RECI\n-------");
                recnik = otvori_recnik();
                printf("Unesi rec: ");
                fgets(rec, 21, stdin);
                printf("\nRec se%s nalazi u recniku.\n",
                    nadji_rec(rec, recnik) == true ? "" : " ne");
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
    puts("Kucaj 0 da prekines igru.");
    printf("Broj pokusaja: %d\n\n", broj_pokusaja);
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
    i = 0;
    while(i == 0 && fgets(nasa_rec, 21, recnik) != NULL)
        if(proveri_rec(nasa_rec, slova, strlen(nasa_rec) - 1) == 0)
            i = 1;
    if(i == 0) {
        puts("\n\nOd datih slova se ne moze sastaviti rec.\n");
        getchar();
        exit(0);
    }
    for(i = 1; i <= broj_pokusaja; i++) {
        rewind(recnik);
        nasao = false;
        printf("\nUnesi rec: ");
        fgets(rec, 31, stdin);
        for(ii = 0; rec[ii] != '\0'; ii++)
            rec[ii] = tolower(rec[ii]);
        potrefio_slova = proveri_rec(rec, slova, ii - 1);
        if(potrefio_slova != 0 && potrefio_slova != IZLAZ)
            printf("%d %s u redu.\n", potrefio_slova,
                potrefio_slova == 1 ? "slovo nije" : "slova nisu");
        else if(potrefio_slova == IZLAZ)
            i = hoces_neces("Da li ste sigurni? y/n") == true ? IZLAZ : --i;
        else if((nasao = nadji_rec(rec, recnik)) == true &&
            hoces_neces("Mozemo da prihvatimo rec. Da li ste sigurni? y/n") == true)
            i = SASTAVIO;
        else if(nasao == false && i != IZLAZ)
            puts("Ne mozemo da prihvatimo rec.\n");
        if(i == SASTAVIO)
            puts("Bravo!");
        else if(i != IZLAZ)
            printf("Ostalo pokusaja: %d\n", broj_pokusaja - i);
    }
    printf("\n-------\nNasa rec: %s", nasa_rec);
    fseek(stdin, 0, SEEK_END);
    getchar();
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
int proveri_rec(const char *rec, const char *slova, int duzina_reci) {
    int i, ii, potrefio_slova = 0;
    bool preskoci_slovo[20], preskoci_rec[20];
    for(i = 0; i < 20; i++) {
        *(preskoci_slovo + i) = false;
        *(preskoci_rec + i) = false;
    }
    if(*rec == '0')
        return IZLAZ;
    for(i = 0; i < 20; i++)
        for(ii = 0; ii < duzina_reci; ii++)
            if(*(slova + i) == *(rec + ii) && preskoci_slovo[i] == false &&
                preskoci_rec[ii] == false) {
                preskoci_slovo[i] = true;
                preskoci_rec[ii] = true;
                potrefio_slova++;
            }
    return duzina_reci - potrefio_slova;
}
bool nadji_rec(const char *rec, FILE *recnik) {
    char rec_iz_recnika[21];
    while(fgets(rec_iz_recnika, 21, recnik) != NULL)
        if(strcmp(rec, rec_iz_recnika) == 0)
            return true;
    return false;
}
bool hoces_neces(const char *poruka) {
    char c;
    puts(poruka);
    do
        scanf("%c", &c);
    while(c != 'y' && c != 'n');
    getchar();
    return c == 'y' ? true : false;
}
void promeni_znak(char *a, char b, char c) {
    if(*a == b)
        *a = c;
}
