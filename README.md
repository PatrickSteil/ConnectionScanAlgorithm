# Connection Scan Algorithm

Programmierpraktikum bei Herrn Prof Schulz

## Inhalt

- GTFS Parser
- GTFS von hier (https://opendata.rnv-online.de/dataset/gtfs-general-transit-feed-specification)
- CSA Algorithmus (https://i11www.iti.kit.edu/extra/publications/dpsw-isftr-13.pdf)


## TO DO

- GTFS Footpath sind nicht vorhanden (normalerweise existiert eine transfers.txt Datei). Vllt Sweepline Algorithmus um per Luftlinie und Geh Geschwindigkeit die Zeit zu bekommen ? Aber muss "transitively closed" sein!
- Trip Flags - und verstehen wie weshalb warum

## Notizen

Im Paper wird von "Minimum Change Time" geredet, also wenn eine Station mehrere Gleise hat, und der Benutzer das Gleis wechseln muss. Dies nimmt Zeit in Anspruch und muss berücksichtigt werden.
Aber in diesen GTFS sind einzelne Gleise als einzelne Station-IDs aufgelistet und deswegen ist hier die "Minimum Change Time" die Dauer des Transfers.
Heißt: "Minimum Change Time" wird mit unseren Footpath abgehandelt.

Wird auch im Paper "Connection Scan Accerlerated" wie folgt erläutert:

It [GTFS] only allows for footpaths between two stops (...) Many feeds model large train stations not as a single stop but each railway platform inside the station as individual stop and connect the platforms using footpaths.

# Quellen

- https://i11www.iti.kit.edu/_media/teaching/sommer2014/routenplanung/vorlesung4.pdf
- https://i11www.iti.kit.edu/_media/teaching/theses/ba-groetschla.pdf
- https://www.youtube.com/watch?v=dQw4w9WgXcQ
- https://newbedev.com/c-split-string-by-regex
- https://www.systutorials.com/how-to-process-a-file-line-by-line-in-c/
- https://www.learncpp.com/cpp-tutorial/overloading-the-comparison-operators/
- https://stackoverflow.com/questions/9706517/sort-a-vector-of-objects-by-an-objects-attribute