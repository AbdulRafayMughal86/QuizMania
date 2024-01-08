#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>

struct Question{
	
	char q[160];
	char hint[100];
	char ans[25];


} questions[100], question; 

void display(const char* name, int type);
void showHangman(int x, int y, int number);
void readQuestions(struct Question ques[]);
void getQuestion(int type, char* ansPtr);
int getDifficulty();
int getType();
void gotoline(int x, int y);
void changeColor(int n);
void print(const char *msg, int x, int y, int times, int pause, int charSpeed);
void printlines(char msg[][60], int lines, int x, int y, int times, int speed);

int main() {
	readQuestions(questions);

    int toEnd = 1;
    char mainChoice = 'A';

    while(toEnd){

        fflush(stdin);
        display("mainMenu", 0);

        fflush(stdin);
        scanf("%c", &mainChoice);
        if(mainChoice >= 'a' && mainChoice <= 'z') mainChoice -= 32;
        while(mainChoice != 'E' && mainChoice != 'S' && mainChoice != 'A'){
            printf("Invalid Choice! \nPlease choose again from choices: ");
            fflush(stdin);
            scanf("%c", &mainChoice);
            if(mainChoice >= 'a' && mainChoice <= 'z') mainChoice -= 32;
        }

        if(mainChoice == 'A'){
            display("About Game", 0);
            continue;
        }       
        else if(mainChoice == 'E')
            break;
    
        char secretWord[30], guessedWord[30], guess;
        int maxAttempts = 6, attempts, type, hintsAvailable, difficulty, toPlay = 1;
        
        display("mainScreen", 1);
        gotoline(0, 10);
        type = getType();

        display("mainScreen", 1);
        gotoline(0, 10);
        difficulty = getDifficulty();

        while(toPlay) {
            display("mainScreen", 0);

            attempts = 0, hintsAvailable = 1;
            if(difficulty == 3)
                hintsAvailable = 0;

            getQuestion(type, secretWord);
            strcpy(guessedWord, secretWord);
            for(int i = 0 ; i < strlen(guessedWord) ; i++){
                if(guessedWord[i] >= 'A' && guessedWord[i] <= 'Z')
                    guessedWord[i] = '_';
                else if(guessedWord[i] >= 'a' && guessedWord[i] <= 'z'){
                    secretWord[i] -= 32;
                    guessedWord[i] = '_';
                }
            }
            display("mainScreen", 0);

            print(question.q, 0, 10, 1, 100, 20);
            display("hint", hintsAvailable);
            display("difficulty", difficulty);
            display("toMain", 0);
            display("toExit", 0);

            while(attempts < maxAttempts){
                gotoline(0, 14);
                printf("Current Word: %s \n", guessedWord);
                printf("Enter a letter:           \b\b\b\b\b\b\b\b\b\b");
            
                showHangman(25, 15, attempts);

                gotoline(16, 15);
                fflush(stdin);
                scanf("%c", &guess);
                if(guess >= 'a' && guess <= 'z') guess -= 32;
                
                if(guess == '1'){
                    if(hintsAvailable){
                        print(question.hint, 0, 12, 1, 0, 40);
                        hintsAvailable = 0;
                        display("hint", hintsAvailable);
                    }
                    continue;
                }
                else if(guess == '2'){
                    char ch;
                    print("Are You Sure(Enter Y to confirm N to deny): ", 40, 28, 1, 0, 50);
                    fflush(stdin);
                    scanf("%c", &ch);
                    if(ch == 'Y' || ch == 'y'){
                        toPlay = 0;
                        break;
                    }
                    else{
                        print("                                                 ", 40, 28, 1, 0, 0);
                        continue;
                    }
                }
                else if(guess == '3'){
                    char ch;
                    print("Are You Sure(Enter Y to confirm N to deny): ", 40, 28, 1, 0, 50);
                    fflush(stdin);
                    scanf("%c", &ch);
                    if(ch == 'Y' || ch == 'y'){
                        toPlay = 0;
                        toEnd = 0;
                        break;
                    }
                    else{
                        print("                                                 ", 40, 28, 1, 0, 0);
                        continue;
                    }
                }
                else if(guess < 'A' || guess > 'Z')
                    continue;

                int found = 0;

                for (int i = 0; i < strlen(secretWord); i++) {
                    if (secretWord[i] == guess) {
                        guessedWord[i] = guess;
                        found = 1;
                    }
                }

                if (!(found)){
                    if(difficulty > 1 && attempts > 0)
                        attempts++;
                    attempts++;
                }

                if (strcmp(guessedWord, secretWord) == 0) {
                    gotoline(0, 14);
                    printf("Current Word: %s \n", guessedWord);
                    print("Well Guessed", 0, 17, 1, 0, 50);
                    gotoline(35,27);
                    system("PAUSE");
                    break;
                }
            }

            if (attempts >= maxAttempts) {
                system("CLS");
                display("mainScreen", 0);
                showHangman(25, 15, 5);

                print("You've run out of attempts!", 65, 13, 1, 300, 30);
                
                print("Answer: ", 65, 14, 1, 0, 50);
                print(secretWord, 73, 14, 1, 0, 50);

                do{
                    print("Enter 1 to play or 2 to goto MainMenu or 0 to exit:    ", 65, 16, 1, 0, 0);
                    gotoline(117, 16);
                    scanf("%d", &toPlay);
                    if(toPlay == 0)
                        toEnd = 0;
                    if(toPlay == 2)
                        toPlay = 0;
                }while(toPlay != 0 && toPlay != 1 && toPlay != 2);
            }	

        }
    }
	display("Exit Screen", 0);

}

