//Created by Patryk Jêdrzejewski

#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <typeinfo>
#include <fstream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
#include <functional>
#include <cstdio>
#include <set>

using namespace std;
chrono::steady_clock sc;

int E, V, a, b;
int MS[1500][1500] = { 0 };
int ile_kolorow=0;
int liczbakolorow=0;
clock_t start;
int minlicznikonfliktow=99999;
int koloryzachlanny;
int maksiteracji=100000;
double maxduration=120;

int taburozmiar; //rozmiar listy tabu
int ilesasiadow;  //rozmiar przeszukiwanego sasiedztwa


bool czyliczba(string s)
{
    for (int i = 0; i < s.length(); i++)
        if (isdigit(s[i]) == false)
            return false;

    return true;
}

int* zachlanny()
{
    static int wynik[1500]={-1};
    //dostepnekolory to bool 0 1
    int dostepnekolory[1500];

    wynik[1]=0;

    for(int i=1; i<=V; i++)
    {
        for(int tmp=1; tmp<=V; tmp++)
        {
            dostepnekolory[tmp]=0;
        }
        for(int j=1; j<=V; j++)
        {
            if(MS[i][j]==1)
            {
                if(wynik[j]>-1)
                {
                    dostepnekolory[wynik[j]]=1;
                }
            }
        }
        int w=1;
        while(dostepnekolory[w]==1)
        {
            w++;
        }
        wynik[i]=w;
    }
    ile_kolorow=0;
    for(int i=1; i<=V; i++)
    {
        if(wynik[i]>ile_kolorow)
        {
            ile_kolorow=wynik[i];
        }
    }
    cout<<"It is possible to color the graph with "<<ile_kolorow<<" colors using greedy algorithm"<<endl;
    koloryzachlanny=ile_kolorow;

    for(int i=1; i<=V; i++)
    {
        cout<<i<<": "<<wynik[i]<<"\t";
    }
    cout<<endl;
    return wynik;
}

