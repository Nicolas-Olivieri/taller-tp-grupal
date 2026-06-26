import { useState, type ReactNode } from "react";
import { Link } from "react-router-dom";
import leatherBg from "@/assets/leather.png";
import heroBg from "@/assets/magic-book.jpg";

function Code({ children }: { children: ReactNode }) {
  return <code className="cmd-key">{children}</code>;
}

function Block({ children, copy }: { children: string; copy?: boolean }) {
  const [copied, setCopied] = useState(false);
  const handleCopy = async () => {
    try {
      await navigator.clipboard.writeText(children);
      setCopied(true);
      setTimeout(() => setCopied(false), 1500);
    } catch {
      /* noop */
    }
  };
  return (
      <div className="relative my-3">
      <pre className="bg-black/60 border border-gold/30 px-4 py-3 pr-24 overflow-x-auto text-sm text-foreground/90">
        <code>{children}</code>
      </pre>
        {copy && (
            <button
                type="button"
                onClick={handleCopy}
                className="absolute top-2 right-2 font-pixel text-[0.55rem] uppercase tracking-widest px-2 py-1 border border-gold/50 text-gold-bright bg-background/70 hover:bg-gold/20 transition-colors"
                aria-label="Copiar al portapapeles"
            >
              {copied ? "¡Copiado!" : "Copiar"}
            </button>
        )}
      </div>
  );
}

function Panel({ title, eyebrow, children }: { title: string; eyebrow: string; children: ReactNode }) {
  return (
      <section className="scroll-mt-24">
        <div
            className="pixel-frame p-6 md:p-10 mb-12"
            style={{ backgroundImage: `url(${leatherBg})`, backgroundSize: "256px 256px", backgroundRepeat: "repeat", imageRendering: "pixelated" }}
        >
          <div className="mb-6">
            <div className="font-pixel text-[0.55rem] md:text-[0.65rem] text-gold-dim uppercase tracking-widest">{eyebrow}</div>
            <h2 className="font-display text-3xl md:text-4xl text-gold mt-1">{title}</h2>
          </div>
          <div className="pixel-frame-inner p-5 md:p-8">{children}</div>
        </div>
      </section>
  );
}

function SubTitle({ children }: { children: ReactNode }) {
  return <h3 className="font-display text-2xl text-gold-bright mt-6 mb-3">⚑ {children}</h3>;
}

