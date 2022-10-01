//Programa principal per ENCRIPTAR MISSATGES


//llibreria estandard
#include <iostream>
//llibreria pels missatges
#include <map>
//llibreria per utilitzar frases
#include <string>
//llibreria per utilitzar vectors i matrius
#include <vector>
//llibreria per ordenar la reixeta
#include <algorithm>
//per utilitzar elements de c++ de manera estandard
using namespace std;


/*
////////MISSATGES\\\\\\\\
*/


//diccionari de missatges, es com un vector de missatges: el primer string es l'identificador, el segon string es el missatge
map<string,string> Mensajes;

//inserta el identificador i el missatge que li passes al diccionari
void nuevo_mensaje(const string& idm, const string& mensaje) {
    Mensajes.insert(make_pair(idm,mensaje));
}
//elimina el missatge del diccionari mitjançant el identificador
void borra_mensaje(const string& idm) {
    Mensajes.erase(idm);
}
//retorna la mida del diccionari
int consultar_tamano() {
    return Mensajes.size();
}
//retorna el missatge en si del diccionari mitjançant el identificador
string consultar_mensaje(const string& idm) {
    map<string,string>::const_iterator it = Mensajes.find(idm);
    return it->second;
}
//retorna si si existeix el missatge al diccionari o no si no existeix el missatge al diccioanri mitjançant el identificador
bool existe_mensaje(const string& idm) {
    map<string,string>::const_iterator it = Mensajes.find(idm);
    return it != Mensajes.end();
}
//escriu per pantalla tots els missatges del diccionari amb el seu identificador
void listar_mensajes() {
    for (map<string,string>::const_iterator it = Mensajes.begin(); it != Mensajes.end(); ++it) {
        cout << it->first << endl << '"' << it->second << '"' << endl;
    }
}


/*
////////REIXETES\\\\\\\\
*/


//estructura que defineix una rejilla
struct Rejilla {    
    int n;  //dimensio, files
    int k;  //longitud, columnes

    vector< pair<int,int> > Rj; //vector posicions de la rejilla
};
//vector de rejillas
vector<Rejilla> Rejillas;

