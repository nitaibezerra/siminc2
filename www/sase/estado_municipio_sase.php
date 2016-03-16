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
<link href="../par/css/estilo.css" type="text/css" rel="stylesheet" />
<link rel="stylesheet" href="../par/barra_governo/css/barra_do_governo.css" type="text/css" />
<script language="javascript" type="text/javascript" src="../par/js/jquery-1.8.1.min.js"></script>
<script language="javascript" type="text/javascript" src="../par/js/acessibilidade.js"></script>
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
		
		window.open('http://painel.mec.gov.br/painel/detalhamentoIndicador/detalhes/municipio/muncod/'+document.getElementById('muncod').value+'/captchadis/1','Indicador','scrollbars=yes,height=700,width=700,status=no,toolbar=no,menubar=no,location=no');
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

<style>
  body{
    background: #EFEFEF !important;
  }
  #menu_janela{
    padding-top:20px;
    padding-bottom:20px;
  }
  #menu_janela div{
    width:250px;
    text-align:center;
    float:left;
  }
  a{
    color:#000;
    background:#dadada;
    margin:1px;
    padding:10px;
    text-decoration:none;
    border:1px solid #ccc;
  }
  a:hover{
    background:#a4a4a4;
    text-decoration: none;
  }

  h2{
    font-size:20px;
  }

  #titulo_janela{
    padding:20px;
    padding-bottom:0px;
    padding-left:29px;
  }

  #form_janela{
    padding:10px;
    padding-left:27px;
  }
  #form_janela div{
    float:left;
  }
  #botao_janela{
    padding:20px;
  }

  .SubTituloDireita{
    font-weight: bold;
  }
</style>

<body class="aumentarFonte">

<div style="border:1px solid #000;width:500px;">

  <div id="menu_janela">
    <div><a style="background:#8f8f8f !important;" href="estado_municipio_sase.php">Ações Detalhadas</a></div>
    <div><a href="estado_municipio_prefeitos_sase.php">Sintese das Ações</a></div>
    <div style="clear:both"></div>
  </div>

  <div id="titulo_janela">
    <h2>Ações Detalhadas</h2>
  </div>
  <hr/>

  <div id="form_janela">
    <div style="margin-right:20px;">
      <label class="SubTituloDireita" width="30%">UF:</label><br/>
      <span>
      <?php
      $sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
      $db->monta_combo('uf', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF', '', '', '200', 'N', 'uf', '', $_REQUEST['uf']); ?>
      <span>
    </div>

    <div>
      <label class="SubTituloDireita">Município:</label><br/>
      <span id="td_municipio"> 
      <?php echo "Selecione uma UF"; ?>
      </span>
    </div>
    
  </div>

  <div style="clear:both"></div>
  <div id="botao_janela">
    <input type="button" class="botao" value="Enviar" onclick="enviarPainel();">
  </div>

</div>

</body>
</html>

