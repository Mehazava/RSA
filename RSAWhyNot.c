#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// 2^(This*32)//~ > 9 * This = len in dec
#define XINT_MAXLENGTH 100
#define UDINT_MAX 4294967295ull
#define ROUNDS_MR 69
#define BYTESXFOUR 7

typedef unsigned long long UQInt;
typedef long long QInt;
typedef unsigned long int UDInt;
typedef struct longstuff{
    UDInt *A;
    int L, N;//  length,  isNegative
}XInt;

FILE *FInp, *FOut, *FEnc;
UDInt BitV[32], NumAlloc = 0;

void XInit(XInt **To){
    if(To == NULL){printf("Null given");}
    *To = NULL;
    (*To) = malloc(sizeof(XInt));
    if((*To) == NULL){printf("nospace 1\n");}
    (*To)->L = XINT_MAXLENGTH - 1;//leftmost used
    (*To)->N = 0;
    (*To)->A = NULL;
    (*To)->A = malloc(XINT_MAXLENGTH * sizeof(UDInt));
    if(((*To)->A) == NULL){printf("nospace 2\n");}
    (*To)->A[(*To)->L] = 0;
}

void XSTDScan(XInt *X){//'0' => end read/ reads <--- way
    X->A[XINT_MAXLENGTH - 1] = 0;
    UDInt ReadN;
    int len;
    scanf("%lu", &ReadN);
    for(len = XINT_MAXLENGTH - 1; ReadN > 0; len--){
        printf("_wow_");
        X->A[len] = ReadN;
        scanf("%lu", &ReadN);
    }
    X->L = (len == XINT_MAXLENGTH - 1) ? (XINT_MAXLENGTH - 1) : (len + 1);
    return;
}

void XFileScan(FILE *F, XInt *X){
    UDInt ReadN;
    int len;
    fscanf(F, "%i ", &(X->L));
    for(len = X->L; len < XINT_MAXLENGTH; len++){
        fscanf(F, "%lu ", X->A + len);
    }
    return;
}

void XFilePrint(FILE *F, XInt *X){
    UDInt PrintN;
    int len;
    fprintf(F, "%i ", X->L);
    for(len = X->L; len  < XINT_MAXLENGTH; len++){
        fprintf(F, "%lu ", X->A[len]);
    }
    return;
}

void XFilePrintBit(FILE *F, XInt *X){
    UDInt PrintN;
    int len, bitN;
    fprintf(F, "%i ", X->L);
    for(len = X->L; len  < XINT_MAXLENGTH; len++){
        for(bitN = 31; bitN >= 0; bitN--){
            fprintf(F, "%i", (X->A[len] & BitV[bitN]) > 0);
        }
    }
    return;
}

void XSTDPrint(XInt *X){
    int len;
    for(len = X->L; len < XINT_MAXLENGTH; len++){
        printf("%lu_", X->A[len]);
    }
    printf("\n");
    if(X->L == XINT_MAXLENGTH - 1){
        printf("(%lu) ", X->A[XINT_MAXLENGTH - 1]);
        return;
    }
    if(X->L == XINT_MAXLENGTH - 2){
        printf("[%llu] ", (UQInt)X->A[XINT_MAXLENGTH - 2]*(UDINT_MAX + 1) + X->A[XINT_MAXLENGTH - 1]);
        return;
    }
    return;
}

void XFree(XInt **X){
    if(*X){
        NumAlloc--;
        free((*X)->A);
        free(*X);
        *X = NULL;
    }else{
        printf("Already empty\n");
        getchar();
    }
}

int XIsZero(XInt *X){//+Free x
    int Res = 0;
    if((X->L == XINT_MAXLENGTH - 1)&&(X->A[XINT_MAXLENGTH - 1] == 0)){Res = 1;}
    XFree(&X);
    return Res;
}

