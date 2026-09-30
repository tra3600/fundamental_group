"""Génère les figures de COURS.md (python3 make_figures.py)."""
import numpy as np, matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.patches import FancyArrowPatch

plt.rcParams.update({"font.size": 10, "axes.spines.top": False, "axes.spines.right": False,
                     "figure.dpi": 100, "savefig.dpi": 160, "savefig.bbox": "tight"})
C = ["#1f77b4", "#d95f02", "#2a9d5c", "#7b3fa0"]
pi = np.pi

def save(name): plt.savefig(f"{name}.png"); plt.close()

def wind(z, c=0):
    w = z - c
    return int(round(np.sum(np.angle(np.roll(w, -1) / w)) / (2 * pi)))

# 1. Relèvement : boucle de degré 2 sur S^1 et son relèvement
t = np.linspace(0, 1, 400)
fig, ax = plt.subplots(1, 2, figsize=(9, 3.6))
th = 2 * pi * 2 * t
r = 1 + 0.06 * t
ax[0].plot(np.cos(th) * r, np.sin(th) * r, color=C[0])
ax[0].annotate("", xy=(np.cos(th[60]) * r[60], np.sin(th[60]) * r[60]), xytext=(np.cos(th[50]) * r[50], np.sin(th[50]) * r[50]),
               arrowprops=dict(arrowstyle="->", color=C[0]))
ax[0].plot([1], [0], "ko"); ax[0].text(1.1, 0.05, "point de base")
ax[0].set_aspect("equal"); ax[0].set_title("Lacet de degré 2 sur $S^1$ (rayon légèrement décalé)")
ax[0].axis("off")
ax[1].plot(t, th / (2 * pi), color=C[0])
ax[1].set_yticks([0, 1, 2]); ax[1].grid(alpha=.3)
ax[1].set_xlabel("$t$"); ax[1].set_ylabel(r"$\tilde\gamma(t)/2\pi$")
ax[1].set_title(r"Relèvement : $\deg=(\tilde\gamma(1)-\tilde\gamma(0))/2\pi=2$")
plt.tight_layout(); save("fig01_relevement")

# 2. Invariance par homotopie
fig, ax = plt.subplots(1, 2, figsize=(9, 3.6))
for k, eps in enumerate([0, 0.5, 1, 2, 3]):
    th = 6 * pi * t + eps * np.sin(4 * pi * t)
    ax[0].plot(t, th / (2 * pi), label=f"$\\varepsilon={eps}$", color=plt.cm.viridis(k / 4))
    ax[1].plot(np.cos(th) * (1 + .1 * k), np.sin(th) * (1 + .1 * k), color=plt.cm.viridis(k / 4), lw=.8)
ax[0].legend(fontsize=8); ax[0].set_xlabel("$t$"); ax[0].set_ylabel(r"$\tilde\gamma_\varepsilon/2\pi$"); ax[0].grid(alpha=.3)
ax[0].set_title("Déformation $6\\pi t+\\varepsilon\\sin 4\\pi t$ : toujours 3 tours")
ax[1].set_aspect("equal"); ax[1].axis("off"); ax[1].set_title("Les lacets (rayons décalés pour lisibilité)")
plt.tight_layout(); save("fig02_homotopie")

# 3. Principe de l'argument
roots = np.array([0.5, -1, 2j, 3])
p = lambda z: (z - 0.5) * (z + 1) * (z - 2j) * (z - 3)
fig, ax = plt.subplots(2, 4, figsize=(11, 5.4))
for j, R in enumerate([0.75, 1.5, 2.5, 3.5]):
    z = R * np.exp(2j * pi * np.linspace(0, 1, 4000, endpoint=False))
    ax[0, j].plot(z.real, z.imag, color=C[0]); ax[0, j].plot(roots.real, roots.imag, "r*", ms=9)
    ax[0, j].set_xlim(-4, 4); ax[0, j].set_ylim(-4, 4); ax[0, j].set_aspect("equal"); ax[0, j].set_title(f"$|z|={R}$")
    w = p(z); ax[1, j].plot(w.real, w.imag, color=C[1], lw=.9); ax[1, j].plot(0, 0, "k+", ms=10, mew=2)
    ax[1, j].set_aspect("equal"); ax[1, j].set_title(f"indice autour de 0 : {wind(w)}")
