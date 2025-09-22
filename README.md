# 📘 Analizador Sintáctico de Fórmulas Proposicionales en C++

---

## 1. Introducción

Este programa implementa un **analizador sintáctico de fórmulas proposicionales** usando C++ y programación orientada a objetos.  
Se basa en la simulación de un **autómata finito** que analiza la fórmula carácter por carácter, validando si está **bien formada** según reglas de la lógica proposicional.  

---

## 2. El autómata

El autómata utilizado para el análisis está representado en el siguiente diagrama:  

![Autómata](https://github.com/joelprada/Analizador-Sintactico/blob/master/Automata-Asin.png)  

Cada estado representa un contexto válido en la fórmula, y las transiciones dependen del carácter leído.

---

## 3. Alfabeto y reglas

El alfabeto definido es:  

z = { (*, &, |, -, =), (p, q, r, s), (, ) }

cpp
Copiar código

Donde:  

- `*` → Conectiva unaria: Negación  
- `&` → Conectiva binaria: AND  
- `|` → Conectiva binaria: OR  
- `-` → Conectiva binaria: Implicación (Condicional)  
- `=` → Conectiva binaria: Doble Implicación (Bicondicional)  
- `p, q, r, s` → Símbolos de proposición  
- `(`, `)` → Paréntesis de agrupación  

Ejemplos de fórmulas válidas:  

1. `((p&q)|(*r=s)-(**p&*q))`  
2. `p&q|*r=s-**p&*q`  
3. `((*p|(q&*r))-((*q&p)-r))`  

---

## 4. Estructura del programa

El programa se organiza en:  

- **Clase `nodo`**: Representa cada carácter de la fórmula en una lista enlazada.  
- **Clase `lista`**: Almacena y recorre la fórmula, además implementa el método `AnalizarFormula` para simular el autómata.  
- **Autómata**: Usa estados (`A, B, C, D, E`) que representan diferentes fases de la validación.  
- **Función `main`**: Interfaz con el usuario para ingresar fórmulas y analizarlas.  

---

## 5. Código completo con comentarios

```cpp
/*  
Analizador sintáctico de fórmulas proposicionales

Este programa:
1. Lee una fórmula lógica proposicional.
2. La almacena en una lista enlazada (TDA).
3. Analiza la validez de la fórmula usando un autómata finito.
*/

#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

//**************************************************************************
// Clase nodo: representa un carácter de la fórmula en la lista enlazada
//**************************************************************************
class nodo {
private:
    char valor;
    nodo *siguiente;
    friend class lista;

public:
    nodo(char v, nodo *sig = NULL) {
        valor = v;
        siguiente = sig;
    }
};

typedef nodo *pnodo;

//**************************************************************************
// Clase lista: almacena la fórmula como lista enlazada y la analiza
//**************************************************************************
class lista {
public:
    lista() { primero = actual = NULL; } // Constructor
    ~lista();                            // Destructor

    void Insertar(char v);               // Insertar un carácter
    bool ListaVacia() { return primero == NULL; }
    void Siguiente() { if (actual) actual = actual->siguiente; }
    void Primero() { actual = primero; }
    bool Actual() { return actual != NULL; }
    int ValorActual() { return actual->valor; }

    // Función principal: analiza la validez de la fórmula
    void AnalizarFormula();

private:
    pnodo primero;
    pnodo actual;
};

//**************************************************************************
// Destructor: libera memoria de la lista
//**************************************************************************
lista::~lista() {
    pnodo aux;
    while (primero) {
        aux = primero;
        primero = primero->siguiente;
        delete aux;
    }
    actual = NULL;
}

//**************************************************************************
// Insertar: añade un carácter al final de la lista
//**************************************************************************
void lista::Insertar(char v) {
    static pnodo ultimo = NULL; // Último nodo para insertar en cola
    if (ListaVacia()) {
        primero = new nodo(v, NULL);
        ultimo = primero;
    } else {
        ultimo->siguiente = new nodo(v, NULL);
        ultimo = ultimo->siguiente;
    }
}

//**************************************************************************
// AnalizarFormula: recorre la lista y simula el autómata
//**************************************************************************
void lista::AnalizarFormula() {
    nodo *aux = primero;
    int PA = 0, PC = 0;        // Contadores de paréntesis
    bool FBFormada = true;     // Bandera de validez
    char estado = 'A';         // Estado inicial

    cout << "-----------------------------------------------" << endl;
    cout << "Analisis de la formula por transicion de estados" << endl;
    cout << "-----------------------------------------------" << endl;
    cout << "Estado | Caracter | Observacion " << endl;
    cout << "-----------------------------------------------" << endl;

    while (aux) {
        cout << "   " << estado << "   |    " << aux->valor << "     |";

        switch (estado) {
        case 'A':
            // Estado inicial
            switch (aux->valor) {
            case '(':
                estado = 'B'; PA++; break;
            case '*':
                estado = 'B'; break;
            case 'p': case 'q': case 'r': case 's':
                estado = 'C'; break;
            default:
                cout << " Error: caracter inesperado al inicio"; FBFormada = false;
                break;
            }
            break;

        case 'B':
            // Después de '(' o negación '*'
            switch (aux->valor) {
            case '(':
                estado = 'B'; PA++; break;
            case '*':
                estado = 'B'; break;
            case 'p': case 'q': case 'r': case 's':
                estado = 'C'; break;
            default:
                cout << " Error: después de '(' o '*' debe ir proposición"; FBFormada = false;
                break;
            }
            break;

        case 'C':
            // Después de proposición
            switch (aux->valor) {
            case '&': case '|': case '-': case '=':
                estado = 'D'; break;
            case ')':
                estado = 'E'; PC++; break;
            default:
                cout << " Error: símbolo inválido después de proposición"; FBFormada = false;
                break;
            }
            break;

        case 'D':
            // Después de conectiva binaria
            switch (aux->valor) {
            case 'p': case 'q': case 'r': case 's':
                estado = 'C'; break;
            case '(':
                estado = 'B'; PA++; break;
            case '*':
                estado = 'B'; break;
            default:
                cout << " Error: después de conectiva debe ir proposición"; FBFormada = false;
                break;
            }
            break;

        case 'E':
            // Después de cerrar paréntesis
            switch (aux->valor) {
            case '&': case '|': case '-': case '=':
                estado = 'D'; break;
            case ')':
                estado = 'E'; PC++; break;
            default:
                cout << " Error: símbolo inválido después de ')'"; FBFormada = false;
                break;
            }
            break;
        }

        cout << endl;
        aux = aux->siguiente;
    }

    cout << "-----------------------------------------------" << endl;

    // Validar balance de paréntesis
    if (PA > PC) {
        cout << "Error: faltan " << PA - PC << " paréntesis de cierre" << endl;
        FBFormada = false;
    }
    if (PA < PC) {
        cout << "Error: sobran " << PC - PA << " paréntesis de cierre" << endl;
        FBFormada = false;
    }

    // Validar estado final
    if (FBFormada) {
        if (estado != 'C' && estado != 'E')
            cout << estado << " → FORMULA INCOMPLETA" << endl;
        else
            cout << "FORMULA BIEN FORMADA" << endl;
    } else {
        cout << "FORMULA MAL FORMADA" << endl;
    }
}

//**************************************************************************
// Programa principal
//**************************************************************************
int main() {
    char formula[80];
    char rta;

    do {
        lista Lista;

        cout << "-----------------------------------------------" << endl;
        cout << " ANALIZADOR DE FORMULAS PROPOSICIONALES" << endl;
        cout << "-----------------------------------------------" << endl;
        cout << "Conectivas: AND(&), OR(|), Implica(-), Doble Implic(=), Neg(*)" << endl;
        cout << "Símbolos proposicionales: p, q, r, s" << endl;
        cout << "Ejemplo: (p|q)&(*p=q)-*p" << endl;
        cout << "-----------------------------------------------" << endl;

        cout << "Ingrese la fórmula: ";
        cin.getline(formula, sizeof(formula) - 1);

        int tam = strlen(formula);
        for (int i = 0; i < tam; i++)
            Lista.Insertar(formula[i]);

        Lista.AnalizarFormula();

        cout << "Pulse ENTER para otra fórmula o 'x' para salir: ";
        rta = tolower(cin.get());
        cin.ignore(); // Limpiar buffer
    } while (rta != 'x');

    return 0;
}
