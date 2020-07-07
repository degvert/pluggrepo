#include <iostream>
#include <clocale>
#include <ctime>
using namespace std;

//Konstanter
//Max och minimum v�rden f�r slump-funktion
int const MAX_VALUE = 36;
int const MIN_VALUE = 1;
//Minsta till�tna insats
int const MIN_BET = 100;
//Faktor att multiplicera insats med vid vinst vid r�tt f�rg respektive nummer
int const COLOR_PAYOUT = 2;
int const NUMBER_PAYOUT = 10; 

//Global struct f�r hjulets utfall; nummer och f�rg.
struct WheelOutcome
{
    int number;
    char color;
};

//Globala variabler
bool gameOver;
int moneyRemaining;
int totMoneyWon;
int numPlayedRounds = 0;
int currentBet;
int gameMode;
int numPicked;
char colPicked;
WheelOutcome wheelOutcome;


//Deklarerar funktioner
void StartUp();
void NewRound();
void PlaceBet();
void ValidateBet();
void PickGameMode();
void PickNumOrCol();
void SpinWheel();
void CalculateColor();
void CalculateOutcome();
void EmptyNewLines();


int main()
{
    //M�jligg�r st�d f�r ��� i konsollf�nstret
    setlocale(LC_ALL, "swedish");
    
    //K�r uppstartsfunktion
    StartUp();

    //En while-loop som k�r s� l�nge spelaren inte v�ljer att avsluta eller har f�r lite pengar p� kontot
    while (!gameOver)
    {
        //Funktioner att anropa under spelets g�ng
        NewRound();
        //If-sats som kontrollerar om spelaren valt att avsluta inf�r ny runda
        if (gameOver)
        return 0;
        PlaceBet();
        PickGameMode();
        PickNumOrCol();
        SpinWheel();
        CalculateColor();
        CalculateOutcome();
    }

}

void StartUp()
{
    //Initierar globala variabler f�r p�g�ende spel och �terst�ende pengar
    gameOver = false;
    moneyRemaining = 1000;
    char answer;
    int age;
    cout << "V�lkommen till Fabians roulettehjul!\n";
    cout << "Du m�ste vara �ver 18 �r f�r att spela.\n";
    //Kontrollera spelarens �lder
    do
    {
        cout << "Ange din �lder: \n";
        cin >> age;
        cin.clear();
        cin.ignore(10000, '\n');
    } while (!age);
    //Om spelaren �r under 18 �r s�tts gameOver-variabel till true
    if (age < 18)
    {
       cout << "Du m�ste vara minst 18 �r f�r att f� spela - avslutar Fabians roulettehjul.\n";
       gameOver = true;
    }
    //Skapa tomrader i konsollf�nstret
    EmptyNewLines();
    
}

void NewRound()
{
    //Variabler f�r ny runda
    //Nolla hjulutfallets nummer inf�r en ny runda
    wheelOutcome.number = 0;
    int answer;

    //Kontrollera att spelaren har tillr�ckligt med pengar f�r att spela en runda.
    if (moneyRemaining < MIN_BET)
    {
        gameOver = true;
        return;
    }
    else
    {
        //Validera att korrekt v�rde matas in fr�n anv�ndaren i en while loop
        //Anv�nder cin.clear f�r att rensa ev. fel-status p� inmatningsstr�mmen. Och cin.ignore f�r att rensa o�nskade v�rden.
        do
        {
            cout << "HUVUDMENY\n";
            cout << "(1) Spela en runda p� kasinohjulet\n";
            cout << "(2) Avsluta spelet och ta ut "  << moneyRemaining << " kr fr�n spelkontot\n";
            cout << endl;
            //Visualisera antalet spelade omg�ngar och �ters�tende pengar p� kontot.
            cout << "------------------------------\n";
            cout << "SPELADE OMG�NGAR:      [  " << numPlayedRounds <<" st ]\n";
            cout << "�TERST�ENDE PENGAR:    [" << moneyRemaining << " kr]\n";
            cout << "TOTAL VINSTSUMMA:      [" << totMoneyWon << " kr]\n";
            cin >> answer;
            cin.clear();
            cin.ignore(10000, '\n');
        } while (!answer || answer < 1 || answer > 2);
        
        //S�tter gameOver till sant om anv�ndaren v�ljer att inte forts�tta den nya rundan
        if (answer == 2)
        {
            cout << "Avslutar spelet. P� �terseende!\n";
            gameOver = true;
        }
    }
    //Skapar tomrader i konsollf�nstret
    EmptyNewLines();
}

