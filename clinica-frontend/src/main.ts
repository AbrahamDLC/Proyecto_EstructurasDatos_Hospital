import "./styles.css";
import { invoke } from "@tauri-apps/api/core";

const app = document.querySelector("#app") as HTMLDivElement;

const errMsg = (code: number): string => {
    const m: Record<number, string> = {
        100: "Error de memoria",
        1:   "OK",
        3:   "Nombre inválido",
        4:   "Nombre inválido",
        5:   "Apellido inválido",
        6:   "Expediente inválido (PS-XXX-XXX)",
        7:   "Fecha inválida",
        8:   "Paciente duplicado",
        600: "Fecha de cita inválida",
        601: "Hora inválida",
        602: "Paciente no encontrado",
        603: "Doctor no encontrado",
        604: "Conflicto de horario con doctor",
        605: "Conflicto de horario con paciente",
        606: "Cita no encontrada",
        0:   "Especialidad inválida",
        2:   "La especialidad ya existe",
        
    };
    return m[code] || `Error ${code}`;
};

// --- PACIENTES ---
//============================================================================
async function renderPacientes() {
    app.innerHTML = `
        <h2>Gestión de Pacientes</h2>
        <div class="form">
            <input id="p-nombre" placeholder="Nombre(s)" />
            <input id="p-apep" placeholder="Apellido Paterno" />
            <input id="p-apem" placeholder="Apellido Materno" />
            <input id="p-exp" placeholder="Expediente (PS-001-001)" />
            <input id="p-dia" type="number" placeholder="Día de nacimiento" min="1" max="31" />
            <input id="p-mes" type="number" placeholder="Mes de nacimiento" min="1" max="12" />
            <input id="p-anio" type="number" placeholder="Año de nacimiento" min="1900" max="2026" />
            <button id="p-btn">Registrar Paciente</button>
        </div>
        <div id="p-msg"></div>
        <div class="table-container">
            <table>
                <thead><tr><th>ID</th><th>Nombre Completo</th><th>Expediente</th><th>Nacimiento</th><th>Estado</th><th>Acciones</th></tr></thead>
                <tbody id="p-tbody"></tbody>
            </table>
        </div>
    `;

    const refresh = async () => {
        await invoke("guardar_datos");
        const raw: string = await invoke("listar_pacientes");
        const pacs: any[] = JSON.parse(raw);
        const tbody = document.querySelector("#p-tbody") as HTMLTableSectionElement;
        
        if (pacs.length === 0) {
            tbody.innerHTML = `<tr><td colspan="6" style="text-align:center; color:#86868b; padding: 32px;">No hay pacientes registrados.</td></tr>`;
            return;
        }

        tbody.innerHTML = pacs.map((p: any) => `
            <tr>
                <td><strong>${p.id}</strong></td>
                <td>${p.nombre} ${p.apellidoPaterno} ${p.apellidoMaterno}</td>
                <td>${p.numeroExpediente}</td>
                <td>${p.fechaNacimiento.dia}/${p.fechaNacimiento.mes}/${p.fechaNacimiento.anio}</td>
                <td>
                    <span class="badge ${p.activo ? 'active' : 'inactive'}">
                        ${p.activo ? "Activo" : "Inactivo"}
                    </span>
                </td>
                <td class="actions">
                    ${p.activo
                        ? `<button class="danger" data-baja="${p.id}">Dar de baja</button>`
                        : `<button class="ok" data-react="${p.id}">Reactivar</button>`}
                </td>
            </tr>
        `).join("");
        
        tbody.querySelectorAll("[data-baja]").forEach((b: Element) => {
            const btn = b as HTMLButtonElement;
            btn.onclick = async () => {
                await invoke("baja_paciente", { id: Number(btn.dataset.baja) });
                refresh();
            };
        });

        tbody.querySelectorAll("[data-react]").forEach((b: Element) => {
            const btn = b as HTMLButtonElement;
            btn.onclick = async () => {
                await invoke("reactivar_paciente", { id: Number(btn.dataset.react) });
                refresh();
            };
        });
    };

    const btnRegistrar = document.querySelector("#p-btn") as HTMLButtonElement;
    btnRegistrar.onclick = async () => {
        const val = (id: string): string => (document.querySelector(id) as HTMLInputElement).value.trim();
        
        const code: number = await invoke("registrar_paciente", {
            nombre: val("#p-nombre"),
            apep: val("#p-apep"),
            apem: val("#p-apem"),
            expediente: val("#p-exp"),
            dia: Number(val("#p-dia")),
            mes: Number(val("#p-mes")),
            anio: Number(val("#p-anio")),
        });

        const msg = document.querySelector("#p-msg") as HTMLDivElement;
        if (code === 1) {
            msg.innerHTML = `<div class="msg ok">Paciente registrado exitosamente.</div>`;
            ["#p-nombre","#p-apep","#p-apem","#p-exp","#p-dia","#p-mes","#p-anio"]
                .forEach((id: string) => (document.querySelector(id) as HTMLInputElement).value = "");
            refresh();
            setTimeout(() => msg.innerHTML = "", 3000);
        } else {
            msg.innerHTML = `<div class="msg err">${errMsg(code)}</div>`;
        }
    };

    refresh();
}

