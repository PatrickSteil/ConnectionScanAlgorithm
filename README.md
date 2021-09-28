# Praktikum_CSA

Programmierpraktikum bei Herrn Prof Schulz

## Inhalt

- GTFS Parser
- GTFS von hier (https://opendata.rnv-online.de/dataset/gtfs-general-transit-feed-specification)
- CSA Algorithmus (https://i11www.iti.kit.edu/extra/publications/dpsw-isftr-13.pdf)


## Aktueller Stand

- Repo erzeugt
- "abgespecktes" GTFS, nur von den Strassenbahnlinien 21, 22, 23, 24, 26

## TO DO

- GTFS auf Korrektheit prüfen (Uhrzeiten, Anzeigen, Stationen, ...)
- Parser
- CSA

## GTFS Cleanup

Ich habe das Original GTFS heruntergeladen, habe es allerdings abgespeckt. Heißt ich habe ein valides kleineres GTFS erzeugt von ein paar Linien (zum Testen). Bei den Anführungszeichen in routes.txt hat mir der Regex "^(.*),(.+)?[\n\r]?" geholfen.