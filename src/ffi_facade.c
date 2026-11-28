// ffi_facade.c
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "ffi_facade.h"
#include "pacientes.h"
#include "doctor.h"
#include "especialidades.h"
#include "cita.h"
#include "cola.h"
#include "pila.h"
#include "login.h"

static listaPacientes      g_pacs;
static listaDoctor         g_drs;
static listaEspecialidades g_esp;
static listaCitas          g_citas;
static colaEspera          g_cola;
static pilaOperaciones     g_pila;
static int                 g_iniciado = 0;

void clinica_init(void) {
    if (g_iniciado) return;
    iniciarLista(&g_pacs);
    iniciarListaDr(&g_drs);
    iniciarEspecialidad(&g_esp);
    iniciarListaCitas(&g_citas);
    iniciarCola(&g_cola);
    iniciarPila(&g_pila);
    g_iniciado = 1;
}

void clinica_shutdown(void) {
    if (!g_iniciado) return;
    liberarLista(&g_pacs);
    liberarListaDr(&g_drs);
    liberarEspecialidad(&g_esp);
    liberarListaCitas(&g_citas);
    liberarCola(&g_cola);
    liberarPila(&g_pila);
    g_iniciado = 0;
}

/* ---- Buffer dinámico mínimo ---- */
typedef struct { char* data; size_t used, cap; } SB;
static void sb_init(SB* b) { b->cap = 4096; b->used = 0; b->data = malloc(b->cap); b->data[0] = '\0'; }
static void sb_need(SB* b, size_t n) {
    while (b->used + n + 1 > b->cap) {
        b->cap *= 2;
        b->data = realloc(b->data, b->cap);
    }
}
static void sb_app(SB* b, const char* fmt, ...) {
    va_list ap; va_start(ap, fmt);
    char tmp[512];
    int n = vsnprintf(tmp, sizeof(tmp), fmt, ap);
    va_end(ap);
    if (n < 0) return;
    sb_need(b, (size_t)n);
    memcpy(b->data + b->used, tmp, (size_t)n);
    b->used += (size_t)n;
    b->data[b->used] = '\0';
}

void clinica_free_string(char* s) { free(s); }

/* ---- Pacientes ---- */
char* clinica_listar_pacientes(void) {
    SB b; sb_init(&b);
    sb_app(&b, "[");
    int first = 1;
    for (nodoPaciente* n = g_pacs.cabeza; n; n = n->siguienteNodo) {
        if (!first) sb_app(&b, ",");
        first = 0;
        sb_app(&b,
            "{\"id\":%d,\"nombre\":\"%s\",\"apellidoPaterno\":\"%s\","
            "\"apellidoMaterno\":\"%s\",\"numeroExpediente\":\"%s\","
            "\"fechaNacimiento\":{\"dia\":%d,\"mes\":%d,\"anio\":%d},"
            "\"activo\":%s}",
            n->dato.id, n->dato.nombre, n->dato.apellidoPaterno,
            n->dato.apellidoMaterno, n->dato.numeroExpediente,
            n->dato.fechaNaciemiento.dia, n->dato.fechaNaciemiento.mes,
            n->dato.fechaNaciemiento.anio,
            n->dato.activo ? "true" : "false");
    }
    sb_app(&b, "]");
    return b.data;
}

int clinica_registrar_paciente(const char* nombre, const char* apep,
                                const char* apem, const char* expediente,
                                int dia, int mes, int anio) {
    paciente p; memset(&p, 0, sizeof(p));
    snprintf(p.nombre,           sizeof(p.nombre),           "%s", nombre);
    snprintf(p.apellidoPaterno,  sizeof(p.apellidoPaterno),  "%s", apep);
    snprintf(p.apellidoMaterno,  sizeof(p.apellidoMaterno),  "%s", apem);
    snprintf(p.numeroExpediente, sizeof(p.numeroExpediente), "%s", expediente);
    p.fechaNaciemiento.dia = dia;
    p.fechaNaciemiento.mes = mes;
    p.fechaNaciemiento.anio = anio;
    int id = 0;
    return registrarPaciente(&g_pacs, p, &id);
}

int clinica_baja_paciente(int id)       { return bajaDePaciente(&g_pacs, id); }
int clinica_reactivar_paciente(int id)  { return reactivarPaciente(&g_pacs, id); }

/* ---- Doctores ---- */
char* clinica_listar_doctores(void) {
    SB b; sb_init(&b);
    sb_app(&b, "[");
    int first = 1;
    for (nodoDoctor* n = g_drs.cabeza; n; n = n->siguienteNodo) {
        if (!first) sb_app(&b, ",");
        first = 0;
        sb_app(&b,
            "{\"id\":%d,\"nombre\":\"%s\",\"apellidoPaterno\":\"%s\","
            "\"apellidoMaterno\":\"%s\",\"cedula\":\"%s\","
            "\"idEspecialidad\":%d,\"pacientesAsignados\":%d,\"activo\":%s}",
            n->dato.idDoctor, n->dato.nombre, n->dato.apellidoP,
            n->dato.appellidoM, n->dato.cedula, n->dato.idEspecialidad,
            n->dato.pacientesAsignados, n->dato.activo ? "true" : "false");
    }
    sb_app(&b, "]");
    return b.data;
}

