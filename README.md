# <img src="/src/resources/icon.png" alt="UdaWave Logo" width="50px"> UdaWave

## Brief description
<dl>
  <dt>Name</dt>
  <dd>UdaWave</dd>
  <dt>Version</dt>
  <dd>&alpha; 0.0.1</dd>
  <dt>Academic Year</dt>
  <dd>2017/2018</dd>
  <dt>Authors</dt>
  <dd>
	  <ul>
		  <li>Andrea <strong>Esposito</strong></li>
		  <li>Graziano <strong>Montanaro</strong>
	  </ul>
	  We are two **I.C.T. students** from *Taranto*. This software and documentation has been created as part of an exam.
  </dd>
  <dt>Brief project's description</dt>
  <dd>Computer lab's exam - <i>On Demand Music</i> program.</dd>
</dl>

## The Project
We have chosen to create a software for *on demand music*. It has some of the basic functions of other apps (like *Spotify*), except for the music itself (due to copyright reasons).
### Important Links
Here are some important links regarding this project.
- <a href="downloadable/Documentazione.pdf" download>Project Documentation (**ITA**)</a>
- <a href="https://github.com/mineand99/UdaWave" target="_blank">Source code (**GitHub**)</a>

### Software Modules
This is the graph of the dependencies of the various modules of the software. The circle represents the sofware itself (the *main file*); the rectangles represents the various modules.


graph LR
A((UdaWave)) --> B[Song]
A --> C[User]
B --> D[Utilities]
C --> D
A --> D
