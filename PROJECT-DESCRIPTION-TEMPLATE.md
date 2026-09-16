# Muzička učionica

119/2022 - Ana Mladenović

Projekat predstavlja interaktivnu 3D scenu muzičke učionice.
Scena sadrži učionicu, klavire, metronom, prozor, sliku i lampu.
Implementirana su dva tipa osvetljenja, upravljanje tačkastim svetlom pomoću tastature,
vremenski događaj sa metronomom i Bloom efekat uz HDR renderovanje.

## Controls

W -> Kretanje kamere napred
S -> Kretanje kamere nazad
A -> Kretanje kamere ulevo
D -> Kretanje kamere udesno
L -> Uključivanje i isključivanje tačkastog svetla
J -> Pomeranje tačkastog svetla ulevo
K -> Pomeranje tačkastog svetla udesno
C -> Promena boje tačkastog svetla između toplog i hladnog
M -> Pokretanje vremenskog događaja sa metronomom
ESC -> Izlazak iz aplikacije

## Features

### Fundamental:

[x] Model with lighting
[x] Two types of lighting with customizable colors and movement through GUI or ACTIONS
[x] Pritisak tastera M --- AFTER 1 SECOND ---Triggers---> Metronom se pomera ulevo --- AFTER 2 SECONDS ---Triggers---> Metronom se pomera udesno

### Group A:

[ ] Frame-buffers with post-processing   
[ ] Off-screen Anti-Aliasing  
[ ] Parallax Mapping
[x] Bloom with the use of HDR

### Group B:

[ ] Deferred Shading  
[ ] Point Shadows  
[ ] SSAO

### Engine improvement:

[ ] ...

## Models:

Piano:
https://sketchfab.com/3d-models/bosenklavier-model-b-d111b2412fb74366929d222ac6fbe359

Piano 2:
https://sketchfab.com/3d-models/old-piano-294ad9f9f0bb4223b8a71c2aec6f7104

Metronome:
https://www.turbosquid.com/3d-models/3d-music-metronome-1445619

Window:
https://sketchfab.com/3d-models/double-arched-window-490-c5e9d383959c480a952c8f7c020bbbc5

Painting:
https://sketchfab.com/3d-models/bc02626756c2404ab415c2df074672bf

Floor lamp:
https://sketchfab.com/3d-models/floor-lamp-3f4b46f5df0a4874ab3e37ca28d2665e#download

## Textures

Parquet
https://angiehomes.co/cdn/shop/files/Pergo-Walnut_-3-strip-Laminated-Flooring-Pergo-1683014194.jpg?v=1683014195&width=600
