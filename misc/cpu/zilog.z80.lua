--[[
    Copyright (C) 2025-2026 Giovanni Alfredo Garciliano Diaz

    This file is part of Lily.

    Lily is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, either version 3 of the License, or (at your option) any later
    version.

    Lily is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
    details.

    You should have received a copy of the GNU General Public License along with
    Lily. If not, see <https://www.gnu.org/licenses/>.
]]

-- Especificación del microprocesador Zilog Z80

-- Correspondencias bits->valor
r_vals_r = {
    [0] = "B", [1] = "C", [2] = "D",
    [3] = "E", [4] = "H", [5] = "L",
    [7] = "A"
}
cc_vals_cc = {
    [0] = "NZ", [1] = "Z",
    [2] = "NC", [3] = "C",
    [4] = "PO", [5] = "PE",
    [6] = "P",  [7] = "M"
}
ccjr_vals_ccjr = {
    [7] = "C", [6] = "NC",
    [5] = "Z", [4] = "NZ"
}
qq_vals_qq = {
    [0] = "BC", [1] = "DE",
    [2] = "HL", [3] = "AF",
}
ss_vals_ss = {
    [0] = "BC", [1] = "DE",
    [2] = "HL", [3] = "SP"
}
pp_vals_pp = {
    [0] = "BC",    [1] = "DE",
    [2] = "IX",    [3] = "SP"
}
rr_vals_rr = {
    [0] = "BC",    [1] = "DE",
    [2] = "IY",    [3] = "SP"
}

