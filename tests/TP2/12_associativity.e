-- Test para asociatividad de operadores
class MAIN
feature
   make
      do
         -- (100 - 50) + 10 = 60
         print(100 - 50 + 10)

         -- (100 / 10) / 2 = 5
         print(100 / 10 / 2)
      end
end
