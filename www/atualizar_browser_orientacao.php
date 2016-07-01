<style type="text/css">
    #aviso_browser_orientacao{
        background: #fff;
        border: 1px red solid;
        padding: 10px;
        margin: 20px auto;
    }

    .aviso_destaque_orientacao{
        background: #FF4500;
        color: #fff;
        font-size: 12pt;
        text-align: justify;
        padding: 10px;
        border: 1px solid #E45600;
    }

    .texto_destaque_orientacao{
        font-weight: bold;
    }

    .box_navegadores_orientacao{
        background-color: #FFF3DF;
        border: 1px solid #E45600;
        float: left;
        overflow: hidden;
        padding: 10px;
        width: 50%;
    }

    .left_orientacao{
        float: left;
    }

    .right_orientacao{
        float: right;
    }

    .clear_orientacao{
        clear: both;
    }

    .aviso_orientacao{
        border: 1px red solid;
    }

    .browsers_orientacao{
        padding: 5px;
    }

    .browsers_orientacao a{
        color: #FF4500;
        margin-left: 5px;
    }

    .browsers_orientacao p{
        margin: 0 0 5px 5px;
    }
</style>

<div id="aviso_browser_orientacao">

    <div class="aviso_orientacao">
        <div class="aviso_destaque_orientacao">
            <p>Para maior <span class="texto_destaque_orientacao">Segurança, qualidade na navegação</span> e <span class="texto_destaque_orientacao">compatibilidade de funcionalidades</span>, pede-se que mantenha seu browser sempre atualizado.</p>
        </div>

        <div style="padding: 10px;">
            <h3>Navegadores gratuitos</h3>
            <p>Abaixo, estão as últimas versões dos navegadores gratuitos mais utilizados.</p>
            <p>Escolha um navegador para realizar o download do site do fabricante.</p>
        </div>

    </div>

    <div class="browsers_orientacao">

        <div class="left_orientacao browsers_orientacao"  style="width: 45%">
            <div class="browser">
                <h3><img src="estrutura/temas/default/img/browsers/opera.png" alt="Google Chrome" /><a href="http://www.opera.com/browser/" target="_blank" onmousedown="countBrowser('o')">Opera</a></h3>
                <p>Navegador com muitas funcionalidades</p>
                <a href="http://www.opera.com/browser/" target="_blank" onmousedown="countBrowser('o')">Download</a>
            </div>

            <div class="browser">
                <h3><img src="estrutura/temas/default/img/browsers/ie.jpg" alt="Google Chrome" /><a href="http://windows.microsoft.com/pt-BR/internet-explorer/downloads/ie" target="_blank" onmousedown="countBrowser('i')">Internet Explorer</a></h3>
                <p>Navegador nativo do Windows</p>
                <a href="http://windows.microsoft.com/pt-BR/internet-explorer/downloads/ie" target="_blank" onmousedown="countBrowser('i')">Download</a>
            </div>
        </div>

        <div class="left_orientacao browsers_orientacao" style="width: 50%">
            <div class="browser">
                <h3><img src="estrutura/temas/default/img/browsers/chrome.png" alt="Google Chrome" /><a href="http://www.google.com/chrome?hl=pt" target="_blank" onmousedown="countBrowser('c')">Google Chrome</a></h3>
                <p>Navegador do Google com interface compacta. Automaticamente sempre atualizado!</p>
                <a href="http://www.google.com/chrome?hl=pt" target="_blank" onmousedown="countBrowser('c')">Download</a>
            </div>

            <div class="browser">
                <h3><img src="estrutura/temas/default/img/browsers/firefox.png" alt="Google Chrome" /><a href="http://www.mozilla.com/firefox/" target="_blank" onmousedown="countBrowser('f')">Firefox</a></h3>
                <p>Navegador largamente utilizado e open-source, altamente extensível e personalizável</p>
                <a href="http://www.mozilla.com/firefox/" target="_blank" onmousedown="countBrowser('f')">Download</a>
            </div>
        </div>

    </div>
    <div class="clear_orientacao"></div>
</div>