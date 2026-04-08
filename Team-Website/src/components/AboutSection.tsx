import { motion } from "framer-motion";
import { useInView } from "framer-motion";
import { useRef } from "react";
import { Cog, Users, Trophy } from "lucide-react";

const stats = [
  { icon: Cog, label: "Engineering Cycles", value: "20+" },
  { icon: Users, label: "Team Members", value: "6" },
  { icon: Trophy, label: "UK Ranking", value: "#1" },
];

const AboutSection = () => {
  const ref = useRef(null);
  const inView = useInView(ref, { once: true, margin: "-100px" });

  return (
    <section id="about" className="py-24 bg-secondary" ref={ref}>
      <div className="container mx-auto px-4">
        <motion.div
          initial={{ opacity: 0, y: 30 }}
          animate={inView ? { opacity: 1, y: 0 } : {}}
          transition={{ duration: 0.6 }}
          className="max-w-3xl mx-auto text-center mb-16"
        >
          <h2 className="font-display text-4xl md:text-5xl text-primary mb-6">
            WHO WE ARE
          </h2>
          <p className="text-foreground/80 text-lg leading-relaxed">
            We are HABS Gliders, a Year 10 mixed team from Haberdashers' Boys' and Girls' School
            competing in VEX V5 Robotics. This season marks our first time competing together as a 
            joint team between the two schools, making this both a technical and collaborative milestone.
          </p>
          <p className="text-foreground/80 text-lg leading-relaxed mt-4">
            Ranked <span className="text-gold font-semibold">1st in the UK</span> in VEX VR for both middle and 
            high school and <span className="text-gold font-semibold">12th in the world</span> in middle 
            school, we are now preparing for the World Championships in St. Louis, Missouri, the first mixed team
            from our schools to ever qualify.
          </p>
        </motion.div>

        <div className="grid grid-cols-1 md:grid-cols-3 gap-8 max-w-4xl mx-auto">
          {stats.map((stat, i) => (
            <motion.div
              key={stat.label}
              initial={{ opacity: 0, y: 30 }}
              animate={inView ? { opacity: 1, y: 0 } : {}}
              transition={{ duration: 0.6, delay: 0.2 + i * 0.15 }}
              className="bg-card border-gradient-gold rounded-lg p-8 text-center"
            >
              <stat.icon className="mx-auto mb-4 text-gold" size={32} />
              <div className="font-display text-4xl text-primary mb-2">{stat.value}</div>
              <div className="text-muted-foreground text-sm uppercase tracking-wider">
                {stat.label}
              </div>
            </motion.div>
          ))}
        </div>
      </div>
    </section>
  );
};

export default AboutSection;