// --- Esqueletos para el resto ---
async function renderDoctores() {
    app.innerHTML = `
        <h2>Gestión de Doctores</h2>
        <div class="form">
            <input id="d-nombre" placeholder="Nombre(s)" />
            <input id="d-apep" placeholder="Apellido Paterno" />
            <input id="d-apem" placeholder="Apellido Materno" />
            <input id="d-cedula" placeholder="Cédula (8 o 9 dígitos)" />
            <select id="d-especialidad" class="form-select">
                <option value="">Cargando especialidades...</option>
            </select>
            <button id="d-btn" style="grid-column: span 2;">Registrar Doctor</button>
        </div>
        <div id="d-msg"></div>
        <div class="table-container">
            <table>
                <thead><tr><th>ID</th><th>Nombre Completo</th><th>Cédula</th><th>Especialidad</th><th>Pacientes</th><th>Estado</th></tr></thead>
                <tbody id="d-tbody"></tbody>
            </table>
        </div>
    `;

    // 1. Llenar el select de especialidades
    const rawEsp = await invoke("listar_especialidades") as string;
    const especialidades: any[] = JSON.parse(rawEsp);
    const selectEsp = document.querySelector("#d-especialidad") as HTMLSelectElement;
    
    if (especialidades.length === 0) {
        selectEsp.innerHTML = `<option value="">No hay especialidades (registra una primero)</option>`;
    } else {
        selectEsp.innerHTML = `<option value="">Selecciona una especialidad...</option>` + 
            especialidades.map((e: any) => `<option value="${e.id}">${e.nombre}</option>`).join("");
    }

    // 2. Función para refrescar la tabla
    const refresh = async () => {
        await invoke("guardar_datos"); 
        const rawDoc = await invoke("listar_doctores") as string;
        const doctores: any[] = JSON.parse(rawDoc);
        const tbody = document.querySelector("#d-tbody") as HTMLTableSectionElement;
        
        if (doctores.length === 0) {
            tbody.innerHTML = `<tr><td colspan="6" style="text-align:center; color:#86868b; padding: 32px;">No hay doctores registrados.</td></tr>`;
            return;
        }

        tbody.innerHTML = doctores.map((d: any) => {
            // Buscar el nombre de la especialidad usando el id
            const esp = especialidades.find((e: any) => e.id === d.idEspecialidad);
            const nombreEsp = esp ? esp.nombre : "Desconocida";

            return `
            <tr>
                <td><strong>${d.id}</strong></td>
                <td>Dr. ${d.nombre} ${d.apellidoPaterno} ${d.apellidoMaterno}</td>
                <td>${d.cedula}</td>
                <td><span class="badge" style="background:#e1e1e6; color:#1d1d1f;">${nombreEsp}</span></td>
                <td>${d.pacientesAsignados}</td>
                <td>
                    <span class="badge ${d.activo ? 'active' : 'inactive'}">
                        ${d.activo ? "Activo" : "Inactivo"}
                    </span>
                </td>
            </tr>
        `}).join("");
    };

    // 3. Evento del botón Registrar
    const btnRegistrar = document.querySelector("#d-btn") as HTMLButtonElement;
    btnRegistrar.onclick = async () => {
        const val = (id: string): string => (document.querySelector(id) as HTMLInputElement).value.trim();
        
        const idEsp = selectEsp.value;

        if (!idEsp) {
            document.querySelector("#d-msg")!.innerHTML = `<div class="msg err">Debes seleccionar una especialidad.</div>`;
            return;
        }

        const code: number = await invoke("registrar_doctor", {
            nombre: val("#d-nombre"),
            apep: val("#d-apep"),
            apem: val("#d-apem"),
            cedula: val("#d-cedula"),
            idEspecialidad: Number(idEsp) // Tauri lo convierte a snake_case para Rust
        });

        const msg = document.querySelector("#d-msg") as HTMLDivElement;
        
        if (code === 1) {
            msg.innerHTML = `<div class="msg ok">Doctor registrado exitosamente.</div>`;
            ["#d-nombre","#d-apep","#d-apem","#d-cedula"].forEach(id => (document.querySelector(id) as HTMLInputElement).value = "");
            selectEsp.value = ""; // Reiniciar el select
            refresh();
            setTimeout(() => msg.innerHTML = "", 3000);
        } else {
            let errorMsg = "Error desconocido";
            switch (code) {
                case 2: errorMsg = "Doctor no encontrado"; break;
                case 3: errorMsg = "ID de doctor inválido"; break;
                case 4: errorMsg = "Cédula inválida (deben ser 8 o 9 dígitos)"; break;
                case 5: errorMsg = "La cédula ya está registrada"; break;
                case 101: errorMsg = "ID inválido"; break;
                case 102: errorMsg = "Nombre o apellido inválido (solo letras)"; break;
            }
            msg.innerHTML = `<div class="msg err">${errorMsg}</div>`;
            
        }
    };

    refresh();
}
//=========================================================================
async function renderEspecialidades() {
    app.innerHTML = `
        <h2>Gestión de Especialidades</h2>
        <div class="form" style="grid-template-columns: 1fr;">
            <input id="e-nombre" placeholder="Nombre de la especialidad (ej. Cardiología)" />
            <button id="e-btn" style="grid-column: span 1;">Registrar Especialidad</button>
        </div>
        <div id="e-msg"></div>
        <div class="table-container">
            <table>
                <thead><tr><th>ID</th><th>Nombre de Especialidad</th></tr></thead>
                <tbody id="e-tbody"></tbody>
            </table>
        </div>
    `;

    const refresh = async () => {
        await invoke("guardar_datos"); 
        const raw: string = await invoke("listar_especialidades");
        const especialidades: any[] = JSON.parse(raw);
        const tbody = document.querySelector("#e-tbody") as HTMLTableSectionElement;
        
        if (especialidades.length === 0) {
            tbody.innerHTML = `<tr><td colspan="2" style="text-align:center; color:#86868b; padding: 32px;">No hay especialidades registradas.</td></tr>`;
            return;
        }

        tbody.innerHTML = especialidades.map((e: any) => `
            <tr>
                <td><strong>${e.id}</strong></td>
                <td>${e.nombre}</td>
            </tr>
        `).join("");
    };

    const btnRegistrar = document.querySelector("#e-btn") as HTMLButtonElement;
    btnRegistrar.onclick = async () => {
        const input = document.querySelector("#e-nombre") as HTMLInputElement;
        const nombre = input.value.trim();
        
        if (!nombre) {
            document.querySelector("#e-msg")!.innerHTML = `<div class="msg err">El nombre no puede estar vacío.</div>`;
            return;
        }

        const code: number = await invoke("registrar_especialidad", { nombre });
        const msg = document.querySelector("#e-msg") as HTMLDivElement;
        
        if (code === 1) {
            msg.innerHTML = `<div class="msg ok">Especialidad registrada exitosamente.</div>`;
            input.value = ""; // Limpiar input
            refresh();
            setTimeout(() => msg.innerHTML = "", 3000);
        } else {
            msg.innerHTML = `<div class="msg err">${errMsg(code)}</div>`;
        }
    };

    refresh();
}
//============================================================================
async function renderCitas() {
    app.innerHTML = `
        <h2>Gestión de Citas</h2>
        <div class="form-grid-citas">
            <select id="c-paciente"><option>Cargando pacientes...</option></select>
            <select id="c-doctor"><option>Cargando doctores...</option></select>
            <input id="c-dia" type="number" placeholder="Día (1-31)" min="1" max="31" />
            <input id="c-mes" type="number" placeholder="Mes (1-12)" min="1" max="12" />
            <input id="c-anio" type="number" placeholder="Año (ej. 2026)" min="2024" max="2030" />
            <input id="c-hora" type="number" placeholder="Hora (0-23)" min="0" max="23" />
            <input id="c-minuto" type="number" placeholder="Minuto (0-59)" min="0" max="59" />
            <button id="c-btn">Agendar Cita</button>
        </div>
        <div id="c-msg"></div>
        <div class="table-container">
            <table>
                <thead><tr><th>ID</th><th>Paciente</th><th>Doctor</th><th>Fecha y Hora</th><th>Estado</th><th>Acciones</th></tr></thead>
                <tbody id="c-tbody"></tbody>
            </table>
        </div>
    `;

    // 1. Cargar catálogos (solo activos)
    const rawPacs = await invoke("listar_pacientes") as string;
    const pacs: any[] = JSON.parse(rawPacs).filter((p: any) => p.activo);
    
    const rawDocs = await invoke("listar_doctores") as string;
    const docs: any[] = JSON.parse(rawDocs).filter((d: any) => d.activo);

    const selPac = document.querySelector("#c-paciente") as HTMLSelectElement;
    const selDoc = document.querySelector("#c-doctor") as HTMLSelectElement;

    if (pacs.length === 0 || docs.length === 0) {
        selPac.innerHTML = `<option value="">Faltan pacientes activos</option>`;
        selDoc.innerHTML = `<option value="">Faltan doctores activos</option>`;
    } else {
        selPac.innerHTML = `<option value="">Selecciona un paciente...</option>` + 
            pacs.map((p: any) => `<option value="${p.id}">${p.id} - ${p.nombre} ${p.apellidoPaterno}</option>`).join("");
        selDoc.innerHTML = `<option value="">Selecciona un doctor...</option>` + 
            docs.map((d: any) => `<option value="${d.id}">${d.id} - Dr. ${d.nombre} ${d.apellidoPaterno}</option>`).join("");
    }

    // 2. Refrescar tabla
    const refresh = async () => {
        await invoke("guardar_datos"); 
        const rawCitas = await invoke("listar_citas") as string;
        const citas: any[] = JSON.parse(rawCitas);
        const tbody = document.querySelector("#c-tbody") as HTMLTableSectionElement;
        
        if (citas.length === 0) {
            tbody.innerHTML = `<tr><td colspan="6" style="text-align:center; color:#86868b; padding: 32px;">No hay citas agendadas.</td></tr>`;
            return;
        }

        tbody.innerHTML = citas.map((c: any) => {
            // Traducir IDs a nombres para que se vea bonito
            const pac = pacs.find((p: any) => p.id === c.idPaciente);
            const doc = docs.find((d: any) => d.id === c.idDoctor);
            const nPac = pac ? `${pac.nombre} ${pac.apellidoPaterno}` : `ID ${c.idPaciente}`;
            const nDoc = doc ? `Dr. ${doc.nombre} ${doc.apellidoPaterno}` : `ID ${c.idDoctor}`;

            // Traducir estado (1 = Programada, 2 = Atendida, 3 = Cancelada)
            let estadoTxt = "", badgeClass = "";
            if (c.estado === 1) { estadoTxt = "Programada"; badgeClass = "active"; }
            else if (c.estado === 2) { estadoTxt = "Atendida"; badgeClass = "badge"; }
            else { estadoTxt = "Cancelada"; badgeClass = "inactive"; }

            // Formatear hora (ej. 9:05)
            const horaFmt = `${String(c.hora).padStart(2, '0')}:${String(c.minuto).padStart(2, '0')}`;

            return `
            <tr>
                <td><strong>${c.id}</strong></td>
                <td>${nPac}</td>
                <td>${nDoc}</td>
                <td>${c.fecha.dia}/${c.fecha.mes}/${c.fecha.anio} a las ${horaFmt}</td>
                <td><span class="badge ${badgeClass}">${estadoTxt}</span></td>
                <td class="actions">
                    ${c.estado === 1 ? `
                        <button class="ok" data-atender="${c.id}">Atender</button>
                        <button class="danger" data-cancelar="${c.id}">Cancelar</button>
                    ` : `<span style="color:#86868b; font-size:12px;">Sin acciones</span>`}
                </td>
            </tr>
        `}).join("");

        // Listeners para botones de la tabla
        tbody.querySelectorAll("[data-atender]").forEach((b: Element) => {
            const btn = b as HTMLButtonElement;
            btn.onclick = async () => {
                await invoke("atender_cita", { id: Number(btn.dataset.atender) });
                refresh();
            };
        });
        tbody.querySelectorAll("[data-cancelar]").forEach((b: Element) => {
            const btn = b as HTMLButtonElement;
            btn.onclick = async () => {
                await invoke("cancelar_cita", { id: Number(btn.dataset.cancelar) });
                refresh();
            };
        });
    };

    // 3. Evento del botón Registrar
    const btnRegistrar = document.querySelector("#c-btn") as HTMLButtonElement;
    btnRegistrar.onclick = async () => {
        const val = (id: string): string => (document.querySelector(id) as HTMLInputElement).value.trim();
        
        const code: number = await invoke("registrar_cita", {
            idPaciente: Number(selPac.value),
            idDoctor: Number(selDoc.value),
            dia: Number(val("#c-dia")), mes: Number(val("#c-mes")), anio: Number(val("#c-anio")),
            hora: Number(val("#c-hora")), minuto: Number(val("#c-minuto"))
        });

        const msg = document.querySelector("#c-msg") as HTMLDivElement;
        
        if (code === 1) {
            msg.innerHTML = `<div class="msg ok">Cita agendada exitosamente.</div>`;
            ["#c-dia","#c-mes","#c-anio","#c-hora","#c-minuto"].forEach(id => (document.querySelector(id) as HTMLInputElement).value = "");
            refresh();
            setTimeout(() => msg.innerHTML = "", 3000);
        } else {
            // Errores de Citas
            let errorMsg = "Error desconocido";
            switch (code) {
                case 600: errorMsg = "Fecha inválida (no puede ser en el pasado)."; break;
                case 601: errorMsg = "Hora inválida."; break;
                case 602: errorMsg = "Paciente no encontrado."; break;
                case 603: errorMsg = "Doctor no encontrado."; break;
                case 604: errorMsg = "El doctor ya tiene una cita a esa misma hora."; break;
                case 605: errorMsg = "El paciente ya tiene una cita a esa misma hora."; break;
            }
            msg.innerHTML = `<div class="msg err">${errorMsg}</div>`;
        }
    };

    refresh();
}
async function renderCola() {
    app.innerHTML = `
        <h2>Cola de Espera</h2>
        <div class="form-grid-citas" style="grid-template-columns: 1fr;">
            <select id="cola-paciente"><option>Cargando pacientes...</option></select>
            <button id="cola-btn-agregar">Añadir a la Fila</button>
        </div>
        
        <div id="cola-msg"></div>
        
        <div class="table-container">
            <div style="display: flex; justify-content: space-between; align-items: center; padding: 16px; border-bottom: 1px solid var(--border-color); background: #fafafa;">
                <h3 style="margin: 0; font-size: 16px;">Fila Actual</h3>
                <button id="cola-btn-siguiente" style="padding: 8px 16px; background: var(--primary-color); color: #fff; border: none; border-radius: var(--radius-md); cursor: pointer; font-weight: 600;">Llamar al Siguiente</button>
            </div>
            <table>
                <thead><tr><th>Posición</th><th>ID</th><th>Paciente</th></tr></thead>
                <tbody id="cola-tbody"></tbody>
            </table>
        </div>
    `;

    // 1. Cargar pacientes activos para el selector
    const rawPacs = await invoke("listar_pacientes") as string;
    const pacs: any[] = JSON.parse(rawPacs).filter((p: any) => p.activo);
    const selPac = document.querySelector("#cola-paciente") as HTMLSelectElement;

    if (pacs.length === 0) {
        selPac.innerHTML = `<option value="">No hay pacientes activos</option>`;
    } else {
        selPac.innerHTML = `<option value="">Selecciona un paciente para la fila...</option>` + 
            pacs.map((p: any) => `<option value="${p.id}">${p.id} - ${p.nombre} ${p.apellidoPaterno}</option>`).join("");
    }

    // 2. Refrescar la tabla de la cola
    const refresh = async () => {
        const rawCola = await invoke("listar_cola") as string;
        const colaIds: number[] = JSON.parse(rawCola);
        const tbody = document.querySelector("#cola-tbody") as HTMLTableSectionElement;
        
        if (colaIds.length === 0) {
            tbody.innerHTML = `<tr><td colspan="3" style="text-align:center; color:#86868b; padding: 32px;">La fila está vacía.</td></tr>`;
            return;
        }

        tbody.innerHTML = colaIds.map((idPaciente: number, index: number) => {
            const pac = pacs.find((p: any) => p.id === idPaciente);
            const nombre = pac ? `${pac.nombre} ${pac.apellidoPaterno}` : `ID ${idPaciente}`;
            
            return `
            <tr>
                <td><strong>#${index + 1}</strong></td>
                <td>${idPaciente}</td>
                <td>${nombre}</td>
            </tr>
        `}).join("");
    };

    // 3. Evento para Añadir a la Fila
    const btnAgregar = document.querySelector("#cola-btn-agregar") as HTMLButtonElement;
    btnAgregar.onclick = async () => {
        const idPac = Number(selPac.value);
        const msg = document.querySelector("#cola-msg") as HTMLDivElement;

        if (!idPac) {
            msg.innerHTML = `<div class="msg err">Debes seleccionar un paciente.</div>`;
            return;
        }

        const code: number = await invoke("encolar", { id: idPac });
        
        if (code === 1) { // COLA_OK
            msg.innerHTML = `<div class="msg ok">Paciente añadido a la fila.</div>`;
            selPac.value = ""; 
            refresh();
            setTimeout(() => msg.innerHTML = "", 3000);
        } else if (code === 3) { // COLA_DUPLICADO
            msg.innerHTML = `<div class="msg err">Ese paciente ya está en la fila.</div>`;
        } else {
            msg.innerHTML = `<div class="msg err">Error al añadir a la fila.</div>`;
        }
    };

    // 4. Evento para Llamar al Siguiente (Desencolar)
    const btnSiguiente = document.querySelector("#cola-btn-siguiente") as HTMLButtonElement;
    btnSiguiente.onclick = async () => {
        const msg = document.querySelector("#cola-msg") as HTMLDivElement;
        
        const idAtendido: number = await invoke("desencolar");
        
        if (idAtendido === -1) {
            msg.innerHTML = `<div class="msg err">La fila está vacía, no hay a quién llamar.</div>`;
        } else {
            const pac = pacs.find((p: any) => p.id === idAtendido);
            const nombre = pac ? pac.nombre : `ID ${idAtendido}`;
            msg.innerHTML = `<div class="msg ok">Llamando a consultorio a: ${nombre}</div>`;
            refresh();
            setTimeout(() => msg.innerHTML = "", 4000);
        }
    };

    refresh();
}

