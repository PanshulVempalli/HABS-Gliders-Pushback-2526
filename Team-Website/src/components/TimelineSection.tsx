import { motion, useInView } from "framer-motion";
import { useRef } from "react";

const events = [
  {
    date: "September 2025",
    title: "Season Kickoff",
    desc: "Conducted full game analysis of Push Back, began drivetrain prototyping, assigned team roles, and started our engineering notebook.",
  },
  {
    date: "October 2025",
    title: "First Build & Competitions",
    desc: "Built our 6-motor hybrid drivetrain with 2.75\" traction wheels, developed outtake and intake systems, and competed in our first regional event.",
  },
  {
    date: "November 2025",
    title: "Major Robot Redesign",
    desc: "Transitioned to the RiGuan architecture after evaluating C-Bot, S-Bot, and Basket-Bot designs. Won league matches and improved scoring efficiency.",
  },
  {
    date: "January 2026",
    title: "Match Loader & Skills Strategy",
    desc: "Developed our unique match loader mechanism, created skills strategy, and ran 250+ VR simulations scoring up to 94 points.",
  },
  {
    date: "February 2026",
    title: "UK Nationals Semifinalists",
    desc: "Advanced to the semifinals at the National Championship. Identified key reliability improvements needed for our intake and match loader systems.",
  },
  {
    date: "March 2026",
    title: "Worlds Preparation",
    desc: "Thinned drivetrain to prevent cube jams, implemented PID control, integrated vision sensors, switched to EZ-Template in C++, and built the HABS Gliders Hub App.",
  },
];

const TimelineSection = () => {
  const ref = useRef(null);
  const inView = useInView(ref, { once: true, margin: "-50px" });

  return (
    <section id="journey" className="py-24" ref={ref}>
      <div className="container mx-auto px-4">
        <motion.h2
          initial={{ opacity: 0 }}
          animate={inView ? { opacity: 1 } : {}}
          transition={{ duration: 0.6 }}
          className="font-display text-4xl md:text-5xl text-primary text-center mb-16"
        >
          OUR JOURNEY
        </motion.h2>

        <div className="relative max-w-3xl mx-auto">
          {/* Vertical line */}
          <div className="absolute left-4 md:left-1/2 top-0 bottom-0 w-px bg-gold/30 -translate-x-1/2" />

          {events.map((event, i) => {
            const isLeft = i % 2 === 0;
            return (
              <motion.div
                key={event.date}
                initial={{ opacity: 0, x: isLeft ? -30 : 30 }}
                animate={inView ? { opacity: 1, x: 0 } : {}}
                transition={{ duration: 0.5, delay: i * 0.15 }}
                className={`relative flex mb-12 ${
                  isLeft ? "md:flex-row" : "md:flex-row-reverse"
                } flex-row`}
              >
                {/* Dot */}
                <div className="absolute left-4 md:left-1/2 w-3 h-3 rounded-full bg-gold -translate-x-1/2 mt-2 z-10" />

                <div
                  className={`ml-10 md:ml-0 md:w-1/2 ${
                    isLeft ? "md:pr-12 md:text-right" : "md:pl-12"
                  }`}
                >
                  <span className="font-display text-gold text-sm tracking-wider">
                    {event.date}
                  </span>
                  <h3 className="font-display text-xl text-primary mt-1 mb-2">
                    {event.title}
                  </h3>
                  <p className="text-foreground/70 text-sm leading-relaxed">
                    {event.desc}
                  </p>
                </div>
              </motion.div>
            );
          })}
        </div>
      </div>
    </section>
  );
};

export default TimelineSection;
