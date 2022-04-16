#include<bits/stdc++.h>
using namespace std;
const int LICZBA_KOLOROW=4;
const int LICZBA_WARTOSCI_KART=6;
const int LICZBA_KOMBINACJI=15;
const int LICZBA_STANOW=pow(LICZBA_KOMBINACJI,LICZBA_WARTOSCI_KART);
const short REMIS=1;
const short WYGRANA=32766;
const short PRZEGRANA=-WYGRANA;
const int STOS_PUSTY=-1;
char ruch[]={'-','a','k','q','j','t','n','A','K','Q','J','T','N'};
char priorytety_ruchy[]={'n','N','t','T','j','J','q','Q','k','K','a','A','-'};
int priorytet[500];
pair<int,int>kombinacje[LICZBA_KOMBINACJI]={{0,0},{0,1},{0,2},{0,3},{0,4},{1,0},{1,1},{1,2},{1,3},{2,0},{2,1},{2,2},{3,0},{3,1},{4,0}};
struct info{
    short wynik;
    char najlepszy_ruch;
};
int przeszukane;
int nr_kombinacji[LICZBA_KOLOROW+1][LICZBA_KOLOROW+1];
vector<pair<int,char>>graf[LICZBA_STANOW];
info dp[LICZBA_STANOW];
int in[LICZBA_STANOW];
bool czy_ustalony[LICZBA_STANOW];
queue<int>kolejka;
void numery_kombinacji(){
    for(int i=0;i<LICZBA_KOMBINACJI;++i){
        nr_kombinacji[kombinacje[i].first][kombinacje[i].second]=i;
    }
}
void oblicz_priorytety(){
    for(int i=0;i<=LICZBA_WARTOSCI_KART*2;++i){
        priorytet[priorytety_ruchy[i]]=i;
    }
}
int stan_vec_to_int(const vector<int>&vec){
    int wyn=0;
    for(int i=0;i<vec.size();++i){
        wyn*=LICZBA_KOMBINACJI;
        wyn+=vec[i];
    }
    return wyn;
}
vector<int>wektor_kombinacji(const vector<pair<int,int>>& vec,bool zamiana){
    vector<int>wyn;
    for(int i=0;i<vec.size();++i){
        if(zamiana==false)wyn.push_back(nr_kombinacji[vec[i].first][vec[i].second]);
        else wyn.push_back(nr_kombinacji[vec[i].second][vec[i].first]);
    }
    return wyn;
}
vector<int>stan_int_to_vec(int stan){
    vector<int>wyn(LICZBA_WARTOSCI_KART);
    for(int i=LICZBA_WARTOSCI_KART-1;i>=0;--i){
        wyn[i]=stan%LICZBA_KOMBINACJI;
        stan/=LICZBA_KOMBINACJI;
    }
    return wyn;
}
vector<pair<int,int>>policz_ile_danych_wartosci_kart(int stan){
    vector<int> numery_kombinacji=stan_int_to_vec(stan);
    vector<pair<int,int>>wyn(LICZBA_WARTOSCI_KART);
    for(int i=0;i<LICZBA_WARTOSCI_KART;++i){
        wyn[i].first=kombinacje[numery_kombinacji[i]].first;
        wyn[i].second=kombinacje[numery_kombinacji[i]].second;
    }
    return wyn;
}
vector<int>policz_ile_kart_na_stosie(const vector<pair<int,int>>& ile_danych_wartosci_kart){
    vector<int>stos(LICZBA_WARTOSCI_KART);
    for(int i=0;i<LICZBA_WARTOSCI_KART;++i){
        stos[i]=LICZBA_KOLOROW-ile_danych_wartosci_kart[i].first-ile_danych_wartosci_kart[i].second;
    }
    return stos;
}
int najw_wartosc_na_stosie(const vector<int>&stos){
    for(int i=0;i<LICZBA_WARTOSCI_KART;++i){
        if(stos[i]>0)return i;
    }
    return STOS_PUSTY;
}
bool jest_stanem_koncowym(vector<pair<int,int>>gracze){
    pair<int,int>suma;
    for(int i=0;i<gracze.size();++i){
        suma.first+=gracze[i].first;
        suma.second+=gracze[i].second;
    }
    if(suma.first==0){
        return true;
    }
    if(suma.second==0){
        if(suma.first==gracze[0].first){
            if(gracze[0].first==1||gracze[0].first==4)return false;
        }
        return true;
    }
    return false;
}
void dodaj_krawedz(int akt, char akcja,vector<pair<int,int>>gracze){
    vector<int>komb=wektor_kombinacji(gracze,true);
    int poprz=stan_vec_to_int(komb);
    if(jest_stanem_koncowym(gracze)==false){
        graf[akt].push_back({poprz,akcja});
        ++in[poprz];
    }
}
void dolozyl_karty(int ile,int szczyt,int akt_stan, vector<pair<int,int>>gracze){
    gracze[szczyt].second+=ile;
    char akcja;
    if(ile==1){
        akcja=ruch[szczyt+1];
    }
    else{
        akcja=ruch[szczyt+LICZBA_WARTOSCI_KART+1];
    }
    dodaj_krawedz(akt_stan,akcja,gracze);
}
void wzial_karty(int ile,int szczyt,int akt_stan,vector<pair<int,int>>gracze,int min_wart){
    if(ile==0){
        dodaj_krawedz(akt_stan,'-',gracze);
    }
    else{
        for(int i=0;i<=min_wart;++i){
            if(gracze[i].second>0){
                --gracze[i].second;
                wzial_karty(ile-1,szczyt,akt_stan,gracze,i);
                ++gracze[i].second;
            }
        }
    }
}
void dodaj_poprzednikow(int stan){
    vector<pair<int,int>>gracze=policz_ile_danych_wartosci_kart(stan);
    vector<int>stos=policz_ile_kart_na_stosie(gracze);
    int szczyt=najw_wartosc_na_stosie(stos);
    if(szczyt==STOS_PUSTY)return;
    int krotnosc=stos[szczyt];
    dolozyl_karty(1,szczyt,stan,gracze);
    if(krotnosc==LICZBA_KOLOROW){
        dolozyl_karty(LICZBA_KOLOROW,szczyt,stan,gracze);
        if(szczyt==LICZBA_WARTOSCI_KART-1){
            dolozyl_karty(LICZBA_KOLOROW-1,szczyt,stan,gracze);
        }
    }
    wzial_karty(3,szczyt,stan,gracze,szczyt);
    if(szczyt==LICZBA_WARTOSCI_KART-1&&krotnosc==1){
        wzial_karty(1,szczyt,stan,gracze,szczyt);
        wzial_karty(2,szczyt,stan,gracze,szczyt);
    }
}
void oznacz_stany_koncowe(int stan, vector<pair<int,int>>gracze){
    pair<int,int>suma;
    for(int i=0;i<gracze.size();++i){
        suma.first+=gracze[i].first;
        suma.second+=gracze[i].second;
    }
    if(suma.first==0){
        czy_ustalony[stan]=true;
        if(suma.second!=0){
            dp[stan].wynik=WYGRANA;
            dp[stan].najlepszy_ruch='-';
        }
        else{
            dp[stan].wynik=REMIS;
            dp[stan].najlepszy_ruch='-';
        }
    }
    else if(suma.second==0){
        czy_ustalony[stan]=true;
        if(suma.first==gracze[0].first){
            if(gracze[0].first==1||gracze[0].first==4){
                dp[stan].wynik=REMIS;
                if(gracze[0].first==1)dp[stan].najlepszy_ruch='a';
                else dp[stan].najlepszy_ruch='A';
            }
        }
        else{
            dp[stan].wynik=PRZEGRANA;
            dp[stan].najlepszy_ruch='-';
        }
    }
}
void stworz_graf(){
    for(int stan=0;stan<LICZBA_STANOW;++stan){
        dodaj_poprzednikow(stan);
        vector<pair<int,int>> v=policz_ile_danych_wartosci_kart(stan);
        oznacz_stany_koncowe(stan,v);
        if(stan%(LICZBA_STANOW/100)==0){
            system("cls");
            cout<<"Tworzenie grafu "<<stan*100/LICZBA_STANOW<<"%"<<endl;
        }
    }
}
void dodaj_stany_koncowe_do_kolejki(){
    for(int stan=0;stan<LICZBA_STANOW;++stan){
        if(in[stan]==0){
            kolejka.push(stan);
            czy_ustalony[stan]=true;
        }
    }
}
void zmniejsz_stopien_i_dodaj_na_kolejke(int poprz){
    --in[poprz];
    if(in[poprz]==0){
        kolejka.push(poprz);
        czy_ustalony[poprz]=true;
    }
}
int wynik_ruch_wczesniej(int wyn){
    if(wyn<REMIS)return -wyn-1;
    if(wyn>REMIS)return -wyn+1;
    return REMIS;
}
void zaktualizuj_wynik(int poprz,char akcja,int nowy){
    if(dp[poprz].wynik==0||nowy>dp[poprz].wynik){
        if(nowy>dp[poprz].wynik)kolejka.push(poprz);
        dp[poprz].wynik=nowy;
        dp[poprz].najlepszy_ruch=akcja;
        if(nowy>REMIS){
            czy_ustalony[poprz]=true;
        }
    }
}
void ustaw_remisy(){
    for(int stan=0;stan<LICZBA_STANOW;++stan){
        if(czy_ustalony[stan]==false){
            kolejka.push(stan);
        }
    }
    while(!kolejka.empty()){
        int akt=kolejka.front();
        kolejka.pop();
        for(int i=0;i<graf[akt].size();++i){
            int poprz=graf[akt][i].first;
            char akcja=graf[akt][i].second;
            if(czy_ustalony[poprz]==false){
                if(dp[poprz].wynik<REMIS||priorytet[akcja]<priorytet[dp[poprz].najlepszy_ruch]){
                    dp[poprz].wynik=REMIS;
                    dp[poprz].najlepszy_ruch=akcja;
                }
            }
        }
    }
}
void oblicz_dp(){
    dodaj_stany_koncowe_do_kolejki();
    while(!kolejka.empty()){
        int akt=kolejka.front();
        kolejka.pop();
        for(int i=0;i<graf[akt].size();++i){
            int poprz=graf[akt][i].first;
            char akcja=graf[akt][i].second;
            int wyn=dp[akt].wynik;
            zmniejsz_stopien_i_dodaj_na_kolejke(poprz);
            int nowy_wyn=wynik_ruch_wczesniej(wyn);
            zaktualizuj_wynik(poprz,akcja,nowy_wyn);
        }
    }
    ustaw_remisy();
}
vector<int>stos(LICZBA_WARTOSCI_KART);
vector<pair<int,int>>gracze(LICZBA_WARTOSCI_KART);



