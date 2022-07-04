#include <iostream>
#include <vector>
#include <iterator>
#include <map>
#include <string>
//#include <bits/stdc++.h>
#include <functional>
#include <utility>
#include <queue>
#include <fstream>

using namespace std;

map<vector<vector<int>>, bool> visited;				  // declaring a map named "visited" that takes vector(which stores vectors that takes integer values) as its key, and a boolean as its value
map<vector<vector<int>>, vector<vector<int>>> parent; // declaring a map named "parent" that takes vectors as its both key and value to keep track of the nodes
vector<vector<int>> goal(3, vector<int>(3));


bool visit(vector<vector<int>> a){ // esta funcion verifica si el nodo ya fue visitado
	return (visited[a] == true);
}


// Manhattan or A*(A-star) search to find the distance
int manhattan(vector<vector<int>> a, int moves){
	int dist = moves;
	for (int i = 0; i < 3; i++){ // for loop to go through each row and column (like matrices). [00,01,02,10,11,12,20,21,22] We'll be using this i,j loop more.
		for (int j = 0; j < 3; j++){
			if (a[i][j] != 0){
				//for (int k = 0; k < 3; k++){
					//for (int l = 0; l < 3; l++){
				int goal_i = (a[i][j] - 1) / 3;
				int goal_j = (a[i][j] - 1) % 3;
				dist += abs(j - goal_j) + abs(i - goal_i);
				//if (a[i][j] == goal[k][l]){
				//	dist += abs(i - k) + abs(j - l); // absolute value since it may be negative

				//}
					//}
				//}
			}
		}
	}
	return dist;
}

//int manhat(vector<vector<int>> node)
//{
//	int sum = 0;
//	for (int y = 0; y < 3; y++)
//	{
//		for (int x = 0; x < 3; x++)
//		{
//			int value = node[y][x];
//			if (value != 0)
//			{
//				int goal_x = (value - 1) % 3;
//				int goal_y = (value - 1) / 3;
//				sum = sum + abs(x - goal_x) + abs(y - goal_y);
//			}
//		}
//	}
//	return sum;
//}

bool isGoal(vector<vector<int>> a){ // fucion que verifica si ya se encuentra en el estado objetivo
	return (a[0][0] == 0 && a[0][1] == 1 && a[0][2] == 2 && a[1][0] == 3 && a[1][1] == 4 && a[1][2] == 5 && a[2][0] == 6 && a[2][1] == 7 && a[2][2] == 8);
	//for (int i = 0; i < 3; i++){
	//	for (int j = 0; j < 3; j++){
	//		if (a[i][j] != goal[i][j]){
	//			return false;
	//		}
	//			//return 0; // if our puzzle tiles aren't same with the goal position. For eg. we want a[0][0] = 1, a[0][1] = 2, a[2][2] = 0, etc.
	//	}
	//}

	//return 1;
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
	bool operator()(estado &a, estado &b){ // overloading the unary operator "()"
		int am = manhattan(a.first, a.second);
		int bm = manhattan(b.first, b.second);
		return am < bm;
	}
};

void print_path(vector<vector<int>> s){
	if (parent.count(s)){	   // to see if our vector s is present in our map named "parent" which takes vectors in it for both key and value, and if it is present
		print_path(parent[s]); // then print the path
	}

	for (int i = 0; i < 3; i++){
		cout << "| ";
		for (int j = 0; j < 3; j++){
			cout <<s[i][j]<<" | ";
		}
		cout << endl;
	}
	cout << endl;
	return;
}

//void print(vector<vector<int>> s){
//	for (int i = 0; i < 3; i++){
//		for (int j = 0; j < 3; j++){
//			printf("%d ", s[i][j]);
//			cout << "valor: [" << i << "][" << j << "]:"<<s[i][j];
//		}
//		cout << endl;
//	}
//}

void solve(vector<vector<int>> a, int moves){

	priority_queue<estado, vector<estado>, cmp> Q; // Priority queue named "Q".
	Q.push(estado(a, moves));
	while (!Q.empty()){
		vector<vector<int>> s = Q.top().first;
		Q.pop();
		visited[s] = true;
		if (s == goal){ // print(s)
			print_path(s);
			break;
		}
		vector<vector<vector<int>>> ns = vecinos(s);
		vector<vector<vector<int>>>::iterator it;
		for (it = ns.begin(); it != ns.end(); it++){
			vector<vector<int>> temp = *it;
			if (!visit(temp)){
				parent.insert(pair<vector<vector<int>>, vector<vector<int>>>(temp, s));
				Q.push(estado(temp, moves + 1));
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

//bool existeSolucion(int inversiones)
//{
//	if (inversiones % 2 == 0)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	};
//}

bool existeSolucion(int inversiones){
	return (inversiones % 2 == 0);
}

int main()
{

	int arregloAux[8];
	int k = 0;
	cout << "Por favor ingresa tu 8 Puzzle como una cadena! \n";
	cout << "\nEjemplo : 0 1 3 4 2 5 7 8 6\n";
	vector<vector<int>> a(3, vector<int>(3));
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cin >> a[i][j];
			if (a[i][j] != 0)
			{
				arregloAux[k] = a[i][j];
				k++;
			}
		}
	}

	/*for (int i = 0; i < 8; i++)
	{
		cout << arregloAux[i] << " ";
	}*/

	cout << endl;
	if (existeSolucion(obtenerInversiones(arregloAux)) == 1)
	{
		cout << "Numero de inversiones: " << obtenerInversiones(arregloAux) << ", por lo tanto existe solucion.\n"
			 << endl;
	}
	else
	{
		cout << "Numero de inversiones: " << obtenerInversiones(arregloAux) << ", por lo tanto no existe solucion.\n"
			 << endl;
	};

	cout << "\nResolviendo el puzzle, espere por favor...\n";
	goal[0][0] = 0;
	goal[0][1] = 1;
	goal[0][2] = 2;
	goal[1][0] = 3;
	goal[1][1] = 4;
	goal[1][2] = 5;
	goal[2][0] = 6;
	goal[2][1] = 7;
	goal[2][2] = 8;
	solve(a, 0);

	return 0;
}