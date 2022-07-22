#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <queue>
#include <iterator>
#include <utility>
#include <algorithm>

using namespace std;

map<vector<vector<int>>, bool> visitados;			 // ddeclarando un mapa llamado "visitados" que toma vector (que almacena vectores que toman valores enteros) como su clave, y un booleano como su valor
map<vector<vector<int>>, vector<vector<int>>> hijos; // declarando un mapa llamado "hijos" que toma vectores como su clave y valor para realizar un seguimiento de los nodos
vector<vector<int>> estadoObjetivo(3, vector<int>(3));
int distancia;
int profundidad = 0;
vector<int> valoresNOdos;

// Manhattan o A*(A-star) buscar para encontrar la distancia
int manhattan(vector<vector<int>> arreglo, int movimientos)
{
	distancia = movimientos;
	for (int i = 0; i < 3; i++){ // for loop para pasar por cada fila y columna (como matrices). [00,01,02,10,11,12,20,21,22] Usaremos más este bucle i,j.
		for (int j = 0; j < 3; j++){
			if (arreglo[i][j] != 0){ // No se cuenta a la ficha 0, que representa a la ficha vacía
				/*
				 *La función de Manhattan es igual a |xf-x0| + |yf-y0|. Donde xf representa a la columna donde se debe encontrar el
                 *valor consultado, yf a la fila donde se debe encontrar el valor. x0 y y0 son las coordenadas [columna][fila] donde se
                 *encuentra el valor actual.
                 *
                 *Como se está utilizando una vector de vectores, el cual es dinamico y tiene una dimension de n*n --> cuadrada.
                 *Esto se puede solucionar al saber que, una posición t se puede dividir en componentes de filas y columnas de acuerdo a:
                 *
                 *  fila = t / numero_total_columnas (entero).   columna =  t % número_total_columnas(entero).
                 *                 *
                 *Por cómo está estructurada la resolución del problema, el valor t correspondiente al número ficha de la posicion de la matriz restado 1
                 *así que con ello, se obtendrá la posición esperada, con respecto a su valor de ficha.
                 *
                 *  Ejemplo: Ficha 1 está en posición 3 del arreglo fichas[]. La posición 3 será igual a: fila = (arreglo[i][j]-1)/3 = 1.
                 *                                                                                        columna = (arreglo[i][j]) MOD 3 = 0.
                 *                                                            Posición 3 de la matriz corresponde a (1; 0) de una matriz.
                 *
                 *  Ahora Ficha 1 debería estar en t = 1. Así que, la posición esperada es: fila = (arreglo[i][j]-1)/3 = 1.
                 *                                                                          columna = (arreglo[i][j]) MOD 3 = 1.
                 *																Posición esperada de t = 1: (1; 1).
                 *
                 */
				int fila = (arreglo[i][j] - 1) / 3;
				int columna = (arreglo[i][j] - 1) % 3;
				distancia += abs(j - columna) + abs(i - fila);
				if (valoresNOdos.size() < 8){
					valoresNOdos.push_back(abs(j - columna) + abs(i - fila));
				}
			}
		}
	}
	return distancia;
}

bool guardar(int i, int j)
{ // boolean guardar para ver si la matriz es 3x3
	return (i >= 0 && i <= 2 && j >= 0 && j <= 2);
}
//Lista de las posibles acciones con las posiciones matriciales 
int dx[] = {-1, 1, 0, 0};// filas 
int dy[] = {0, 0, 1, -1};// columnas

vector<vector<vector<int>>> vecinos(vector<vector<int>> a)
{
	pair<int, int> pos; // vector de pares denominado "pos" (empareja un entero con otro entero)
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (a[i][j] == 0)
			{
				pos.first = i;
				pos.second = j;
				break;
			}
		}
	}

	vector<vector<vector<int>>> resp; // vector de "respuesta" para agregar los valores correctos empujando hacia atrás
	for (int k = 0; k < 4; k++)
	{
		int cx = pos.first;	 // para tener un seguimiento de los valores "i"
		int cy = pos.second; // para tener un seguimiento de los valores "j"
		vector<vector<int>> n = a;
		if (guardar(cx + dx[k], cy + dy[k]))
		{
			swap(n[cx + dx[k]][cy + dy[k]], n[cx][cy]); // intercambio de matrices
			resp.push_back(n);
		}
	}

	return resp; // vector resultado o respuesta
}


void estadosPrint(vector<vector<int>> s)
{
	if (hijos.count(s))
	{							// para ver si nuestro vector s está presente en nuestro mapa llamado "hijos" que toma vectores tanto para clave como para valor, y si está presente
		estadosPrint(hijos[s]); // luego imprima la ruta
		profundidad++;
	}

	for (int i = 0; i < 3; i++)
	{
		cout << "| ";
		for (int j = 0; j < 3; j++)
		{
			cout << s[i][j] << " | ";
		}
		cout << endl;
	}
	cout << endl;
	return;
}

