//
//   The HTML PAGE
//
const char HTML_Information[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" href="style.css" type="text/css" />
<script src="microajax.js"></script> 

<center>
    <h1>INFORMAZIONE</h1>
    <hr>
    <table width="100%">
        <tr>
            <td class="tl"><a href="/manual" class="btn nod">&ltrif;MANUALE</a></td>
            <td class="tr"><a href="/settings" class="btn nod">IMPOSTAZIONI &rtrif;</a></td>
        </tr>
        <tr>
            <td colspan="2">
                <hr>
            </td>
        </tr>
        <tr>
            <td colspan="2"><strong class="b">OROLOGIO</strong></td>
        </tr>
        <tr>
            <td>Giorno:</td>
            <td><span id="sD"></span></td>
        </tr>
        <tr>
            <td>Orario:</td>
            <td><span id="sT"></span></td>
        </tr>
        <tr>
            <td colspan="2">
                <hr>
            </td>
        </tr>
        <tr>
            <td colspan="2">
                <strong class="b">RELE 1</strong>
            </td>
        </tr>
        <tr>
            <td>Partenza:</td>
            <td><span id="sP1"></span></td>
        </tr>
        <tr>
            <td>Duration:</td>
            <td><span id="sD1"></span>min</td>
        </tr>
        <tr>
            <td>Status:</td>
            <td><span id="sS1"></span></td>
        </tr>
        <tr>
            <td colspan="2">
                <hr>
            </td>
        </tr>
        <tr>
            <td colspan=2><strong class="b">RELE 2</strong></td>
        </tr>
        <tr>
            <td>Partenza:</td>
            <td><span id="sP2"></span></td>
        </tr>
        <tr>
            <td>Duration:</td>
            <td><span id="sD2"></span>min</td>
        </tr>
        <tr>
            <td>Status:</td>
            <td><span id="sS2"></span></td>
        </tr>
        <tr>
            <td colspan="2">
                <hr>
            </td>
        </tr>
        <tr>
            <td colspan="2" class="c"><a href="javascript:GetValues()" class="btn nod">Aggiornare</a></td>
        </tr>
    </table>
</center>

<script>
function GetValues()
{
  setValues("/informationvalues");
}

window.onload = function ()
{
  load("style.css","css", function() 
  {
    load("microajax.js","js", function() 
    {
        GetValues();
    });
  });
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}



</script>
)=====" ;


//
// FILL WITH INFOMATION
// 

void send_information_values_html ()
{
  DateTime now = rtc.now();
  String values ="";

  // FIX !!!
  //String() + (now.hour() < 10 ? "0" : "") + now.hour() + ':' + (now.minute() < 10 ? "0" : "") + now.minute()
  //
  values += "sD|" +  (String) now.day() + "." + (String) now.month() + "." + (String) now.year() + "|div\n";
  values += "sT|" +  String() + (now.hour() < 10 ? "0" : "") + now.hour() + ':' + (now.minute() < 10 ? "0" : "") + now.minute() +  "|div\n";

  values += "sP1|" + (String)config.R1_hh +  ":"  +(String)config.R1_mm +  "|div\n";
  values += "sD1|" + (String)config.R1_dd +  "|div\n";
  if(R1_state==HIGH){
    values += "sS1|<strong class=\"r\">OFF</strong></span>|div\n";
  }
  else
  {
    values += "sS1|<strong class=\"g\">ON</strong></span>|div\n";
  }

  values += "sP2|" + (String)config.R2_hh +  ":"  +(String)config.R2_mm +  "|div\n";
  values += "sD2|" + (String)config.R2_dd +  "|div\n";
  if(R2_state==HIGH){
    values += "sS2|<strong class=\"r\">OFF</strong></span>|div\n";
  }
  else
  {
    values += "sS2|<strong class=\"g\">ON</strong></span>|div\n";
  }

  server.send ( 200, "text/plain", values);

}

