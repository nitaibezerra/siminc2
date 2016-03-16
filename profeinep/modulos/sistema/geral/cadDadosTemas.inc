<?php 


// ------------------------------------------------------------------------------------
$requisicao = $_REQUEST["requisicao"];
$temaId = $_REQUEST["tasid"]; 
$temaDescricao = pg_escape_string($_REQUEST["tasdsc"]);
// ------------------------------------------------------------------------------------

if($requisicao=="inserir"){
	if( isConcluidaInsercao($temaDescricao) ) {
		exibirMensagemSucessoAndRetornar();
	}
}  

function isConcluidaInsercao($descricao) {
	global $db;
	$sql = "INSERT INTO 
				profeinep.tipoassunto(tasdsc, tasstatus)
			VALUES('".$descricao."', 'A')";
	$db->executar($sql);
	return $db->commit();		
}

if($requisicao=="alterar"){
	if( isConcluidaAlteracao($temaId,$temaDescricao) ) {
		exibirMensagemSucessoAndRetornar();
	}
} 

function isConcluidaAlteracao($temaId, $temaDescricao) {
	global $db;
	$sql = "UPDATE
				profeinep.tipoassunto
			SET
				tasdsc = '" . $temaDescricao ."'
			WHERE
				tasid = ".$temaId;	
	$db->executar($sql);
	return $db->commit();		
}

function exibirMensagemSucessoAndRetornar() {
	echo "<script>
			alert('Dados gravados com sucesso.');
			window.opener.location.href = '?modulo=sistema/geral/listaTemas&acao=A';
			self.close();
		  </script>";
	die();
}



if( $temaId ) {
	global $db;
	$sql = "SELECT * FROM profeinep.tipoassunto WHERE tasid = ".$temaId;
	$dados = $db->carregar($sql);
	extract($dados[0]);
} else {
	$tasid = "";
	$tasdsc = "";
}


monta_titulo( 'PROFE/INEP', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório' );

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script src="../includes/calendario.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<form id="formulario" action="profeinep.php?modulo=sistema/geral/cadDadosTemas&acao=A" method="post" enctype="multipart/form-data">
	<input type="hidden" id="requisicao" name="requisicao" value="1">
	<input type="hidden" id="tasid" name="tasid" value="<?=$tasid?>">
 
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
	    	<td class ="SubTituloDireita" align="right">Descrição:</td>
	        <td>
	        	<?=campo_texto('tasdsc', 'S', 'S', '', 70, 100, '', '', 'left', '',  0, 'id="tasdsc"', '', '', ''); ?>
	       	</td>
	    </tr>
		<tr style="background-color: #c0c0c0; text-align: center;">
			<td colspan="2">
				<input type="button" id="btSalvar" value="Salvar" style="cursor:pointer" onclick="procederCadastro();" />
				<input type="button" id="btFechar" value="Fechar" style="cursor:pointer" onclick="self.close();" />
			</td>
		</tr>
	</table>
</form>

<script>


var btSalvar	=	document.getElementById("btSalvar");
var btFechar	=	document.getElementById("btFechar");

function desabilitarBotoes() {
	btSalvar.disabled	= true;
	btFechar.disabled	= true;	
}

function habilitarBotoes() {
	btSalvar.disabled	= false;
	btFechar.disabled	= false;	
}



function procederCadastro()
{
	desabilitarBotoes();
	if( preenchimentoAceito() ) {
		definirInserirOuAlterar();
		submeterFormulario();
	} 
}

function preenchimentoAceito() {
	var descricao  = document.getElementById("tasdsc");
	if(descricao.value == "") {
		alert("O campo 'Descrição' deve ser preenchido.");
		descricao.focus();
		habilitarBotoes();
		return false;
	}	
	return true;
}

function definirInserirOuAlterar() {
	var id = document.getElementById("tasid");
	var requisicao = document.getElementById("requisicao");
	requisicao.value = ( id==null || id.value=="" ) ? "inserir" : "alterar";

}

function submeterFormulario() {
	var formulario = document.getElementById("formulario");
	formulario.submit();
}

</script>