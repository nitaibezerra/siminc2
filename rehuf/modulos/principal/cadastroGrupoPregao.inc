<?php

if ( $_REQUEST['insere'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	
	insereGrupo($_REQUEST['grunome']);
	exit;
}

if ( $_REQUEST['atualiza'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	atualizaGrupo($_REQUEST['gruid'], $_REQUEST['grunome']);
	exit;
}

$gruid   = $_REQUEST['gruid'];
$grunome = $_REQUEST['grunome'];

function insereGrupo($grupo){
	global $db;
	
	$sql = "SELECT 
			  gruid,
			  grustatus
			FROM
			  rehuf.grupoitens
			WHERE lower(grunome) = lower('$grupo')";

	$dados = $db->pegaLinha(iconv( "UTF-8", "ISO-8859-1", $sql));
		
	if($dados){ //verifico se nome do grupo já estar cadastrado
		if($dados['grustatus'] == "I"){ //Se grustatus = 'I', alterar o status para 'A'
			$sql = "UPDATE 
					  rehuf.grupoitens  
					SET 
					  grustatus = 'A'
					 
					WHERE 
					  gruid = '{$dados['gruid']}'";
					
			$db->executar( iconv( "UTF-8", "ISO-8859-1", $sql) );
			$res = $db->commit();
			if($res == "1"){
				echo $res;
			}else{
				echo "0";
			}
		}else{
			echo "U";
		}
	}else{//se não estuver cadastrado eu insiro o grupo
		$sql = "INSERT INTO 
				  rehuf.grupoitens(
				  grunome,
				  grustatus
				) 
				VALUES (
				  upper('$grupo'),
				  'A'
				)";
		
		$db->executar( iconv( "UTF-8", "ISO-8859-1", $sql) );
		$res = $db->commit();
		
		if($res == "1"){
			echo $res;
		}else{
			echo "0";
		}
	}	
}

function atualizaGrupo($gruid, $grunome){
	global $db;
	
	$sql = "UPDATE 
			  rehuf.grupoitens  
			SET 
			  grunome = upper('$grunome')
			 
			WHERE 
			  gruid = $gruid";
			
	$db->executar( iconv( "UTF-8", "ISO-8859-1", $sql) );
	$res = $db->commit();
	
	if($res == "1"){
		echo $res;
	}else{
		echo "0";
	}
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( 'Cadastro de Grupo', '' );
?>
<style>
@CHARSET "ISO-8859-1";

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 55%;
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
</style>
<body onload="document.getElementById('grunome').style.textTransform='uppercase';">

<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="frmGrupoPregao" name="frmTabelaApoio" action="" method="post" enctype="multipart/form-data" >

<input type="hidden" name="gruid" id="gruid" value="<?=$gruid; ?>">

<table id="tblGrupoPregao" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Nome do Grupo:</td>
		<td><?=campo_texto( 'grunome', 'S', 'S', '', 40, 50, '', '','','','','id="grunome"',''); ?></td>
	</tr>
	<tr>
		<th align="center" colspan="2"><input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" onclick="validaFormGrupo();">
		<input type="button" value="Cancelar" name="btnCancelar" id="btnCancelar" onclick="Voltar();"></th>
	</tr>
</table>
<div id="erro"> </div>
</form>
<script type="text/javascript">
$('grunome').focus();
function Voltar(){
	window.location.href = 'rehuf.php?modulo=principal/listaGrupoPregao&acao=A';
}
function validaFormGrupo(){
	if($('grunome').value == ""){
		alert('O campo nome do grupo é de preenchimento obrigatório!');
		$('grunome').focus();
		return false;
	}else{
		$('loader-container').show();
		
		if($('gruid').value == ""){
			var myAjax = new Ajax.Request('rehuf.php?modulo=principal/cadastroGrupoPregao&acao=A',{
								method:		'post',
								parameters: '&insere=true&grunome='+$('grunome').value,
								asynchronous: false,
								onComplete: function(res){
									$('erro').innerHTML = res.responseText;
									if( res.responseText == 1 ){
										alert("Grupo cadastrado com sucesso!");
										Voltar();
									}else if(res.responseText == 'U'){
										alert('Grupo já cadastrado');
									}else{
										alert('Operação não realizada, entre em contato com administrador do sistema!');	
									}
								}						
							});
		}else{
			var myAjax = new Ajax.Request('rehuf.php?modulo=principal/cadastroGrupoPregao&acao=A',{
								method:		'post',
								parameters: '&atualiza=true&gruid='+$('gruid').value+'&grunome='+$('grunome').value,
								asynchronous: false,
								onComplete: function(res){
									$('erro').innerHTML = res.responseText;
									if( res.responseText == 1 ){
										alert("Grupo cadastrado com sucesso!");
										Voltar();
									}else{
										alert('Operação não realizada, entre em contato com administrador do sistema!');	
									}
								}						
							});
		}
		
		$('loader-container').hide();
	}
}
</script>
</body>