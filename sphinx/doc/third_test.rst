my third test -- code
---------------------

.. highlight:: none
    :linenothreshold: 1

this is normal word, but below words will be code style ::

    these words only need beginning with blanks and jumping one blank line.

    these words also can be multiline

here is some c code

.. code-block:: c
    :emphasize-lines: 5

    #include <iostream>

    int main()
    {
        std::cout<<"hello world"<<std::endl;
        return 1;
    }

here is c code from file

.. literalinclude:: hello.cpp
    :language: c
    :emphasize-lines: 5





