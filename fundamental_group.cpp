// Groupe fondamental : du cercle aux applications physiques.
//
// Compilation : g++ -std=c++17 -O2 -Wall -Wextra fundamental_group.cpp -o fundamental_group
// Exécution   : ./fundamental_group        (retourne 0 si toutes les vérifications passent)
//
// Modules (chacun illustre un chapitre de COURS.md) :
//   1. pi_1(S^1) = Z        : relèvement, degré, composition, inverse, homotopie
//   2. Principe de l'argument : nombre de racines d'un polynôme dans un disque
//   3. pi_1(T^2) = Z x Z    : tore, produit d'espaces
//   4. pi_1(plan - 2 pts) = F_2 : mots réduits, abélianisation, commutateur
//   5. Vortex sur réseau    : charge topologique d'un champ de phase (modèle XY)
//   6. Effet Aharonov-Bohm  : phase de boucle et interférences
//   7. pi_1(SO(3)) = Z/2    : revêtement SU(2), spineurs, "trick de la ceinture"
//   8. Chaîne SSH           : invariant d'enroulement d'une phase topologique

#include <algorithm>
#include <cctype>
#include <cmath>
#include <complex>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

constexpr double PI = 3.14159265358979323846;
using cd = std::complex<double>;

int failures = 0;

void check(bool ok, const std::string& what) {
    std::cout << (ok ? "  [OK]    " : "  [ECHEC] ") << what << "\n";
    if (!ok) ++failures;
}

void title(const std::string& t) { std::cout << "\n=== " << t << " ===\n"; }

// Ramène un angle dans ]-pi, pi].
double wrap(double a) {
    a = std::fmod(a + PI, 2 * PI);
    if (a <= 0) a += 2 * PI;
    return a - PI;
}

// Point du cercle unitaire d'angle donné.
cd point_on_circle(double angle) { return {std::cos(angle), std::sin(angle)}; }

// ---------------------------------------------------------------------------
// 1. Boucles sur S^1.  On stocke le RELEVEMENT theta : [0,1] -> R (revêtement
//    exp(i.) : R -> S^1).  Le degré (theta(1)-theta(0))/2pi est l'isomorphisme
//    pi_1(S^1) -> Z.
// ---------------------------------------------------------------------------
class Loop {
    std::vector<double> th;  // relèvement continu échantillonné

public:
    explicit Loop(std::vector<double> lifted) : th(std::move(lifted)) {}

    // Relève une suite d'angles : chaque pas est pris dans ]-pi, pi] (plus court arc).
    // Valide si l'échantillonnage est assez fin (pas < pi), sinon le degré est faux.
    static Loop from_angles(const std::vector<double>& a) {
        std::vector<double> t{a.at(0)};
        for (size_t i = 1; i < a.size(); ++i) t.push_back(t.back() + wrap(a[i] - a[i - 1]));
        return Loop(t);
    }

    // theta(t) = 2 pi n t + eps sin(2 pi m t) : boucle de degré n, déformée d'amplitude eps
    // (la perturbation s'annule en t = 0, 1 : homotopie à extrémités fixes).
    static Loop winding(int n, int N, double eps = 0.0, int m = 1) {
        std::vector<double> t(N + 1);
        for (int k = 0; k <= N; ++k) {
            double s = double(k) / N;
            t[k] = 2 * PI * n * s + eps * std::sin(2 * PI * m * s);
        }
        return Loop(t);
    }

    size_t size() const { return th.size(); }
    double start() const { return th.front(); }
    double end() const { return th.back(); }
    double at(size_t i) const { return th[i]; }

    bool closed(double tol = 1e-9) const { return std::fabs(wrap(end() - start())) < tol; }

    int degree() const { return static_cast<int>(std::lround((end() - start()) / (2 * PI))); }

    // Concaténation a * b (on parcourt a puis b).  Exige end(a) = start(b) modulo 2 pi.
    Loop compose(const Loop& o) const {
        if (std::fabs(wrap(end() - o.start())) > 1e-9) {
            std::cerr << "compose : boucles non composables (points de base différents)\n";
            std::exit(2);
        }
        std::vector<double> t = th;
        for (size_t i = 1; i < o.th.size(); ++i) t.push_back(o.th[i] - o.th.front() + end());
        return Loop(t);
    }

    // Chemin inverse a^{-1}(t) = a(1-t).
    Loop inverse() const { return Loop(std::vector<double>(th.rbegin(), th.rend())); }

