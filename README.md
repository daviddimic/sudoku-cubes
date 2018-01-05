# RG098-sudoku-kocke
Korisnik izbegava kocke koje ga gadjaju tako sto skace. Kada ga pogodi onda resava sudoku kocku: kocka koja sa svake strane ima razlicitu postavku klasicne sudoku igre.
Na raspolaganju ima 2 pomoci: mogucnost da se u potpunosti resi jedan sudoku.
Onemoguceno je da se na neko polje unese broj ako se tu ne moze nalaziti (ako postoji isti u vrsti, koloni ili 3x3 bloku), sto znaci da ako je sudoku u potpunosti popunjen on je resen tacno.
Sudoku kocka je resena ako su svih 6 tabli popunjene, onda korisnik moze odabrati da ponovo igra.

## Screenshot

![sudoku-kocka](https://raw.githubusercontent.com/MATF-RG17/RG098-sudoku-kocke/master/Screenshots/2018-01-05.png)

## Installation

Kompajliranje sa:

`make`

Pokretanje programa sa:

`./sudoku`

## Usage

| **Key** | **Opis** |
| :---  | :--- |
| `w` `s` `a` `d` | Rotira se kocka |
| `↑` `↓` `←` `→`   | Kretanje po tekucoj tabli  |
| `+` `-` | Zumiranje |
| `1-9` | Unos broja |
| `0` | Brisanje unetog broja |
| `q` | Brisanje svih unetih brojeva na tekucoj tabli |
| `h` | Pomoc: resava celu tablu (max 2. puta moguce iskoristiti) |
| `n`  | Nova igra  |
| `space`  | Skoci |
| `ESC` | Izlaz iz programa |