void zapytania(string s, int kto,bool czy_odczytac);
void odczytaj(){
    vector<int>komb=wektor_kombinacji(gracze,false);
    int stan=stan_vec_to_int(komb);
    int wyn=dp[stan].wynik;
    char ruch=dp[stan].najlepszy_ruch;
    cout<<"NAJLEPSZY RUCH: "<<ruch<<"\n";
    if(wyn>REMIS){
        cout<<"WYGRANA W "<<WYGRANA-wyn<<" RUCHACH\n";
    }
    else if(wyn<REMIS){
        cout<<"PRZEGRANA W "<<wyn-PRZEGRANA<<" RUCHACH\n";
    }
    else {
        cout<<"REMIS\n";
    }
    string s_ruch;
    s_ruch+=ruch;
    zapytania(s_ruch,0,false);
}
void wczytaj_gre(bool czy_odczytac){
    cout<<"Podaj liczby na stosie od A do 9\n";
    for(int i=0;i<LICZBA_WARTOSCI_KART;++i){
        cin>>stos[i];
    }
    cout<<"Podaj liczby twoich kart od A do 9\n";
    for(int i=0;i<LICZBA_WARTOSCI_KART;++i){
        cin>>gracze[i].first;
        gracze[i].second=LICZBA_KOLOROW-stos[i]-gracze[i].first;
    }
    if(czy_odczytac==true)odczytaj();
}
void wez_karty(int ile,int kto){
    if(ile>0){
        int szczyt=najw_wartosc_na_stosie(stos);
        if(szczyt==STOS_PUSTY)return;
        int krotnosc=stos[szczyt];
        if(szczyt==LICZBA_WARTOSCI_KART-1&&krotnosc==1)return;
        --stos[szczyt];
        if(kto==0)++(gracze[szczyt].first);
        else ++(gracze[szczyt].second);
        ++gracze[szczyt].second;
        wez_karty(ile-1,kto);
    }
}
void zapytania(string s,int kto,bool czy_odczytac){
    if(s=="reset"){
        wczytaj_gre(true);
        return;
    }
    if(s=="show"){
        cout<<"stos od A do 9\n";
        for(int i=0;i<LICZBA_WARTOSCI_KART;++i){
            cout<<stos[i]<<" ";
        }
        cout<<"twoje karty od A do 9\n";
        for(int i=0;i<LICZBA_WARTOSCI_KART;++i){
            cout<<gracze[i].first<<" ";
        }
        return;
    }
    for(int i=1;i<=LICZBA_WARTOSCI_KART;++i){
        if(s[0]==ruch[i]){
            if(kto==0){
                --(gracze[i-1].first);
                ++stos[i-1];
            }
            else{
                --(gracze[i-1].second);
                ++stos[i-1];
            }
        }
    }
    for(int i=LICZBA_WARTOSCI_KART+1;i<=LICZBA_WARTOSCI_KART*2;++i){
        if(s[0]==ruch[i]){
            if(kto==0){
                stos[i-LICZBA_WARTOSCI_KART-1]+=gracze[i-LICZBA_WARTOSCI_KART-1].first;
                gracze[i-LICZBA_WARTOSCI_KART-1].first=0;
            }
            else{
                stos[i-LICZBA_WARTOSCI_KART-1]+=gracze[i-LICZBA_WARTOSCI_KART-1].second;
                gracze[i-LICZBA_WARTOSCI_KART-1].second=0;
            }
        }
    }
    if(s[0]=='-'){
        wez_karty(3,kto);
    }
    if(czy_odczytac==true)odczytaj();
}

int main(){
    ios::sync_with_stdio(false);
    numery_kombinacji();
    oblicz_priorytety();
    stworz_graf();
    oblicz_dp();
    wczytaj_gre(true);
    while(true){
        string s;
        cin>>s;
        zapytania(s,1,true);
    }
    return 0;
}

