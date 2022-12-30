#pragma once

#include "cstdint"

enum class FTPMessageType : uint8_t
{
    USERNAME = 1,
    PASSWORD,
    DOWNLOAD_FILE,
    UPLOAD_FILE,
    GET_HELP,
    QUIT,


    SHORT_RESPONSE,

    FILE_CONTENT,
    HELP_CONTENT
};

