Question:
A game engine uses CSV (comma separated value) files to list what asset(s) a level needs to completely load. These CSV files are present, and have this structure per line: “type, GUID, relative file path”. An example is shown below:

Example Input:
File: frontend.csv
IMAGE,{6A006A92-4701-4668-B9D1-74A859333519},"images\legalscreen1.tiff"
MODEL,{405E2094-6BDF-479E-90B4-A48BF810AC23},"models\hero_hires.obj"
SOUNDBANK,{760A345E-8E95-407D-87B5-3A374290AF0A},"sounds\frontend.sfx"
ASSETLIST,{302B2A77-3EFC-4487-AA31-EDA51C0C356C},"common_frontend.csv"
IMAGE,{2990B650-170D-4A61-B3D4-58FFAC6B86DE},"images\legalscreen 02.tiff"

In the above list, there are two images, a model, a soundbank, and a reference to another CSV file to use for additional file(s) to load.

Problem:
A number of the image files on disk are duplicates of each other -- bytewise identical in file length & contents, but with distinct filenames.  Your job is to write a command line tool that loads CSVs, detects references to duplicate images and replaces them with references to the original.  This is intended to be run non-interactively, as part of a build process. 

Correct Example Output #1 (picking images\legalscreen1.tiff file):
Fixed File: frontend.csv
IMAGE,{6A006A92-4701-4668-B9D1-74A859333519},"images\legalscreen1.tiff"
MODEL,{405E2094-6BDF-479E-90B4-A48BF810AC23},"models\hero_hires.obj"
SOUNDBANK,{760A345E-8E95-407D-87B5-3A374290AF0A},"sounds\frontend.sfx"
ASSETLIST,{302B2A77-3EFC-4487-AA31-EDA51C0C356C},"common_frontend.csv"
IMAGE,{2990B650-170D-4A61-B3D4-58FFAC6B86DE},"images\legalscreen1.tiff"

Correct Example Output #2 (picking images\online\legalscreen 02.tiff)
Fixed File: frontend.csv
IMAGE,{6A006A92-4701-4668-B9D1-74A859333519},"images\legalscreen 02.tiff"
MODEL,{405E2094-6BDF-479E-90B4-A48BF810AC23},"models\hero_hires.obj"
SOUNDBANK,{760A345E-8E95-407D-87B5-3A374290AF0A},"sounds\frontend.sfx"
ASSETLIST,{302B2A77-3EFC-4487-AA31-EDA51C0C356C},"common_frontend.csv"
IMAGE,{2990B650-170D-4A61-B3D4-58FFAC6B86DE},"images\legalscreen 02.tiff"

Requirement / Answers to frequently asked questions:
-You tool can be written in in C++, C#, or Python
-Code must run on a Windows 7 desktop machine 32-bit or 64-bit
-The tool should process multiple CSVs pass from the command line like this: <YourTool> frontend.csv anotherfile.csv
-Informative error messages are desired
-GUI is not desired
-An ASSETLIST entry refers to additional asset(s) and should be loaded and processed recursively as part of running the tool.
-Identical images referenced across any loaded CSV in should be detected and fixed.  
-It does not matter which copy of the duplicate image is referenced (see Correct Output #1 and #2 above).
-3rd party helper packages are allowed only if they have a business-friendly license -- e.g. no GPL/LGPL packages. 
-If this outline above does not give clarification as to what you should do for various cases, code what you think is most appropriate, and put in a note why you chose that approach.
-If you have additional questions, ask!  