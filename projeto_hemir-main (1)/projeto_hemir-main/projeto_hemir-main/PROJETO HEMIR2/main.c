#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#define MAX_USUARIOS 50
#define admin_login "admin"
#define admin_senha "12345678"

int count_estagiario = 0;

struct usuario {
    char nome[100];
    char senha[100];
};

struct usuario usuarios[MAX_USUARIOS];

void pedir_usuario(int x) {
    char usuario_x[100];
    char senha_x[100];
    int validade = 0;
    printf("Digite seu nome (usu�rio %d): ", x + 1);
    fflush(stdin);
    fgets(usuario_x, 100, stdin);
    usuario_x[strcspn(usuario_x, "\n")] = 0;
    for (int i = 0; i < strlen(usuario_x); i++) {
        usuario_x[i] = toupper(usuario_x[i]);
    }
    strcpy(usuarios[x].nome, usuario_x);
    while (validade == 0) {
        printf("Digite sua senha (usu�rio %d): ", x + 1);
        fgets(senha_x, 100, stdin);
        senha_x[strcspn(senha_x, "\n")] = 0;
        if (strlen(senha_x) != 8) {
            puts("Senha dever� conter 8 d�gitos");
            validade = 0;
        } else {
            strcpy(usuarios[x].senha, senha_x);
            count_estagiario++;
            validade = 1;
        }
    }
}