int XIsZeroNoDel(XInt *X){//+Free x
    int Res = 0;
    if((X->L == XINT_MAXLENGTH - 1)&&(X->A[XINT_MAXLENGTH - 1] == 0)){Res = 1;}
    return Res;
}

int XAbsComp(XInt *A, XInt *B){//done
    if(A->L == B->L){
        for(int li = A->L; li < XINT_MAXLENGTH; li++){
            if(A->A[li] < B->A[li]){
                return 1;//B is bigger
            }else if(A->A[li] > B->A[li]){
                return -1;//A is bigger
            }
        }
        return 0;//eq
    }
    if(A->L > B->L){
        return 1;
    }else{
        return -1;
    }
}

XInt * XAbsSub(XInt *Big, XInt *Smol){
    XInt *New;
    QInt Cur = 0;
    int LA = Smol->L, LB = Big->L, LI, Last = XINT_MAXLENGTH - 1;
    XInit(&New);
    for(LI = XINT_MAXLENGTH - 1; LI >= LA; LI--){
        Cur += Big->A[LI];
        Cur -= Smol->A[LI];
        if(Cur < 0){
            New->A[LI] = Cur + UDINT_MAX + 1;
            Cur = -1;
        }else{
            New->A[LI] = Cur;
            Cur = 0;
        }
        if(New->A[LI]){Last = LI;}
    }
    for(; LI >= LB; LI--){
        Cur += Big->A[LI];
        if(Cur < 0){
            New->A[LI] = Cur + UDINT_MAX + 1;
            Cur = -1;
        }else{
            New->A[LI] = Cur;
            Cur = 0;
        }
        if(New->A[LI]){Last = LI;}
    }
    if(New->A[LB] == 0){New->L = Last;}else{New->L = LB;}
    return New;
}

void XVoidAbsSub(XInt *Big, XInt *Smol){//done
    QInt Cur = 0;
    int LA = Smol->L, LB = Big->L, LI, Last = XINT_MAXLENGTH - 1;
    for(LI = XINT_MAXLENGTH - 1; LI >= LA; LI--){
        Cur += Big->A[LI];
        Cur -= Smol->A[LI];
        if(Cur < 0){
            Big->A[LI] = Cur + UDINT_MAX + 1;
            Cur = -1;
        }else{
            Big->A[LI] = Cur;
            Cur = 0;
        }
        if(Big->A[LI]){Last = LI;}
    }
    for(; LI >= LB; LI--){
        Cur += Big->A[LI];
        if(Cur < 0){
            Big->A[LI] = Cur + UDINT_MAX + 1;
            Cur = -1;
        }else{
            Big->A[LI] = Cur;
            Cur = 0;
        }
        if(Big->A[LI]){Last = LI;}
    }
    if(Big->A[LB] == 0){Big->L = Last;}
}

void XSVoidAbsSub(XInt *Big, UDInt Smol){//Big >= Smol guaranteed
    QInt Cur = 0 - Smol;
    int LB = Big->L, LI, Last = XINT_MAXLENGTH - 1;
    for(LI = XINT_MAXLENGTH - 1; Cur != 0; LI--){
        Cur += Big->A[LI];
        if(Cur < 0){
            Big->A[LI] = Cur + UDINT_MAX + 1;
            Cur = -1;
        }else{
            Big->A[LI] = Cur;
            Cur = 0;
        }
        if(Big->A[LI]){Last = LI;}
    }
    if(Big->A[LB] == 0){Big->L = Last;}
}

XInt * XAbsAdd(XInt *Big, XInt *Smol){
    XInt *New;
    UQInt Cur = 0;
    int LA = Smol->L, LB = Big->L, LI;
    XInit(&New);
    for(LI = XINT_MAXLENGTH - 1; LI >= LA; LI--){
        Cur += (UQInt)Big->A[LI] + Smol->A[LI];
        New->A[LI] = Cur & UDINT_MAX;
        Cur >>= 32;
    }
    for(; LI >= LB; LI--){
        Cur += Big->A[LI];
        New->A[LI] = Cur & UDINT_MAX;
        Cur >>= 32;
    }
    if(Cur){
        New->A[LI] = Cur;
        New->L = LI;
    }else{
        New->L = LI + 1;
    }
    return New;
}

