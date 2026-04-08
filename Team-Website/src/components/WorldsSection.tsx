import { motion, useInView } from "framer-motion";
import { useEffect, useMemo, useRef, useState } from "react";
import { MapPin, Calendar, Target } from "lucide-react";

const getCountdown = (targetDate: Date) => {
  const now = new Date().getTime();
  const distance = targetDate.getTime() - now;

  if (distance <= 0) {
    return { days: 0, hours: 0, minutes: 0, seconds: 0, completed: true };
  }

  const days = Math.floor(distance / (1000 * 60 * 60 * 24));
  const hours = Math.floor((distance % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60));
  const minutes = Math.floor((distance % (1000 * 60 * 60)) / (1000 * 60));
  const seconds = Math.floor((distance % (1000 * 60)) / 1000);

  return { days, hours, minutes, seconds, completed: false };
};

const WorldsSection = () => {
  const ref = useRef(null);
  const inView = useInView(ref, { once: true, margin: "-50px" });
  const targetDate = useMemo(() => new Date("2026-04-25T00:00:00Z"), []);
  const [countdown, setCountdown] = useState(() => getCountdown(targetDate));

  useEffect(() => {
    const timer = setInterval(() => {
      setCountdown(getCountdown(targetDate));
    }, 1000);

    return () => clearInterval(timer);
  }, [targetDate]);

  return (
    <section id="worlds" className="py-24 relative overflow-hidden" ref={ref}>
      {/* Decorative gear */}
      <div className="absolute -right-20 -top-20 w-64 h-64 border border-gold/10 rounded-full animate-gear-spin opacity-20" />
      <div className="absolute -left-16 -bottom-16 w-48 h-48 border border-gold/10 rounded-full animate-gear-spin opacity-15" />

      <div className="container mx-auto px-4 relative z-10">
        <motion.h2
          initial={{ opacity: 0 }}
          animate={inView ? { opacity: 1 } : {}}
          transition={{ duration: 0.6 }}
          className="font-display text-4xl md:text-5xl text-gradient-gold text-center mb-6"
        >
          WORLD CHAMPIONSHIPS
        </motion.h2>

        <motion.p
          initial={{ opacity: 0 }}
          animate={inView ? { opacity: 1 } : {}}
          transition={{ duration: 0.6, delay: 0.15 }}
          className="text-foreground/70 text-lg text-center max-w-2xl mx-auto mb-6"
        >
          For the first time in our schools' history, a joint team from Habs Boys and Habs Girls is heading to the VEX Robotics World Championships as the first team from Habs Girls' to qualify. We're ready to represent Haberdashers' Elstree Schools on the global stage.
        </motion.p>

        <motion.div
          initial={{ opacity: 0, y: 20 }}
          animate={inView ? { opacity: 1, y: 0 } : {}}
          transition={{ duration: 0.6, delay: 0.3 }}
          className="flex flex-wrap items-center justify-center gap-6 max-w-3xl mx-auto mb-10"
        >
          <div className="grid grid-cols-4 gap-3 text-center text-foreground/90">
            {[
              { label: "Days", value: countdown.days },
              { label: "Hours", value: countdown.hours },
              { label: "Minutes", value: countdown.minutes },
              { label: "Seconds", value: countdown.seconds },
            ].map((item) => (
              <div key={item.label} className="bg-card border border-gold/20 rounded-lg p-3 min-w-[70px]">
                <div className="font-display text-2xl text-gold">{item.value}</div>
                <div className="text-xs uppercase tracking-wider text-muted-foreground">{item.label}</div>
              </div>
            ))}
          </div>
          <div className="text-center text-sm text-foreground/80">Until launch day (April 25, 2026)</div>
        </motion.div>

        <motion.div
          initial={{ opacity: 0, y: 20 }}
          animate={inView ? { opacity: 1, y: 0 } : {}}
          transition={{ duration: 0.6, delay: 0.45 }}
          className="max-w-3xl mx-auto rounded-lg bg-card border border-gold/30 p-8 shadow-lg mb-12"
        >
          <p className="font-display text-lg text-gold text-center">
            Proud to be representing the UK on the Worlds stage
          </p>
        </motion.div>

        <div className="flex flex-col md:flex-row gap-8 justify-center items-center max-w-3xl mx-auto mb-16">
          {[
            { icon: MapPin, label: "St. Louis, Missouri", sub: "USA" },
            { icon: Calendar, label: "April 25th, 2026", sub: "World Championship" },
            { icon: Target, label: "Push Back", sub: "VEX V5 Game" },
          ].map((item, i) => (
            <motion.div
              key={item.label}
              initial={{ opacity: 0, y: 20 }}
              animate={inView ? { opacity: 1, y: 0 } : {}}
              transition={{ duration: 0.5, delay: 0.3 + i * 0.1 }}
              className="flex items-center gap-4 bg-card border-gradient-gold rounded-lg px-8 py-5 w-full md:w-auto"
            >
              <item.icon className="text-gold shrink-0" size={28} />
              <div>
                <div className="font-display text-primary text-lg">{item.label}</div>
                <div className="text-muted-foreground text-xs uppercase tracking-wider">{item.sub}</div>
              </div>
            </motion.div>
          ))}
        </div>

        <motion.div
          initial={{ opacity: 0, y: 20 }}
          animate={inView ? { opacity: 1, y: 0 } : {}}
          transition={{ duration: 0.6, delay: 0.7 }}
          className="max-w-3xl mx-auto bg-card border-gradient-gold rounded-lg p-8"
        >
          <h3 className="font-display text-2xl text-primary mb-4 text-center">
            WORLDS PREP UPGRADES
          </h3>
          <div className="grid grid-cols-1 sm:grid-cols-2 gap-4">
            {[
              "Narrowed drivetrain channel to prevent cube jams",
              "Implemented PID control for precision autonomous",
              "Added dual vision sensors for alignment",
              "Migrated codebase from Python to C++ (EZ-Template)",
              "Upgraded to 2\" black flex wheel intake rollers",
              "Built HABS Gliders Hub App for match strategy",
            ].map((item) => (
              <div key={item} className="flex items-start gap-3">
                <div className="w-1.5 h-1.5 rounded-full bg-gold mt-2 shrink-0" />
                <span className="text-foreground/70 text-sm">{item}</span>
              </div>
            ))}
          </div>
        </motion.div>
      </div>
    </section>
  );
};

export default WorldsSection;
