#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    CLINICO,
    PEDIATRA,
    CARDIOLOGISTA
} Especialidade;

typedef enum {
    AGENDADA,
    REALIZADA,
    CANCELADA
} StatusConsulta;

typedef struct {
    char nome[100];
    char crm[20];
    Especialidade especialidade;
    char telefone[20];
} Medico;

typedef struct {
    char nome[100];
    char cpf[15];
    char telefone[20];
} Paciente;

typedef struct {
    int id;
    char cpfPaciente[15];
    char crmMedico[20];
    char data[11];
    char hora[6];
    StatusConsulta status;
} Consulta;

// Variáveis globais
Medico* medicos = NULL;
int totalMedicos = 0;

Paciente* pacientes = NULL;
int totalPacientes = 0;

Consulta* consultas = NULL;
int totalConsultas = 0;

// Função para obter nome da especialidade
const char* nomeEspecialidade(Especialidade esp) {
    switch (esp) {
        case CLINICO: return "Clínico Geral";
        case PEDIATRA: return "Pediatra";
        case CARDIOLOGISTA: return "Cardiologista";
        default: return "Desconhecida";
    }
}

// Função para obter nome do status da consulta
const char* nomeStatus(StatusConsulta s) {
    switch (s) {
        case AGENDADA: return "Agendada";
        case REALIZADA: return "Realizada";
        case CANCELADA: return "Cancelada";
        default: return "Desconhecido";
    }
}

// -------------------- Funções de persistência --------------------
void carregarMedicos() {
    FILE* f = fopen("medicos.bin", "rb");
    if (f) {
        fread(&totalMedicos, sizeof(int), 1, f);
        medicos = malloc(totalMedicos * sizeof(Medico));
        fread(medicos, sizeof(Medico), totalMedicos, f);
        fclose(f);
    }
}

void salvarMedicos() {
    FILE* f = fopen("medicos.bin", "wb");
    if (f) {
        fwrite(&totalMedicos, sizeof(int), 1, f);
        fwrite(medicos, sizeof(Medico), totalMedicos, f);
        fclose(f);
    }
}

void carregarPacientes() {
    FILE* f = fopen("pacientes.bin", "rb");
    if (f) {
        fread(&totalPacientes, sizeof(int), 1, f);
        pacientes = malloc(totalPacientes * sizeof(Paciente));
        fread(pacientes, sizeof(Paciente), totalPacientes, f);
        fclose(f);
    }
}

void salvarPacientes() {
    FILE* f = fopen("pacientes.bin", "wb");
    if (f) {
        fwrite(&totalPacientes, sizeof(int), 1, f);
        fwrite(pacientes, sizeof(Paciente), totalPacientes, f);
        fclose(f);
    }
}

void carregarConsultas() {
    FILE* f = fopen("consultas.bin", "rb");
    if (f) {
        fread(&totalConsultas, sizeof(int), 1, f);
        consultas = malloc(totalConsultas * sizeof(Consulta));
        fread(consultas, sizeof(Consulta), totalConsultas, f);
        fclose(f);
    }
}

void salvarConsultas() {
    FILE* f = fopen("consultas.bin", "wb");
    if (f) {
        fwrite(&totalConsultas, sizeof(int), 1, f);
        fwrite(consultas, sizeof(Consulta), totalConsultas, f);
        fclose(f);
    }
}

// -------------------- Funções auxiliares --------------------
Medico* buscarMedicoPorCRM(const char* crm) {
    for (int i = 0; i < totalMedicos; i++) {
        if (strcmp(medicos[i].crm, crm) == 0) {
            return &medicos[i];
        }
    }
    return NULL;
}

Paciente* buscarPacientePorCPF(const char* cpf) {
    for (int i = 0; i < totalPacientes; i++) {
        if (strcmp(pacientes[i].cpf, cpf) == 0) {
            return &pacientes[i];
        }
    }
    return NULL;
}

// -------------------- Cadastro e Remoção --------------------
void cadastrarMedico() {
    medicos = realloc(medicos, (totalMedicos + 1) * sizeof(Medico));
    Medico* m = &medicos[totalMedicos];

    printf("Nome: ");
    fgets(m->nome, 100, stdin);
    strtok(m->nome, "\n");

    printf("CRM: ");
    fgets(m->crm, 20, stdin);
    strtok(m->crm, "\n");

    printf("Telefone: ");
    fgets(m->telefone, 20, stdin);
    strtok(m->telefone, "\n");

    int esp;
    do {
        printf("Especialidade (0-Clinico, 1-Pediatra, 2-Cardiologista): ");
        scanf("%d", &esp);
        getchar();
    } while (esp < 0 || esp > 2);
    m->especialidade = (Especialidade)esp;

    totalMedicos++;
    salvarMedicos();
    printf("Medico cadastrado com sucesso!\n");
}