void changeColor(int n){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n);
}
void gotoline(int x, int y){
	COORD pos = {(short)x, (short)y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void readQuestions(struct Question ques[]){
	FILE *fptr = fopen("Questions.txt", "r");
	if(fptr == NULL){
		printf("File not found!");
		exit(1);
	}
	for(int i = 0 ; i < 100 ; i++){
		fgets(ques[i].q, 180, fptr);
        if(ques[i].q[strlen(ques[i].q) - 1] == '\n')
		    ques[i].q[strlen(ques[i].q) - 1] = '\0';
		
        fgets(ques[i].hint, 100, fptr);
        if(ques[i].hint[strlen(ques[i].hint) - 1] == '\n')
		    ques[i].hint[strlen(ques[i].hint) - 1] = '\0';
		
        fgets(ques[i].ans, 30, fptr);
        if(ques[i].ans[strlen(ques[i].ans) - 1] == '\n')
		    ques[i].ans[strlen(ques[i].ans) - 1] = '\0';
	}
	fclose(fptr);
}
void getQuestion(int type, char* ansPtr){
	srand(time(0));
	int i;
    switch(type){
        case 1:
            i = rand() % 25; break;
        case 2:
            i = rand() % 25 + 25; break;
        case 3:
            i = rand() % 25 + 50; break;
        case 4:
            i = rand() % 25 + 75; break;
        default:
            i = rand() % 100;
    }
    strcpy(question.q, questions[i].q);
    strcpy(question.hint, questions[i].hint);
    strcpy(question.ans, questions[i].ans);
    strcpy(ansPtr, questions[i].ans + 8);
}
int getType(){
	int choice;
	printf("Enter the choice of questions for upcoming quiz: \n\n");
	printf("1: History.\n");
	printf("2: General Science.\n");
	printf("3: Computer Science.\n");
	printf("4: Human Biology.\n");
	printf("0: Everytype of questions. \n\n");

	printf("Enter your choice: ");
	scanf("%d", &choice);

	while(choice < 0 || choice > 4){
		printf("Invalid Choice! \nPlease choose again from given choices: ");
		scanf("%d", &choice);
	}
	return choice;
}
int getDifficulty(){
	int difficulty;
	printf("Enter the difficulty level of questions for upcoming quiz: \n\n");
	printf("1: Easy (For Beginners, 5 attempts with a hint).\n");
	printf("2: Medium (For Amatuers, 3 attempts with a hint).\n");
	printf("3: Hard (For Professionals, 3 attempts no hints available).\n\n");

	printf("Enter your choice: ");
	scanf("%d", &difficulty);

	while(difficulty < 1 || difficulty > 3){
		printf("Invalid difficulty! \nPlease choose again: ");
		scanf("%d", &difficulty);
	}
	return difficulty;

}
void display(const char* name, int type){
	char ascii[][60] = {
		"  ____        _      __  __             _       ",
		" / __ \\      (_)    |  \\/  |           (_)      ",
		"| |  | |_   _ _ ___ | \\  / | __ _ _ __  _  __ _ ",
		"| |  | | | | | |_  // |\\/| |/ _` | '_ \\| |/ _` |",
		"| |__| | |_| | |/ / | |  | | (_| | | | | | (_| |",
		" \\___\\_\\\\__,_|_/___ |_|  |_|\\__,_|_| |_|_|\\__,_|"
	};
    if(strcmp(name, "mainMenu") == 0){
        system("CLS");
		printlines(ascii, 6, 25, 2, 1, 0);
        
        gotoline(45, 10);
        printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",220,220,220,220,220,220,220,220,220,220,220,220,220,220,220);
        gotoline(45, 11);
        printf("%c  MAIN MENU  %c",221, 222 );
        gotoline(45, 12);
        printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",223,223,223,223,223,223,223,223,223,223,223,223,223,223,223);
        gotoline(45, 13);

		gotoline(15, 14);
        printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",220,220,220,220,220,220,220,220,220,220,220,220,220,220,220);
        gotoline(15, 15);
        printf("%c  Press \'S\'  %c",221, 222 );
        gotoline(15, 16);
        printf("%c   To Start  %c",221, 222 );
        gotoline(15, 17);
        printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",223,223,223,223,223,223,223,223,223,223,223,223,223,223,223);
        gotoline(15, 18);

        gotoline(45, 14);
        printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",220,220,220,220,220,220,220,220,220,220,220,220,220,220,220);
        gotoline(45, 15);
        printf("%c  Press \'A\'  %c",221, 222 );
        gotoline(45, 16);
        printf("%c About Game  %c",221, 222 );
        gotoline(45, 17);
        printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",223,223,223,223,223,223,223,223,223,223,223,223,223,223,223);
        gotoline(45, 18);

        gotoline(75, 14);
        printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",220,220,220,220,220,220,220,220,220,220,220,220,220,220,220);
        gotoline(75, 15);
        printf("%c  Press \'E\'  %c",221, 222 );
        gotoline(75, 16);
        printf("%c   To Exit   %c",221, 222 );
        gotoline(75, 17);
        printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",223,223,223,223,223,223,223,223,223,223,223,223,223,223,223);
        gotoline(75, 18);

        print("QuizMania: ", 1, 19, 1, 750, 100);
        print("A game where diverse questions and a hangman twist await you.", 12,  19, 1, 300, 30);
        print("How many can you solve before you run out of guesses?", 12, 20, 1, 300, 30);

        print("Enter Your Choice: ", 0, 22, 1, 0, 0);
	}
    else if(strcmp(name, "About Game") == 0){
        char ask = 1;
        while(1){

            system("CLS");
            printlines(ascii, 6, 25, 2, 1, 0);
            
            print("Choose from the options below that best suit your needs: \n", 0, 10, 1, 100, 20);
            print("1. About Game.      \n2. Features      \n3. How to Play      \n4. To Exit.", 0, 11, 1, 300, 30);

            print("Enter Your Choice: ", 0, 16, 1, 0, 30);
            scanf("%d", &ask);

            while(ask < 1 || ask > 4){
                printf("Invalid Choice! Please Choose again: ");
                fflush(stdin);
                scanf("%d", &ask);
            }
            system("CLS");
            printlines(ascii, 6, 25, 2, 1, 0);

            switch(ask){
                
                case 1:

                    print("ABOUT GAME: ", 0, 11, 1, 300, 50);

                    print("Introduction: \n", 5, 13, 1, 200, 50);
                    print("Quizmania is an engaging and  educational game that combines the elements of quiz and \n", 15, 14, 1, 0, 0);
                    print("hangman. You can choose from a  variety of topics, such as history, computer science, \n", 15, 15, 1, 0, 0); 
                    print("or general science, and  adjustable  difficulty and hints.  Quizmania  will test your \n", 15, 16, 1, 0, 0);
                    print("knowledge, expand your horizons, and provide you with hours of fun. It is a game that \n", 15, 17, 1, 0, 0);
                    print("stimulates your mind, enhances your learning, and delights your senses.\n", 15, 18, 1, 300, 0);
                        
                    print("Objective: \n", 5, 20, 1, 200, 50);
                    print("To enhance the learning experience by incorporating a hangman game in a quiz app thats \n", 15, 21, 1, 0, 0);
                    print("stimulate the learners\' dopamine levels and motivation while acquiring new knowledge.\n\n", 15, 22, 1, 300, 0);
                    break;

                case 2:
                    print("Features: \n", 0, 11, 1, 300, 50);

                    print("Hangman Integration: \n", 5, 13, 1, 100, 50); 
                    print("A unique and engaging quiz game that combines the classic word game of hangman with challenging \n", 15, 14, 1, 0, 0);
                    print("trivia  questions.  By playing this game,  you can have  fun while  testing your  knowledge and \n", 15, 15, 1, 0, 0);
                    print("skills in various topics. \n", 15, 16, 1, 200, 0);
                    
                    print("Difficulty Levels: \n", 5, 18, 1, 100, 50);
                    print("This game has different difficulty levels to suit your preferences and abilities. \n", 15, 19, 1, 0, 0);
                    print("Whether you are a beginner or an expert, you will find a challenge that matches your skills.\n", 15, 20, 1, 0, 0);        
                    print("1. Easy: For beginners who want to learn new words in a fun way. \n", 15, 21, 1, 50, 0);
                    print("         5 attempts and a hint for answer.\n", 15, 22, 1, 50, 0);
                    print("2. Medium: For amateurs who want to challenge themselves and improve their vocabulary. \n", 15, 23, 1, 50, 0);
                    print("           3 attempts and a hint for answer.\n", 15, 24, 1, 50, 0);
                    print("3. Hard: For professionals who are confident in their knowledge and want to test their limits. \n", 15, 25, 1, 50, 0);
                    print("         3 attempts and no hints available.\n", 15, 26, 1, 200, 0);

                    print("Variety of Topics: \n", 5, 28, 1, 100, 50);        
                    print("This quiz game has a variety of topics that will challenge your brain and keep you entertained. \n", 15, 29, 1, 0, 0);
                    print("Whether you are interested in  history, science, sports,  entertainment, or anything else, you\n", 15, 30, 1, 0, 0);
                    print("will find something that suits your taste.\n", 15, 31, 1, 200, 0);

                    print("Hints: \n", 5, 33, 1, 100, 50);
                    print("To make the game more interesting and helpful, you can also use a hint (Not on Hard Difficulty)\n", 15, 34, 1, 0, 0);
                    print("that reveals a sentence related to the answer.  The hint can give you some clues or facts that \n", 15, 35, 1, 0, 0);
                    print("can help you guess the word(answer) \n\n", 15, 36, 1, 200, 0);
                    break;
                    
                case 3:
                    print("How To Play: \n", 0, 11, 1, 200, 50);

                    print("1. Select a topic from the available options to answer questions related to it. \n", 5, 13, 1, 300, 0);
                    print("2. Select your preferred difficulty level for the quiz from the given options. \n", 5, 14, 1, 300, 0);
                    print("3. Read the questions carefully  and enter the answer one character at a time. If you enter a\n", 5, 15, 1, 0, 0); 
                    print("   correct character, it will be displayed on the blank spaces else one of your attempts will \n", 5, 16, 1, 0, 0);
                    print("   be used and a part of the hangman will be drawn.\n", 5, 17, 1, 300, 0);
                    print("4. You can also use a hint(if available) if you need some help.\n", 5, 18, 1, 300, 0);
                    print("5. To navigate back to the main menu or gracefully exit the game, simply press the designated \n", 5, 19, 1, 0, 0);
                    print("   keys at your convenience\n", 5, 20, 1, 500, 0);

                    print("Do have a great time playing this quiz game and expanding your knowledge and skills\n\n", 0, 22, 1, 50, 20);

            }
            if(ask == 4)
                break;    
            system("PAUSE");
        }
            
    }
	else if(strcmp(name, "mainScreen") == 0){
        system("CLS");
		printlines(ascii, 6, 25, 2, 1, 0);
		if(type == 1)
			showHangman(60, 10, 5);
	}
    else if(strcmp(name, "hint") == 0){
            gotoline(65, 18);
            printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220);
            gotoline(65, 19);
        if(type == 0){
            printf("%c   No Hints   %c",221, 222 );
            gotoline(65, 20);
            printf("%c  Available   %c",221, 222 );
        }
        else if(type == 1){
            printf("%c    Press 1   %c",221, 222 );
            gotoline(65, 20);
            printf("%c   For Hint   %c",221, 222 );
        }
            gotoline(65, 21);
            printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223);
            gotoline(65, 22);
    }
	else if(strcmp(name, "difficulty") == 0){
            gotoline(75, 13);
            printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220);
            gotoline(75, 14);
            printf("%c  Difficulty  %c",221, 222 );
            gotoline(75, 15);
        if(type == 1){
            printf("%c     Easy     %c",221, 222 );
        }
		else if(type == 2){
            printf("%c    Medium    %c",221, 222 );
        }
		else if(type == 3){
            printf("%c     Hard     %c",221, 222 );
        }
            gotoline(75, 16);
            printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223);
            gotoline(75, 17);
	}
	else if(strcmp(name, "toMain") == 0){
            gotoline(85, 18);
            printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220);
            gotoline(85, 19);
            printf("%c   Press  2   %c",221, 222 );
            gotoline(85, 20);
            printf("%c For MainMenu %c",221, 222 );
            gotoline(85, 21);
            printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223);
            gotoline(85, 22);
    }
    else if(strcmp(name, "toExit") == 0){
            gotoline(75, 23);
            printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220);
            gotoline(75, 24);
            printf("%c   Press  3   %c",221, 222 );
            gotoline(75, 25);
            printf("%c    To Exit   %c",221, 222 );
            gotoline(75, 26);
            printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223);
            gotoline(75, 27);
    }
	else if(strcmp(name, "Exit Screen") == 0){
        system("CLS");
		print("THANKS FOR PLAYING THIS GAME", 40, 3, 1, 0, 100);
		char credits[6][60] = {
			{"         CREDITS            "},
			{"                            "},
			{"Abdul Rafay Mughal 23K-0667 "},
			{"     Abdul Moiz 23K-0898    "},
			{"    Raahim Irfan 23K-0837   "},
			{"                            "}
		};
		for(int i = 20 ; i >= 5 ; i--){
			printlines(credits, 6, 40, i, 1, 500);
		}
    }
}
void printlines(char msg[][60], int lines, int x, int y, int times, int speed){
	for(int i = 0 ; i < times ; i++){
		int pos_x = x, pos_y = y;
		for(int r = 0 ; r < lines ; r++){
			pos_y = y + r;
			gotoline(pos_x, pos_y);
			printf(" %s ", msg[r]);
		}
		Sleep(speed);
		x++;
	}
}
void print(const char *msg, int x, int y, int times, int pause, int charSpeed){
	for(int i = 0 ; i < times ; i++){
		gotoline(x, y);
		for(int i = 0 ; msg[i] != '\0' ; i++){
			printf("%c", msg[i]);
			Sleep(charSpeed);
		}
		Sleep(pause);
		x++;
	}
}
void showHangman(int x, int y, int number){
	char hangman[6][11][60] = {
		{
			"       +----------+              ",
			"       |                         ",
			"       |                         ",
			"       |                         ",
			"       |                         ",
			"       |                         ",
			"       |                         ",
			"       |                         ",
			"  +----+-----------------------+ ",
			" /                            /  ",
			"+----------------------------+   "
		},
		{
			"       +----------+              ",
			"       |          |              ",
			"       |         (.)             ",
			"       |          |              ",
			"       |                         ",
			"       |                         ",
			"       |                         ",
			"       |                         ",
			"  +----+-----------------------+ ",
			" /                            /  ",
			"+----------------------------+   "
		},
		{
			"       +----------+              ",
			"       |          |              ",
			"       |         (.)             ",
			"       |          |\\            ",
			"       |            |            ",
			"       |                         ",
			"       |                         ",
			"       |                         ",
			"  +----+-----------------------+ ",
			" /                            /  ",
			"+----------------------------+   "
		},
		{
			"       +----------+              ",
			"       |          |              ",
			"       |         (.)             ",
			"       |         /|\\            ",
			"       |        | | |            ",
			"       |                         ",
			"       |                         ",
			"       |                         ",
			"  +----+-----------------------+ ",
			" /                            /  ",
			"+----------------------------+   "
		},
		{  "       +----------+              ",
		   "       |          |              ",
		   "       |         (.)             ",
		   "       |         /|\\            ",
		   "       |        | | |            ",
		   "       |           \\            ",
		   "       |            \\           ",
		   "       |            '            ",
		   "  +----+-----------------------+ ",
		   " /                            /  ",
		   "+----------------------------+   "
		},
		{
			"       +----------+              ",
			"       |          |              ",
			"       |         (.)             ",
			"       |         /|\\            ",
			"       |        | | |            ",
			"       |         / \\            ",
			"       |        /   \\           ",
			"       |        '   '            ",
			"  +----+-----------------------+ ",
			" /                            /  ",
			"+----------------------------+   "
		}
	};
	printlines(hangman[number], 11, x, y, 1, 0);
}