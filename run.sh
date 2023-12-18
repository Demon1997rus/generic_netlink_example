#!/bin/bash

# Путь к директории с бинарниками
BIN_DIR="./bin"

# Запуск сервера
$BIN_DIR/generic_netlink_server &

# Пауза для уверенности, что сервер успел запуститься
sleep 1

# Запуск клиента
$BIN_DIR/generic_netlink_client

# Ожидание завершения сервера
wait
