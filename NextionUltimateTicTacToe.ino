#include <SoftwareSerial.h> //comment out to run with mkr1010. USB serial will be disabled
#include <Nextion.h>

SoftwareSerial nextion(3,2); //comment out to run with mkr1010
Nextion myNextion(nextion,9600);


const int row=3,col=3;
int BigGame[row][col];
int Game1[row][col];
int Game2[row][col];
int Game3[row][col];
int Game4[row][col];
int Game5[row][col];
int Game6[row][col];
int Game7[row][col];
int Game8[row][col];
int Game9[row][col];

char charmsg[80];
char*msg;
int field, event, page, id, type;

int turn = 1;
int (*CurrentGame)[3][3]; //declaring a pointer
String rowMove;
String columnMove;
bool GameOver = false;

void setup() {
  Serial.begin(9600);
  myNextion.init();
}

void loop() {
  String message = myNextion.listen();
  field = 0;
  message.toCharArray(charmsg,80);
  if (message != ""){
    Serial.println(charmsg);
    msg=strtok(charmsg," ");
    while(msg!=NULL){
      field++;
      if (field==1)event=atoi(msg);
      if (field==2)page=strtol(msg,NULL,16);
      if (field==3)id=strtol(msg,NULL,16);
      if (field==4)type=atoi(msg);
      msg=strtok(NULL," ");
    }
  
    Serial.println(event);
    Serial.println(page);
    Serial.println(id);
    
    if (event == 65 && type == 1){
      if((id != 10) && (id != 11)){
        if (page != 0){
          makemove(*CurrentGame,id);
          endturn();
          goBack();
        }
        else if (page == 0){
          setCurrentGame(id);
          checkGameWon(id);
        }
      }
    }
    if (GameOver){
      goBack();
      myNextion.setComponentText("b4","Game Over");
      delay(8000);
      restartGame();
    }
  }

delay(500);
}


void setCurrentGame(int id){
    if(id == 1){CurrentGame = &Game1;}
    else if (id == 2){CurrentGame = &Game2;}
    else if (id == 3){CurrentGame = &Game3;}
    else if (id == 4){CurrentGame = &Game4;}
    else if (id == 5){CurrentGame = &Game5;}
    else if (id == 6){CurrentGame = &Game6;}
    else if (id == 7){CurrentGame = &Game7;}
    else if (id == 8){CurrentGame = &Game8;}
    else if (id == 9){CurrentGame = &Game9;}
    drawGame(*CurrentGame);
}

void goBack(){
  myNextion.sendCommand("page page0");
  CurrentGame = &BigGame;
  drawGame(BigGame);
}

void makemove(int currentgame[3][3], int id){
  String symbol = "";
  if (turn == 1){
    symbol = "X";
  }
  else if (turn == 2){
    symbol = "O";
  }
  if(id == 1){ currentgame[0][0] = turn;
  myNextion.setComponentText("b0",symbol);}
  else if (id == 2){ currentgame[0][1] = turn;
  myNextion.setComponentText("b1",symbol);}
  else if (id == 3){ currentgame[0][2] = turn;
  myNextion.setComponentText("b2",symbol);}
  else if (id == 4){ currentgame[1][0] = turn;
  myNextion.setComponentText("b3",symbol);}
  else if (id == 5){ currentgame[1][1] = turn;
  myNextion.setComponentText("b4",symbol);}
  else if (id == 6){ currentgame[1][2] = turn;
  myNextion.setComponentText("b5",symbol);}
  else if (id == 7){ currentgame[2][0] = turn;
  myNextion.setComponentText("b6",symbol);}
  else if (id == 8){ currentgame[2][1] = turn;
  myNextion.setComponentText("b7",symbol);}
  else if (id == 9){ currentgame[2][2] = turn;
  myNextion.setComponentText("b8",symbol);}

  Serial.println("The current game board: " + String(page));
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      Serial.print(currentgame[i][j]);
      Serial.print("|");
    }
    Serial.println("");
  }
  delay(1000);
  
  Serial.println("move made");
  checkWinner(currentgame);
}

void endturn(){
  if (turn == 1){
  turn = 2;
  }
  else if(turn == 2){
  turn = 1;
  }
  Serial.print("Turn switched to: ");
  Serial.println(turn);
}

void drawGame(int board[][3]){
  int k = 0;
  for (int i=0; i<3; i++){
    for (int j=0; j<3; j++){
      String buttonid = "b" + String(k);
      if (board[i][j]== 1){
        myNextion.setComponentText(buttonid,"X");
      }
      else if (board[i][j]== 2){
        myNextion.setComponentText(buttonid,"O");
      }
      k++;
    }
  }
}

void checkWinner(int board[][3]){ //https://stackoverflow.com/questions/35110576/tic-tac-toe-c-check-print-winner
 int winner;
 for (int i=0; i<=2; i++){
  if(board[i][0]==board[i][1] && board[i][1]==board[i][2] && board[i][0]!=0){
      winner=board[i][0];
  }
 }

 for(int i=0; i<=2; i++){
  if (board[0][i]==board[1][i] && board[1][i]==board[2][i] && board[0][i]!=0){
      winner=board[0][i];
  }
 }

 if(board[0][0]==board[1][1] && board[1][1]==board[2][2] && board[0][0]!=0){
  winner=board[0][0];
 }

 if(board[0][2]==board[1][1] && board[1][1]==board[2][0] && board [0][2]!=0){
  winner=board[0][2];
 }

 if(board[0][0]==board[0][1] && board[0][1]==board[0][2]&& board[0][2]==board[0][1]&& board[1][0]==board [1][1]&& board[1][1]==board [1][2]&& board[1][2]==board[2][0]&&board[2][0]==board [2][1]&& board[2][1]==board [2][2] && board [0][0]!=0){
  winner=0;
 }
 if (winner == 1||winner == 2){
  if (board != BigGame){
    makemove(BigGame, page);
  }
  else if (board == BigGame){
    GameOver = true;
  }
 }
 Serial.println("Displaying: BigGame");
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      Serial.print(BigGame[i][j]);
      Serial.print("|");
    }
    Serial.println("");
  }
}

void checkGameWon(int id){
  int k = 1;
  for (int i=0; i<3; i++){
    for (int j=0; j<3; j++){
      if ((BigGame[i][j]== 1||BigGame[i][j]==2)&&(id==k)){
        goBack();
      }
      k++;
    }
  }
  delay(1000);
}
   
void restartGame(){
  for (int i=0; i<3; i++){
    for (int j=0; j<3; j++){
      BigGame[i][j]=0;
      Game1[i][j]=0;
      Game2[i][j]=0;
      Game3[i][j]=0;
      Game4[i][j]=0;
      Game5[i][j]=0;
      Game6[i][j]=0;
      Game7[i][j]=0;
      Game8[i][j]=0;
      Game9[i][j]=0;
    }
  }
  turn = 1;
  GameOver = false;
  goBack();
}
