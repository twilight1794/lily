-- Especificación del microprocesador Intel 8086

return {
    id = "intel.8086",
    registros = {
        AL = {
            tamano = 8,
            desplazamiento = 0
        },
        AH = {
            tamano = 8,
            desplazamiento = 8
        },
        AX = {
            tamano = 16,
            desplazamiento = 0
        },
        CL = {
            tamano = 8,
            desplazamiento = 16
        },
        CH = {
            tamano = 8,
            desplazamiento = 24
        },
        CX = {
            tamano = 16,
            desplazamiento = 16
        },
        DL = {
            tamano = 8,
            desplazamiento = 32
        },
        DH = {
            tamano = 8,
            desplazamiento = 40
        },
        DX = {
            tamano = 16,
            desplazamiento = 32
        },
        BL = {
            tamano = 8,
            desplazamiento = 48
        },
        BH = {
            tamano = 8,
            desplazamiento = 56
        },
        BX = {
            tamano = 16,
            desplazamiento = 48
        },
        SP = {
            tamano = 16,
            desplazamiento = 64
        },
        BP = {
            tamano = 16,
            desplazamiento = 80
        },
        SI = {
            tamano = 16,
            desplazamiento = 96
        },
        DI = {
            tamano = 16,
            desplazamiento = 112
        },
        IP = {
            tamano = 16,
            desplazamiento = 128
        },
        ES = {
            tamano = 16,
            desplazamiento = 144
        },
        CS = {
            tamano = 16,
            desplazamiento = 160
        },
        SS = {
            tamano = 16,
            desplazamiento = 176
        },
        DS = {
            tamano = 16,
            desplazamiento = 192
        },
        CF = {
            tamano = 1,
            desplazamiento = 208
        },
        PF = {
            tamano = 1,
            desplazamiento = 209
        },
        AF = {
            tamano = 1,
            desplazamiento = 210
        },
        ZF = {
            tamano = 1,
            desplazamiento = 211
        },
        SF = {
            tamano = 1,
            desplazamiento = 212
        },
        TF = {
            tamano = 1,
            desplazamiento = 213
        },
        IF = {
            tamano = 1,
            desplazamiento = 214
        },
        DF = {
            tamano = 1,
            desplazamiento = 215
        },
        OF = {
            tamano = 1,
            desplazamiento = 216
        },
    },
    operandos = {
    },
    ensamble = {
        AAA = { 0x37 },
        AAD = { 0xd5, 0x0a },
        AAM = { 0xd4, 0x0a },
        AAS = { 0x3f },
        ADC = {
        },
        ADD = {
        },
        AND = {
        },
        CALL = {
        },
        CBW = { 0x66, 0x98 },
        CLC = { 0xf8 },
        CLD = { 0xfc },
        CLI = { 0xfa },
        CMC = { 0xf5 },
        CMP = {
        },
        CMPSB = { 0xa6 },
        CMPSW = { 0x66, 0xa7 },
        CWD = { 0x66, 0x99 },
        DAA = { 0x27 },
        DAS = { 0x2f },
        DEC = {
        },
        DIV = {
        },
        ESC = {
        },
        HLT = { 0xf4 },
        IDIV = {
        },
        IMUL = {
        },
        IN = {
            --falta al, imm8; ax, imm8
            {
                { "al", "dx" }
            },
            {
                { "ax", "dx" }
            },
        },
        INC = {
        },
        INT = {
        },
        INTO = { 0xce },
        IRET = { 0xcf },
        JA = {
        },
        JAE = {
        },
        JB = {
        },
        JBE = {
        },
        JC = {
        },
        JE = {
        },
        JG = {
        },
        JGE = {
        },
        JL = {
        },
        JLE = {
        },
        JNA = {
        },
        JNAE = {
        },
        JNB = {
        },
        JNBE = {
        },
        JNC = {
        },
        JNE = {
        },
        JNG = {
        },
        JNGE = {
        },
        JNL = {
        },
        JNLE = {
        },
        JNO = {
        },
        JNP = {
        },
        JNS = {
        },
        JNZ = {
        },
        JO = {
        },
        JP = {
        },
        JPE = {
        },
        JPO = {
        },
        JS = {
        },
        JZ = {
        },
        JCXZ = { 0xe3 },
        JMP = {
        },
        LAHF = { 0x9f },
        LDS = {
        },
        LEA = {
        },
        LES = {        },
        --LOCK = {        }, --es un prefijo
        LODSB = { 0xac },
        LODSW = { 0x66, 0xad },
        LOOP = {
        },
        LOOPE = {
        },
        LOOPNE = {
        },
        LOOPNZ = {
        },
        LOOPZ = {
        },
        MOV = {
        },
        MOVSB = { 0xa4 },
        MOVSW = { 0x66, 0xa5 },
        MUL = {
        },
        NEG = {
        },
        NOP = { 0x90 },
        NOT = {
        },
        OR = {
        },
        OUT = {
        },
        POP = {
        },
        POPF = { 0x9d },
        PUSH = {
        },
        PUSHF = { 0x9c },
        RCL = {
        },
        RCR = {
        },
        --
        --[[REP = {
        },
        REPE = {
        },
        REPNE = {
        },
        REPNZ = {
        },
        REPZ = {
            },]]
        RET = { -- tratar
        },
        RETN = {
            {
                {},
                { 0xc3 }
            },
            {
                { "imm16" },
                function (r1)
                    return { 0xc2, r1 } -- tratar valor binario
                end
            }
        },
        RETF = {
            {
                {},
                { 0xcb }
            },
            {
                { "imm16" },
                function (r1)
                    return { 0xca, r1 }
                end
            }
        },
        ROL = {
        },
        ROR = {
        },
        SAHF = { 0x9e },
        SAL = {
        },
        SAR = {
        },
        SBB = {
        },
        SCASB = { 0xae },
        SCASW = { 0x66, 0xaf },
        SHL = {
        },
        SHR = {
        },
        STC = { 0xf9 },
        STD = { 0xfd },
        STI = { 0xfb },
        STOSB = { 0xaa },
        STOSW = { 0x66, 0xab },
        SUB = {
        },
        TEST = {
        },
        WAIT = { 0x9b },
        XCHG = {
        },
        XLAT = {
        },
        XOR = {
        },
    },
    opcodes = {
    },
    desensamble = {
    }
}
