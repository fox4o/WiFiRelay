//
//   The HTML PAGE
//
const char HTML_Settings[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" href="style.css" type="text/css" />
<script src="microajax.js"></script> 

<form action="" method="post">
<center>
    <h1>IMPOSTAZIONI</h1>
    <hr>
    <table width="100%">
        <tr>
            <td class="tl"><a href="/" class="btn nod">&ltrif;INFORMAZIONE</a></td>
            <td class="tr"><a href="/manual" class="btn nod">MANUALE &rtrif;</a></td>
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
            <td class="tr">Giorno:</td>
            <td>
                <input type="text" id="sDD" name="sDD" size="2" value="01">
            </td>
        </tr>
        <tr>
            <td class="tr">Messe:</td>
            <td>
                <input type="text" id="sDM" name="sDM" size="2" value="01">
            </td>
        </tr>
        <tr>
            <td class="tr">Anno:</td>
            <td>
                <input type="text" id="sDY" name="sDY" size="4" value="2000">
            </td>
        </tr>
        <tr>
            <td class="tr">Orario:</td>
            <td>
                <input type="text" id="sTH" name="sTH" size="2" value="00">:<input type="text" id="sTM" name="sTM" size="2" value="00">
            </td>
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
            <td>
                <input type="text" id="sR1H" name="sR1H" size="2" value="00">:<input type="text" id="sR1M" name="sR1M" size="2" value="00">
            </td>
        </tr>
        <tr>
            <td>Duration:</td>
            <td>
                <input type="text" id="sR1D" name="sR1D" size="2" value="00"> min
            </td>
        </tr>
        <tr>
            <td>Status:</td>
            <td><span id="sS1"><strong class="g">ON</strong></span></td>
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
            <td>
                <input type="text" id="sR2H" name="sR2H" size="2" value="00">:<input type="text" id="sR2M" name="sR2M" size="2" value="00">
            </td>
        </tr>
        <tr>
            <td>Duration:</td>
            <td>
                <input type="text" id="sR2D" name="sR2D" size="2" value="00"> min
            </td>
        </tr>
        <tr>
            <td>Status:</td>
            <td><span id="sS2"><strong class="r">OFF</strong></span></td>
        </tr>
        <tr>
            <td colspan="2">
                <hr>
            </td>
        </tr>
        <tr>
            <td colspan="2" class="c"><input type="submit" value="Salva" class="btn nod"></td>
        </tr>
    </table>
</center>
</form>


<script>
function GetValues()
{
  setValues("/settingsvalues");
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

void send_settings_html()
{
  int dd=1;
  int dm=1;
  int dy=2000;
  int th=0;
  int tm=0;
  if (server.args() > 0 )  // Save Settings
  {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "sDD") dd = server.arg(i).toInt(); 
      if (server.argName(i) == "sDM") dm = server.arg(i).toInt(); 
      if (server.argName(i) == "sDY") dy = server.arg(i).toInt(); 
      if (server.argName(i) == "sTH") th = server.arg(i).toInt(); 
      if (server.argName(i) == "sTM") tm = server.arg(i).toInt(); 
      if (server.argName(i) == "sR1H") config.R1_hh = server.arg(i).toInt(); 
      if (server.argName(i) == "sR1M") config.R1_mm = server.arg(i).toInt(); 
      if (server.argName(i) == "sR1D") config.R1_dd = server.arg(i).toInt(); 
      if (server.argName(i) == "sR2H") config.R2_hh = server.arg(i).toInt(); 
      if (server.argName(i) == "sR2M") config.R2_mm = server.arg(i).toInt(); 
      if (server.argName(i) == "sR2D") config.R2_dd = server.arg(i).toInt(); 
    }
    // Update date time
    rtc.adjust(DateTime(dy, dm, dd, th, tm, 0));
    WriteConfig();
  }
  
  Serial.println("open: PAGE_Settings"); 
  server.send ( 200, "text/html", HTML_Settings );
   
}

void send_settings_values_html ()
{
  DateTime now = rtc.now();
  String values ="";

  values += "sDD|" +  (String) now.day() + "|input\n";
  values += "sDM|" +  (String) now.month() + "|input\n";
  values += "sDY|" +  (String) now.year() + "|input\n";
  values += "sTH|" +  (String) now.hour() + "|input\n";
  values += "sTM|" +  (String) now.minute() + "|input\n";

  values += "sR1H|" + (String)config.R1_hh +  "|input\n";
  values += "sR1M|" + (String)config.R1_mm +  "|input\n";
  values += "sR1D|" + (String)config.R1_dd +  "|input\n";
  if(R1_state==HIGH){
    values += "sS1|<strong class=\"r\">OFF</strong></span>|div\n";
  }
  else
  {
    values += "sS1|<strong class=\"g\">ON</strong></span>|div\n";
  }

  values += "sR2H|" + (String)config.R2_hh +  "|input\n";
  values += "sR2M|" + (String)config.R2_mm +  "|input\n";
  values += "sR2D|" + (String)config.R2_dd +  "|input\n";
  if(R2_state==HIGH){
    values += "sS2|<strong class=\"r\">OFF</strong></span>|div\n";
  }
  else
  {
    values += "sS2|<strong class=\"g\">ON</strong></span>|div\n";
  }

  server.send ( 200, "text/plain", values);

}

