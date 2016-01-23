/*
 * Lies das Tutorial:
 *   - https://github.com/niccokunzmann/ledtable/tree/master/arduino/libraries/ledtable/examples/de_10_tutorial
 *   - lokal unter Dokumente/Arduino/libraries/ledtable/de_10_tutorial
 *
 * Übrigens ist alles, was so grau ist nud zwischen 
 * /* und * / steht ein Kommentar für Menschen, der nicht 
 * vom Computer ausgewertet wird.
 * 
 * Die Zeile "#include <ledtable.h>" sagt, dass wir schon 
 * vorgefertigten Quelltext für den LED-Tisch für unser 
 * Programm verwenden wollen.
 */
#include <ledtable.h>

/*
 * Wir nennen einen LED-Tisch jetzt "table" uns sagen, wie der 
 * LED-Tisch aussieht, weil der Arduino das nicht selbst kann.
 * 6  - der LED-Tisch wurde mit dem grünen Kabel an Pin "6" 
 *      angeschossen. 
 * 12 - der LED-Tisch ist 12 Pixel breit.
 * 100 - der LED-Tisch ist 13 Pixel hoch.
 * PIXELORDER<snake> - die LEDs in dem LED-Tisch sind als 
 *                     eine Schlange angeordnet.
 */
LEDTable table = LEDTable(6, 12, 100, PIXELORDER<snake>);
/* 
 * (!) Bitte überprüfe die Angaben und schaue auf den LED-Tisch, 
 *     ob das stimmt.
 * (!) Du kannst jetzt augf den "✓" Knopf drücken, um das Programm 
 *     zu testen. Es sollte sowas da stehen:
 * 
 *         Sketch uses 3,216 bytes (10%) of program storage space. Maximum is 30,720 bytes.
 *         Global variables use 223 bytes (10%) of dynamic memory, leaving 1,825 bytes for local variables. Maximum is 2,048 bytes.
 */

void setup()
{
    /* 
     *  Alles, was in setup steht, wird nur einmal ausgeführt. 
     * Danach wird setup ausgeführt.
     *  
     * (!) Die folgende Zeile sagt, dass wir unseren "table" 
     *     anfangen wollen, zu benutzen. Entferne die "//" Zeichen, 
     *     um die Zeile ausführen zu lassen.
     *     Jetzt sollte sie farbig werden.
     */
    //table.begin()
    /*
     * (!) Test dein Programm mit dem "✓" Knopf!
     *     Oh, ein Fehler!
     * 
     *         exit status 1
     *         expected ';' before '}' token
     *   
     *     Wir haben etwas falsch geschrieben. Schaue Dir vielleicht 
     *     auch die anderen Zeilen an, um festzustellen, was sie von 
     *     dieser unterscheidet.
     *     
     *     
     *     
     *     
     *     Da muss ein ";" hinter die Klammern.
     *     Die Zeile sollte "table.begin();" heißen.
     */


    /*
     * Die folgende Zeile setzt die Grundfarbe. 
     * Welche Farben kennst du?
     * Du kannst alle auf 
     * http://html-color-codes.info/color-names nachschlagen.
     * (!) Fülle den Tisch mit einer Farbe deiner Wahl!
     * (!) Lade das Programm auf den Arduino mit dem "→" Knopf.
     *     Wenn du es nicht drauf laden kannst, musst du eventuell 
     *     das rote Kabel rausziehen, weil der LED-Tisch zu viel 
     *     Strom verbraucht.
     */
    table.fill(color_black);


    /*
     * Damit der Tisch nicht zu hell wird und zu viel Strom 
     * verbruacht, können wir die Helligkeit setzen:
     */
    table.brightness(20);
    /*
     * Wenn der Tisch zu viel Strom verbraucht, dann werden 
     * die Farben verfälscht.
     */


    /*
     * Jetzt geht das Malen los!
     * (!) Entferne "//" von der nächsten Zeile, 
     *     um den Pixel an der Stelle x=0, y=0 zu färben.
     */
    //table.fill(0,0, color_green);
    
    //table.fill(1,0, color_green);
    /*
     * Modifiziere die Zeilen so, wie es dir gefällt.
     * Ich male eine braune Brücke über einen Fluss.
     */
    //table.fill(2,0, color_blue);
    //table.fill(3,0, color_green);
    //table.fill(4,0, color_green);
    //table.fill(1, 1, color_brown);
    //table.fill(2, 2, color_brown);
    //table.fill(3, 1, color_brown);
    /*
     * Schön und groß ist sie nicht, ich danke das kannst du besser!
     * (!) Male hier, was du möchtest. Im nächsten Tutorial geht es darum, 
     *     wie man schneller malt and Pixel für Pixel.
     */

    /*
     * "table.show()" musst du immer dann ausführen, nachdem 
     * Du Malanweisungen gegeben hast, damit sie angezeigt werden.
     */
    table.show();


}

void loop()
{
  /* 
   * Alles, was in loop steht, wird immer und immer wieder ausgeführt.
   * Der Arduino kann nicht anhaltne und nichts tun. Er muss immer 
   * beschäftigt werden.
   */
}


