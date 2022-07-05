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

using namespace std;

map<vector<vector<int>>, bool> visitados;				  // declaring a map named "visitados" that takes vector(which stores vectors that takes integer values) as its key, and a boolean as its value
map<vector<vector<int>>, vector<vector<int>>> hijos; // declaring a map named "hijos" that takes vectors as its both key and value to keep track of the nodes
vector<vector<int>> estadoObjetivo(3, vector<int>(3));

bool visit(vector<vector<int>> a){ // esta funcion verifica si el nodo ya fue visitado
	return (visitados[a] == true);
}

// Manhattan or A*(A-star) search to find the distance
int manhattan(vector<vector<int>> arreglo, int movimientos){
	int distancia = movimientos;
	for (int i = 0; i < 3; i++){ // for loop to go through each row and column (like matrices). [00,01,02,10,11,12,20,21,22] We'll be using this i,j loop more.
		for (int j = 0; j < 3; j++){
			if (arreglo[i][j] != 0){
				int estadoObjetivo_i = (arreglo[i][j] - 1) / 3;
				int estadoObjetivo_j = (arreglo[i][j] - 1) % 3;
				distancia += abs(j - estadoObjetivo_j) + abs(i - estadoObjetivo_i);
			}
		}
	}
	return distancia;
}

bool estadoObjetivoAlcanzado(vector<vector<int>> a){ // fucion que verifica si ya se encuentra en el estado objetivo
	return (a == estadoObjetivo);
	//return (a[0][0] == 0 && a[0][1] == 1 && a[0][2] == 2 && a[1][0] == 3 && a[1][1] == 4 && a[1][2] == 5 && a[2][0] == 6 && a[2][1] == 7 && a[2][2] == 8);
	// for (int i = 0; i < 3; i++){
	//	for (int j = 0; j < 3; j++){
	//		if (a[i][j] != estadoObjetivo[i][j]){
	//			return false;
	//		}
	//			//return 0; // if our puzzle tiles aren't same with the estadoObjetivo position. For eg. we want a[0][0] = 1, a[0][1] = 2, a[2][2] = 0, etc.
	//	}
	// }

	// return 1;
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

struct cmp
{
	bool operator()(estado &a, estado &b)
	{ // overloading the unary operator "()"
		int am = manhattan(a.first, a.second);
		int bm = manhattan(b.first, b.second);
		return am < bm;
	}
};

void print_path(vector<vector<int>> s){
	if (hijos.count(s)){	   // to see if our vector s is present in our map named "hijos" which takes vectors in it for both key and value, and if it is present
		print_path(hijos[s]); // then print the path
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

	priority_queue<estado, vector<estado>, cmp> colaP; // Priority queue named "Q".
	colaP.push(estado(a, moves));
	while (!colaP.empty()){
		vector<vector<int>> aux = colaP.top().first;
		colaP.pop();
		visitados[aux] = true;
		if (aux == estadoObjetivo){ // print(s)
			print_path(aux);
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

void ejemplosPuzzle(){
	srand(time(NULL));
    int num, arreglo[9];
	cout << endl<< "Ejemplo: ";
	for(int i=0; i<9; i++){
        do{
            num = rand() % 9;
		}while(repeticionDominio(num, arreglo));
        arreglo[i] = num;
        cout << arreglo[i] << "  ";
    }
	cout << "\n\n";
}



int main()
{

	int arregloAux[8];
	int k = 0;
	cout << "Por favor ingresa tu 8 Puzzle como una cadena! \n";
	ejemplosPuzzle();
	vector<vector<int>> estadoInicial(3, vector<int>(3));
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			cin >> estadoInicial[i][j];
			if (estadoInicial[i][j] != 0){
				arregloAux[k] = estadoInicial[i][j];
				k++;
			}
		}
	}

	cout << endl;

	if (existeSolucion(obtenerInversiones(arregloAux))){
			cout << "Numero de inversiones: " << obtenerInversiones(arregloAux) << ", por lo tanto existe solucion.\n"
				 << endl;
			cout << "\nResolviendo el puzzle, espere por favor...\n";
			estadoObjetivo = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
			encontrarSolucion(estadoInicial, 0);
	}else{
		cout << "Numero de inversiones: " << obtenerInversiones(arregloAux) << ", por lo tanto no existe solucion.\n"<< endl;
	};

	return 0;
}