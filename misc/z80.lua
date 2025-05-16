-- Especificación del microprocesador Zilog Z80

--[[
  Habrá disponible un objeto global para controlar la plataforma, L
  L.leer_memoria(pos)
  L.escribir_memoria(pos, v)
  L.leer_registro(reg)
  L.escribir_registro(reg, v)
]]

-- Correspondencias valor->bits
r_vals = {
    { 0, 'B' },
    { 1, 'C' },
    { 2, 'D' },
    { 3, 'E' },
    { 4, 'H' },
    { 5, 'L' },
    { 7, 'A' }
}
dd_vals = {
    { 0, 'BC' },
    { 1, 'DE' },
    { 2, 'HL' },
    { 3, 'SP' },
}

-- 
return {
    id = "zilog.z80",
    registros = {
        -- Registros principales
        A = {
            tamano = 8,
            desplazamiento = 0
        },
        F = {
            tamano = 8,
            desplazamiento = 8
        },
        AF = {
            tamano = 16,
            desplazamiento = 0
        },
        B = {
            tamano = 8,
            desplazamiento = 16
        },
        C = {
            tamano = 8,
            desplazamiento = 24
        },
        BC = {
            tamano = 16,
            desplazamiento = 16
        },
        D = {
            tamano = 8,
            desplazamiento = 32
        },
        E = {
            tamano = 8,
            desplazamiento = 40
        },
        DE = {
            tamano = 16,
            desplazamiento = 32
        },
        H = {
            tamano = 8,
            desplazamiento = 48
        },
        L = {
            tamano = 8,
            desplazamiento = 56
        },
        HL = {
            tamano = 16,
            desplazamiento = 48
        },
        -- Registros de propósito especial
        IX = {
            tamano = 16,
            desplazamiento = 64
        },
        IY = {
            tamano = 16,
            desplazamiento = 80
        },
        PC = {
            tamano = 16,
            desplazamiento = 96
        },
        SP = {
            tamano = 16,
            desplazamiento = 108
        },
        I = {
            tamano = 8,
            desplazamiento = 116
        },
        R = {
            tamano = 8,
            desplazamiento = 124
        },
        -- Registros alternativos principales
        AFX = {
            tamano = 16,
            desplazamiento = 132
        },
        XX = {
            tamano = 64,
            desplazamiento = 132
        },
        -- Banderas
        CF = {
            tamano = 1,
            desplazamiento = 8
        },
        NF = {
            tamano = 1,
            desplazamiento = 9
        },
        VF = {
            tamano = 1,
            desplazamiento = 10
        },
        HF = {
            tamano = 1,
            desplazamiento = 12
        },
        ZF = {
            tamano = 1,
            desplazamiento = 14
        },
        SF = {
            tamano = 1,
            desplazamiento = 15
        },
    },
    grupos = {
        r = "(a|b|c|d|e|h|l)",
        n = "%d\+"
        qq = "(bc|de|hl|af)",
        m = "(hl)"
    },
    ensamble = {
        LD = {
            -- Grupo de carga de 8 bits
            {
                { "r", "r" },
                function (r1, r2) return 64 + r_vals[r1]<<3 + r_vals[r2] end
            },
            {
                { "r", "n" },
                function () end
            },
            {
                { "r", "dhl" },
                function () end
            },
            {
                { "r", "dix" },
                function () end
            },
            {
                { "r", "diy" },
                function () end
            },
            {
                { "dhl", "r" },
                function () end
            },
            {
                { "dix", "r" },
                function () end
            },
            {
                { "diy", "r" },
                function () end
            },
            {
                { "dhl", "n" },
                function () end
            },
            {
                { "dix", "n" },
                function () end
            },
            {
                { "diy", "n" },
                function () end
            },
            {
                { "ra", "dbc" },
                function () end
            },
            {
                { "ra", "dde" },
                function () end
            }
            {
                { "ra", "dnn" },
                function () end
            },
            {
                { "dbc", "ra" },
                function () end
            },
            {
                { "dde", "ra" },
                function () end
            },
            {
                { "dnn", "ra" },
                function () end
            },
            {
                { "ra", "ri" },
                function () end
            },
            {
                { "ra", "rr" },
                function () end
            },
            {
                { "ri", "ra" },
                function () end
            },
            {
                { "rr", "ra" },
                function () end
            },
            -- Grupo de carga de 16 bits
            {
                { "dd", "nn" },
                function () end
            },
            {
                { "rix", "nn" },
                function () end
            },
            {
                { "riy", "nn" },
                function () end
            },
            {
                { "rhl", "dnn" },
                function () end
            },
            {
                { "dd", "dnn" },
                function () end
            },
            {
                { "rix", "dnn" },
                function () end
            },
            {
                { "riy", "dnn" },
                function () end
            },
            {
                { "dnn", "rhl" },
                function () end
            },
            {
                { "dnn", "dd" },
                function () end
            },
            {
                { "dnn", "rix" },
                function () end
            },
            {
                { "dnn", "riy" },
                function () end
            },
            {
                { "rsp", "rhl" },
                function () end
            },
            {
                { "rsp", "riy" },
                function () end
            },
        },
        PUSH: {
            {
                { "qq" },
                function () end
            },
            {
                { "rix" },
                function () end
            },
            {
                { "riy" },
                function () end
            }
        },
        POP:  {
            {
                { "qq" },
                function () end
            },
            {
                { "rix" },
                function () end
            },
            {
                { "riy" },
                function () end
            }
        },
        -- Grupo de búsqueda, intercambio y transferencia
        EX: {
            {
                { 'rde', 'rhl' },
                { 0xeb }
            },
            {
                { 'raf', 'raf' },
                { 0x08 }
            },
            {
                { 'dsp', 'rhl' },
                { 0xe3 }
            },
            {
                { 'dsp', 'rix' },
                { 0xdd, 0xe3 }
            },
            {
                { 'dsp', 'riy' },
                { 0xfd, 0xe3 }
            }
        },
        EXX: { 0xd9 },
        LDI: { 0xed, 0xa0 },
        LDIR: { 0xed, 0xb0 },
        LDD: { 0xed, 0xa8 },
        LDDR: { 0xed, 0xb8 },
        CPI: { 0xed, 0xa1 },
        CPIR: { 0xed, 0xb1 },
        CPD: { 0xed, 0xa9 },
        CPDR: { 0xed, 0xb9 },
        ADD: {
            -- Grupo aritmético de 8 bits
            {
                { "r2" },
                function () end
            },
            {
                { "n" },
                function () end
            },
            {
                { "dix" },
                function () end
            },
            {
                { "diy" },
                function () end
            }
            -- Grupo aritmético de 16 bits
            {
                { "rhl", "ss" },
                function () end
            },
            {
                { "rix", "pp" },
                function () end
            },
            {
                { "riy", "rr" },
                function () end
            }
        },
        ADC: {
            {
                { "r2" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "n" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "dix" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "diy" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "rhl", "ss" },
                function () end
            }
        },
        SUB: {
            {
                { "r2" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "n" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "dix" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "diy" },
                {
                    "ADD",
                    function () end
                }
            }
        },
        SBC: {
            {
                { "r2" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "n" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "dix" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "diy" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "rhl", "ss" },
                function () end
            }
        },
        AND: {
            {
                { "r2" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "n" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "dix" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "diy" },
                {
                    "ADD",
                    function () end
                }
            }
        },
        OR: {
            {
                { "r2" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "n" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "dix" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "diy" },
                {
                    "ADD",
                    function () end
                }
            }
        },
        XOR: {
            {
                { "r2" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "n" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "dix" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "diy" },
                {
                    "ADD",
                    function () end
                }
            }
        },
        CP: {
            {
                { "r2" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "n" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "dix" },
                {
                    "ADD",
                    function () end
                }
            },
            {
                { "diy" },
                {
                    "ADD",
                    function () end
                }
            }
        },
        INC: {
            {
                { "r2" },
                function () end
            },
            {
                { "dix" },
                function () end
            },
            {
                { "diy" },
                function () end
            },
        },
        DEC: {},
        -- Grupo de propòsito general y control de CPU
        DAA: { 0x27 },
        CPL: { 0x2f },
        NEG: { 0xed, 0x44 }
        CCF: { 0x3f },
        SCF: { 0x37},
        NOP: { 0x00 },
        HALT: { 0x76 },
        DI: { 0xf3 },
        EI: { 0xfb },
        IM: {
            { "im" },
            function (im)
                local v = 0
                if im == 1 then v = 2
                else if im == 2 then v = 3 end
            return { 0xed, 70+im} end
        },
        -- Grupo de rotaciòn y desplazamiento
        RLCA: { 0x7 },
        RLA: { 0x17 },
        RRCA: { 0xf },
        RRA: { 0x1f },
        RLC: {
            {
                { "r" },
                function () end
            },
            {
                { "dhl" },
                { 0xcb, 0x06 }
            },
            {
                { "dix" },
                function () end
            },
            {
                { "diy" },
                function () end
            },
        },
        RL: {
            "RLC",
            function () end
        },
        RRC: {
            "RLC",
            function () end
        },
        RR: {
            "RLC",
            function () end
        },
        SLA: {
            "RLC",
            function () end
        },
        SRA: {
            "RLC",
            function () end
        },
        SRL: {
            "RLC",
            function () end
        },
        RLD: {
            "RLC",
            function () end
        },
        RRD: {
            "RLC",
            function () end
        },
        -- Grupo de bits
        BIT: {
            {
                { "b", "r" }
            },
            {
                { "b", "dhl" }
            },
            {
                { "b", "dix" }
            },
            {
                { "b", "diy" }
            }
        },
        SET: {
            "BIT",
            function () end
        },
        RES: { -- Ver si es igual que bit
            {
                { "b", "m" },
                function () end
            }
        },
        -- Grupo de salto
        JP: {
            {
                { "nn" },
                function () end
            },
            {
                { "cc", "nn" }.
                function () end
            },
            {
                { "dhl" },
                { 0xe9 }
            },
            {
                { "dix" },
                { 0xdd, 0xe9 }
            },
            {
                { "diy" },
                { 0xfd, 0xe9 }
            }
        },
        JR: {
            {
                { "e" },
                function () end
            },
            {
                { "fc", "e" },
                function () end
            },
            {
                { "fnc", "e" },
                function () end
            },
            {
                { "fz", "e" },
                function () end
            },
            {
                { "fnz", "e" },
                function () end
            }
        },
        DJNZ: {
            {
                { "e" },
                function () end
            }
        },
        -- Grupo de llamada y retorno
        CALL: {
            {
                { "nn" },
                function () end
            },
            {
                { "cc", "nn" },
                function () end
            }
        },
        RET: {
            {
                {},
                { 0xc9 }
            },
            {
                { "cc" },
                function () end
            }
        },
        RETI: { 0xed, 0x4d },
        RETN: { 0xed, 0x45 },
        RST: {
            {
                { "p" },
                function () end
            }
        },
        -- Grupo de E/S
        IN: {
            {
                { "ra", "dn" },
                function () end
            },
            {
                { "r", "dc" },
                function () end
            }
        },
        INI: { 0xed, 0xa2 },
        INIR: { 0xed, 0xb2 },
        IND: { 0xed, 0xaa },
        INDR: { 0xed, 0xba },
        OUT: {
            {
                { "dn", "g:ra" },
                function () end
            },
            {
                { "dc", "r" },
                function () end
            }
        },
        OUTI: { 0xed, 0xa3 },
        OTIR: { 0xed, 0xb3 },
        OUTD: { 0xed, 0xab },
        OTDR: { 0xed, 0xbb },
    },
    opcodes = {
        
    },
    desensamble = {
        
    }
}
]]
