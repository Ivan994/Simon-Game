#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
																//radi se na rezberi paj platfromi, iz kojeg stanja ide u koje stanje...
#define WRITE_BUF_LEN 1024										// hrtimer.
#define READ_BUF_LEN 3
#define FIRST_LVL 1

//Global variables
int gpioDev;
int uRandomDev;

unsigned char buffer[WRITE_BUF_LEN]; //kada upisujemo u bafer (1024)
unsigned char bufferRead[READ_BUF_LEN]; // on cita stanje, korak i level

//Functions
void GameInitialization();
void LevelsInitialization();
void WriteToDriver();
void ReadFromDriver();

int main()
{

    GameInitialization();

    int input;

    do
    {
        printf("Welcome to Simon Game\n");
        printf("1. Start\n");
        printf("2. Exit\n");
        printf("Input: ");
        scanf(" %d", &input);

        if (input == 1)
        {
            LevelsInitialization();
            
            int i;					//ovo je da znamo koji su brojevi, da ne moramo da pamtimo xDD da gledamo na ekran
            for (i = 0; i < 10; i++)	
            {
                printf("Buffer %d\n", buffer[i]);
            }

            WriteToDriver();

            // Treaba ispis da zavrsim da lepo moze da se prati tok igre
            while(1)
            {
                usleep(10000000);
                ReadFromDriver();
                printf("Buffer0 in %d\n",bufferRead[0]);
                printf("Buffer1 in %d\n",bufferRead[1]);
                printf("Buffer2 in %d\n",bufferRead[2]);
                printf("---------------------\n");

            }
            
        }

    } while (input != 2);

    close(gpioDev);
    close(uRandomDev);

    return 0;
}

void GameInitialization()
{

    gpioDev = open("/dev/gpio_driver", O_RDWR); //otvori nas drajver
    uRandomDev = open("/dev/urandom", O_RDONLY);	//otvori urandom drajver, iz njega cupamo random brojeve

    if (gpioDev < 0)
    {
        printf("Error, can not open gpio driver.\n");
        exit(-1);
    }

    if (uRandomDev < 0)
    {
        printf("Error, can not open urandom driver.\n");
        exit(-1);
    }
}

void LevelsInitialization()						//ucita 1023 random broja iz drajvera
{

    int i;
    memset(buffer, 0, WRITE_BUF_LEN);
    for (int i = FIRST_LVL; i <= WRITE_BUF_LEN; i++)		//ide od 1 do 1024, i za svaki taj ucita jednu vrednost
    {														// a krecemo od 1 jer je 0 stanje, a od 1 krecu brojevi
        int retValue = read(uRandomDev, &buffer[i], 1);		//ucita random brojeve
        if (retValue == 0)
        {
            printf("Error, can not read from urandom driver.\n");
            exit(-2);
        }
        buffer[i] %= 4;					//vrati broj od 0 do 3 i upise u taj bafer. 
    }
}
void WriteToDriver()
{

    buffer[0] = 1;						//to da on posalje da je stanje = 1
    int retValue = write(gpioDev, buffer, WRITE_BUF_LEN);	//on ga ucita, promeni stanje u 1 i krece igra

    if (retValue < 0)
    {
        printf("Error, can not write to gpio driver.\n");
        exit(-1);
    }
}

void ReadFromDriver()
{
    close(gpioDev);
    gpioDev = open("/dev/gpio_driver", O_RDWR);
    if (gpioDev < 0)
    {
        printf("Error, can not open gpio driver.\n");
        exit(-1);
    }
    int retValue = read(gpioDev, bufferRead, READ_BUF_LEN);
    if (retValue < 0)
    {
        printf("Error, can not read from gpio driver.\n");
        exit(-1);
    }
}