export default function Manual() {
  const [open, setOpen] = useState(false);

  return (
      <div className="min-h-screen relative">
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

        <section className="relative overflow-hidden border-b-4 border-gold/30">
          <div className="absolute inset-0 scanlines">
            <img src={heroBg} alt="" className="pixel h-full w-full object-cover opacity-40" />
            <div className="absolute inset-0 bg-gradient-to-b from-background/60 via-background/40 to-background" />
          </div>
          <div className="relative mx-auto max-w-5xl px-4 py-24 md:py-36 text-center">
            <div className="font-pixel text-[0.6rem] md:text-[0.75rem] text-gold-bright tracking-[0.3em] uppercase mb-6 torch-flicker">
              ⚜  Pergamino III  ⚜
            </div>
            <h1 className="font-display text-5xl md:text-7xl lg:text-8xl title-glow leading-[1.1]">
              Manual de<br />Instalación
            </h1>
            <p className="mt-8 max-w-2xl mx-auto text-xl md:text-2xl text-foreground/85">
              Guía para compilar, configurar y ejecutar el servidor y cliente del proyecto.
            </p>
          </div>
        </section>

        <main className="mx-auto max-w-5xl px-4 py-16">
          <Panel eyebrow="Capítulo I" title="Requerimientos de sistema">
            <SubTitle>Dependencias de Compilación</SubTitle>
            <ul className="list-disc list-inside text-foreground/90 space-y-1">
              <li><Code>git</Code></li>
              <li><Code>cmake</Code> (versión 3.24 como mínima)</li>
              <li><Code>build-essential</Code></li>
              <li><Code>ninja-build</Code></li>
              <li><Code>pkg-config</Code></li>
              <li><Code>gcc-13</Code></li>
              <li><Code>g++-13</Code></li>
            </ul>

            <SubTitle>Sistema Operativo</SubTitle>
            <ul className="list-disc list-inside text-foreground/90 space-y-1">
              <li><Code>Ubuntu 24.04</Code></li>
            </ul>
            <p className="text-foreground/80 mt-2 italic">
              Ubuntu 24.04 tiene el soporte principal, pero el proyecto también puede ejecutarse en otras distribuciones basadas en UNIX.
            </p>

            <SubTitle>Dependencias gráficas / auditivas</SubTitle>
            <ul className="list-disc list-inside text-foreground/90 space-y-1 columns-1 md:columns-2">
              {["qt6-base-dev","libasound2-dev","libopus-dev","libopusfile-dev","libxmp-dev","libfluidsynth-dev","fluidsynth","libwavpack-dev","libfreetype-dev","libsdl2-dev","libsdl2-image-dev","libsdl2-mixer-dev","libsdl2-ttf-dev"].map(p => (
                  <li key={p}><Code>{p}</Code></li>
              ))}
            </ul>
          </Panel>

          <Panel eyebrow="Capítulo II" title="Pasos para compilar el proyecto">
            <SubTitle>1. Ejecutar el instalador installer.sh</SubTitle>
            <Block copy>sudo bash installer.sh</Block>
            <p className="text-foreground/90">o darle permisos de ejecución:</p>
            <Block copy>chmod +x installer.sh</Block>
            <p className="text-foreground/90">y luego ejecutar:</p>
            <Block copy>sudo ./installer.sh</Block>

            <SubTitle>2. Ingresar tu contraseña</SubTitle>

            <SubTitle>3. Espera a que se realice la instalación</SubTitle>
            <p className="text-foreground/90">
              Dependiendo de tu conexión, puede tardar entre 10 a 15 minutos. El instalador se encarga de bajar todas las dependencias necesarias para jugar. Además, creará los lanzadores en el escritorio.
            </p>
          </Panel>

          <Panel eyebrow="Capítulo III" title="Pasos para configurar el software">
            <SubTitle>Modificar archivos de Configuración y/o Assets</SubTitle>
            <p className="text-foreground/90 mb-2">El instalador automáticamente guarda:</p>
            <ul className="list-disc list-inside text-foreground/90 space-y-1">
              <li>Los archivos de configuración en <Code>/etc/taller-tp</Code></li>
              <li>Los archivos de assets (imágenes, animaciones, audios y mapa) en <Code>/var/taller-tp</Code></li>
            </ul>
            <p className="text-foreground/90 mt-2">No es necesario mover ningún archivo manualmente a estas carpetas.</p>

            <SubTitle>Actualizar el Mapa del servidor</SubTitle>
            <ol className="list-decimal list-inside text-foreground/90 space-y-2">
              <li>El instalador guardó el lanzador <Code>editor.sh</Code> en el escritorio.</li>
              <li>Ejecuta el lanzador <Code>editor.sh</Code>.</li>
              <li>
                Dibuja el mapa que quieras:
                <ul className="list-disc list-inside ml-5 mt-1 space-y-1">
                  <li>No olvides ponerle zonas seguras.</li>
                  <li>Puedes cargar el mapa actual con el botón “Cargar Mapa”.</li>
                  <li>Busca en <Code>/var/taller-tp/resources/map</Code> el archivo <Code>map.bin</Code>.</li>
                  <li>O busca otro archivo que ya hayas guardado con el editor.</li>
                </ul>
              </li>
              <li>Haz click en el botón “Guardar Mapa”.</li>
              <li>El servidor carga el mapa siempre desde <Code>/var/taller-tp/resources/map/map.bin</Code> ⇒ Reemplázalo.</li>
              <li>Cuidado: al modificar el mapa es muy probable que los archivos de persistencia se hayan corrompido.</li>
            </ol>

            <SubTitle>Borrar la persistencia de jugadores y clanes</SubTitle>
            <ol className="list-decimal list-inside text-foreground/90 space-y-1">
              <li>Busca en <Code>/var/taller-tp/resources/</Code>.</li>
              <li>Borra los archivos <Code>player_data.bin</Code> e <Code>index.bin</Code>.</li>
              <li>Cuidado: borrar solo uno de los archivos dejará un estado inválido.</li>
            </ol>

            <SubTitle>Levantar el Servidor</SubTitle>
            <ol className="list-decimal list-inside text-foreground/90 space-y-1">
              <li>El instalador guardó el lanzador <Code>server.sh</Code> en el escritorio.</li>
              <li>Ejecuta el lanzador <Code>server.sh</Code>.</li>
              <li>Se notificará por terminal sobre qué puerto se está usando.</li>
            </ol>

            <SubTitle>Lanzar el Cliente</SubTitle>
            <ol className="list-decimal list-inside text-foreground/90 space-y-1">
              <li>El instalador guardó el lanzador <Code>client.sh</Code> en el escritorio.</li>
              <li>Ejecuta el lanzador <Code>client.sh</Code>.</li>
              <li>Si levantas el servidor por tu cuenta, la dirección IP será <Code>127.0.0.1</Code> (o <Code>localhost</Code>) y el puerto lo dirá el lanzador.</li>
              <li>Si tú no levantaste el servidor, tendrás que preguntarle a quien lo haya levantado por estos 2 valores.</li>
            </ol>
          </Panel>

          <Panel eyebrow="Capítulo IV" title="¿Cómo se Juega?">
            <ul className="list-disc list-inside text-foreground/90 space-y-2">
              <li>Puedes crearte tu personaje si es la primera vez que te conectas al servidor.</li>
              <li>Una vez en el mundo te mueves con las teclas <Code>WASD</Code> o con las flechas de dirección.</li>
              <li>Las estrellas del suelo indican que te encuentras en una zona segura: nadie puede atacarte mientras la estés pisando.</li>
              <li>Habla con los NPCs haciéndoles click izquierdo.</li>
              <li>Ataca a las criaturas del mundo haciéndoles click izquierdo.</li>
              <li>Sube de nivel, consigue oro y equípate items poderosos con click derecho en los items de tu inventario.</li>
              <li>Habla con amigos y envía comandos haciendo click en el chat y luego escribiendo. Dejarás de escribir si presionas <Code>ESC</Code>.</li>
              <li>Presiona la tecla <Code>H</Code> para consultar la guía de comandos.</li>
            </ul>
          </Panel>

          <div className="text-center">
            <Link to="/" className="pixel-btn">← Volver al Inicio</Link>
          </div>
        </main>

        <footer className="border-t-2 border-gold/30 bg-parchment-dark/60 mt-10">
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