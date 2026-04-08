import Navbar from "@/components/Navbar";
import HeroSection from "@/components/HeroSection";
import AboutSection from "@/components/AboutSection";
import TimelineSection from "@/components/TimelineSection";
import TeamSection from "@/components/TeamSection";
import RobotSection from "@/components/RobotSection";
import AchievementsSection from "@/components/AchievementsSection";
import WorldsSection from "@/components/WorldsSection";
import Footer from "@/components/Footer";

const Index = () => {
  return (
    <div className="min-h-screen">
      <Navbar />
      <HeroSection />
      <AboutSection />
      <TimelineSection />
      <TeamSection />
      <RobotSection />
      <AchievementsSection />
      <WorldsSection />
      <Footer />
    </div>
  );
};

export default Index;
