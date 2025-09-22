/*Analizador sint�ctico de Formulas proposicionales
La sintaxis de la l�gica proposicional se especifica mediante un alfabeto
Y un conjunto de reglas de formaci�n.
Programa en C++ Orientado a objetos, construido bajo plataforma Linux.
Algoritmo basado en el an�lisis de la transici�n de estados de un Aut�mata.
Lee una cadena "Formula" y luego la almacena en una TDA lista enlazada.
Posteriormente analiza si la formula esta bien formada o no. Para ello
Se basa en an�lisis de la transici�n de estados del aut�mata adjunto
Para ejecutar use ./asintactico.bin en una consola de comandos


Alfabeto z={(*,&,|,-,=),(p,q,r,s),'(',')'}

Donde: * : Representa a la conectiva Unaria Negaci�n
Donde: & : Representa a la conectiva binaria AND
Donde: | : Representa a la conectiva binaria OR
Donde: - : Representa a la conectiva binaria Implicaci�n o Condicional
Donde: = : Representa a la conectiva binaria Doble Implicaci�n o Bicondicional
Donde: p,q,r,s : Representan los s�mbolos de proposici�n

Ejemplos de Formulas validas:

1)   ((p&q)|(*r=s)-(**p&*q))
2)   p&q|*r=s-**p&*q
3)   ((*p|(q&*r))-((*q&p)-r))

Por: Joel M. Prada 
joel.prada@gmail.com
*/

#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;
/*Optimize Branch*/ 
//**************************************************************************
class nodo
{
private:
	char valor;
	nodo *siguiente;
	friend class lista;

public:
	nodo(char v, nodo *sig = NULL)
	{
		valor = v;
		siguiente = sig;
	}
};