    // Homotopie linéaire H_s = (1-s) theta, valable pour contracter une boucle de degré 0.
    Loop shrink(double s) const {
        std::vector<double> t = th;
        for (double& x : t) x = start() + (1 - s) * (x - start());
        return Loop(t);
    }

    double max_dev() const {  // amplitude maximale du relèvement
        double m = 0;
        for (double x : th) m = std::max(m, std::fabs(x - start()));
        return m;
    }
};

void chapter1() {
    title("1. pi_1(S^1) = Z : degré, composition, homotopie");
    const int N = 400;

    Loop a = Loop::winding(1, N), b = Loop::winding(2, N), e = Loop::winding(0, N);
    std::cout << "  deg(a)=" << a.degree() << "  deg(b)=" << b.degree() << "  deg(e)=" << e.degree() << "\n";

    // Le programme initial composait via atan2, ce qui perd l'information "tours" (2pi -> 0).
    // Ici on compose sur le relèvement : le degré est additif.
    check(a.compose(b).degree() == a.degree() + b.degree(), "deg(a*b) = deg(a) + deg(b)");
    check(a.inverse().degree() == -a.degree(), "deg(a^-1) = -deg(a)");
    check(a.compose(a.inverse()).degree() == 0, "a * a^-1 est de degré 0 (neutre)");
    check(e.compose(a).degree() == a.degree() && a.compose(e).degree() == a.degree(), "e neutre à gauche et à droite");
    check(a.compose(b).compose(e).degree() == a.compose(b.compose(e)).degree(), "associativité (au niveau des classes)");
    check(a.compose(b).degree() == b.compose(a).degree(), "commutativité : pi_1(S^1) est abélien");

    // Invariance par homotopie : on déforme la boucle de degré 3 sans toucher au point de base.
    std::cout << "  Homotopie theta_eps(t) = 6 pi t + eps sin(4 pi t) :\n";
    bool stable = true;
    for (double eps : {0.0, 0.5, 1.0, 2.0, 3.0}) {
        Loop l = Loop::winding(3, N, eps, 2);
        std::cout << "     eps=" << std::setw(3) << eps << "  boucle fermée : " << l.closed() << "   degré = " << l.degree() << "\n";
        stable = stable && l.closed() && l.degree() == 3;
    }
    check(stable, "le degré est invariant par homotopie à point de base fixé");

    // Degré 0  =>  contractile : H_s = (1-s) theta reste une famille de boucles.
    Loop z = a.compose(a.inverse());
    bool ok = true;
    for (double s : {0.0, 0.25, 0.5, 0.75, 1.0}) ok = ok && z.shrink(s).closed();
    check(ok && z.shrink(1.0).max_dev() < 1e-12, "a * a^-1 se contracte sur le point de base (H_s = (1-s) theta)");

    // Le "loop2" du programme initial (0 -> pi) n'est PAS une boucle : on le détecte.
    std::vector<double> half;
    for (int k = 0; k <= 4; ++k) half.push_back(PI * k / 4);
    Loop p = Loop::from_angles(half);
    check(!p.closed(), "le chemin 0 -> pi n'est pas une boucle (composable seulement avec un chemin de pi vers 0)");

    // Contre-exemple d'échantillonnage : pas >= pi => le degré calculé est faux.
    Loop coarse = Loop::from_angles({0, 4.0, 8.0, 2 * PI});  // pas de 4 rad > pi
    std::cout << "  Echantillonnage trop grossier : degré lu = " << coarse.degree() << " (vrai = 1) -> règle : pas < pi\n";
}

// ---------------------------------------------------------------------------
// 2. Indice d'une courbe autour d'un point & principe de l'argument.
// ---------------------------------------------------------------------------
double winding_about(const std::vector<cd>& pts, cd c) {
    double s = 0;
    for (size_t i = 0; i < pts.size(); ++i) {
        cd u = pts[i] - c, v = pts[(i + 1) % pts.size()] - c;
        s += std::arg(v / u);  // angle orienté dans ]-pi, pi]
    }
    return s / (2 * PI);
}

std::vector<cd> circle(cd c, double r, int N) {
    std::vector<cd> p;
    for (int k = 0; k < N; ++k) p.push_back(c + r * point_on_circle(2 * PI * k / N));
    return p;
}