-- Correspondencias valor->bits
function r_vals(v)
    vals = {
        B = 0, C = 1, D = 2,
        E = 3, H = 4, L = 5,
        A = 7
    }
    return (type(v) == "string" and #v == 1 and vals[v]) or (type(v) == "table" and #v == 1 and v[1] == "HL")
end
i_vals = {
    IX = 0xdd,
    IY = 0xfd
}
cc_vals = {
    NZ = 0, Z = 1,
    NC = 2, C = 3,
    PO = 4, PE = 5,
    P = 6, M = 7
}
ccjr_vals = {
    C = 7, NC = 6,
    Z = 5, NZ = 4,
}
qq_vals = {
    BC = 0,
    DE = 1,
    HL = 2,
    AF = 3
}
ss_vals = {
    BC = 0,
    DE = 1,
    HL = 2,
    SP = 3
}
pp_vals = {
    BC = 0,
    DE = 1,
    IX = 2,
    SP = 3
}
rr_vals = {
    BC = 0,
    DE = 1,
    IY = 2,
    SP = 3
}

-- Funciones de modificación comunes
adc_tr = {
    "ADD",
    function (bytes)
        if (#bytes < 3) then idx = 1
        else idx = 2 end
        bytes[idx] = bytes[idx] + 8
        return bytes
    end
}
sub_tr = {
    "ADD",
    function (bytes)
        if (#bytes < 3) then idx = 1
        else idx = 2 end
        bytes[idx] = bytes[idx] + 0x10
        return bytes
    end
}
sbc_tr = {
    "ADD",
    function (bytes)
        if (#bytes < 3) then idx = 1
        else idx = 2 end
        bytes[idx] = bytes[idx] + 0x18
        return bytes
    end
}
and_tr = {
    "ADD",
    function (bytes)
        if (#bytes < 3) then idx = 1
        else idx = 2 end
        bytes[idx] = bytes[idx] + 0x20
        return bytes
    end
}
or_tr = {
    "ADD",
    function (bytes)
        if (#bytes < 3) then idx = 1
        else idx = 2 end
        bytes[idx] = bytes[idx] + 0x30
        return bytes
    end
}
xor_tr = {
    "ADD",
    function (bytes)
        if (#bytes < 3) then idx = 1
        else idx = 2 end
        bytes[idx] = bytes[idx] + 0x28
        return bytes
    end
}
cp_tr = {
    "ADD",
    function (bytes)
        if (#bytes < 3) then idx = 1
        else idx = 2 end
        bytes[idx] = bytes[idx] + 0x38
        return bytes
    end
}
dec_tr = {
    "INC",
    function (bytes)
        if (#bytes == 3) then idx = 2
        else idx = 1 end
        bytes[idx] = bytes[idx] + 1
        return bytes
    end
}

-- Objeto a devolver
return {
    id = "zilog.z80",
    max_memoria = 65536,
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
        X = {
            tamano = 48,
            desplazamiento = 16
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
            tamano = 48,
            desplazamiento = 148
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
        -- Registros de uso interno
        IM = {
            tamano = 8,
            desplazamiento = 196
        },
        IFF1 = {
            tamano = 1,
            desplazamiento = 204
        },
        IFF2 = {
            tamano = 1,
            desplazamiento = 205
        }
    },
    registro_programa = "PC",
    tipos = {
        r = function (v) return r_vals(v) end,
        di = function (v) return type(v) == "table" and (#v == 1 or #v == 2) and i_vals[v[1]] and ((not v[2]) or (type(v[2]) == "number" and v[2] >= -128 and v[2] < 128)) end,
        n = function (v) return type(v) == "number" and v >= 0 and v < 256 end,
        nn = function (v) return type(v) == "number" and v >= 0 and v < 65565 end,
        d = function (v) return type(v) == "number" and v >= -128 and v < 128 end,
        b = function (v) return type(v) == "number" and v >= 0 and v < 8 end,
        e = function (v) return type(v) == "number" and v >= -126 and v < 130 end,
        -- FIX: implementar table:en
        cc = function (v) return cc_vals[v] end,
        qq = function (v) return qq_vals[v] end,
        ss = function (v) return ss_vals[v] end,
        pp = function (v) return pp_vals[v] end,
        rr = function (v) return rr_vals[v] end,
        im = function (v) return type(v) == "number" and v >= 0 and v < 3 end,
        dbc = function (v) return type(v) == "table" and #v == 1 and v[1] == "BC" end,
        dde = function (v) return type(v) == "table" and #v == 1 and v[1] == "DE" end,
        dnn = function (v) return type(v) == "table" and #v == 1 and type(v[1]) == "number" and v[1] >= 0 and v[1] <= 65565 end,
        ra = function (v) return v == "A" end,
        rir = function (v) return v == "I" end,
        rr = function (v) return v == "R" end,
        ri = function (v) return v == "IX" or v == "IY" end,
        rhl = function (v) return v == "HL" end,
        rsp = function (v) return v == "SP" end,
        rde = function (v) return v == "DE" end,
        raf = function (v) return v == "AF" end,
        dsp = function (v) return type(v) == "table" and #v == 1 and v[1] == "SP" end,
        dn = function (v) return type(v) == "table" and #v == 1 and type(v[1]) == "number" and v[1] >= 0 and v[1] < 256 end,
        dc = function (v) return type(v) == "table" and #v == 1 and v[1] == "C" end,
        --dhl = function (v) return type(v) == "table" and #v == 1 and v[1] == "SP" end, -- solo para JP
        ccjr = function (v) return ccjr_vals[v] end,
        rix = function (v) return v == "IX" end, -- solo para ADD
        riy = function (v) return v == "IY" end -- solo para ADD
    },
    ensamble = {
        LD = {
            -- Grupo de carga de 8 bits
            {
                { "r", "r" },
                function (r1, r2)
                    return { 64 + (r_vals(r1)<<3) + r_vals(r2) } end
            },
            {
                { "r", "n" },
                function (r, n) return { 6 + (r_vals(r)<<3), n } end
            },
            {
                { "r", "di" },
                function (r, di)
                    return {
                        i_vals[di[1]],
                        0x46 + (r_vals(r)<<3),
                        di[2] or 0
                    }
                end
            },
            {
                { "di", "r" },
                function (di, r)
                    return {
                        i_vals[di[1]],
                        0x70 + r_vals(r),
                        di[2] or 0
                    }
                end
            },
            {
                { "di", "n" },
                function (di, n)
                    return {
                        i_vals[di[1]], 0x36,
                        di[2] or 0, n
                    }
                end
            },
            {
                { "ra", "dbc" },
                { 0x0a }
            },
            {
                { "ra", "dde" },
                { 0x1a }
            },
            {
                { "ra", "dnn" },
                function (ra, dnn) return { 0x3a, uint16l(dnn[1]) } end
            },
            {
                { "dbc", "ra" },
                { 0x02 }
            },
            {
                { "dde", "ra" },
                { 0x12 }
            },
            {
                { "dnn", "ra" },
                function (dnn, ra) return { 0x32, uint16l(dnn[1]) } end
            },
            {
                { "ra", "rir" },
                { 0xed, 0x57 }
            },
            {
                { "ra", "rr" },
                { 0xed, 0x5f }
            },
            {
                { "rir", "ra" },
                { 0xed, 0x47 }
            },
            {
                { "rr", "ra" },
                { 0xed, 0x4f }
            },
            -- Grupo de carga de 16 bits
            {
                { "ss", "nn" },
                function (ss, nn) return { 1 + (ss_vals[ss]<<4), uint16l(nn) } end
            },
            {
                { "ri", "nn" },
                function (ri, nn) return { i_vals[ri], 0x21, uint16l(nn) } end
            },
            {
                { "rhl", "dnn" },
                function (rhl, dnn) return { 0x2a, uint16l(dnn[1]) } end
            },
            {
                { "ss", "dnn" },
                function (ss, dnn) return { 0xed, 0x4a + (ss_vals[ss]<<4), uint16l(dnn[1]) } end
            },
            {
                { "ri", "dnn" },
                function (ri, dnn) return { i_vals[ri], 0x2a, uint16l(dnn[1]) } end
            },
            {
                { "dnn", "rhl" },
                function (dnn, rhl) return { 0x22, uint16l(dnn[1]) } end
            },
            {
                { "dnn", "ss" },
                function (dnn, ss) return { 0xed, 0x67, uint16l(dnn[1]) } end
            },
            {
                { "dnn", "ri" },
                function (dnn, ri) return { i_vals[ri], 0x22, uint16l(dnn[1]) }  end
            },
            {
                { "rsp", "rhl" },
                { 0xf9 }
            },
            {
                { "rsp", "ri" },
                function (rsp, ri) return { i_vals[ri], 0xf9 } end
            },
        },
        PUSH = {
            {
                { "qq" },
                function (qq) return { 0xc5 + qq_vals[qq] } end
            },
            {
                { "ri" },
                function (ri) return { i_vals[ri], 0xe5 } end
            },
        },
        POP =  {
            {
                { "qq" },
                function (qq) return { 0xc1 + qq_vals[qq] } end
            },
            {
                { "ri" },
                function (ri) return { i_vals[ri], 0xe1 } end
            },
        },
        -- Grupo de búsqueda, intercambio y transferencia
        EX = {
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
                { 'dsp', 'ri' },
                { i_vals[ri], 0xe3 }
            },
        },
        EXX = { 0xd9 },
        LDI = { 0xed, 0xa0 },
        LDIR = { 0xed, 0xb0 },
        LDD = { 0xed, 0xa8 },
        LDDR = { 0xed, 0xb8 },
        CPI = { 0xed, 0xa1 },
        CPIR = { 0xed, 0xb1 },
        CPD = { 0xed, 0xa9 },
        CPDR = { 0xed, 0xb9 },
        ADD = {
            -- Grupo aritmético de 8 bits
            {
                { "r" },
                function (r) return { 0x80 + r_vals(r) } end
            },
            {
                { "n" },
                function (n) return { 0xc6, n } end
            },
            {
                { "di" },
                function (di) return { i_vals[di[1]], 0x86, di[2] or 0 } end
            },
            -- Grupo aritmético de 16 bits
            {
                { "rhl", "ss" },
                function (rhl, ss) return { 9 + (ss_vals[ss]<<4) } end
            },
            {
                { "rix", "pp" },
                function (rix, pp) return { 0xdd, 9 + (pp_vals[pp]<<4) } end
            },
            {
                { "riy", "rr" },
                function (riy, rr) return { 0xfd, 9 + (rr_vals[rr]<<4) } end
            }
        },
        ADC = {
            -- Grupo aritmético de 8 bits
            {
                { "r" },
                adc_tr
            },
            {
                { "n" },
                adc_tr
            },
            {
                { "di" },
                adc_tr
            },
            -- Grupo aritmético de 16 bits
            {
                { "rhl", "ss" },
                function (rhl, ss) return { 0xed, 0x4a + (ss_vals[ss]<<4) } end
            }
        },
        SUB = {
            {
                { "r" },
                sub_tr
            },
            {
                { "n" },
                sub_tr
            },
            {
                { "di" },
                sub_tr
            }
        },
        SBC = {
            -- Grupo aritmético de 8 bits
            {
                { "r" },
                sbc_tr
            },
            {
                { "n" },
                sbc_tr
            },
            {
                { "di" },
                sbc_tr
            },
            -- Grupo aritmético de 16 bits
            {
                { "rhl", "ss" },
                function (rhl, ss) return { 0xed, 0x42 + (ss_vals[ss]<<4) } end
            }
        },
        AND = {
            {
                { "r" },
                and_tr
            },
            {
                { "n" },
                and_tr
            },
            {
                { "di" },
                and_tr
            }
        },
        OR = {
            {
                { "r" },
                or_tr
            },
            {
                { "n" },
                or_tr
            },
            {
                { "di" },
                or_tr
            }
        },
        XOR = {
            {
                { "r" },
                xor_tr
            },
            {
                { "n" },
                xor_tr
            },
            {
                { "di" },
                xor_tr
            }
        },
        CP = {
            {
                { "r" },
                cp_tr
            },
            {
                { "n" },
                cp_tr
            },
            {
                { "di" },
                cp_tr
            }
        },
        INC = {
            -- Grupo aritmético de 8 bits
            {
                { "r" },
                function (r) return { 4 + (r_vals(r)<<3) } end
            },
            {
                { "di" },
                function (di) return { i_vals[di[1]], 0x34, di[2] or 0 } end
            },
            -- Grupo aritmético de 16 bits
            {
                { "ss" },
                function (ss) return { 3 + (ss_vals[ss]<<4) } end
            },
            {
                { "ri" },
                function (ri) return { i_vals[ri], 0x23 } end
            }
        },
        DEC = {
            -- Grupo aritmético de 8 bits
            {
                { "r" },
                dec_tr
            },
            {
                { "di" },
                dec_tr
            },
            -- Grupo aritmético de 16 bits
            {
                { "ss" },
                function (ss) return { 0xb + (ss_vals[ss]<<4) } end
            },
            {
                { "ri" },
                function (ri) return { i_vals[ri], 0x2b } end
            }
        },
        -- Grupo de propósito general y control de CPU
        DAA = { 0x27 },
        CPL = { 0x2f },
        NEG = { 0xed, 0x44 },
        CCF = { 0x3f },
        SCF = { 0x37 },
        NOP = { 0x00 },
        HALT = { 0x76 },
        DI = { 0xf3 },
        EI = { 0xfb },
        IM = {
            { "im" },
            function (im)
                local v = 0
                if im == 1 then v = 2
                elseif im == 2 then v = 3 end
            return { 0xed, 70+v } end
        },
        -- Grupo de rotación y desplazamiento
        RLCA = { 0x7 },
        RLA = { 0x17 },
        RRCA = { 0xf },
        RRA = { 0x1f },
        RLC = {
            {
                { "r" },
                function (r) return { 0xcb, r_vals(r) } end
            },
            {
                { "di" },
                function (di) return { i_vals[di[1]], 0xcb, di[2] or 0, 0x6} end
            }
        },
        RL = {
            "RLC",
            function (bytes)
                bytes[2] = bytes[2] + 0x10
                return bytes
            end
        },
        RRC = {
            "RLC",
            function (bytes)
                bytes[2] = bytes[2] + 8
                return bytes
            end
        },
        RR = {
            "RLC",
            function (bytes)
                bytes[2] = bytes[2] + 0x18
                return bytes
            end
        },
        SLA = {
            "RLC",
            function (bytes)
                bytes[2] = bytes[2] + 0x20
                return bytes
            end
        },
        SRA = {
            "RLC",
            function (bytes)
                bytes[2] = bytes[2] + 0x28
                return bytes
            end
        },
        SRL = {
            "RLC",
            function (bytes)
                bytes[2] = bytes[2] + 0x38
                return bytes
            end
        },
        RLD = { 0xed, 0x6f },
        RRD = { 0xed, 0x67 },
        -- Grupo de bits
        BIT = {
            {
                { "b", "r" },
                function (b, r) return { 0xcb, 0x40 + (b<<3) + r_vals[r] } end,
            },
            {
                { "b", "di" },
                function (b, di) return { i_vals[di[1]], 0xcb, di[2] or 0, 0x46 + (b<<3) } end
            }
        },
        SET = {
            "BIT",
            function (bytes)
                bytes[#bytes] = bytes[#bytes] + 0xc0
                return bytes
            end
        },
        RES = {
            "BIT",
            function (bytes)
                bytes[#bytes] = bytes[#bytes] + 0x80
                return bytes
            end
        },
        -- Grupo de salto
        JP = {
            {
                { "nn" },
                function (nn) return { 0xc3, uint16l(nn) } end
            },
            {
                { "cc", "nn" },
                function (cc, nn) return { 0xc2 + (cc_vals[cc]<<3), uint16l(nn) } end
            },
            {
                { "rhl" },
                { 0xe9 }
            },
            {
                { "ri" },
                function (ri) return { i_vals[ri], 0xe9 } end
            },
        },
        JR = {
            {
                { "e" },
                function (e) return { 0x18, e-2 } end
            },
            {
                { "ccjr", "e" },
                function (ccjr, e) return { ccjr_vals[ccjr]<<3, e-2 } end
            },
        },
        DJNZ = {
            {
                { "e" },
                function (e) return { 0x10, e-2 } end
            }
        },
        -- Grupo de llamada y retorno
        CALL = {
            {
                { "nn" },
                function (nn) return { 0xcd, uint16l(nn) } end
            },
            {
                { "cc", "nn" },
                function (cc, nn) return { 0xc4 + (cc_vals[cc]<<3), uint16l(nn) } end
            }
        },
        RET = {
            {
                {},
                { 0xc9 }
            },
            {
                { "cc" },
                function (cc) return { 0xc0 + (cc_vals[cc]<<3) } end
            }
        },
        RETI = { 0xed, 0x4d },
        RETN = { 0xed, 0x45 },
        RST = {
            {
                { "uint3" },
                function (pt) return { 0xc7 + pt } end
            }
        },
        -- Grupo de E/S
        IN = {
            {
                { "ra", "dn" },
                function (ra, dn) return { 0xdb, dn[1] } end
            },
            {
                { "r", "dc" },
                function (r, dc) return { 0xed, 0x40 + r_vals[r] } end
            }
        },
        INI = { 0xed, 0xa2 },
        INIR = { 0xed, 0xb2 },
        IND = { 0xed, 0xaa },
        INDR = { 0xed, 0xba },
        OUT = {
            {
                { "dn", "ra" },
                function (dn, ra) return { 0xd3, dn[1] } end
            },
            {
                { "dc", "r" },
                function (dc, r) return { 0xed, 0x41 + r_vals[r] } end
            }
        },
        OUTI = { 0xed, 0xa3 },
        OTIR = { 0xed, 0xb3 },
        OUTD = { 0xed, 0xab },
        OTDR = { 0xed, 0xbb },
    },
    opcodes = {
        {
            { 0 }, -- NOP
            function (bytes) end
        },
        {
            { 2 }, -- LD [%BC], %A
            function (bytes)
                L.escribir_memoria(L.leer_registro("BC"), L.leer_registro("A"))
            end
        },
        {
            { 7 }, -- RLCA
            function (bytes)
                local a = L.leer_registro("A")
                local b7 = (a & 0x80) >> 7
                L.escribir_registro("HF", 0)
                L.escribir_registro("NF", 0)
                L.escribir_registro("CF", b7)
                L.escribir_registro("A", ((a << 1) & 0xff) + b7)
            end
        },
        {
            { 8 }, -- EX %AF, %AF
            function (bytes)
                local af = L.leer_registro("AF")
                local afx = L.leer_registro("AFX")
                L.escribir_registro("AF", afx)
                L.escribir_registro("AFX", af)
            end
        },
        {
            { 0xf }, -- RRCA
            function (bytes)
                local a = L.leer_registro("A")
                local b0 = (a & 1)
                L.escribir_registro("HF", 0)
                L.escribir_registro("NF", 0)
                L.escribir_registro("CF", b0)
                L.escribir_registro("A", (a >> 1) + (b0 << 7))
            end
        },
        {
            { 0x17 }, -- RLA
            function (bytes)
                local a = L.leer_registro("A")
                local cf = L.leer_registro("CF")
                L.escribir_registro("HF", 0)
                L.escribir_registro("NF", 0)
                L.escribir_registro("CF", (a & 0x80) >> 7)
                L.escribir_registro("A", ((a << 1) & 0xff) + cf)
            end
        },
        {
            { 0x18, true }, -- JR e
            function (bytes)
                local pc = L.leer_registro("PC")
                L.escribir_registro("PC", pc + bytes[2])
            end
        },
        {
            { 0x1f }, -- RRA
            function (bytes)
                local a = L.leer_registro("A")
                local cf = L.leer_registro("CF")
                L.escribir_registro("HF", 0)
                L.escribir_registro("NF", 0)
                L.escribir_registro("CF", a & 1)
                L.escribir_registro("A", (a >> 1) + (cf << 7))
            end
        },
        {
            { 0x34 }, -- INC [%HL]
            function (bytes)
                local d = L.leer_registro("HL")
                local v = L.leer_memoria(d) + 1
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                -- FIX: H
                if (v == 0x7f) then
                    L.escribir_registro("VF", 1)
                else
                    L.escribir_registro("VF", 0)
                end
                L.escribir_registro("NF", 0)
                L.escribir_memoria(d, v)
            end
        },
        {
            { 0x37 }, -- SCF
            function (bytes)
                L.escribir_registro("CF", 1)
                L.escribir_registro("HF", 0)
                L.escribir_registro("NF", 0)
            end
        },
        {
            { 0x3f }, -- CCF
            function (bytes)
                local cf = L.leer_registro("CF")
                if cf == 1 then
                    L.escribir_registro("CF", 0)
                else
                    L.escribir_registro("CF", 1)
                end
                -- FIX: H
                L.escribir_registro("NF", 0)
            end
        },
        {
            { 0xa }, -- LD %A, [%BC]
            function (bytes)
                L.escribir_registro("A", L.leer_memoria(L.leer_registro("BC")))
            end
        },
        {
            { 0x10, true }, -- DJNZ e
            function (bytes)
                local b = L.leer_registro("B") - 1
                L.escribir_registro("B", b)
                if (b ~= 0) then
                    L.escribir_registro("PC", L.leer_registro("PC") + e)
                end
            end
        },
        {
            { 0x12 }, -- LD [%DE], %A
            function (bytes)
                L.escribir_memoria(L.leer_registro("DE"), L.leer_registro("A"))
            end
        },
        {
            { 0x1a }, -- LD %A, [%DE]
            function (bytes)
                L.escribir_registro("A", L.leer_memoria(L.leer_registro("DE")))
            end
        },
        {
            { 0x22, true, true }, -- LD [nn], %HL
            function (bytes)
                local nn = bytes[2] + (bytes[3] << 8)
                L.escribir_memoria(nn, L.leer_registro("L"))
                L.escribir_memoria(nn + 1, L.leer_registro("H"))
            end
        },
        {
            { 0x2a, true, true }, -- LD %HL, [nn]
            function (bytes)
                local d = bytes[2] + (bytes[3] << 8)
                L.escribir_registro("H", L.leer_memoria(d + 1))
                L.escribir_registro("L", L.leer_memoria(d))
            end
        },
        {
            { 0x2f }, -- CPL
            function (bytes)
                L.escribir_registro("A", ~L.leer_registro("A"))
            end
        },
        {
            { 0x32, true, true }, -- LD [nn], %A
            function (bytes)
                local dnn = bytes[2] + (bytes[3]<<8)
                L.escribir_memoria(dnn, L.leer_registro("A"))
            end
        },
        {
            { 0x35 }, -- DEC [%HL]
            function (bytes)
                local d = L.leer_registro("HL")
                local v = L.leer_memoria(d) - 1
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                -- FIX: H
                if (v == 0x80) then
                    L.escribir_registro("VF", 1)
                else
                    L.escribir_registro("VF", 0)
                end
                L.escribir_registro("NF", 1)
                L.escribir_memoria(d, v)
            end
        },
        {
            { 0x36, true }, -- LD [%HL], n
            function (bytes)
                L.escribir_memoria(L.leer_registro("HL"), bytes[2])
            end
        },
        {
            { 0x3a, true, true }, -- LD %A, [nn]
            function (bytes)
                local dnn = bytes[2] + (bytes[3]<<8)
                L.escribir_registro("A", L.leer_memoria(dnn))
            end
        },
        {
            { 0x76 }, -- HALT
            function (bytes)
                L.detener()
            end
        },
        {
            { 0x86 }, -- ADD %A, [%HL]
            function (bytes)
                local p2 = L.leer_memoria(L.leer_registro("HL"))
                local p1 = L.leer_registro("A")
                local v = p1 + p2
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                -- FIX: H
                -- FIX: V
                L.escribir_registro("NF", 0)
                -- FIX: C
                L.escribir_registro("A", v)
            end
        },
        {
            { 0x8e }, -- ADC %A, [%HL]
            function (bytes)
                local p2 = L.leer_memoria(L.leer_registro("HL"))
                local p1 = L.leer_registro("A")
                local cf = 0
                if (L.leer_registro("CF") == 1) then
                    cf = 1
                end
                local v = p1 + p2 + cf
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                -- FIX: H
                -- FIX: V
                L.escribir_registro("NF", 0)
                -- FIX: C
                L.escribir_registro("A", v)
            end
        },
        {
            { 0x96 }, -- SUB %A, [%HL]
            function (bytes)
                local p2 = L.leer_memoria(L.leer_registro("HL"))
                local p1 = L.leer_registro("A")
                local v = p1 - p2
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                -- FIX: H
                -- FIX: V
                L.escribir_registro("NF", 0)
                -- FIX: C
                L.escribir_registro("A", v)
            end
        },
        {
            { 0x9e }, -- SBC %A, [%HL]
            function (bytes)
                local p2 = L.leer_memoria(L.leer_registro("HL"))
                local p1 = L.leer_registro("A")
                local cf = 0
                if (L.leer_registro("CF") == 1) then
                    cf = 1
                end
                local v = p1 - p2 - cf
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                -- FIX: H
                -- FIX: V
                L.escribir_registro("NF", 0)
                -- FIX: C
                L.escribir_registro("A", v)
            end
        },
        {
            { 0xa6 }, -- AND %A, [%HL]
            function (bytes)
                local p2 = L.leer_memoria(L.leer_registro("HL"))
                local p1 = L.leer_registro("A")
                local v = p1 & p2
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                L.escribir_registro("HF", 1)
                -- FIX: V
                L.escribir_registro("NF", 0)
                L.escribir_registro("CF", 0)
                L.escribir_registro("A", v)
            end
        },
        {
            { 0xae }, -- XOR %A, [%HL]
            function (bytes)
                local p2 = L.leer_memoria(L.leer_registro("HL"))
                local p1 = L.leer_registro("A")
                local v = p1 ~ p2
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                L.escribir_registro("HF", 0)
                if (v % 2 == 0) then
                    L.escribir_registro("VF", 1)
                else
                    L.escribir_registro("VF", 0)
                end
                L.escribir_registro("NF", 0)
                L.escribir_registro("CF", 0)
                L.escribir_registro("A", v)
            end
        },
        {
            { 0xb6 }, -- OR %A, [%HL]
            function (bytes)
                local p2 = L.leer_memoria(L.leer_registro("HL"))
                local p1 = L.leer_registro("A")
                local v = p1 | p2
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                L.escribir_registro("HF", 0)
                -- FIX: V
                L.escribir_registro("NF", 0)
                L.escribir_registro("CF", 0)
                L.escribir_registro("A", v)
            end
        },
        {
            { 0xbe }, -- CP %A, [%HL]
            function (bytes)
                local p2 = L.leer_memoria(L.leer_registro("HL"))
                local p1 = L.leer_registro("A")
                local v = p1 - p2
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                -- FIX: H
                -- FIX: V
                L.escribir_registro("NF", 1)
                -- FIX: C
            end
        },
        {
            { 0xc3, true, true }, -- JP nn
            function (bytes)
                L.escribir_registro("PC", bytes[2] + (bytes[3] << 8))
            end
        },
        {
            { 0xc6, true }, -- ADD %A, n
            function (bytes)
                local v = L.leer_registro("A") + bytes[2]
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                -- FIX: H
                -- FIX: V
                L.escribir_registro("NF", 0)
                -- FIX: C
                L.escribir_registro("A", v)
            end
        },
        {
            { 0xc9 }, -- RET
            function (bytes)
                local sp = L.leer_registro("SP")
                local pc = L.leer_memoria(sp) + (L.leer_memoria(sp + 1) << 8)
                L.escribir_registro("SP", sp + 2)
                L.escribir_registro("PC", pc)
            end
        },
        {
            { 0xcb },
            {
                {
                    { 0x06 }, -- RLC [%HL]
                    function (bytes)
                        local d = L.leer_registro("HL")
                        local hlv = L.leer_memoria(d)
                        local b7 = (hlv & 0x80) >> 7
                        local v = ((hlv << 1) & 0xff) + b7
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF", 0)
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", b7)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0x16 }, -- RL [%HL]
                    function (bytes)
                        local d = L.leer_registro("HL")
                        local hlv = L.leer_memoria(d)
                        local v = ((hlv << 1) & 0xff) + L.leer_registro("CF")
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", (hlv & 0x80) >> 7)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0xe }, -- RRC [%HL]
                    function (bytes)
                        local d = L.leer_registro("HL")
                        local hlv = L.leer_memoria(d)
                        local b0 = hlv & 1
                        local v = (hlv >> 1) + (b0 << 7)
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", b1)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0x1e }, -- RR [%HL]
                    function (bytes)
                        local d = L.leer_registro("HL")
                        local hlv = L.leer_memoria(d)
                        local v = (hlv >> 1) + L.leer_registro("CF")
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", hlv & 1)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0x26 }, -- SLA [%HL]
                    function (bytes)
                        local d = L.leer_registro("HL")
                        local hlv = L.leer_registro(d)
                        local v = (hlv << 1) & 0xff
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", (hlv & 0x80) >> 7)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0x2e }, -- SRA [%HL]
                    function (bytes)
                        local d = L.leer_registro("HL")
                        local hlv = L.leer_registro(d)
                        local v = (hlv >> 1) + (hlv & 0x80)
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", hlv & 1)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0x3e }, -- SRL [%HL]
                    function (bytes)
                        local d = L.leer_registro("HL")
                        local hlv = L.leer_memoria(d)
                        local v = rv >> 1
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", hlv & 1)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { function (byte) return byte & 0xf8 == 0 end }, -- RLC r
                    function (bytes)
                        local r = rr_vals_rr[bytes[2] & 7]
                        local rv = L.leer_registro(r)
                        local b7 = (rv & 0x80) >> 7
                        local v = ((rv << 1) & 0xff) + b7
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", b7)
                        L.escribir_registro(r, v)
                    end
                },
                {
                    { function (byte) return byte & 0xf8 == 0x10 end }, -- RL r
                    function (bytes)
                        local r = rr_vals_rr[bytes[2] & 7]
                        local rv = L.leer_registro(r)
                        local v = ((rv << 1) & 0xff) + L.leer_registro("CF")
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", (rv & 0x80) >> 7)
                        L.escribir_registro(r, v)
                    end
                },
                {
                    { function (byte) return byte & 0xf8 == 8 end }, -- RRC r
                    function (bytes)
                        local r = rr_vals_rr[bytes[2] & 7]
                        local rv = L.leer_registro(r)
                        local b0 = rv & 1
                        local v = (rv >> 1) + (b0 << 7)
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", b1)
                        L.escribir_registro(r, v)
                    end
                },
                {
                    { function (byte) return byte & 0xf8 == 0x18 end }, -- RR r
                    function (bytes)
                        local r = rr_vals_rr[bytes[2] & 7]
                        local rv = L.leer_registro(r)
                        local v = (rv >> 1) + L.leer_registro("CF")
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", rv & 1)
                        L.escribir_registro(r, v)
                    end
                },
                {
                    { function (byte) return byte & 0xc7 == 0x20 end }, -- SLA r
                    function (bytes)
                        local r = rr_vals_rr[bytes[2] & 7]
                        local rv = L.leer_registro(r)
                        local v = (rv << 1) & 0xff
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", (rv & 0x80) >> 7)
                        L.escribir_registro(r, v)
                    end
                },
                {
                    { function (byte) return byte & 0xc7 == 0x38 end }, -- SRA r
                    function (bytes)
                        local r = rr_vals_rr[bytes[2] & 7]
                        local rv = L.leer_registro(r)
                        local v = (rv >> 1) + (rv & 0x80)
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", rv & 1)
                        L.escribir_registro(r, v)
                    end
                },
                {
                    { function (byte) return byte & 0xc7 == 0x38 end }, -- SRL r
                    function (bytes)
                        local r = rr_vals_rr[bytes[2] & 7]
                        local rv = L.leer_registro(r)
                        local v = rv >> 1
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", rv & 1)
                        L.escribir_registro(r, v)
                    end
                },
                {
                    { function (byte) return byte & 0xc7 == 0x86 end }, -- RES b, [%HL]
                    function (bytes)
                        local b = ~(1 << ((bytes[2] & 0x38) >> 3))
                        local d = L.leer_registro("HL")
                        L.escribir_memoria(d, L.leer_memoria(d) & b)
                    end
                },
                {
                    { function (byte) return byte & 0xc0 == 0x80 end }, -- RES b, r
                    function (bytes)
                        local b = ~(1 << ((bytes[2] & 0x38) >> 3))
                        local r = r_vals_r[bytes[2] & 0x7]
                        L.escribir_registro(r, L.leer_registro(r) & b)
                    end
                },
                {
                    { function (byte) return byte & 0xc7 == 0xc6 end }, -- SET b, [%HL]
                    function (bytes)
                        local b = 1 << ((bytes[2] & 0x38) >> 3)
                        local d = L.leer_registro("HL")
                        L.escribir_memoria(d, L.leer_memoria(d) | b)
                    end
                },
                {
                    { function (byte) return byte & 0xc0 == 0xc0 end }, -- SET b, r
                    function (bytes)
                        local b = 1 << ((bytes[2] & 0x38) >> 3)
                        local r = r_vals_r[bytes[2] & 0x7]
                        L.escribir_registro(r, L.leer_registro(r) | b)
                    end
                },
                {
                    { function (byte) return byte & 0xc7 == 0x46 end }, -- BIT b, [%HL]
                    function (bytes)
                        local b = 1 << ((bytes[2] & 0x38) >> 3)
                        local v = L.leer_memoria(L.leer_registro("HL")) & b
                        -- FIX: S unknown
                        if (v == 0) then
                            L.escribir_registro("ZF", 0)
                        else
                            L.escribir_registro("ZF", 1)
                        end
                        L.escribir_registro("HF", 1)
                        -- FIX: V unknown
                        L.escribir_registro("NF", 0)
                    end
                },
                {
                    { function (byte) return byte & 0xc0 == 0x40 end }, -- BIT b, r
                    function (bytes)
                        local b = 1 << ((bytes[2] & 0x38) >> 3)
                        local r = r_vals_r[bytes[2] & 0x7]
                        local v = L.leer_registro(r) & b
                        -- FIX: S unknown
                        if (v == 0) then
                            L.escribir_registro("ZF", 0)
                        else
                            L.escribir_registro("ZF", 1)
                        end
                        L.escribir_registro("HF", 1)
                        -- FIX: V unknown
                        L.escribir_registro("NF", 0)
                    end
                },
            }
        },
        {
            { 0xcd, true, true }, -- CALL nn
            function (bytes)
                local pc = L.leer_registro("PC") + 3
                local sp = L.leer_registro("SP")
                L.escribir_memoria(sp - 1, (pc & 0xff00) >> 8)
                L.escribir_memoria(sp - 2, pc & 0xff)
                L.escribir_registro("SP", sp - 2)
                L.escribir_registro("PC", bytes[2] + (bytes[3] << 8))
            end
        },
        {
            { 0xce, true }, -- ADC %A, n
            function (bytes)
                local cf = 0
                if (L.leer_registro("CF") == 1) then
                    cf = 1
                end
                local v = L.leer_registro("A") + bytes[2] + cf
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                -- FIX: H
                -- FIX: V
                L.escribir_registro("NF", 0)
                -- FIX: C
                L.escribir_registro("A", v)
            end
        },
        {
            { 0xd6, true }, -- SUB %A, n
            function (bytes)
                local v = L.leer_registro("A") - bytes[2]
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                -- FIX: H
                -- FIX: V
                L.escribir_registro("NF", 0)
                -- FIX: C
                L.escribir_registro("A", v)
            end
        },
        {
            { 0xd9 }, -- EXX
            function (bytes)
                local x = L.leer_registro("X")
                local xx = L.leer_registro("XX")
                L.escribir_registro("X", xx)
                L.escribir_registro("XX", x)
            end
        },
        {
            { 0xdd },
            {
                {
                    { 0x21, true, true }, -- LD %IX, nn
                    function (bytes)
                        L.escribir_registro("IX", bytes[3] + (bytes[4] << 8))
                    end
                },
                {
                    { 0x22, true, true }, -- LD [nn], %IX
                    function (bytes)
                        local nn = bytes[3] + (bytes[4] << 8)
                        local ix = L.leer_registro("IX")
                        L.escribir_memoria(nn, ix & 0xff)
                        L.escribir_memoria(nn + 1, (ix & 0xff00) >> 8)
                    end
                },
                {
                    { 0x23 }, -- INC %IX
                    function (bytes)
                        L.escribir_registro("IX", L.leer_registro("IX") + 1)
                    end
                },
                {
                    { 0x2a, true, true }, -- LD %IX, [nn]
                    function (bytes)
                        local nn = bytes[3] + (bytes[4] << 8)
                        L.escribir_registro("IX", L.leer_memoria(nn) + (L.leer_memoria(nn + 1) << 8))
                    end
                },
                {
                    { 0x2b }, -- DEC %IX
                    function (bytes)
                        L.escribir_registro("IX", L.leer_registro("IX") - 1)
                    end
                },
                {
                    { 0x34, true }, -- INC [%IX, d]
                    function (bytes)
                        local d = L.leer_registro("IX") + bytes[3]
                        local v = L.leer_memoria(d) + 1
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        -- FIX: H
                        if (v == 0x7f) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0x35, true }, -- DEC [%IX, d]
                    function (bytes)
                        local d = L.leer_registro("IX") + bytes[3]
                        local v = L.leer_memoria(d) - 1
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        -- FIX: H
                        if (v == 0x80) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 1)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0x36, true, true }, -- LD [%IX, d], n
                    function (bytes)
                        local dix = L.leer_registro("IX") + bytes[3]
                        L.escribir_memoria(dix, bytes[4])
                    end
                },
                {
                    { 0x86, true }, -- ADD %A, [%IX, d]
                    function (bytes)
                        local p2 = L.leer_memoria(L.leer_registro("IX") + bytes[3])
                        local p1 = L.leer_registro("A")
                        local v = p1 + p2
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        -- FIX: H
                        -- FIX: V
                        L.escribir_registro("NF", 0)
                        -- FIX: C
                        L.escribir_registro("A", v)
                    end
                },
                {
                    { 0x8e, true }, -- ADC %A, [%IX, d]
                    function (bytes)
                        local p2 = L.leer_memoria(L.leer_registro("IX") + bytes[3])
                        local p1 = L.leer_registro("A")
                        local cf = 0
                        if (L.leer_registro("CF") == 1) then
                            cf = 1
                        end
                        local v = p1 + p2 + cf
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        -- FIX: H
                        -- FIX: V
                        L.escribir_registro("NF", 0)
                        -- FIX: C
                        L.escribir_registro("A", v)
                    end
                },
                {
                    { 0x96, true }, -- SUB %A, [%IX, d]
                    function (bytes)
                        local p2 = L.leer_memoria(L.leer_registro("IX") + bytes[3])
                        local p1 = L.leer_registro("A")
                        local v = p1 - p2
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        -- FIX: H
                        -- FIX: V
                        L.escribir_registro("NF", 0)
                        -- FIX: C
                        L.escribir_registro("A", v)
                    end
                },
                {
                    { 0x9e, true }, -- SBC %A, [%IX, d]
                    function (bytes)
                        local p2 = L.leer_memoria(L.leer_registro("IX") + bytes[3])
                        local p1 = L.leer_registro("A")
                        local cf = 0
                        if (L.leer_registro("CF") == 1) then
                            cf = 1
                        end
                        local v = p1 - p2 - cf
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        -- FIX: H
                        -- FIX: V
                        L.escribir_registro("NF", 0)
                        -- FIX: C
                        L.escribir_registro("A", v)
                    end
                },
                {
                    { 0xa6, true }, -- AND %A, [%IX, d]
                    function (bytes)
                        local p2 = L.leer_memoria(L.leer_registro("IX") + bytes[3])
                        local p1 = L.leer_registro("A")
                        local v = p1 & p2
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF", 1)
                        -- FIX: V
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", 0)
                        L.escribir_registro("A", v)
                    end
                },
                {
                    { 0xae, true }, -- XOR %A, [%IX, d]
                    function (bytes)
                        local p2 = L.leer_memoria(L.leer_registro("IX") + bytes[3])
                        local p1 = L.leer_registro("A")
                        local v = p1 ~ p2
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF", 0)
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", 0)
                        L.escribir_registro("A", v)
                    end
                },
                {
                    { 0xb6, true }, -- OR %A, [%IX, d]
                    function (bytes)
                        local p2 = L.leer_memoria(L.leer_registro("IX") + bytes[3])
                        local p1 = L.leer_registro("A")
                        local v = p1 | p2
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF", 0)
                        -- FIX: V
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", 0)
                        L.escribir_registro("A", v)
                    end
                },
                {
                    { 0xbe, true }, -- CP %A, [%IX, d]
                    function (bytes)
                        local p2 = L.leer_memoria(L.leer_registro("IX") + bytes[3])
                        local p1 = L.leer_registro("A")
                        local v = p1 - p2
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        -- FIX: H
                        -- FIX: V
                        L.escribir_registro("NF", 1)
                        -- FIX: C
                    end
                },
                {
                    { 0xcb, true, 6 }, -- RLC [%IX, d]
                    function (bytes)
                        local d = L.leer_registro("IX") + bytes[3]
                        local ixv = L.leer_memoria(d)
                        local b7 = (ixv & 0x80) >> 7
                        local v = ((ixv << 1) & 0xff) + b7
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF", 0)
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", b7)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0xcb, true, 0x16 }, -- RL [%IX, d]
                    function (bytes)
                        local d = L.leer_registro("IX") + bytes[3]
                        local ixv = L.leer_memoria(d)
                        local v = ((ixv << 1) & 0xff) + L.leer_registro("CF")
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", (ixv & 0x80) >> 7)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0xcb, true,  0xe }, -- RRC [%IX, d]
                    function (bytes)
                        local d = L.leer_registro("IX") + bytes[3]
                        local ixv = L.leer_memoria(d)
                        local b0 = ixv & 1
                        local v = (ixv >> 1) + (b0 << 7)
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", b1)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0xcb, true, 0x1e }, -- RR [%IX, d]
                    function (bytes)
                        local d = L.leer_registro("IX") + bytes[3]
                        local ixv = L.leer_memoria(d)
                        local v = (ixv >> 1) + L.leer_registro("CF")
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", ixv & 1)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0xcb, true, 0x26 }, -- SLA [%IX, d]
                    function (bytes)
                        local d = L.leer_registro("IX") + bytes[3]
                        local ixv = L.leer_registro(d)
                        local v = (ixv << 1) & 0xff
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", (ixv & 0x80) >> 7)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0xcb, true, 0x2e }, -- SRA [%IX, d]
                    function (bytes)
                        local d = L.leer_registro("IX") + bytes[3]
                        local ixv = L.leer_registro(d)
                        local v = (ixv >> 1) + (ixv & 0x80)
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", ixv & 1)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0xcb, true, 0x3e }, -- SRL [%IX, d]
                    function (bytes)
                        local d = L.leer_registro("IX") + bytes[3]
                        local ixv = L.leer_memoria(d)
                        local v = ixv >> 1
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", ixv & 1)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0xcb, true, function (byte) return byte & 0xc7 == 0x46 end }, -- BIT b, [%IX, d]
                    function (bytes)
                        local b = 1 << ((bytes[4] & 0x38) >> 3)
                        local v = L.leer_memoria(L.leer_registro("IX") + bytes[3]) & b
                        -- FIX: S unknown
                        if (v == 0) then
                            L.escribir_registro("ZF", 0)
                        else
                            L.escribir_registro("ZF", 1)
                        end
                        L.escribir_registro("HF", 1)
                        -- FIX: V unknown
                        L.escribir_registro("NF", 0)
                    end
                },
                {
                    { 0xcb, true, function (byte) return byte & 0xc7 == 0xc6 end }, -- SET b, [%IX, d]
                    function (bytes)
                        local b = 1 << ((bytes[4] & 0x38) >> 3)
                        local d = L.leer_registro("IX") + bytes[3]
                        L.escribir_memoria(d, L.leer_memoria(d) | b)
                    end
                },
                {
                    { 0xcb, true, function (byte) return byte & 0xc7 == 0x86 end }, -- RES b, [%IX, d]
                    function (bytes)
                        local b = ~(1 << ((bytes[4] & 0x38) >> 3))
                        local d = L.leer_registro("IX") + bytes[3]
                        L.escribir_memoria(d, L.leer_memoria(d) & b)
                    end
                },
                {
                    { 0xe1 }, -- POP %IX
                    function (bytes)
                        local sp = L.leer_registro("SP")
                        local v = L.leer_memoria(sp) + (L.leer_memoria(sp + 1) << 8)
                        L.escribir_registro("IX", v)
                        L.escribir_registro("SP", sp + 2)
                    end
                },
                {
                    { 0xe3 }, -- EX [%SP], %IX
                    function (bytes)
                        local ix = L.leer_registro("IX")
                        local sp = L.leer_registro("SP")
                        local dspl = L.leer_memoria(sp)
                        local dsph = L.leer_memoria(sp + 1)
                        L.escribir_registro("IX", (dsph << 8) + dspl)
                        L.escribir_memoria(sp, ix & 0xff)
                        L.escribir_memoria(sp + 1, (ix & 0xff00) >> 8)
                    end
                },
                {
                    { 0xe5 }, -- PUSH %IX
                    function (bytes)
                        local sp = L.leer_registro("SP")
                        local ix = L.leer_registro("IX")
                        L.escribir_memoria(sp - 2, ix & 0xff)
                        L.escribir_memoria(sp - 1, (ix & 0xff00) >> 8)
                        L.escribir_registro("SP", sp - 2)
                    end
                },
                {
                    { 0xe9 }, -- JP %IX
                    function (bytes)
                        L.escribir_registro("PC", L.leer_registro("IX"))
                    end
                },
                {
                    { 0xf9 }, -- LD %SP, %IX
                    function (bytes)
                        L.escribir_registro("SP", L.leer_registro("IX"))
                    end
                },
                {
                    { function (byte) return byte & 0xc7 == 0x46 end, true }, -- LD r, [%IX, d]
                    function (bytes)
                        local r = r_vals_r[(bytes[1] & 0x38) >> 3]
                        local dix = L.leer_registro("IX") + bytes[3]
                        L.escribir_registro(r, L.leer_memoria(dix))
                    end
                },
                {
                    { function (byte) return byte & 0xf8 == 0x70 end, true }, -- LD [%IX, d], r
                    function (bytes)
                        local r = r_vals_r[bytes[2] & 7]
                        local dix = L.leer_registro("IX") + bytes[3]
                        L.escribir_memoria(dix, r)
                    end
                },
                {
                    { function (byte) return byte & 0xcf == 9 end }, -- ADD %IX, pp
                    function (bytes)
                        local ix = L.leer_registro("IX")
                        local pp = L.leer_registro(pp_vals_pp[(bytes[2] & 0x30) >> 4])
                        local v = ix + pp
                        -- FIX: H
                        L.escribir_registro("NF", 0)
                        -- FIX: C
                        L.escribir_registro("IX", v)
                    end
                }
            }
        },
        {
            { 0xde, true }, -- SBC %A, n
            function (bytes)
                local cf = 0
                if (L.leer_registro("CF") == 1) then
                    cf = 1
                end
                local v = L.leer_registro("A") - bytes[2] - cf
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                -- FIX: H
                -- FIX: V
                L.escribir_registro("NF", 0)
                -- FIX: C
                L.escribir_registro("A", v)
            end
        },
        {
            { 0xe3 }, -- EX [%SP], %HL
            function (bytes)
                local h = L.leer_registro("H")
                local l = L.leer_registro("L")
                local sp = L.leer_registro("SP")
                local dspl = L.leer_memoria(sp)
                local dsph = L.leer_memoria(sp + 1)
                L.escribir_registro("H", dsph)
                L.escribir_registro("L", dspl)
                L.escribir_memoria(sp, l)
                L.escribir_memoria(sp + 1, h)
            end
        },
        {
            { 0xe6, true }, -- AND %A, n
            function (bytes)
                local v = L.leer_registro("A") & bytes[2]
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                L.escribir_registro("HF", 1)
                -- FIX: V
                L.escribir_registro("NF", 0)
                L.escribir_registro("CF", 0)
                L.escribir_registro("A", v)
            end
        },
        {
            { 0xe9 }, -- JP %HL
            function (bytes)
                L.escribir_registro("PC", L.leer_registro("HL"))
            end
        },
        {
            { 0xeb }, -- EX %DE, %HL
            function (bytes)
                local de = L.leer_registro("DE")
                local hl = L.leer_registro("HL")
                L.escribir_registro("DE", hl)
                L.escribir_registro("HL", de)
            end
        },
        {
            { 0xed },
            {
                {
                    { 0x44 }, -- NEG
                    function (bytes)
                        local v_viejo = L.leer_registro("A")
                        local v = ~v_viejo
                        L.escribir_registro("A", v)
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        -- FIX: H
                        if (v_viejo == 0x80) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 1)
                        if (v_viejo == 0) then
                            L.escribir_registro("CF", 0)
                        else
                            L.escribir_registro("CF", 1)
                        end
                    end
                },
                {
                    { 0x45 }, -- RETN
                    function (byte) return 1/nil end
                },
                {
                    {
                        function (byte)
                            return byte == 0x46 or byte == 0x56 or byte == 0x5e
                        end
                    },
                    function (bytes)
                        L.escribir_registro("IM", math.floor(bytes[2]/10) - 7)
                    end
                },
                {
                    { 0x47 }, -- LD %I, %A
                    function (bytes) L.escribir_registro("I", L.leer_registro("A")) end
                },
                {
                    { 0x4d }, -- RETI
                    function (byte) return 1/nil end
                },
                {
                    { 0x4f }, -- LD %R, %A
                    function (bytes) L.escribir_registro("R", L.leer_registro("A")) end
                },
                {
                    { 0x57 }, -- LD %A, %I
                    function (bytes) L.escribir_registro("A", L.leer_registro("I")) end
                },
                {
                    { 0x5f }, -- LD %A, %R
                    function (bytes) L.escribir_registro("A", L.leer_registro("R")) end
                },
                {
                    { 0x67 }, -- RRD
                    function (bytes)
                        local a = L.leer_registro("A")
                        local dhl = L.leer_registro("HL")
                        local hl = L.leer_memoria(dhl)
                        local va = (a & 0xf0) + (hl & 0xf)
                        if (va < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (va == 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        L.escribir_registro("HF", 0)
                        if (va % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("A", va)
                        L.escribir_memoria(dhl, ((hl & 0xf0) >> 4) + ((a & 0xf) << 4))
                    end
                },
                {
                    { 0x6f }, -- RLD
                    function (bytes)
                        local a = L.leer_registro("A")
                        local dhl = L.leer_registro("HL")
                        local hl = L.leer_memoria(dhl)
                        local va = (a & 0xf0) + ((hl & 0xf0) >> 4)
                        if (va < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (va == 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        L.escribir_registro("HF", 0)
                        if (va % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("A", va)
                        L.escribir_memoria(dhl, ((hl & 0xf) << 4) + (a & 0xf))
                    end
                },
                {
                    { 0xa0 }, -- LDI
                    function (bytes)
                        local dhl = L.leer_memoria(L.leer_registro("HL"))
                        L.escribir_memoria(L.leer_registro("DE"), dhl)
                        L.escribir_registro("DE", L.leer_registro("DE") + 1)
                        L.escribir_registro("HL", L.leer_registro("HL") + 1)
                        L.escribir_registro("BC", L.leer_registro("BC") - 1)
                    end
                },
                {
                    { 0xa1 }, -- CPI
                    function (bytes)
                        local comp = L.leer_registro("A") - L.leer_memoria(L.leer_registro("HL"))
                        if (comp < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (comp == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        --FIX: H
                        L.escribir_registro("NF", 1)
                        L.escribir_registro("HL", L.leer_registro("HL") + 1)
                        local bc = L.leer_registro("BC") - 1
                        L.escribir_registro("BC", bc)
                        if (bc ~= 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                    end
                },
                {
                    { 0xa8 }, -- LDD
                    function (bytes)
                        local dhl = L.leer_memoria(L.leer_registro("HL"))
                        L.escribir_memoria(L.leer_registro("DE"), dhl)
                        L.escribir_registro("DE", L.leer_registro("DE") - 1)
                        L.escribir_registro("HL", L.leer_registro("HL") - 1)
                        L.escribir_registro("BC", L.leer_registro("BC") - 1)
                    end
                },
                {
                    { 0xa9 }, -- CPD
                    function (bytes)
                        local comp = L.leer_registro("A") - L.leer_memoria(L.leer_registro("HL"))
                        if (comp < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (comp == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        --FIX: H
                        L.escribir_registro("NF", 1)
                        L.escribir_registro("HL", L.leer_registro("HL") - 1)
                        local bc = L.leer_registro("BC") - 1
                        L.escribir_registro("BC", bc)
                        if (bc ~= 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                    end
                },
                {
                    { 0xb0 }, -- LDIR
                    function (bytes)
                        while (L.leer_registro("BC") ~= 0) do
                            local dhl = L.leer_memoria(L.leer_registro("HL"))
                            L.escribir_memoria(L.leer_registro("DE"), dhl)
                            L.escribir_registro("DE", L.leer_registro("DE") + 1)
                            L.escribir_registro("HL", L.leer_registro("HL") + 1)
                            L.escribir_registro("BC", L.leer_registro("BC") - 1)
                        end
                    end
                },
                {
                    { 0xb1 }, -- CPIR
                    function (bytes)
                        repeat
                            local bc = L.leer_registro("BC") - 1
                            local hl = L.leer_registro("HL")
                            local comp = L.leer_registro("A") - L.leer_memoria(hl)
                            L.escribir_registro("BC", bc)
                            if (comp < 0) then
                                L.escribir_registro("SF", 1)
                            else
                                L.escribir_registro("SF", 0)
                            end
                            if (comp == 0) then
                                L.escribir_registro("ZF", 1)
                            else
                                L.escribir_registro("ZF", 0)
                            end
                            -- FIX: H
                            if (bc ~= 0) then
                                L.escribir_registro("VF", 1)
                            else
                                L.escribir_registro("VF", 0)
                            end
                            L.escribir_registro("NF", 1)
                            if (bc ~= 0 and comp ~= 0) then
                                L.escribir_registro("HL", hl + 1)
                                L.escribir_registro("BC", bc)
                            end
                        until (bc ~= 0 and comp ~= 0)
                    end
                },
                {
                    { 0xb8 }, -- LDDR
                    function (bytes)
                        while (L.leer_registro("BC") ~= 0) do
                            local dhl = L.leer_memoria(L.leer_registro("HL"))
                            L.escribir_memoria(L.leer_registro("DE"), dhl)
                            L.escribir_registro("DE", L.leer_registro("DE") - 1)
                            L.escribir_registro("HL", L.leer_registro("HL") - 1)
                            L.escribir_registro("BC", L.leer_registro("BC") - 1)
                        end
                    end
                },
                {
                    { 0xb9 }, -- CPDR
                    function (bytes)
                        repeat
                            local bc = L.leer_registro("BC") - 1
                            local hl = L.leer_registro("HL")
                            local comp = L.leer_registro("A") - L.leer_memoria(hl)
                            L.escribir_registro("BC", bc)
                            if (comp < 0) then
                                L.escribir_registro("SF", 1)
                            else
                                L.escribir_registro("SF", 0)
                            end
                            if (comp == 0) then
                                L.escribir_registro("ZF", 1)
                            else
                                L.escribir_registro("ZF", 0)
                            end
                            -- FIX: H
                            if (bc ~= 0) then
                                L.escribir_registro("VF", 1)
                            else
                                L.escribir_registro("VF", 0)
                            end
                            L.escribir_registro("NF", 1)
                            if (bc ~= 0 and comp ~= 0) then
                                L.escribir_registro("HL", hl - 1)
                                L.escribir_registro("BC", bc)
                            end
                        until (bc ~= 0 and comp ~= 0)
                    end
                },
                {
                    { function (byte) return (byte & 0xcf) == 0x4b end, true, true }, -- LD ss, [nn]
                    function (bytes)
                        local ss = ss_vals_ss[(bytes[1] & 0x30) >> 4]
                        local nn = bytes[3] + (bytes[4] << 8)
                        L.escribir_registro(ss, L.leer_memoria(nn) + (L.leer_memoria(nn + 1) << 8))
                    end
                },
                {
                    { function (byte) return byte & 0xcf == 0x42 end, true, true }, -- LD [nn], ss
                    function (bytes)
                        local v = L.leer_registro(ss_vals_ss[(bytes[1] & 0x30) >> 4])
                        local nn = bytes[2] + (bytes[3] << 8)
                        L.escribir_memoria(nn, v & 0xff)
                        L.escribir_memoria(nn + 1, (v & 0xff00) >> 8)
                    end
                },
                {
                    { function (byte) return byte & 0xcf == 0x4a end }, -- ADC %HL, ss
                    function (bytes)
                        local hl = L.leer_registro("HL")
                        local ss = L.leer_registro(ss_vals_ss[(bytes[2] & 0x30) >> 4])
                        local cf = L.leer_registro("CF")
                        local v = hl + ss + cf
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        -- FIX: H
                        -- FIX: V
                        L.escribir_registro("NF", 0)
                        -- FIX: C
                        L.escribir_registro("HL", v)
                    end
                },
                {
                    { function (byte) return byte & 0xcf == 0x42 end }, -- SBC %HL, ss
                    function (bytes)
                        local hl = L.leer_registro("HL")
                        local ss = L.leer_registro(ss_vals_ss[(bytes[2] & 0x30) >> 4])
                        local cf = L.leer_registro("CF")
                        local v = hl - ss - cf
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        -- FIX: H
                        -- FIX: V
                        L.escribir_registro("NF", 1)
                        -- FIX: C
                        L.escribir_registro("HL", v)
                    end
                }
            }
        },
        {
            { 0xee, true }, -- XOR %A, n
            function (bytes)
                local v = L.leer_registro("A") ~ bytes[2]
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                L.escribir_registro("HF", 0)
                if (v % 2 == 0) then
                    L.escribir_registro("VF", 1)
                else
                    L.escribir_registro("VF", 0)
                end
                L.escribir_registro("NF", 0)
                L.escribir_registro("CF", 0)
                L.escribir_registro("A", v)
            end
        },
        {
            { 0xf3 }, -- DI
            function (bytes)
                L.escribir_registro("IFF1", 0)
                L.escribir_registro("IFF2", 0)
            end
        },
        {
            { 0xf6, true }, -- OR %A, n
            function (bytes)
                local v = L.leer_registro("A") | bytes[2]
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                L.escribir_registro("HF", 0)
                -- FIX: V
                L.escribir_registro("NF", 0)
                L.escribir_registro("CF", 0)
                L.escribir_registro("A", v)
            end
        },
        {
            { 0xf9 }, -- LD %SP, %HL
            function (bytes)
                L.escribir_registro("SP", L.leer_registro("HL"))
            end
        },
        {
            { 0xfb }, -- EI
            function (bytes)
                L.escribir_registro("IFF1", 1)
                L.escribir_registro("IFF2", 1)
            end
        },
        {
            { 0xfd },
            {
                {
                    { 0x21, true, true }, -- LD %IY, nn
                    function (bytes)
                        L.escribir_registro("IY", bytes[3] + (bytes[4] << 8))
                    end
                },
                {
                    { 0x22, true, true }, -- LD [nn], %IY
                    function (bytes)
                        local nn = bytes[3] + (bytes[4] << 8)
                        local iy = L.leer_registro("IY")
                        L.escribir_memoria(nn, iy & 0xff)
                        L.escribir_memoria(nn + 1, (iy & 0xff00) >> 8)
                    end
                },
                {
                    { 0x23 }, -- INC %IY
                    function (bytes)
                        L.escribir_registro("IY", L.leer_registro("IY") + 1)
                    end
                },
                {
                    { 0x2a, true, true }, -- LD %IY, [nn]
                    function (bytes)
                        local nn = bytes[3] + (bytes[4] << 8)
                        L.escribir_registro("IX", L.leer_memoria(nn) + (L.leer_memoria(nn + 1) << 8))
                    end
                },
                {
                    { 0x2b }, -- DEC %IY
                    function (bytes)
                        L.escribir_registro("IY", L.leer_registro("IY") - 1)
                    end
                },
                {
                    { 0x34, true }, -- INC [%IY, d]
                    function (bytes)
                        local d = L.leer_registro("IY") + bytes[3]
                        local v = L.leer_memoria(d) + 1
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        -- FIX: H
                        if (v == 0x7f) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0x35, true }, -- DEC [%IY, d]
                    function (bytes)
                        local d = L.leer_registro("IY") + bytes[3]
                        local v = L.leer_memoria(d) - 1
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        -- FIX: H
                        if (v == 0x80) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 1)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0x36, true, true }, -- LD [%IY, d], n
                    function (bytes)
                        local diy = L.leer_registro("IY") + bytes[3]
                        L.escribir_memoria(diy, bytes[4])
                    end
                },
                {
                    { 0x86, true }, -- ADD %A, [%IY, d]
                    function (bytes)
                        local p2 = L.leer_memoria(L.leer_registro("IY") + bytes[3])
                        local p1 = L.leer_registro("A")
                        local v = p1 + p2
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        -- FIX: H
                        -- FIX: V
                        L.escribir_registro("NF", 0)
                        -- FIX: C
                        L.escribir_registro("A", v)
                    end
                },
                {
                    { 0x8e, true }, -- ADC %A, [%IY, d]
                    function (bytes)
                        local p2 = L.leer_memoria(L.leer_registro("IY") + bytes[3])
                        local p1 = L.leer_registro("A")
                        local cf = 0
                        if (L.leer_registro("CF") == 1) then
                            cf = 1
                        end
                        local v = p1 + p2 + cf
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        -- FIX: H
                        -- FIX: V
                        L.escribir_registro("NF", 0)
                        -- FIX: C
                        L.escribir_registro("A", v)
                    end
                },
                {
                    { 0x96, true }, -- SUB %A, [%IY, d]
                    function (bytes)
                        local p2 = L.leer_memoria(L.leer_registro("IY") + bytes[3])
                        local p1 = L.leer_registro("A")
                        local v = p1 - p2
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        -- FIX: H
                        -- FIX: V
                        L.escribir_registro("NF", 0)
                        -- FIX: C
                        L.escribir_registro("A", v)
                    end
                },
                {
                    { 0x9e, true }, -- SBC %A, [%IY, d]
                    function (bytes)
                        local p2 = L.leer_memoria(L.leer_registro("IY") + bytes[3])
                        local p1 = L.leer_registro("A")
                        local cf = 0
                        if (L.leer_registro("CF") == 1) then
                            cf = 1
                        end
                        local v = p1 - p2 - cf
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        -- FIX: H
                        -- FIX: V
                        L.escribir_registro("NF", 0)
                        -- FIX: C
                        L.escribir_registro("A", v)
                    end
                },
                {
                    { 0xa6, true }, -- AND %A, [%IY, d]
                    function (bytes)
                        local p2 = L.leer_memoria(L.leer_registro("IY") + bytes[3])
                        local p1 = L.leer_registro("A")
                        local v = p1 & p2
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF", 1)
                        -- FIX: V
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", 0)
                        L.escribir_registro("A", v)
                    end
                },
                {
                    { 0xae, true }, -- XOR %A, [%IY, d]
                    function (bytes)
                        local p2 = L.leer_memoria(L.leer_registro("IY") + bytes[3])
                        local p1 = L.leer_registro("A")
                        local v = p1 ~ p2
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF", 0)
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", 0)
                        L.escribir_registro("A", v)
                    end
                },
                {
                    { 0xb6, true }, -- OR %A, [%IY, d]
                    function (bytes)
                        local p2 = L.leer_memoria(L.leer_registro("IY") + bytes[3])
                        local p1 = L.leer_registro("A")
                        local v = p1 | p2
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF", 0)
                        -- FIX: V
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", 0)
                        L.escribir_registro("A", v)
                    end
                },
                {
                    { 0xbe, true }, -- CP %A, [%IY, d]
                    function (bytes)
                        local p2 = L.leer_memoria(L.leer_registro("IY") + bytes[3])
                        local p1 = L.leer_registro("A")
                        local v = p1 - p2
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        -- FIX: H
                        -- FIX: V
                        L.escribir_registro("NF", 1)
                        -- FIX: C
                    end
                },
                {
                    { 0xcb, true, 6 }, -- RLC [%IY, d]
                    function (bytes)
                        local d = L.leer_registro("IY") + bytes[3]
                        local iyv = L.leer_memoria(d)
                        local b7 = (iyv & 0x80) >> 7
                        local v = ((hlv << 1) & 0xff) + b7
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF", 0)
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", b7)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0xcb, true, 0x16 }, -- RL [%IY, d]
                    function (bytes)
                        local d = L.leer_registro("IY") + bytes[3]
                        local iyv = L.leer_memoria(d)
                        local v = ((iyv << 1) & 0xff) + L.leer_registro("CF")
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", (iyv & 0x80) >> 7)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0xcb, true,  0xe }, -- RRC [%IY, d]
                    function (bytes)
                        local d = L.leer_registro("IY") + bytes[3]
                        local iyv = L.leer_memoria(d)
                        local b0 = iyv & 1
                        local v = (iyv >> 1) + (b0 << 7)
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", b1)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0xcb, true, 0x1e }, -- RR [%IY, d]
                    function (bytes)
                        local d = L.leer_registro("IY") + bytes[3]
                        local iyv = L.leer_memoria(d)
                        local v = (iyv >> 1) + L.leer_registro("CF")
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", iyv & 1)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0xcb, true, 0x26 }, -- SLA [%IY, d]
                    function (bytes)
                        local d = L.leer_registro("IY") + bytes[3]
                        local iyv = L.leer_registro(d)
                        local v = (iyv << 1) & 0xff
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", (iyv & 0x80) >> 7)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0xcb, true, 0x2e }, -- SRA [%IY, d]
                    function (bytes)
                        local d = L.leer_registro("IY") + bytes[3]
                        local iyv = L.leer_registro(d)
                        local v = (iyv >> 1) + (iyv & 0x80)
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", iyv & 1)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0xcb, true, 0x3e }, -- SRL [%IY, d]
                    function (bytes)
                        local d = L.leer_registro("IY") + bytes[3]
                        local iyv = L.leer_memoria(d)
                        local v = iyv >> 1
                        if (v < 0) then
                            L.escribir_registro("SF", 1)
                        else
                            L.escribir_registro("SF", 0)
                        end
                        if (v == 0) then
                            L.escribir_registro("ZF", 1)
                        else
                            L.escribir_registro("ZF", 0)
                        end
                        L.escribir_registro("HF")
                        if (v % 2 == 0) then
                            L.escribir_registro("VF", 1)
                        else
                            L.escribir_registro("VF", 0)
                        end
                        L.escribir_registro("NF", 0)
                        L.escribir_registro("CF", iyv & 1)
                        L.escribir_memoria(d, v)
                    end
                },
                {
                    { 0xcb, true, function (byte) return byte & 0xc7 == 0x46 end }, -- BIT b, [%IY, d]
                    function (bytes)
                        local b = 1 << ((bytes[4] & 0x38) >> 3)
                        local v = L.leer_memoria(L.leer_registro("IY") + bytes[3]) & b
                        -- FIX: S unknown
                        if (v == 0) then
                            L.escribir_registro("ZF", 0)
                        else
                            L.escribir_registro("ZF", 1)
                        end
                        L.escribir_registro("HF", 1)
                        -- FIX: V unknown
                        L.escribir_registro("NF", 0)
                    end
                },
                {
                    { 0xcb, true, function (byte) return byte & 0xc7 == 0xc6 end }, -- SET b, [%IY, d]
                    function (bytes)
                        local b = 1 << ((bytes[4] & 0x38) >> 3)
                        local d = L.leer_registro("IY") + bytes[3]
                        L.escribir_memoria(d, L.leer_memoria(d) | b)
                    end
                },
                {
                    { 0xcb, true, function (byte) return byte & 0xc7 == 0x86 end }, -- RES b, [%IY, d]
                    function (bytes)
                        local b = ~(1 << ((bytes[4] & 0x38) >> 3))
                        local d = L.leer_registro("IY") + bytes[3]
                        L.escribir_memoria(d, L.leer_memoria(d) & b)
                    end
                },
                {
                    { 0xe1 }, -- POP %IY
                    function (bytes)
                        local sp = L.leer_registro("SP")
                        local v = L.leer_memoria(sp) + (L.leer_memoria(sp + 1) << 8)
                        L.escribir_registro("IY", v)
                        L.escribir_registro("SP", sp + 2)
                    end
                },
                {
                    { 0xe3 }, -- EX [%SP], %IY
                    function (bytes)
                        local iy = L.leer_registro("IY")
                        local sp = L.leer_registro("SP")
                        local dspl = L.leer_memoria(sp)
                        local dsph = L.leer_memoria(sp + 1)
                        L.escribir_registro("IY", (dsph << 8) + dspl)
                        L.escribir_memoria(sp, iy & 0xff)
                        L.escribir_memoria(sp + 1, (iy & 0xff00) >> 8)
                    end
                },
                {
                    { 0xe5 }, -- PUSH %IX
                    function (bytes)
                        local sp = L.leer_registro("SP")
                        local iy = L.leer_registro("IY")
                        L.escribir_memoria(sp - 2, iy & 0xff)
                        L.escribir_memoria(sp - 1, (iy & 0xff00) >> 8)
                        L.escribir_registro("SP", sp - 2)
                    end
                },
                {
                    { 0xe9 }, -- JP %IY
                    function (bytes)
                        L.escribir_registro("PC", L.leer_registro("IY"))
                    end
                },
                {
                    { 0xf9 }, -- LD %SP, %IY
                    function (bytes)
                        L.escribir_registro("SP", L.leer_registro("IY"))
                    end
                },
                {
                    { function (byte) return byte & 0xc7 == 0x46 end, true }, -- LD r, [%IY, d]
                    function (bytes)
                        local r = r_vals_r((bytes[1] & 0x38) >> 3)
                        local diy = L.leer_registro("IY") + bytes[3]
                        L.escribir_registro(r, L.leer_memoria(diy))
                    end
                },
                {
                    { function (byte) return byte & 0xf8 == 0x70 end, true }, -- LD [%IY, d], r
                    function (bytes)
                        local r = r_vals_r[bytes[2] & 7]
                        local diy = L.leer_registro("IY") + bytes[3]
                        L.escribir_memoria(diy, r)
                    end
                },
                {
                    { function (byte) return byte & 0xcf == 9 end }, -- ADD %IY, pp
                    function (bytes)
                        local iy = L.leer_registro("IY")
                        local pp = L.leer_registro(rr_vals_rr[(bytes[2] & 0x30) >> 4])
                        local v = iy + pp
                        -- FIX: H
                        L.escribir_registro("NF", 0)
                        -- FIX: C
                        L.escribir_registro("IY", v)
                    end
                }
            }
        },
        {
            { 0xfe, true }, -- CP %A, n
            function (bytes)
                local v = L.leer_registro("A") - bytes[2]
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                -- FIX: H
                -- FIX: V
                L.escribir_registro("NF", 1)
                -- FIX: C
            end
        },
        {
            { function (byte) return byte & 0xc7 == 0x46 end }, -- LD r, [%HL]
            function (bytes)
                local r = r_vals_r[(bytes[1] & 0x38) >> 3]
                local v = L.leer_memoria(L.leer_registro("HL"))
                L.escribir_registro(r, v)
            end
        },
        {
            { function (byte) return byte & 0xf8 == 0x70 end }, -- LD [%HL], r
            function (bytes)
                local r = r_vals_r[bytes[1] & 7]
                local v = L.leer_registro(r)
                L.escribir_memoria(L.leer_registro("HL"), v)
            end
        },
        {
            { function (byte) return byte & 0xc0 == 0x40 end }, -- LD r, r
            function (byte)
                local r1 = r_vals_r[(bytes[1] & 0x38) >> 3]
                local r2 = r_vals_r[bytes[1] & 7]
                L.escribir_registro(r1, L.leer_registro(r2))
            end
        },
        {
            { function (byte) return byte & 0xc7 == 6 end, true }, -- LD r, n
            function (bytes)
                local r = r_vals_r[(bytes[1] & 0x38) >> 3]
                L.escribir_registro(r, bytes[2])
            end
        },
        {
            { function (byte) return byte & 0xcf == 1 end, true, true }, -- LD ss, nn
            function (bytes)
                local ss = ss_vals_ss[(bytes[1] & 0x30) >> 4]
                L.escribir_registro(ss, bytes[2] + (bytes[3] << 8))
            end
        },
        {
            { function (byte) return byte & 0xcf == 0xc5 end }, -- PUSH qq
            function (bytes)
                local sp = L.leer_registro("SP")
                local v = L.leer_registro(qq_vals_qq[(bytes[1] & 0x30)] >> 4)
                L.escribir_memoria(sp - 2, v & 0xff)
                L.escribir_memoria(sp - 1, (v & 0xff00) >> 8)
                L.escribir_registro("SP", sp - 2)
            end
        },
        {
            { function (byte) return byte & 0xcf == 0xc1 end }, -- POP qq
            function (bytes)
                local sp = L.leer_registro("SP")
                local v = L.leer_memoria(sp) + (L.leer_memoria(sp + 1) << 8)
                L.escribir_registro(qq_vals_qqp[(bytes[1] & 0x30) >> 4], v)
                L.escribir_registro("SP", sp + 2)
            end
        },
        {
            { function (byte) return byte & 0xf8 == 0x80 end }, -- ADD %A, r
            function (bytes)
                local r = L.leer_registro(r_vals_r[bytes[1] & 7])
                local a = L.leer_registro("A")
                local v = r + a
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                -- FIX: H
                -- FIX: V
                L.escribir_registro("NF", 0)
                -- FIX: C
                L.escribir_registro("A", v)
            end
        },
        {
            { function (byte) return byte & 0xf8 == 0x88 end }, -- ADC %A, r
            function (bytes)
                local r = L.leer_registro(r_vals_r[bytes[1] & 7])
                local a = L.leer_registro("A")
                local cf = 0
                if (L.leer_registro("CF") == 1) then
                    cf = 1
                end
                local v = r + a + cf
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                -- FIX: H
                -- FIX: V
                L.escribir_registro("NF", 0)
                -- FIX: C
                L.escribir_registro("A", v)
            end
        },
        {
            { function (byte) return byte & 0xf8 == 0x90 end }, -- SUB %A, r
            function (bytes)
                local r = L.leer_registro(r_vals_r[bytes[1] & 7])
                local a = L.leer_registro("A")
                local v = a - r
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                -- FIX: H
                -- FIX: V
                L.escribir_registro("NF", 0)
                -- FIX: C
                L.escribir_registro("A", v)
            end
        },
        {
            { function (byte) return byte & 0xf8 == 0x98 end }, -- SBC %A, r
            function (bytes)
                local r = L.leer_registro(r_vals_r[bytes[1] & 7])
                local a = L.leer_registro("A")
                local cf = 0
                if (L.leer_registro("CF") == 1) then
                    cf = 1
                end
                local v = a - r - cf
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                -- FIX: H
                -- FIX: V
                L.escribir_registro("NF", 0)
                -- FIX: C
                L.escribir_registro("A", v)
            end
        },
        {
            { function (byte) return byte & 0xf8 == 0xa0 end }, -- AND %A, r
            function (bytes)
                local r = L.leer_registro(r_vals_r[bytes[1] & 7])
                local a = L.leer_registro("A")
                local v = a & r
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                L.escribir_registro("HF", 1)
                -- FIX: V
                L.escribir_registro("NF", 0)
                L.escribir_registro("CF", 0)
                L.escribir_registro("A", v)
            end
        },
        {
            { function (byte) return byte & 0xf8 == 0xb0 end }, -- OR %A, r
            function (bytes)
                local r = L.leer_registro(r_vals_r[bytes[1] & 7])
                local a = L.leer_registro("A")
                local v = a | r
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                L.escribir_registro("HF", 0)
                -- FIX: V
                L.escribir_registro("NF", 0)
                L.escribir_registro("CF", 0)
                L.escribir_registro("A", v)
            end
        },
        {
            { function (byte) return byte & 0xf8 == 0xa8 end }, -- XOR %A, r
            function (bytes)
                local r = L.leer_registro(r_vals_r[bytes[1] & 7])
                local a = L.leer_registro("A")
                local v = a ~ r
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                L.escribir_registro("HF", 0)
                if (v % 2 == 0) then
                    L.escribir_registro("VF", 1)
                else
                    L.escribir_registro("VF", 0)
                end
                L.escribir_registro("NF", 0)
                L.escribir_registro("CF", 0)
                L.escribir_registro("A", v)
            end
        },
        {
            { function (byte) return byte & 0xf8 == 0xb8 end }, -- CP %A, r
            function (bytes)
                local r = L.leer_registro(r_vals_r[bytes[1] & 7])
                local a = L.leer_registro("A")
                local v = a - r
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                -- FIX: H
                -- FIX: V
                L.escribir_registro("NF", 1)
                -- FIX: C
            end
        },
        {
            { function (byte) return byte & 0xc7 == 4 end }, -- INC r
            function (bytes)
                local r = r_vals_r[(bytes[1] & 0x38) >> 3]
                local v = L.leer_registro(r) + 1
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                -- FIX: H
                if (v == 0x7f) then
                    L.escribir_registro("VF", 1)
                else
                    L.escribir_registro("VF", 0)
                end
                L.escribir_registro("NF", 0)
                L.escribir_registro(r, v)
            end
        },
        {
            { function (byte) return byte & 0xc7 == 5 end }, -- DEC r
            function (bytes)
                local r = r_vals_r[(bytes[1] & 0x38) >> 3]
                local v = L.leer_registro(r) - 1
                if (v < 0) then
                    L.escribir_registro("SF", 1)
                else
                    L.escribir_registro("SF", 0)
                end
                if (v == 0) then
                    L.escribir_registro("ZF", 1)
                else
                    L.escribir_registro("ZF", 0)
                end
                -- FIX: H
                if (v == 0x80) then
                    L.escribir_registro("VF", 1)
                else
                    L.escribir_registro("VF", 0)
                end
                L.escribir_registro("NF", 1)
                L.escribir_registro(r, v)
            end
        },
        {
            { function (byte) return byte & 0xcf == 9 end }, -- ADD %HL, ss
            function (bytes)
                local hl = L.leer_registro("HL")
                local ss = L.leer_registro(ss_vals_ss[(bytes[1] & 0x30) >> 4])
                local v = hl + ss
                -- FIX: H
                L.escribir_registro("NF", 0)
                -- FIX: C
                L.escribir_registro("HL", v)
            end
        },
        {
            { function (byte) return byte & 0xcf == 3 end }, -- INC ss
            function (bytes)
                local ss = ss_vals_ss[(bytes[1] & 0x30) >> 4]
                L.escribir_registro(ss, L.leer_registro(ss) + 1)
            end
        },
        {
            { function (byte) return byte & 0xcf == 0xb end }, -- DEC ss
            function (bytes)
                local ss = ss_vals_ss[(bytes[1] & 0x30) >> 4]
                L.escribir_registro(ss, L.leer_registro(ss) - 1)
            end
        },
        {
            { function (byte) return byte & 0xc7 == 0xc2 end, bytes, bytes }, -- JP cc, nn
            function (bytes)
                local idx = (bytes[1] & 0x38) >> 3
                local cc = cc_vals_cc[idx]
                if (cc == "NZ" and L.leer_registro("ZF") == 1) or
                    (cc == "Z" and L.leer_registro("ZF") == 0) or
                    (cc == "NC" and L.leer_registro("CF") == 1) or
                    (cc == "C" and L.leer_registro("CF") == 0) or
                    (cc == "PO" and L.leer_registro("VF") == 1) or
                    (cc == "PE" and L.leer_registro("VF") == 0) or
                    (cc == "P" and L.leer_registro("SF") == 0) or
                    (cc == "M" and L.leer_registro("SF") == 1) then
                    L.escribir_registro("PC", bytes[2] + (bytes[3] << 8))
                end
            end
        },
        {
            { function (byte) return byte & 0xc7 == 0 end, true }, -- JR ccjr, e
            function (bytes)
                local idx = bytes[1] >> 3
                local cc = ccjr_vals_ccjr[idx]
                if (cc == "C" and L.leer_registro("CF") == 1) or
                    (cc == "NC" and L.leer_registro("CF") == 0) or
                    (cc == "Z" and L.leer_registro("ZF") == 1) or
                    (cc == "NZ" and L.leer_registro("ZF") == 0) then
                    L.escribir_registro("PC", L.leer_registro("PC") + bytes[2])
                end
            end
        },
        {
            { function (byte) return byte & 0xc7 == 0xc4 end, true, true }, -- CALL cc, nn
            function (bytes)
                local idx = (bytes[1] & 0x38) >> 3
                local cc = cc_vals_cc[idx]
                if (cc == "NZ" and L.leer_registro("ZF") == 1) or
                    (cc == "Z" and L.leer_registro("ZF") == 0) or
                    (cc == "NC" and L.leer_registro("CF") == 1) or
                    (cc == "C" and L.leer_registro("CF") == 0) or
                    (cc == "PO" and L.leer_registro("VF") == 1) or
                    (cc == "PE" and L.leer_registro("VF") == 0) or
                    (cc == "P" and L.leer_registro("SF") == 0) or
                    (cc == "M" and L.leer_registro("SF") == 1) then
                    local pc = L.leer_registro("PC") + 3
                    local sp = L.leer_registro("SP")
                    L.escribir_memoria(sp - 1, (pc & 0xff00) >> 8)
                    L.escribir_memoria(sp - 2, pc & 0xff)
                    L.escribir_registro("SP", sp - 2)
                    L.escribir_registro("PC", bytes[2] + (bytes[3] << 8))
                end
            end
        },
        {
            { function (byte) return  end }, -- RET cc
            function (bytes)
                local idx = (bytes[1] & 0x38) >> 3
                local cc = cc_vals_cc[idx]
                if (cc == "NZ" and L.leer_registro("ZF") == 1) or
                    (cc == "Z" and L.leer_registro("ZF") == 0) or
                    (cc == "NC" and L.leer_registro("CF") == 1) or
                    (cc == "C" and L.leer_registro("CF") == 0) or
                    (cc == "PO" and L.leer_registro("VF") == 1) or
                    (cc == "PE" and L.leer_registro("VF") == 0) or
                    (cc == "P" and L.leer_registro("SF") == 0) or
                    (cc == "M" and L.leer_registro("SF") == 1) then
                    local sp = L.leer_registro("SP")
                    local pc = L.leer_memoria(sp) + (L.leer_memoria(sp + 1) << 8)
                    L.escribir_registro("SP", sp + 2)
                    L.escribir_registro("PC", pc)
                end
            end
        },
        {
            { function (byte) return byte & 0xc7 == 0xc7 end }, -- RST p
            function (bytes)
                local pc = L.leer_registro("PC") + 3
                local sp = L.leer_registro("SP")
                L.escribir_memoria(sp - 1, (pc & 0xff00) >> 8)
                L.escribir_memoria(sp - 2, pc & 0xff)
                L.escribir_registro("SP", sp - 2)
                L.escribir_registro("PC", bytes[1] & 0x38)
            end
        }
    },
    desensamble = {
        {
            { 0x76 },
            "HALT"
        },
        {
            { 0xed },
            {
                {
                    function (byte)
                        return byte == 0x46 or byte == 0x56 or byte == 0x5e
                    end,
                    function (bytes)
                        return "IM " .. math.floor(bytes[1]/10)-7
                    end
                },
            }
        }
    }
}
