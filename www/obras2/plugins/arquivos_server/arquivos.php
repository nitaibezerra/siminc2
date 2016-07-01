<!DOCTYPE html
PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
  <head>
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
	<title>FGV Arquivos</title>
	<script type="text/javascript" src="js/tree/_lib/jquery.js"></script>
	<script type="text/javascript" src="js/tree/_lib/jquery.cookie.js"></script>
	<script type="text/javascript" src="js/tree/_lib/jquery.hotkeys.js"></script>
	<script type="text/javascript" src="js/tree/jquery.jstree.js"></script>
	<script type="text/javascript" src="js/tree/_docs/syntax/!script.js"></script>
	<script type="text/javascript" src="js/jquery-ui-1.8.20.custom.min.js"></script>
	<script type="text/javascript" src="js/ui/jquery.ui.core.js"></script>
    <script type="text/javascript" src="js/ui/jquery.ui.widget.js"></script>
    <script type="text/javascript" src="js/ui/jquery.ui.mouse.js"></script>
    <script type="text/javascript" src="js/ui/jquery.ui.resizable.js"></script>
    <script type="text/javascript" src="js/ui/jquery.ui.dialog.js"></script>
    <script type="text/javascript" src="js/fileuploader.js"></script>
    <script type="text/javascript" src="js/slimScroll.js"></script>
    <script type="text/javascript" src="js/jquery.contextmenu.r2.js"></script>
    <link type="text/css" rel="stylesheet" href="arquivos.css" media="all" />
	<link type="text/css" rel="stylesheet" href="js/tree/_docs/syntax/!style.css"/>
	<link type="text/css" rel="stylesheet" href="js/tree/_docs/!style.css"/>
	<link type="text/css" rel="stylesheet" href="css/redmond/jquery.ui.all.css" />
  </head>
  <body id="corpo">
      <div id="explorer">
          <div id="demoResize" class="demo" style="height:600px; width: 180px;">
              <div id="demo" class="demo" style="height:600px; width: 180px;"></div>
          </div>
          <div id="caminhoLista"  class="demo" style="height:600px; width: 680px;">
              <div id="botoes"    class="demo" style="height:35px;  width: 680px;">
                    <div id="divNovaPasta">
                        <img src="./css/dropbox-icons/novaPasta32x32.png" title="Nova Pasta" onclick="criarNovaPasta();"/>
                    </div>
                    <div class="qq-upload-button" title="Enviar arquivos"></div>
                    <div id="divRenomear">
                        <img id="imgRenomearDesab" src="./css/dropbox-icons/editar_nome_d32x32.png" title="Selecione para renomear" onclick="$( '#dialogRenomearNenhum' ).dialog('open');"/>
                        <img id="imgRenomear"      src="./css/dropbox-icons/editar_nome32x32.png" title="Renomear" style="display: none;" onclick="renomearArquivo();"/>
                    </div>
                    <div id="divExcluir">
                        <img id="imgExcluirDesab" src="./css/dropbox-icons/excluir_d32x32.png" title="Selecione para excluir" onclick="$( '#dialogExcluirNenhum' ).dialog('open');"/>
                        <img id="imgExcluir"      src="./css/dropbox-icons/excluir32x32.png"   title="Excluir" style="display: none;" onclick="$( '#dialogExcluirVarios' ).dialog('open');"/>
                    </div>
              </div>
              <div id="caminho"   class="demo" style="height:35px;  width: 680px;"></div>
              <div id="cabecalho" class="demo" style="height:25px;  width: 680px;">
                <div id="cabecalhoNome" onclick="ordenar('nome', this);">
                    Nome
                </div>
                <div id="cabecalhoModificado"  onclick="ordenar('tamanho', this);">
                    Tamanho
                </div>
                <div id="cabecalhoData"  onclick="ordenar('modificacao', this);">
                    Modificado
                </div>
              </div>
              <div id="contetLista" class="demo ui-widget-content qq-uploader" style="width: 700px;">
                  <div class="" style="height:530px; width: 700px;">
                      <div id="lista" class="demo qq-upload-list"></div>
                      <div id="listaDrop" class="qq-upload-drop-area" style="height:540px; width: 700px; display: none;">
                        <span>Solte os arquivos aqui para fazer upload.</span>
                      </div>
                  </div>
              </div>
          </div>

          <!-- DIV COM O MENU DE CONOTEXTO -->
          <?php $desab = isset($_REQUEST['desabilita']) ? 'Desab' : ''; ?>
          <div class="contextMenu" id="menuLista<?= $desab; ?>">
            <ul>
              <li id="nova"><img src="./css/dropbox-icons/novaPasta16x16.png">&nbsp;</img> Nova Pasta</li>
            </ul>
          </div>
          <div class="contextMenu" id="menuItem<?= $desab; ?>">
            <ul>
              <li id="nova"><img src="./css/dropbox-icons/novaPasta16x16.png">&nbsp;</img> Nova Pasta</li>
              <li id="renomear"><img src="./css/dropbox-icons/editar_nome.gif">&nbsp;</img> Renomear</li>
              <li id="excluir"><img style="width: 16px; height: 16px; "src="./css/dropbox-icons/excluir16x16.png">&nbsp;</img> Excluir</li>
            </ul>
          </div>

          <div id="dialogNovaPasta" title="Criar nova pasta">
                Nome: <input type="text" id="nomeNovaPasta" name="nomeNovaPasta"  style="width:350px"></input>
          </div>

          <div id="dialogExcluir" title="Excluir arquivo">
                Tem certeza que deseja excluir o arquivo <b><label id="arqExcluir"></label></b>?
          </div>

          <div id="dialogExcluirVarios" title="Excluir arquivos selecionados">
                Tem certeza que deseja excluir os arquivos selecionados?
          </div>

          <div id="dialogExcluirNenhum" title="Excluir arquivos selecionados">
                Selecione pelo menos um arquivo para ser excluído.
          </div>

          <div id="dialogRenomear" title="Renomear arquivo">
                Nome: <input type="text" id="nomeRenomear" name="nomeRenomear" style="width:350px"></input>
          </div>

          <div id="dialogRenomearNenhum" title="Renomear arquivo">
                Selecione somente um arquivo para ser renomeado.
          </div>

          <script src="arquivos2.js" type="text/javascript"></script>
      </div>
  </body>
</html>