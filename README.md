# Komputergrafika házifeladatok

##Fordítás, futtatás

bevgraf-vs-freeglut/bevgraf-vs-freeglut.sln fájlt kell futtatni Visual Studioban. Ezután a zöld nyíllal futtatható a program.

##1. Házi feladat - B-Spline 

A feladat egy, az első és az utolsó kontrollpontját interpoláló **B-Spline görbe** kirajzolására képes program elkészítése, 
mely a görbe megjelenítése mellett a kontrollpontok elhelyezését, mozgatását és törlését biztosítja. 
A görbepontokat a gyakorlaton megismert, mátrixokon alapuló módszer segítségével kell számolni.

**Program:** *Bspline.cpp*

**Kezelési útmutató:**

*Bal egérgomb*bal pontokat helyezhetünk le és meglévő pontokat mozgathatunk. *Jobb egérgomb*bal a meglévő pontokat törölhetjük.

---

##2. Házi feladat - NURBS

A feladat egy **NURBS**-görbe kirajzolására képes program elkészítése, mely a görbe megjelenítése mellett a görbe alakját 
befolyásoló adatok valós idejű megváltoztatását is lehetővé teszi. A görbepontokat a gyakorlaton megismert képlet 
segítségével kell kiszámolni.

**Program:** *NURBS.cpp*

**Kezelési útmutató:**

*Bal egérgomb*bal pontokat helyezhetünk le és meglévő pontokat mozgathatunk. Ha az egér miutatót egy lerakott pont fölé 
helyezzük ,akkor az *egér görgő* segítségével tudjuk az adott pont súlyát állítani. *e* betű lenyomásával elsőnek a 
változtatni kívánt csomópontot, majd annak új értékét adhatjukmeg. *+* és *-* gombokkal lehet növelni/csökkenteni a 
fokszámot. *r* betűvel a csomópontokat lehet kiírni. *f* betűvel az összes pont súlyértékét lehet kiíratni.

---

##3. Házi feladat - Bézier-felület

A feladat egy **bikubikus Bézier-felület** kirajzolására képes program elkészítése, mely a felület megjelenítése mellett 
a kontrollpontok áthelyezését biztosítja. A felület pontjait a gyakorlaton megismert képlet segítségével kell kiszámolni.

**Program:** *Bezier.cpp*

**Kezelési útmutató:**

*Bal egérgomb*bal lehet kiválasztani a bezier felület változtatni kívánt kontrolpontját. 
Ha a konzolban megjelenik a `selected` szöveg akkor a pont kiválasztása sikeres. A kiválasztott pont

x koordinátáját *X*-el lehet növelni *x*-el lehet csökkenteni,

y koordinátáját *Y*-el lehet növelni *y*-el lehet csökkenteni,

z koordinátáját *Z*-el lehet növelni *Z*-el lehet csökkenteni.

A kiválasztott pontot *jobb egérgomb*bal tudjuk elengedni. 
A kamera horizontális forgatásához az *e* (jobbra) illetve *q* (balra) gombok, 
a vertikális forgatáshoz a *w* (föl) illetve *s* (le) gombok és a közelítéshez a *d* (közelít) 
illetve *a* (távolodik) gombok szolgálnak.

---

##4. Házi feladat - Subdivision-görbe

A feladat egy zárt **subdivision-görbe** kirajzolására képes program elkészítése, mely a görbe pontjait a 
*Beccari és munkatársai* által kidolgozott *interpolating 4-point C² non-stationary ternary* séma segítségével számolja ki.

**Program:** *Subdivision.cpp*

**Kezelési útmutató:**

*Bal egérgomb*bal pontokat helyezhetünk le és meglévő pontokat mozgathatunk. 
Az *1, 2, 3, 4* gombokkal lehet megjeleníteni és elrejteni a különböző iterációkhozhoz tartozó kontrollpoligonokat. 
*7* és *9* gombokkal lehet a tensiont csökkenteni és növelni vagy a *8* gombbal lehet egyedi tension értéket adni.
*5* gombbal lehet a lehelyezett pontokat törölni.
