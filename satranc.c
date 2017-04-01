/* --------------------------------------------------------------- */
/*                                                                 */ 
/* HW08_141044067_Gokce_Demir                                      */ 
/* hw8.c                                                           */
/*  21/04/2016 tarihinde olusturuldu.                              */ 
/* Acıklama :                                                      */ 
/* İki kullanicinin karsilikli oynadigi  satranc oyunu.            */
/*                                                                 */                                 
/* --------------------------------------------------------------- */


#include <stdio.h>
#define BOARD_SIZE 8

/*fonksiyon prototipleri */
void getPosition(char *col, int *row); 
int isValidCell(char col, int row);
int isKnightMovable(char *board, char sourceCol,
                    int sourceRow, char targetCol, int targetRow);
int isRookMovable(char *board , char sourceCol,
                     int sourceRow, char targetCol, int targetRow);
int isPawnMovable(char *board , char sourceCol,
                     int sourceRow, char targetCol, int targetRow);
int isBishopMovable(char *board , char sourceCol,
                    int sourceRow, char targetCol, int targetRow);
int isQueenMovable(char *board , char sourceCol,
                   int sourceRow, char targetCol, int targetRow);
int isKingMovable(char *board, char sourceCol, int sourceRow, 
                  char targetCol, int targetRow);                   
void printBoard(char *board);
int isPieceMovable(char *board, char sc, int sr, char tc, int tr);
int isInCheck(char* board);
int makeMove(char *board, char sc, int sr, char tc, int tr);
char getPlayer(char* board,char sc,int sr);
int isBlack(char currPlayer);
int isWhite(char currPlayer);

/* satrancin elemanlarini diziye atamaya saglayan fonksiyon */
void initBoard(char *board)
{
    int i=0;  
    char printBoard[]={'r','n','b','q','k','b','n','r',
                        'p','p','p','p','p','p','p','p',
                        ' ',' ',' ',' ',' ',' ',' ',' ',
                        ' ',' ',' ',' ',' ',' ',' ',' ',
                        ' ',' ',' ',' ',' ',' ',' ',' ',
                        ' ',' ',' ',' ',' ',' ',' ',' ',
                        'P','P','P','P','P','P','P','P',
                        'R','N','B','Q','K','B','N','R'};
    
    /* dizinin elemanlarini tek tek basma */                             
    for( i=0; i < 64 ; ++i )
    {
        board[i] = printBoard[i];   
    }     
}                                                                         
void emptyBuffer()
{
	while ( getchar() != '\n' );
}

int main() {
    char board [BOARD_SIZE*BOARD_SIZE], empty;
    int player = 1; /* 1 white, 0 black */
    char sc,tc; /* source/target cols */
    int sr,tr; /* source/target rows */
    int moveStatus = 0;
    int checkStatus = 0;
    char currPlayer;

    initBoard(board);

    do {
        printBoard(board);
        printf("%s player move > ", player ? "White" : "Black");
        getPosition(&sc,&sr);
        scanf("%c", &empty);
        getPosition(&tc,&tr);
        scanf("%c", &empty);
        currPlayer = getPlayer(board, sc,sr);

        if(!isValidCell(sc,sr)) {
            printf("Source position is invalid\n");
            continue;
        }
        if(!isValidCell(tc,tr)) {
            printf("Target position is invalid\n");
            continue;
        }
        if((isBlack(currPlayer) && player) ||
           (isWhite(currPlayer) && !player)) {
            printf("Illegal piece. \n");
            continue;
        }

        moveStatus = makeMove(board,sc,sr,tc,tr);
        switch(moveStatus) {
            case 0:
                printf("Invalid move!\n");
                break;
            case 1:
                printf("Your king is in check!\n");
                ++checkStatus;
                break;
            case 3:
                printf("Check!\n ");
            case 2:
                player = !player;
                checkStatus = 0;
                break;
        }
    } while(checkStatus < 2);
    printf("%s player WINS!\n", player ? "Black" : "White");
    return 0;
}
/*Parametre olarak gelen sourceCol ve sourceRow gore tasin bulundugu indeksi
 bulma ve tahtadaki bu indeksin uzerinde bulunan tasi tespit etme 
 Return Degeri : indekste bulunan tasi return eder. */
