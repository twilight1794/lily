-- Especificación del microprocesador Zilog Z80

--[[
  Habrá disponible un objeto global para controlar la plataforma, L
  L.leer_memoria(pos)
  L.escribir_memoria(pos, v)
  L.leer_registro(reg)
  L.escribir_registro(reg, v)
]]

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
pt_vals = {
    [0] = 0,
    [0x08] = 1,
    [0x10] = 2,
    [0x18] = 3,
    [0x20] = 4,
    [0x28] = 5,
    [0x30] = 6,
    [0x38] = 7
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
        -- Registros de uso interno
        IM = {
            tamano = 8,
            desplazamiento = 196
        }
    },
    tipos = {
        r = function (v) return r_vals(v) end,
        di = function (v) return type(v) == "table" and (#v == 1 or #v == 2) and i_vals[v[1]] and (not v[2] or (type(v2) == "number" and v[2] >= -128 and v[2] < 128)) end,
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
        pt = function (v) return pt_vals[v] end,
        dhl = function (v) return type(v) == "table" and #v == 1 and v[1] == "SP" end, -- solo para JP
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
                function (dd, dnn) return { 0xed, 0x4a + (ss_vals[ss]<<4), uint16l(dnn[1]) } end
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
                function (r) return { 4 + (r_vals[r]<<3) } end
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
                function (ri) return { i_vals[ri[1]], 0x23 } end
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
                function (ri) return { i_vals[ri[1]], 0x2b } end
            }
        },
        -- Grupo de propósito general y control de CPU
        DAA = { 0x27 },
        CPL = { 0x2f },
        NEG = { 0xed, 0x44 },
        CCF = { 0x3f },
        SCF = { 0x37},
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
                function (r) return { 0xcb, r_vals[r] } end
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
                function (cc, nn) return { 0xc2 + cc_vals[cc], uint16l(nn) } end
            },
            {
                { "dhl" },
                { 0xe9 }
            },
            {
                { "di" },
                function (di) return { i_vals[di[1]], 0xe9 } end
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
                function (cc, nn) return { 0xc4 + cc_vals[cc], uint16l(nn) } end
            }
        },
        RET = {
            {
                {},
                { 0xc9 }
            },
            {
                { "cc" },
                function (cc) return { 0xc0 + cc_vals[cc] } end
            }
        },
        RETI = { 0xed, 0x4d },
        RETN = { 0xed, 0x45 },
        RST = {
            {
                { "pt" },
                function (pt) return { 0xc7 + pt_vals[pt] } end
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
        -- Grupo de carga de 8 bits
        -- Grupo de carga de 16 bits
        -- Grupo de búsqueda, intercambio y transferencia
        -- Grupo aritmético de 8 bits
        -- Grupo aritmético de 16 bits
        -- Grupo de propósito general y control de CPU
        {
            { 0x76 }, -- HALT
            function ()
                lily.detener()
            end
        },
        {
            { 0xed },
            {
                {
                    function (byte)
                        return byte == 0x46 or byte == 0x56 or byte == 0x5e
                    end,
                    function (...)
                        local args = {...}
                        lily.escribir_registro('mi', math.floor(args[2]/10))
                    end
                }
            }
        }

        -- Grupo de rotación y desplazamiento
        -- Grupo de bits
        -- Grupo de salto
        -- Grupo de llamada y retorno
        -- Grupo de E/S
    },
    desensamble = {
        -- Grupo de carga de 8 bits
        -- Grupo de carga de 16 bits
        -- Grupo de búsqueda, intercambio y transferencia
        -- Grupo aritmético de 8 bits
        -- Grupo aritmético de 16 bits
        -- Grupo de propósito general y control de CPU
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
                    function (...)
                        return "IM " .. math.floor(({...})[1]/10)-7
                    end
                }
            }
        }
        -- Grupo de rotación y desplazamiento
        -- Grupo de bits
        -- Grupo de salto
        -- Grupo de llamada y retorno
        -- Grupo de E/S
    }
}
