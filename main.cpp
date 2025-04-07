#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "include/raylib.h"

using namespace std;

void inizializza(const string &segreta, string &attuale) {
    attuale = string(segreta.size(), '_');
}

bool controlla(const string &segreta, char lettera, string &attuale) {
    bool trovata = false;
    for (int i = 0; i < segreta.size(); ++i) {
        if (segreta[i] == lettera) {
            attuale[i] = lettera;
            trovata = true;
        }
    }
    return trovata;
}

void incrementa_errori(int &conta_errori, const string &segreta, char lettera, string &attuale) {
    if (!controlla(segreta, lettera, attuale)) {
        conta_errori++;
    }
}

void disegna(const Texture2D &mostro, int errori) {
    DrawTexture(mostro, 0, 0, WHITE);
    int w = mostro.width;
    int h = mostro.height;
    for (int i = errori; i < 9; ++i) {
        DrawRectangle((i % 3) * (w / 3), (i / 3) * (h / 3), w / 3, h / 3, WHITE);
    }
}

vector<string> carica(const string &nomefile) {
    vector<string> parole;
    string parola;
    ifstream in(nomefile);
    if (in) {
        while (in >> parola) {
            parole.push_back(parola);
        }
    }
    return parole;
}

int main(void) {
    const int MAX_ERRORI = 9;
    int conta_errori = 0;
    string segreta, provate, attuale;
    vector<string> parole;

    srand(time(NULL)); // inizializzazione del seme random

    parole = carica("../assets/parole.txt");
    if (parole.empty()) {
        cout << "Errore nell'apertura del file" << endl;
        return 1;
    }

    segreta = parole[rand() % parole.size()];
    inizializza(segreta, attuale);

    const int screenWidth = 1000;
    const int screenHeight = 650;

    InitWindow(screenWidth, screenHeight, "Attento al mostro");
    SetTargetFPS(60);
    Texture2D mostro = LoadTexture("john_china.jpg");
    mostro.width = 950;
    mostro.height = 400;
    int stato;
    const int GIOCO = 1;
    const int FINALE = 2;
    stato = GIOCO;

    while (!WindowShouldClose()) {
        if (stato == GIOCO) {
            char lettera = GetCharPressed();
            if (lettera >= 'a' && lettera <= 'z') {
                cout << "Hai premuto: " << lettera << endl;
                if (provate.find(lettera) == string::npos) {
                    provate += lettera;
                    incrementa_errori(conta_errori, segreta, lettera, attuale);
                }
            }

            if (conta_errori >= MAX_ERRORI || segreta == attuale) {
                stato = FINALE;
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);
            disegna(mostro, conta_errori);
            DrawText(attuale.c_str(), 20, 480, 60, BLACK);
            DrawText("Numero di errori:", 20, 400, 45, BLACK);
            DrawText(to_string(conta_errori).c_str(), 440, 400, 45, BLACK);
            EndDrawing();
        } else if (stato == FINALE) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            if (conta_errori >= MAX_ERRORI) {
                DrawText("Hai perso!", 100, 200, 50, RED);
                DrawText(("La parola era: " + segreta).c_str(), 100, 300, 40, BLACK);
            } else {
                DrawText("Hai vinto!", 100, 200, 50, DARKGREEN);
            }
            EndDrawing();
        }
    }

    UnloadTexture(mostro);
    CloseWindow();
    return 0;
}
