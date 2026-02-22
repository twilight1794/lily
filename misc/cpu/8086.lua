local r8 = { AL = 0, CL = 1, DL = 2, BL = 3, AH = 4, CH = 5, DH = 6, BH = 7 }
local r16 = { AX = 0, CX = 1, DX = 2, BX = 3, SP = 4, BP = 5, SI = 6, DI = 7 }
local seg = { ES = 0, CS = 1, SS = 2, DS = 3 }

local function modrm(rd, rs, is16)
    return 0xC0 | ((is16 and r16[rs] or r8[rs]) << 3) | (is16 and r16[rd] or r8[rd])
end

local function ext(r, op, is16)
    return 0xC0 | (op << 3) | (is16 and r16[r] or r8[r])
end

return {
    id = "intel.8086",
    registros = {
        AL = { tamano = 8, desplazamiento = 0 },
        AH = { tamano = 8, desplazamiento = 8 },
        AX = { tamano = 16, desplazamiento = 0 },
        CL = { tamano = 8, desplazamiento = 16 },
        CH = { tamano = 8, desplazamiento = 24 },
        CX = { tamano = 16, desplazamiento = 16 },
        DL = { tamano = 8, desplazamiento = 32 },
        DH = { tamano = 8, desplazamiento = 40 },
        DX = { tamano = 16, desplazamiento = 32 },
        BL = { tamano = 8, desplazamiento = 48 },
        BH = { tamano = 8, desplazamiento = 56 },
        BX = { tamano = 16, desplazamiento = 48 },
        SP = { tamano = 16, desplazamiento = 64 },
        BP = { tamano = 16, desplazamiento = 80 },
        SI = { tamano = 16, desplazamiento = 96 },
        DI = { tamano = 16, desplazamiento = 112 },
        IP = { tamano = 16, desplazamiento = 128 },
        ES = { tamano = 16, desplazamiento = 144 },
        CS = { tamano = 16, desplazamiento = 160 },
        SS = { tamano = 16, desplazamiento = 176 },
        DS = { tamano = 16, desplazamiento = 192 },
        CF = { tamano = 1, desplazamiento = 208 },
        PF = { tamano = 1, desplazamiento = 209 },
        AF = { tamano = 1, desplazamiento = 210 },
        ZF = { tamano = 1, desplazamiento = 211 },
        SF = { tamano = 1, desplazamiento = 212 },
        TF = { tamano = 1, desplazamiento = 213 },
        IF = { tamano = 1, desplazamiento = 214 },
        DF = { tamano = 1, desplazamiento = 215 },
        OF = { tamano = 1, desplazamiento = 216 }
    },
    ensamble = {
        AAA = { 0x37 },
        AAD = { 0xD5, 0x0A },
        AAM = { 0xD4, 0x0A },
        AAS = { 0x3F },
        ADC = {
            { { "r8", "r8" }, function(r1, r2) return { 0x12, modrm(r1, r2, false) } end },
            { { "r16", "r16" }, function(r1, r2) return { 0x13, modrm(r1, r2, true) } end },
            { { "al", "imm8" }, function(i) return { 0x14, i } end },
            { { "ax", "imm16" }, function(i) return { 0x15, i & 0xFF, (i >> 8) & 0xFF } end }
        },
        ADD = {
            { { "r8", "r8" }, function(r1, r2) return { 0x02, modrm(r1, r2, false) } end },
            { { "r16", "r16" }, function(r1, r2) return { 0x03, modrm(r1, r2, true) } end },
            { { "al", "imm8" }, function(i) return { 0x04, i } end },
            { { "ax", "imm16" }, function(i) return { 0x05, i & 0xFF, (i >> 8) & 0xFF } end }
        },
        AND = {
            { { "r8", "r8" }, function(r1, r2) return { 0x22, modrm(r1, r2, false) } end },
            { { "r16", "r16" }, function(r1, r2) return { 0x23, modrm(r1, r2, true) } end },
            { { "al", "imm8" }, function(i) return { 0x24, i } end },
            { { "ax", "imm16" }, function(i) return { 0x25, i & 0xFF, (i >> 8) & 0xFF } end }
        },
        CALL = {
            { { "rel16" }, function(e) return { 0xE8, e & 0xFF, (e >> 8) & 0xFF } end },
            { { "r16" }, function(r) return { 0xFF, ext(r, 2, true) } end }
        },
        CBW = { 0x98 },
        CLC = { 0xF8 },
        CLD = { 0xFC },
        CLI = { 0xFA },
        CMC = { 0xF5 },
        CMP = {
            { { "r8", "r8" }, function(r1, r2) return { 0x3A, modrm(r1, r2, false) } end },
            { { "r16", "r16" }, function(r1, r2) return { 0x3B, modrm(r1, r2, true) } end },
            { { "al", "imm8" }, function(i) return { 0x3C, i } end },
            { { "ax", "imm16" }, function(i) return { 0x3D, i & 0xFF, (i >> 8) & 0xFF } end }
        },
        CMPSB = { 0xA6 },
        CMPSW = { 0xA7 },
        CWD = { 0x99 },
        DAA = { 0x27 },
        DAS = { 0x2F },
        DEC = {
            { { "r16" }, function(r) return 0x48 | r16[r] end },
            { { "r8" }, function(r) return { 0xFE, ext(r, 1, false) } end }
        },
        DIV = {
            { { "r8" }, function(r) return { 0xF6, ext(r, 6, false) } end },
            { { "r16" }, function(r) return { 0xF7, ext(r, 6, true) } end }
        },
        HLT = { 0xF4 },
        IDIV = {
            { { "r8" }, function(r) return { 0xF6, ext(r, 7, false) } end },
            { { "r16" }, function(r) return { 0xF7, ext(r, 7, true) } end }
        },
        IMUL = {
            { { "r8" }, function(r) return { 0xF6, ext(r, 5, false) } end },
            { { "r16" }, function(r) return { 0xF7, ext(r, 5, true) } end }
        },
        IN = {
            { { "al", "imm8" }, function(i) return { 0xE4, i } end },
            { { "ax", "imm8" }, function(i) return { 0xE5, i } end },
            { { "al", "dx" }, { 0xEC } },
            { { "ax", "dx" }, { 0xED } }
        },
        INC = {
            { { "r16" }, function(r) return 0x40 | r16[r] end },
            { { "r8" }, function(r) return { 0xFE, ext(r, 0, false) } end }
        },
        INT = {
            { { "3" }, { 0xCC } },
            { { "imm8" }, function(i) return { 0xCD, i } end }
        },
        INTO = { 0xCE },
        IRET = { 0xCF },
        JA = { { { "e" }, function(e) return { 0x77, e } end } },
        JAE = { { { "e" }, function(e) return { 0x73, e } end } },
        JB = { { { "e" }, function(e) return { 0x72, e } end } },
        JBE = { { { "e" }, function(e) return { 0x76, e } end } },
        JC = { { { "e" }, function(e) return { 0x72, e } end } },
        JE = { { { "e" }, function(e) return { 0x74, e } end } },
        JG = { { { "e" }, function(e) return { 0x7F, e } end } },
        JGE = { { { "e" }, function(e) return { 0x7D, e } end } },
        JL = { { { "e" }, function(e) return { 0x7C, e } end } },
        JLE = { { { "e" }, function(e) return { 0x7E, e } end } },
        JNA = { { { "e" }, function(e) return { 0x76, e } end } },
        JNAE = { { { "e" }, function(e) return { 0x72, e } end } },
        JNB = { { { "e" }, function(e) return { 0x73, e } end } },
        JNBE = { { { "e" }, function(e) return { 0x77, e } end } },
        JNC = { { { "e" }, function(e) return { 0x73, e } end } },
        JNE = { { { "e" }, function(e) return { 0x75, e } end } },
        JNG = { { { "e" }, function(e) return { 0x7E, e } end } },
        JNGE = { { { "e" }, function(e) return { 0x7C, e } end } },
        JNL = { { { "e" }, function(e) return { 0x7D, e } end } },
        JNLE = { { { "e" }, function(e) return { 0x7F, e } end } },
        JNO = { { { "e" }, function(e) return { 0x71, e } end } },
        JNP = { { { "e" }, function(e) return { 0x7B, e } end } },
        JNS = { { { "e" }, function(e) return { 0x79, e } end } },
        JNZ = { { { "e" }, function(e) return { 0x75, e } end } },
        JO = { { { "e" }, function(e) return { 0x70, e } end } },
        JP = { { { "e" }, function(e) return { 0x7A, e } end } },
        JPE = { { { "e" }, function(e) return { 0x7A, e } end } },
        JPO = { { { "e" }, function(e) return { 0x7B, e } end } },
        JS = { { { "e" }, function(e) return { 0x78, e } end } },
        JZ = { { { "e" }, function(e) return { 0x74, e } end } },
        JCXZ = { 0xE3 },
        JMP = {
            { { "rel8" }, function(e) return { 0xEB, e } end },
            { { "rel16" }, function(e) return { 0xE9, e & 0xFF, (e >> 8) & 0xFF } end },
            { { "r16" }, function(r) return { 0xFF, ext(r, 4, true) } end }
        },
        LAHF = { 0x9F },
        LEA = {
            { { "r16", "m" }, function(r, m) return { 0x8D, modrm(r, m, true) } end }
        },
        LDS = {
            { { "r16", "m" }, function(r, m) return { 0xC5, modrm(r, m, true) } end }
        },
        LES = {
            { { "r16", "m" }, function(r, m) return { 0xC4, modrm(r, m, true) } end }
        },
        LODSB = { 0xAC },
        LODSW = { 0xAD },
        LOOP = { { { "e" }, function(e) return { 0xE2, e } end } },
        LOOPE = { { { "e" }, function(e) return { 0xE1, e } end } },
        LOOPNE = { { { "e" }, function(e) return { 0xE0, e } end } },
        LOOPNZ = { { { "e" }, function(e) return { 0xE0, e } end } },
        LOOPZ = { { { "e" }, function(e) return { 0xE1, e } end } },
        MOV = {
            { { "r8", "r8" }, function(rd, rs) return { 0x8A, modrm(rd, rs, false) } end },
            { { "r16", "r16" }, function(rd, rs) return { 0x8B, modrm(rd, rs, true) } end },
            { { "r8", "imm8" }, function(r, i) return { 0xB0 | r8[r], i } end },
            { { "r16", "imm16" }, function(r, i) return { 0xB8 | r16[r], i & 0xFF, (i >> 8) & 0xFF } end }
        },
        MOVSB = { 0xA4 },
        MOVSW = { 0xA5 },
        MUL = {
            { { "r8" }, function(r) return { 0xF6, ext(r, 4, false) } end },
            { { "r16" }, function(r) return { 0xF7, ext(r, 4, true) } end }
        },
        NEG = {
            { { "r8" }, function(r) return { 0xF6, ext(r, 3, false) } end },
            { { "r16" }, function(r) return { 0xF7, ext(r, 3, true) } end }
        },
        NOP = { 0x90 },
        NOT = {
            { { "r8" }, function(r) return { 0xF6, ext(r, 2, false) } end },
            { { "r16" }, function(r) return { 0xF7, ext(r, 2, true) } end }
        },
        OR = {
            { { "r8", "r8" }, function(r1, r2) return { 0x0A, modrm(r1, r2, false) } end },
            { { "r16", "r16" }, function(r1, r2) return { 0x0B, modrm(r1, r2, true) } end },
            { { "al", "imm8" }, function(i) return { 0x0C, i } end },
            { { "ax", "imm16" }, function(i) return { 0x0D, i & 0xFF, (i >> 8) & 0xFF } end }
        },
        OUT = {
            { { "imm8", "al" }, function(i) return { 0xE6, i } end },
            { { "imm8", "ax" }, function(i) return { 0xE7, i } end },
            { { "dx", "al" }, { 0xEE } },
            { { "dx", "ax" }, { 0xEF } }
        },
        POP = {
            { { "r16" }, function(r) return 0x58 | r16[r] end },
            { { "es" }, { 0x07 } },
            { { "ss" }, { 0x17 } },
            { { "ds" }, { 0x1F } }
        },
        POPF = { 0x9D },
        PUSH = {
            { { "r16" }, function(r) return 0x50 | r16[r] end },
            { { "es" }, { 0x06 } },
            { { "cs" }, { 0x0E } },
            { { "ss" }, { 0x16 } },
            { { "ds" }, { 0x1E } }
        },
        PUSHF = { 0x9C },
        RCL = {
            { { "r8", "1" }, function(r) return { 0xD0, ext(r, 2, false) } end },
            { { "r16", "1" }, function(r) return { 0xD1, ext(r, 2, true) } end },
            { { "r8", "cl" }, function(r) return { 0xD2, ext(r, 2, false) } end },
            { { "r16", "cl" }, function(r) return { 0xD3, ext(r, 2, true) } end }
        },
        RCR = {
            { { "r8", "1" }, function(r) return { 0xD0, ext(r, 3, false) } end },
            { { "r16", "1" }, function(r) return { 0xD1, ext(r, 3, true) } end },
            { { "r8", "cl" }, function(r) return { 0xD2, ext(r, 3, false) } end },
            { { "r16", "cl" }, function(r) return { 0xD3, ext(r, 3, true) } end }
        },
        RET = { { {}, { 0xC3 } } },
        RETN = {
            { {}, { 0xC3 } },
            { { "imm16" }, function(i) return { 0xC2, i & 0xFF, (i >> 8) & 0xFF } end }
        },
        RETF = {
            { {}, { 0xCB } },
            { { "imm16" }, function(i) return { 0xCA, i & 0xFF, (i >> 8) & 0xFF } end }
        },
        ROL = {
            { { "r8", "1" }, function(r) return { 0xD0, ext(r, 0, false) } end },
            { { "r16", "1" }, function(r) return { 0xD1, ext(r, 0, true) } end }
        },
        ROR = {
            { { "r8", "1" }, function(r) return { 0xD0, ext(r, 1, false) } end },
            { { "r16", "1" }, function(r) return { 0xD1, ext(r, 1, true) } end }
        },
        SAHF = { 0x9E },
        SAL = {
            { { "r8", "1" }, function(r) return { 0xD0, ext(r, 4, false) } end },
            { { "r16", "1" }, function(r) return { 0xD1, ext(r, 4, true) } end }
        },
        SAR = {
            { { "r8", "1" }, function(r) return { 0xD0, ext(r, 7, false) } end },
            { { "r16", "1" }, function(r) return { 0xD1, ext(r, 7, true) } end }
        },
        SBB = {
            { { "r8", "r8" }, function(r1, r2) return { 0x1A, modrm(r1, r2, false) } end },
            { { "r16", "r16" }, function(r1, r2) return { 0x1B, modrm(r1, r2, true) } end }
        },
        SCASB = { 0xAE },
        SCASW = { 0xAF },
        SHL = {
            { { "r8", "1" }, function(r) return { 0xD0, ext(r, 4, false) } end },
            { { "r16", "1" }, function(r) return { 0xD1, ext(r, 4, true) } end }
        },
        SHR = {
            { { "r8", "1" }, function(r) return { 0xD0, ext(r, 5, false) } end },
            { { "r16", "1" }, function(r) return { 0xD1, ext(r, 5, true) } end }
        },
        STC = { 0xF9 },
        STD = { 0xFD },
        STI = { 0xFB },
        STOSB = { 0xAA },
        STOSW = { 0xAB },
        SUB = {
            { { "r8", "r8" }, function(r1, r2) return { 0x2A, modrm(r1, r2, false) } end },
            { { "r16", "r16" }, function(r1, r2) return { 0x2B, modrm(r1, r2, true) } end },
            { { "al", "imm8" }, function(i) return { 0x2C, i } end },
            { { "ax", "imm16" }, function(i) return { 0x2D, i & 0xFF, (i >> 8) & 0xFF } end }
        },
        TEST = {
            { { "r8", "r8" }, function(r1, r2) return { 0x84, modrm(r1, r2, false) } end },
            { { "r16", "r16" }, function(r1, r2) return { 0x85, modrm(r1, r2, true) } end },
            { { "al", "imm8" }, function(i) return { 0xA8, i } end },
            { { "ax", "imm16" }, function(i) return { 0xA9, i & 0xFF, (i >> 8) & 0xFF } end }
        },
        WAIT = { 0x9B },
        XCHG = {
            { { "ax", "r16" }, function(r) return 0x90 | r16[r] end },
            { { "r8", "r8" }, function(r1, r2) return { 0x86, modrm(r1, r2, false) } end },
            { { "r16", "r16" }, function(r1, r2) return { 0x87, modrm(r1, r2, true) } end }
        },
        XLAT = { 0xD7 },
        XOR = {
            { { "r8", "r8" }, function(r1, r2) return { 0x32, modrm(r1, r2, false) } end },
            { { "r16", "r16" }, function(r1, r2) return { 0x33, modrm(r1, r2, true) } end },
            { { "al", "imm8" }, function(i) return { 0x34, i } end },
            { { "ax", "imm16" }, function(i) return { 0x35, i & 0xFF, (i >> 8) & 0xFF } end }
        }
    }
}