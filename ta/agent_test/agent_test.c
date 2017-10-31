/*
 * Copyright (c) 2016, Hisilicon Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <tee_ta_api.h>
#include <tee_api.h>
#include <stdlib.h>
#include <string.h>
#include <trace.h>
#include <utee_defines.h>
#include <utee_syscalls.h>
#include <tee_internal_api_extensions.h>

TEE_Result TA_CreateEntryPoint(void)
{
	return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void)
{
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
		TEE_Param params[4],
		void **session_ctx)
{
	(void)param_types;
	(void)params;
	(void)session_ctx;
	return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void *session_ctx)
{
	(void)session_ctx;
}

TEE_Result TA_InvokeCommandEntryPoint(void *session_ctx,
		uint32_t cmd_id, uint32_t param_types,
		TEE_Param params[4])
{
	TEE_Result res;
	char *va;
	unsigned int i, len;

	(void)session_ctx;
	(void)param_types;
	(void)cmd_id;

	len = params[0].value.b;
	va = malloc(len);
	if (!va)
		return TEE_ERROR_OUT_OF_MEMORY;

	memset(va, 0x55, len);

	res = TEE_AgentCall(params[0].value.a, va, len);
	if (res != TEE_SUCCESS) {
		EMSG("agentcall failed rc :%d\n", res);
		return res;
	}

	for (i = 0; i < len; i++)
		if (va[i] != (0x55 + 1))
			break;

	if (i != len)
		res = TEE_ERROR_GENERIC;

	return res;
}
