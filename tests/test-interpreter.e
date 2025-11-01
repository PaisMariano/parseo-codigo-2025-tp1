-- test.e
class MAIN
feature
    make
    do
        10 + 5 * 2 -- El parser lo convertirá en una expresión
    end
end
