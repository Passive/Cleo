#pragma once
#include "includes.h"

#define MAX_NAME 32

// TODO: implement driver structs
typedef struct SystemQueryInformation_t {
	CHAR local_user[512];
	CHAR host[124];
	std::string inet_addr;
} SystemQueryInformation_t;