void PlaceBet()
{
    //Variabel f�r input fr�n anv�ndare
    int answer;
    //Anv�ndare matar in �nskat val av insats.
    //Validera att korrekt v�rde matas in fr�n anv�ndaren i en while loop. Ser till att anv�nda cin.clear och cin.ignore f�r att undvika fel vid upprepad inmatning.
    do
    {
        cout << "V�lj din insats:\n"; 
        cout << "(1) : 100 kr\n";
        cout << "(2) : 300 kr\n(3) : 500 kr\n";
        //cout << endl << moneyRemaining << " kr tillg�ngligt.";
        cin >> answer;
        cin.clear();
        cin.ignore(10000, '\n');
    } while (!answer || answer < 1 || answer > 3);

    //
    switch (answer)
    {
    case 1:
        currentBet = 100;
        break;
    case 2:
        currentBet = 300;
        break;
    case 3: 
        currentBet = 500;
        break;
    }
    //Kontrollera vald insats i ValidateBet-funktion
    ValidateBet();

    //Skapa tomrader i konsollf�nstret
    EmptyNewLines();
}

void ValidateBet()
{
    //Kontrollerar att man inte valt en f�r h�g insats mot �terst�ende pengar
    //Reducerar den godtyckliga valda insatsen fr�n anv�ndarens nuvarande saldo
    if (currentBet > moneyRemaining)
    {
        cout << "F�r h�g insats vald. V�lj en godtycklig insats. Du har " << moneyRemaining << " kronor kvar p� kontot. ";
        PlaceBet();
    }
    else
    {
        moneyRemaining -= currentBet;
    }  
}

void PickGameMode()
{
    //Funktion f�r att l�ta anv�ndaren v�lja spell�ge: nummer eller f�rg och tilldela detta till global variabel f�r spell�ge

    //Do while-loop f�r att validera inmatningen. 
    //Loopar till godtycklig inmatning gjorts. Tilldelar gameMode-variabel.
    do 
    {
        cout << "V�lj �nskat spell�ge:\n";
        cout << "(1): Spela p� nummer - 10 g�nger vid vinst\n";
        cout << "(2): Spela p� f�rg - 2 g�nger insats vid vinst\n";
        cin >> gameMode;
        cin.clear();
        cin.ignore(10000, '\n');
    } 
    while (!gameMode || gameMode < 1 || gameMode > 2);
    //Skapa tomrader i konsollf�nstret
    EmptyNewLines();
}

void PickNumOrCol()
{
    //Anv�ndare matar in sitt bet beroende p� spell�ge: nummer eller f�rg.
    //GAME MODE 1 �R NUMMER
    //GAME MODE 2 �R F�RG
    switch (gameMode)
    {
    //Vid val av siffra valideras att inmatning �r en siffra och mellan intervallet 1-36.
    case 1:
        do
        {
            cout << "Ange �nskat nummer mellan 1-36: \n";
            cin >> numPicked;
            cin.clear();
            cin.ignore(10000, '\n');
        } 
        while (!numPicked || numPicked < 1 || numPicked > 36);
        break;
        
    //Vid val av f�rg valideras att inmatning 
    case 2:
        do
        {
            cout << "Ange �nskad f�rg: \n";
            cout << "Svart (S) \n";
            cout << "R�d (R) \n";
            cin >> colPicked;
            cin.clear();
            cin.ignore(10000, '\n');
        } 
        while (colPicked != 's' && colPicked != 'r' && colPicked != 'S' && colPicked != 'R');
        break;
        
    }
    //Skapa tomrader i konsollf�nstret
    EmptyNewLines();

}

void SpinWheel()
{
    //Streck f�r gr�nssnittets skull
    cout << "//////////////////////////////////\n";
    //Funktion f�r att snurra p� roulettehjulet och slumpa fram ett nummer mellan 1 och 36.
    srand(time(0));
    wheelOutcome.number = rand() % 36 + 1;
    cout << "ROULETTEHJULET STANNAR P�\n";
    cout << "NUMMER: " << wheelOutcome.number << endl;


}

