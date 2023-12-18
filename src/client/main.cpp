#include <QCoreApplication>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>
#include <errno.h>

#define MAX_PAYLOAD 1024

int fd;
sockaddr_nl src_addr, dest_addr;
iovec iov;
nlmsghdr* nlh = nullptr;
msghdr msg;

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    qDebug() << "Start client";

    const int pidClient = 3;
    const int pidServer = 4;

    // Открываю сокет протокол NETLINK_GENERIC
    fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_GENERIC);
    if (fd < 0)
    {
        perror("socket()");
        return -1;
    }

    // Заполняем структуру нашего локального адреса
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_groups = 0;
    src_addr.nl_pid = pidClient;

    // Привязываем сокет к локальному адресу
    if (bind(fd, reinterpret_cast<sockaddr*>(&src_addr), sizeof(src_addr)) < 0)
    {
        perror("bind()");
        close(fd);
        return -1;
    }

    // Заполним сообщение с заголовком(размер заголовка + максимальная нагрузка)
    nlh = reinterpret_cast<nlmsghdr*>(malloc(NLMSG_SPACE(MAX_PAYLOAD)));
    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = src_addr.nl_pid;
    nlh->nlmsg_flags = 0;

    // Составляем запрос серверу в виде Json
    QJsonObject request;
    request["action"] = "plus";
    request["arg_1"] = 1;
    request["arg_2"] = 2;

    //Добавляем запрос в полезную нагрузку
    strcpy(reinterpret_cast<char*>(NLMSG_DATA(nlh)), QJsonDocument(request).toJson().constData());

    //Далее заполняем структуру для ввода-вывода
    iov.iov_base = reinterpret_cast<void*>(nlh);  // базовый адрес заголовка сообщения netlink
    iov.iov_len = nlh->nlmsg_len;                 // длина сообщения по сетевой ссылке

    // Заполняем структуру адреса сервера
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = pidServer;
    dest_addr.nl_groups = 0;

    // заполняем структуру, описывающую сообщения, отправленные "sendmsg" и полученные "recvmsg"
    msg.msg_name = (void*)&dest_addr;     // Адрес для отправки / получения с него.
    msg.msg_namelen = sizeof(dest_addr);  // Длина адресных данных.
    msg.msg_iov = &iov;                   // Массив данных для отправки/получения.
    msg.msg_iovlen = 1;                   // В нашем случае всего 1 элемент в массиве

    sleep(3);
    // Отправляем запрос серверу
    int bytes = sendmsg(fd, &msg, 0);
    if (bytes < 0)
    {
        perror("sendmsg()");
        return -1;
    }

    // Далее ждем ответ сервера
    qDebug() << "Wait server";
    recvmsg(fd, &msg, 0);
    QJsonObject respond = QJsonDocument::fromJson(reinterpret_cast<char*>(NLMSG_DATA(nlh))).object();
    if (respond.contains("result"))
    {
        // Тут совершаются какие то действия над результатом сервера
        qDebug() << respond.value("result").toInt();
    }
    else
    {
        qCritical() << "Result not found";
    }

    close(fd);
    return app.exec();
}
