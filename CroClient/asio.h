#pragma once
#pragma warning (push, 0)
#define ASIO_HAS_STD_ARRAY
#define ASIO_HAS_STD_ADDRESSOF
#define ASIO_HAS_STD_TYPE_TRAITS
#define ASIO_HAS_STD_SHARED_PTR
#define ASIO_HAS_CSTDINT
#define ASIO_HAS_STD_SYSTEM_ERROR
#define ASIO_STANDALONE
#define ASIO_HEADERONLY
#include <asio.hpp>
#pragma warning (pop)

using namespace asio::ip;