char getPlayer(char* board,char sc,int sr)
{
    int sourceIndex;
    sourceIndex=((int)sc-'a')+((8-sr)*8); 
    
    return board[sourceIndex];
}
/*Tasin siyah olup olmadigini test eden fonksiyon.
   Return Degeri: integer bir deger return eder, bu deger tasin siyah olma 
        durumuna gore degisir */
int isBlack(char currPlayer)
{  
    if(currPlayer>='a' && currPlayer<='z')
        return 1;
    else
        return 0;
}
/*Tasin beyaz olup olmadigini test eden fonksiyon.
   Return Degeri: integer bir deger return eder, bu deger tasin beyaz olma 
        durumuna gore degisir */
int isWhite(char currPlayer)
{    
    if(currPlayer>='A' && currPlayer<='Z')
        return 1;
    else
        return 0;
}
/* Kralin sah olup olmama durumunu test eden fonksiyon. 
    Return degeri olarak eger beyaz tas sah durumunda ise return 1 ,siyah tas
    sah durumunda ise return 2, ve eger sah durumu yok ise her ikisi icinde
    return 0 dondurur.       */
int isInCheck(char* board)
{
    int i,j,targetIndex=0,sourceIndex=0;
    int tr,sr;
    char tc,sc;
    
    for(i=0;i<64;i++)
    {   /*tahta uzerinden beyaz sahin(king) yerini bulma */
        if(board[i]=='K')
        {
            /*sekiz--> sutun ve satir sayisindan gelir */
            targetIndex=i;
            tc=(targetIndex%8)+'a'; /*indeksten sahin sutununu bulma */
            tr=8-(targetIndex/8);      /*indeksten satir sayisini bulma */
            
            for(j=0;j<64;j++)
            {   
                sourceIndex=j;
                sc=(sourceIndex%8)+'a';
                sr=8-(sourceIndex/8);
                
                if(board[j]=='r')
                {
                    if((isRookMovable(board,sc,sr,tc,tr))==1)
                        return 1;
                    else
                        return 0;
                }
                else if(board[j]=='n')
                {
                    if((isKnightMovable(board,sc,sr,tc,tr))==1)                     
                         return 1;
                    else
                        return 0;
                }
                else if(board[j]=='b')
                {
                    if((isBishopMovable(board,sc,sr,tc,tr))==1)
                        return 1;
                    else
                        return 0;
                }
                else if(board[j]=='q')
                {
                    if((isQueenMovable(board,sc,sr,tc,tr))==1)
                        return 1;
                    else
                        return 0;
                }
                else if(board[j]=='p')
                {  
                    if((isPawnMovable(board,sc,sr,tc,tr))==1)
                        return 1;
                    else
                        return 0;
                }                             
           }
          
       }
       /*tahta uzerinden beyaz sahin(king) yerini bulma */
       else if(board[i]=='k')
       {
            /*sekiz--> sutun ve satir sayisindan gelir */
            targetIndex=i;
            tc=(targetIndex%8)+'a'; /*indeksten sutunu bulma */
            tr=8-(targetIndex/8);      /*indeksten satir sayisini bulma */
            
            for(j=0;j<64;j++)
            {   
                sourceIndex=j;
                sc=(sourceIndex%8)+'a';
                sr=8-(sourceIndex/8);
                
                if(board[j]=='R')
                {
                    if((isRookMovable(board,sc,sr,tc,tr))==1)
                        return 2;
                    else
                        return 0;
                }
                else if(board[j]=='N')
                {
                    if((isKnightMovable(board,sc,sr,tc,tr))==1)                   
                        return 2;
                    else
                        return 0;
                }
                else if(board[j]=='B')
                {
                    if((isBishopMovable(board,sc,sr,tc,tr))==1)
                         return 2;
                    else
                        return 0;
                }
                else if(board[j]=='Q')
                {
                    if((isQueenMovable(board,sc,sr,tc,tr))==1)
                        return 2;
                    else
                        return 0;
                }
                else if(board[j]=='P')
                {   
                    if((isPawnMovable(board,sc,sr,tc,tr))==1)
                        return 2;
                    else
                        return 0;
                }                              
           }               
       }
                 
   }
   return 0;
       
}
/* Eger sah durumu yok ise ve istenilen tas hedefe uygun sekilde gidebiliyorsa
   tasi istedigi hedefe goturur. Eger sah durumu soz konusu ise hareket edemez
   ve tahta ayni sekilde kalir. 
  Return Degeri: Eger istenilen tas hedefe hareket edemez ise return 0 
    dondurur, kullanicinin kendi tasi sah durumuna dusmusse return 1 dondurur,
   sah durumu yok ise hedefe hareket etme kosulunu saglıyorsa return 2 dondurur,
   rakip oyuncunun tasi sah durumunda ise hareet edilir ve return 3 dondurur.*/
