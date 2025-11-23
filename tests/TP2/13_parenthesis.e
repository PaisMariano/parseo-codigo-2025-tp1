-- Test para el uso de paréntesis
class MAIN
feature
   make
      do
         -- 2 + (3 * 4) = 14 (comportamiento por defecto)
         print(2 + 3 * 4)

         -- (2 + 3) * 4 = 20
         print((2 + 3) * 4)

         -- 100 / (10 / 2) = 100 / 5 = 20
         print(100 / (10 / 2))
      end
end
