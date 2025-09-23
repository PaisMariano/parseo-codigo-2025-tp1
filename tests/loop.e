class LOOP_EXAMPLE
feature
    count_down
        local
            i: INTEGER
        do
            from
                i := 5
            until
                i = 0
            loop
                print (i.out)
                i := i - 1
            end
        end
end
