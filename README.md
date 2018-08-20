# RocketKinematics

Status update: 

Winter 2017

I've completed the Arduino code needed for this project. (Of course, interested and able students - and anyone else - are *very* welcome to replace or improve it!)

I've completed and bench-tested the latest rev of the electronics. I'm pretty confident that we can "fly with" these for our Spring term class(es).

The code and an ExpressPCB .pcb file are downloadable from a public repository at:

https://github.com/parkerswanson/RocketKinematics

I will put together a list of parts and tools required (or recommended) over the next week or so. I'll also write summary instructions. Of course parts go out of stock and adjustments are made. For example, the Estes rocket we used in Winter 2017 term was purchased very cheaply on a close-out sale. For Spring term I'm recommending (and ordering for LBCC):

http://www.estesrockets.com/rockets/e2x/001955-top-shottm

This rocket is "Easy to Assemble" and large enough to carry our electronics, but it doesn't have a payload bay, so we will add one using parts from this excellent company:

https://www.apogeerockets.com/

We will use Estes "E" class one-time-use black powder rocket motors. These require extra-cost HazMat shipping, so it's advisable to minimize the number of orders.

Electronics:

Parts will be ordered from:
https://www.adafruit.com/
and
http://www.digikey.com/

The PC boards will be made by
https://www.expresspcb.com/

using the .pcb file (proprietary format) downloadable from github, as above. They come in a batch of 3 "Mini-Pro" boards, each of which will be bandsawed into 3 boards for the rockets, resulting in a cost of less than $7 per board per rocket.

A number of workshop tools will be required, including *good* soldering stations (about $50 each), electronics assembly tools, etc. I will write out a suggested list. I like to buy such tools from:

http://www.mcmelectronics.com/

Considering the time required to incorporate this project into a class:

I think the students can assemble the rocket in about 3 hours.

I think, with intensive coaching and plenty of available tools, they can assemble the electronics in another 3-5 hours. The time will depend on their experience and skill. Students who are already "makers" on their own will be prized role-models! Unfortunately, our experience has been that students rarely acquire "maker" skills during their usual college or university courses. 

A cheerful, competent Teaching Assistant will be invaluable!

The rocket flights can occupy a pleasant weekend morning. (I will write out a list of launch equipment needed.) Expected apogee will be about 300m, so a calm day and a big space (at least 500 ft radius) without threatening trees or obstacles will be needed!

Data analysis can be done as homework.

Many students will want to do additional flights and data analysis. This is habit-forming!

As I mentioned, I will be writing up lists and instructions. Meanwhile, please feel free to contact me with questions. 

Thanks for your interest in "Microcontrollers in Research & Design: Kinematics of Model Rockets".
Parker

From: Parker Swanson <swansop@linnbenton.edu>
Date: Fri, Mar 3, 2017 at 8:33 PM
Subject: Re: Rockets and data collection

This is a follow-up to my message a couple of days ago, below: 

I'll attach a graph of data collected and plotted by PH131 student James Sinnett, on the flight of his rocket Sat 25 Feb. The blue line, right axis, shows vertical acceleration; the red line, left axis shows altitude. The x axis is in milliseconds. The acceleration events show the motor firing, then the specified delay of about 6 seconds, then high-amplitude shaking caused by the firing of the parachute-ejection charge and the parachute deployment. You can note the few seconds of near-zero-g near apogee. Peak acceleration during motor firing was approximately 8 g, so our 16-g range accelerometer was an appropriate choice.

Rather satisfying is to compare the plot of acceleration during motor firing with the thrust curve of an Estes E-12 motor as measured by the Natl Assoc of Rocketry, in the second attached graphic. 

We will continue to refine this experiment, for future presentation to AAPT members. Thanks for your interest in our PH131 "Microcontrollers in R & D: Model Rocket Kinematics" project!

Parker

On Wed, Mar 1, 2017 at 11:40 AM, Parker Swanson <swansop@linnbenton.edu> wrote:

Thanks for your interest! 

A report from PH131, "Microcontrollers in R & D": In Winter & Spring terms students in this 1-unit "workshop" course build model rockets and equip them with microcontroller-based instrumentation to measure acceleration (in 3 axes), pressure, and temperature (thus altitude) during flight. The students and I intend to keep refining this experiment and present it as a workshop at an AAPT National Meeting within a couple of years. 

The electronics uses an Arduino Feather "Adalogger" from Adafruit, a 3-axis digital accelerometer, and a pressure/temperature sensor. The little system can write 100 datapoints per second to a micro-SD card (3 axis accel, plus temp, pressure, calculated altitude). 

Of course all those functions are available Off The Shelf, but it's more fun if the students put some "skin in the game" by assembling electronics and loading up (and modifying if they want) the Arduino code. It would be possible, but somewhat frustrating, to assemble the electronics on a solderable kludge-board, but I laid out a little custom PCB for them (now on its 3rd rev.). All the supplies for the course, including a rocket kit, cost about $130. The rocket uses an Estes E12-6 motor (12 N average thrust, 6 secs delay between motor cutoff & parachute ejection), so the rockets fly to a satisfying 300m+ apogee.

As I said, I intend to polish up this experiment and present it as a workshop at a AAPT National Meeting (and in Oregon if requested). 

Any questions, please ask.

Parker Swanson, MSEE; amateur radio KO7Q

Computer Science Instructor: Computer Systems Department

   Software and Electrical Engineering,
   
   Microcontroller hardware/software development,
   
   Advisor, LBCC Space Exploration Team
   
   NAR # 100949 SR
   
swansop@linnbenton.edu 

(office): 541 917-4274

Linn-Benton Community College

6500 Pacific Blvd. SW

Albany, OR 97321, USA
