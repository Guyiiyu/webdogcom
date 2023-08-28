#include "login.h"

/*
 * 登录认证
 * @param server 认证服务器地址
 * @param port 认证服务器端口号
 * @param user_account 校园网用户名
 * @param user_password 校园网密码
 * @param ip 主机校园网ip
 * @param wlan_ac_name 连接的AC名字,可NULL
 * @return 成功请求返回1，失败返回0。具体认证信息看输出。
 * */
int login(const char *server, const char *port, const char *user_account, const char *user_password, const char *ip, const char *wlan_ac_name) {
    if(wlan_ac_name == NULL) {
        wlan_ac_name = "";
    }

    // 构建URL
    char request[4096];
    snprintf(request, sizeof(request),
             "GET /eportal/?c=Portal&a=login&callback=dr%ld&login_method=1&user_account=%s&user_password=%s&wlan_user_ip=%s&wlan_user_mac=000000000000&wlan_ac_ip=&wlan_ac_name=%s&jsVersion=3.0&_=%ld HTTP/1.1\r\n"
                       "Host: %s:%s\r\n"
                       "Connection: keep-alive\r\n"
                       "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.90 Safari/537.36 Edg/89.0.774.57\r\n"
                       "Accept: */*\r\n"
                       "Referer: http://%s/\r\n"
                       "Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6\r\n\r\n",
             (int64_t)time(NULL), user_account, user_password, ip, wlan_ac_name, (int64_t)time(NULL), server, port, server);

    // 创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    // 解析主机名
    struct hostent *server_info = gethostbyname(server);
    if (server_info == NULL) {
        perror("gethostbyname");
        close(sockfd);
        return 1;
    }

    // 设置服务器地址结构
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy(&server_addr.sin_addr.s_addr, server_info->h_addr, server_info->h_length);
    server_addr.sin_port = htons(atoi(port));

    // 连接到服务器
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sockfd);
        return 1;
    }

    // 发送HTTP请求
    if (write(sockfd, request, strlen(request)) == -1) {
        perror("write");
        close(sockfd);
        return 1;
    }

    // 接收并打印服务器响应
    char response[4096];
    ssize_t bytes_received;
    while ((bytes_received = read(sockfd, response, sizeof(response) - 1)) > 0) {
        response[bytes_received] = '\0';
        printf("%s", response);
    }

    // 关闭套接字
    close(sockfd);

    return 0;
}