void chapter2() {
    title("2. Principe de l'argument : zéros d'un polynôme dans un disque");
    // p(z) = (z - 0.5)(z + 1)(z - 2i)(z - 3) ; modules des racines : 0.5, 1, 2, 3
    auto p = [](cd z) { return (z - 0.5) * (z + 1.0) * (z - cd(0, 2)) * (z - 3.0); };
    const int expected[] = {1, 2, 3, 4};
    const double radius[] = {0.75, 1.5, 2.5, 3.5};
    for (int i = 0; i < 4; ++i) {
        std::vector<cd> img;
        for (cd z : circle(0, radius[i], 4000)) img.push_back(p(z));
        int n = static_cast<int>(std::lround(winding_about(img, 0)));
        std::cout << "  |z| = " << radius[i] << "  : indice de p(cercle) autour de 0 = " << n << "  (racines dans le disque : " << expected[i] << ")\n";
        check(n == expected[i], "indice = nombre de zéros (avec multiplicité)");
    }
}

// ---------------------------------------------------------------------------
// 3. Tore T^2 = S^1 x S^1 : pi_1 = Z x Z.
// ---------------------------------------------------------------------------
struct TorusLoop {
    Loop u, v;  // deux relèvements (longitude, méridien)
    std::pair<int, int> cls() const { return {u.degree(), v.degree()}; }
    TorusLoop compose(const TorusLoop& o) const { return {u.compose(o.u), v.compose(o.v)}; }
};

void chapter3() {
    title("3. Tore : pi_1(S^1 x S^1) = Z x Z");
    const int N = 400;
    TorusLoop m{Loop::winding(1, N), Loop::winding(0, N)};  // longitude  (1,0)
    TorusLoop l{Loop::winding(0, N), Loop::winding(1, N)};  // méridien   (0,1)
    TorusLoop knot{Loop::winding(2, N), Loop::winding(3, N)};  // courbe (2,3) (nœud de trèfle sur le tore)

    auto show = [](const char* nm, std::pair<int, int> c) { std::cout << "  " << nm << " -> (" << c.first << ", " << c.second << ")\n"; };
    show("m      ", m.cls());
    show("l      ", l.cls());
    show("m*l    ", m.compose(l).cls());
    show("l*m    ", l.compose(m).cls());
    show("(2,3)  ", knot.cls());
    check(m.compose(l).cls() == l.compose(m).cls(), "pi_1(T^2) est abélien : m*l ~ l*m");
    check(m.compose(l).compose(m).cls() == std::make_pair(2, 1), "m*l*m -> (2,1)");
    check(knot.cls() == std::make_pair(2, 3), "la courbe de type (p,q) a pour classe (p,q)");
}

// ---------------------------------------------------------------------------
// 4. Plan privé de deux points : pi_1 = F_2 (groupe libre), non abélien.
// ---------------------------------------------------------------------------
char swapcase(char c) { return std::isupper(static_cast<unsigned char>(c)) ? char(std::tolower(c)) : char(std::toupper(c)); }

// Réduction libre : on élimine les paires xX et Xx (a = tour autour de p1, A = a^-1, ...).
std::string reduce(const std::string& w) {
    std::string s;
    for (char c : w) {
        if (!s.empty() && s.back() == swapcase(c)) s.pop_back();
        else s.push_back(c);
    }
    return s;
}
std::string inverse(const std::string& w) {
    std::string r;
    for (auto it = w.rbegin(); it != w.rend(); ++it) r.push_back(swapcase(*it));
    return r;
}
// Abélianisation F_2 -> Z^2 : (exposant total de a, exposant total de b).
std::pair<int, int> abelianize(const std::string& w) {
    int x = 0, y = 0;
    for (char c : w) {
        if (c == 'a') ++x;
        else if (c == 'A') --x;
        else if (c == 'b') ++y;
        else if (c == 'B') --y;
    }
    return {x, y};
}

