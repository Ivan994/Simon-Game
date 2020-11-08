# Simon-Game

Tema zadatka bila je realizacija igrice Simon Game za jednog igrača.
Cilj ove igre je da se pogodi redosled uključivanja LE dioda pomoću tastera
povezanih na Raspberry Pi 2 platformu.

Bilo je neophodno realizovati sledeće delove:

-Rukovalac za LE diode i tastere  (GPIO driver) koji omogućava korisničkoj
aplikaciji da proizvoljnu LE diodu uključi-isključi, odnosno preuzme vrednost
sa tastera.

-Rukovalac za generisanje pseudoslučajnih brojeva ili koristiti postojeći
generator 'random' ili 'urandom'.

-Korisničku aplikaciju koja realizuje opisanu igru. Takođe treba da prikazuje
stanja u kojima se igra nalazi.