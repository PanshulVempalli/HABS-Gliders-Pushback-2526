import { motion, useInView } from "framer-motion";
import { useRef } from "react";
import { ArrowLeft, Target, Zap, Shield, Timer, Trophy, ParkingSquare, Square, ArrowUp, ArrowDown, Bot, Car, Package, RefreshCw } from "lucide-react";
import { Link } from "react-router-dom";
import fieldImg from "@/assets/pushback-field.jpg";
import actionImg from "@/assets/pushback-action.jpg";
import Navbar from "@/components/Navbar";
import Footer from "@/components/Footer";

const scoringData = [
  { category: "Each Block Scored", points: "3 pts", icon: Square },
  { category: "Controlled Zone (Long Goal)", points: "10 pts", icon: Target },
  { category: "Controlled Center Goal (Upper)", points: "8 pts", icon: ArrowUp },
  { category: "Controlled Center Goal (Lower)", points: "6 pts", icon: ArrowDown },
  { category: "1 Parked Robot", points: "8 pts", icon: ParkingSquare },
  { category: "2 Parked Alliance Robots", points: "30 pts", icon: Car },
  { category: "Autonomous Bonus", points: "10 pts", icon: Bot },
];

const phases = [
  {
    icon: Timer,
    title: "Autonomous Period",
    color: "text-gold",
    desc: "The match begins with a 15-second autonomous period where robots operate using only pre-programmed instructions - no driver input allowed. Scoring here earns the Autonomous Bonus (10 points), which can be decisive in close matches.",
    tips: [
      "Requires precision and consistency",
      "Pre-programmed routes using sensors (IMU, vision)",
      "High reward for early scoring advantage",
    ],
  },
  {
    icon: Zap,
    title: "Driver Control Period",
    color: "text-cream",
    desc: "The main phase of the match where drivers control their robots to score blocks into goals. Speed, cycle efficiency, and mechanical reliability are critical. Teams can also use match loaders to introduce additional blocks onto the field.",
    tips: [
      "Cycle speed determines scoring output",
      "Match loading can boost block count",
      "Defensive play can disrupt opponents",
    ],
  },
  {
    icon: ParkingSquare,
    title: "Endgame",
    color: "text-gold-bright",
    desc: "In the final seconds, robots attempt to park in designated zones for bonus points. Parking both alliance robots earns a massive 30-point bonus - often the difference between winning and losing.",
    tips: [
      "High-value bonus in limited time",
      "Requires quick repositioning",
      "2-robot park is a game changer (30 pts)",
    ],
  },
];

