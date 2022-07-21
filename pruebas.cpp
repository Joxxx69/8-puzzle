#include <iostream>
#include <string>
#include <vector>
#include <map>
#include<cstdlib>
#include<ctime>
#include <functional>
#include <queue>
#include <iterator>
#include <utility>

#define ll long long
#define ii pair<int, int>
#define dl pair <ll, ll>
#define vi vector <int>
#define vl vector <ll>
#define vii vector <ii>

using namespace std;

map<vector<vector<int>>, bool> visitados;				  // ddeclarando un mapa llamado "visitados" que toma vector (que almacena vectores que toman valores enteros) como su clave, y un booleano como su valor
map<vector<vector<int>>, vector<vector<int>>> hijos; //declarando un mapa llamado "hijos" que toma vectores como su clave y valor para realizar un seguimiento de los nodos
vector<vector<int>> estadoObjetivo(3, vector<int>(3));
int distancia;

bool visit(vector<vector<int>> a){ // esta funcion verifica si el nodo ya fue visitado
	return (visitados[a] == true);
}

// Manhattan o A*(A-star) buscar para encontrar la distancia
int manhattan(vector<vector<int>> arreglo, int movimientos){
	//int distancia = movimientos;
	distancia = movimientos;
	for (int i = 0; i < 3; i++){ // for loop para pasar por cada fila y columna (como matrices). [00,01,02,10,11,12,20,21,22] Usaremos más este bucle i,j.
		for (int j = 0; j < 3; j++){
			if (arreglo[i][j] != 0){
				int estadoObjetivo_i = (arreglo[i][j] - 1) / 3;
				int estadoObjetivo_j = (arreglo[i][j] - 1) % 3;
				distancia += abs(j - estadoObjetivo_j) + abs(i - estadoObjetivo_i);
			}
		}
	}
	//dist(distancia);
	return distancia;
}

void dist(int distancia){
	cout << "esta es la distancia"<<distancia;
}

//int distaciaManhattan(){
//	ios_base::sync_with_stdio(0);
//	cin.tie(0);
//	cout.tie(0);
//	int n;
//	cin >> n;
//	ii num[ n * n + 1];
//	int in;
//	for (int i = 0; i < n;i++){
//		for (int j = 0; j < n;j++){
//			cin >> in;
//			num[in] = {i, j};
//		}
//	}
//	int suma = 0;
//	for (int i = 1; i < n * n;i++){
//		suma += abs(num[i].firts - num[i + 1].first) + abs(num[i].secod - num[i+1].second);
//	}
//	cout << suma << endl;
//	return suma;
//}

bool estadoObjetivoAlcanzado(vector<vector<int>> a){ // fucion que verifica si ya se encuentra en el estado objetivo
	return (a == estadoObjetivo);
}

bool guardar(int i, int j){ // boolean guardar para ver si la matriz es 3x3
	return (i >= 0 && i <= 2 && j >= 0 && j <= 2);
}

int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, 1, -1};

vector<vector<vector<int>>> vecinos(vector<vector<int>> a){
	pair<int, int> pos; // vector de pares denominado "pos" (empareja un entero con otro entero)
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			if (a[i][j] == 0){
				pos.first = i;
				pos.second = j;
				break;
			}
		}
	}

	vector<vector<vector<int>>> resp; // vector de "respuesta" para agregar los valores correctos empujando hacia atrás
	for (int k = 0; k < 4; k++){
		int cx = pos.first;	 // para tener un seguimiento de los valores "i"
		int cy = pos.second; // para tener un seguimiento de los valores "j"
		vector<vector<int>> n = a;
		if (guardar(cx + dx[k], cy + dy[k])){
			swap(n[cx + dx[k]][cy + dy[k]], n[cx][cy]); // intercambio de matrices
			resp.push_back(n);
		}
	}

	return resp; // vector resultado o respuesta
}

