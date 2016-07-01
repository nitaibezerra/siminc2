<?php
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo('Incluir Foto','&nbsp;');
?>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-2" />
<meta name="description" content="vamapaull - flash designer and developer - vamapaull's portfolio" />
<meta name="keywords" content="flash,web,design,development,developer,designer,coder,code,actionscript,script,animation,animations,interactive" />
<script src="./componentes/swfobject.js" language="javascript"></script>
<center>
    <div id="flashArea" class="flashArea" style="height: 100%;">
        <p align="center">
            This content requires the Adobe Flash Player.<br />
            <a href="http://www.adobe.com/go/getflashplayer">
                <img src="http://www.adobe.com/images/shared/download_buttons/get_flash_player.gif"
                     alt="Get Adobe Flash player" /></a><br />
                <a href="http://www.macromedia.com/go/getflash">Get Flash</a>
        </p>
    </div>
    <script type="text/javascript">
        var mainswf = new SWFObject("./componentes/take_picture.swf", "main", "700", "400", "9", "#FFFFFF");
        mainswf.addParam("scale", "noscale");
        mainswf.addParam("wmode", "window");
        mainswf.addParam("allowFullScreen", "true");
        mainswf.write("flashArea");
    </script>
</center>