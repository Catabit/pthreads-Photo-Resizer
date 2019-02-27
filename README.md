Tema 1 APD - Bitire Catalin-Constantin 335CC

Enunt: "Implementarea unui program capabil sa redimensioneze o imagine micsorand pierderea de informatie folosind
tehnica anti-aliasing de tip super sampling antialiasing (SSAA sau FSAA).
Acest program va fi implementat obligatoriu folosind thread-uri prin API-ul oferit de libraria pthread,
ın limbajul C. Rezolvarea va trebui sa scaleze cu numarul de thread-uri, ramanand intr-un interval de eficienta
acceptat (determinat empiric prin limitarea timpului de executie la testare)."

Rezolvare:

Pentru impartirea workload-ului pe threaduri am ales sa impart inaltimea imaginii de iesire (input.height/resize_factor)
in bucati egale pentru fiecare thread. (Ex: imagine de 400px pe 4 threaduri - fiecare thread ia 100 randuri de 100px
T0: [0-99]
T1: [100-199]
T2: [200-299]
T3: [300-399]
)

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
