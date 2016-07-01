<?php

if(!$_SESSION['par']['adpid'] || !$_SESSION['par']['prgid'] || !$_SESSION['par']['inuid']){
	echo '<script type="text/javascript"> 
    		alert("Sessão Expirou.\nFavor selecione o programa novamente!");
    		window.close();
    	  </script>';
    die;
}

include_once '_funcoes_escolaativa.php';
include_once APPRAIZ.'par/classes/modelo/EscolaAtiva.class.inc';
include_once APPRAIZ.'par/classes/modelo/EscolaAtivaEscolas.class.inc';
include_once APPRAIZ.'par/classes/modelo/EscolaAtivaPessoas.class.inc';


if( $_REQUEST["req"] == "gravarSessao" ){
	
	$_SESSION["arEntidadesSelecionadas"] = $_REQUEST["arChecks"];

}
$itrid = pegarItridEscolaAtiva( $_SESSION['par']['inuid'] );
//$obEntidade= new Entidade();

if( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ){
	$arEscolas = montarRelacionamentoEscolasAtivasPorEstuf( $_REQUEST["estuf"], $_REQUEST['nome'], $_REQUEST['codigo'] );
}
else{
	$arEscolas = montarRelacionamentoEscolasAtivasPorMuncod( $_REQUEST["muncod"], $_REQUEST['nome'], $_REQUEST['codigo'] );
}

$arEntid = recuperarEscolasPorMuncod( $_REQUEST["muncod"] );

$arEntidadesSelecionadas = $_SESSION["arEntidadesSelecionadas"];

?>
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="-1">
    <title><?php echo $titulo ?></title>

    <script type="text/javascript" src="/includes/funcoes.js"></script>
    <script type="text/javascript" src="/includes/prototype.js"></script>
    <script type="text/javascript" src="/includes/JQuery/jquery.js"></script>

    <link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
	
    <style type="text/css" media="screen">
        #loader-container,
        #LOADER-CONTAINER
        {
            background: none;
            position: absolute;
            width: 100%;
            text-align: center;
            z-index: 8000;
            height: 100%;
        }

        #loader {
            background-color: #fff;
            color: #000033;
            width: 300px;
            border: 2px solid #cccccc;
            font-size: 12px;
            padding: 25px;
            font-weight: bold;
            margin: 150px auto;
        }
        
        #toolTipQtd{
        	width: 100px; 
        	height: 50px; 
        	background: #fff; 
        	position: absolute;
        	border: 1px #555 solid;
        	padding: 5px;
        	display: none;
        }
        
    </style>
  </head>
  <body>
    <div id="loader-container">    
      <div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
    </div>    
    <form id="bus" name="bus" onsubmit="return false;">
    	<center><br><label>Buscar escola por nome ou código: </label><br>
    	<input type="text" id="busca" name="busca" value="<?=$_REQUEST['nome'].$_REQUEST['codigo'];?>"><input type="button" onclick="buscar();" name="btbuscar" value="Buscar">
    	<br>
    	<?php
    	if ($_REQUEST['codigo'] || $_REQUEST['nome'])
    	{
    		echo "<a href=\"javascript:history.go(-1)\">Voltar</a>";
    	}
    	?>
    	</center><br>
    	<input name="muncod" id="muncod" type="hidden" value="<?=$_REQUEST['muncod'] ?>">
    	<input name="estuf" id="estuf" type="hidden" value="<?=$_REQUEST['estuf'] ?>">
    </form>
    <form action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="frmEscolas" name="frmEscolas" onsubmit="return false;">    
    <table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
      <tr style="background-color: #cccccc;">
        <td>
          <div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Relacionar Escolas
          <br>
          
          </div>
          <div style="margin: 0; padding: 0; height: 450px; width: 100%; background-color: #eee; border: none;" class="div_rolagem">
			<?php $db->monta_lista_simples($arEscolas, array( '', 'Codigo', 'Escolas' ), 5000, 10, 'N', '100%'); ?>			
          </div>
          <br />
        </td>
      </tr>
      <tr>
      	<td>
      		<input type="button" value="Ok" onclick="window.close();">
      	</td>
     </tr>
    </table>
    </form>
  </body>

  <script type="text/javascript">
  <!--    
         
    function buscar(){ 
    	var busca = $('busca').value;
    	var muncod = $('muncod').value;
    	var estuf = $('estuf').value;
    	

    	if (estuf)
    	{
	    	if(isNaN(busca))
	    	{
	    		window.location.href = 'par.php?modulo=principal/programas/feirao_programas/popupEscolaTerra&acao=A&estuf=' + estuf + '&nome='+busca;
	    	} else {
	    		window.location.href = 'par.php?modulo=principal/programas/feirao_programas/popupEscolaTerra&acao=A&estuf=' + estuf + '&codigo='+busca;
	    	}
	    } else {
	    	if(isNaN(busca))
	    	{
	    		window.location.href = 'par.php?modulo=principal/programas/feirao_programas/popupEscolaTerra&acao=A&muncod=' + muncod + '&nome='+busca;
	    	} else {
	    		window.location.href = 'par.php?modulo=principal/programas/feirao_programas/popupEscolaTerra&acao=A&muncod=' + muncod + '&codigo='+busca;
	    	}
	    }	    
  	}
     
    function selecionarTodas()
    {
        var items = Form.getElements($('frmEscolas'));

        for (var i = 0; i < items.length; i++) {
            if (items[i].getAttribute('type') == 'checkbox' &&
                /entid/.test(items[i].getAttribute('id')))
            {
                items[i].checked = $('selecionar').checked;
            }
        }
    }

    function validarFrmEscolas()
    {
        var checks = $('frmEscolas').getInputs('checkbox', 'entid[]');
        var submit = true;
        
        for (var i = 0, length = checks.length; i < length; i++) {
            if (checks[i].checked || checks[i].checked == 'checked') {
                var qfaqtd = $('frmEscolas').getInputs('text');
				
				if(qfaqtd[i+1].value == '' || qfaqtd[i+1].value == 0 ){
					 //qfaqtd.setStyle({backgroundColor: '#ffd'});
					 submit = false;
				}
				
            }
        }

		 if (submit == false){
		 	alert('Para escolas selecionadas, a quantidade deve ser informada!');
		  	return submit;
		 }else{
		  	return submit;
		 }
		 

    }
    
    function preencherTodasQtds(){
    	var qtdPadrao = document.getElementById( 'qfaqtdTotal' ).value;
        var arInputQtd = document.getElementsByTagName('input');
        	
        if( qtdPadrao ){
	    	for( i = 0; i < arInputQtd.length; i++ ){
	    		if( arInputQtd[i].name.substr( 0, 6 ) == 'qfaqtd' )
    				arInputQtd[i].value = qtdPadrao;
	    	}
    	}
    }
    
    function mouseOverToolTip( objeto ){
    	MouseOver( objeto );
		document.getElementById( 'toolTipQtd' ).style.display = "block";    
    }
    
    function mouseOutToolTip( objeto ){
    	MouseOut( objeto );
		document.getElementById( 'toolTipQtd' ).style.display = "none";    
    }

	function marcarMarcados(){
	
		var arEntid = window.opener.document.getElementsByName('entid_escolas[]');
		for( i=0; i<arEntid.length; i++ ){
			if($("entid_"+ arEntid[i].value)) {
        		$("entid_"+ arEntid[i].value).setAttribute("checked","checked");
        	}
		}
	}

	jQuery.noConflict();
	jQuery(document).ready(function() {
		marcarMarcados();
	});

    $('loader-container').hide();
  -->
  </script>
</html>