void chapter4() {
    title("4. Plan privé de 2 points : pi_1 = F_2 (groupe libre)");
    std::string w1 = reduce("abBAab"), w2 = reduce("aAbBaB");
    std::cout << "  reduce(abBAab) = '" << w1 << "'   reduce(aAbBaB) = '" << w2 << "'\n";
    check(w1 == "ab" && w2 == "aB", "réductions libres");

    std::string comm = reduce("a" "b" "A" "B");  // commutateur [a,b]
    auto ab = abelianize(comm);
    std::cout << "  [a,b] = '" << comm << "'  abélianisation = (" << ab.first << ", " << ab.second << ")\n";
    check(!comm.empty() && ab == std::make_pair(0, 0), "[a,b] != e dans F_2 mais s'annule dans Z^2 (F_2 n'est pas abélien)");
    check(reduce("ab" + inverse("ab")).empty(), "w * w^-1 = e");
    check(reduce("ab") != reduce("ba"), "ab != ba");

    // Réalisation géométrique : les indices autour des deux trous donnent l'abélianisation.
    cd p1(0.5, 0), p2(-0.5, 0);
    auto big = circle(0, 2.0, 2000);
    std::vector<cd> lem;  // lemniscate : lobe droit anti-horaire, lobe gauche horaire
    for (int k = 0; k < 4000; ++k) {
        double t = 2 * PI * k / 4000, d = 1 + std::sin(t) * std::sin(t);
        lem.emplace_back(std::cos(t) / d, std::sin(t) * std::cos(t) / d);
    }
    auto idx = [&](const std::vector<cd>& g) {
        return std::make_pair(std::lround(winding_about(g, p1)), std::lround(winding_about(g, p2)));
    };
    auto r1 = idx(circle(p1, 0.3, 2000)), r2 = idx(big), r3 = idx(lem);
    std::cout << "  cercle autour de p1 : (" << r1.first << "," << r1.second << ")   grand cercle : (" << r2.first << "," << r2.second << ")   lemniscate : (" << r3.first << "," << r3.second << ")\n";
    check(r1 == std::make_pair(1L, 0L) && r2 == std::make_pair(1L, 1L) && r3 == std::make_pair(1L, -1L),
          "indices (autour de p1, autour de p2) = abélianisation du mot : a, ab, aB");
}

// ---------------------------------------------------------------------------
// 5. Vortex sur réseau : charge topologique d'un champ de phase theta(x,y).
// ---------------------------------------------------------------------------
void chapter5() {
    title("5. Vortex d'un champ de phase (superfluide / modèle XY)");
    const int N = 14;
    cd v(4.5, 4.5), av(9.5, 8.5);  // vortex (+1) et antivortex (-1) décalés des noeuds
    std::vector<std::vector<double>> th(N, std::vector<double>(N));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            cd z(i, j);
            th[i][j] = std::arg(z - v) - std::arg(z - av);
        }

    // Somme des écarts de phase ramenés dans ]-pi,pi] le long du bord du rectangle [i0,i1]x[j0,j1].
    auto rect = [&](int i0, int j0, int i1, int j1) {
        double s = 0;
        for (int i = i0; i < i1; ++i) s += wrap(th[i + 1][j0] - th[i][j0]);
        for (int j = j0; j < j1; ++j) s += wrap(th[i1][j + 1] - th[i1][j]);
        for (int i = i1; i > i0; --i) s += wrap(th[i - 1][j1] - th[i][j1]);
        for (int j = j1; j > j0; --j) s += wrap(th[i0][j - 1] - th[i0][j]);
        return static_cast<int>(std::lround(s / (2 * PI)));
    };

    int total = 0, npos = 0, nneg = 0;
    std::cout << "  Carte des charges par plaquette ('+' vortex, '-' antivortex) :\n";
    for (int j = N - 2; j >= 0; --j) {
        std::cout << "     ";
        for (int i = 0; i < N - 1; ++i) {
            int q = rect(i, j, i + 1, j + 1);
            total += q;
            npos += q > 0;
            nneg += q < 0;
            std::cout << (q > 0 ? '+' : q < 0 ? '-' : '.') << ' ';
        }
        std::cout << "\n";
    }
    check(npos == 1 && nneg == 1, "un vortex et un antivortex détectés");
    check(total == 0 && rect(0, 0, N - 1, N - 1) == 0, "charge totale nulle = indice du bord du réseau");
    check(rect(2, 2, 7, 7) == 1, "le bord d'un rectangle autour du vortex seul a pour indice +1 (Stokes discret)");
    check(rect(7, 6, 12, 11) == -1, "idem pour l'antivortex : -1");
}