bool estadoObjetivoAlcanzado(vector<vector<int>> a)
{ // fucion que verifica si ya se encuentra en el estado objetivo
	return (a == estadoObjetivo);
}
bool visit(vector<vector<int>> a)
{ // esta funcion verifica si el nodo ya fue visitado
	return (visitados[a] == true);
}
typedef pair<vector<vector<int>>, int> estado; // nuevo tipo definido llamado "estado" que es un vector de pares


/* Un predicado binario que toma dos elementos (de tipoT) como argumentos y devuelve un bool.
La expresión comp(a,b), donde comp es un objeto de este tipo y a y b son elementos del contenedor,
devolverá true si se considera que a va antes de b en el estricto ordenamiento débil que define la 
función.los prioridad_cola utiliza esta función para mantener los elementos ordenados de una manera 
que conserva las propiedades del montón (es decir, que el elemento extraído sea el último de acuerdo 
con este estricto ordenamiento débil ). */
struct comparador{  // este elem
	bool operator()(estado &a, estado &b){ // sobrecargando el operador unario "()"
		int am = manhattan(a.first, a.second);
		int bm = manhattan(b.first, b.second);
		return am < bm;
	}
};


void encontrarSolucion(vector<vector<int>> a, int moves)
{

	priority_queue<estado, vector<estado>, comparador> colaP; // cola de prioridades donde el primer elemento es el menor
	colaP.push(estado(a, moves));
	while (!colaP.empty())
	{
		vector<vector<int>> aux = colaP.top().first;
		colaP.pop();
		visitados[aux] = true;
		// if (aux == estadoObjetivo){ // print(s)
		if (estadoObjetivoAlcanzado(aux))
		{ // print(s)
			cout << endl;
			estadosPrint(aux);
			break;
		}
		vector<vector<vector<int>>> ns = vecinos(aux);
		vector<vector<vector<int>>>::iterator it;
		for (it = ns.begin(); it != ns.end(); it++)
		{
			vector<vector<int>> temp = *it;
			if (!visit(temp))
			{
				hijos.insert(pair<vector<vector<int>>, vector<vector<int>>>(temp, aux));
				colaP.push(estado(temp, moves + 1));
			}
		}
	}
	return;
}

int obtenerInversiones(int arr[])
{
	int contador = 0;
	for (int i = 0; i < 7; i++)
	{
		for (int j = i + 1; j < 8; j++)
		{
			if (arr[j] && arr[i] && arr[i] > arr[j])
			{
				contador++;
			}
		}
	}
	return contador;
}

bool existeSolucion(int inversiones)
{
	return (inversiones % 2 == 0);
}

bool repeticionDominio(int num, int arreglo[])
{
	for (int i = 0; i < 10; i++)
	{
		if (num == arreglo[i])
		{
			return true;
		}
	}
	return false;
}

int *ejemplosPuzzle(int arreglo[])
{
	srand(time(NULL));
	int num;
	for (int i = 0; i < 9; i++)
	{
		do
		{
			num = rand() % 9;
		} while (repeticionDominio(num, arreglo));
		arreglo[i] = num;
	}
	return arreglo;
}

int main()
{

	int arreglo[9], arregloAux[8], k = 0, w = 0;
	int *arregloEstados = ejemplosPuzzle(arreglo);
	vector<vector<int>> estadoInicial(3, vector<int>(3));
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			estadoInicial[i][j] = arregloEstados[w];
			w++;
			if (estadoInicial[i][j] != 0)
			{
				arregloAux[k] = estadoInicial[i][j];
				k++;
			}
		}
	}
	cout << endl
		 << "Estado Inicial: " << endl;
	estadosPrint(estadoInicial);
	int distanciaManhattan = manhattan(estadoInicial,0);

	if (manhattan(estadoInicial, 0) <= 15)
	{
		cout << "El valor heuristico es de " << distanciaManhattan << endl;
		if (existeSolucion(obtenerInversiones(arregloAux)))
		{
			cout << "Numero de inversiones: " << obtenerInversiones(arregloAux) << ", por lo tanto existe solucion.\n"
				 << endl;
			cout << "\nResolviendo el puzzle, espere por favor...\n";
			estadoObjetivo = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
			cout << endl;
			encontrarSolucion(estadoInicial, 0);
			cout << "Distancia Manhattan: " << distanciaManhattan;
			cout << endl;
			cout << "Costo de cada nodo: ";
			for (int idx : valoresNOdos)
			{
				cout << idx << " ";
			}
			cout << endl;
			cout << "Profundidad: " << profundidad << endl;
		}
		else
		{
			cout << "Numero de inversiones: " << obtenerInversiones(arregloAux) << ", por lo tanto no existe solucion.\n"
				 << endl;
		};
	}
	else
	{
		cout << "El costo heuristico es " << manhattan(estadoInicial, 0) << ", por lo tanto es mayor a 10";
	}
	return 0;
}