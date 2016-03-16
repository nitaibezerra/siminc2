<style type="text/css">
    #aviso_browser{
        background: #fff;
        border: 1px red solid;
        width: 95%;
        padding: 10px;
        margin: 20px auto;
        display: none;
    }

    .aviso_destaque{
        background: #FF4500;
        color: #fff;
        font-size: 12pt;
        text-align: justify;
        padding: 10px;
        border: 1px solid #E45600;
    }

    .texto_destaque{
        font-weight: bold;
    }

    .box_navegadores{
        background-color: #FFF3DF;
        border: 1px solid #E45600;
        float: left;
        overflow: hidden;
        padding: 10px;
        width: 50%;
    }

    .left{
        float: left;
    }

    .right{
        float: right;
    }

    .clear{
        clear: both;
    }

    .aviso{
        width: 50%;
        border: 1px red solid;
    }

    .browsers{
        width: 49%;
        padding: 5px;
    }

    .browsers a{
        color: #FF4500;
        margin-left: 5px;
    }

    .browsers p{
        margin: 0 0 5px 5px;
    }
</style>

<div id="aviso_browser">

    <div class="left aviso">
        <div class="aviso_destaque">
            <p>O navegador que você está utilizando está desatualizado. Ele possui <span class="texto_destaque">falhas de segurança</span> e uma <span class="texto_destaque">lista limitada de funcionalidades</span>. Você perderá qualidade na navegação de alguns sites.</p>
        </div>

        <div style="padding: 10px;">
            <h3>Navegadores gratuitos</h3>
            <p>Ao lado estão as últimas versões dos navegadores gratuitos mais utilizados.</p>
            <p>Escolha um navegador para realizar o download do site do fabricante.</p>
        </div>

    </div>

    <div class="right browsers">

        <div class="left browsers" style="width: 50%">
            <div class="browser">
                <h3><img src="/estrutura/temas/default/img/browsers/chrome.png" alt="Google Chrome" /><a href="http://www.google.com/chrome?hl=pt" target="_blank" onmousedown="countBrowser('c')">Google Chrome</a></h3>
                <p>Navegador do Google com interface compacta. Automaticamente sempre atualizado!</p>
                <a href="http://www.google.com/chrome?hl=pt" target="_blank" onmousedown="countBrowser('c')">Download</a>
            </div>

            <div class="browser">
                <h3><img src="/estrutura/temas/default/img/browsers/firefox.png" alt="Google Chrome" /><a href="http://www.mozilla.com/firefox/" target="_blank" onmousedown="countBrowser('f')">Firefox</a></h3>
                <p>Navegador largamente utilizado e open-source, altamente extensível e personalizável</p>
                <a href="http://www.mozilla.com/firefox/" target="_blank" onmousedown="countBrowser('f')">Download</a>
            </div>
        </div>

        <div class="left browsers"  style="width: 45%">
            <div class="browser">
                <h3><img src="/estrutura/temas/default/img/browsers/opera.png" alt="Google Chrome" /><a href="http://www.opera.com/browser/" target="_blank" onmousedown="countBrowser('o')">Opera</a></h3>
                <p>Navegador com muitas funcionalidades</p>
                <a href="http://www.opera.com/browser/" target="_blank" onmousedown="countBrowser('o')">Download</a>
            </div>

            <div class="browser">
                <h3><img src="/estrutura/temas/default/img/browsers/ie.png" alt="Google Chrome" /><a href="http://windows.microsoft.com/pt-BR/internet-explorer/downloads/ie" target="_blank" onmousedown="countBrowser('i')">Internet Explorer</a></h3>
                <p>Navegador nativo do Windows</p>
                <a href="http://windows.microsoft.com/pt-BR/internet-explorer/downloads/ie" target="_blank" onmousedown="countBrowser('i')">Download</a>
            </div>
        </div>

    </div>
    <div class="clear"></div>
</div>