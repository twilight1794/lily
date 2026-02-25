-- Especificación de la arquitectura Little Computer 3
-- a como fue definida en https://en.wikipedia.org/wiki/Little_Computer_3
-- y https://www.cs.utexas.edu/~fussell/courses/cs310h/lectures/Lecture_10-310h.pdf
-- Tiene registros de 16 bits, direcciones de 16 bits, opcodes de 4 bits, instrucciones de 16 bits

-- Devuelve el valor numérico de un registro rx
local function obt_r(r)
    return tonumber(r:sub(2, 3))
end

return {
    id = "lc3",
    registros = {
        -- Registros de propósito general
        R0 = {
            tamano = 16,
            desplazamiento = 0,
        },
        R1 = {
            tamano = 16,
            desplazamiento = 16,
        },
        R2 = {
            tamano = 16,
            desplazamiento = 32,
        },
        R3 = {
            tamano = 16,
            desplazamiento = 48,
        },
        R4 = {
            tamano = 16,
            desplazamiento = 64,
        },
        R5 = {
            tamano = 16,
            desplazamiento = 80,
        },
        R6 = {
            tamano = 16,
            desplazamiento = 96,
        },
        R7 = {
            tamano = 16,
            desplazamiento = 112,
        },
        -- Otros registros
        PC = {
            tamano = 16,
            desplazamiento = 128
        },
        NF = {
            tamano = 1,
            desplazamiento = 144
        },
        ZF = {
            tamano = 1,
            desplazamiento = 145
        },
        PF = {
            tamano = 1,
            desplazamiento = 146
        }
    },
    tipos = {
        r = function(v)
            return type(v) == "string" and v:sub(1,2) == "r" and (tonumber(v:sub(2,3)) >= 0 and tonumber(v:sub(2,3)) < 8)
        end,
        uint5 = function(v) return type(v) == "number" and v >= 0 and v < 31 end,
        dir9 = function(v) return type(v) == "number" and v >= -256 and v < 256 end,
        ["*ind9"] = function(desp) return #desp == 1 and type(desp[1]) == "number" and desp[1] >= -256 and desp[1] < 256 end,
        ["*ind6"] = function(desp) return #desp == 2 and type(desp[1]) == "string" and desp[1]:sub(1,2) == "r" and (tonumber(desp[1]:sub(2,3)) >= 0 and tonumber(desp[1]:sub(2,3)) < 8) and type(desp[2]) == "number" and desp[2] >= -32 and desp[2] < 32 end,
    },
    ensamble = {
          -- Instrucciones de operación
          ADD = {
              {
                  { "r", "r", "r" },
                  function(rd, r1, r2)
                      return {
                          0x10 | (obt_r(rd) << 1) | ((obt_r(r1) & 4) >> 2),
                          ((obt_r(r1) & 3) << 6) | obt_r(r2)
                      }
                  end
              },
              {
                  { "r", "r", "uint5" },
                  function (rd, r1, n)
                      return {
                          0x10 | (obt_r(rd) << 1) | ((obt_r(r1) & 4) >> 2),
                          0x20 | ((obt_r(r1) & 3) << 6) | n
                      }
                  end
              }
          },
          AND = {
              {
                  { "r", "r", "r" },
                  { "ADD", function(bytes) return { bytes[1] + 0x40, bytes[2] } end }
              },
              {
                  { "r", "r", "uint5" },
                  { "ADD", function(bytes) return { bytes[1] + 0x40, bytes[2] } end }
              }
          },
          NOT = {
              {
                  { "r", "r" },
                  function(r1, r2)
                      return {
                          0x90 | (obt_r(r1) << 1) | ((obt_r(r2) & 4) >> 2),
                          0x3F | ((obt_r(r2) & 3) << 6)
                      }
                  end,
              }
          },
          -- Instrucciones de movimiento
          LD = {
              {
                  { "r", "dir9" },
                  function(r, v) return { 0x20 | (obt_r(r) << 1) | ((v & 0x100) >> 8), v & 0xFF } end
              },
              {
                  { "r", "ind9" },
                  function(r, v) return { 0xA0 | (obt_r(r) << 1) | ((v[1] & 0x100) >> 8), v[1] & 0xFF } end
              },
              {
                  { "r", "ind6" },
                  function(r, v) return { 0x60 | obt_r(r) << 1 |((obt_r(v[1]) & 4) >> 2), ((obt_r(v[1]) & 3) << 6) | v[2] } end
              }
          },
          LEA = {
              {
                  { "r", "dir9" },
                  function(r, v) return { 0x70 | (obt_r(r) << 1) | ((v & 0x100) >> 8), v & 0xFF } end
              }
          },
          ST = {
              {
                  { "r", "dir9" },
                  { "LD", function(bytes) return { bytes[1] + 0x10, bytes[2] } end }
              },
              {
                  { "r", "ind9" },
                  { "LD", function(bytes) return { bytes[1] + 0x10, bytes[2] } end }
              },
              {
                  { "r", "ind6" },
                  { "LD", function(bytes) return { bytes[1] + 0x10, bytes[2] } end }
              }
          },
          -- Instrucciones de control
          BR = {
              {
                  { "uint3", "dir9" },
                  function (f, v) return { (f << 1) | ((v & 0x100) >> 8), v & 0xFF } end
              }
          },
          JMP = {
              {
                  { "r" },
                  function (r) return { 0xC0 | ((obt_r(r) & 4) >> 2), ((obt_r(r) & 3) << 6) } end
              }
          },
          JSR = nil,
          JSRR = nil,
          RET = nil,
          TRAP = {
              {
                  { "uint8" },
                  function (v) return { 0xF0, v } end
              }
        }
    },
    opcodes = {
    },
    desensamble = {
    }
}