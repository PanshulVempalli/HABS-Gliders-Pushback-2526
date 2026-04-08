import { motion } from "framer-motion";
import logo from "@/assets/habs-gliders-logo.png";
import heroBg from "@/assets/hero-bg.jpg";

const HeroSection = () => {
  return (
    <section className="relative min-h-screen flex items-center justify-center overflow-hidden pb-24">
      {/* Background image */}
      <div className="absolute inset-0">
        <img
          src={heroBg}
          alt=""
          className="w-full h-full object-cover opacity-30"
        />
        <div className="absolute inset-0 bg-navy-gradient" />
      </div>

      <div className="relative z-10 container mx-auto px-4 text-center">
        <motion.div
          initial={{ opacity: 0, y: 30 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ duration: 0.8 }}
        >
          <img
            src={logo}
            alt="HABS Gliders and Haberdashers' Schools logos"
            className="mx-auto w-full max-w-2xl mb-8"
          />
        </motion.div>

        <motion.h1
          initial={{ opacity: 0, y: 20 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ duration: 0.8, delay: 0.3 }}
          className="font-display text-5xl md:text-7xl lg:text-8xl font-bold text-gradient-gold mb-4"
        >
          HABS GLIDERS: Journey to Worlds
        </motion.h1>

        <motion.p
          initial={{ opacity: 0, y: 20 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ duration: 0.8, delay: 0.5 }}
          className="font-display text-xl md:text-2xl text-gold tracking-[0.2em] mb-6"
        >
          TEAM 34071B · VEX V5 ROBOTICS
        </motion.p>

        <motion.p
          initial={{ opacity: 0, y: 20 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ duration: 0.8, delay: 0.7 }}
          className="text-foreground/80 text-lg md:text-xl max-w-2xl mx-auto mb-6 font-body"
        >
          The first mixed team from Haberdashers' Elstree Schools to qualify for the
          VEX Robotics World Championships. Join us as we share our story, achievements, and the path to St. Louis, Missouri, April 25th, 2026.
        </motion.p>

        <motion.p
          initial={{ opacity: 0, y: 20 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ duration: 0.8, delay: 0.85 }}
          className="text-foreground/40 text-sm mx-auto mb-10 font-body"
        >
          Created by team member{" "}
          <a
            href="#team"
            onClick={() => {
              setTimeout(() => {
                const el = document.getElementById("panshul-card");
                if (el) {
                  el.scrollIntoView({ behavior: "smooth", block: "center" });
                  el.classList.add("highlight-card");
                  setTimeout(() => el.classList.remove("highlight-card"), 2000);
                }
              }, 400);
            }}
            className="text-gold hover:text-gold/80 underline underline-offset-2 cursor-pointer transition-colors"
          >
            Panshul Vempalli
          </a>
        </motion.p>

        <motion.a
          href="#about"
          initial={{ opacity: 0 }}
          animate={{ opacity: 1 }}
          transition={{ duration: 0.8, delay: 1 }}
          className="inline-block bg-gold text-black px-8 py-3 font-display tracking-wider text-sm hover:bg-gold/90 transition-colors rounded-sm font-semibold"
        >
          EXPLORE OUR JOURNEY
        </motion.a>
      </div>
    </section>
  );
};

export default HeroSection;
