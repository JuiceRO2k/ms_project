#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm> // Pentru min_element și max_element
#include <conio.h> // Pentru detectarea tastelor
using namespace std;

const double c = 343.0; // Viteza sunetului în aer (m/s)
const double dx = 0.01; // Pas spațial (m)
const double dt = 0.00001; // Pas temporal (s)
const int nx = 100; // Număr de puncte pe axa x
const int steps = 1000; // Număr de pași de timp


void initialize(vector<double>& p) {
    for (int i = 0; i < nx; ++i) {
        double x = i * dx;
        /* 
            Funcția "initialize()" generează o undă inițială sub forma unui impuls gaussian 
            centrat la x=0.5x=0.5. Aceasta reprezintă o undă incidentă care va fi propagată. 
        */
        p[i] = exp(-100 * (x - 0.5) * (x - 0.5)); 
    }
}

void update_wave(vector<double>& p, vector<double>& p_prev, vector<double>& p_next) {
    double c2 = c * c;
    for (int i = 1; i < nx - 1; ++i) {
        /*
            Funcția "update_wave()" implementează formula discretizată a ecuației undei 
            pentru fiecare punct xixi​ din grilă, cu excepția marginilor. Se folosesc trei vectori:
            - p_prev: starea anterioară a undei
            - p: starea curentă a undei
            - p_next: starea următoare a undei
        */
        p_next[i] = 2 * p[i] - p_prev[i] + c2 * (dt * dt / (dx * dx)) * (p[i + 1] - 2 * p[i] + p[i - 1]);
    }
}

void print_wave(const vector<double>& p) {
    /*
        Funcția "print_wave()" creează o reprezentare grafică simplă a undei 
        curente folosind caractere * pentru valori apropiate de un nivel discret.
    */

    const int height = 20; // Înălțimea graficului în terminal
    double max_value = *max_element(p.begin(), p.end());
    double min_value = *min_element(p.begin(), p.end());
    double scale = height / (max_value - min_value);

    for (int h = height; h >= 0; --h) {
        for (int i = 0; i < nx; ++i) {
            double value = scale * (p[i] - min_value);
            if (round(value) == h) {
                cout << "*";
            } else {
                cout << " ";
            }
        }
        cout << "\n";
    }
    cout << string(nx, '-') << "\n"; // Linie de bază
}

int main() {
    vector<double> p(nx, 0.0), p_prev(nx, 0.0), p_next(nx, 0.0);
    vector<vector<double>> history;

    // Inițializăm unda inițială
    initialize(p);
    p_prev = p;

    // Propagăm unda pentru un număr de pași de timp
    for (int step = 0; step < steps; ++step) {
        update_wave(p, p_prev, p_next);
        p_prev = p;
        p = p_next;

        if (step % 100 == 0) {
            history.push_back(p); // Salvăm graficul curent
        }
    }

    int current_index = 0;
    char key;
    bool running = true;

    // Rulăm o buclă pentru a afișa graficele salvate (sub forma unui meniu cu sageti <-/->/ESC)
    while (running) {
        system("cls"); // Curățăm ecranul
        cout << "Graficul " << current_index + 1 << " din " << history.size() << ":\n" << "(Timp: " << current_index * 100 * dt << "s)\n\n\n";
        print_wave(history[current_index]);
        cout << "[  <- Anterior | Urmatorul -> | ESC: TERMINARE PROGRAM  ]\n";

        key = _getch();
        if (key == 27) { // Esc pentru a ieși
            running = false;
        } else if (key == 75 && current_index > 0) { // Sageata Stanga
            current_index--;
        } else if (key == 77 && current_index < history.size() - 1) { // Sageata Dreapta
            current_index++;
        }
    }

    return 0;
}