void removerMedico() {
    printf("CRM do medico: ");
    char crm[20];
    fgets(crm, 20, stdin);
    strtok(crm, "\n");

    for (int i = 0; i < totalMedicos; i++) {
        if (strcmp(medicos[i].crm, crm) == 0) {
            for (int j = i; j < totalMedicos - 1; j++) {
                medicos[j] = medicos[j + 1];
            }
            totalMedicos--;
            medicos = realloc(medicos, totalMedicos * sizeof(Medico));
            salvarMedicos();
            printf("Medico removido.\n");
            return;
        }
    }
    printf("Medico nao encontrado.\n");
}

void cadastrarPaciente() {
    pacientes = realloc(pacientes, (totalPacientes + 1) * sizeof(Paciente));
    Paciente* p = &pacientes[totalPacientes];

    printf("Nome: ");
    fgets(p->nome, 100, stdin);
    strtok(p->nome, "\n");

    printf("CPF: ");
    fgets(p->cpf, 15, stdin);
    strtok(p->cpf, "\n");

    printf("Telefone: ");
    fgets(p->telefone, 20, stdin);
    strtok(p->telefone, "\n");

    totalPacientes++;
    salvarPacientes();
    printf("Paciente cadastrado com sucesso!\n");
}

void removerPaciente() {
    printf("CPF do paciente: ");
    char cpf[15];
    fgets(cpf, 15, stdin);
    strtok(cpf, "\n");

    for (int i = 0; i < totalPacientes; i++) {
        if (strcmp(pacientes[i].cpf, cpf) == 0) {
            for (int j = i; j < totalPacientes - 1; j++) {
                pacientes[j] = pacientes[j + 1];
            }
            totalPacientes--;
            pacientes = realloc(pacientes, totalPacientes * sizeof(Paciente));
            salvarPacientes();
            printf("Paciente removido.\n");
            return;
        }
    }
    printf("Paciente nao encontrado.\n");
}

// -------------------- Consultas --------------------
void agendarConsulta() {
    char cpf[15], crm[20];

    printf("CPF do paciente: ");
    fgets(cpf, 15, stdin);
    strtok(cpf, "\n");
    if (buscarPacientePorCPF(cpf) == NULL) {
        printf("Paciente nao encontrado!\n");
        return;
    }

    printf("CRM do medico: ");
    fgets(crm, 20, stdin);
    strtok(crm, "\n");
    if (buscarMedicoPorCRM(crm) == NULL) {
        printf("Medico nao encontrado!\n");
        return;
    }

    consultas = realloc(consultas, (totalConsultas + 1) * sizeof(Consulta));
    Consulta* c = &consultas[totalConsultas];

    c->id = totalConsultas + 1;
    strcpy(c->cpfPaciente, cpf);
    strcpy(c->crmMedico, crm);

    printf("Data (DD/MM/AAAA): ");
    fgets(c->data, 11, stdin);
    strtok(c->data, "\n");

    printf("Hora (HH:MM): ");
    fgets(c->hora, 6, stdin);
    strtok(c->hora, "\n");

    c->status = AGENDADA;

    totalConsultas++;
    salvarConsultas();
    printf("Consulta agendada com sucesso!\n");
}

void atualizarStatusConsulta() {
    printf("ID da consulta: ");
    int id;
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < totalConsultas; i++) {
        if (consultas[i].id == id) {
            int s;
            do {
                printf("Novo status (0-Agendada, 1-Realizada, 2-Cancelada): ");
                scanf("%d", &s);
                getchar();
            } while (s < 0 || s > 2);

            consultas[i].status = (StatusConsulta)s;
            salvarConsultas();
            printf("Status atualizado!\n");
            return;
        }
    }
    printf("Consulta nao encontrada!\n");
}

// -------------------- Relatórios --------------------
void listarTodosMedicos() {
    printf("\n--- Medicos Cadastrados ---\n");
    for (int i = 0; i < totalMedicos; i++) {
        printf("Nome: %s | CRM: %s | Especialidade: %s | Tel: %s\n",
               medicos[i].nome,
               medicos[i].crm,
               nomeEspecialidade(medicos[i].especialidade),
               medicos[i].telefone);
    }
}

void listarTodosPacientes() {
    printf("\n--- Pacientes Cadastrados ---\n");
    for (int i = 0; i < totalPacientes; i++) {
        printf("Nome: %s | CPF: %s | Tel: %s\n",
               pacientes[i].nome, pacientes[i].cpf, pacientes[i].telefone);
    }
}

void listarTodasConsultas() {
    printf("\n--- Consultas Cadastradas ---\n");
    for (int i = 0; i < totalConsultas; i++) {
        printf("ID:%d | Data:%s Hora:%s | Medico:%s | Paciente:%s | Status:%s\n",
               consultas[i].id, consultas[i].data, consultas[i].hora,
               consultas[i].crmMedico, consultas[i].cpfPaciente,
               nomeStatus(consultas[i].status));
    }
}

