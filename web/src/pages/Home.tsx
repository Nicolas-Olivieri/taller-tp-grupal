import { useState } from "react";
import { Link } from "react-router-dom";
import leatherBg from "@/assets/leather.png";

export default function Home() {
  const [open, setOpen] = useState(false);

  return (
      <div className="min-h-screen relative flex flex-col">
        <div
            aria-hidden
            className="fixed inset-0 -z-10 pixel"
            style={{ backgroundImage: `url(${leatherBg})`, backgroundSize: "512px 512px", backgroundRepeat: "repeat", imageRendering: "pixelated" }}
        />
        <div aria-hidden className="fixed inset-0 -z-10 bg-background/70" />

        <header className="sticky top-0 z-40 border-b-2 border-gold/40 bg-background/95 backdrop-blur">
          <div className="mx-auto flex max-w-6xl items-center justify-between px-4 py-3">
            <Link to="/" className="flex items-center gap-3">
              <span className="font-pixel text-[0.7rem] text-gold torch-flicker">⚔</span>
              <span className="font-display text-xl text-gold-bright">Argentum Online</span>
            </Link>
            <nav className="hidden md:flex items-center gap-1">
              <Link to="/" className="font-pixel text-[0.6rem] uppercase tracking-widest px-3 py-2 text-muted-foreground hover:text-gold-bright transition-colors">Trailer</Link>
              <Link to="/aventurero" className="font-pixel text-[0.6rem] uppercase tracking-widest px-3 py-2 text-muted-foreground hover:text-gold-bright transition-colors">Comandos</Link>
              <Link to="/manual" className="font-pixel text-[0.6rem] uppercase tracking-widest px-3 py-2 text-muted-foreground hover:text-gold-bright transition-colors">Instalación</Link>
              <a href="https://github.com/Nicolas-Olivieri/taller-tp-grupal" target="_blank" rel="noreferrer" className="pixel-btn ml-3">GitHub</a>
            </nav>
            <button onClick={() => setOpen((v) => !v)} className="md:hidden font-pixel text-[0.7rem] text-gold border border-gold/50 px-3 py-2">
              ≡
            </button>
          </div>
          {open && (
              <div className="md:hidden border-t border-gold/30 bg-parchment-dark px-4 py-3 flex flex-col gap-2">
                <Link to="/" onClick={() => setOpen(false)} className="font-pixel text-[0.65rem] uppercase tracking-widest py-2 text-foreground/90">▸ Trailer</Link>
                <Link to="/aventurero" onClick={() => setOpen(false)} className="font-pixel text-[0.65rem] uppercase tracking-widest py-2 text-foreground/90">▸ Comandos</Link>
                <Link to="/manual" onClick={() => setOpen(false)} className="font-pixel text-[0.65rem] uppercase tracking-widest py-2 text-foreground/90">▸ Instalación</Link>
              </div>
          )}
        </header>

        <main className="flex-1 mx-auto max-w-5xl px-4 py-16 md:py-24 text-center w-full">
          <div className="font-pixel text-[0.6rem] md:text-[0.75rem] text-gold-bright tracking-[0.3em] uppercase mb-6 torch-flicker">
            ⚜  Pergamino I  ⚜
          </div>
          <h1 className="font-display text-3xl md:text-4xl lg:text-5xl title-glow leading-[1.1] mb-12">
            ¡Ya disponible!
          </h1>

          <div className="relative w-full max-w-4xl mx-auto aspect-video mb-12 pixel-frame p-2 bg-black/50">
            <iframe
                className="w-full h-full"
                src="https://www.youtube.com/embed/F_oyiC2E7cQ"
                title="Argentum Online Trailer"
                frameBorder="0"
                allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
                allowFullScreen
            ></iframe>
          </div>

          <div className="mt-10 flex flex-wrap justify-center gap-x-16 gap-y-6">
            <Link to="/aventurero" className="pixel-btn">Manual del Aventurero</Link>
            <Link to="/manual" className="pixel-btn">Manual de Instalación</Link>
          </div>
        </main>

        <footer className="border-t-2 border-gold/30 bg-parchment-dark/60 mt-auto">
          <div className="mx-auto max-w-5xl px-4 py-10 text-center">
            <div className="font-pixel text-[0.6rem] text-gold tracking-widest uppercase mb-3 torch-flicker">
              ⚔  Forjado para Taller cátedra Veiga  ⚔
            </div>
            <p className="text-muted-foreground">
              <a href="https://github.com/bono-neer" target="_blank" rel="noreferrer" className="text-gold-bright hover:underline">Bono Neer</a>
              {" · "}
              <a href="https://github.com/Nicolas-Olivieri" target="_blank" rel="noreferrer" className="text-gold-bright hover:underline">Nicolás Olivieri</a>
              {" · "}
              <a href="https://github.com/bskorokhod" target="_blank" rel="noreferrer" className="text-gold-bright hover:underline">Bogdan Skorokhod</a>
              {" · "}
              <a href="https://github.com/OrianaToscano" target="_blank" rel="noreferrer" className="text-gold-bright hover:underline">Oriana Toscano</a>
            </p>
          </div>
        </footer>
      </div>
  );
}