int clinica_registrar_doctor(const char* nombre, const char* apep,
                              const char* apem, const char* cedula,
                              int idEspecialidad) {
    doctor d; memset(&d, 0, sizeof(d));
    snprintf(d.nombre,     sizeof(d.nombre),     "%s", nombre);
    snprintf(d.apellidoP,  sizeof(d.apellidoP),  "%s", apep);
    snprintf(d.appellidoM, sizeof(d.appellidoM), "%s", apem);
    snprintf(d.cedula,     sizeof(d.cedula),     "%s", cedula);
    d.idEspecialidad = idEspecialidad;
    int id = 0;
    return registrarDr(&g_drs, d, &id);
}

/* ---- Especialidades ---- */
char* clinica_listar_especialidades(void) {
    SB b; sb_init(&b);
    sb_app(&b, "[");
    int first = 1;
    for (nodoEspecialidad* n = g_esp.cabeza; n; n = n->siguienteNodo) {
        if (!first) sb_app(&b, ",");
        first = 0;
        sb_app(&b, "{\"id\":%d,\"nombre\":\"%s\"}",
               n->dato.id, n->dato.especialidadN);
    }
    sb_app(&b, "]");
    return b.data;
}

int clinica_registrar_especialidad(const char* nombre) {
    especialidad e; memset(&e, 0, sizeof(e));
    snprintf(e.especialidadN, sizeof(e.especialidadN), "%s", nombre);
    return registrarEspecialidad(&g_esp, e);
}

/* ---- Citas ---- */
char* clinica_listar_citas(void) {
    SB b; sb_init(&b);
    sb_app(&b, "[");
    int first = 1;
    for (nodoCita* n = g_citas.cabeza; n; n = n->siguienteNodo) {
        if (!first) sb_app(&b, ",");
        first = 0;
        sb_app(&b,
            "{\"id\":%d,\"idPaciente\":%d,\"idDoctor\":%d,"
            "\"fecha\":{\"dia\":%d,\"mes\":%d,\"anio\":%d},"
            "\"hora\":%d,\"minuto\":%d,\"estado\":%d}",
            n->dato.id, n->dato.idPaciente, n->dato.idDoctor,
            n->dato.fechaCita.dia, n->dato.fechaCita.mes, n->dato.fechaCita.anio,
            n->dato.hora, n->dato.minuto, (int)n->dato.estado);
    }
    sb_app(&b, "]");
    return b.data;
}

int clinica_registrar_cita(int idPaciente, int idDoctor,
                            int dia, int mes, int anio, int hora, int minuto) {
    cita c; memset(&c, 0, sizeof(c));
    c.idPaciente = idPaciente;
    c.idDoctor   = idDoctor;
    c.fechaCita.dia = dia; c.fechaCita.mes = mes; c.fechaCita.anio = anio;
    c.hora = hora; c.minuto = minuto;
    int id = 0;
    return registrarCita(&g_citas, &g_pacs, &g_drs, c, &id);
}

int clinica_cancelar_cita(int id) { return cancelarCita(&g_citas, &g_drs, id); }
int clinica_atender_cita(int id)  { return atenderCita(&g_citas, &g_drs, id); }

/* ---- Cola ---- */
char* clinica_listar_cola(void) {
    SB b; sb_init(&b);
    sb_app(&b, "[");
    int first = 1;
    for (nodoCola* n = g_cola.frente; n; n = n->siguienteNodo) {
        if (!first) sb_app(&b, ",");
        first = 0;
        sb_app(&b, "%d", n->idPaciente);
    }
    sb_app(&b, "]");
    return b.data;
}
int clinica_encolar(int idPaciente) { return encolar(&g_cola, idPaciente); }
int clinica_desencolar(void) {
    int id = -1;
    if (desencolar(&g_cola, &id) != COLA_OK) return -1;
    return id;
}

/* ---- Deshacer ---- */
/* --- Wrappers limpios para Rust --- */
int clinica_deshacer_safe(void) {
    return deshacerUltima(&g_pila, &g_pacs);
}

int clinica_desencolar_safe(void) {
    int id = -1;
    if (desencolar(&g_cola, &id) != COLA_OK) return -1;
    return id;
}

/* ==========================================
   PERSISTENCIA DE DATOS
   ========================================== */
#include <stdio.h>

/* ==========================================
   PERSISTENCIA DE DATOS CORREGIDA
   ========================================== */
