import { Link } from "react-router-dom";
import { ArrowLeft } from "lucide-react";

const CopyrightPage = () => {
  const year = new Date().getFullYear();

  return (
    <div className="min-h-screen bg-background text-foreground px-6 py-16 max-w-3xl mx-auto">
      <Link to="/" className="inline-flex items-center gap-2 text-gold text-sm mb-8 hover:text-gold/80 transition-colors">
        <ArrowLeft size={16} /> Back to Home
      </Link>
      <h1 className="font-display text-4xl text-primary mb-2">COPYRIGHT NOTICE</h1>
      <p className="text-muted-foreground text-sm mb-10">Last updated: April 2026</p>

      <section className="mb-8">
        <h2 className="font-display text-xl text-primary mb-3">Website Ownership</h2>
        <p className="text-foreground/80 leading-relaxed">
          This website (<strong>habs-gliders-34071b.vercel.app</strong>) was designed and developed by{" "}
          <strong>Panshul Vempalli</strong>, a member of HABS Gliders (Team 34071B), competing in the VEX V5
          Robotics World Championships 2026. All website design, code, layout, and structure is the intellectual
          property of Panshul Vempalli © {year}.
        </p>
      </section>

      <section className="mb-8">
        <h2 className="font-display text-xl text-primary mb-3">Team Content</h2>
        <p className="text-foreground/80 leading-relaxed">
          All content relating to HABS Gliders (Team 34071B) — including team bios, achievements, engineering
          descriptions, strategy documentation, and journey narrative — is the collective property of HABS Gliders
          and Haberdashers' Elstree Schools © {year}. This content may not be reproduced, copied, or distributed
          without prior written permission.
        </p>
      </section>

      <section className="mb-8">
        <h2 className="font-display text-xl text-primary mb-3">Images & Media</h2>
        <p className="text-foreground/80 leading-relaxed">
          All images used on this website are either owned by HABS Gliders, sourced from royalty-free providers,
          or used with appropriate attribution. Unauthorised reproduction of any image or media from this site is
          prohibited.
        </p>
      </section>

      <section className="mb-8">
        <h2 className="font-display text-xl text-primary mb-3">Third-Party References</h2>
        <p className="text-foreground/80 leading-relaxed">
          VEX Robotics, VEX V5, Push Back, and related names are trademarks of Innovation First International (IFI).
          This website is an independent team site and is not affiliated with or endorsed by VEX Robotics or IFI.
          Haberdashers' Elstree Schools is referenced for identification purposes only.
        </p>
      </section>

      <section className="mb-8">
        <h2 className="font-display text-xl text-primary mb-3">Permitted Use</h2>
        <p className="text-foreground/80 leading-relaxed">
          You may view and share links to this website freely. You may not copy, reproduce, republish, scrape,
          or distribute any content from this site for commercial purposes without express written consent from
          Panshul Vempalli solely.
        </p>
      </section>

      <section className="mb-8">
        <h2 className="font-display text-xl text-primary mb-3">Contact</h2>
        <p className="text-foreground/80 leading-relaxed">
          For copyright enquiries, permissions, or any questions relating to this website or its content
          (including all team members of HABS Gliders), please contact Panshul Vempalli directly at{" "}
          <a
            href="mailto:panshulvempalli@gmail.com"
            className="text-gold underline underline-offset-2 hover:text-gold/80 transition-colors"
          >
            panshulvempalli@gmail.com
          </a>
          . All enquiries on behalf of any HABS Gliders team member should be directed to this address.
        </p>
      </section>

      <p className="text-muted-foreground/50 text-xs mt-12 border-t border-border pt-6">
        © {year} HABS Gliders · Website by Panshul Vempalli · All rights reserved.
      </p>
    </div>
  );
};

export default CopyrightPage;
