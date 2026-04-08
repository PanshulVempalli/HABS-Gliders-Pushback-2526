import { motion, useInView } from "framer-motion";
import { useRef } from "react";

const members = [
  {
    name: "Panshul",
    role: "Programming & Strategy Lead",
    desc: "First year in VEX. Strengths in coding:Our driver and autonomous code and VEX Vr which is currenlty the highest in the UK . Hands-on experience through Greenpower. Roles: Lead Programmer and Strategist, documenting in the notebook and game engineer.Creator of this website ",
  },
  {
    name: "Kayan",
    role: "Driver & Programmer",
    desc: "Led a VEX IQ team in Year 8, strong coder taking CS IGCSE early. Extensive hands-on experience through Greenpower competition, leading the HABS PowerStrike team focusing on design and performance.",
  },
  {
    name: "Joshua",
    role: "Build Lead & CAD Designer",
    desc: "First year competing in VEX. Strengths in efficiency and CAD expertise. Main focus is design and mechanical assembly.",
  },
  {
    name: "Asha P.",
    role: "Builder & Online Challenges",
    desc: "Three years of VEX experience (Years 7-9) as a builder. Expert problem-solving skills honed through consistent building experience. Primary focus on mechanical assembly, secondary on online skills.",
  },
  {
    name: "Tasnim",
    role: "Notebook Lead & Designer",
    desc: "Competed in VEX IQ Year 9, reached two regional competitions. Strengths in creativity, art skills, and organization. Primary focus on documentation and design phase support.",
  },
  {
    name: "Asha R.",
    role: "Builder & Skills",
    desc: "Competed in VEX Year 8 and Year 9. Expert at replicating builds quickly and effectively from reference pictures or CAD models. Exceptional building skills.",
  },
];

const TeamSection = () => {
  const ref = useRef(null);
  const inView = useInView(ref, { once: true, margin: "-50px" });

  return (
    <section id="team" className="py-24 bg-secondary" ref={ref}>
      <div className="container mx-auto px-4">
        <motion.h2
          initial={{ opacity: 0 }}
          animate={inView ? { opacity: 1 } : {}}
          transition={{ duration: 0.6 }}
          className="font-display text-4xl md:text-5xl text-primary text-center mb-4"
        >
          MEET THE TEAM
        </motion.h2>
        <motion.p
          initial={{ opacity: 0 }}
          animate={inView ? { opacity: 1 } : {}}
          transition={{ duration: 0.6, delay: 0.1 }}
          className="text-muted-foreground text-center mb-16 max-w-xl mx-auto"
        >
          A mixed Year 10 team from Haberdashers' Boys' and Girls' School, united by a passion for robotics.
        </motion.p>

        <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 gap-6 max-w-5xl mx-auto">
          {members.map((m, i) => (
            <motion.div
              key={m.name}
              id={m.name === "Panshul" ? "panshul-card" : undefined}
              initial={{ opacity: 0, y: 20 }}
              animate={inView ? { opacity: 1, y: 0 } : {}}
              transition={{ duration: 0.5, delay: 0.1 + i * 0.1 }}
              className="bg-card border-gradient-gold rounded-lg p-6 hover:glow-gold transition-shadow duration-300"
            >
              <div className="w-12 h-12 rounded-full bg-gold/20 flex items-center justify-center mb-4">
                <span className="font-display text-gold text-lg">
                  {m.name.split(" ").map((n) => n[0]).join("")}
                </span>
              </div>
              <h3 className="font-display text-lg text-primary">{m.name}</h3>
              <p className="text-gold text-xs uppercase tracking-wider mb-3">{m.role}</p>
              <p className="text-foreground/60 text-sm leading-relaxed">{m.desc}</p>
            </motion.div>
          ))}
        </div>
      </div>
    </section>
  );
};

export default TeamSection;
