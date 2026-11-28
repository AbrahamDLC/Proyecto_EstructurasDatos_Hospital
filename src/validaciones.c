#include "validaciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int es_letra_ascii(unsigned char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static int es_digito_ascii(unsigned char c) {
    return (c >= '0' && c <= '9');
}

// Verifica letras acentuadas y ñ/Ñ en codificación UTF-8 (2 bytes: 0xC3 + segundo byte)
static int es_acento_espanol_utf8(unsigned char c1, unsigned char c2) {
    if (c1 != 0xC3) return 0;
    switch (c2) {
        case 0x81: // Á
        case 0x89: // É
        case 0x8D: // Í
        case 0x91: // Ñ
        case 0x93: // Ó
        case 0x9A: // Ú
        case 0xA1: // á
        case 0xA9: // é
        case 0xAD: // í
        case 0xB1: // ñ
        case 0xB3: // ó
        case 0xBA: // ú
            return 1;
        default:
            return 0;
    }
}

int validaStrings(const char* cadenaTxt) {
    if (cadenaTxt == NULL || cadenaTxt[0] == '\0') return 0;

    size_t i = 0;
    while (cadenaTxt[i] != '\0') {
        unsigned char c = (unsigned char)cadenaTxt[i];
        if (es_letra_ascii(c) || c == ' ') {
            i++;
        } else if (c == 0xC3 && cadenaTxt[i + 1] != '\0' &&
                   es_acento_espanol_utf8(c, (unsigned char)cadenaTxt[i + 1])) {
            i += 2;
        } else {
            return 0;
        }
    }
    return 1;
}

// Formato: PS-000-000
int validaExpediente(const char* exped) {
    if (exped == NULL || strlen(exped) != 10) return 0;

    if (exped[0] != 'P' || exped[1] != 'S' || exped[2] != '-' || exped[6] != '-') {
        return 0;
    }

    int indices_digitos[6] = {3, 4, 5, 7, 8, 9};
    for (int i = 0; i < 6; i++) {
        if (!es_digito_ascii((unsigned char)exped[indices_digitos[i]])) {
            return 0;
        }
    }
    return 1;
}

// Formato: 8 a 10 caracteres alfanuméricos
int validaClave(const char* contrasenia) {
    if (contrasenia == NULL) return 0;
    size_t len = strlen(contrasenia);
    if (len < 8 || len > 10) return 0;

    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)contrasenia[i];
        if (!es_letra_ascii(c) && !es_digito_ascii(c)) {
            return 0;
        }
    }
    return 1;
}

// Formato: exactamente 8 caracteres (letras, números, '.' o '?')
int validaUsuario(const char* user) {
    if (user == NULL || strlen(user) != 8) return 0;

    for (size_t i = 0; i < 8; i++) {
        unsigned char c = (unsigned char)user[i];
        if (!es_letra_ascii(c) && !es_digito_ascii(c) && c != '.' && c != '?') {
            return 0;
        }
    }
    return 1;
}

// Formato: 8 o 9 dígitos
int validaCedula(const char* cedula) {
    if (cedula == NULL) return 0;
    size_t len = strlen(cedula);
    if (len < 8 || len > 9) return 0;

    for (size_t i = 0; i < len; i++) {
        if (!es_digito_ascii((unsigned char)cedula[i])) {
            return 0;
        }
    }
    return 1;
}

bool validaID(int *id) {
    char buffer[12];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return false;
    buffer[strcspn(buffer, "\n")] = '\0';

    if (buffer[0] == '\0') return false;
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (!isdigit((unsigned char)buffer[i])) return false;
    }

    *id = atoi(buffer);
    return *id > 0;
}

int validaLogin(const char* c) {
    return validaClave(c);
}

int validaEspecialidad(listaEspecialidades* lista, const char* esp) {
    if (lista == NULL || esp == NULL) return 0;
    if (validaStrings(esp) != 1) return 0;
    return buscarNombreEspecialidad(lista, esp) == NULL;
}