// Relatório consultas por paciente
void listarConsultasPorPaciente() {
    char cpf[15];
    printf("Informe CPF do paciente: ");
    fgets(cpf, 15, stdin);
    strtok(cpf, "\n");

    Paciente* p = buscarPacientePorCPF(cpf);
    if (!p) {
        printf("Paciente nao encontrado!\n");
        return;
    }

    printf("\n--- Consultas do paciente %s ---\n", p->nome);
    for (int i = 0; i < totalConsultas; i++) {
        if (strcmp(consultas[i].cpfPaciente, cpf) == 0) {
            Medico* m = buscarMedicoPorCRM(consultas[i].crmMedico);
            printf("ID:%d | Data:%s Hora:%s | Medico: %s | Status: %s\n",
                   consultas[i].id, consultas[i].data, consultas[i].hora,
                   m ? m->nome : "Desconhecido",
                   nomeStatus(consultas[i].status));
        }
    }
}

// Relatório consultas por médico
void listarConsultasPorMedico() {
    char crm[20];
    printf("Informe CRM do medico: ");
    fgets(crm, 20, stdin);
    strtok(crm, "\n");

    Medico* m = buscarMedicoPorCRM(crm);
    if (!m) {
        printf("Medico nao encontrado!\n");
        return;
    }

    printf("\n--- Consultas do medico %s ---\n", m->nome);
    for (int i = 0; i < totalConsultas; i++) {
        if (strcmp(consultas[i].crmMedico, crm) == 0) {
            Paciente* p = buscarPacientePorCPF(consultas[i].cpfPaciente);
            printf("ID:%d | Data:%s Hora:%s | Paciente: %s | Status: %s\n",
                   consultas[i].id, consultas[i].data, consultas[i].hora,
                   p ? p->nome : "Desconhecido",
                   nomeStatus(consultas[i].status));
        }
    }
}

// Relatório consultas por especialidade
void listarConsultasPorEspecialidade() {
    printf("\n--- Consultas por Especialidade ---\n");
    for (int i = 0; i < totalConsultas; i++) {
        Medico* m = buscarMedicoPorCRM(consultas[i].crmMedico);
        Paciente* p = buscarPacientePorCPF(consultas[i].cpfPaciente);
        if (m && p) {
            printf("ID: %d | Data: %s | Hora: %s | Medico: %s | Especialidade: %s | Paciente: %s | Status: %s\n",
                   consultas[i].id,
                   consultas[i].data,
                   consultas[i].hora,
                   m->nome,
                   nomeEspecialidade(m->especialidade),
                   p->nome,
                   nomeStatus(consultas[i].status));
        }
    }
}

// Relatório consultas por data
void listarConsultasPorData() {
    char data[11];
    printf("Informe data (DD/MM/AAAA): ");
    fgets(data, 11, stdin);
    strtok(data, "\n");

    printf("\n--- Consultas na data %s ---\n", data);
    for (int i = 0; i < totalConsultas; i++) {
        if (strcmp(consultas[i].data, data) == 0) {
            Medico* m = buscarMedicoPorCRM(consultas[i].crmMedico);
            Paciente* p = buscarPacientePorCPF(consultas[i].cpfPaciente);
            if (m && p) {
                printf("ID: %d | Hora: %s | Medico: %s | Especialidade: %s | Paciente: %s | Status: %s\n",
                       consultas[i].id,
                       consultas[i].hora,
                       m->nome,
                       nomeEspecialidade(m->especialidade),
                       p->nome,
                       nomeStatus(consultas[i].status));
            }
        }
    }
}

// -------------------- Menu --------------------
void menu() {
    int opcao;
    do {
        printf("\n--- Sistema Clínica Médica ---\n");
        printf("1. Cadastrar Médico\n");
        printf("2. Remover Médico\n");
        printf("3. Cadastrar Paciente\n");
        printf("4. Remover Paciente\n");
        printf("5. Agendar Consulta\n");
        printf("6. Atualizar Status Consulta\n");
        printf("7. Listar Médicos\n");
        printf("8. Listar Pacientes\n");
        printf("9. Listar Consultas\n");
        printf("10. Consultas por Paciente\n");
        printf("11. Consultas por Médico\n");
        printf("12. Consultas por Especialidade\n");
        printf("13. Consultas por Data\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: cadastrarMedico(); break;
            case 2: removerMedico(); break;
            case 3: cadastrarPaciente(); break;
            case 4: removerPaciente(); break;
            case 5: agendarConsulta(); break;
            case 6: atualizarStatusConsulta(); break;
            case 7: listarTodosMedicos(); break;
            case 8: listarTodosPacientes(); break;
            case 9: listarTodasConsultas(); break;
            case 10: listarConsultasPorPaciente(); break;
            case 11: listarConsultasPorMedico(); break;
            case 12: listarConsultasPorEspecialidade(); break;
            case 13: listarConsultasPorData(); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opcao invalida.\n"); break;
        }
    } while (opcao != 0);
}

int main() {
    carregarMedicos();
    carregarPacientes();
    carregarConsultas();

    menu();

    free(medicos);
    free(pacientes);
    free(consultas);

    return 0;
}




