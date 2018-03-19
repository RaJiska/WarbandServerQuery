#pragma once

#define FREEZE_REGS pushad
#define RESTORE_REGS popad

void hook_PlayerJoined(void);
void hook_ChatMessageSent(void);