void XVoidAbsAdd(XInt *Big, XInt *Smol){
    UQInt Cur = 0;
    int LI;
    if(Smol->L < Big->L){
        for(LI = Big->L - 1; LI >= Smol->L; LI--){
            Big->A[LI] = 0;
        }
        Big->L = Smol->L;
    }
    for(LI = XINT_MAXLENGTH - 1; LI >= Smol->L; LI--){
        Cur += (UQInt)Big->A[LI] + Smol->A[LI];
        Big->A[LI] = Cur & UDINT_MAX;
        Cur >>= 32;
    }
    for(; Cur; LI--){
        Cur += Big->A[LI];
        Big->A[LI] = Cur & UDINT_MAX;
        Cur >>= 32;
    }
    if(LI < Big->L - 1){
        Big->L--;
    }
}

XInt * XAdd(XInt *A, XInt *B){
    XInt *Res;
    if(1 == XAbsComp(A, B)){
        Res = A;
        A = B;
        B = Res;
    }//A abs is bigger or eq now
    if(A->N){
        if(B->N){
            Res = XAbsAdd(A, B);
        }else{
            Res = XAbsSub(A, B);
        }
        Res->N = 1;
    }else{
        if(B->N){
            Res = XAbsSub(A, B);
        }else{
            Res = XAbsAdd(A, B);
        }
    }
    return Res;
}

XInt * XSub(XInt *A, XInt *B){
    B->N++;
    B->N %= 2;
    XInt *Res = XAdd(A, B);
    B->N++;
    B->N %= 2;
    return Res;
}

void XCopy(XInt *To, XInt *From){
    To->L = From->L;
    To->N = From->N;
    for(int i = XINT_MAXLENGTH - 1; i >= From->L; i--){
        To->A[i] = From->A[i];
    }
}

XInt * XSMult(XInt *A, UDInt B){
    XInt *Res;
    XInit(&Res);
    UQInt Mult = 0;
    int i;
    //printf("Mult start");
    for(i = XINT_MAXLENGTH - 1; i >= A->L; i--){
        Mult += (UQInt)A->A[i] * B;
        Res->A[i] = Mult & UDINT_MAX;
        Mult >>= 32;
    }
    Res->L = A->L;
    if(Mult){
        Res->A[i] = Mult;
        Res->L = i;
    }
    return Res;
}

void XSVoidMult(XInt *A, UDInt B){
    UQInt Mult = 0;
    int i;
    for(i = XINT_MAXLENGTH - 1; i <= A->L; i--){
        Mult += A->A[i] * B;
        A->A[i] = Mult & UDINT_MAX;
        Mult >>= 32;
    }
    if(Mult){
        A->A[i] = Mult;
        A->L = i;
    }
}

XInt * XMult(XInt *A, XInt *B){
    XInt *Res = NULL, *Add = NULL;
    XInit(&Res);
    if(XIsZeroNoDel(A)||XIsZeroNoDel(B)){return Res;}

    if(XAbsComp(A, B) == 1){
        Add = A;
        A = B;
        B = Add;
        Add = NULL;
    }

    int AL = A->L, BL = B->L, i, j, k;
    for(i = XINT_MAXLENGTH - 1; i >= B->L; i--){
        Add = XSMult(A, B->A[i]);
        j = XINT_MAXLENGTH - 1 - i;//shl apropriately
        for(k = Add->L; k < XINT_MAXLENGTH; k++){
            Add->A[k - j] = Add->A[k];
        }
        for(k = XINT_MAXLENGTH - j; k < XINT_MAXLENGTH; k++){
            Add->A[k] = 0;
        }
        Add->L = Add->L - j;
        XVoidAbsAdd(Res, Add);
        XFree(&Add);
    }
    if((A->N + B->N) % 2){Res->N = 1;}
    return Res;
}

