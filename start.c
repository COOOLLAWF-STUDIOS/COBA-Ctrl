#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void replaceInFile(const char *filename, const char *target, const char *replacement) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("无法打开文件");
        return;
    }

    FILE *tempFile = fopen("temp.env", "w");
    if (!tempFile) {
        perror("无法创建临时文件");
        fclose(file);
        return;
    }

    char line[256];
    int found = 0;
    size_t targetLen = strlen(target);
    size_t replacementLen = strlen(replacement);

    while (fgets(line, sizeof(line), file)) {
        char *pos = line;
        while ((pos = strstr(pos, target)) != NULL) {
            size_t restLen = strlen(pos + targetLen);
            fwrite(line, 1, pos - line, tempFile);
            fwrite(replacement, 1, replacementLen, tempFile);
            pos += targetLen;
            memcpy(line, pos, restLen + 1);
            found = 1;
        }
        fputs(line, tempFile);
    }

    fclose(file);
    fclose(tempFile);

    if (!found) {
        printf("未找到目标字符串 %s\n", target);
        remove("temp.env");
        return;
    }

    if (remove(filename) != 0) {
        perror("删除原文件失败");
        remove("temp.env");
        return;
    }

    if (rename("temp.env", filename) != 0) {
        perror("重命名文件失败");
        remove("temp.env");
    }
}

void copyFile(const char *src, const char *dest) {
    FILE *source = fopen(src, "r");
    if (!source) {
        perror("无法打开源文件");
        return;
    }

    FILE *destination = fopen(dest, "w");
    if (!destination) {
        perror("无法创建目标文件");
        fclose(source);
        return;
    }

    char buffer[1024];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        fwrite(buffer, 1, bytesRead, destination);
    }

    fclose(source);
    fclose(destination);
}

int main() {
    char id[100];
    char token[100];
    int choice;

    SetConsoleOutputCP(CP_UTF8);

    while (1) {
        printf("欢迎使用openbmclapi启动器\n");
        printf("1. 启动openbmclapi\n");
        printf("2. 编辑配置文件\n");
        printf("3. 编译openbmclapi\n");
        printf("4. 退出\n");
        printf("5. 高级选项\n");

        printf("请输入选项的编号（1-4）：");
        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("无效的输入，请重新输入\n");
            continue;
        }

        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        switch (choice) {
            case 1:
                if (system("node dist/index.js") != 0) {
                    printf("启动openbmclapi失败，请检查node.js是否安装正确，或者你选择编译了吗？\n");
                }
                break;
            case 2:
                printf("请输入ID: ");
                if (scanf("%99s", id) != 1) {
                    printf("无效的输入\n");
                    return 1;
                }

                printf("请输入Token: ");
                if (scanf("%99s", token) != 1) {
                    printf("无效的输入\n");
                    return 1;
                }

                FILE *envFile = fopen(".env", "r");
                if (!envFile) {
                    printf(".env 文件不存在，从 bin 目录复制\n");
                    copyFile("bin/.env", ".env");
                } else {
                    fclose(envFile);
                }

                replaceInFile(".env", "clid", id);
                replaceInFile(".env", "cltoken", token);
                printf("配置文件已更新\n");
                break;
            case 3:
                printf("编译openbmclapi\n");
                break;
            case 4:
                printf("退出程序\n");
                return 0;
            case 5:
                handleAdvancedOptions();
                break;
            default:
                printf("无效的选项编号\n");
                break;
        }
    }

    return 0;
}

void handleAdvancedOptions() {
    char id[100];
    char port[100];
    char puport[100];
    int danger_choice;

    printf("高级选项\n");
    printf("1. 配置端口\n");
    printf("2. 配置外部端口\n");
    printf("3. 配置BYOC\n");
    printf("4. 配置域名\n");
    printf("请输入选项的编号（1-4）：");

    if (scanf("%d", &danger_choice) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        printf("无效的输入，请重新输入\n");
        return;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    switch (danger_choice) {
        case 1:
            printf("请输入ID: ");
            if (scanf("%99s", id) != 1) {
                printf("无效的输入\n");
                return;
            }

            printf("请输入内部端口: ");
            if (scanf("%99s", port) != 1) {
                printf("无效的输入\n");
                return;
            }

            FILE *envFile = fopen(".env", "r");
            if (!envFile) {
                printf(".env 文件不存在，从 bin 目录复制\n");
                copyFile("bin/.dangerenv", ".env");
            } else {
                fclose(envFile);
            }
            break;
        case 2:
            printf("请输入ID: ");
            if (scanf("%99s", id) != 1) {
                printf("无效的输入\n");
                return;
            }

            printf("请输入外部端口: ");
            if (scanf("%99s", puport) != 1) {
                printf("无效的输入\n");
                return;
            }

            envFile = fopen(".env", "r");
            if (!envFile) {
                printf(".env 文件不存在，从 bin 目录复制\n");
                copyFile("bin/.dangerenv", ".env");
            } else {
                fclose(envFile);
            }
            break;
        case 3:
            printf("该功能尚未实现\n");
            break;
        case 4:
            printf("该功能尚未实现\n");
            break;
        default:
            printf("无效的选项编号\n");
            break;
    }
}
