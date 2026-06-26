import { useEffect, useState, type ReactNode } from "react";
import { Link, useLocation } from "react-router-dom";
import bgGif from "@/assets/animacion.gif";
import iconLoot from "@/assets/icon-loot.png";
import iconMagic from "@/assets/icon-magic.png";
import iconClasses from "@/assets/icon-crown.png";
import iconCombat from "@/assets/icon-chest.png";
import leatherBg from "@/assets/leather.png";

const heroBanner = bgGif;

function scrollToId(id: string) {
    const el = document.getElementById(id);
    if (el) el.scrollIntoView({ behavior: "smooth", block: "start" });
}

type CmdItem = { keys: string[]; desc: ReactNode };

const generalCommands: CmdItem[] = [
    { keys: ["/ayuda", "h"], desc: "Abre esta guía o cierra cualquier panel de comandos abierto." },
    { keys: ["/ayuda-npc"], desc: "Muestra comandos para relacionarse con NPCs." },
    { keys: ["/ayuda-clan"], desc: "Muestra comandos de clanes." },
    { keys: ["/ayuda-cheat"], desc: "Muestra trucos del modo desarrollo." },
    { keys: ["C"], desc: "Abre el chat para escribir comandos o enviar mensajes." },
    { keys: ["@nombre <msg>"], desc: "Envía un mensaje privado a ese jugador." },
    { keys: ["/inventario"], desc: "Lista los items que tienes en el inventario." },
    { keys: ["/resucitar"], desc: "Revives como fantasma. Tardará más mientras más lejos estés de un sacerdote." },
    { keys: ["/meditar", "M"], desc: "Recuperas maná en trance." },
    { keys: ["/tirar", "Q"], desc: "Tiras al suelo el item seleccionado." },
    { keys: ["/tomar", "E"], desc: "Recoges el item u oro del suelo." },
    { keys: ["/viajar"], desc: "(vinculado a un totem) Viajas por el éter hacia tu destino." },
];

const clanNone: CmdItem[] = [
    { keys: ["/fundar-clan <nombre>"], desc: "Fundar tu propio clan y jurarle lealtad. Piénsalo bien: no podrás abandonarlo." },
    { keys: ["/unirse <clan>"], desc: "Enviar solicitud de unión a un clan existente." },
];
const clanLeader: CmdItem[] = [
    { keys: ["/revisar-clan"], desc: "Lista los miembros del clan y solicitudes pendientes." },
    { keys: ["/clan-aceptar <user>"], desc: "Aceptar la solicitud de un usuario." },
    { keys: ["/clan-rechazar <user>"], desc: "Rechazar la solicitud de un usuario." },
    { keys: ["/clan-ban <user>"], desc: "Banear a un usuario. Sus futuras solicitudes serán descartadas." },
    { keys: ["/clan-kick <user>"], desc: "Expulsar a un miembro del clan." },
];
const clanMember: CmdItem[] = [
    { keys: ["/dejar-clan"], desc: "Abandonar tu clan." },
];

const npcGeneric: CmdItem[] = [
    { keys: ["/listar"], desc: "Pedirle al NPC que liste los items que tiene." },
    { keys: ["/comprar <item>"], desc: "Comprar 1 unidad del item que tenga a la venta." },
];
const npcMerchant: CmdItem[] = [
    { keys: ["/vender <item>"], desc: "Vender 1 unidad de ese item." },
];
const npcBanker: CmdItem[] = [
    { keys: ["/depositar oro <n>", "/retirar"], desc: "Depositar o retirar oro." },
    { keys: ["/depositar <item>", "/retirar <item>"], desc: "Depositar o retirar un item específico." },
];
const npcPriest: CmdItem[] = [
    { keys: ["/curar"], desc: "Te curas por completo." },
    { keys: ["/resucitar"], desc: "Te resucita inmediatamente." },
];

const cheats: CmdItem[] = [
    { keys: ["/cheat-set-xp <n>"], desc: "Obtienes el nivel de experiencia que te asignes." },
    { keys: ["/cheat-gain-gold <n>"], desc: "Ganas esa cantidad de oro (o el máximo que permita tu nivel)." },
    { keys: ["/cheat-kill-self"], desc: "Te conviertes en fantasma." },
    { keys: ["/cheat-infinite-recoverables"], desc: "Vida y maná infinitos. Vuelve a ingresarlo para desactivarlo." },
    { keys: ["/cheat-get-item <item>"], desc: "De ser posible, agrega ese item a tu inventario." },
    { keys: ["/cheat-kill-creatures"], desc: "Mata a todas las criaturas del mundo." },
];

