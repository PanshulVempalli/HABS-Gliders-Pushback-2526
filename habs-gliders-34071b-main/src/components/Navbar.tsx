import { useState } from "react";
import { motion, AnimatePresence } from "framer-motion";
import { Menu, X } from "lucide-react";
import { Link, useLocation } from "react-router-dom";

const links = [
  { label: "About", href: "#about" },
  { label: "Journey", href: "#journey" },
  { label: "Team", href: "#team" },
  { label: "Robot", href: "#robot" },
  { label: "Achievements", href: "#achievements" },
  { label: "Worlds", href: "#worlds" },
  { label: "Push Back", href: "/push-back" },
];

const Navbar = () => {
  const [open, setOpen] = useState(false);
  const location = useLocation();
  const isHome = location.pathname === "/";

  const renderLink = (l: { label: string; href: string }, onClick?: () => void) => {
    const className = "text-muted-foreground hover:text-primary transition-colors text-sm font-medium uppercase tracking-wider";

    if (l.href.startsWith("/")) {
      return (
        <Link key={l.href} to={l.href} className={className} onClick={onClick}>
          {l.label}
        </Link>
      );
    }

    // Hash links: if not on home, navigate to home + hash
    const href = isHome ? l.href : `/${l.href}`;
    return (
      <a key={l.href} href={href} className={className} onClick={onClick}>
        {l.label}
      </a>
    );
  };

  return (
    <nav className="fixed top-0 left-0 right-0 z-50 bg-background/80 backdrop-blur-md border-b border-border">
      <div className="flex items-center justify-between py-3 pl-10 pr-3 w-full">
        <Link to="/" className="font-display text-4xl tracking-wider text-primary">
          HABS GLIDERS
          <span className="text-gold ml-2 text-xl font-body">34071B</span>
        </Link>

        {/* Desktop */}
        <div className="hidden md:flex gap-6">
          {links.map((l) => renderLink(l))}
        </div>

        {/* Mobile toggle */}
        <button
          className="md:hidden text-primary"
          onClick={() => setOpen(!open)}
        >
          {open ? <X size={24} /> : <Menu size={24} />}
        </button>
      </div>

      <AnimatePresence>
        {open && (
          <motion.div
            initial={{ height: 0, opacity: 0 }}
            animate={{ height: "auto", opacity: 1 }}
            exit={{ height: 0, opacity: 0 }}
            className="md:hidden overflow-hidden bg-background border-b border-border"
          >
            <div className="flex flex-col gap-2 p-4">
              {links.map((l) => renderLink(l, () => setOpen(false)))}
            </div>
          </motion.div>
        )}
      </AnimatePresence>
    </nav>
  );
};

export default Navbar;