//llegeix un rejilla nova i la ordena 
void leer_rejilla(Rejilla& r, int n, int k) {
    r.n = n;
    r.k = k;
    for (int i = 0; i < k; ++i) {
        pair<int,int> ij;
        cin >> ij.first >> ij.second;
        
        r.Rj.push_back(make_pair(ij.first,ij.second));
    }

    int I = 0;
    for (int i = k; i < 4*k; ++i) {
        int x = n+1-r.Rj[I].second;
        int y = r.Rj[I].first;
        
        r.Rj.push_back(make_pair(x,y));
        I++;
    }
    //ordenacio perque codifiqui bé
    sort(r.Rj.begin(),r.Rj.begin()+k);          //1ra fila
    sort(r.Rj.begin()+k,r.Rj.begin()+2*k);      //2na fila
    sort(r.Rj.begin()+2*k,r.Rj.begin()+3*k);    //3ra fila
    sort(r.Rj.begin()+3*k,r.Rj.end());          //4ta fila
}
//comprova si la rejilla es valida per codificar o no comprovant que s omplin totes les posicions
bool rejilla_valida(const Rejilla& r) {
    vector < vector < char > > mat(r.n+1, vector< char > (r.n+1));      //matriz n*n

    for (int x = 0; x < 4*r.k; ++x) {
        if (mat[r.Rj[x].first][r.Rj[x].second] != 'x') mat[r.Rj[x].first][r.Rj[x].second] = 'x';
        else return false;
    }
    return true;
}
//retorna el tamany del vector de reixetes
int consultar_tamano_r() {
    return Rejillas.size();
}
//retorna la dimensio de la reixeta, files
int consultar_dimension(const Rejilla& r) {
    return r.n;
}
//retorna la longitud de la reixeta, columnes
int consultar_longitud(const Rejilla& r) {
    return r.k;
}
//afegeix una nova reixeta, comprova que siguin valides les dimensions i longituds, la llegeix i comprova que sigui valida, si ho compleix la guarda al vector reixetes.
void nueva_rejilla() {
    Rejilla r;
    int n, k;
    cin >> n >> k;
    
    if (4*k != (n*n)) {
        //leer posiciones aunque sea no valida
        string s;
        getline(cin,s);
        cout << "error: dimensiones incorrectas de la rejilla";
    }
    else {
        leer_rejilla(r,n,k);
        if (not rejilla_valida(r)) {
            cout << "error: la rejilla con sus giros no cubre todas las posiciones N x N";
        }
        else {
            Rejillas.push_back(r);
            cout << consultar_tamano_r();
        }
    }
    cout << endl;
}
//codifica la rejilla que es passa
void codificar_rejilla_2(const Rejilla& r, const string& input, string& output) {
    int dimension = r.n*r.n;
    int longitud = input.size();
    
    int blokes;
    if (longitud%dimension == 0) blokes = longitud/dimension;
    else blokes = longitud/dimension +1;
    
    vector < vector<char> > mat(r.n+1,vector<char>(r.n+1));     //matriz n*n
    
    int y = 0;
    for (int i = 0; i < blokes; ++i) {
        //llenar matriz n*n con coordenadas de vector 4k
        for (int x = 0; x < 4*r.k; ++x) {
            if (y < longitud) mat[r.Rj[x].first-1][r.Rj[x].second-1] = input[y];
            else mat[r.Rj[x].first-1][r.Rj[x].second-1] = ' ';
            ++y;
        }
        //construir el output con matriz n*n
        for (int f = 0; f < r.n; ++f) {
            for (int c = 0; c < r.n; ++c) {
                output += mat[f][c];
            }
        }
    }
}
//envia a codificar la reixeta del identificador
void codificar_rejilla(int idr, const string& input, string& output) {
    codificar_rejilla_2(Rejillas[idr-1],input,output);
}
//decodifica la rejilla que es passa
void decodificar_rejilla_2(const Rejilla& r, const string& input, string& output) {
    int longitud = input.size();
    int dimension = r.n*r.n;
    int blokes;
    
    if (longitud%dimension == 0) blokes = longitud/dimension;
    else blokes = longitud/dimension +1;
    
    vector < vector<char> > mat(r.n+1,vector<char>(r.n+1));     //matriz n*n

    int y = 0;
    for (int i = 0; i < blokes; ++i) {
        //llenar matriz n*n
        for (int f = 0; f < r.n; ++f) {
            for (int c = 0; c < r.n; ++c) {
                if (y < longitud) mat[f][c] = input[y];
                else mat[f][c] = ' ';
                ++y;
            }
        }
        //construir el output con matriz n*n con coordenadas de vector 4k
        for (int x = 0; x < 4*r.k; ++x) {
            output += mat[r.Rj[x].first-1][r.Rj[x].second-1];
        }
    }
}
//envia a decodificar la reixeta del identificador si el tamany del missatge es correcte
void decodificar_rejilla(int idr, const string& input, string& output) {
    int x = consultar_dimension(Rejillas[idr-1])*consultar_dimension(Rejillas[idr-1]);
    if (input.size()%x != 0) cout << "error: la dimension del mensaje es inadecuada para la rejilla";
    else {
        decodificar_rejilla_2(Rejillas[idr-1],input,output);
        cout << '"' << output << '"';
    }
    cout << endl;
}


//retorna si si la reixeta existeix en el vector no si la reixeta no existeix en el vector
bool existe_rejilla(const int& idr) {
    return idr > 0 and idr <= Rejillas.size();
}
//escriu una reixeta sencera per pantalla
void listar_Rejilla_2(const Rejilla& r) {
    cout << r.n << ' ' << r.k << endl;
    
    for (int i = 0; i < 4*r.k; i++) {
        if (i == r.k-1 or i == r.k*2-1 or i == r.k*3-1 or i == r.k*4-1) cout << '(' << r.Rj[i].first << ',' << r.Rj[i].second << ')' << endl;   //para endl
        else cout << '(' << r.Rj[i].first << ',' << r.Rj[i].second << ')' << ' '; 
    }
}
//escriu per pantalla totes les reixetes del vector amb el seu identificador
void listar_rejillas() {
    for (int i = 0; i < Rejillas.size(); ++i) {
        cout << "Rejilla " << i+1 << ':' << endl;
        listar_Rejilla_2(Rejillas[i]);
    }
}