int clinica_guardar_datos(const char* ruta) {
    FILE* f = fopen(ruta, "wb");
    if (!f) return 0;

    // Guardar Pacientes (Cantidad + Datos)
    int n_pacs = g_pacs.cantidad;
    fwrite(&n_pacs, sizeof(int), 1, f);
    for (nodoPaciente* n = g_pacs.cabeza; n; n = n->siguienteNodo)
        fwrite(&n->dato, sizeof(paciente), 1, f);
    
    // Guardar Doctores
    int n_drs = g_drs.cantidad;
    fwrite(&n_drs, sizeof(int), 1, f);
    for (nodoDoctor* n = g_drs.cabeza; n; n = n->siguienteNodo)
        fwrite(&n->dato, sizeof(doctor), 1, f);
        
    // Guardar Especialidades
    int n_esp = g_esp.cantidad;
    fwrite(&n_esp, sizeof(int), 1, f);
    for (nodoEspecialidad* n = g_esp.cabeza; n; n = n->siguienteNodo)
        fwrite(&n->dato, sizeof(especialidad), 1, f);

    // Guardar Citas
    int n_citas = g_citas.cantidad;
    fwrite(&n_citas, sizeof(int), 1, f);
    for (nodoCita* n = g_citas.cabeza; n; n = n->siguienteNodo)
        fwrite(&n->dato, sizeof(cita), 1, f);

    fclose(f);
    return 1;
}

int clinica_cargar_datos(const char* ruta) {
    FILE* f = fopen(ruta, "rb");
    if (!f) return 0;

    int n;
    
    // Cargar Pacientes
    if (fread(&n, sizeof(int), 1, f) == 1) {
        for (int i = 0; i < n; i++) {
            paciente p;
            fread(&p, sizeof(paciente), 1, f);
            nodoPaciente* nuevo = malloc(sizeof(nodoPaciente));
            nuevo->dato = p;
            nuevo->siguienteNodo = NULL;
            if (!g_pacs.cabeza) g_pacs.cabeza = nuevo;
            else g_pacs.cola->siguienteNodo = nuevo;
            g_pacs.cola = nuevo;
            g_pacs.cantidad++;
            if (p.id >= g_pacs.sigId) g_pacs.sigId = p.id + 1;
        }
    }

    // Cargar Doctores
    if (fread(&n, sizeof(int), 1, f) == 1) {
        for (int i = 0; i < n; i++) {
            doctor d;
            fread(&d, sizeof(doctor), 1, f);
            nodoDoctor* nuevo = malloc(sizeof(nodoDoctor));
            nuevo->dato = d;
            nuevo->siguienteNodo = NULL;
            if (!g_drs.cabeza) g_drs.cabeza = nuevo;
            else g_drs.cola->siguienteNodo = nuevo;
            g_drs.cola = nuevo;
            g_drs.cantidad++;
            if (d.idDoctor >= g_drs.sigId) g_drs.sigId = d.idDoctor + 1;
        }
    }

    // Cargar Especialidades
    if (fread(&n, sizeof(int), 1, f) == 1) {
        for (int i = 0; i < n; i++) {
            especialidad e;
            fread(&e, sizeof(especialidad), 1, f);
            nodoEspecialidad* nuevo = malloc(sizeof(nodoEspecialidad));
            nuevo->dato = e;
            nuevo->siguienteNodo = NULL;
            if (!g_esp.cabeza) g_esp.cabeza = nuevo;
            else g_esp.cola->siguienteNodo = nuevo;
            g_esp.cola = nuevo;
            g_esp.cantidad++;
            if (e.id >= g_esp.sigId) g_esp.sigId = e.id + 1;
        }
    }

    // Cargar Citas
    if (fread(&n, sizeof(int), 1, f) == 1) {
        for (int i = 0; i < n; i++) {
            cita c;
            fread(&c, sizeof(cita), 1, f);
            nodoCita* nuevo = malloc(sizeof(nodoCita));
            nuevo->dato = c;
            nuevo->siguienteNodo = NULL;
            if (!g_citas.cabeza) g_citas.cabeza = nuevo;
            else g_citas.cola->siguienteNodo = nuevo;
            g_citas.cola = nuevo;
            g_citas.cantidad++;
            if (c.id >= g_citas.sigId) g_citas.sigId = c.id + 1;
        }
    }

    fclose(f);
    return 1;
}

/* ==========================================
   LOGIN
   ========================================== */
static int g_intentos_login = 0;

int clinica_login(const char* usuario, const char* clave) {
    // Credenciales fijas para el sistema
    const char* user_correcto = "admin123";
    const char* pass_correcta = "admin1234";

    int res_user = ingresarUsuario(usuario, user_correcto);
    if (res_user != LOGIN_EXITOSO) {
        // Si el usuario está mal, simulamos error de credenciales
        return 3; // ERROR_CREDENCIALES
    }

    // Llamamos a tu función de login.c que maneja los intentos
    int res_pass = ingresarClave(clave, pass_correcta, &g_intentos_login);
    
    // res_pass devolverá 1 (LOGIN_EXITOSO), 2 (FORMATO), 3 (CREDENCIALES) o 4 (BLOQUEADO)
    return res_pass;
}