ax[0, 0].set_ylabel("cercle + racines ★"); ax[1, 0].set_ylabel("image $p(z)$")
plt.tight_layout(); save("fig03_argument")

# 4. Tore : carré identifié + courbes (p,q) en 3D
fig = plt.figure(figsize=(10, 4.2))
a = fig.add_subplot(1, 2, 1)
for (pp, qq), c in zip([(1, 0), (0, 1), (2, 3)], C):
    s = np.linspace(0, 1, 2000)
    x, y = (pp * s) % 1, (qq * s) % 1
    x[np.r_[False, np.abs(np.diff(x)) > .5]] = np.nan; y[np.r_[False, np.abs(np.diff(y)) > .5]] = np.nan
    a.plot(x, y, color=c, label=f"classe $({pp},{qq})$", lw=1.4)
a.set_aspect("equal"); a.legend(fontsize=8, loc="upper right", bbox_to_anchor=(1.45, 1)); a.set_title("Carré dont les côtés opposés sont identifiés")
a.set_xlabel("longitude"); a.set_ylabel("méridien")
b = fig.add_subplot(1, 2, 2, projection="3d")
u, v = np.meshgrid(np.linspace(0, 2 * pi, 60), np.linspace(0, 2 * pi, 30))
R0, r0 = 2, 1
b.plot_surface((R0 + r0 * np.cos(v)) * np.cos(u), (R0 + r0 * np.cos(v)) * np.sin(u), r0 * np.sin(v), alpha=.15, color="gray", linewidth=0)
s = np.linspace(0, 2 * pi, 600)
b.plot((R0 + 1.02 * np.cos(3 * s)) * np.cos(2 * s), (R0 + 1.02 * np.cos(3 * s)) * np.sin(2 * s), 1.02 * np.sin(3 * s), color=C[2], lw=2)
b.set_title("Courbe de type $(2,3)$ (nœud de trèfle)"); b.set_axis_off(); b.set_box_aspect((1, 1, .5))
plt.tight_layout(); save("fig04_tore")

# 5. Plan privé de deux points
p1, p2 = 0.5, -0.5
tt = np.linspace(0, 2 * pi, 4000, endpoint=False)
curves = [("$a$ : autour de $p_1$", p1 + .3 * np.exp(1j * tt)), ("$ab$ : grand cercle", 2 * np.exp(1j * tt)),
          ("$aB$ : lemniscate", (np.cos(tt) + 1j * np.sin(tt) * np.cos(tt)) / (1 + np.sin(tt) ** 2))]
fig, ax = plt.subplots(1, 3, figsize=(11, 3.6))
for k, (nm, z) in enumerate(curves):
    ax[k].plot(z.real, z.imag, color=C[k]); ax[k].plot([p1, p2], [0, 0], "ko", ms=6)
    ax[k].text(p1, -.22, "$p_1$", ha="center"); ax[k].text(p2, -.22, "$p_2$", ha="center")
    i = 200 if k != 2 else 100
    ax[k].annotate("", xy=(z[i + 40].real, z[i + 40].imag), xytext=(z[i].real, z[i].imag), arrowprops=dict(arrowstyle="->", color=C[k], lw=1.6))
    ax[k].set_aspect("equal"); ax[k].axis("off")
    ax[k].set_title(f"{nm}\nindices $({wind(z, p1)},{wind(z, p2)})$")
plt.tight_layout(); save("fig05_plan_perce")

# 6. Vortex / antivortex
N = 14; v, av = 4.5 + 4.5j, 9.5 + 8.5j
X, Y = np.meshgrid(np.arange(N), np.arange(N), indexing="ij"); Z = X + 1j * Y
th = np.angle(Z - v) - np.angle(Z - av)
wr = lambda a: (a + pi) % (2 * pi) - pi
fig, ax = plt.subplots(figsize=(5.4, 5))
ax.quiver(X, Y, np.cos(th), np.sin(th), th % (2 * pi), cmap="twilight", pivot="mid", scale=26)
for i in range(N - 1):
    for j in range(N - 1):
        q = int(round((wr(th[i + 1, j] - th[i, j]) + wr(th[i + 1, j + 1] - th[i + 1, j]) + wr(th[i, j + 1] - th[i + 1, j + 1]) + wr(th[i, j] - th[i, j + 1])) / (2 * pi)))
        if q: ax.plot(i + .5, j + .5, "o", ms=16, mfc="none", mec="red" if q > 0 else "blue", mew=2.5)
