#include <iostream>
#include <clocale>
#include <ctime>
using namespace std;

//Konstanter
//Max och minimum värden för slump-funktion
int const MAX_VALUE = 36;
int const MIN_VALUE = 1;
//Minsta tillåtna insats
int const MIN_BET = 100;
//Faktor att multiplicera insats med vid vinst vid rätt färg respektive nummer
int const COLOR_PAYOUT = 2;
int const NUMBER_PAYOUT = 10; 

//Global struct för hjulets utfall; nummer och färg.
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
    //Möjliggör stöd för åäö i konsollfönstret
    setlocale(LC_ALL, "swedish");
    
    //Kör uppstartsfunktion
    StartUp();

    //En while-loop som kör så länge spelaren inte väljer att avsluta eller har för lite pengar på kontot
    while (!gameOver)
    {
        //Funktioner att anropa under spelets gång
        NewRound();
        //If-sats som kontrollerar om spelaren valt att avsluta inför ny runda
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
    //Initierar globala variabler för pågående spel och återstående pengar
    gameOver = false;
    moneyRemaining = 1000;
    char answer;
    int age;
    cout << "Välkommen till Fabians roulettehjul!\n";
    cout << "Du måste vara över 18 år för att spela.\n";
    //Kontrollera spelarens ålder
    do
    {
        cout << "Ange din ålder: \n";
        cin >> age;
        cin.clear();
        cin.ignore(10000, '\n');
    } while (!age);
    //Om spelaren är under 18 år sätts gameOver-variabel till true
    if (age < 18)
    {
       cout << "Du måste vara minst 18 år för att få spela - avslutar Fabians roulettehjul.\n";
       gameOver = true;
    }
    //Skapa tomrader i konsollfönstret
    EmptyNewLines();
    
}

void NewRound()
{
    //Variabler för ny runda
    //Nolla hjulutfallets nummer inför en ny runda
    wheelOutcome.number = 0;
    int answer;

    //Kontrollera att spelaren har tillräckligt med pengar för att spela en runda.
    if (moneyRemaining < MIN_BET)
    {
        gameOver = true;
        return;
    }
    else
    {
        //Validera att korrekt värde matas in från användaren i en while loop
        //Använder cin.clear för att rensa ev. fel-status på inmatningsströmmen. Och cin.ignore för att rensa oönskade värden.
        do
        {
            cout << "HUVUDMENY\n";
            cout << "(1) Spela en runda på kasinohjulet\n";
            cout << "(2) Avsluta spelet och ta ut "  << moneyRemaining << " kr från spelkontot\n";
            cout << endl;
            //Visualisera antalet spelade omgångar och återsåtende pengar på kontot.
            cout << "------------------------------\n";
            cout << "SPELADE OMGÅNGAR:      [  " << numPlayedRounds <<" st ]\n";
            cout << "ÅTERSTÅENDE PENGAR:    [" << moneyRemaining << " kr]\n";
            cout << "TOTAL VINSTSUMMA:      [" << totMoneyWon << " kr]\n";
            cin >> answer;
            cin.clear();
            cin.ignore(10000, '\n');
        } while (!answer || answer < 1 || answer > 2);
        
        //Sätter gameOver till sant om användaren väljer att inte fortsätta den nya rundan
        if (answer == 2)
        {
            cout << "Avslutar spelet. På återseende!\n";
            gameOver = true;
        }
    }
    //Skapar tomrader i konsollfönstret
    EmptyNewLines();
}

