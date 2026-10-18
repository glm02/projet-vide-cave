# Skill `c-pic-iut-geii`

Skill pour Claude : coder en C pour PIC (MPLAB X / XC8) **comme le demandent les enseignants du département GEII de l'IUT Lyon 1** (modules INFO2, INFO3 et SAE).

- N'utilise que les notions vues en cours (E/S, masques, Timer2, interruptions IOC, Timer0, EUSART, SPI, PWM, CAN, LCD).
- Reprend les squelettes de programme et le style des sujets de TP (INFO2 registres directs, INFO3 MCC, SAE22 `libsae22`).
- Applique les bonnes pratiques du cours : plus petit type, flags remis à zéro le plus tôt possible, ISR courtes, variables partagées `volatile` + section critique, lectures/écritures multi-bits par masques.
- Mode pédagogique par défaut : indices, questions et squelettes `// à compléter` plutôt que la solution complète.

## Installation

Copier le dossier contenant `SKILL.md` dans le dossier de skills de Claude (par exemple `~/.claude/skills/c-pic-iut-geii/`), ou l'ajouter depuis les réglages de skills sur claude.ai.
