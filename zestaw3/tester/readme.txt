Kompilacja:                         make run
Uruchomienie netcat'a:              nc -v -u 0.0.0.0 2020

Pozbycie się plików wykonawczych:   make clean

Uruchomiony serwer po podłączeniu nowego klienta
otrzymuje 5-krotnie wiadomość zawierającą jeden
znak i automatycznie odsyła komunikat. Po po 
uruchomieniu najpierw netcat'a a dopiero później
serwera problem nie występuje i komunikacja odbywa
się bez problemu.
Po usunięciu flagi -v, problem nie występuje. 

Serwer będzie nasłuchiwał klientów do momentu przerwania
jego działania za pomocą sygnału wysyłanego "CTRL+C".

Dodatkowo do automatyzacji procesu testowania znajduje się
równiez skrypt "test_for_sum.py" który uruchomi wszystkie 
testy w folderze ./test o nazwie "test-dane[liczba].txt"
który porówna wynik otrzymany z wynikiem oczekiwanym 
znajdującym się w pliku "test-wynik[liczba].txt".

Program musi znajdować się w tym samym folderze co folder
zawierający testowe pliki.

Wystarczy go uruchomić komendą: 
    
    python3 test_for_sum.py