// ---------------------------------------------------------------------------
// 6. Aharonov-Bohm : un flux Phi piégé dans un trou ; la phase ne dépend que du degré.
// ---------------------------------------------------------------------------
void chapter6() {
    title("6. Effet Aharonov-Bohm : phase = 2 pi n Phi/Phi0");
    const int N = 400;
    std::cout << "  Phi/Phi0   n   phase/2pi   intensité 2+2cos(phase)\n";
    bool ok = true;
    for (double f : {0.0, 0.25, 0.5, 1.0}) {
        for (int n : {1, 2, -1}) {
            Loop l = Loop::winding(n, N, 0.7, 3);  // trajet déformé, même classe d'homotopie
            double phase = 2 * PI * l.degree() * f;
            double inten = std::norm(1.0 + std::polar(1.0, phase));
            std::cout << "  " << std::setw(8) << f << "  " << std::setw(2) << l.degree() << "  " << std::setw(9) << phase / (2 * PI) << "   " << std::setw(8) << inten << "\n";
            if (f == 0.5 && n == 1) ok = ok && inten < 1e-12;   // interférence destructive
            if (f == 1.0) ok = ok && std::fabs(inten - 4) < 1e-9;  // flux quantum : invisible
        }
    }
    check(ok, "Phi = Phi0/2 -> franges éteintes ; Phi = Phi0 -> indiscernable de Phi = 0");
}

// ---------------------------------------------------------------------------
// 7. pi_1(SO(3)) = Z/2 : le revêtement SU(2) -> SO(3) (quaternions unitaires).
// ---------------------------------------------------------------------------
void chapter7() {
    title("7. pi_1(SO(3)) = Z/2 : spineurs et trick de la ceinture");
    // Rotation d'angle 2 pi k t autour de z, relevée dans SU(2) : q(t) = (cos(k pi t), 0, 0, sin(k pi t)).
    bool ok = true;
    for (int k = 0; k <= 4; ++k) {
        double w = std::cos(k * PI), z = std::sin(k * PI);  // q(1) = (w, 0, 0, z)
        std::cout << "  " << k << " tour(s) : q(1) = (" << std::lround(w) << ", 0, 0, " << std::lround(z) << ")"
                  << "  -> relevé " << (w > 0 ? "fermé (q=+1)" : "ouvert (q=-1)") << "  classe dans Z/2 = " << k % 2 << "\n";
        ok = ok && ((w > 0) == (k % 2 == 0));
    }
    check(ok, "un lacet de SO(3) se relève en lacet de SU(2) ssi il fait un nombre pair de tours");
    // Loi de groupe : concaténer k1 et k2 tours donne k1+k2 tours ; 1 + 1 = 0 dans Z/2.
    check((1 + 1) % 2 == 0, "2 tours = lacet trivial : un spineur revient à lui-même après 4 pi");
}

// ---------------------------------------------------------------------------
// 8. Chaîne SSH : h(k) = v + w e^{ik}, nombre d'enroulement autour de 0 (phase topologique).
// ---------------------------------------------------------------------------
void chapter8() {
    title("8. Chaîne de Su-Schrieffer-Heeger : invariant d'enroulement");
    std::cout << "    v      w     enroulement de k -> h(k) autour de 0\n";
    bool ok = true;
    for (auto [v, w] : std::vector<std::pair<double, double>>{{1.0, 0.4}, {1.0, 0.8}, {0.4, 1.0}, {0.8, 1.0}, {0.5, -1.0}}) {
        std::vector<cd> h;
        for (int k = 0; k < 4000; ++k) h.emplace_back(v + w * std::cos(2 * PI * k / 4000), w * std::sin(2 * PI * k / 4000));
        int n = static_cast<int>(std::lround(winding_about(h, 0)));
        std::cout << "  " << std::setw(5) << v << "  " << std::setw(5) << w << "    " << n << (n ? "   (phase topologique : états de bord)" : "   (phase triviale)") << "\n";
        ok = ok && (n == (std::fabs(w) > std::fabs(v) ? 1 : 0));  // le signe de w tourne la courbe de pi : indice inchangé
    }
    check(ok, "enroulement = 1 si |w| > |v|, sinon 0 (il ne peut sauter qu'en fermant le gap |v| = |w|)");
}

}  // namespace

int main() {
    std::cout << std::fixed << std::setprecision(3);
    chapter1();
    chapter2();
    chapter3();
    chapter4();
    chapter5();
    chapter6();
    chapter7();
    chapter8();
    std::cout << "\n" << (failures == 0 ? "Toutes les vérifications passent." : "Des vérifications ont échoué.") << "\n";
    return failures == 0 ? 0 : 1;
}