////////////////////////////////////////////////////////////////////////
////////////////////    t a b u ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
int tabucol(int wynik[])
{
    //ranking
    //queen6 == 7
    //miles250 == 8
    //gc1000_300013 == 112 #
    //gc500 == 66 #
    //le450_5a = 8#

    //nasze wyniki          //t  //rep
    //queen6 == 7           //5 //36
    //miles250 == 8         //5 //36
    //gc1000_300013 == 149  //20 //250
    //gc500 == 76           //10 //200
    //le450_5a = 6          //6  //20
                            //8 //35

    if(liczbakolorow>=ile_kolorow)
    {
         cout<<"It is possible to color the graph with "<<ile_kolorow<<" colors"<<endl;
         for(int i=1; i<=V; i++)
         {
             cout<<i<<": "<<wynik[i]<<"\t";
         }
         return 0;
    }

    //int taburozmiar=liczbakolorow*V;
    //taburozmiar=5;//rozmiar listy tabu
    // ilesasiadow=36; //rozmiar przeszukiwanego sasiedztwa
    int debug=1;
    double duration=0;

    queue<string> tabukolejka;
    queue<string> zapasowa;

    for(int i=1; i<=V; i++)
    {
        if(wynik[i]>=liczbakolorow)
        {
            wynik[i]=liczbakolorow;
        }
    }

    int aspirationlevel[100000]={-1000};
    int iteracje=0;
    int licznikkonfliktow=0;
    start = clock();
    int minimumbledow=0;
    while(iteracje<maksiteracji && duration<=maxduration)
    {
        licznikkonfliktow=0;
        int candidates_to_move[10000]={999999};
        int ile=0;

        for(int i=1; i<=V; i++)
        {
            for(int j=i+1; j<=V; j++)
            {
                if(MS[i][j]==1)
                {
                    if(wynik[i]==wynik[j])
                    {
                        int temp=0;
                        for(int y=0; y<ile; y++)
                        {
                            if(candidates_to_move[y]!=i)
                            {
                                temp++;
                            }
                        }
                        if(temp==ile)
                        {
                            candidates_to_move[ile]=i;
                            ile=ile+1;
                        }

                        temp=0;
                        for(int y=0; y<ile; y++)
                        {
                            if(candidates_to_move[y]!=j)
                            {
                                temp++;
                            }
                        }
                        if(temp==ile)
                        {
                            candidates_to_move[ile]=j;
                            ile=ile+1;
                        }
                        licznikkonfliktow=licznikkonfliktow+1;
                    }
                }
            }
        }

        //error counter for debug purposes
        //cout<<licznikkonfliktow<<endl;
        if(licznikkonfliktow<minlicznikonfliktow)
        {
            minlicznikonfliktow=licznikkonfliktow;
        }

        if(licznikkonfliktow==0)
        {
            int ile_kolorow=0;
            for(int i=1; i<=V; i++)
            {
                if(wynik[i]>ile_kolorow)
                {
                    ile_kolorow=wynik[i];
                }
            }
            cout<<"It is possible to color the graph with "<<ile_kolorow<<" colors using tabu-search metaheuristic"<<endl;
            for(int i=1; i<=V; i++)
            {
                cout<<i<<": "<<wynik[i]<<"\t";
            }
            cout<<endl<<"Computing time: "<<duration;
            return 0;
        }
        int nowywynik[100000]={0};
        int wezel=0;
        int nowykolor=0;

        for(int x=0; x<ilesasiadow; x++)
        {
            wezel=candidates_to_move[rand()%ile];
            nowykolor=rand()%(liczbakolorow-1)+1;

            if(wynik[wezel]==nowykolor)
            {
                nowykolor=liczbakolorow;
            }

            for(int i=1; i<=V; i++)
            {
                nowywynik[i]=wynik[i];
            }

            nowywynik[wezel]=nowykolor;

            int nowekonflikty=0;

            for(int i=1; i<=V; i++)
            {
                for(int j=i+1; j<=V; j++)
                {
                    if(MS[i][j]==1 && nowywynik[i]==nowywynik[j])
                    {
                        nowekonflikty=nowekonflikty+1;
                    }
                }
            }
            int zmienna=0;
            if(nowekonflikty<licznikkonfliktow)
            {

                aspirationlevel[licznikkonfliktow]=licznikkonfliktow-1;

                if(nowekonflikty <= aspirationlevel[licznikkonfliktow])
                {

                    aspirationlevel[licznikkonfliktow]=nowekonflikty-1;


                    string ab=to_string(wezel)+"->"+to_string(nowykolor);
                    zmienna=0;

                    while (!tabukolejka.empty())
                    {

                        if(tabukolejka.front()==ab)
                        {
                            zmienna=1;
                            zapasowa.push(tabukolejka.front());
                            tabukolejka.pop();
                        }
                        else{tabukolejka.pop();}

                    }

                    while (!zapasowa.empty())
                    {

                        tabukolejka.push(zapasowa.front());
                        zapasowa.pop();

                    }

                    if(zmienna==1 && debug==1)
                    {
                        //cout<<"tabu permitted; ", licznikkonfliktow, "-> ", nowekonflikty;
                    }
                }
                else
                {
                    string ab=to_string(wezel)+"->"+to_string(nowykolor);
                    zmienna=0;

                    while (!tabukolejka.empty())
                    {
                       // cout << tabukolejka.front()<<endl;
                        if(tabukolejka.front()==ab)
                        {
                            zmienna=1;
                            zapasowa.push(tabukolejka.front());
                            tabukolejka.pop();
                        }
                        else{tabukolejka.pop();}
                    }

                    //cout<<endl<<endl;
                    while (!zapasowa.empty())
                    {

                        tabukolejka.push(zapasowa.front());
                        zapasowa.pop();

                    }
                    if(zmienna==1)
                    {
                        continue;
                    }
                }
                if(debug==1)
                {
                   // cout<<licznikkonfliktow<<" -> "<<nowekonflikty<<endl;
                }
                break;
            }
        }
        //if(minimumbledow<licznikkonfliktow || minimumbledow<no)
        string xyz=to_string(wezel)+"->"+to_string(wynik[wezel]);
        tabukolejka.push(xyz);
        if(tabukolejka.size() > taburozmiar)
        {
            tabukolejka.pop();
        }
        for(int wpr=1; wpr<=V; wpr++)
        {
            wynik[wpr]=nowywynik[wpr];
        }

        iteracje=iteracje+1;
        if(debug==1 && iteracje%10000 ==0 )
        {
            cout<<"Iterations "<<iteracje<<" out of "<<maksiteracji<<endl;
        }

        duration=(clock()-start)/(double) CLOCKS_PER_SEC;
    }
    cout<<endl<<"Computing time: "<<duration;

    //podsumowanie
    int kolory=0;
    for(int i=1; i<=V; i++)
    {
        if(wynik[i]>kolory)
        {
            kolory=wynik[i];
        }
    }

    if(licznikkonfliktow != 0)
    {
        cout<<endl<<"For "<<liczbakolorow<<" colors algorithm couldn't find a way to color a graph "<<endl;
        return 0;
    }
    else
    {
        cout<<endl<<"It is possible to color the graph with "<<kolory<<" colors using tabu-search metaheuristic"<<endl;
        for(int i=1; i<=V; i++)
        {
            cout<<i<<" "<<wynik[i]<<"\t";
        }
        return 0;
    }
}