int main(void) {
    system("COLOR 1F");
    setlocale(LC_ALL, "Portuguese");
    FILE *pArquivo_horario;
    FILE *pArquivo_usuarios;
    FILE *pArquivo_horario_saida;
    puts("***************************************************************************");
    puts("**            PROJETO PONTO ELETR�NICO PARA ESTAGI�RIOS                  **");
    puts("**                     PROF: HEMIR SANTIAGO                              **");
    puts("**                                                                       **");
    puts("***************************************************************************");

    pArquivo_usuarios = fopen("Usuarios_registrados.txt", "r");
    int count = 0;

    while (fgets(usuarios[count].nome, 100, pArquivo_usuarios) != NULL) {
        usuarios[count].nome[strcspn(usuarios[count].nome, "\n")] = 0;
        if (fgets(usuarios[count].senha, 100, pArquivo_usuarios) != NULL) {
            usuarios[count].senha[strcspn(usuarios[count].senha, "\n")] = 0;
            count++;
            count_estagiario++;
        }
    }
    fclose(pArquivo_usuarios);

    for (;;) {
        char opcao1;
        char opcao2;
        char opcao3;
        puts("PONTO ELETR�NICO -- MENU PRINCIPAL:");
        puts("1. Registrar Usu�rio");
        puts("2. Registrar Hor�rio");
        puts("3. Gerar Relat�rio");
        puts("4. Sair");
        scanf(" %c", &opcao1);

        switch (opcao1) {
            case '1':
                pArquivo_usuarios = fopen("Usuarios_registrados.txt", "a");
                char nome_adm[100];
                char senha_adm[100];
                printf("Digite o login de admin: ");
                fflush(stdin);
                fgets(nome_adm, 100, stdin);
                nome_adm[strcspn(nome_adm, "\n")] = 0;
                printf("Digite a senha de admin: ");
                fgets(senha_adm, 100, stdin);
                senha_adm[strcspn(senha_adm, "\n")] = 0;
                if (strcmp(nome_adm, admin_login) == 0 && strcmp(senha_adm, admin_senha) == 0) {
                    pedir_usuario(count_estagiario);
                    fprintf(pArquivo_usuarios, "%s\n%s\n", usuarios[count_estagiario - 1].nome, usuarios[count_estagiario - 1].senha);
                } else {
                    puts("Login ou senha de admin errado");
                }
                fclose(pArquivo_usuarios);
                break;
            case '2':
                puts("1. Hor�rio de entrada");
                puts("2. Hor�rio de sa�da");
                scanf(" %c", &opcao2);
                switch (opcao2) {
                    case '1':
                        pArquivo_horario = fopen("Horario_de_entrada.txt", "a");
                        if (strcmp(usuarios[0].senha, "\0") != 0) {
                            time_t tempo = time(NULL);
                            struct tm tm = *localtime(&tempo);
                            printf("DATA E HOR�RIO: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

                            struct tm tempo_limite = *localtime(&tempo);
                            tempo_limite.tm_hour = 12;
                            tempo_limite.tm_min = 0;
                            tempo_limite.tm_sec = 0;
                            tempo_limite.tm_mon = tm.tm_mon;
                            tempo_limite.tm_mday = tm.tm_mday;
                            double atraso = difftime(tempo, mktime(&tempo_limite));

                            int encontrado = 0;
                            char nome_input[100];
                            char senha_input[100];
                            printf("Digite seu nome: ");
                            fflush(stdin);
                            fgets(nome_input, 100, stdin);
                            nome_input[strcspn(nome_input, "\n")] = 0;
                            for (int i = 0; i < strlen(nome_input); i++) {
                                nome_input[i] = toupper(nome_input[i]);
                            }
                            printf("Digite sua senha: ");
                            fgets(senha_input, 100, stdin);
                            senha_input[strcspn(senha_input, "\n")] = 0;
                            for (int i = 0; i < count_estagiario; i++) {
                                if (strcmp(nome_input, usuarios[i].nome) == 0 && strcmp(senha_input, usuarios[i].senha) == 0) {
                                    encontrado = 1;
                                    if (atraso > 0) {
                                        time_t atraso_time_t = (time_t)atraso;
                                        struct tm *atraso_tm = gmtime(&atraso_time_t);

                                        char tempo_de_chegada[100];
                                        char tempo_atraso_str[100];

                                        strftime(tempo_de_chegada, sizeof(tempo_de_chegada), "%Y-%m-%d %H:%M:%S", &tm);
                                        strftime(tempo_atraso_str, sizeof(tempo_atraso_str), "%H:%M:%S", atraso_tm);

                                        printf("Voc� se atrasou %d horas %d minutos e %d segundos\n", atraso_tm->tm_hour, atraso_tm->tm_min, atraso_tm->tm_sec);
                                        fprintf(pArquivo_horario, "Tempo de entrada de %s: %s com um atraso de %s\n", usuarios[i].nome, tempo_de_chegada, tempo_atraso_str);
                                    } else {
                                        puts("Voc� chegou dentro do hor�rio");
                                        char tempo_de_chegada[100];
                                        strftime(tempo_de_chegada, sizeof(tempo_de_chegada), "%Y-%m-%d %H:%M:%S", &tm);
                                        fprintf(pArquivo_horario, "Tempo de entrada de %s: %s sem atraso\n", usuarios[i].nome, tempo_de_chegada);
                                    }
                                    break;
                                }
                            }
                            if (encontrado == 0) {
                                puts("Nome ou senha incorretos");
                            }
                        } else {
                            puts("Nenhum usu�rio registrado");
                        }
                        fclose(pArquivo_horario);
                        break;
                    case '2':
                        pArquivo_horario_saida = fopen("Horario_de_saida.txt", "a");
                        if (strcmp(usuarios[0].senha, "\0") != 0) {
                            time_t tempo = time(NULL);
                            struct tm tm = *localtime(&tempo);
                            printf("DATA E HOR�RIO: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
                            int encontrado = 0;
                            char nome_input[100];
                            char senha_input[100];
                            printf("Digite seu nome: ");
                            fflush(stdin);
                            fgets(nome_input, 100, stdin);
                            nome_input[strcspn(nome_input, "\n")] = 0;
                            for (int i = 0; i < strlen(nome_input); i++) {
                                nome_input[i] = toupper(nome_input[i]);
                            }
                            printf("Digite sua senha: ");
                            fgets(senha_input, 100, stdin);
                            senha_input[strcspn(senha_input, "\n")] = 0;
                            for (int i = 0; i < count_estagiario; i++) {
                                if (strcmp(nome_input, usuarios[i].nome) == 0 && strcmp(senha_input, usuarios[i].senha) == 0) {
                                    encontrado = 1;
                                    char tempo_de_saida[100];
                                    strftime(tempo_de_saida, sizeof(tempo_de_saida), "%Y-%m-%d %H:%M:%S", &tm);
                                    fprintf(pArquivo_horario_saida, "Tempo de sa�da de %s: %s\n", usuarios[i].nome, tempo_de_saida);
                                    puts("Hor�rio registrado com sucesso");
                                    break;
                                }
                            }
                            if (encontrado == 0) {
                                printf("Nome ou senha incorretos\n");
                            }
                        } else {
                            printf("Nenhum usu�rio registrado\n");
                        }
                        fclose(pArquivo_horario_saida);
                        break;
                    default:
                        puts("Op��o inv�lida");
                }
                break;
            case '3':
                puts("1. Relat�rio dos hor�rios de entrada");
                puts("2. Relat�rio dos hor�rios de sa�da");
                scanf(" %c", &opcao3);
                switch (opcao3) {
                    case '1':
                        printf("Digite o login de admin: ");
                        fflush(stdin);
                        fgets(nome_adm, 100, stdin);
                        nome_adm[strcspn(nome_adm, "\n")] = 0;
                        printf("Digite a senha de admin: ");
                        fgets(senha_adm, 100, stdin);
                        senha_adm[strcspn(senha_adm, "\n")] = 0;
                        if(strcmp(nome_adm,admin_login) == 0 && strcmp(senha_adm,admin_senha) == 0) {
                            pArquivo_horario = fopen("Horario_de_entrada.txt", "r");
                            rewind(pArquivo_horario);
                            char linha[256];
                            printf("\n--- Relat�rio de Hor�rio ---\n");
                            while (fgets(linha, sizeof(linha), pArquivo_horario) != NULL) {
                                printf("%s", linha);
                            }
                            printf("-----------------------------\n");
                            fclose(pArquivo_horario);
                        } else {
                            puts("Login ou senha de admin errado");
                        }
                        break;
                    case '2':
                        printf("Digite o login de admin: ");
                        fflush(stdin);
                        fgets(nome_adm, 100, stdin);
                        nome_adm[strcspn(nome_adm, "\n")] = 0;
                        printf("Digite a senha de admin: ");
                        fgets(senha_adm, 100, stdin);
                        senha_adm[strcspn(senha_adm, "\n")] = 0;
                        if(strcmp(nome_adm,admin_login) == 0 && strcmp(senha_adm,admin_senha) == 0) {
                            pArquivo_horario_saida = fopen("Horario_de_saida.txt", "r");
                            rewind(pArquivo_horario_saida);
                            char linha2[256];
                            printf("\n--- Relat�rio de Hor�rio ---\n");
                            while (fgets(linha2, sizeof(linha2), pArquivo_horario_saida) != NULL) {
                                printf("%s", linha2);
                            }
                            printf("-----------------------------\n");
                            fclose(pArquivo_horario_saida);
                        } else {
                            puts("Login ou senha de admin errado");
                        }
                        break;
                    default:
                        puts("Op��o inv�lida");
                }
                break;
            case '4':
                exit(0);
                break;
            default:
                puts("Op��o inv�lida");
        }
    }
}