const PushBackPage = () => {
  const heroRef = useRef(null);
  const heroInView = useInView(heroRef, { once: true });
  const scoringRef = useRef(null);
  const scoringInView = useInView(scoringRef, { once: true, margin: "-50px" });
  const phasesRef = useRef(null);
  const phasesInView = useInView(phasesRef, { once: true, margin: "-50px" });
  const stratRef = useRef(null);
  const stratInView = useInView(stratRef, { once: true, margin: "-50px" });

  return (
    <div className="min-h-screen">
      <Navbar />

      {/* Hero */}
      <section className="relative pt-24 pb-16 overflow-hidden" ref={heroRef}>
        <div className="absolute inset-0">
          <img src={fieldImg} alt="" className="w-full h-full object-cover opacity-20" />
          <div className="absolute inset-0 bg-navy-gradient" />
        </div>
        <div className="container mx-auto px-4 relative z-10 pt-16">
          <Link
            to="/"
            className="inline-flex items-center gap-2 text-gold text-sm mb-8 hover:text-cream transition-colors"
          >
            <ArrowLeft size={16} /> Back to Home
          </Link>

          <motion.h1
            initial={{ opacity: 0, y: 20 }}
            animate={heroInView ? { opacity: 1, y: 0 } : {}}
            transition={{ duration: 0.7 }}
            className="font-display text-5xl md:text-7xl text-gradient-gold mb-4"
          >
            PUSH BACK
          </motion.h1>
          <motion.p
            initial={{ opacity: 0, y: 20 }}
            animate={heroInView ? { opacity: 1, y: 0 } : {}}
            transition={{ duration: 0.7, delay: 0.2 }}
            className="font-display text-xl text-gold tracking-[0.15em] mb-6"
          >
            VEX V5 ROBOTICS COMPETITION · 2025/26 SEASON GAME
          </motion.p>
          <motion.p
            initial={{ opacity: 0, y: 20 }}
            animate={heroInView ? { opacity: 1, y: 0 } : {}}
            transition={{ duration: 0.7, delay: 0.35 }}
            className="text-foreground/80 text-lg max-w-3xl leading-relaxed"
          >
            Push Back is the official VEX V5 Robotics Competition game for the 2025-2026 season.
            Two alliances of two robots each compete to score blocks into goals, control zones, and
            park for bonus points. Matches combine autonomous programming, driver skill, and
            strategic thinking in an intense, fast-paced format.
          </motion.p>
        </div>
      </section>

      {/* How it works visual */}
      <section className="py-20 bg-secondary">
        <div className="container mx-auto px-4">
          <div className="max-w-3xl mx-auto">
            <h2 className="font-display text-3xl md:text-4xl text-primary mb-6">HOW THE GAME WORKS</h2>
            <div className="space-y-4 text-foreground/75 leading-relaxed">
              <p>
                The field features <span className="text-gold font-semibold">long goals</span> on each side,
                a <span className="text-gold font-semibold">center goal</span> (with upper and lower scoring),
                and <span className="text-gold font-semibold">parking zones</span> in the corners.
              </p>
              <p>
                <span className="text-cream font-semibold">Blocks</span> are scattered across the field at the start.
                Robots must intake these blocks and score them into goals. Additional blocks can be introduced
                through <span className="text-gold font-semibold">match loading stations</span>.
              </p>
              <p>
                A key strategic element is <span className="text-cream font-semibold">zone control</span> -
                having more blocks on your side of a long goal earns 10 bonus points per controlled zone.
                This means simply scoring the most blocks isn't always the best strategy; you need to
                outscore your opponent in each individual zone.
              </p>
            </div>
          </div>
        </div>
      </section>

      {/* Scoring breakdown */}
      <section className="py-20" ref={scoringRef}>
        <div className="container mx-auto px-4">
          <motion.h2
            initial={{ opacity: 0 }}
            animate={scoringInView ? { opacity: 1 } : {}}
            transition={{ duration: 0.6 }}
            className="font-display text-3xl md:text-4xl text-primary text-center mb-4"
          >
            SCORING BREAKDOWN
          </motion.h2>
          <motion.p
            initial={{ opacity: 0 }}
            animate={scoringInView ? { opacity: 1 } : {}}
            transition={{ duration: 0.6, delay: 0.1 }}
            className="text-muted-foreground text-center mb-12 max-w-xl mx-auto"
          >
            Understanding every point opportunity is critical for competitive success.
          </motion.p>

          <div className="max-w-2xl mx-auto">
            {scoringData.map((item, i) => (
              <motion.div
                key={item.category}
                initial={{ opacity: 0, x: -20 }}
                animate={scoringInView ? { opacity: 1, x: 0 } : {}}
                transition={{ duration: 0.4, delay: 0.1 * i }}
                className="flex items-center justify-between py-4 border-b border-border last:border-b-0"
              >
                <div className="flex items-center gap-4">
                  <div className="w-9 h-9 rounded-lg bg-gold/10 flex items-center justify-center shrink-0">
                    <item.icon className="text-gold" size={18} />
                  </div>
                  <span className="text-foreground/80 font-medium">{item.category}</span>
                </div>
                <span className="font-display text-gold text-lg">{item.points}</span>
              </motion.div>
            ))}
          </div>
        </div>
      </section>

      {/* Match phases */}
      <section className="py-20 bg-secondary" ref={phasesRef}>
        <div className="container mx-auto px-4">
          <motion.h2
            initial={{ opacity: 0 }}
            animate={phasesInView ? { opacity: 1 } : {}}
            transition={{ duration: 0.6 }}
            className="font-display text-3xl md:text-4xl text-primary text-center mb-16"
          >
            MATCH PHASES
          </motion.h2>

          <div className="grid grid-cols-1 md:grid-cols-3 gap-8 max-w-5xl mx-auto">
            {phases.map((phase, i) => (
              <motion.div
                key={phase.title}
                initial={{ opacity: 0, y: 30 }}
                animate={phasesInView ? { opacity: 1, y: 0 } : {}}
                transition={{ duration: 0.5, delay: 0.2 * i }}
                className="bg-card border-gradient-gold rounded-lg p-8"
              >
                <phase.icon className={`${phase.color} mb-4`} size={32} />
                <h3 className="font-display text-xl text-primary mb-3">{phase.title}</h3>
                <p className="text-foreground/60 text-sm leading-relaxed mb-4">{phase.desc}</p>
                <div className="space-y-2">
                  {phase.tips.map((tip) => (
                    <div key={tip} className="flex items-start gap-2">
                      <div className="w-1.5 h-1.5 rounded-full bg-gold mt-1.5 shrink-0" />
                      <span className="text-foreground/50 text-xs">{tip}</span>
                    </div>
                  ))}
                </div>
              </motion.div>
            ))}
          </div>
        </div>
      </section>

      {/* Our strategy */}
      <section className="py-20" ref={stratRef}>
        <div className="container mx-auto px-4">
          <motion.h2
            initial={{ opacity: 0 }}
            animate={stratInView ? { opacity: 1 } : {}}
            transition={{ duration: 0.6 }}
            className="font-display text-3xl md:text-4xl text-primary text-center mb-6"
          >
            OUR STRATEGIC APPROACH
          </motion.h2>
          <motion.p
            initial={{ opacity: 0 }}
            animate={stratInView ? { opacity: 1 } : {}}
            transition={{ duration: 0.6, delay: 0.1 }}
            className="text-muted-foreground text-center mb-12 max-w-2xl mx-auto"
          >
            From our game analysis, we identified the key principles that drive our competitive strategy.
          </motion.p>

          <div className="grid grid-cols-1 sm:grid-cols-2 gap-6 max-w-4xl mx-auto">
            {[
              {
                icon: RefreshCw,
                title: "Cycle Speed Over Complexity",
                desc: "Reliable, fast scoring cycles consistently outperform complex mechanisms. We prioritised repeatable intake-score-return loops.",
              },
              {
                icon: Target,
                title: "Zone Control Strategy",
                desc: "Rather than just scoring volume, we focus on controlling zones in long goals - scoring 3 on each side to guarantee the 10-point control bonus.",
              },
              {
                icon: Shield,
                title: "Defensive Resilience",
                desc: "Our 6-motor drivetrain and RiGuan architecture provide the pushing power and stability to maintain position under defensive pressure.",
              },
              {
                icon: Bot,
                title: "Autonomous Advantage",
                desc: "With PID control, vision sensors, and 250+ VR simulation runs, our autonomous routines are built for consistency and early-match advantage.",
              },
              {
                icon: Package,
                title: "Match Loading Efficiency",
                desc: "Our piston-actuated match loader with synchronised gears ensures rapid, reliable block introduction with minimal alignment time.",
              },
              {
                icon: ParkingSquare,
                title: "Endgame Reliability",
                desc: "We plan for the 30-point double park - our autonomous routines account for final positioning to maximise endgame scoring.",
              },
            ].map((item, i) => (
              <motion.div
                key={item.title}
                initial={{ opacity: 0, y: 20 }}
                animate={stratInView ? { opacity: 1, y: 0 } : {}}
                transition={{ duration: 0.5, delay: 0.1 * i }}
                className="bg-card border-gradient-gold rounded-lg p-6"
              >
                <div className="w-10 h-10 rounded-lg bg-gold/10 flex items-center justify-center mb-4">
                  <item.icon className="text-gold" size={20} />
                </div>
                <h3 className="font-display text-lg text-primary mb-2">{item.title}</h3>
                <p className="text-foreground/60 text-sm leading-relaxed">{item.desc}</p>
              </motion.div>
            ))}
          </div>
        </div>
      </section>

      {/* Key conclusions */}
      <section className="py-16 bg-secondary">
        <div className="container mx-auto px-4 text-center">
          <div className="max-w-3xl mx-auto bg-card border-gradient-gold rounded-lg p-10">
            <Trophy className="mx-auto text-gold mb-4" size={40} />
            <h3 className="font-display text-2xl text-primary mb-4">KEY CONCLUSION</h3>
            <p className="text-foreground/70 leading-relaxed italic">
              "Reliability would outperform complexity. Speed of repeatable cycles would outweigh
              high-risk mechanisms. Stable drivetrain control was foundational to all scoring methods.
              Intake consistency was more important than advanced manipulation. Early autonomous
              reliability could create competitive advantage."
            </p>
            <p className="text-gold text-sm mt-4 font-display tracking-wider">
              - FROM OUR GAME ANALYSIS
            </p>
          </div>
        </div>
      </section>

      <Footer />
    </div>
  );
};

export default PushBackPage;
