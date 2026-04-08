import { motion, useInView } from "framer-motion";
import { useRef } from "react";
import { Award, Globe, Star, Smartphone } from "lucide-react";

const achievements = [
  {
    icon: Globe,
    title: "#1 in the UK",
    desc: "Ranked 1st in the UK for both middle and high school in VEX VR.",
  },
  {
    icon: Star,
    title: "#12 in the World",
    desc: "Ranked 12th globally in VEX VR Middle School division.",
  },
  {
    icon: Award,
    title: "National Semifinalists",
    desc: "Advanced to the semifinals at the UK VEX V5 National Championship 2026.",
  },
  {
    icon: Award,
    title: "Judges Award",
    desc: "Awarded Judges Award at the League of Legends Regional Event.",
  },
];

const AchievementsSection = () => {
  const ref = useRef(null);
  const inView = useInView(ref, { once: true, margin: "-50px" });

  return (
    <section id="achievements" className="py-24 bg-secondary" ref={ref}>
      <div className="container mx-auto px-4">
        <motion.h2
          initial={{ opacity: 0 }}
          animate={inView ? { opacity: 1 } : {}}
          transition={{ duration: 0.6 }}
          className="font-display text-4xl md:text-5xl text-primary text-center mb-16"
        >
          ACHIEVEMENTS
        </motion.h2>

        <div className="grid grid-cols-1 sm:grid-cols-2 gap-8 max-w-4xl mx-auto">
          {achievements.map((a, i) => (
            <motion.div
              key={a.title}
              initial={{ opacity: 0, scale: 0.95 }}
              animate={inView ? { opacity: 1, scale: 1 } : {}}
              transition={{ duration: 0.5, delay: 0.15 * i }}
              className="bg-card border-gradient-gold rounded-lg p-8 text-center hover:glow-gold transition-shadow duration-300"
            >
              <a.icon className="mx-auto mb-4 text-gold" size={36} />
              <h3 className="font-display text-xl text-primary mb-2 font-semibold">{a.title}</h3>
              <p className="text-foreground/50 text-sm">{a.desc}</p>
            </motion.div>
          ))}
        </div>

        {/* VR Skills highlight */}
        <motion.div
          initial={{ opacity: 0, y: 20 }}
          animate={inView ? { opacity: 1, y: 0 } : {}}
          transition={{ duration: 0.6, delay: 0.7 }}
          className="mt-12 text-center"
        >
          <div className="inline-block bg-card border-gradient-gold rounded-lg px-10 py-6">
            <div className="font-display text-5xl text-gradient-gold mb-2">95</div>
            <div className="text-muted-foreground text-sm uppercase tracking-wider">
              VR Skills High Score · 250+ Simulated Runs
            </div>
          </div>
        </motion.div>
      </div>
    </section>
  );
};

export default AchievementsSection;
