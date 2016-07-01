<?php

function removerperiodoreferencia($dados) {
	global $db;
	$sql = "DELETE FROM cte.execucaosubacao WHERE perid='".$dados['perid']."'";
	$db->executar($sql);
	$sql = "DELETE FROM cte.periodoreferencia WHERE perid='".$dados['perid']."'";
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('Período removido com sucesso');
			window.location='?modulo=principal/monitora_periodoreferencia&acao=A';
		  </script>";
	exit;
}

function atualizarperiodoreferencia($dados) {
	global $db;
	$sql = "UPDATE cte.periodoreferencia
   			SET perdsc='".$dados['perdsc']."', perdtiniciocad='".formata_data_sql($dados['perdtiniciocad'])."', perdtterminocad='".formata_data_sql($dados['perdtterminocad'])."', 
       		perdtinicioref='".$dados['ano'].$dados['perdtinicioref_mes']."', perdtterminoref='".$dados['ano'].$dados['perdtterminoref_mes']."'
 			WHERE perid='".$dados['perid']."'";
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('Período atualizado com sucesso');
			window.location='?modulo=principal/monitora_periodoreferencia&acao=A';
		  </script>";
	exit;
}
function inserirperiodoreferencia($dados) {
	global $db;
	$sql = "INSERT INTO cte.periodoreferencia(
            perdsc, perstatus, perdtiniciocad, perdtterminocad, perdtinicioref, 
            perdtterminoref)
    		VALUES ('".$dados['perdsc']."', 'A', '".formata_data_sql($dados['perdtiniciocad'])."', '".formata_data_sql($dados['perdtterminocad'])."',
    		'".$dados['ano'].$dados['perdtinicioref_mes']."','".$dados['ano'].$dados['perdtterminoref_mes']."');";
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('Período inserido com sucesso');
			window.location='?modulo=principal/monitora_periodoreferencia&acao=A';
		  </script>";
	exit;
	
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
}

$msg = '';
include_once APPRAIZ . 'includes/workflow.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$titulo_modulo = "PAR - Plano de Metas";
monta_titulo( $titulo_modulo,'Monitoramento do PAR');

if($_REQUEST['perid']) {
	$requisicao = 'atualizarperiodoreferencia';
	$periodoreferencia = $db->pegaLinha("SELECT * FROM cte.periodoreferencia WHERE perid='".$_REQUEST['perid']."'");
	if($periodoreferencia) {
		$perdsc = $periodoreferencia['perdsc'];
		$perdtiniciocad = $periodoreferencia['perdtiniciocad'];
		$perdtterminocad = $periodoreferencia['perdtterminocad'];
		$ano = substr($periodoreferencia['perdtinicioref'],0,4);
		$perdtinicioref_mes = substr($periodoreferencia['perdtinicioref'],4,2);
		$perdtterminoref_mes = substr($periodoreferencia['perdtterminoref'],4,2);
		
	} else {
		echo "<script>
				alert('Erro em paramêtros. Entre em contato com a equipe técnica');
				window.location='?modulo=principal/monitora_periodoreferencia&acao=A';
			  </script>";
		exit;
	}
	
} else {
	$requisicao = 'inserirperiodoreferencia';
}
?>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script>
function Excluir(url, msg) {
	if(confirm(msg)) {
		window.location = url;
	}
}
function validarperiodoreferencia() {
	if(document.formulario.elements['perdsc'].value == '') {
		alert("'Descrição' é obrigatório");
		return false;
	}
	if(document.formulario.elements['perdtiniciocad'].value == '') {
		alert("'Data início de cadastramento' é obrigatório");
		return false;
	}
	if(document.formulario.elements['perdtterminocad'].value == '') {
		alert("'Data fim de cadastramento' é obrigatório");
		return false;
	}

	if(document.formulario.elements['perdtinicioref_mes'].value == '') {
		alert("'Mês início de monitoramento' é obrigatório");
		return false;
	}
	if(document.formulario.elements['perdtterminoref_mes'].value == '') {
		alert("'Mês fim de monitoramento' é obrigatório");
		return false;
	}
	if(document.formulario.elements['ano'].value == '') {
		alert("'Ano de monitoramento' é obrigatório");
		return false;
	}

	
	document.getElementById('btnsalvar').disabled=true;
	document.getElementById('formulario').submit();
}
</script>
<form action="?modulo=principal/monitora_periodoreferencia&acao=A" method="post" name="formulario" id="formulario">
<input type="hidden" name="requisicao" value="<? echo $requisicao; ?>">
<? if($_REQUEST['perid']){ ?>
<input type="hidden" name="perid" value="<? echo $_REQUEST['perid']; ?>">
<? }?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
	<td class="SubTituloDireita">Descrição :</td>
	<td><? echo campo_texto('perdsc', "N", "S", "Número de vagas ofertadas", 50, 250, "", "", '', '', 0, '' ); ?></td>
	</tr>
	
	<tr>
	<td class="SubTituloDireita">Data início de cadastramento :</td>
	<td><? echo campo_data( 'perdtiniciocad', 'N', 'S', '', 'S' ); ?></td>
	</tr>
	<tr>
	<td class="SubTituloDireita">Data fim de cadastramento :</td>
	<td><? echo campo_data( 'perdtterminocad', 'N', 'S', '', 'S' ); ?></td>
	</tr>
	<tr>

	
	<tr>
	<td class="SubTituloDireita">Data de monitoramento :</td>
	<td>de <?
	$sql = "SELECT mescod as codigo, mesdsc as descricao FROM public.meses"; 
	$db->monta_combo('perdtinicioref_mes', $sql, 'S', "Selecione...", '', '', '', '', 'N');
	echo " até ";
	$sql = "SELECT mescod as codigo, mesdsc as descricao FROM public.meses"; 
	$db->monta_combo('perdtterminoref_mes', $sql, 'S', "Selecione...", '', '', '', '', 'N');
	echo " do ano ";
	$sql = "SELECT ano as codigo, ano as descricao FROM public.anos ORDER BY ano"; 
	$db->monta_combo('ano', $sql, 'S', "Selecione...", '', '', '', '', 'N');
	?></td>
	</tr>
	<tr bgcolor="#C0C0C0">
	<td colspan="2" align="center"><input type='button' id='btnsalvar' onclick="return validarperiodoreferencia();" value='Salvar'> <input type='button' onclick="window.location='?modulo=principal/monitora_periodoreferencia&acao=A';" value='Novo'></td>
	</tr>
</table>
</form>
<?

$sql = "SELECT '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/monitora_periodoreferencia&acao=A&perid='|| perid ||'\'\");\"> <img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"Excluir(\'?modulo=principal/monitora_periodoreferencia&acao=A&requisicao=removerperiodoreferencia&perid='|| perid ||'\',\'A exclusão deste período de referência apagará registros de Execução do monitoramento. Deseja realmente excluir?\');\">', perdsc, 'de '||to_char(perdtiniciocad, 'dd/mm/YYYY')||' até '||to_char(perdtterminocad, 'dd/mm/YYYY') as periodocadastramento, 'de '||SUBSTR(perdtinicioref,5,2)||'/'||SUBSTR(perdtinicioref,1,4)||' ate '||SUBSTR(perdtterminoref,5,2)||'/'||SUBSTR(perdtterminoref,1,4) as periodoreferencia FROM cte.periodoreferencia";
$db->monta_lista( $sql, array("","Descrição","Período de cadastramento","Período de monitoramento"), 100, 10, 'N', 'center', '' );
?>
