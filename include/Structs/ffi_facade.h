// ffi_facade.h
#ifndef FFI_FACADE_H
#define FFI_FACADE_H

#ifdef __cplusplus
extern "C" {
#endif

void clinica_init(void);
void clinica_shutdown(void);

/* Pacientes — lista en JSON, retorno con clinica_free_string */
char* clinica_listar_pacientes(void);
int   clinica_registrar_paciente(const char* nombre, const char* apep,
                                  const char* apem, const char* expediente,
                                  int dia, int mes, int anio);
int   clinica_baja_paciente(int id);
int   clinica_reactivar_paciente(int id);

/* Doctores */
char* clinica_listar_doctores(void);
int   clinica_registrar_doctor(const char* nombre, const char* apep,
                                const char* apem, const char* cedula,
                                int idEspecialidad);

/* Especialidades */
char* clinica_listar_especialidades(void);
int   clinica_registrar_especialidad(const char* nombre);

/* Citas */
char* clinica_listar_citas(void);
int   clinica_registrar_cita(int idPaciente, int idDoctor,
                              int dia, int mes, int anio, int hora, int minuto);
int   clinica_cancelar_cita(int id);
int   clinica_atender_cita(int id);

/* Cola */
char* clinica_listar_cola(void);
int   clinica_encolar(int idPaciente);
int   clinica_desencolar(void);

/* Deshacer */
int clinica_deshacer(void);

/* Liberar strings devueltos */
void clinica_free_string(char* s);

int clinica_deshacer_safe(void);
int clinica_desencolar_safe(void);
int clinica_guardar_datos(const char* ruta);
int clinica_cargar_datos(const char* ruta);
int clinica_login(const char* usuario, const char* clave);

#ifdef __cplusplus
}
#endif
#endif