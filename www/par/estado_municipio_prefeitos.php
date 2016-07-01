<?php
$_REQUEST['baselogin'] = "simec_desenvolvimento";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(30000);

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

session_start();
 
// CPF do administrador de sistemas
if(!$_SESSION['usucpf']) {
	$_SESSION['usucpforigem'] = '';
	$_SESSION['usucpf'] = '';
}

$db = new cls_banco();

function carregarMunicipiosPorUF($dados) {
	global $db;
	$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$dados['estuf']."' ORDER BY mundescricao";
	$combo = $db->monta_combo($dados['name'], $sql, 'S', 'Selecione', (($dados['onclick'])?$dados['onclick']:''), '', '', '200', 'S', $dados['id'], true, $dados['valuecombo']);
	
	if($dados['returncombo']) return $combo;
	else echo $combo;
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Ministério da Educação</title>
<link href="css/estilo.css" type="text/css" rel="stylesheet" />
<link rel="stylesheet" href="barra_governo/css/barra_do_governo.css" type="text/css" />
<script language="javascript" type="text/javascript" src="js/jquery-1.8.1.min.js"></script>
<script language="javascript" type="text/javascript" src="js/acessibilidade.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
</head>
<script>
function carregarMunicipiosPorUF(estuf) {
	if(estuf) {
		ajaxatualizar('requisicao=carregarMunicipiosPorUF&id=muncod&name=muncod&estuf='+estuf,'td_municipio');
	} else {
		document.getElementById('td_municipio').innerHTML = "Selecione uma UF";
	}
}

function enviarPainel() {
	if(document.getElementById('muncod')) {
		if(document.getElementById('muncod').value=='') {
			alert('Selecione um Município');
			return false;
		}
		
		window.open('http://simec.mec.gov.br/par/prefeitos/prefeitos.php?muncod='+document.getElementById('muncod').value,'Indicador','scrollbars=yes,height=700,width=800,status=no,toolbar=no,menubar=no,location=no');
		void(0);
		
	} else {
		alert('Selecione uma UF');
		return false;
	}
	
}

function ajaxatualizar(params,iddestinatario) {
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: params,
   		async: false,
   		success: function(html){
   			if(iddestinatario!='') {
   				document.getElementById(iddestinatario).innerHTML = html;
   			}
   		}
	});

}

</script>

<body class="aumentarFonte">
<!-- 
<div id="barra-brasil-v3">
  <div id="barra-brasil-v3-marca"> Brasil &ndash; Governo Federal &ndash; Minist&eacute;rio da Educa&ccedil;&atilde;o </div>
</div>
 -->
 
<div id="barra-brasil" style="background:#7F7F7F; height: 20px; padding:0 0 0 10px;display:block;"> 
	<ul id="menu-barra-temp" style="list-style:none;">
		<li style="display:inline; float:left;padding-right:10px; margin-right:10px; border-right:1px solid #EDEDED"><a href="http://brasil.gov.br" style="font-family:sans,sans-serif; text-decoration:none; color:white;">Portal do Governo Brasileiro</a></li> 
		<li><a style="font-family:sans,sans-serif; text-decoration:none; color:white;" href="http://epwg.governoeletronico.gov.br/barra/atualize.html">Atualize sua Barra de Governo</a></li>
	</ul>
</div>
		 
<div id="geral">

  <div id="topo">
  	<h1><a href="http://encontroprefeitos2013.mec.gov.br">Encontro Nacional com Novos Prefeitos e Prefeitas</a></h1>
    <a href="http://portal.mec.gov.br" class="portalMec">Portal MEC</a>
    <div class="fonte">
    	<span class="aumentar">Aumentar Fonte</span>
      <span class="normal">Fonte Normal</span>
      <span class="diminuir">Diminuir Fonte</span>
    </div>
    <div class="pesquisa">
       <ul>
      	<li><a href="http://simec.mec.gov.br/par/estado_municipio.php">Ações do MEC no seu Município</a></li>
        <li><a href="http://simec.mec.gov.br/par/estado_municipio_prefeitos.php">Síntese das Ações do MEC</a></li>
      </ul>
    </div>
    <h2>Síntese das Ações do MEC</h2>
  </div>
  <div id="content" class="home">

<style>
            .tabela p{ text-align:left; display:block; float:left; margin-right:12px; }
            .tabela .botao{ background:#4f807c; color:#fff; border:0 }
</style>


<div class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

<p>
      <span><label class="SubTituloDireita" width="30%">UF</label></span>
<span>
      <?

      $sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";

      $db->monta_combo('uf', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF', '', '', '200', 'N', 'uf', '', $_REQUEST['uf']);

      ?>
<span>
</p>

<p>
      <span><label class="SubTituloDireita" width="30%">Município</label></span>

      <span id="td_municipio">

      <? 

            echo "Selecione uma UF";

      ?>
</span>
</p>

      <input type="button" class="botao" value="Enviar" onclick="enviarPainel();">

</div>

    
  </div>
  <div id="lateral">
  </div>
  <div id="rodape">
  	<div id="rodapeTexto">
    	© 2013 Ministério da Educação. Todos os direitos reservados.
    </div>
  </div>
  <div id="rodapeDetalhe">
  	</div>	
</div>

<!-- Fim barra governo -->
<script src="//static00.mec.gov.br/barragoverno/barra.js" type="text/javascript"></script>

</body>
</html>

