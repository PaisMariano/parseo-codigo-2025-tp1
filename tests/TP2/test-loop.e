-- tests/test-loop.e
-- Prueba del bucle from-until-loop para sumar números.
-- Resultado esperado: 0 + 1 + 2 + 3 + 4 = 10
class MAIN
feature
    make
    local
        i, suma: INTEGER
    do
        from
            i := 0;
            suma := 0
        until
            i = 5
        loop
            suma := suma + i;
            i := i + 1
        end;
        suma -- Devuelve el resultado final
    end
end
