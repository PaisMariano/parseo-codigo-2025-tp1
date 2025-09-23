class CONDITIONAL_EXAMPLE
feature
    check_number (n: INTEGER)
        do
            if n > 0 then
                print ("positivo%N")
            else
                print ("no positivo%N")
            end
        end
end