void XShR(XInt *X){//done
    for(int i = XINT_MAXLENGTH - 1; i > X->L; i--){
        X->A[i] = ((X->A[i - 1] % 2) << 31) | (X->A[i] >> 1);
    }
    X->A[X->L] >>= 1;
    if(X->A[X->L] == 0){X->L++;}
}

XInt * XRem(XInt *X, XInt *By){//done
    XInt *Rem = NULL, *Sub = NULL, *T = NULL;
    XInit(&Rem);
    XCopy(Rem, X);
    if(0){
        T = XAdd(Rem, By);
        XFree(&Rem);
        Rem = T;
    }

    if(XAbsComp(Rem, By) == 1){
        return Rem;
    }//by is bigger
    XInit(&Sub);
    int LDif = By->L - X->L + 1;
    int Shifted = LDif * 32;
    for(int i = XINT_MAXLENGTH - LDif; i < XINT_MAXLENGTH; i++){//sub is << by until its bigger than X
        Sub->A[i] = 0;
    }
    for(int i = XINT_MAXLENGTH - 1; i >= By->L; i--){
        Sub->A[i - LDif] = By->A[i];
    }
    Sub->L = By->L - LDif;
    Sub->N = 0;
    while(Shifted >= 0){//Sub is shifted >> and substracted from X when res is positive
        if(XAbsComp(Rem, Sub) == 1){
            XShR(Sub);
            Shifted--;
        }else{
            XVoidAbsSub(Rem, Sub);
        }
    }
    XFree(&Sub);
    return Rem;
}

XInt * XSRem(XInt *X, int By){//done
    XInt *Shorty = NULL, *Ans = NULL;
    XInit(&Shorty);
    Shorty->A[XINT_MAXLENGTH - 1] = By;
    Ans = XRem(X, Shorty);
    XFree(&Shorty);
    return Ans;
}

XInt * XDiv(XInt *X, XInt *By){
    XInt *Rem = NULL, *Sub = NULL, *T = NULL, *Res = NULL;
    XInit(&Res);
    if(XAbsComp(X, By) == 1){
        return Res;
    }//by is bigger
    XInit(&Rem);
    XCopy(Rem, X);
    if(0){
        XInt *T = XAdd(Rem, By);
        XFree(&Rem);
        Rem = T;
    }
    XInit(&Sub);
    int LDif = By->L - X->L + 1;
    int Shifted = LDif * 32;
    for(int i = XINT_MAXLENGTH - LDif; i < XINT_MAXLENGTH; i++){//sub is << by until its bigger than X
        Sub->A[i] = 0;
    }
    for(int i = XINT_MAXLENGTH - 1; i >= By->L; i--){
        Sub->A[i - LDif] = By->A[i];
    }
    Sub->L = By->L - LDif;
    Sub->N = 0;

    while(Shifted >= 0){//Sub is shifted >> and substracted from X when res is positive
        if(XAbsComp(Rem, Sub) == 1){
            XShR(Sub);
            Shifted--;
        }else{
            XVoidAbsSub(Rem, Sub);
            if((Shifted > 32)&&(Res->L == XINT_MAXLENGTH - 1)){
                Res->L = XINT_MAXLENGTH - 1 - (Shifted / 32);
            }
            Res->A[XINT_MAXLENGTH - 1 - (Shifted / 32)] |= BitV[Shifted % 32];
        }
    }
    XFree(&Rem);
    XFree(&Sub);
    return Res;

}