function Cmd({ children }: { children: ReactNode }) {
    return <code className="cmd-key">{children}</code>;
}

function CmdRow({ cmd }: { cmd: CmdItem }) {
    return (
        <li className="grid grid-cols-1 gap-2 border-b border-dashed border-border/40 py-3 md:grid-cols-[minmax(0,18rem)_1fr] md:gap-6 md:items-baseline last:border-0">
            <div className="flex flex-wrap gap-1.5">
                {cmd.keys.map((k, i) => (
                    <span key={i} className="flex items-baseline gap-1.5">
            <Cmd>{k}</Cmd>
                        {i < cmd.keys.length - 1 && <span className="text-gold-dim text-sm">o</span>}
          </span>
                ))}
            </div>
            <p className="text-foreground/90 leading-snug">{cmd.desc}</p>
        </li>
    );
}

function SectionPanel({
                          id, title, eyebrow, icon, children, iconShrink,
                      }: { id: string; title: string; eyebrow: string; icon: string; children: ReactNode; iconShrink?: boolean }) {
    return (
        <section id={id} className="scroll-mt-24">
            <div
                className="pixel-frame p-6 md:p-10 mb-20"
                style={{ backgroundImage: `url(${leatherBg})`, backgroundSize: "256px 256px", backgroundRepeat: "repeat", imageRendering: "pixelated" }}
            >
                <div className="flex items-center gap-4 mb-6">
                    <div className="flex-shrink-0 h-[77px] w-[77px] md:h-[90px] md:w-[90px] flex items-center justify-center">
                        <img src={icon} alt="" className={`pixel max-h-full max-w-full drop-shadow-[3px_3px_0_rgba(0,0,0,0.8)] ${iconShrink ? 'scale-[0.8]' : ''}`} loading="lazy" />
                    </div>
                    <div>
                        <div className="font-pixel text-[0.55rem] md:text-[0.65rem] text-gold-dim uppercase tracking-widest">{eyebrow}</div>
                        <h2 className="font-display text-3xl md:text-4xl text-gold mt-1">{title}</h2>
                    </div>
                </div>
                <div className="pixel-frame-inner p-5 md:p-8">
                    {children}
                </div>
            </div>
        </section>
    );
}

function TipBox({ title, children }: { title: string; children: ReactNode }) {
    return (
        <div className="my-5 border-l-4 border-gold bg-black/40 px-4 py-3 relative">
            <div className="absolute -left-3 -top-3 font-pixel text-[0.6rem] bg-gold text-background px-2 py-1">★</div>
            <div className="font-pixel text-[0.6rem] text-gold-bright uppercase tracking-wider mb-1">{title}</div>
            <p className="text-foreground/90">{children}</p>
        </div>
    );
}

