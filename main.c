#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <conio.h>
void otvori_recnik(void);
int proveri_rec(const char *, int);
bool nadji_rec(void);
bool sastavi_rec(void);
bool hoces_neces(const char *);
bool preskoci_rec[20];
char slova[20], nema_slova[20], rec[21], nasa_rec[21];
FILE *recnik;
int main(int argc, char *argv[]) {
    int i, ii = 0, iii, potrefio_slova, broj_pokusaja = 10, duzina = 0;
    while((i = getopt(argc, argv, "np:hs")) != -1)
        switch(i) {
            case 'n':
                puts("PROVERA RECI\n-------");
                otvori_recnik();
                printf("Unesi rec: ");
                fgets(rec, 21, stdin);
                printf("\nRec se%s nalazi u recniku.\n", nadji_rec() ? "" : " ne");
                getch();
                return 0;
            case 'p':
                broj_pokusaja = atoi(optarg);
                break;
            case 'h':
                puts("-n proveri da li se rec nalazi u recniku\n"
                    "-p promeni broj pokusaja\n"
                    "-s racunar sastavlja rec sa zadatim slovima");
                getch();
                return 0;
            case 's':
                otvori_recnik();
                printf("\nSlova: \n\t");
                while(true) {
                    iii = getch();
                    if(iii == '\b') {
                        if(ii > 0) {
                            printf("\b\b  \b\b");
                            ii--;
                        } else
                            putchar('\a');
                        continue;
                    }
                    if(iii == '\r') {
                        if(ii < 20) {
                            putchar('\a');
                            continue;
                        } else
                            break;
                    }
                    if(ii == 20) {
                        putchar('\a');
                        continue;
                    }
                    printf("|%c", slova[ii++] = iii);
                }
                sastavi_rec() ? printf("|\n\n-------\nNasa rec: %s", nasa_rec) :
                    puts("Od datih slova se ne moze sastaviti rec.");
                getch();
                return 0;
        }
    printf("Koristi -h zastavicu pri pokretanju za pomoc.\n"
        "Kucaj 0 da prekines igru.\n"
        "Broj pokusaja: %d\n\n", broj_pokusaja);
    otvori_recnik();
    srand(time(NULL));
    for(i = broj_pokusaja; i; i /= 10)
        duzina++;
    printf("\nSlova:\n\t|");
    do
        for(i = 0; i < 20; ) {
            slova[i] = rand() % 26 + 97;
            if(slova[i] != 'q' && slova[i] != 'w' && slova[i] != 'x' && slova[i] != 'y')
                i++;
        }
    while(!sastavi_rec());
    for(i = 0; i < 20; i++)
        printf("%c|", slova[i]);
    for(i = 1; i <= broj_pokusaja; ) {
        memset(nema_slova, 20, '\0');
        printf("\n\n%*d|", duzina, i);
        fgets(rec, 21, stdin);
        if(rec[0] == '0') {
            if(hoces_neces("Da li ste sigurni da zelite da izadjete? y/n"))
                break;
            else
                continue;
        }
        for(ii = 0; rec[ii] != '\0'; ii++)
            rec[ii] = tolower(rec[ii]);
        potrefio_slova = proveri_rec(rec, --ii);
        if(potrefio_slova != 0) {
            fputs(potrefio_slova == 1 ? "Nedostaje slovo:\n\t|" : "Nedostaju slova:\n\t|", stdout);
            for(iii = 0; iii < ii; iii++)
                if(!preskoci_rec[iii])
                    printf("%c|", rec[iii]);
        } else if(nadji_rec()) {
            if(hoces_neces("Mozemo da prihvatimo rec. Da li ste sigurni? y/n")) {
                puts("Bravo!");
                break;
            }
        } else
            fputs("Ne mozemo da prihvatimo rec.", stdout);
        i++;
    }
    printf("\n-------\nNasa rec: %s", nasa_rec);
    fseek(stdin, 0, SEEK_END);
    getch();
    return 0;
}
void otvori_recnik() {
    char putanja[201];
    if((recnik = fopen("recnik.txt", "r")) == NULL) {
        puts("Unesi putanju recnika:");
        fgets(putanja, 301, stdin);
        putanja[strlen(putanja) - 1] = '\0';
        if((recnik = fopen(putanja, "r")) == NULL) {
            perror("\nGreska");
            printf("Kod greske: %d", errno);
            getchar();
            exit(errno);
        }
    }
}
int proveri_rec(const char *rec_za_proveru, int duzina_reci) {
    int i = 20, ii, potrefio_slova = 0;
    bool preskoci_slovo[20];
    while(i--)
        preskoci_rec[i] = preskoci_slovo[i] = false;
    for(i = 0; i < 20; i++)
        for(ii = 0; ii < duzina_reci; ii++)
            if(slova[i] == rec_za_proveru[ii] && !preskoci_slovo[i] && !preskoci_rec[ii]) {
                preskoci_slovo[i] = preskoci_rec[ii] = true;
                potrefio_slova++;
            }
    return duzina_reci - potrefio_slova;
}
bool nadji_rec() {
    char rec_iz_recnika[21];
    rewind(recnik);
    while(fgets(rec_iz_recnika, 21, recnik) != NULL)
        if(strcmp(rec, rec_iz_recnika) == 0)
            return true;
    return false;
}
bool sastavi_rec() {
    rewind(recnik);
    while(fgets(nasa_rec, 21, recnik) != NULL)
        if(proveri_rec(nasa_rec, strlen(nasa_rec) - 1) == 0)
            return true;
    return false;
}
bool hoces_neces(const char *poruka) {
    char c = '\0';
    int i = 0;
    while(poruka[i] != '\0')
        putchar(poruka[i++]);
    while((c = getch()) != 'y' && c != 'n')
        putchar('\a');
    while(i--)
        fputs("\b \b", stdout);
    return c == 'y';
}