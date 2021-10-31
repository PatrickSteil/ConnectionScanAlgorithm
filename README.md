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

# Fehler in Quellen

Im csa\_overview [https://arxiv.org/pdf/1703.05997v1.pdf] steht auf Seite 20 "Further, τc[3] is 10 because the journey s@6 -> x -> y -> t@11 with 3 legs exists". It should update τc[3] to 11, the error is repeated on the bottom of the page "The resulting profile has the value {(6, (∞, 12, 10 [11])), (7, (∞, 12, 12)), (∞, (∞, ∞, ∞))}"
# Quellen

- https://i11www.iti.kit.edu/_media/teaching/sommer2014/routenplanung/vorlesung4.pdf
- https://i11www.iti.kit.edu/_media/teaching/theses/ba-groetschla.pdf
- https://arxiv.org/pdf/1703.05997v1.pdf
- https://www.youtube.com/watch?v=dQw4w9WgXcQ
- https://newbedev.com/c-split-string-by-regex
- https://www.systutorials.com/how-to-process-a-file-line-by-line-in-c/
- https://www.learncpp.com/cpp-tutorial/overloading-the-comparison-operators/
- https://stackoverflow.com/questions/9706517/sort-a-vector-of-objects-by-an-objects-attribute
