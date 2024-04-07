#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Principio de Responsabilidad Única (SRP)
// La clase Comentario tiene la única responsabilidad de representar un comentario.
class Comentario {
private:
    string contenido;

public:
    Comentario(const string& contenido) : contenido(contenido) {}

    string obtenerContenido() const {
        return contenido;
    }
};

// Principio de Responsabilidad Única (SRP)
// La clase Proyecto tiene múltiples responsabilidades:
// - Representa un proyecto.
// - Almacena comentarios asociados al proyecto.
// - Gestiona el estado del proyecto.
class Proyecto {
private:
    string nombre;
    string descripcion;
    string fechaLimite;
    bool completada;
    static const int MAX_COMENTARIOS = 100;
    Comentario* comentarios[MAX_COMENTARIOS];
    int numComentarios;

public:
    Proyecto(const string& nombre, const string& descripcion, const string& fechaLimite)
        : nombre(nombre), descripcion(descripcion), fechaLimite(fechaLimite), completada(false), numComentarios(0) {}

    void marcarComoCompletada() {
        completada = true;
    }

    bool estaCompletada() const {
        return completada;
    }

    string obtenerNombre() const {
        return nombre;
    }

    string obtenerDescripcion() const {
        return descripcion;
    }

    string obtenerFechaLimite() const {
        return fechaLimite;
    }

    int obtenerNumComentarios() const {
        return numComentarios;
    }

    string obtenerComentario(int indice) const {
        return comentarios[indice]->obtenerContenido();
    }

    void agregarComentario(const string& contenido) {
        if (numComentarios < MAX_COMENTARIOS) {
            comentarios[numComentarios++] = new Comentario(contenido);
        } else {
            cout << "No se pueden agregar más comentarios." << endl;
        }
    }
};

// Principio de Segregación de Interfaces (ISP)
// La clase ProyectoManager tiene múltiples responsabilidades agrupadas en una sola interfaz.
// Sería mejor dividir las funcionalidades relacionadas con la gestión de proyectos en interfaces más pequeñas y específicas.
class ProyectoManager {
private:
    static const int MAX_PROYECTOS = 100;
    Proyecto* proyectos[MAX_PROYECTOS];
    int numProyectos;

public:
    ProyectoManager() : numProyectos(0) {}

    ~ProyectoManager() {
        for (int i = 0; i < numProyectos; ++i) {
            delete proyectos[i];
        }
    }

    void agregarProyecto(Proyecto* proyecto) {
        if (numProyectos < MAX_PROYECTOS) {
            proyectos[numProyectos++] = proyecto;
            cout << "Proyecto agregado exitosamente." << endl;
        } else {
            cout << "No se pueden agregar más proyectos." << endl;
        }
    }

    void mostrarProyectos() const {
        cout << "Lista de proyectos:" << endl;
        for (int i = 0; i < numProyectos; ++i) {
            cout << i + 1 << ". ";
            cout << "Nombre: " << proyectos[i]->obtenerNombre() << endl;
            cout << "   Descripción: " << proyectos[i]->obtenerDescripcion() << endl;
            cout << "   Fecha límite: " << proyectos[i]->obtenerFechaLimite() << endl;
            cout << "   Estado: " << (proyectos[i]->estaCompletada() ? "Completada" : "Pendiente") << endl;
            if (proyectos[i]->obtenerNumComentarios() > 0) {
                cout << "   Comentarios:" << endl;
                for (int j = 0; j < proyectos[i]->obtenerNumComentarios(); ++j) {
                    cout << "      - " << proyectos[i]->obtenerComentario(j) << endl;
                }
            }
            cout << "-------------------" << endl;
        }
    }

    Proyecto* seleccionarProyecto() const {
        cout << "Seleccione un proyecto:" << endl;
        mostrarProyectos();
        int opcion;
        cout << "Ingrese el número del proyecto: ";
        cin >> opcion;
        cin.ignore();
        if (opcion >= 1 && opcion <= numProyectos) {
            return proyectos[opcion - 1];
        } else {
            cout << "Número de proyecto no válido." << endl;
            return nullptr;
        }
    }

    void eliminarProyecto(Proyecto* proyecto) {
        for (int i = 0; i < numProyectos; ++i) {
            if (proyectos[i] == proyecto) {
                delete proyectos[i];
                for (int j = i; j < numProyectos - 1; ++j) {
                    proyectos[j] = proyectos[j + 1];
                }
                numProyectos--;
                cout << "Proyecto eliminado exitosamente." << endl;
                return;
            }
        }
        cout << "Proyecto no encontrado." << endl;
    }