int makeMove(char *board, char sc, int sr, char tc, int tr)
{
    int kingCheck,moveControl;   
    int targetIndex,sourceIndex;
    char currPlayer,temp;
    
    kingCheck=isInCheck(board); /*sah durumu yok ise 0 return  eder */
    /* tas hareket edebiliyorsa 1 degerini return eder */
    moveControl=isPieceMovable(board,sc,sr,tc,tr); 
    
    targetIndex=((int)tc-'a')+((8-tr)*8); 
    sourceIndex=((int)sc-'a')+((8-sr)*8);   
    currPlayer=board[sourceIndex];
               
    if(moveControl==0)
        return 0;
   
    else if(kingCheck==0)
    {   
        board[targetIndex]=currPlayer;
        board[sourceIndex]=' ';        
        kingCheck=isInCheck(board);
        if(kingCheck==0)
            return 2;
        else          
            return 3;
       
    }
    else if((isWhite(currPlayer)==1 && kingCheck==1) ||
                (isBlack(currPlayer)==1 && kingCheck==2))
    {   
        temp=board[targetIndex];
        board[targetIndex]=currPlayer;
        board[sourceIndex]=' ';
        kingCheck=isInCheck(board);
        
        if(kingCheck==0)
            return 2;
        else if((isWhite(currPlayer)==1 && kingCheck==1) ||
                (isBlack(currPlayer)==1 && kingCheck==2))
        {                
                board[sourceIndex]=board[targetIndex];
                board[targetIndex]=temp;
                               
                return 1;
        }
        else       
             return 3;
    } 
    else 
        return 3;           
    return 0;                           
}                  
                    
/* satranc tahtasini ekrana basmaya yarayan fonksiyon */
void printBoard(char *board)
{
    int i,j,k; /* for dongusu icin sayaclar */
    char temp[8][8];
                    
                    
     printf("  a b c d e f g h\n");
     printf("  - - - - - - - -\n");
     
	 k=0;
     for(i=0;i<8;i++)
     {
        for(j=0;j<8;j++)
        {
            temp[i][j]=board[k];           
            k++;
        }
     }
     k=0;
     for(i=8;i>0;i--)
     {
        printf("%d|",i);
        for(j=0;j<8;j++)
        {   
            if(j<7)
            {
                 printf("%c ",temp[k][j]);
            }
            else
                 printf("%c",temp[k][j]);           
        }
        ++k;
        printf("|\n");
     }
              
printf("  - - - - - - - -\n");

}

/*  sutun ve satir kontrolu yapan fonksiyon 
    fonsiyon integer deger return edecek, return edecegi deger sutun ve satir 
    sartına baglı olarak degisecek                                           */
int isValidCell(char col, int row)
{
    if(col=='a' || col=='b' || col=='c' || col=='d' || col=='e' 
            || col=='f' || col=='g' || col=='h' || row>0 || row<9)   
        return 1;
     else
        return 0;
}   
/*  satir ve sutun girdilerini alma  */       
void getPosition(char *col, int *row)
{
    scanf(" %c",col);
    scanf("%d",row);
}    
/*  Parametre olarak gelen koordinatlardan tasin indeksini ve ilerleyecegi 
    hedefdeki koordinat degerlerini integer bi degere cevirir, daha sonrasinda 
    gelen karekter degerine bagli olarak gerekli fonksiyonlari cagirir.
    return degeri: integer deger dondurur, bu deger fonsiyonlarin dondurdugu
    degere gore degisir.                                                  */    
