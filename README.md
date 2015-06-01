# s2s

## Example

Considering the file _sample1.s2s_:

    {a
        |echo "This is a"
        {c
            |echo "This is c"
        }
        {b
            |echo "This is b"
            ?c
        }
        ?b
    }
    ?a

The command :
    
    cat sample1.s2s | s2s 


will display :

    echo "This is a"
    echo "This is b"
    echo "This is c"
