import { motion, useInView } from "framer-motion";
import { useRef } from "react";
import { Wrench, Zap, Eye, Cpu } from "lucide-react";

const features = [
  {
    icon: Wrench,
    title: "6-Motor Drivetrain",
    desc: "Hybrid drop-centre configuration with 2.75\" traction wheels in the centre and omni wheels front/back for optimal balance of power and manoeuvrability.",
  },
  {
    icon: Zap,
    title: "RiGuan Architecture",
    desc: "Selected after evaluating C-Bot, S-Bot, and Basket-Bot designs. Scored highest on weighted criteria for speed, stability, and defensive resilience.",
  },
  {
    icon: Eye,
    title: "Dual Vision Sensors",
    desc: "Front sensor for intake alignment, rear sensor for goal scoring alignment. Combined with bumper switch for contact-based confirmation.",
  },
  {
    icon: Cpu,
    title: "PID & EZ-Template",
    desc: "Precision autonomous control with tuned PID constants. Migrated from Python to C++ using EZ-Template for voltage-based motor control.",
  },
];

const RobotSection = () => {
  const ref = useRef(null);
  const inView = useInView(ref, { once: true, margin: "-50px" });

  return (
    <section id="robot" className="py-24" ref={ref}>
      <div className="container mx-auto px-4">
        <motion.h2
          initial={{ opacity: 0 }}
          animate={inView ? { opacity: 1 } : {}}
          transition={{ duration: 0.6 }}
          className="font-display text-4xl md:text-5xl text-primary text-center mb-4"
        >
          THE ROBOT
        </motion.h2>
        <motion.p
          initial={{ opacity: 0 }}
          animate={inView ? { opacity: 1 } : {}}
          transition={{ duration: 0.6, delay: 0.1 }}
          className="text-muted-foreground text-center mb-16 max-w-2xl mx-auto"
        >
          Engineered through 20+ design cycles of iterative testing, our robot is built to compete at the highest level in Push Back.
        </motion.p>

        <div className="grid grid-cols-1 md:grid-cols-2 gap-8 max-w-4xl mx-auto">
          {features.map((f, i) => (
            <motion.div
              key={f.title}
              initial={{ opacity: 0, y: 20 }}
              animate={inView ? { opacity: 1, y: 0 } : {}}
              transition={{ duration: 0.5, delay: 0.15 * i }}
              className="flex gap-5 bg-card border-gradient-gold rounded-lg p-6"
            >
              <div className="shrink-0 w-12 h-12 rounded-lg bg-gold/10 flex items-center justify-center">
                <f.icon className="text-gold" size={24} />
              </div>
              <div>
                <h3 className="font-display text-lg text-primary mb-2">{f.title}</h3>
                <p className="text-foreground/60 text-sm leading-relaxed">{f.desc}</p>
              </div>
            </motion.div>
          ))}
        </div>

        {/* Key subsystems */}
        <motion.div
          initial={{ opacity: 0, y: 20 }}
          animate={inView ? { opacity: 1, y: 0 } : {}}
          transition={{ duration: 0.6, delay: 0.7 }}
          className="mt-16 max-w-4xl mx-auto bg-card border-gradient-gold rounded-lg p-8"
        >
          <h3 className="font-display text-2xl text-primary mb-6 text-center">KEY SUBSYSTEMS</h3>
          <div className="grid grid-cols-1 sm:grid-cols-3 gap-6 text-center">
            {[
              { label: "Intake", detail: "2\" black flex wheels with anti-slip mats, green motor for reliable torque" },
              { label: "Outtake", detail: "Flywheel-based scoring with compression funneling for fast, jam-free cycles" },
              { label: "Match Loader", detail: "Piston-actuated with synchronised gears on both sides to prevent skewing" },
            ].map((s) => (
              <div key={s.label}>
                <div className="font-display text-gold text-lg mb-2">{s.label}</div>
                <p className="text-foreground/60 text-sm">{s.detail}</p>
              </div>
            ))}
          </div>
        </motion.div>
      </div>
    </section>
  );
};

export default RobotSection;
