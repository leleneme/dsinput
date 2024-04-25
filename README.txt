======== DS(i)nput - Make the DS(i) be a wireless controller for Linux ========

This project makes use of a server running on Linux that receives buttons from
the Nintendo DS(i) via UDP. On Linux, the controller is simulated using uinput
(requires kernel 2.6.20 or later, as far as I know).

For the DS client, it is required to have devkitPro (https://devkitpro.org/) and
dsiwifi (https://github.com/shinyquagsire23/dsiwifi/) installed and properly co-
nfigured.

The project is licensed under the MIT License, found in `LICENSE` at the root
of the repository.