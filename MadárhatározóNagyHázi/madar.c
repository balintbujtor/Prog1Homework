#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { TELEPULES, MEZOGAZDASAG, ERDO, VIZPART, HEGYSEG, RET } ELOHELY;
				// 0		1			2		3		4		5
typedef enum { PARTIMADAR, BAGOLY, VARJU, GALAMB, TYUK, HARKALY, ENEKESMADAR, HOSSZULABUMADAR, RAGADOZO, SIRALY, RECE } ALAK;
				//0			1		2		3		4		5		6					7			8		9    10
typedef enum { FEHER, SZURKE, BARNA, SARGA, NARANCS, PIROS, KEK, ZOLD, FEKETE } TOLLAZAT;
				//0		1		2		3		4		5	 6	  7		 8 

typedef struct {
	char name[100];
	ELOHELY hely;
	ALAK alak;
	TOLLAZAT color;
	int eszleles;
} bird;

typedef struct {
	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int time; // 0630, 1645, 2206
	char place[60];
}log;

typedef struct loglistelem {
	log eszleles;
	struct loglistelem *next;
	struct loglistelem *prev;
}loglistelem;

typedef struct birdlistelem {
	bird madar;
	struct birdlistelem *next;
	loglistelem *loghead;
	loglistelem *logtail;
}birdlistelem;

void elohelyprinter(int elohely)
{
	switch (elohely)
	{
	case TELEPULES: printf("TELEPULES\n");
		break;
	case MEZOGAZDASAG: printf("MEZOGAZDASAG\n");
		break;
	case ERDO: printf("ERDO\n");
		break;
	case VIZPART: printf("VIZPART\n");
		break;
	case HEGYSEG: printf("HEGYSEG\n");
		break;
	case RET: printf("RET\n");
		break;
		default: printf("nincs egyezes\n");
			break;
	}
}

void alakprinter(int alak)
{
	switch (alak)
	{
	case PARTIMADAR: printf("PARTIMADAR\n");
		break;
	case BAGOLY: printf("BAGOLY\n");
		break;
	case VARJU: printf("VARJU\n");
		break;
	case GALAMB: printf("GALAMB\n");
		break;
	case TYUK: printf("TYUK\n");
		break;
	case HARKALY: printf("HARKALY\n");
		break;
	case ENEKESMADAR: printf("ENEKESMADAR\n");
		break;
	case HOSSZULABUMADAR: printf("HOSSZULABUMADAR\n");
		break;
	case RAGADOZO: printf("RAGADOZO\n");
		break;
	case SIRALY: printf("SIRALY\n");
		break;
	case RECE: printf("RECE\n");
		break;
	default: printf("nincs egyezés\n");
		break;
	}
}

void tollazatprinter(int tollazat)
{
	switch (tollazat)
	{
	case FEHER: printf("FEHER\n");
		break;
	case SZURKE: printf("SZURKE\n");
		break;
	case BARNA: printf("BARNA\n");
		break;
	case SARGA: printf("SARGA\n");
		break;
	case NARANCS: printf("NARANCS\n");
		break;
	case PIROS: printf("PIROS\n");
		break;
	case KEK: printf("KEK\n");
		break;
	case ZOLD: printf("ZOLD\n");
		break;
	case FEKETE: printf("FEKETE\n");
		break;
	default: printf("nincs egyezes\n");
		break;
	}
}

