Tema 1 APD - Bitire Catalin-Constantin 335CC

Implementare Task 1:

Pentru impartirea workload-ului pe threaduri am ales sa impart inaltimea imaginii de iesire (input.height/resize_factor)
in bucati egale pentru fiecare thread. (Ex: imagine de 400px pe 4 threaduri - fiecare thread ia 100 randuri de 100px
T0: [0-99]
T1: [100-199]
T2: [200-299]
T3: [300-399]

Pentru calculele efective a valorilor pixelilor, am ales sa parcurg matricea de iesire si in functie de pixelul curent
sa gasesc in matricea de intrare pozitiile corespunzatoare acestuia (cei resize_factor^2 pixeli).

Fac diferentierea intre tipurile alb-negru si color prin faptul ca matricile pentru color au structura
Rand0: (r g b) (r g b) (r g b) ...
Rand1: (r g b) (r g b) (r g b) ...

iar pentru cele alb-negru au structura:
Rand0: pixel0 pixel1 pixel2 ...
Rand1: pixel0 pixel1 pixel2 ...

astfel parcurgerile pentru color sunt separate si diferite (j+=3).

In functia resize eliberez memoria pentru matricea de input de care nu mai e nevoie, iar in functia de file output
eliberez memoria pentru matricea de output. Aceste eliberari de memorie costa aproximativ 12% scalability score.


Implementare Task 2:

Pentru impartirea workload-ului pe threaduri am ales sa impart inaltimea imaginii de iesire (input.height/resize_factor)
in bucati egale pentru fiecare thread. (Ex: imagine de 400px pe 4 threaduri - fiecare thread ia 100 randuri de 100px
T0: [0-99]
T1: [100-199]
T2: [200-299]
T3: [300-399]

Pentru fiecare pixel(x, y) este calculata distanta matematica pana la dreapta de ecuatie data, distanta rezultata este
scalata la rezolutia imaginii pentru a se pastra 'spatiul de 100x100cm' si in functie de aceasta distanta (daca este mai
mica ca 3) se stabileste daca pixelul trebuie setat la valoarea 0 (negru).

In functia de file output eliberez memoria pentru matricea de output.