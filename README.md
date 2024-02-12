Seam carving is a method to scale the aspect ratio of images that aims to keep the proportions and shape of relevant contents and tries to reduce distortion. It was originally presented by Shai Avidan at Mitsubishi Electric Research Labs (MERL) and Ariel Shamir (Interdisciplinary Center and MERL) at the computer graphics conference SIG- GRAPH 2007. An in-depth description of the algorithm, image and video examples as well as further use cases and extensions can be found on the website of the inventors:
http://www.faculty.idc.ac.il/arik/SCWeb/imret/.

The algorithm generates a dispensability score for all areas/pixels of an image. Using this score, paths along the most dispensable pixels are searched that lie crosswise to the compressed or elongated direction. At these seams, pixels are added or removed. Different variants of the base algorithm usually distinguish themselves by the strategies they use to calculate the dispensability score. In the base version, we are interested in, it is done by considering only the difference between the color values of neighboring pixels. We call this difference the energy. Pixels with the least energy are the most dispensable ones. We also treat only the downsizing case.
Essentially, the algorithm works as follows:
• Calculate the local energy of every pixel
• Calculate the energy of all adjacent vertical pixel-paths • Find the pixel-paths with minimal energy
• Remove the pixels of this path from the image


You have to install the image viewer nomacs (or
any image viewer of your choice) by executing this command:
sudo pacman -S --noconfirm nomacs
You can now inspect the generated image out.ppm or the test files (e.g. test/data/small2.ppm):
nomacs output.ppm
If you want to inspect specific pixels you can press ctrl+I (or select Panels → Toolbars → Statusbar) to show the
statusbar, which tells you about the coordinate and RGB values of the pixel at the position of your cursor.
