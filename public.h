#pragma once

#ifdef _WIN32
#include <SDKDDKVer.h>
#endif

#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#define ASIO_STANDALONE
#define ASIO_HAS_STD_CHRONO
#define _CRT_NONSTDC_NO_DEPRECATE
#ifndef _WIN32
#define LINUX
#endif

#ifdef _WIN32
#pragma warning(disable : 4503)
#pragma warning(disable : 4996)
#endif

#ifdef _WIN32
#include <SDKDDKVer.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <algorithm>
#include <array>
#include <asio.hpp>
#include <chrono>
#include <condition_variable>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include <iostream>
#ifndef _WIN32
#include <arpa/inet.h>
#include <dirent.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <error.h>
#else
#include <WinSock2.h>
#include <direct.h>
#include <io.h>
#endif

typedef asio::ip::tcp::endpoint tcp_endpoint;
typedef asio::ip::udp::endpoint udp_endpoint;
typedef asio::ip::tcp::socket tcp_socket;
typedef asio::ip::udp::socket udp_socket;
typedef asio::ip::tcp::resolver tcp_resolver;

using asio::ip::address_v4;

uint16_t const kPort = 4000;