birdlistelem* madarfilescan(char *fname)
{
	FILE *in;

	in = fopen(fname, "r");

	if (in == NULL)
		printf("Error opening file: %s", fname);

	//utolso listaelemnek helyfoglalás
	birdlistelem* new = (birdlistelem *)malloc(sizeof(birdlistelem));
	new->next = NULL;

	//az utolso listaelem logheadjének lefoglalása
	loglistelem* head = (loglistelem*)malloc(sizeof(loglistelem));
	new->loghead = head;

	//az utolso listaelem logtailjének lefoglalása
	loglistelem* tail = (loglistelem*)malloc(sizeof(loglistelem));
	new->logtail = tail;

	//összefûzés
	head->next = tail;
	tail->prev = head;
	
	//adatbeolvasás
	fscanf(in, "%s\n%d %d %d\n", new->madar.name, &new->madar.alak, &new->madar.color, &new->madar.hely);
		
	printf("%s\n%d %d %d\n", new->madar.name, new->madar.alak, new->madar.color, new->madar.hely);

	while (!feof(in)) {

		//új elemnek helyfoglalás
		birdlistelem* p = (birdlistelem *)malloc(sizeof(birdlistelem));
		//az új elem következõ elemre mutató pointere a régi elem
		p->next = new;
		//most már az új elem a régi
		new = p;

		//strázsák hozzáadása a madár észleléseihez és összefûzésük
		loglistelem* head = (loglistelem*)malloc(sizeof(loglistelem));
		new->loghead = head;
		loglistelem* tail = (loglistelem*)malloc(sizeof(loglistelem));
		new->logtail = tail;
		head->next = tail;
		tail->prev = head;

		//adatbeolvasás
		fscanf(in, "%s\n%d %d %d\n", new->madar.name, &new->madar.alak, &new->madar.color, &new->madar.hely);
		new->madar.eszleles = 0;
		printf("%s\n%d %d %d\n", new->madar.name, new->madar.alak, new->madar.color, new->madar.hely);
	}
	

	if (fclose(in) != 0)
		printf("Error closing file: %s", fname);

	return new;
}

void logfilescan(char* logfile, birdlistelem* madarhead)
{
	FILE* log = fopen(logfile, "r");
	

	if (log == NULL)
		printf("Error opening file: %s", logfile);

	while (!feof(log))
	{
		//kulcs beolvasása = a madár neve amit keresünk a láncolt listában - ehhez fogjuk hozzáadni a logokat
		birdlistelem* sch = madarhead;
		char key[100];
		fscanf(log, "%s\n", key);
		printf("sikeres olvasas %s \n", key);

		//egyezés keresése fv-el
		while (strcmp(key, sch->madar.name) != 0)
		{
			sch = sch->next;
		}
		if (sch == NULL)
		{
			printf("nincs ilyen madar");
			return;
		}
		else
			printf("sikeres egyezes %s \n", sch->madar.name);

		int eszlelesszam = 0;
		sch->madar.eszleles = eszlelesszam;

		//logdátumok olvasás
		//helyfoglalás egy uj észlelésnek
		loglistelem* uj = (loglistelem *)malloc(sizeof(loglistelem));
		//logok vége elõtti elemének a másolása, hogy az érkezõ pointer értéke ne sérüljön
		loglistelem* elozo = sch->loghead;
		//ciklusfejben van a beolvasás
		while (fscanf(log, "%d %d %d %d %s\n", &uj->eszleles.year, &uj->eszleles.month, &uj->eszleles.day, &uj->eszleles.time, uj->eszleles.place) == 5)
		{
			//ellenõrzés
			printf("%d %d %d %d %s\n", uj->eszleles.year, uj->eszleles.month, uj->eszleles.day, uj->eszleles.time, uj->eszleles.place);
			//új észlelés befûzése a záróstrázsa elé

			uj->next = sch->logtail;
			uj->prev = elozo;
			uj->next->prev = uj;
			elozo->next = uj;

			//az új elem lecserélése
			elozo = uj;

			//helyfoglalás
			
			uj->next = (loglistelem *)malloc(sizeof(loglistelem));
			uj = uj->next;
			sch->madar.eszleles = ++eszlelesszam;		
		}
	}

	printf("\n\n\n\n");
	if (fclose(log) != 0)
		printf("Error closing file: %s", logfile);
}

void logprinter(loglistelem* fej, loglistelem* farok)
{
	loglistelem* fejel = farok->prev;
	while (fejel != fej)
	{
		printf("%d %d %d %d %s\n", fejel->eszleles.year, fejel->eszleles.month, fejel->eszleles.day, fejel->eszleles.time, fejel->eszleles.place);
		fejel = fejel->prev;
	}
}

void dataprinter(birdlistelem* madar)
{
	printf("Faj:\t\t %s\n", madar->madar.name);
	printf("Elohely: \t");
	elohelyprinter(madar->madar.hely);
	printf("Alak: \t\t");
	alakprinter(madar->madar.alak);
	printf("Tollazat: \t");
	tollazatprinter(madar->madar.color);
	printf("Eszlelesek szama: %d\n\n", madar->madar.eszleles);
	
	if(madar->madar.eszleles != 0)
		logprinter(madar->loghead, madar->logtail);

	printf("\n\n");

}

