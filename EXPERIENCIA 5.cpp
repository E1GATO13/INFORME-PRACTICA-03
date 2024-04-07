#include <iostream>
#include <string>

using namespace std;

// Principio de Responsabilidad Única (SRP)
// La clase Tarea tiene la responsabilidad de representar una tarea y su estado.
class Tarea {
protected:
    string nombre;
    string descripcion;
    string fechaLimite;
    bool completada;

public:
    Tarea(const string& nombre, const string& descripcion, const string& fechaLimite)
        : nombre(nombre), descripcion(descripcion), fechaLimite(fechaLimite), completada(false) {}

    virtual void marcarComoCompletada() {
        completada = true;
    }

    bool estaCompletada() const {
        return completada;
    }

    virtual void mostrarInformacion(int indice) const {
        cout << indice + 1 << ". ";
        cout << "Nombre: " << nombre << endl;
        cout << "   Descripción: " << descripcion << endl;
        cout << "   Fecha límite: " << fechaLimite << endl;
        cout << "   Estado: " << (completada ? "Completada" : "Pendiente") << endl;
    }
};

// Principio Abierto/Cerrado (OCP)
// La clase GestionTareas está abierta para extensión (mediante herencia de Tarea)
// y cerrada para modificación (no se modifican las clases existentes al agregar nuevos tipos de Tarea).
class TareaSimple : public Tarea {
public:
    TareaSimple(const string& nombre, const string& descripcion, const string& fechaLimite)
        : Tarea(nombre, descripcion, fechaLimite) {}
};

class TareaRecurrente : public Tarea {
private:
    string frecuencia;

public:
    TareaRecurrente(const string& nombre, const string& descripcion, const string& fechaLimite, const string& frecuencia)
        : Tarea(nombre, descripcion, fechaLimite), frecuencia(frecuencia) {}

    void mostrarInformacion(int indice) const override {
        cout << indice + 1 << ". ";
        cout << "Nombre: " << nombre << endl;
        cout << "   Descripción: " << descripcion << endl;
        cout << "   Fecha límite: " << fechaLimite << endl;
        cout << "   Frecuencia: " << frecuencia << endl;
        cout << "   Estado: " << (completada ? "Completada" : "Pendiente") << endl;
    }
};

// Principio de Segregación de Interfaces (ISP)
// La interfaz ITareaManager define solo los métodos necesarios para la gestión de tareas.
class ITareaManager {
public:
    virtual void agregarTarea(Tarea* tarea) = 0;
    virtual void mostrarTareas() const = 0;
    virtual void marcarTareaComoCompletada(int indice) = 0;
};

// Principio de Inversión de Dependencia (DIP)
// GestionTareas depende de la interfaz ITareaManager en lugar de clases concretas de Tarea.
class GestionTareas : public ITareaManager {
private:
    static const int MAX_TAREAS = 10000;
    Tarea* tareas[MAX_TAREAS];
    int numTareas;

public:
    GestionTareas() {
        numTareas = 0;
    }

    void agregarTarea(Tarea* tarea) override {
        if (numTareas < MAX_TAREAS) {
            tareas[numTareas++] = tarea;
            cout << "Tarea agregada exitosamente." << endl;
        } else {
            cout << "No se pueden agregar más tareas." << endl;
        }
    }

    void mostrarTareas() const override {
        cout << "Lista de tareas:" << endl;
        for (int i = 0; i < numTareas; ++i) {
            tareas[i]->mostrarInformacion(i);
            cout << "-------------------" << endl;
        }
    }

    void marcarTareaComoCompletada(int indice) override {
        if (indice >= 0 && indice < numTareas) {
            tareas[indice]->marcarComoCompletada();
            cout << "Tarea marcada como completada." << endl;
        } else {
            cout << "Índice de tarea no válido." << endl;
        }
    }
};

void mostrarMenu() {
    cout << "------ Menú ------" << endl;
    cout << "1. Crear tarea simple" << endl;
    cout << "2. Crear tarea recurrente" << endl;
    cout << "3. Ver tareas" << endl;
    cout << "4. Marcar tarea como completada" << endl;
    cout << "5. Salir" << endl;
    cout << "-------------------" << endl;
    cout << "Ingrese su opción: ";
}

string obtenerFechaHora() {
    string fechaHora;
    cout << "Ingrese la fecha y hora de la tarea (YYYY-MM-DD HH:MM): ";
    getline(cin, fechaHora);
    return fechaHora;
}

int main() {
    GestionTareas gestion;

    int opcion;
    do {
        mostrarMenu();
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1: {
                string nombre, descripcion, fechaLimite;
                cout << "Ingrese el nombre de la tarea: ";
                getline(cin, nombre);
                cout << "Ingrese la descripción de la tarea: ";
                getline(cin, descripcion);
                fechaLimite = obtenerFechaHora();
                gestion.agregarTarea(new TareaSimple(nombre, descripcion, fechaLimite));
                break;
            }
            case 2: {
                string nombre, descripcion, fechaLimite, frecuencia;
                cout << "Ingrese el nombre de la tarea: ";
                getline(cin, nombre);
                cout << "Ingrese la descripción de la tarea: ";
                getline(cin, descripcion);
                fechaLimite = obtenerFechaHora();
                cout << "Ingrese la frecuencia de la tarea: ";
                getline(cin, frecuencia);
                gestion.agregarTarea(new TareaRecurrente(nombre, descripcion, fechaLimite, frecuencia));
                break;
            }
            case 3:
                gestion.mostrarTareas();
                break;
            case 4: {
                int indice;
                cout << "Ingrese el índice de la tarea que desea marcar como completada: ";
                cin >> indice;
                cin.ignore();
                gestion.marcarTareaComoCompletada(indice - 1);
                break;
            }
            case 5:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                cout << "Opción no válida. Por favor, ingrese una opción válida." << endl;
                break;
        }
    } while (opcion != 5);

    return 0;
}
