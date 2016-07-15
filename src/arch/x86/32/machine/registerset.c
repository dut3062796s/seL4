/*
 * Copyright 2014, General Dynamics C4 Systems
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(GD_GPL)
 */

#include <config.h>
#include <arch/api/constants.h>
#include <arch/machine/registerset.h>
#include <arch/machine/fpu.h>
#include <arch/object/structures.h>
#include <machine/debug.h>

const register_t msgRegisters[] = {
    EDI, EBP
};

const register_t frameRegisters[] = {
    FaultIP, ESP, FLAGS, EAX, EBX, ECX, EDX, ESI, EDI, EBP
};

const register_t gpRegisters[] = {
    TLS_BASE, FS, GS
};

const register_t exceptionMessage[] = {
    [seL4_UserException_FaultIP] = FaultIP,
    [seL4_UserException_SP] = ESP,
    [seL4_UserException_FLAGS] = FLAGS
};

const register_t syscallMessage[] = {
    [seL4_UnknownSyscall_EAX] = EAX,
    [seL4_UnknownSyscall_EBX] = EBX,
    [seL4_UnknownSyscall_ECX] = ECX,
    [seL4_UnknownSyscall_EDX] = EDX,
    [seL4_UnknownSyscall_ESI] = ESI,
    [seL4_UnknownSyscall_EDI] = EDI,
    [seL4_UnknownSyscall_EBP] = EBP,
    [seL4_UnknownSyscall_FaultIP] = FaultIP,
    [seL4_UnknownSyscall_SP] = ESP,
    [seL4_UnknownSyscall_FLAGS] = FLAGS
};

#ifdef CONFIG_VTX
const register_t crExitRegs[] = {
    EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI
};
#endif

void Mode_initContext(user_context_t* context)
{
    context->registers[EAX] = 0;
    context->registers[EBX] = 0;
    context->registers[ECX] = 0;
    context->registers[EDX] = 0;
    context->registers[ESI] = 0;
    context->registers[EDI] = 0;
    context->registers[EBP] = 0;
    context->registers[DS] = SEL_DS_3;
    context->registers[ES] = SEL_DS_3;
    context->registers[FS] = SEL_NULL;
    context->registers[GS] = SEL_NULL;
    context->registers[ESP] = 0;
}

word_t sanitiseRegister(register_t reg, word_t v)
{
    if (reg == FLAGS) {
        /* Set architecturally defined high and low bits */
        v |=  FLAGS_HIGH;
        v &= ~FLAGS_LOW;
        /* require user to have interrupts and no traps */
        v |=  FLAGS_IF;
        /* Disallow setting Trap Flag: use the API instead */
        v &= ~FLAGS_TF;
        /* remove any other bits that shouldn't be set */
        v &=  FLAGS_MASK;
    }
    if (reg == FS || reg == GS) {
        if (v != SEL_TLS && v != SEL_IPCBUF) {
            v = 0;
        }
    }
    return v;
}