XInt * XPowMod(XInt *X, XInt *Pow, XInt *Mod){//should be ok
    XInt *CurMult, *Temp, *Res;
    XInit(&Res);
    XInit(&CurMult);
    XCopy(CurMult, X);
    Res->A[XINT_MAXLENGTH - 1] = 1;
    int CurD, CurL;
    for(CurL = XINT_MAXLENGTH - 1; CurL >= Pow->L; CurL--){
        for(CurD = 0; CurD < 32; CurD++){
            if(Pow->A[CurL] & BitV[CurD]){//if bit is 1 then mult by num for current digit
                Temp = XMult(Res, CurMult);
                XFree(&Res);
                Res = XRem(Temp, Mod);
                XFree(&Temp);
            }
            Temp = XMult(CurMult, CurMult);//num^2 for next digit
            XFree(&CurMult);
            CurMult = XRem(Temp, Mod);
            XFree(&Temp);
        }
    }
    XFree(CurMult);
    return Res;

}

void XRand(XInt *To, QInt Len){//Len <=> Len * 4 + <=4 bytes used for num(first 4B != 0)//for 512 bit len = 15//done//works
    UDInt *CurUD;
    int i;
    To->L = XINT_MAXLENGTH - 1 - Len;
    To->N = 0;
    for(i = 0, CurUD = To->A + (XINT_MAXLENGTH - 1); i <= Len; i++, CurUD--){
        *CurUD = rand();
        *CurUD <<= 17;
        *CurUD |= rand();
        *CurUD <<= 2;
        *CurUD |= rand() % 4;
    }
    if(To->A[To->L] == 0){
        To->A[To->L] = (UDInt)rand() + 1;
    }
}

int XIsPrime(XInt *X){//X is big
    if((X->A[XINT_MAXLENGTH - 1] % 2) == 0){
        return 0;
    }
    if(XIsZero(XSRem(X, 3))){
        return 0;
    }
    if(XIsZero(XSRem(X, 5))){
        return 0;
    }
    if(XIsZero(XSRem(X, 7))){
        return 0;
    }
    if(XIsZero(XSRem(X, 11))){
        return 0;
    }
    QInt lol = XINT_MAXLENGTH - 1, TPow = 0;//X - 1 = 2^TPow * Odd
    XInt *Odd = NULL, *ReduOne = NULL, *Temp = NULL, *Test = NULL;
    int j;

    XInit(&ReduOne);//X - 1
    XCopy(ReduOne, X);
    XSVoidAbsSub(ReduOne, 1);

    XInit(&Odd);
    XCopy(Odd, ReduOne);
    while(!(Odd->A[XINT_MAXLENGTH - 1] % 2)){
        TPow++;
        XShR(Odd);
    }


    XInit(&Test);//Test is [2, X-2]
    for(int i = 0; i < ROUNDS_MR; i++){//////////////////////////////////////////////////////
        XRand(Test, 15);

        Temp = XRem(Test, ReduOne);//test < x-1
        XFree(&Test);
        Test = Temp;
        Temp = NULL;

        if((Test->L == XINT_MAXLENGTH - 1)&&(Test->A[XINT_MAXLENGTH - 1] <= 1)){//test >= 2
            Test->A[XINT_MAXLENGTH - 1] += 2;
        }

        Temp = XPowMod(Test, Odd, X);//get rand()^odd (mod X) in res
        XFree(&Test);
        Test = Temp;
        Temp = NULL;

        j = 1;
        if((XAbsComp(Test, ReduOne) == 0)||((Test->L == XINT_MAXLENGTH - 1)&&(Test->A[Test->L] == 1))){
            j = TPow + 1;//skip to next i
        }
        for(; j < TPow; j++){
            Temp = XMult(Test, Test);
            XFree(&Test);

            Test = XRem(Temp, X);
            XFree(&Temp);//Res = Res ^2 mod X

            if((XAbsComp(Test, ReduOne) == 0)){
                j = TPow + 1;//skip to next i
            }else if((Test->L == XINT_MAXLENGTH - 1)&&(Test->A[XINT_MAXLENGTH - 1] == 1)){//-1
                XFree(&Test);
                XFree(&ReduOne);
                XFree(&Odd);
                return 0;//is not prime
            }
        }
        if(j == TPow){
            XFree(&Test);
            XFree(&ReduOne);
            XFree(&Odd);
            return 0;//is not prime
        }
    }
    XFree(&Test);
    XFree(&ReduOne);
    XFree(&Odd);
    return 1;//is prime
}

