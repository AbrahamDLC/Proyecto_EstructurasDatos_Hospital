// src-tauri/src/main.rs
#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

use libloading::{Library, Symbol};
use std::ffi::{CStr, CString};
use std::sync::Mutex;
use tauri::State;
use tauri::Manager;
use tauri::path::BaseDirectory;

// Tipos de las funciones del dylib
type FnLogin = unsafe extern "C" fn(*const i8, *const i8) -> i32;
type FnVoid    = unsafe extern "C" fn();
type FnStr     = unsafe extern "C" fn() -> *mut i8;
type FnFree    = unsafe extern "C" fn(*mut i8);
type FnIntInt  = unsafe extern "C" fn(i32) -> i32;
type FnStrInt  = unsafe extern "C" fn(*const i8) -> i32;
type FnRegPac  = unsafe extern "C" fn(*const i8,*const i8,*const i8,*const i8,i32,i32,i32) -> i32;
type FnRegDoc  = unsafe extern "C" fn(*const i8,*const i8,*const i8,*const i8,i32) -> i32;
type FnRegCita = unsafe extern "C" fn(i32,i32,i32,i32,i32,i32,i32) -> i32;
type FnPathInt = unsafe extern "C" fn(*const i8) -> i32;
type FnIntVoid = unsafe extern "C" fn() -> i32;


struct Clinic { lib: Library }

impl Clinic {
    fn new(app: &tauri::AppHandle) -> Self {
        // Detecta el sistema operativo para elegir la librería correcta
        let lib_name = if cfg!(target_os = "windows") {
            "libclinica.dll"
        } else if cfg!(target_os = "macos") {
            "libclinica.dylib"
        } else {
            "libclinica.so" // Para Linux
        };

        let path = app.path().resolve(lib_name, BaseDirectory::Resource)
            .expect("No se pudo encontrar la ruta de la librería dinámica");
            
        let lib = unsafe { Library::new(&path) }
            .expect(&format!("No se pudo cargar la librería: {}", lib_name));
            
        unsafe {
            let init: Symbol<FnVoid> = lib.get(b"clinica_init\0").unwrap();
            init();
            
            let home = std::env::var("HOME").unwrap_or_else(|_| ".".to_string());
            let db_path = format!("{}/clinica.db", home);
            let ruta = cstr(&db_path);
            
            let cargar: Symbol<FnPathInt> = lib.get(b"clinica_cargar_datos\0").unwrap();
            cargar(ruta.as_ptr());
        }
        Self { lib }
    }
}


fn cstr(s: &str) -> CString { CString::new(s).unwrap() }

macro_rules! call_str {
    ($lib:expr, $name:literal) => {{
        unsafe {
            let f: Symbol<FnStr> = $lib.get($name).unwrap();
            let free: Symbol<FnFree> = $lib.get(b"clinica_free_string\0").unwrap();
            let p = f();
            if p.is_null() { return "[]".to_string(); }
            let s = CStr::from_ptr(p).to_string_lossy().into_owned();
            free(p);
            s
        }
    }};
}

// --- Comandos expuestos al frontend ---

#[tauri::command]
fn listar_pacientes(state: State<'_, Mutex<Clinic>>) -> String {
    let st = state.lock().unwrap();
    call_str!(st.lib, b"clinica_listar_pacientes\0")
}

#[tauri::command]
fn registrar_paciente(
    state: State<'_, Mutex<Clinic>>,
    nombre: String, apep: String, apem: String, expediente: String,
    dia: i32, mes: i32, anio: i32,
) -> i32 {
    let st = state.lock().unwrap();
    let (n, ap, am, ex) = (cstr(&nombre), cstr(&apep), cstr(&apem), cstr(&expediente));
    unsafe {
        let f: Symbol<FnRegPac> = st.lib.get(b"clinica_registrar_paciente\0").unwrap();
        f(n.as_ptr(), ap.as_ptr(), am.as_ptr(), ex.as_ptr(), dia, mes, anio)
    }
}

#[tauri::command]
fn baja_paciente(state: State<'_, Mutex<Clinic>>, id: i32) -> i32 {
    let st = state.lock().unwrap();
    unsafe { let f: Symbol<FnIntInt> = st.lib.get(b"clinica_baja_paciente\0").unwrap(); f(id) }
}

#[tauri::command]
fn reactivar_paciente(state: State<'_, Mutex<Clinic>>, id: i32) -> i32 {
    let st = state.lock().unwrap();
    unsafe { let f: Symbol<FnIntInt> = st.lib.get(b"clinica_reactivar_paciente\0").unwrap(); f(id) }
}

#[tauri::command]
fn listar_doctores(state: State<'_, Mutex<Clinic>>) -> String {
    let st = state.lock().unwrap();
    call_str!(st.lib, b"clinica_listar_doctores\0")
}

#[tauri::command]
fn registrar_doctor(state: State<'_, Mutex<Clinic>>, nombre: String, apep: String,
                    apem: String, cedula: String, id_especialidad: i32) -> i32 {
    let st = state.lock().unwrap();
    let (n, ap, am, ce) = (cstr(&nombre), cstr(&apep), cstr(&apem), cstr(&cedula));
    unsafe {
        let f: Symbol<FnRegDoc> = st.lib.get(b"clinica_registrar_doctor\0").unwrap();
        f(n.as_ptr(), ap.as_ptr(), am.as_ptr(), ce.as_ptr(), id_especialidad)
    }
}