void addlog(birdlistelem* madarhead)
{
	birdlistelem* kereses = madarhead;
	loglistelem* newlog = (loglistelem*)malloc(sizeof(loglistelem));
	char key[100];

	printf("adja meg melyik madarhoz akar eszlelest hozzaadni (nagybetukkel):");
	scanf("%s", key);
	printf("Adja meg az adatokat(ev honap nap ido hely): ");
	scanf("%d %d %d %d %s", &newlog->eszleles.year, &newlog->eszleles.month, &newlog->eszleles.day, &newlog->eszleles.time, newlog->eszleles.place);	

	//faj keresese
	while (strcmp(key, kereses->madar.name) != 0)
	{
		kereses = kereses->next;
	}

	if (kereses == NULL)
	{
		printf("nincs ilyen madar");
		return;
	}
	else
		printf("sikeres egyezes %s \n", kereses->madar.name);

	//elozo elem cimének elmentése
	loglistelem* elozo = kereses->logtail->prev;

	newlog->next = kereses->logtail;
	kereses->logtail->prev = newlog;
	newlog->prev = elozo;
	elozo->next = newlog;

	kereses->madar.eszleles++;

}

void kereses(birdlistelem* madarhead)
{
	int feladat;
	birdlistelem* kereses = madarhead;
	char key[100];
	int alaknum, tollnum, elonum;

	printf("adja meg mi alapjan szeretne keresni:\n 1\t nev alapjan\n 2\t alak alapjan\n 3\t tollazat alapjan\n 4\t elohely alapjan\n");
	scanf("%d", &feladat);

	switch (feladat)
	{

	case 1: 	
		printf("adja meg melyik madarra akar rakeresni (nagybetukkel):");
		scanf("%s", key);

		while (strcmp(key, kereses->madar.name) != 0)
		{
			kereses = kereses->next;
		}

		printf("sikeres egyezes %s \n", kereses);
		dataprinter(kereses);
		break;

	//alak alapjan
	case 2: 
		printf("adjon meg egy szamot\n");
		printf("0 - PARTIMADAR\n1 - BAGOLY\n2 - VARJU\n3 - GALAMB\n4 - TYUK\n5 - HARKALY\n6 - ENEKESMADAR\n7 - HOSSZULABUMADAR\n8 - RAGADOZO\n9 - SIRALY\n10 - RECE\n");
		scanf("%d", &alaknum);
		if (alaknum < 0 || alaknum > 10)
			printf("ervenytelen adat");
		else
			while (kereses != NULL)
			{
				if (alaknum == kereses->madar.alak)
				{
					dataprinter(kereses);
				}
				kereses = kereses->next;
			}
		break;

	//tollazat
	case 3:
		printf("adjon meg egy szamot\n");
		printf("0 - FEHER\n1 - SZURKE\n2 - BARNA\n3 - SARGA\n4 - NARANCS\n5 - PIROS\n6 - KEK\n7 - ZOLD\n8 - FEKETE\n");
		scanf("%d", &tollnum);
		if (tollnum < 0 || tollnum > 8)
			printf("ervenytelen adat");
		else
			while (kereses != NULL)
			{
				if (tollnum == kereses->madar.color)
				{
					dataprinter(kereses);
				}
				kereses = kereses->next;
			}
		break;

	//elohely
	case 4:
		printf("adjon meg egy szamot\n");
		printf("0 - TELEPULES\n1 - MEZOGAZDASAG\n2 - ERDO\n3 - VIZPART\n4 - HEGYSEG\n5 - RET\n");
		scanf("%d", &elonum);
		if (elonum < 0 || elonum > 5)
			printf("ervenytelen adat");
		else
			while (kereses != NULL)
			{
				if (elonum == kereses->madar.hely)
				{
					dataprinter(kereses);
				}
				kereses = kereses->next;
			}
		break;

	default: printf("ervenytelen szam");
		break;
	}
}