void XEuc(XInt *A, XInt *B, XInt **X, XInt **Y, XInt **D){
    XInt *Q, *R, *X1, *X2, *Y1, *Y2, *NA, *NB, *T1, *T2;
    XInit(X);
    XInit(Y);
    XInit(D);

    if((B->L == XINT_MAXLENGTH - 1)&&(B->A[XINT_MAXLENGTH - 1] == 0)){
        XCopy(*D, A);
        (*X)->A[XINT_MAXLENGTH - 1] = 1;
        (*Y)->A[XINT_MAXLENGTH - 1] = 0;

        return;
    }

    XInit(&X1);
    XInit(&X2);
    XInit(&Y1);
    XInit(&Y2);
    XInit(&NA);
    XCopy(NA, A);
    XInit(&NB);
    XCopy(NB, B);

    X1->A[XINT_MAXLENGTH - 1] = 0;
    Y1->A[XINT_MAXLENGTH - 1] = 1;
    X2->A[XINT_MAXLENGTH - 1] = 1;
    Y2->A[XINT_MAXLENGTH - 1] = 0;

    while((NB->L < XINT_MAXLENGTH - 1)||(NB->A[XINT_MAXLENGTH - 1] != 0)){
        if(NB->N){
            printf("Negative");
            getchar();
        }

        Q = XDiv(NA, NB);//q = a/b
        T1 = XMult(Q, NB);
        R = XSub(NA, T1);//r = a-q*b
        if(R->N){
            printf("NegativeR");
            getchar();
        }
        XFree(&T1);

        XFree(X);
        T1 = XMult(Q, X1);
        *X = XSub(X2, T1);//*x = x2-q*x1
        XFree(&T1);

        XFree(Y);//*y = y2-q*y1
        T1 = XMult(Q, Y1);
        *Y = XSub(Y2, T1);
        XFree(&T1);

        XCopy(NA, NB);
        XCopy(NB, R);

        XCopy(X2, X1);
        XCopy(X1, *X);
        XCopy(Y2, Y1);
        XCopy(Y1, *Y);

        XFree(&R);
        XFree(&Q);
    }

    XCopy(*D, NA);
    XCopy(*X, X2);
    XCopy(*Y, Y2);

    XFree(&NA);
    XFree(&NB);
    XFree(&X1);
    XFree(&X2);
    XFree(&Y1);
    XFree(&Y2);
}

XInt * XInvMod(XInt *A, XInt *Mod){
    XInt *Res, *D, *X, *Y;
    XInit(&Res);
    XEuc(A, Mod, &X, &Y, &D);
    XFree(&Y);
    if((D->L == XINT_MAXLENGTH - 1)&&(D->A[XINT_MAXLENGTH - 1] == 1)){
        XFree(&D);
        XFree(&Res);
        return X;
    }
    XFree(&D);
    XFree(&X);
    return Res;
}

