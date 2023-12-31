#include "include/config.h"
#include "include/logging.h"

/*
 * 打印读取到的信息
 */
void print_config(struct Config *config) {
    printf("Server: %s\n", config->server);
    printf("port: %s\n", config->port);
    printf("network_segment: %s\n", config->network_segment);
    printf("subnet_mask: %s\n", config->subnet_mask);
    printf("user_account: %s\n", config->user_account);
    printf("user_password: %s\n", config->user_password);
    printf("wlan_ac_name: %s\n", config->wlan_ac_name);
}

/*
 * 获取文件中的登录配置
 * @return 成功返回配置结构体，否则返回NULL
 */
struct Config *get_config(char *filepath) {
    struct Config *config = (struct Config *) malloc(sizeof(struct Config));
    // 打开文件以读取信息
    FILE *file = fopen(filepath, "r");

    // 检查文件是否成功打开
    if (file == NULL) {
        perror("Error opening file");
        logging("Error opening config file");
        return NULL;
    }

    if (fscanf(file, "server = '%16[^']'\n", config->server) != 1) {
        fprintf(stderr, "Error reading server from file\n");
        logging("Error reading server from file");
        fclose(file);
        return NULL;
    }

    if (fscanf(file, "port = '%6[^']'\n", config->port) != 1) {
        fprintf(stderr, "Error reading port from file\n");
        logging("Error reading port from file");
        fclose(file);
        return NULL;
    }

    if (fscanf(file, "network_segment = '%16[^']'\n", config->network_segment) != 1) {
        fprintf(stderr, "Error reading network_segment from file\n");
        logging("Error reading network_segment from file");
        fclose(file);
        return NULL;
    }

    if (fscanf(file, "subnet_mask = '%16[^']'\n", config->subnet_mask) != 1) {
        fprintf(stderr, "Error reading network_segment from file\n");
        logging("Error reading network_segment from file");
        fclose(file);
        return NULL;
    }

    if (fscanf(file, "user_account = '%21[^']'\n", config->user_account) != 1) {
        fprintf(stderr, "Error reading user_account from file\n");
        logging("Error reading user_account from file");
        fclose(file);
        return NULL;
    }

    if (fscanf(file, "user_password = '%21[^']'\n", config->user_password) != 1) {
        fprintf(stderr, "Error reading user_password from file\n");
        logging("Error reading user_password from file");
        fclose(file);
        return NULL;
    }

    if (fscanf(file, "wlan_ac_name = '%32[^']'\n", config->wlan_ac_name) != 1) {
        bzero(config->wlan_ac_name, sizeof(config->wlan_ac_name));
        puts("WLAN_AC_NAME is empty.");
        logging("WLAN_AC_NAME is empty.");
    }

    // 关闭文件
    fclose(file);
    print_config(config);
    return config;
}