typedef pair<vector<vector<int>>, int> estado; // nuevo tipo definido llamado "estado" que es un vector de pares

struct cmp{
	bool operator()(estado &a, estado &b){ // sobrecargando el operador unario "()"
		int am = manhattan(a.first, a.second);
		int bm = manhattan(b.first, b.second);
		return am < bm;
	}
};

void printMovimientos(vector<vector<int>> s){
	if (hijos.count(s)){	   // para ver si nuestro vector s está presente en nuestro mapa llamado "hijos" que toma vectores tanto para clave como para valor, y si está presente
		printMovimientos(hijos[s]); // luego imprima la ruta
	}

	for (int i = 0; i < 3; i++){
		cout << "| ";
		for (int j = 0; j < 3; j++){
			cout << s[i][j] << " | ";
		}
		cout << endl;
	}
	cout << endl;
	return;
}


void encontrarSolucion(vector<vector<int>> a, int moves){

	priority_queue<estado, vector<estado>, cmp> colaP; // cola de prioridades donde el primer elemento es el mayor
	colaP.push(estado(a, moves));
	while (!colaP.empty()){
		vector<vector<int>> aux = colaP.top().first;
		colaP.pop();
		visitados[aux] = true;
		cout << "esta es la distancia: " << distancia;
		if (aux == estadoObjetivo){ // print(s)
			cout << endl;
			printMovimientos(aux);
			break;
		}
		vector<vector<vector<int>>> ns = vecinos(aux);
		vector<vector<vector<int>>>::iterator it;
		for (it = ns.begin(); it != ns.end(); it++){
			vector<vector<int>> temp = *it;
			if (!visit(temp)){
				hijos.insert(pair<vector<vector<int>>, vector<vector<int>>>(temp, aux));
				colaP.push(estado(temp, moves + 1));
			}
		}
	}
	return;
}

int obtenerInversiones(int arr[]){
	int contador = 0;
	for (int i = 0; i < 7; i++){
		for (int j = i + 1; j < 8; j++){
			if (arr[j] && arr[i] && arr[i] > arr[j]){
				contador++;
			}
		}
	}
	return contador;
}

bool existeSolucion(int inversiones){
	return (inversiones % 2 == 0);
}

bool repeticionDominio(int num, int arreglo[]){
    for(int i=0; i<10; i++){
        if(num == arreglo[i]){
            return true;
		}
	}
    return false;
}

int *ejemplosPuzzle(int arreglo[]){
	srand(time(NULL));
    int num;
	for(int i=0; i<9; i++){
		do{
			num = rand() % 9;
		}while(repeticionDominio(num, arreglo));
		arreglo[i] = num;
	}
	return arreglo;
}

int main(){
    int  arreglo[9],arregloAux[8],k=0,w=0;
	int *arregloEstados = ejemplosPuzzle(arreglo);
	cout << "Por favor ingresa tu 8 Puzzle como una cadena! \n";
	vector<vector<int>> estadoInicial(3, vector<int>(3));
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			estadoInicial[i][j] = arregloEstados[w];
			w++;
			if (estadoInicial[i][j] != 0){
				arregloAux[k] = estadoInicial[i][j];
				k++;
			}
		}
	}
	cout << endl<< "Estado Inicial: ";
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			cout<<estadoInicial[i][j]<<"  ";
		}
	}

	cout << endl;

	if (existeSolucion(obtenerInversiones(arregloAux))){
		cout<< "Numero de inversiones: " << obtenerInversiones(arregloAux) << ", por lo tanto existe solucion.\n"<< endl;
		cout << "\nResolviendo el puzzle, espere por favor...\n";
		estadoObjetivo = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
		cout << endl;
		encontrarSolucion(estadoInicial, 0);
		cout << "esta es la distancia: " << distancia;

	}else{
		cout << "Numero de inversiones: " << obtenerInversiones(arregloAux) << ", por lo tanto no existe solucion.\n"<< endl;
	};
	return 0;
}