const tabs: Record<string, () => Promise<void>> = {
    pacientes: renderPacientes,
    doctores: renderDoctores,
    especialidades: renderEspecialidades,
    citas: renderCitas,
    cola: renderCola,
};

document.querySelectorAll("nav button").forEach((btnElement: Element) => {
    const btn = btnElement as HTMLButtonElement;
    btn.onclick = async () => {
        document.querySelectorAll("nav button").forEach((b: Element) => (b as HTMLButtonElement).classList.remove("active"));
        btn.classList.add("active");
        await tabs[btn.dataset.tab!]();
    };
});
// --- PANTALLA DE CARGA ---
// --- LOGIN ---
function mostrarLogin() {
    // Ocultar la barra de navegación y el main para que no se vean
    (document.querySelector("nav") as HTMLElement).style.display = "none";
    app.style.display = "none";
    
    // Quitar splash si aún existe
    const oldSplash = document.getElementById("splash-screen");
    if (oldSplash) oldSplash.remove();

    const loginDiv = document.createElement("div");
    loginDiv.id = "login-screen";
    loginDiv.innerHTML = `
        <div class="login-box">
            <h1>Clínica</h1>
            <p>Ingresa tus credenciales para acceder al sistema</p>
            <input id="l-user" placeholder="Usuario (ej. admin123)" autocomplete="off" />
            <input id="l-pass" type="password" placeholder="Contraseña (ej. admin1234)" />
            <div id="l-msg"></div>
            <button id="l-btn">Iniciar Sesión</button>
        </div>
    `;
    document.body.appendChild(loginDiv);

    const btn = document.querySelector("#l-btn") as HTMLButtonElement;
    const msg = document.querySelector("#l-msg") as HTMLDivElement;

    btn.onclick = async () => {
        const user = (document.querySelector("#l-user") as HTMLInputElement).value;
        const pass = (document.querySelector("#l-pass") as HTMLInputElement).value;

        // Llamamos al backend
        const code: number = await invoke("login", { usuario: user, clave: pass });

        if (code === 1) { // LOGIN_EXITOSO
            msg.innerHTML = `<div class="msg ok">Acceso concedido. Entrando...</div>`;
            setTimeout(async () => {
                loginDiv.remove();
                (document.querySelector("nav") as HTMLElement).style.display = "flex";
                app.style.display = "block";
                
                // Activar la primera pestaña por defecto
                const primerBoton = document.querySelector("nav button");
                if (primerBoton) primerBoton.classList.add("active");
                
                await renderPacientes();
            }, 800);
        } else if (code === 4) { // ERROR_BLOQUEADO (5 intentos)
            msg.innerHTML = `<div class="msg err">Cuenta bloqueada por demasiados intentos fallidos.</div>`;
            btn.disabled = true;
            btn.style.background = "#ccc";
            btn.innerText = "Bloqueado";
        } else if (code === 3) { // ERROR_CREDENCIALES
            msg.innerHTML = `<div class="msg err">Usuario o contraseña incorrectos.</div>`;
        } else { // ERROR_FORMATO_INVALIDO
            msg.innerHTML = `<div class="msg err">Formato inválido. Usuario: 8 alfanuméricos. Clave: 8 a 10 caracteres.</div>`;
        }
    };
}

// --- PANTALLA DE CARGA E INICIALIZACIÓN ---
const splash = document.createElement("div");
splash.id = "splash-screen";
splash.innerHTML = `
    <div class="spinner"></div>
    <div class="splash-title">Clínica</div>
    <div class="splash-subtitle">Iniciando sistema y cargando datos...</div>
`;
document.body.appendChild(splash);

async function initApp() {
    try {
        // Despertamos el backend
        await invoke("listar_pacientes"); 
        
        // Esperamos un momento por la animación
        await new Promise(resolve => setTimeout(resolve, 1000));

        // Quitamos la pantalla de carga
        splash.classList.add("fade-out");
        setTimeout(() => splash.remove(), 600);

        // Mostramos el Login en lugar de ir directo a Pacientes
        mostrarLogin();

    } catch (error) {
        splash.innerHTML = `
            <div style="color: var(--danger-color); text-align: center;">
                <h2 style="margin-bottom: 10px;">Error al iniciar</h2>
                <p>No se pudo conectar con el motor de la base de datos.</p>
            </div>
        `;
        console.error(error);
    }
}

// Arrancamos la app
initApp();