#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief App entry point. This function can never exit.
 * @param arg necessary in oder to be compatible with osThreadFunc_t remains unused, nullptr is injected, don't dereference it.
 */
void app_main(void* arg);

#ifdef __cplusplus
}
#endif