int main(int argc, const char* argv[])
{
    int wyjscie = 0;
    int wyjscie2 = 0;

    ifstream graf;
    int wybor=0;
    cout<<"1. queen6\n2. miles250 \n3. gc_1000_300013 \n4. gc500 \n5. le450_5a \n6. own"<<endl;
    while(wybor==0 || wybor>6)
    {
            cin>>wybor;
            if(wybor==1)
            {
                graf.open("queen6.txt");
                taburozmiar=5; //rozmiar listy tabu
                ilesasiadow=36;  //rozmiar przeszukiwanego sasiedztwa
            }
            if(wybor==2)
            {
                graf.open("miles250.txt");
                taburozmiar=5; //rozmiar listy tabu
                ilesasiadow=36;  //rozmiar przeszukiwanego sasiedztwa
            }
            if(wybor==3)
            {
                graf.open("gc_1000_300013.txt");
                taburozmiar=20; //rozmiar listy tabu
                ilesasiadow=250;  //rozmiar przeszukiwanego sasiedztwa
            }
            if(wybor==4)
            {
                graf.open("gc500.txt");
                taburozmiar=10; //rozmiar listy tabu
                ilesasiadow=200;  //rozmiar przeszukiwanego sasiedztwa
            }
            if(wybor==5)
            {
                graf.open("le450_5a.txt");
                taburozmiar=8; //rozmiar listy tabu
                ilesasiadow=35;  //rozmiar przeszukiwanego sasiedztwa
            }
            if(wybor==6)
            {
                graf.open("zeroin.i.2.txt");
                taburozmiar=5; //rozmiar listy tabu
                ilesasiadow=36;  //rozmiar przeszukiwanego sasiedztwa
            }
    }

    int c = 0;
    string e;
    srand(static_cast<unsigned int>(time(nullptr)));
    int x = 0;
    string xd;

    int ilekrawedzi=0;
    string nic, nic2;


    graf >> V ;
    while (!graf.eof())
    {
        graf >> a >> b;
        ilekrawedzi++;
        MS[a][b] = 1;
        MS[b][a] = 1;
    }
    graf.close();



    /*cout << "Macierz sasiedztwa" << endl;
    for (int i = 1; i <= V; i++)
    {
        for (int j = 1; j <= V; j++)
            cout << MS[i][j] << "\t";
        cout << endl;
    }*/

    int* pokolorowanie = zachlanny();
    /*for(int i=1; i<=V; i++)
    {
        cout<<i<<" "<<pokolorowanie[i]<<endl;
    }*/

    cout<<"Type number to check if it is possible to color the graph with that amount of colors"<<endl;
    cin>>liczbakolorow;

    tabucol(pokolorowanie);
}
