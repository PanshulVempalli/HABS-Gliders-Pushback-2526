import { Link } from "react-router-dom";

const Footer = () => {
  const year = new Date().getFullYear();
  return (
    <footer className="py-12 border-t border-border">
      <div className="container mx-auto px-4 text-center">
        <p className="font-display text-primary text-lg tracking-wider mb-2">
          HABS GLIDERS 34071B
        </p>
        <p className="text-muted-foreground text-sm">
          Haberdashers' Elstree Schools · VEX V5 Robotics · Push Back 2025-2026 Season
        </p>
        <p className="text-muted-foreground/50 text-xs mt-4">
          <Link to="/copyright" className="hover:text-muted-foreground transition-colors underline underline-offset-2">
            © {year} HABS Gliders · Website by Panshul Vempalli. All rights reserved.
          </Link>
        </p>
      </div>
    </footer>
  );
};

export default Footer;