ax.plot([], [], "o", mfc="none", mec="red", mew=2, label="vortex $+1$"); ax.plot([], [], "o", mfc="none", mec="blue", mew=2, label="antivortex $-1$")
ax.legend(loc="upper center", bbox_to_anchor=(.5, -.02), ncol=2, frameon=False)
ax.set_aspect("equal"); ax.set_title("Champ de phase $\\theta$ et charges topologiques"); ax.axis("off")
save("fig06_vortex")

# 7. Aharonov-Bohm
f = np.linspace(0, 2, 400)
fig, ax = plt.subplots(figsize=(6.2, 3.4))
for n, c in zip([1, 2], C): ax.plot(f, 2 + 2 * np.cos(2 * pi * n * f), color=c, label=f"$n={n}$")
ax.set_xlabel("$\\Phi/\\Phi_0$"); ax.set_ylabel("intensité $2+2\\cos\\varphi$"); ax.grid(alpha=.3); ax.legend()
ax.set_title("Aharonov–Bohm : interférence selon le degré $n$ du lacet")
plt.tight_layout(); save("fig07_aharonov_bohm")

# 8. SSH
k = np.linspace(0, 2 * pi, 500)
fig, ax = plt.subplots(1, 2, figsize=(8, 3.6))
for a_, (vv, ww, nm) in zip(ax, [(1, .6, "$v=1,\\ w=0{,}6$ : $W=0$ (triviale)"), (.6, 1, "$v=0{,}6,\\ w=1$ : $W=1$ (topologique)")]):
    h = vv + ww * np.exp(1j * k); a_.plot(h.real, h.imag, color=C[0]); a_.plot(0, 0, "r*", ms=11)
    a_.set_aspect("equal"); a_.set_title(nm); a_.axhline(0, c="gray", lw=.4); a_.axvline(0, c="gray", lw=.4)
plt.tight_layout(); save("fig08_ssh")

# 9. SU(2) -> SO(3)
fig, ax = plt.subplots(1, 2, figsize=(8.6, 3.8))
for a_, kk in zip(ax, [1, 2]):
    s = np.linspace(0, kk * pi, 400); a_.plot(np.cos(s), np.sin(s), color=C[kk - 1], lw=2)
    a_.plot([1], [0], "ko"); a_.plot([np.cos(kk * pi)], [np.sin(kk * pi)], "s", color="red", ms=8)
    a_.set_aspect("equal"); a_.set_xlabel("$q_0=\\cos(\\varphi/2)$"); a_.set_ylabel("$q_3=\\sin(\\varphi/2)$")
    a_.set_title(f"{kk} tour{'s' if kk > 1 else ''} : $q(1)={int(round(np.cos(kk*pi))):+d}$ → " + ("lacet ouvert, non trivial" if kk == 1 else "lacet fermé, trivial"))
plt.tight_layout(); save("fig09_so3")

# 10. Déroulement de phase et saut de cycle
x = np.linspace(0, 1, 400); true = 14 * x ** 1.5
fig, ax = plt.subplots(1, 3, figsize=(11, 3.3))
ax[0].plot(x, true, color=C[0]); ax[0].set_title("phase vraie (relèvement)")
ax[1].plot(x, (true + pi) % (2 * pi) - pi, ".", ms=2, color=C[1]); ax[1].set_title("phase mesurée (modulo $2\\pi$)")
xs = np.linspace(0, 1, 400); w = (true + pi) % (2 * pi) - pi
un = np.cumsum(np.r_[w[0], wr(np.diff(w))]); ax[2].plot(x, true, color=C[0], label="vraie", lw=3, alpha=.4); ax[2].plot(x, un, "--", color=C[2], label="déroulée, pas fin")
xc = np.linspace(0, 1, 6); wc = ((14 * xc ** 1.5) + pi) % (2 * pi) - pi
unc = np.cumsum(np.r_[wc[0], wr(np.diff(wc))]); ax[2].plot(xc, unc, "o-", color="red", label="pas $>\\pi$ : saut de cycle")
ax[2].legend(fontsize=7); ax[2].set_title("déroulement")
for a_ in ax: a_.set_xlabel("$x$"); a_.grid(alpha=.3)
plt.tight_layout(); save("fig10_deroulement")
print("ok")