int isPieceMovable(char *board, char sc, int sr, char tc, int tr)
{
    int index,result=0; /*board arrayindeki indeksleri bulma */  
    char piece;
        
    /* girilen komuta gore board arrayindeki indeksleri bulma */
    index=((int)sc-'a')+((8-sr)*8);
   
    piece=board[index];
        /*indekse baglı olarak switch case yapisiyla fonksiyonlar cagirma */
        switch (piece)
        {
            case 'r': 
            case 'R':
                      result=isRookMovable(board,sc,sr,tc,tr);  
                      break;
            case 'n':
            case 'N':
                        result=isKnightMovable(board,sc,sr,tc,tr);
                        break;
            case 'b':
            case 'B':
                        result=isBishopMovable(board,sc,sr,tc,tr);
                        break;
            case 'q':
            case 'Q':
                        result=isQueenMovable(board,sc,sr,tc,tr);
                        break;                       
            case 'k':
            case 'K':
                        result=isKingMovable(board,sc,sr,tc,tr);
                        break;                        
            case 'p':
            case 'P':   
                        result=isPawnMovable(board,sc,sr,tc,tr);                     
                        break; 
            default:                      
                        break;                                  
         }    
     return result; /*fonksiyonlarin dondurugu degere gore */
} 
/* piyon: capraz kontrol ve bir adım sonraki adimina gerekli kosullar 
    dogrultusunda (rakip takimin tasi veya kendi tasini test etme )
    bir adım ilerisini kontrol etme. 
    return degeri : fonskiyon integer dondurur, bu integer targetteki hedefe
    ulasıp ulasamayacaginin kontrolu olarak belirtilebilir.             */                          
int isPawnMovable(char *board , char sourceCol,int sourceRow,
                  char targetCol, int targetRow)                  
{
    char piece;
    int targetIndex,index;
    int sart=0;  /* return degerini atama */
    /*girilen koordinatlara gore target ve source indeksi bulmada
                (ascii kod cevirme)                                 */
    targetIndex=((int)targetCol-'a')+((8-targetRow)*8); 
    index=((int)sourceCol-'a')+((8-sourceRow)*8);   
    piece=board[index];
    /*      eger tas siyah ise(lowercase)                          */
    if(piece=='p')
    {   
        /*rakibin tasi varsa (siyah)                           */
        if(board[targetIndex]>='A' && board[targetIndex]<='Z')
        {   
           /*piyonun rakibi capraz yeme kosulu */
            if(board[index+9]!=' ' && index+9==targetIndex)           
                sart=1;                           
            if(board[index+7]!=' ' && index+7==targetIndex)            
                sart=1;                           
        }           
         /*onu bossa bi adim gitme kosulu */   
        if(board[index+8]==' ' && (index+8)==(targetIndex))       
            sart=1;                       
        
            return sart;
      }
       /*      eger tas beyaz ise(uppercase) yukaridaki 
                    kosullar beyaz tas icinde gecerlidir                 */  
      if(piece=='P')
      {        
            if(board[targetIndex]>='a' && board[targetIndex]<='z' 
                        && board[targetIndex]!=' ')
            {               
                if(board[index-9]!=' ' || index-9==targetIndex)               
                    sart=1;                              
                if(board[index-7]!=' ' || index-7==targetIndex)                
                    sart=1;                               
             }  
               
             if(board[index-8]==' ' && index-8==targetIndex)
                sart=1;                                      
           return sart;
       }   
     return sart;               
} 
/* at: l seklinde gidisi ve target indeksin bos veyahut dolu oldugunu kontrol
   etme.
   return degeri:fonskiyon integer deger dondurur, bu integer deger hedefe
    ulasıp ulasamayacaginin kontrolu olarak belirtilebilir.             */             