void CalculateColor()
{
    //Ett roulettehjuls siffror �r f�rgade i svart eller r�tt enligt vissa regler. Vi �r i detta program endast intresserade av siffrorna 1 till 36.
    //Inom intervallen 1-10 och 19-28 �r oj�mna tal r�da och j�mna svarta. 
    //Inom intervallen 11-18 och 29-36 �r oj�mna tal svarta och j�mna r�da.
    //Nedan f�ljer sammanfl�tade if-satser f�r att urskilja f�rg p� det framslumpade v�rdet mellan 1-36
    if (wheelOutcome.number <= 10)
        if (wheelOutcome.number % 2 == 0)
            wheelOutcome.color = 's';
        else wheelOutcome.color = 'r';
    else if (wheelOutcome.number >= 19 && wheelOutcome.number <= 28)
        if (wheelOutcome.number % 2 == 0)
            wheelOutcome.color = 's';
        else wheelOutcome.color = 'r';
    else if (wheelOutcome.number >= 11 && wheelOutcome.number <= 18)
        if (wheelOutcome.number % 2 == 0)
            wheelOutcome.color = 'r';
        else wheelOutcome.color = 's';
    else if (wheelOutcome.number >= 29 && wheelOutcome.number <= 36)
        if (wheelOutcome.number % 2 == 0)
            wheelOutcome.color = 'r';
        else wheelOutcome.color = 's';

    //En switch f�r att skriva ut f�rgen i en str�ng ist�llet f�r char som anv�nds i datatypen
    switch (wheelOutcome.color)
    {
    case 's':
        cout << "F�RG: SVART\n";
        break;

    case 'r':
        cout << "F�RG: R�D\n";
        break;
    }
    
    
    //Streck f�r gr�nssnittet skull
    cout << "//////////////////////////////////\n";
    //Skapa tomrader i konsollf�nstret
    EmptyNewLines();
}

void CalculateOutcome()
{
    //Funktion f�r att ber�kna utfall av rundan. Eventuella vinstpengar �r beroende p� val av spell�ge(f�rg eller nummer) och ber�knas d�refter.
    //R�tt f�rg ger 2x insats. R�tt nummer ger 10x insats. 
    //GAME MODE 1 �R NUMMER
    //GAME MODE 2 �R F�RG


    //Lokala variabler
    int moneyWon;
    bool roundWin = false;
    switch (gameMode)
    {
    //Vid spell�ge val av nummer
    case 1: 
        //If-sats f�r att kontrollera om r�tt nummer valts mot hjulets utfall
        //Beroende p� vinst/f�rlust uppdateras boolean till sant eller falskt f�r att kunna hantera cout utan switch statement
        if (wheelOutcome.number == numPicked)
        {
            //Ber�knar vinst genom att multiplicera med konstanten f�r r�tt nummer-odds
            moneyWon = currentBet * NUMBER_PAYOUT;
            cout << "GRATTIS! DU VALDE R�TT NUMMER!\n";
            cout << "DU VINNER " << moneyWon << " kr.\n";
            cout << "Pengarna har satts �ver p� ditt konto.\n";
            roundWin = true;
        }
        else roundWin = false;
        break;

    //Vid spell�ge val av f�rg
    case 2:
        //If-sats f�r att kontrollera om r�tt f�rg valts mot hjulets utfall
        if (wheelOutcome.color == colPicked)
        {
            //Ber�knar vinst genom att multiplicera med konstanten f�r r�tt f�rg-odds
            moneyWon = currentBet * COLOR_PAYOUT;
            cout << "GRATTIS! DU VALDE R�TT F�RG!\n";
            cout << "DU VINNER " << moneyWon << " kr.\n";
            cout << "Pengarna har satts �ver p� ditt konto.\n";
            roundWin = true;
        }
        else roundWin = false;
        break;
    }
    //Visa text beroende p� vinst eller f�rlust oavsett spell�ge
    //Adderar denna rundans ev. vinst till variabel f�r totala vinst
    //L�gger tillbaka vinsten till anv�ndarens konto
    
    if (roundWin)
    {
        totMoneyWon += moneyWon;
        moneyRemaining += moneyWon;
        cout << endl;
        cout << "------------------------------------\n";
        cout << "Du har totalt vunnit " << totMoneyWon << " sedan du b�rjade spela.\n";
    }
    else cout << "Tyv�rr ingen vinst denna g�ng. B�ttre lycka n�sta g�ng.\n";
    
    //Skapa tomrader i konsollf�nstret
    EmptyNewLines();
    
    //Addera variabel f�r antalet spelade omg�ngar med 1
    numPlayedRounds++;
    
    //F�r att inte direkt �terv�nda till f�rsta menyn ber jag anv�ndare bekr�fta utfallet
    int answer;
    do
    {
        cout << "Val:\n";
        cout << "(1) : Tillbaka till huvudmenyn\n";
        cout << "(2) : Ta ut pengarna (" << moneyRemaining << "kr) och avsluta spel\n";
        cin >> answer;
        cin.clear();
        cin.ignore(10000, '\n');
    } while (answer != 1 && answer != 2);
    
    //S�tter variabel gameOver till true om anv�ndare valt att avsluta spelet
    if (answer == 2)
        gameOver = true;

    //Skapa tomrader i konsollf�nstret
    EmptyNewLines();
}

void EmptyNewLines()
{
    //Funktion f�r att skapa ett par tomma rader f�r att ge ett renare gr�nssnitt mellan funktionerna
    //Loopar en en ny rad 2 g�nger
    for (int i = 0; i < 2; i++)
    {
        cout << "\n";
    }
}