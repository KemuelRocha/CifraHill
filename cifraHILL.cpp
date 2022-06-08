#include <iostream>
#include <string>
using namespace std;

#define M 3

//Função para calcular a inversa de forma bem manual
int inverse(int b)
{
	int inv;
	int q, r, r1 = 26, r2 = b, t, t1 = 0, t2 = 1;

	while (r2 > 0) {
		q = r1 / r2;
		r = r1 - q * r2;
		r1 = r2;
		r2 = r;

		t = t1 - q * t2;
		t1 = t2;
		t2 = t;
	}

	if (r1 == 1) {
		inv = t1;
		if (inv < 0)
			inv = inv + 26;
		return inv;
	}

	return -1;
}

void inverseMatrix(int key[][3], int inv[][3])
{
	int C[3][3]; // matriz que vai receber a chave
	int A[3][3]; // Matriz adjunta
		
		
	// calculo da determinante de matriz 3x3 estendida pela regra de Sarrus
	// nessa parte está sendo feito os calculos nas diagonais, 
	// da direita para a esquerda soma e da esquerda para a diretira subtrai
	C[0][0] = key[1][1] * key[2][2] - key[2][1] * key[1][2];
	C[0][1] = -(key[1][0] * key[2][2] - key[2][0] * key[1][2]);
	C[0][2] = key[1][0] * key[2][1] - key[1][1] * key[2][0];
	C[1][0] = -(key[0][1] * key[2][2] - key[2][1] * key[0][2]);
	C[1][1] = key[0][0] * key[2][2] - key[2][0] * key[0][2];
	C[1][2] = -(key[0][0] * key[2][1] - key[2][0] * key[0][1]);
	C[2][0] = key[0][1] * key[1][2] - key[0][2] * key[1][1];
	C[2][1] = -(key[0][0] * key[1][2] - key[1][0] * key[0][2]);
	C[2][2] = key[0][0] * key[1][1] - key[1][0] * key[0][1];

	//Organiza os valores de cada diagonal calculada e salva em A
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			A[i][j] = C[j][i];
		}
	}

	// Faz o calculo final da determinante da matriz C
	int det = key[0][0] * C[0][0] + key[0][1] * C[0][1] + key[0][2] * C[0][2]; 

	if (det == 0 || det % 2 == 0 || det % 13 == 0) {
		printf("Não foi encriptado. Insira uma chave valida.\n");
		exit(1);
	}

	int invs = inverse(det);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			inv[i][j] = A[i][j] * invs;
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (inv[i][j] >= 0) {
				inv[i][j] = inv[i][j] % 26;
			}
			else {
				for (int x = 0;; x++) {
					if (x * 26 + inv[i][j] > 0) {
						inv[i][j] = x * 26 + inv[i][j];
						break;
					}
				}
			}
		}
	}
}

string encriptando(string text, int key[][3])
{
	string cifrado = "";
	int k = 0;
	int input[3];

	while (k < text.length()) {
		input[0] = text[k++] - 65;
		input[1] = text[k++] - 65;
		input[2] = text[k++] - 65;

		for (int i = 0; i < M; i++) {
			int cifra = 0;
			for (int j = 0; j < M; j++) {
				cifra += key[i][j] * input[j];
			}
			cifrado += (cifra % 26) + 65;
		}
	}

	return cifrado;
}

string descriptando(string s, int inv[][3])
{
	string d = "";
	int k = 0;
	int input[3];

	while (k < s.length()) {
		input[0] = s[k++] - 65;
		input[1] = s[k++] - 65;
		input[2] = s[k++] - 65;

		for (int i = 0; i < M; i++) {
			int decifrar = 0;
			for (int j = 0; j < M; j++) {
				decifrar += inv[i][j] * input[j];
			}
			d += (decifrar % 26) + 65;
		}
	}
	return d;
}

int main()
{
	// chave em forma de matriz
	int key[3][3] = {
		{ 6, 24, 1 },
		{ 13, 16, 10 },
		{ 20, 17, 15 }
	};

	// recebendo texto claro
	string text = "ACT";
	
	// encriptando...
	string c = encriptando(text, key);
	cout << "Texto cifrado: " << c;

	int inv[3][3];
	inverseMatrix(key, inv);
	
	// descripitando...
	cout << "\n\nTexto descriptado: " << descriptando(c, inv);

	return 0;
}
