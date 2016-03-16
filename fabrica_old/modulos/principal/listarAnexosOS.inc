<?

if($_POST['aosid']){
	$flag = false;
	foreach($_POST['aosid'] as $aosid){
		if($_POST['taoid_'.$aosid]){
			$sql = "UPDATE fabrica.anexoordemservico 
					SET taoid = ".$_POST['taoid_'.$aosid]." 
					WHERE aosid = ".$aosid;
			$db->executar($sql);
			$flag = true;
		}
	}
	
	if($flag) {
		$db->commit();
		die("<script>
				alert('Operação Efetuada com Sucesso.');
				location.href = 'fabrica.php?modulo=principal/listarAnexosOS&acao=A';
			</script>");
	}
	
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

echo '<br>';

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="./js/fabrica.js"></script>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script type="text/javascript">

function limparCampos(){
	$('input:not(:submit,:hidden,:button,[readonly=""],[readonly="readonly"]),textarea,select').val("");
}

function submeteFormulario()
{
	/*
	$('#usunomerequisitante option').each(function()
	{
		$(this).attr('selected',true);
	});
	$('#usufiscal option').each(function()
	{
		$(this).attr('selected',true);
	});
	*/
	$('#form_listar').submit();
}
</script>
<?php

$titulo_modulo = "Listar Anexos OS";
monta_titulo($titulo_modulo, '');



//recupera perfis de usuario
$pfls = arrayPerfil();

//permissao PERFIL_PREPOSTO
/*
if(in_array(PERFIL_PREPOSTO,$pfls) && !in_array(PERFIL_SUPER_USUARIO,$pfls) && !in_array(PERFIL_ADMINISTRADOR, $pfls)){
	$andPreposto = " AND esd.esdid = ".WF_ESTADO_DETALHAMENTO;
}
*/

?>

<form name="formulario" id="form_listar" method="post" action="">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita" width="25%">Nº OS</td>
			<td><input class="normal" type="text" name="odsid" value="<?php echo $_REQUEST['odsid']; ?>"></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%">Tipo OS</td>
			<td><?php 
 				    $sql = "SELECT 
					tosid as codigo,
					tosdsc as descricao
					FROM fabrica.tipoordemservico 
					WHERE tosstatus = 'A'
					and tosid in (2,3)
					ORDER BY descricao";
	  			   $db->monta_combo('tosid', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'tosid', '', $_REQUEST['tosid']); 
			?></td>
		</tr>		
		<tr>
			<td></td>
			<td >
				<input type="button" value="Pesquisar" onclick="submeteFormulario();">
				&nbsp;
				<input type="button" value="Limpar" id="limpar" onclick="limparCampos();">
			</td>
		</tr>
	</table>
	<input type="hidden" name="pesquisar" value="pesquisar">


<?php

$sql = "SELECT 
			os.odsid,
			os.scsid,
			tos.tosdsc,
			o.aosdsc,
			to_char(aosdtinclusao::timestamp,'DD/MM/YYYY HH24:MI') AS dtanexo,
			u.usunome, 
			t.taodsc,
			'<input type=hidden name=aosid[] value=\"'||o.aosid||'\"><select name=taoid_'||o.aosid||'><option value>Selecione</option><option value=28>Relatório de contagem de PF Estimada</option><option value=29>Relatório de contagem de PF Detalhada</option></select>' as tpcontagem
		FROM fabrica.ordemservico os 
		LEFT JOIN fabrica.tipoordemservico tos ON tos.tosid=os.tosid
		LEFT JOIN fabrica.anexoordemservico o on o.odsid = os.odsid
	    LEFT join fabrica.tipoanexoordem t on t.taoid = o.taoid
	    LEFT join public.arquivo a on a.arqid = o.arqid
	    LEFT join seguranca.usuario u on u.usucpf = a.usucpf
		WHERE o.aosstatus='A' and os.tosid in (2,3) and o.taoid = 26
		";
//dbg($sql,1);
if( isset($_REQUEST['pesquisar']) ){


	if($_REQUEST['tosid'])
	{
		$sql .= " AND os.tosid = ".$_REQUEST['tosid']." ";
	}	
	if(is_numeric($_REQUEST['odsid'])){
		$sql .= " AND os.odsid = '{$_REQUEST['odsid']}' ";
	}
	
}
$sql .= " ORDER BY 1 ";



$cabecalho     = array("Nº OS", "Nº SS", "Tipo OS", "Nome do Arquivo", "Data Inclusão", "Incluído por", "Tipo Anexo Atual", "Tipo Anexo Novo");
$tamanho       = array("10%","10%","20%","30%","10%","15%","15%","15%");
$alinhamento   = array("center","center","left","left","center","left","left","left");
//$db->monta_lista_simples($sql,$cabecalho,50,5,'N','center',$par2,"",$tamanho,$alinhamento);
$db->monta_lista_simples($sql,$cabecalho,50,5,'N','95%');

?>
<center>
<input type="button" value="Salvar" onclick="submeteFormulario();">
</center>

</form>