    void marcarComoCompletado(Proyecto* proyecto) {
        proyecto->marcarComoCompletada();
        cout << "Proyecto marcado como completado." << endl;
    }

    void agregarComentario(Proyecto* proyecto) {
        string contenido;
        cout << "Ingrese el contenido del comentario: ";
        getline(cin, contenido);
        proyecto->agregarComentario(contenido);
        cout << "Comentario agregado exitosamente." << endl;
    }

    void exportarDatos(const string& nombreArchivo) const {
        ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            cout << "No se pudo abrir el archivo '" << nombreArchivo << "'." << endl;
            return;
        }

        archivo << "Lista de proyectos:" << endl;
        for (int i = 0; i < numProyectos; ++i) {
            archivo << i + 1 << ". ";
            archivo << "Nombre: " << proyectos[i]->obtenerNombre() << endl;
            archivo << "   Descripción: " << proyectos[i]->obtenerDescripcion() << endl;
            archivo << "   Fecha límite: " << proyectos[i]->obtenerFechaLimite() << endl;
            archivo << "   Estado: " << (proyectos[i]->estaCompletada() ? "Completada" : "Pendiente") << endl;
            if (proyectos[i]->obtenerNumComentarios() > 0) {
                archivo << "   Comentarios:" << endl;
                for (int j = 0; j < proyectos[i]->obtenerNumComentarios(); ++j) {
                    archivo << "      - " << proyectos[i]->obtenerComentario(j) << endl;
                }
            }
            archivo << "-------------------" << endl;
        }

        archivo.close();
        cout << "Datos exportados exitosamente al archivo '" << nombreArchivo << "'." << endl;
    }
};

// Funciones de presentación
void mostrarMenuPrincipal() {
    cout << "------ Menú ------" << endl;
    cout << "1. Crear proyecto" << endl;
    cout << "2. Gestionar proyectos" << endl;
    cout << "3. Ver proyectos" << endl;
    cout << "4. Exportar datos a archivo" << endl;
    cout << "5. Salir" << endl;
    cout << "-------------------" << endl;
    cout << "Ingrese su opción: ";
}

void mostrarMenuGestionProyectos() {
    cout << "------ Menú Gestionar Proyectos ------" << endl;
    cout << "1. Eliminar proyecto" << endl;
    cout << "2. Marcar como completado" << endl;
    cout << "3. Agregar comentario" << endl;
    cout << "4. Volver al menú principal" << endl;
    cout << "-------------------" << endl;
    cout << "Ingrese su opción: ";
}

string obtenerFechaHora() {
    string fechaHora;
    cout << "Ingrese la fecha y hora del proyecto (YYYY-MM-DD HH:MM): ";
    getline(cin, fechaHora);
    return fechaHora;
}

int main() {
    ProyectoManager proyectoManager;

    int opcion;
    do {
        mostrarMenuPrincipal();
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1: {
                string nombre, descripcion, fechaLimite;
                cout << "Ingrese el nombre del proyecto: ";
                getline(cin, nombre);
                cout << "Ingrese la descripción del proyecto: ";
                getline(cin, descripcion);
                fechaLimite = obtenerFechaHora();
                proyectoManager.agregarProyecto(new Proyecto(nombre, descripcion, fechaLimite));
                break;
            }
            case 2: {
                int subopcion;
                do {
                    mostrarMenuGestionProyectos();
                    cin >> subopcion;
                    cin.ignore();

                    switch (subopcion) {
                        case 1: {
                            Proyecto* proyecto = proyectoManager.seleccionarProyecto();
                            if (proyecto) {
                                proyectoManager.eliminarProyecto(proyecto);
                            }
                            break;
                        }
                        case 2: {
                            Proyecto* proyecto = proyectoManager.seleccionarProyecto();
                            if (proyecto) {
                                proyectoManager.marcarComoCompletado(proyecto);
                            }
                            break;
                        }
                        case 3: {
                            Proyecto* proyecto = proyectoManager.seleccionarProyecto();
                            if (proyecto) {
                                proyectoManager.agregarComentario(proyecto);
                            }
                            break;
                        }
                        case 4:
                            cout << "Volviendo al menú principal..." << endl;
                            break;
                        default:
                            cout << "Opción no válida. Por favor, ingrese una opción válida." << endl;
                            break;
                    }
                } while (subopcion != 4);
                break;
            }
            case 3:
                proyectoManager.mostrarProyectos();
                break;
            case 4: {
                string nombreArchivo;
                cout << "Ingrese el nombre del archivo para exportar los datos (por ejemplo, datos_proyectos.txt): ";
                getline(cin, nombreArchivo);
                proyectoManager.exportarDatos(nombreArchivo);
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