void PlaceBet()
{
    //Variabel för input från användare
    int answer;
    //Användare matar in önskat val av insats.
    //Validera att korrekt värde matas in från användaren i en while loop. Ser till att använda cin.clear och cin.ignore för att undvika fel vid upprepad inmatning.
    do
    {
        cout << "Välj din insats:\n"; 
        cout << "(1) : 100 kr\n";
        cout << "(2) : 300 kr\n(3) : 500 kr\n";
        //cout << endl << moneyRemaining << " kr tillgängligt.";
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

    //Skapa tomrader i konsollfönstret
    EmptyNewLines();
}

void ValidateBet()
{
    //Kontrollerar att man inte valt en för hög insats mot återstående pengar
    //Reducerar den godtyckliga valda insatsen från användarens nuvarande saldo
    if (currentBet > moneyRemaining)
    {
        cout << "För hög insats vald. Välj en godtycklig insats. Du har " << moneyRemaining << " kronor kvar på kontot. ";
        PlaceBet();
    }
    else
    {
        moneyRemaining -= currentBet;
    }  
}

void PickGameMode()
{
    //Funktion för att låta användaren välja spelläge: nummer eller färg och tilldela detta till global variabel för spelläge

    //Do while-loop för att validera inmatningen. 
    //Loopar till godtycklig inmatning gjorts. Tilldelar gameMode-variabel.
    do 
    {
        cout << "Välj önskat spelläge:\n";
        cout << "(1): Spela på nummer - 10 gånger vid vinst\n";
        cout << "(2): Spela på färg - 2 gånger insats vid vinst\n";
        cin >> gameMode;
        cin.clear();
        cin.ignore(10000, '\n');
    } 
    while (!gameMode || gameMode < 1 || gameMode > 2);
    //Skapa tomrader i konsollfönstret
    EmptyNewLines();
}

void PickNumOrCol()
{
    //Användare matar in sitt bet beroende på spelläge: nummer eller färg.
    //GAME MODE 1 ÄR NUMMER
    //GAME MODE 2 ÄR FÄRG
    switch (gameMode)
    {
    //Vid val av siffra valideras att inmatning är en siffra och mellan intervallet 1-36.
    case 1:
        do
        {
            cout << "Ange önskat nummer mellan 1-36: \n";
            cin >> numPicked;
            cin.clear();
            cin.ignore(10000, '\n');
        } 
        while (!numPicked || numPicked < 1 || numPicked > 36);
        break;
        
    //Vid val av färg valideras att inmatning 
    case 2:
        do
        {
            cout << "Ange önskad färg: \n";
            cout << "Svart (S) \n";
            cout << "Röd (R) \n";
            cin >> colPicked;
            cin.clear();
            cin.ignore(10000, '\n');
        } 
        while (colPicked != 's' && colPicked != 'r' && colPicked != 'S' && colPicked != 'R');
        break;
        
    }
    //Skapa tomrader i konsollfönstret
    EmptyNewLines();

}

void SpinWheel()
{
    //Streck för gränssnittets skull
    cout << "//////////////////////////////////\n";
    //Funktion för att snurra på roulettehjulet och slumpa fram ett nummer mellan 1 och 36.
    srand(time(0));
    wheelOutcome.number = rand() % 36 + 1;
    cout << "ROULETTEHJULET STANNAR PÅ\n";
    cout << "NUMMER: " << wheelOutcome.number << endl;


}

void CalculateColor()
{
    //Ett roulettehjuls siffror är färgade i svart eller rött enligt vissa regler. Vi är i detta program endast intresserade av siffrorna 1 till 36.
    //Inom intervallen 1-10 och 19-28 är ojämna tal röda och jämna svarta. 
    //Inom intervallen 11-18 och 29-36 är ojämna tal svarta och jämna röda.
    //Nedan följer sammanflätade if-satser för att urskilja färg på det framslumpade värdet mellan 1-36
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

    //En switch för att skriva ut färgen i en sträng istället för char som används i datatypen
    switch (wheelOutcome.color)
    {
    case 's':
        cout << "FÄRG: SVART\n";
        break;

    case 'r':
        cout << "FÄRG: RÖD\n";
        break;
    }
    
    
    //Streck för gränssnittet skull
    cout << "//////////////////////////////////\n";
    //Skapa tomrader i konsollfönstret
    EmptyNewLines();
}

void CalculateOutcome()
{
    //Funktion för att beräkna utfall av rundan. Eventuella vinstpengar är beroende på val av spelläge(färg eller nummer) och beräknas därefter.
    //Rätt färg ger 2x insats. Rätt nummer ger 10x insats. 
    //GAME MODE 1 ÄR NUMMER
    //GAME MODE 2 ÄR FÄRG


    //Lokala variabler
    int moneyWon;
    bool roundWin = false;
    switch (gameMode)
    {
    //Vid spelläge val av nummer
    case 1: 
        //If-sats för att kontrollera om rätt nummer valts mot hjulets utfall
        //Beroende på vinst/förlust uppdateras boolean till sant eller falskt för att kunna hantera cout utan switch statement
        if (wheelOutcome.number == numPicked)
        {
            //Beräknar vinst genom att multiplicera med konstanten för rätt nummer-odds
            moneyWon = currentBet * NUMBER_PAYOUT;
            cout << "GRATTIS! DU VALDE RÄTT NUMMER!\n";
            cout << "DU VINNER " << moneyWon << " kr.\n";
            cout << "Pengarna har satts över på ditt konto.\n";
            roundWin = true;
        }
        else roundWin = false;
        break;

    //Vid spelläge val av färg
    case 2:
        //If-sats för att kontrollera om rätt färg valts mot hjulets utfall
        if (wheelOutcome.color == colPicked)
        {
            //Beräknar vinst genom att multiplicera med konstanten för rätt färg-odds
            moneyWon = currentBet * COLOR_PAYOUT;
            cout << "GRATTIS! DU VALDE RÄTT FÄRG!\n";
            cout << "DU VINNER " << moneyWon << " kr.\n";
            cout << "Pengarna har satts över på ditt konto.\n";
            roundWin = true;
        }
        else roundWin = false;
        break;
    }
    //Visa text beroende på vinst eller förlust oavsett spelläge
    //Adderar denna rundans ev. vinst till variabel för totala vinst
    //Lägger tillbaka vinsten till användarens konto
    
    if (roundWin)
    {
        totMoneyWon += moneyWon;
        moneyRemaining += moneyWon;
        cout << endl;
        cout << "------------------------------------\n";
        cout << "Du har totalt vunnit " << totMoneyWon << " sedan du började spela.\n";
    }
    else cout << "Tyvärr ingen vinst denna gång. Bättre lycka nästa gång.\n";
    
    //Skapa tomrader i konsollfönstret
    EmptyNewLines();
    
    //Addera variabel för antalet spelade omgångar med 1
    numPlayedRounds++;
    
    //För att inte direkt återvända till första menyn ber jag användare bekräfta utfallet
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
    
    //Sätter variabel gameOver till true om användare valt att avsluta spelet
    if (answer == 2)
        gameOver = true;

    //Skapa tomrader i konsollfönstret
    EmptyNewLines();
}

void EmptyNewLines()
{
    //Funktion för att skapa ett par tomma rader för att ge ett renare gränssnitt mellan funktionerna
    //Loopar en en ny rad 2 gånger
    for (int i = 0; i < 2; i++)
    {
        cout << "\n";
    }
}