void hatarozas(birdlistelem* madarhead)
{
	int alaknum, tollnum, elonum;
	birdlistelem* search = madarhead;


	printf("adja meg a parametereket! (mindharom kell)");
	printf("0 - PARTIMADAR\n1 - BAGOLY\n2 - VARJU\n3 - GALAMB\n4 - TYUK\n5 - HARKALY\n6 - ENEKESMADAR\n7 - HOSSZULABUMADAR\n8 - RAGADOZO\n9 - SIRALY\n10 - RECE\n");
	scanf("%d", &alaknum);
	printf("0 - FEHER\n1 - SZURKE\n2 - BARNA\n3 - SARGA\n4 - NARANCS\n5 - PIROS\n6 - KEK\n7 - ZOLD\n8 - FEKETE\n");
	scanf("%d", &tollnum);
	printf("0 - TELEPULES\n1 - MEZOGAZDASAG\n2 - ERDO\n3 - VIZPART\n4 - HEGYSEG\n5 - RET\n");
	scanf("%d", &elonum);

	if (elonum < 0 || elonum > 5 || tollnum < 0 || tollnum > 8 || alaknum < 0 || alaknum > 10)
		printf("ervenytelen adat");
	else
		while (search != NULL)
		{
			if ((elonum == search->madar.hely) && (alaknum == search->madar.alak) && (tollnum == search->madar.color))
			{
				dataprinter(search);
			}
			search = search->next;
		}
}

void special(birdlistelem* madarhead)
{
	//Melyik volt a 6:00-9:00h között legtöbbször megfigyelt énekesmadár?

	int eszlelt, maxeszlelt = 0;
	birdlistelem *aktualis = madarhead;
	birdlistelem *maxmadar = madarhead;

	while (aktualis != NULL)
	{
		if (aktualis->madar.alak == ENEKESMADAR)
		{
			eszlelt = 0;
			loglistelem* counter = aktualis->loghead;
			for (int i = 0; i < aktualis->madar.eszleles; i++)
			{
				if ((counter->eszleles.time >= 600) && (counter->eszleles.time < 900))
					eszlelt++;
				counter = counter->next;
			}

			if (eszlelt > maxeszlelt)
			{
				maxeszlelt = eszlelt;
				maxmadar = aktualis;
			}
		}

		aktualis = aktualis->next;
	}

	printf("A legtobbet eszlelt enekesmadar: %s, ami ennyiszer volt eszlelve: %d", maxmadar->madar.name, maxeszlelt);
}

void welcome(birdlistelem* madarhead)
{
	int task;

	printf("mit szeretne csinalni?\n 1 \t megfigyeles hozzaadasa\n 2 \t kereses\n 3 \t madarhatarozas\n 4 \t specialis feladat\n");
	printf("adja meg a szamot:");
	scanf("%d", &task);

	switch (task)
	{
	case 1: addlog(madarhead);
		break;
	case 2: kereses(madarhead);
		break;
	case 3: hatarozas(madarhead);
		break;
	case 4: special(madarhead);
		break;
	default: printf("ervenytelen szam");
		break;
	}
}

void noslavery(birdlistelem* madarhead)
{
	//míg végig nem ér a listán
	birdlistelem* slaverunner = madarhead;
	birdlistelem* slavesupport;
	while (slaverunner != NULL)
	{
		//ha van eszleles a madarhoz
		if (slaverunner->madar.eszleles != 0)
		{
			//amíg végig nem ér az észleléseken
			loglistelem* slaves = slaverunner->loghead->next;
			loglistelem* slvnext;
			for(int i = 0; i < slaverunner->madar.eszleles; i++)
			{
				slvnext = slaves->next;
				free(slaves);
				slaves = slvnext;
			}
		}

		free(slaverunner->loghead);
		free(slaverunner->logtail);

		slavesupport = slaverunner->next;
		free(slaverunner);
		slaverunner = slavesupport;
	}
}

int main(void)
{
	int action;

	birdlistelem *madarhead = madarfilescan("madar.txt");
	birdlistelem* printer = madarhead;

	logfilescan("log.txt", madarhead);

	while (printer != NULL)
	{
		dataprinter(printer);
		printer = printer->next;
	}

	welcome(madarhead);

	printf("szeretne meg valamit csinalni? 1/0");
	scanf("%d", &action);
	while (action != 0)
	{
		welcome(madarhead);
		printf("szeretne meg valamit csinalni? 1/0");
		scanf("%d", &action);
	}

	noslavery(madarhead);

	return 0;
}