typedef nodo *pnodo;
//**************************************************************************
class lista
{
public:
	lista() { primero = actual = NULL; } // Constructor
	~lista();							 // Destructor
	void Insertar(char v);
	bool ListaVacia() { return primero == NULL; }
	void Siguiente()
	{
		if (actual)
			actual = actual->siguiente;
	}
	void Primero() { actual = primero; }
	void Ultimo()
	{
		Primero();
		if (!ListaVacia())
			while (actual->siguiente)
				Siguiente();
	}
	bool Actual() { return actual != NULL; }
	int ValorActual() { return actual->valor; }
	void AnalizarFormula(); // Funci�n para analizar la validez de la formula

private:
	pnodo primero;
	pnodo actual;
};
//**************************************************************************
lista::~lista()
{
	pnodo aux;
	while (primero)
	{
		aux = primero;
		primero = primero->siguiente;
		delete aux;
	}
	actual = NULL;
}
//**************************************************************************
void lista::Insertar(char v)
{
	pnodo siguiente;

	if (ListaVacia())
	{
		primero = new nodo(v, primero);
		siguiente = primero;
	}
	else
	{
		siguiente->siguiente = new nodo(v, siguiente->siguiente);
		siguiente = siguiente->siguiente;
	}
}
/* *************************************************************************
Nota: Despu�s de encontrar el primer error, los siguientes pueden ser inexactos
porque el aut�mata continua su recorrido aceptando un caracter en un
estado donde no deb�a, no realizando la respectiva transici�n hacia otro
estado sino hasta el siguiente ciclo si aplica. En este instante se deber�
abortar el an�lisis y dar como resultado que la formula esta mal formada pero
se continua para seguir evaluando toda la formula.
*/
void lista::AnalizarFormula()
{
	nodo *aux;
	aux = primero;
	int PA = 0, PC = 0;
	bool FBFormada = true;
	char estado = 'A'; // Definimos el estado Inicial como 'A'
	cout << "-------------------------------------------------------------------------------" << endl;
	cout << "- - - - - - - Analisis de la formula por transicion de estados  - - - - - - - -" << endl;
	cout << "-------------------------------------------------------------------------------" << endl;
	cout << "Estado | Caracter | Observacion " << endl;
	cout << "-------------------------------------------------------------------------------" << endl;
	while (aux)
	{
		cout << "   " << estado << "   |    " << aux->valor << "     |";
		switch (estado)
		{
		case 'A':
		{
			switch (aux->valor)
			{
			case '(':
				estado = 'B';
				PA++;
				break;
			case '*':
				estado = 'B';
				break;
			case 'p':
			case 'q':
			case 'r':
			case 's':
				estado = 'C';
				break;
			// Trancisiones a error
			case ')':
				cout << " No debe iniciar una formula con ')'";
				FBFormada = false;
				break;
			case '&':
			case '|':
			case '-':
			case '=':
				cout << " No debe iniciar una formula con una conectiva binaria";
				FBFormada = false;
				break;
			default:
				cout << " Caracter no pertenece al alfabeto";
				FBFormada = false;
				break;
			}
			break;
		}
		case 'B':
		{
			switch (aux->valor)
			{
			case '(':
				estado = 'B';
				PA++;
				break;
			case '*':
				estado = 'B';
				break;
			case 'p':
			case 'q':
			case 'r':
			case 's':
				estado = 'C';
				break;
			// Trancisiones a error
			case ')':
				cout << " Despues de Negacion o de '(' no debe cerrar parentisis";
				FBFormada = false;
				break;
			case '&':
			case '|':
			case '-':
			case '=':
				cout << " Despues de Negacion o de '(' no debe ir una conect. binaria";
				FBFormada = false;
				break;
			default:
				cout << " Caracter no pertenece al alfabeto ";
				FBFormada = false;
				break;
			}
			break;
		}
		case 'C':
		{
			switch (aux->valor)
			{
			case '&':
			case '|':
			case '-':
			case '=':
				estado = 'D';
				break;
			case ')':
				estado = 'E';
				PC++;
				break;
			// Trancisiones a error
			case 'p':
			case 'q':
			case 'r':
			case 's':
				cout << " Despues de un simb de proposicion (p,q,r,s) no debe ir otro";
				FBFormada = false;
				break;
			case '*':
				cout << " Despues de un simb. de proposicion no debe ir negacion";
				FBFormada = false;
				break;
			case '(':
				cout << " Despues de un simb. de proposicion no debe abrir parentisis";
				FBFormada = false;
				break;
			default:
				cout << " Caracter no pertenece al alfabeto";
				FBFormada = false;
				break;
			}
			break;
		}
		case 'D':
		{
			switch (aux->valor)
			{
			case 'p':
			case 'q':
			case 'r':
			case 's':
				estado = 'C';
				break;
			case '(':
				estado = 'B';
				PA++;
				break;
			case '*':
				estado = 'B';
				break;
			// Trancisiones a error
			case ')':
				cout << " Despues de una conectiva binaria no debe cerrar parentisis";
				FBFormada = false;
				break;
			case '&':
			case '|':
			case '-':
			case '=':
				cout << " Despues de una conectiva binaria no debe ir otra.";
				FBFormada = false;
				break;
			default:
				cout << " Caracter no pertenece al alfabeto";
				FBFormada = false;
				break;
			}
			break;
		}
		case 'E':
		{
			switch (aux->valor)
			{
			case '&':
			case '|':
			case '-':
			case '=':
				estado = 'D';
				break;
			case ')':
				estado = 'E';
				PC++;
				break;
			// Trancisiones a error
			case 'p':
			case 'q':
			case 'r':
			case 's':
				cout << " Despues de ')' no debe ir un simb. de proposicion (p,q,r,s)";
				FBFormada = false;
				break;
			case '*':
				cout << " Despues de ')' no debe ir la conectiva unaria negacion";
				FBFormada = false;
				break;
			case '(':
				cout << " Despues de cerrar parentisis no debe abrir parentisis";
				FBFormada = false;
				break;
			default:
				cout << " Caracter no pertenece al alfabeto";
				FBFormada = false;
				break;
			}
			break;
		}
		}
		cout << endl;
		aux = aux->siguiente;
	}
	cout << "-------------------------------------------------------------------------------" << endl;
	if (PA > PC)
	{
		cout << "Error de parentisis, posiblemente falta cerrar " << PA - PC << " Parentisis" << endl;
		FBFormada = false;
	}
	if (PA < PC)
	{
		cout << "Error de parentisis, posiblemente se cerraron " << PC - PA << " Parentisis de mas" << endl;
		FBFormada = false;
	}
	// Si la formula esta bien formada hasta donde va, pero no acaba en un estado terminal del automata, entonces...
	if (FBFormada)
	{
		if (estado != 'C' && estado != 'E') // Si no acaba en un estado terminal
			cout << estado << " FORMULA INCOMPLETA" << endl;
		else
			cout << "FORMULA BIEN FORMADA" << endl;
	}
	else
		cout << "FORMULA MAL FORMADA" << endl;
}
//**************************************************************************
int main()
{
	char formula[80];
	char rta;
	do
	{
		lista Lista;
		cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
		cout << "- - - - - - - - - - -  ANALIZADOR DE FORMULAS PROPOSICIONALES - - - - - - - - -" << endl;
		cout << "- - - - - - - ALGORITMO BASADO EN TRANSICION DE ESTADOS DE UN AUTOMATA  - - - -" << endl;
		cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
		cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
		cout << "-------------------------------------------------------------------------------" << endl;
		cout << "Conectivas: AND: &      OR: |      Implica: -      Doble Implic: =      Neg : *" << endl;
		cout << "Simbolos de proposicion: p, q, r, s  Parentisis: ( )" << endl;
		cout << "Ejemplo: (p|q)&(*p=q)-*p" << endl;
		cout << "-------------------------------------------------------------------------------" << endl;
		cout << "Ingrese la formula: ";
		cin.getline(formula, sizeof(formula) - 1); // No tenemos en cuenta el caracter de fin de linea
		int tam = strlen(formula);
		for (int i = 0; i <= tam - 1; i++)
		{
			Lista.Insertar(formula[i]);
		}
		Lista.AnalizarFormula();
		Lista.~lista(); // Llamamos al destructor, el cual libera de memoria los nodos de la lista
		cout << "Pulse (ENTER) para analizar otra formula. Pulse 'x' y luego (ENTER) para salir.     " << endl;
		rta = tolower(cin.get());
		cout << "-------------------------------------------------------------------------------" << endl;
	} while (rta != 'x');
	return 0;
}