int isKnightMovable(char *board, char sourceCol,
                    int sourceRow, char targetCol, int targetRow)                    
{
    char piece;
    int targetIndex,index,sart=0;
 /*sutun ve satir konumuna gore target ve source col.'un indekisinin bulunmasi*/ 
    targetIndex=((int)targetCol-'a')+((8-targetRow)*8); 
    index=((int)sourceCol-'a')+((8-sourceRow)*8);   
    piece=board[index];
    /*siyah tas(lowercase) icin hareket kontrolu */
    if(piece=='n')
    {               
        if( (board[targetIndex]>='A' && board[targetIndex]<='Z') 
                || board[targetIndex]==' ')
        {
            if(targetIndex==index+10)
                sart=1;
            else if(targetIndex==index-10)
                sart=1;
            else if(targetIndex==index+6)
                sart=1;
            else if(targetIndex==index-6)
                sart=1;
            else if(targetIndex==index+17)
                sart=1;
            else if(targetIndex==index-17)
                sart=1;
            else if(targetIndex==index+15)
                sart=1;
            else if(targetIndex==index-15)
                sart=1;
            else
                sart=0;
        }
     }
     if(piece=='N')
     {       
        if((board[targetIndex]>='a' && board[targetIndex]<='z') 
                || board[targetIndex]==' ')
        {
            if(targetIndex==index+10)
                sart=1;
            else if(targetIndex==index-10)
                sart=1;
            else if(targetIndex==index+6)
                sart=1;
            else if(targetIndex==index-6)
                sart=1;
            else if(targetIndex==index+17)
                sart=1;
            else if(targetIndex==index-17)
                sart=1;
            else if(targetIndex==index+15)
                sart=1;
            else if(targetIndex==index-15)
                sart=1;
            else
                sart=0;
        }           
    } 
    return sart;       
}
/* fil capraz hareket eder ve istedigi sayida adim atabilir. bunu bulunurken de
    target col ve source col ve row karsılastırmalari yaptim. 
    return degeri:fonskiyon integer deger dondurur, bu integer 
    hedefe ulasıp ulasamayacaginin kontrolu olarak belirtilebilir.*/                
int isBishopMovable(char *board , char sourceCol,
                    int sourceRow, char targetCol, int targetRow)
{
    char piece;
    int i,sart=0,count=0; /*donguler icin sayaclar */
    int targetIndex,index; /*koordinatlarin indeks degiskenleri */
   
    /*ascii kodu yardımıyla char degeri int(indeks degerine ) cevirme */
    targetIndex=((int)targetCol-'a')+((8-targetRow)*8); 
    index=((int)sourceCol-'a')+((8-sourceRow)*8);   
    piece=board[index];
 
    /*siyah tas(lowercase) icin hareket kontrolu */
    if(piece>='a' && piece<='z')
    {      
        if( (board[targetIndex]>='A' && board[targetIndex]<='Z') 
                || board[targetIndex]==' ')
        {                
                if(targetCol<sourceCol && targetRow>sourceRow)
                {
                    for(i=index-9;i>targetIndex;i-=9)
                    {                   
                        if(board[i]!=' ')
                            count=1;
                    }                  
                    if(count==0 && index%9==targetIndex%9)                                        
                        sart=1;                                        
                    else
                        return sart;                                     
                }                
                else if(targetCol>sourceCol && targetRow>sourceRow)
                {                   
                    for(i=index-7;i>targetIndex;i-=7)
                    {                    
                        if(board[i]!=' ')
                            count=1;
                    }                   
                    if(count==0 && index%7==targetIndex%7)                                        
                        sart=1;                                       
                    else 
                        return sart;                 
                }
                else if(targetCol<sourceCol && targetRow<sourceRow)
                {
                    for(i=index+7;i<targetIndex;i+=7)
                    {                    
                        if(board[i]!=' ')
                            count=1;
                    }                   
                    if(count==0 && index%7==targetIndex%7)                                        
                        sart=1;                                                               
                    else
                        return sart;                
                }
                else if(targetCol>sourceCol && targetRow<sourceRow)
                {
                    for(i=index+9;i<targetIndex;i+=9)
                    {                    
                        if(board[i]!=' ')
                            count=1;
                    } 
                    if(count==0 && index%9==targetIndex%9)                                       
                        sart=1;                                       
                    else 
                        return sart;                
                }
            return sart;                                                                                       
         }
    else
        return sart;                 
    }
   /*beyaz tas(uppercase) icin hareket kontrolu */
    if(piece>='A' && piece<='Z')
    {      
        if( (board[targetIndex]>='a' && board[targetIndex]<='z') 
                || board[targetIndex]==' ')
        {                
                if(targetCol<sourceCol && targetRow>sourceRow)
                {
                    for(i=index-9;i>targetIndex;i-=9)
                    {                   
                        if(board[i]!=' ')
                            count=1;
                    }                  
                    if(count==0 && index%9==targetIndex%9)                                        
                        sart=1;                                       
                    else
                        return sart;                                   
                }
                else if(targetCol>sourceCol && targetRow>sourceRow)
                {
                    for(i=index-7;i>targetIndex;i-=7)
                    {                    
                        if(board[i]!=' ')
                            count=1;
                    }                   
                    if(count==0 && index%7==targetIndex%7)                                      
                        sart=1;                                        
                    else
                        return sart;                   
                }
                else if(targetCol<sourceCol && targetRow<sourceRow)
                {
                    for(i=index+7;i<targetIndex;i+=7)
                    {                    
                        if(board[i]!=' ')
                            count=1;
                    }                   
                    if(count==0 && index%7==targetIndex%7)                                        
                        sart=1;                                                               
                    else
                        return sart;                  
                }
                else if(targetCol>sourceCol && targetRow<sourceRow)
                {
                    for(i=index+9;i<targetIndex;i+=9)
                    {                    
                        if(board[i]!=' ')
                            count=1;
                    } 
                    if(count==0 && index%9==targetIndex%9)                                       
                        sart=1;                                      
                    else
                        return sart;                  
                }                            
                    return sart;                
                                                                                                          
         }
    else
        return sart;   
    }
    return sart;
}
/* kale düz hareket eder ve istedigi kadar adim ilerler. yukari veya asagi gitme
    durumu, ya da saga sola istegi kadar gitme durumu hesaplama ve de onunde
    kendi tasinin oldugu durumu test etme.
    return degeri:fonskiyon integer deger dondurur, bu integer deger
     hedefe ulasıp ulasamayacaginin kontrolu olarak belirtilebilir.*/  