#[tauri::command]
fn listar_especialidades(state: State<'_, Mutex<Clinic>>) -> String {
    let st = state.lock().unwrap();
    call_str!(st.lib, b"clinica_listar_especialidades\0")
}

#[tauri::command]
fn registrar_especialidad(state: State<'_, Mutex<Clinic>>, nombre: String) -> i32 {
    let st = state.lock().unwrap();
    let n = cstr(&nombre);
    unsafe { let f: Symbol<FnStrInt> = st.lib.get(b"clinica_registrar_especialidad\0").unwrap(); f(n.as_ptr()) }
}

#[tauri::command]
fn listar_citas(state: State<'_, Mutex<Clinic>>) -> String {
    let st = state.lock().unwrap();
    call_str!(st.lib, b"clinica_listar_citas\0")
}

#[tauri::command]
fn registrar_cita(state: State<'_, Mutex<Clinic>>, id_paciente: i32, id_doctor: i32,
                  dia: i32, mes: i32, anio: i32, hora: i32, minuto: i32) -> i32 {
    let st = state.lock().unwrap();
    unsafe {
        let f: Symbol<FnRegCita> = st.lib.get(b"clinica_registrar_cita\0").unwrap();
        f(id_paciente, id_doctor, dia, mes, anio, hora, minuto)
    }
}

#[tauri::command]
fn cancelar_cita(state: State<'_, Mutex<Clinic>>, id: i32) -> i32 {
    let st = state.lock().unwrap();
    unsafe { let f: Symbol<FnIntInt> = st.lib.get(b"clinica_cancelar_cita\0").unwrap(); f(id) }
}

#[tauri::command]
fn atender_cita(state: State<'_, Mutex<Clinic>>, id: i32) -> i32 {
    let st = state.lock().unwrap();
    unsafe { let f: Symbol<FnIntInt> = st.lib.get(b"clinica_atender_cita\0").unwrap(); f(id) }
}

#[tauri::command]
fn listar_cola(state: State<'_, Mutex<Clinic>>) -> String {
    let st = state.lock().unwrap();
    call_str!(st.lib, b"clinica_listar_cola\0")
}
#[tauri::command]
fn encolar(state: State<'_, Mutex<Clinic>>, id: i32) -> i32 {
    let st = state.lock().unwrap();
    unsafe { let f: Symbol<FnIntInt> = st.lib.get(b"clinica_encolar\0").unwrap(); f(id) }
}
#[tauri::command]
fn desencolar(state: State<'_, Mutex<Clinic>>) -> i32 {
    let st = state.lock().unwrap();
    unsafe { 
        // Usamos la función _safe que creamos en C
        let f: Symbol<FnIntVoid> = st.lib.get(b"clinica_desencolar_safe\0").unwrap(); 
        f() 
    }
}

#[tauri::command]
fn deshacer(state: State<'_, Mutex<Clinic>>) -> i32 {
    let st = state.lock().unwrap();
    unsafe { 
        // Usamos la función _safe que creamos en C
        let f: Symbol<FnIntVoid> = st.lib.get(b"clinica_deshacer_safe\0").unwrap(); 
        f() 
    }
}

#[tauri::command]
fn guardar_datos(state: State<'_, Mutex<Clinic>>) -> i32 {
    let st = state.lock().unwrap();
    let home = std::env::var("HOME").unwrap_or_else(|_| ".".to_string());
    let db_path = format!("{}/clinica.db", home);
    let ruta = cstr(&db_path);
    
    unsafe {
        let f: Symbol<FnPathInt> = st.lib.get(b"clinica_guardar_datos\0").unwrap();
        return f(ruta.as_ptr());
    }
}

// ¡AHORA SÍ, LOGIN ESTÁ AFUERA!
#[tauri::command]
fn login(state: State<'_, Mutex<Clinic>>, usuario: String, clave: String) -> i32 {
    let st = state.lock().unwrap();
    let u = cstr(&usuario);
    let c = cstr(&clave);
    unsafe {
        let f: Symbol<FnLogin> = st.lib.get(b"clinica_login\0").unwrap();
        return f(u.as_ptr(), c.as_ptr());
    }
}

fn main() {
    tauri::Builder::default()
    .setup(|app| {
        let clinic = Clinic::new(app.handle());
        app.manage(Mutex::new(clinic));
        Ok(())
    })
        .invoke_handler(tauri::generate_handler![
            listar_pacientes, 
            registrar_paciente, 
            baja_paciente, 
            reactivar_paciente,
            listar_doctores, 
            registrar_doctor,
            listar_especialidades, 
            registrar_especialidad,
            listar_citas, 
            registrar_cita, 
            cancelar_cita, 
            atender_cita,
            listar_cola, 
            encolar, 
            desencolar, 
            deshacer, 
            guardar_datos,
            login
        ])
        .run(tauri::generate_context!())
        .expect("error al iniciar Tauri");
}