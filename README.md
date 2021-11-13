# Connection Scan Algorithm

Programming internship with Mr Prof Schulz

## Inhalt

- GTFS Parser
- GTFS (https://opendata.rnv-online.de/dataset/gtfs-general-transit-feed-specification)
- CSA Algorithm (https://i11www.iti.kit.edu/extra/publications/dpsw-isftr-13.pdf)


## TO DO

- Profile Journey Extraction
- Profile Journey & Leg optimization
- Sun Idea
- Correct Connections with calendar

# Error in Sources

Im csa\_overview [https://arxiv.org/pdf/1703.05997v1.pdf] steht auf Seite 20 "Further, τc[3] is 10 because the journey s@6 -> x -> y -> t@11 with 3 legs exists". It should update τc[3] to 11, the error is repeated on the bottom of the page "The resulting profile has the value {(6, (∞, 12, 10 [11])), (7, (∞, 12, 12)), (∞, (∞, ∞, ∞))}"

# Example Output Journey Extraction Profile (from Wasserturm to Exerzierplatz)

Profile 1:
Walking from 247521 to 245902:		Wasserturm	[09:57:42]	->	Rosengarten	[10:00:00]
5-2-1005-30060:		Rosengarten	[10:00:00]	->	Nationaltheater	[10:02:00]
5-2-1005-30060:		Bonifatiuskirche	[10:07:00]	->	Exerzierplatz	[10:08:00]
Walking from 240402 to 240401:		Exerzierplatz	[10:08:00]	->	Exerzierplatz	[10:09:03]

Profile 2:
Walking from 247521 to 245902:		Wasserturm	[10:05:42]	->	Rosengarten	[10:08:00]
5-34-2005-36300:		Rosengarten	[10:08:00]	->	Nationaltheater	[10:10:00]
5-34-2005-36300:		Bonifatiuskirche	[10:15:00]	->	Exerzierplatz	[10:16:00]
Walking from 240402 to 240401:		Exerzierplatz	[10:16:00]	->	Exerzierplatz	[10:17:03]

Profile 3:
Walking from 247521 to 247522:		Wasserturm	[10:08:21]	->	Wasserturm	[10:09:00]
2-1-1-35580:		Wasserturm	[10:09:00]	->	Rosengarten	[10:10:00]
2-1-1-35580:		Theresienkrankenhaus	[10:13:00]	->	Universitätsklinikum	[10:14:00]
Walking from 554404 to 554402:		Universitätsklinikum	[10:14:54]	->	Universitätsklinikum	[10:15:00]
14-202-1014-35880:		Universitätsklinikum	[10:15:00]	->	Lange Rötterstraße	[10:16:00]
14-202-1014-35880:		Bonifatiuskirche	[10:17:00]	->	Exerzierplatz	[10:18:00]
Walking from 240402 to 240401:		Exerzierplatz	[10:18:00]	->	Exerzierplatz	[10:19:03]

Profile 4:
Walking from 247521 to 245902:		Wasserturm	[10:15:42]	->	Rosengarten	[10:18:00]
14-204-3014-36900:		Rosengarten	[10:18:00]	->	Nationaltheater	[10:20:00]
14-204-3014-36900:		Bonifatiuskirche	[10:25:00]	->	Exerzierplatz	[10:26:00]
Walking from 240402 to 240401:		Exerzierplatz	[10:26:00]	->	Exerzierplatz	[10:27:03]

Profile 5:
Walking from 247521 to 247522:		Wasserturm	[10:18:21]	->	Wasserturm	[10:19:00]
2-851-1-36600:		Wasserturm	[10:19:00]	->	Rosengarten	[10:20:00]
2-851-1-36600:		Theresienkrankenhaus	[10:23:00]	->	Universitätsklinikum	[10:24:00]
Walking from 554404 to 554402:		Universitätsklinikum	[10:24:54]	->	Universitätsklinikum	[10:25:00]
5-2-3-31380:		Universitätsklinikum	[10:25:00]	->	Lange Rötterstraße	[10:26:00]
5-2-3-31380:		Lange Rötterstraße	[10:26:00]	->	Bonifatiuskirche	[10:27:00]
5-2-1005-31260:		Bonifatiuskirche	[10:27:00]	->	Exerzierplatz	[10:28:00]
Walking from 240402 to 240401:		Exerzierplatz	[10:28:00]	->	Exerzierplatz	[10:29:03]

Profile 6:
Walking from 247521 to 247522:		Wasserturm	[10:28:21]	->	Wasserturm	[10:29:00]
2-1-1-36780:		Wasserturm	[10:29:00]	->	Rosengarten	[10:30:00]
2-1-1-36780:		Theresienkrankenhaus	[10:33:00]	->	Universitätsklinikum	[10:34:00]
Walking from 554404 to 554402:		Universitätsklinikum	[10:34:54]	->	Universitätsklinikum	[10:35:00]
14-202-1014-37080:		Universitätsklinikum	[10:35:00]	->	Lange Rötterstraße	[10:36:00]
14-202-1014-37080:		Bonifatiuskirche	[10:37:00]	->	Exerzierplatz	[10:38:00]
Walking from 240402 to 240401:		Exerzierplatz	[10:38:00]	->	Exerzierplatz	[10:39:03]

Profile 7:
Walking from 247521 to 245902:		Wasserturm	[10:35:42]	->	Rosengarten	[10:38:00]
5-34-2005-38100:		Rosengarten	[10:38:00]	->	Nationaltheater	[10:40:00]
5-34-2005-38100:		Bonifatiuskirche	[10:45:00]	->	Exerzierplatz	[10:46:00]
Walking from 240402 to 240401:		Exerzierplatz	[10:46:00]	->	Exerzierplatz	[10:47:03]

Profile 8:
Walking from 247521 to 247522:		Wasserturm	[10:38:21]	->	Wasserturm	[10:39:00]
2-1-1-37380:		Wasserturm	[10:39:00]	->	Rosengarten	[10:40:00]
2-1-1-37380:		Theresienkrankenhaus	[10:43:00]	->	Universitätsklinikum	[10:44:00]
Walking from 554404 to 554402:		Universitätsklinikum	[10:44:54]	->	Universitätsklinikum	[10:45:00]
5-62-1005-36060:		Universitätsklinikum	[10:45:00]	->	Lange Rötterstraße	[10:46:00]
5-62-1005-36060:		Bonifatiuskirche	[10:47:00]	->	Exerzierplatz	[10:48:00]
Walking from 240402 to 240401:		Exerzierplatz	[10:48:00]	->	Exerzierplatz	[10:49:03]

Profile 9:
Walking from 247521 to 245902:		Wasserturm	[10:45:42]	->	Rosengarten	[10:48:00]
14-204-3014-38700:		Rosengarten	[10:48:00]	->	Nationaltheater	[10:50:00]
14-204-3014-38700:		Bonifatiuskirche	[10:55:00]	->	Exerzierplatz	[10:56:00]
Walking from 240402 to 240401:		Exerzierplatz	[10:56:00]	->	Exerzierplatz	[10:57:03]

Profile 10:
Walking from 247521 to 247522:		Wasserturm	[10:48:21]	->	Wasserturm	[10:49:00]
2-851-1-38400:		Wasserturm	[10:49:00]	->	Rosengarten	[10:50:00]
2-851-1-38400:		Theresienkrankenhaus	[10:53:00]	->	Universitätsklinikum	[10:54:00]
Walking from 554404 to 554402:		Universitätsklinikum	[10:54:54]	->	Universitätsklinikum	[10:55:00]
14-202-1014-38280:		Universitätsklinikum	[10:55:00]	->	Lange Rötterstraße	[10:56:00]
14-202-1014-38280:		Bonifatiuskirche	[10:57:00]	->	Exerzierplatz	[10:58:00]
Walking from 240402 to 240401:		Exerzierplatz	[10:58:00]	->	Exerzierplatz	[10:59:03]

(This Query took 22.338 ms)
# Sources

- https://i11www.iti.kit.edu/_media/teaching/sommer2014/routenplanung/vorlesung4.pdf
- https://i11www.iti.kit.edu/_media/teaching/theses/ba-groetschla.pdf
- https://arxiv.org/pdf/1703.05997v1.pdf
- https://www.youtube.com/watch?v=dQw4w9WgXcQ
- https://newbedev.com/c-split-string-by-regex
- https://www.systutorials.com/how-to-process-a-file-line-by-line-in-c/
- https://www.learncpp.com/cpp-tutorial/overloading-the-comparison-operators/
- https://stackoverflow.com/questions/9706517/sort-a-vector-of-objects-by-an-objects-attribute
