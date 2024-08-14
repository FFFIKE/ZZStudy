#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

// �źŴ�����
void callback(int num)
{
    while (1)
    {
        pid_t pid = waitpid(-1, NULL, WNOHANG);
        if (pid <= 0)
        {
            printf("�ӽ�����������, �����ӽ��̱����������\n");
            break;
        }
        printf("child die, pid = %d\n", pid);
    }
}

int childWork(int cfd);
int main()
{
    // 1. �����������׽���
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
    {
        perror("socket");
        exit(0);
    }

    // 2. ��socket()����ֵ�ͱ��ص�IP�˿ڰ󶨵�һ��
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10000);   // ��˶˿�
    // INADDR_ANY������������IP, ����������������������IP��ַ
    // �������Դ�������һ��IP��ַ
    // �����һ�����ڱ��صİ󶨲���
    addr.sin_addr.s_addr = INADDR_ANY;  // ������ֵΪ0 == 0.0.0.0
    //    inet_pton(AF_INET, "192.168.237.131", &addr.sin_addr.s_addr);
    int ret = bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("bind");
        exit(0);
    }

    // 3. ���ü���
    ret = listen(lfd, 128);
    if (ret == -1)
    {
        perror("listen");
        exit(0);
    }

    // ע���źŵĲ�׽
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = callback;
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD, &act, NULL);

    // ���ܶ���ͻ�������, ����Ҫѭ������ accept
    while (1)
    {
        // 4. �����ȴ������ܿͻ�������
        struct sockaddr_in cliaddr;
        int clilen = sizeof(cliaddr);
        int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &clilen);
        if (cfd == -1)
        {
            if (errno == EINTR)
            {
                // accept���ñ��ź��ж���, �������, ������-1
                // ���µ���һ��accept
                continue;
            }
            perror("accept");
            exit(0);

        }
        // ��ӡ�ͻ��˵ĵ�ַ��Ϣ
        char ip[24] = { 0 };
        printf("�ͻ��˵�IP��ַ: %s, �˿�: %d\n",
            inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, sizeof(ip)),
            ntohs(cliaddr.sin_port));
        // �µ������Ѿ�������, �����ӽ���, ���ӽ��̺�����ͻ���ͨ��
        pid_t pid = fork();
        if (pid == 0)
        {
            // �ӽ��� -> �Ϳͻ���ͨ��
            // ͨ�ŵ��ļ�������cfd���������ӽ�����
            // �ӽ��̲��������
            close(lfd);
            while (1)
            {
                int ret = childWork(cfd);
                if (ret <= 0)
                {
                    break;
                }
            }
            // �˳��ӽ���
            close(cfd);
            exit(0);
        }
        else if (pid > 0)
        {
            // �����̲��Ϳͻ���ͨ��
            close(cfd);
        }
    }
    return 0;
}


// 5. �Ϳͻ���ͨ��
int childWork(int cfd)
{

    // ��������
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    int len = read(cfd, buf, sizeof(buf));
    if (len > 0)
    {
        printf("�ͻ���say: %s\n", buf);
        write(cfd, buf, len);
    }
    else if (len == 0)
    {
        printf("�ͻ��˶Ͽ�������...\n");
    }
    else
    {
        perror("read");
    }

    return len;
}