export default function Aventurero() {
    const [open, setOpen] = useState(false);
    const location = useLocation();

    useEffect(() => {
        const target = (location.state as { scrollTo?: string } | null)?.scrollTo;
        if (target) {
            setTimeout(() => scrollToId(target), 0);
        }
    }, [location.state]);

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

            <section id="top" className="relative overflow-hidden border-b-4 border-gold/30">
                <div className="absolute inset-0 scanlines">
                    <img src={heroBanner} alt="" width={1920} height={768}
                         className="pixel h-full w-full object-cover opacity-60" />
                    <div className="absolute inset-0 bg-gradient-to-b from-background/40 via-background/30 to-background" />
                </div>
                <div className="relative mx-auto max-w-5xl px-4 py-24 md:py-36 text-center">
                    <div className="font-pixel text-[0.6rem] md:text-[0.75rem] text-gold-bright tracking-[0.3em] uppercase mb-6 torch-flicker">
                        ⚜  Pergamino II  ⚜
                    </div>
                    <h1 className="font-display text-5xl md:text-7xl lg:text-8xl title-glow leading-[1.1]">
                        Manual del<br />Aventurero
                    </h1>
                    <p className="mt-8 max-w-2xl mx-auto text-xl md:text-2xl text-foreground/85">
                        Pulsa <Cmd>h</Cmd> o escribe <Cmd>/ayuda</Cmd> en el chat para invocar esta guía dentro del juego.
                    </p>
                    <div className="mt-10 flex flex-wrap justify-center gap-x-16 gap-y-6">
                        <button onClick={() => scrollToId("comandos")} className="pixel-btn">Comenzar a leer</button>
                    </div>
                </div>
            </section>

            <main className="mx-auto max-w-5xl px-4 py-20">
                <SectionPanel id="comandos" eyebrow="Capítulo I" title="Comandos Generales" icon={iconCombat}>
                    <p className="mb-5 text-muted-foreground italic">Las palabras de poder que todo aventurero debe conocer antes de enfrentarse a lo desconocido.</p>
                    <ul className="divide-y divide-border/30">
                        {generalCommands.map((c, i) => <CmdRow key={i} cmd={c} />)}
                    </ul>
                    <TipBox title="Sabiduría del Maestro">
                        Recuerda que todos los comandos empiezan con <Cmd>/</Cmd> y que con <Cmd>ESC</Cmd> puedes cerrar el chat.
                    </TipBox>
                </SectionPanel>

                <SectionPanel id="clanes" eyebrow="Capítulo II" title="La Hermandad de Clanes" icon={iconClasses} iconShrink>
                    <h3 className="font-display text-2xl text-gold-bright mt-2 mb-3">⚑ Aún no perteneces a un clan</h3>
                    <ul className="divide-y divide-border/30">{clanNone.map((c, i) => <CmdRow key={i} cmd={c} />)}</ul>

                    <h3 className="font-display text-2xl text-gold-bright mt-8 mb-3">⚑ Fundaste tu propio clan</h3>
                    <ul className="divide-y divide-border/30">{clanLeader.map((c, i) => <CmdRow key={i} cmd={c} />)}</ul>

                    <h3 className="font-display text-2xl text-gold-bright mt-8 mb-3">⚑ Eres miembro de un clan</h3>
                    <ul className="divide-y divide-border/30">{clanMember.map((c, i) => <CmdRow key={i} cmd={c} />)}</ul>

                    <TipBox title="Consejo Estratégico">
                        Si estás cerca de tus compañeros de clan, recibirás un bonus de ataque y defensa. La unión hace la fuerza.
                    </TipBox>
                </SectionPanel>

                <SectionPanel id="npcs" eyebrow="Capítulo III" title="Interacción con NPCs" icon={iconLoot}>
                    <TipBox title="Tip de Control">
                        Hablas con los NPC haciendo click izquierdo sobre ellos. Si no responden, acércate más.
                    </TipBox>

                    <h3 className="font-display text-2xl text-gold-bright mt-6 mb-3">✦ NPC genérico</h3>
                    <ul className="divide-y divide-border/30">{npcGeneric.map((c, i) => <CmdRow key={i} cmd={c} />)}</ul>

                    <h3 className="font-display text-2xl text-gold-bright mt-8 mb-3">✦ Comerciante</h3>
                    <ul className="divide-y divide-border/30">{npcMerchant.map((c, i) => <CmdRow key={i} cmd={c} />)}</ul>

                    <h3 className="font-display text-2xl text-gold-bright mt-8 mb-3">✦ Banquero</h3>
                    <ul className="divide-y divide-border/30">{npcBanker.map((c, i) => <CmdRow key={i} cmd={c} />)}</ul>

                    <h3 className="font-display text-2xl text-gold-bright mt-8 mb-3">✦ Sacerdote</h3>
                    <ul className="divide-y divide-border/30">{npcPriest.map((c, i) => <CmdRow key={i} cmd={c} />)}</ul>
                </SectionPanel>

                <SectionPanel id="trucos" eyebrow="Capítulo IV · Prohibido" title="Trucos del Hechicero" icon={iconMagic}>
                    <TipBox title="¡Advertencia!">
                        Estos comandos pertenecen al modo desarrollo / pruebas. Úsalos con honor — o no los uses.
                    </TipBox>
                    <ul className="divide-y divide-border/30 mt-4">{cheats.map((c, i) => <CmdRow key={i} cmd={c} />)}</ul>
                </SectionPanel>
            </main>

            <footer className="border-t-2 border-gold/30 bg-parchment-dark/60">
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