int isRookMovable(char *board , char sourceCol,
                     int sourceRow, char targetCol, int targetRow)                    
{
    char piece;
    int i,count=0,targetIndex,index,sart=0;
    
    targetIndex=((int)targetCol-97)+((8-targetRow)*8); 
    index=((int)sourceCol-97)+((8-sourceRow)*8);   
    piece=board[index];
    /*siyah tas(lowercase) icin hareket kontrolu */
    if(piece>='a' && piece<='z')
    { 
        if( (board[targetIndex]>='A' && board[targetIndex]<='Z') 
                    || board[targetIndex]==' ')   
        {
            if(targetCol==sourceCol)
            {            
                if(index<targetIndex)
                {
                    for(i=index+8;i<targetIndex;i+=8)
                    {
                        if(board[i]!=' ')
                            count=1;
                    }
                    if(count==0)                                        
                            sart=1;                                       
                    else
                            return sart;
                }
                else if(index>targetIndex){
                    for(i=index-8;i>targetIndex;i-=8)
                    {
                        if(board[i]!=' ')
                            count=1;
                    }
                    if(count==0 )                                      
                            sart=1;                                      
                    else
                            return sart;                        
                }            
            }
            else if(targetRow==sourceRow)
            {
                if(index<targetIndex)
                {
                    for(i=index+1;i<targetIndex;i+=1)
                    {
                        if(board[i]!=' ')
                            count=1;
                    }
                    if(count==0)                                      
                            sart=1;                                        
                    else
                        return sart;
                }
                else if(index>targetIndex)
                {
                    for(i=index-1;i>targetIndex;i-=1)
                    {
                        if(board[i]!=' ')
                            count=1;
                    }
                    if(count==0)                                       
                            sart=1;                                        
                    else
                         return sart;
                }
            }
            else
                return sart;
      }
    }
    if(piece>='A' && piece<='Z')
    { 
        if( (board[targetIndex]>='a' && board[targetIndex]<='z') 
                    || board[targetIndex]==' ')   
        {          
            if(targetCol==sourceCol)
            {
            
                if(index<targetIndex)
                {
                    for(i=index+8;i<targetIndex;i+=8)
                    {
                        if(board[i]!=' ')
                            count=1;
                    }
                    if(count==0)                                        
                            sart=1;                                        
                    else
                            return sart;
                }
                else if(index>targetIndex){
                    for(i=index-8;i>targetIndex;i-=8)
                    {
                        if(board[i]!=' ')
                            count=1;
                    }
                    if(count==0 )                                       
                            sart=1;                                       
                    else
                            return sart;
                        
                }            
            }
            else if(targetRow==sourceRow)
            {
                if(index<targetIndex){
                    for(i=index+1;i<targetIndex;i+=1)
                    {
                        if(board[i]!=' ')
                            count=1;
                    }
                    if(count==0)                                        
                            sart=1;                                        
                    else
                        return sart;
                    }
                else if(index>targetIndex){
                    for(i=index-1;i>targetIndex;i-=1)
                    {
                        if(board[i]!=' ')
                            count=1;
                    }
                    if(count==0)                                     
                            sart=1;                                       
                    else
                         return sart;
                }
            }
            else
                return sart;
      }
    }
    return sart;
                  
}  
/* sah bir adim ileri veya geri, veyahut carpraz hareket edebilme ozelligine
    sahip, bu fonksiyonda girilen target indeksine gore hedefe ulasıp ulasmama
    kosulunu test eder.8 adim ileri veya geri, 9 ve 7 adim ise carpraza gitme
    durumuna gore degisir.
    return degeri: fonksiyon integer deger dondurur, bu integer deger
    hedefe ulasıp ulasamayacaginin kontrolu olarak belirtilebilir.*/                                               
