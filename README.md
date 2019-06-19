
# Submission checklist:
- [ ] 2) shooter1: copy of shooter0 (as is right now)
- [ ] 1) shooter0: then revert resource_manager in shooter0 ;-(
- [ ] 2) reduce number of dynamically bound calls to 1 (visitor pattern?)
- [ ] 2) add performance counters (ms per frame, ms per draw, ms per update)
- [ ] 3) shooter2: SAT: implement sat_collide()
- [ ] 3) shooter2: extend API as requested in the assignment ...
- [ ] 4) Quadtree<T>....

# TODO must-haves

- [x] fix cmake files
- [x] fix upstream compilation bugs
- [ ] grasp the code
- [x] fix memory leaks by using strong ownership
- [x] optimize image/canvas loading such that each canvas with the same image pixels use the same underlying resource
- [ ] improved collision manager

# TODO optionals
- [ ] use uniform initialization (wrt. vexing parses)
- [ ] use left-to-right reading for variable declarations (`auto const x = value;`)
- [ ] custom game object (ship for second player? alternative asteroids? hmm...)
