"""COURS.md -> COURS.pdf  (Markdown + KaTeX + Chromium).

Prérequis : pip install markdown ; npm install katex playwright ; un Chromium.
Usage : KATEX_DIR=/chemin/node_modules/katex/dist python3 build_pdf.py
"""
import html, os, re, subprocess, sys, pathlib
import markdown

root = pathlib.Path(__file__).resolve().parent.parent
katex = pathlib.Path(os.environ.get("KATEX_DIR", "node_modules/katex/dist")).resolve()
src = (root / "COURS.md").read_text(encoding="utf-8")

# 1. Protéger les formules du parseur Markdown.
store = []
def keep(m, display):
    store.append((m.group(1), display)); return f"@@MATH{len(store) - 1}@@"
src = re.sub(r"\$\$(.+?)\$\$", lambda m: keep(m, True), src, flags=re.S)
src = re.sub(r"\$((?:[^$\n\\]|\\.)+?)\$", lambda m: keep(m, False), src)

body = markdown.markdown(src, extensions=["tables", "fenced_code", "toc"], output_format="html5")
for i, (tex, display) in enumerate(store):
    tex = html.escape(tex, quote=False)
    body = body.replace(f"@@MATH{i}@@", f"\\[{tex}\\]" if display else f"\\({tex}\\)")

body = body.replace('src="figures/', f'src="file://{root}/figures/')

css = """
@page { size: A4; margin: 18mm 16mm; }
body { font: 10.5pt/1.5 'DejaVu Serif', Georgia, serif; color:#1b1b1b; max-width: 100%; }
h1 { font-size: 22pt; border-bottom: 2px solid #1f77b4; padding-bottom: 6px; }
h2 { font-size: 15pt; color:#1f77b4; margin-top: 26px; page-break-after: avoid; }
h3 { font-size: 12pt; page-break-after: avoid; }
img { display:block; max-width: 100%; margin: 10px auto 0; page-break-inside: avoid; }
p em:only-child { display:block; text-align:center; font-size: 9pt; color:#555; margin-bottom: 12px; }
table { border-collapse: collapse; margin: 10px 0; font-size: 9pt; page-break-inside: avoid; }
th, td { border: 1px solid #bbb; padding: 3px 6px; } th { background:#eef3f8; }
blockquote { border-left: 3px solid #1f77b4; margin: 10px 0; padding: 2px 12px; background:#f5f8fb; }
code, pre { font: 9pt 'DejaVu Sans Mono', monospace; background:#f3f3f3; }
pre { padding: 6px 10px; white-space: pre-wrap; } hr { border:0; border-top:1px solid #ccc; }
"""
page = f"""<!doctype html><html lang="fr"><head><meta charset="utf-8"><title>Le groupe fondamental</title>
<link rel="stylesheet" href="file://{katex}/katex.min.css"><style>{css}</style>
<script src="file://{katex}/katex.min.js"></script><script src="file://{katex}/contrib/auto-render.min.js"></script></head>
<body>{body}
<script>renderMathInElement(document.body, {{delimiters:[{{left:'\\\\[',right:'\\\\]',display:true}},{{left:'\\\\(',right:'\\\\)',display:false}}],throwOnError:false}});
document.body.setAttribute('data-ready','1');</script></body></html>"""
out_html = root / "tools" / "_cours.html"
out_html.write_text(page, encoding="utf-8")

js = f"""
const {{ chromium }} = require(process.env.PLAYWRIGHT_DIR || 'playwright');
(async () => {{
  const b = await chromium.launch({{ executablePath: process.env.CHROMIUM || undefined }});
  const p = await b.newPage();
  await p.goto('file://{out_html}');
  await p.waitForSelector('body[data-ready]'); await p.evaluate(() => document.fonts.ready);
  await p.pdf({{ path: '{root / "COURS.pdf"}', format: 'A4', printBackground: true,
                 margin: {{ top: '18mm', bottom: '18mm', left: '16mm', right: '16mm' }} }});
  await b.close();
}})();
"""
(root / "tools" / "_print.js").write_text(js)
subprocess.run(["node", str(root / "tools" / "_print.js")], check=True)
out_html.unlink(); (root / "tools" / "_print.js").unlink()
print("COURS.pdf écrit")