int isKingMovable(char *board, char sourceCol, int sourceRow, 
                  char targetCol, int targetRow)
{
    char piece;
    int targetIndex,index,sart=0;
    
    targetIndex=((int)targetCol-'a')+((8-targetRow)*8); 
    index=((int)sourceCol-'a')+((8-sourceRow)*8);   
    piece=board[index];

    if(piece=='k')
    {        
        if( (board[targetIndex]>='A' && board[targetIndex]<='Z')
                 || board[targetIndex==' '])
        {             
                if(index+9==targetIndex)                
                    sart=1;                
                else if(index-9==targetIndex)                
                    sart=1;                  
                else if(index-7==targetIndex)               
                    sart=1;                 
                else if(index+7==targetIndex)                
                    sart=1;                
                else if(index-8==targetIndex)                
                    sart=1;                
                else if(index+8==targetIndex)                
                    sart=1;                  
                else if(index+1==targetIndex)                
                    sart=1;                
                else if(index+1==targetIndex)                
                    sart=1;                                   
       }
       else
           sart=0;     
    }    
    else if(piece=='K')
    {        
        if( (board[targetIndex]>='a' && board[targetIndex]<='z')
             || board[targetIndex]==' ')
        {           
                if(index+9==targetIndex)                
                    sart=1;                
                else if(index-9==targetIndex)               
                    sart=1;                  
                else if(index-7==targetIndex)                
                    sart=1;                  
                else if(index+7==targetIndex)               
                    sart=1;                
                else if(index-8==targetIndex)               
                    sart=1;                
                else if(index+8==targetIndex)               
                    sart=1;                  
                else if(index-1==targetIndex)                
                    sart=1;               
                else if(index+1==targetIndex)               
                    sart=1;                                    
        }        
        else
            sart=0;     
    }
    return sart;
}
/* Vezir, kale ve fil fonksiyonunun sahip oldugu tum hareketleri yapabilme 
   ozelligine sahiptir.
   Return Degeri: Fonksiyon integer bir deger dondurur. Bu deger hedefe ulasip
   ulasamayacagina bagli olarak degisir.                  */
int isQueenMovable(char *board , char sourceCol,
                   int sourceRow, char targetCol, int targetRow)                   
{
    if(isRookMovable(board,sourceCol,sourceRow,targetCol,targetRow) ||
       isBishopMovable(board,sourceCol,sourceRow,targetCol,targetRow))
        return 1;
    else
        return 0;  
}
                    
/********************************************************************/
/*                        end of the hw8.c                          */
/********************************************************************/
          
            
            
    
