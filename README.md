# 3x3 Tic-Tac-Toe
A játék függvényeit a TicTacToe.h-ben deklaráltam és TicTacToe.c-ben implementáltam. A kártya beépített függvényei közül a drawing, display, és a touch.h-t függvényeit használtam.

 A program előszőr a pálya kirajzolásával kezdődik, ez a DrawField függvény segítségével történik. A képernyőre a zöld kört a DrawCircle függvénnyel, a piros X-et a DrawLine függvénnyel rajzoljuk ki. 
*	A kör kezd. Minden egyes érintés után játékost vált a program. Ez addig zajlik, amíg az összes rublika be nem telik, vagy az egyik játékos nyer, vagy már csak döntetlen lehet a végeredmény. Ezek után a program a GameResult program segítségével eldönti, hogy melyik játékos nyer, majd a GameOver függvény kiírja a végeredményt a képernyő közepére, miután a program előtte letörölte a pályát. 4 másodperc elteltével elkezdődk az újabb játék a változók alapértékbe állítása után. 

A pályát 9 cellára osztottam fel *(0-8)*, aminek a középponti pixel-jét tároltam el, és ez alapján rajzoltam rublikába az X-et vagy kört. Annak eldöntése, hogy valaki nyert-e, minden egyes érintés után ellenőrzésre kerül. Ez úgy történik, hogy egy függvény minden egyes sorban, oszlopban és átlóban összeadja a cellában található szimbólum értékét *(1 – kör, 2 – X, 0- üres)*, majd ha az egyik játékos nyer, akkor visszatér 1-el, vagy 2-el, aszerint, hogy melyik. 
* A játék már az utolsó előtti lépésnél képes eldönteni a végkimenetelt.