int main ()
{

    cout << "<<<< PROGRAMA D'ENCRIPTACIÓ DE MISSATGES MITJANÇANT REIXETES >>>>" << endl;
    cout << "Escriu (instrucciones) per veure les funcionalitats del programa" << endl;
    
    //comandos, mensaje a leer, s: terminar lecturas con getline();
    string comando, input, s;
    cin >> comando;
    
    //identificadores
    int idr;

    //mensaje
    string idm, mensaje;
    //rejilla
    Rejilla rej;
    
    while (comando != "fin") {
        
        //instrucciones
        if (comando == "instrucciones") {
            cout << '#' << comando << endl;
            cout << "1-Escriu nuevo_mensaje o nm per afegir un missatge nou" << endl;
            cout << endl;
            cout << "2-Escriu borra_mensaje o bm per eliminar un missatge" << endl;
            cout << endl;
            cout << "3-Escriu listar_mensajes o lm per listar tots els missatges" << endl;
            cout << endl;
            cout << "4-Escriu nueva_rejilla o nr per afegir una reixeta nova" << endl;
            cout << endl;
            cout << "5-Escriu listar_rejillas o lr per listar totes les reixetes" << endl;
            cout << endl;
            cout << "6-Escriu codificar_rejilla o cr per codificar un missatge nou" << endl;
            cout << endl;
            cout << "7-Escriu codificar_guardado_rejilla o cgr per codificar un missatge guardat" << endl;
            cout << endl;
            cout << "8-Escriu decodificar_rejilla o dr per decodificar un missatge nou" << endl;
            cout << endl;
            cout << "9-Escriu informacio o info per veure informacio extra del programa" << endl;
            cout << endl;
            cout << "10-Escriu fin per finalitzar el programa" << endl;
            cout << endl;
        }
        //informacio
        else if (comando == "informacio" or comando == "info") {
            cout << '#' << comando << ' ' << endl;
            cout << "Autor: Maksym Plashkin Ksenyuk" << endl;
            cout << "Curs: 2n BATX" << endl;
            cout << "Data: 13/12/2021" << endl;
        }
        
        //nuevo_mensaje
        else if (comando == "nm" or comando == "nuevo_mensaje") {
            getline(cin,s);
            cin >> idm;
            getline(cin,s);
            getline(cin,mensaje);
            
            cout << '#' << comando << ' ' << idm << endl;
            
            if (existe_mensaje(idm)) cout << "error: ya existe un mensaje con ese identificador";
            else {
                nuevo_mensaje(idm,mensaje);
                cout  << consultar_tamano();
            }
            cout << endl;
        }
        //nueva_rejilla
        else if (comando == "nr" or comando == "nueva_rejilla") {
            cout << '#' << comando << endl;
            nueva_rejilla();
        }
        //borra_mensaje
        else if (comando == "bm" or comando == "borra_mensaje") {
            cin >> idm;
            
            cout << '#' << comando << ' ' << idm << endl;
            
            if (not existe_mensaje(idm)) cout << "error: el mensaje no existe";
            else {
                borra_mensaje(idm);
                cout << consultar_tamano();
            }
            cout << endl;
        }
        //listar_mensajes
        else if (comando == "lm" or comando == "listar_mensajes") {
            cout << '#' << comando << endl;
            listar_mensajes();
        }
        //listar_rejillas
        else if (comando == "lr" or comando == "listar_rejillas") {
            cout << '#' << comando << endl;
            listar_rejillas();
        }
        //codificar_rejilla
        else if (comando == "cr" or comando == "codificar_rejilla") {
            cin >> idr;
            getline(cin,input);
            getline(cin,input);
            
            cout << '#' << comando << ' ' << idr << endl;
            
            if (not existe_rejilla(idr)) cout << "error: la rejilla no existe";
            else {
                string output;
                codificar_rejilla(idr,input,output);
                cout << '"' << output << '"';
            }
            cout << endl;
        }
        //codificar_guardado_rejilla
        else if (comando == "cgr" or comando == "codificar_guardado_rejilla") {
            cin >> idm >> idr;
            
            cout << '#' << comando << ' ' << idm << ' ' << idr << endl;
            
            if (not existe_mensaje(idm)) cout << "error: el mensaje no existe"; 
            else if (not existe_rejilla(idr)) cout << "error: la rejilla no existe"; 
            else {
                string output;
                codificar_rejilla(idr,consultar_mensaje(idm),output);
                cout << '"' << output << '"';
            }
            cout << endl;
        }
        //decodificar_rejilla
        else if (comando == "dr" or comando == "decodificar_rejilla") {
            cin >> idr;
            getline(cin,input);
            getline(cin,input);
            
            cout << '#' << comando << ' ' << idr << endl;
            
            string output;
            if (not existe_rejilla(idr)) cout << "error: la rejilla no existe" << endl;
            else decodificar_rejilla(idr,input,output);
        }
        cin >> comando;        
    }
}