int main() {
    for(UDInt WeW = 1, i = 0; i < 32; WeW <<= 1, i++){
        BitV[i] = WeW;
    }

    FInp = fopen("input.txt", "r");
    FEnc = fopen("encrypted.txt", "w+");
    FOut = fopen("output.txt", "w+");

    srand(time(NULL));

    XInt *Q, *P, *Eul, *E, *D, *Mod;
    QInt i = 0;

    XInit(&Q);
    XInit(&P);
    XInit(&E);
    XInit(&D);

    printf("------------------------starting prime nums\n");

    long TimerD = clock();
    do{
        XRand(Q, BYTESXFOUR);
        i++;
    }while(!(XIsPrime(Q)));
    TimerD = clock() - TimerD;
    printf("----first prime found, %lli generated, %.3lf second(s) spent\n", i, ((double)(TimerD))/CLOCKS_PER_SEC);
    //getchar();

    i = 0;
    TimerD = clock();
    do{
        XRand(P, BYTESXFOUR);
        i++;
    }while(!(XIsPrime(P)));
    TimerD = clock() - TimerD;
    printf("----second prime found, %lli generated, %.3lf second(s) spent\n", i, ((double)(TimerD))/CLOCKS_PER_SEC);
    //getchar();

    XCopy(E, Q);//e = q - 1
    XCopy(D, P);//d = p - 1
    i = XINT_MAXLENGTH - 1;
    while(E->A[i] == 0){
        E->A[i--] = 4294967295;//2^32-1
    }
    E->A[i]--;
    if(E->A[i] == 0){E->L++;}
    i = XINT_MAXLENGTH - 1;
    while(D->A[i] == 0){
        D->A[i--] = 4294967295;//2^32-1
    }
    D->A[i]--;
    if(D->A[i] == 0){D->L++;}

    Eul = XMult(E, D);//eul = (q-1)(p-1)
    Mod = XMult(Q, P);//mod = (q)(p)


    XInt *N2 = NULL, *NRes = NULL;
    do{
        do{//e = short prime(33 - 64 bit)
            XRand(E, 1);
        }while(!(XIsPrime(E)));
        printf("generated exp\n");
        //getchar();

        XFree(&D);
        D = XInvMod(E, Eul);
        NRes = XMult(E, D);
        if(N2){XFree(N2);}
        N2 = XRem(NRes, Eul);
        /*
        printf("\n*mod:\n");
        XSTDPrint(N2);
        fprintf(FOut, "\n---------------------------------\nQ: ");
        XFilePrintBit(FOut, Q);
        fprintf(FOut, "\n\nP: ");
        XFilePrintBit(FOut, P);
        fprintf(FOut, "\n\nE: ");
        XFilePrintBit(FOut, E);
        fprintf(FOut, "\n\nD: ");
        XFilePrintBit(FOut, D);
        fprintf(FOut, "\n\nEul: ");
        XFilePrintBit(FOut, Eul);
        fprintf(FOut, "\n\nMod: ");
        XFilePrintBit(FOut, Mod);
        fflush(FOut);
        getchar();
        */
        XFree(&NRes);
    }while(!((N2->L == XINT_MAXLENGTH - 1)&&(N2->A[XINT_MAXLENGTH - 1] == 1)));
    XFree(&N2);

    XInt *XPrint = NULL, *XRead = NULL;

    printf("Start main part\n");
    i = 0;
    TimerD = clock();
    int C;
    while((C = fgetc(FInp)) >= 0){//done
        i++;
        XInit(&XRead);
        XRead->A[XINT_MAXLENGTH - 1] = C;
        printf("Read ch %i\n", C);
        XPrint = XPowMod(XRead, E, Mod);
        XFree(&XRead);
        XFilePrint(FEnc, XPrint);
        XRead = XPowMod(XPrint, D, Mod);
        XFree(XPrint);
        if(XRead->L == XINT_MAXLENGTH - 1){
            C = XRead->A[XINT_MAXLENGTH - 1];
            printf("Got ch %i\n", C);
            fputc(C, FOut);
        }
        XFree(&XRead);
    }
    TimerD = clock() - TimerD;
    printf("----Program done, %lli characters translated, %.3lf second(s) spent\n", i, ((double)(TimerD))/CLOCKS_PER_SEC);
    getchar();
    fclose(FInp);
    fclose(FEnc);
    fclose(FOut);
    return 0;
}
