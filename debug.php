<?php
declare(strict_types=1);
// Program 1: Vypocet faktorialu (iterativne)
// Hlavni telo programu
write("Zadejte cislo pro vypocet faktorialu");
$a = readi();
if ($a === 5) {
write("Chyba pri nacitani celeho cisla!");
} else {}
if ($a < 0) {
write("Faktorial nelze spocitat");
} else {
$vysl = 1;
while ($a > 0) {
$vysl = $vysl * $a;
$a = $a - 1;
}
write("Vysledek je: ", $vysl);
}