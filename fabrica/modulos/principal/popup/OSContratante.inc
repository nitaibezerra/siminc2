<?php
$odsid = $_GET['odsid'];
$tosid = $_GET['tosid'];

if($odsid){
	$sql = "select * from fabrica.ordemservico where odsid = $odsid";
	$dadosOS = $db->pegaLinha($sql);
	extract($dadosOS);
	
	$sql = "select esdid FROM workflow.documento doc 
			inner join fabrica.ordemservico osv on osv.docidpf = doc.docid 
			where odsidpai=$odsid";
	$esdid = $db->pegaUm($sql);
}


//permissao para edição da tela
$habil = 'S';
if($esdid == WF_ESTADO_CPF_AGUARDANDO_CONTAGEM) {
	$habil = 'N';
} else {
	$habil = 'S';
}

if(!verificaPermissaoEdicao()) $habil = 'N';



$sql = "SELECT a.ansdtrecebimento, s.docid, to_char(a.ansprevinicio,'dd/mm/YYYY') as ansprevinicio, to_char(a.ansprevtermino,'dd/mm/YYYY') as ansprevtermino FROM fabrica.analisesolicitacao a 
		LEFT JOIN fabrica.solicitacaoservico s ON a.scsid=s.scsid 
		WHERE ansid='".$_SESSION['fabrica_var']['ansid']."'";
$dadosan = $db->pegaLinha($sql);

if($_POST['requisicao']){
	$msg = $_POST['requisicao']($_POST);
}

$titulo = "Detalhar OS da Contratante".($odsid ? " $odsid" : "");
monta_titulo( $titulo, obrigatorio().'Indica campo obrigatório.' );

?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<style>
	.TituloTabela{background-color: #C5C5C5;font-weight:bold}
	.center{text-align:center}
	.link{cursor: pointer;}
	.middle{vertical-align: middle;}
</style>
<script>
	function limparCampos()
	{
		$('input:not(:hidden,:button,[readonly=""],[readonly="readonly"]),textarea,select').val("");
	}
	
	function salvarOS2()
	{

		if(document.form_cad_os.odsdetalhamento.value == ''){
			alert("Preencha a Descrição do serviço");
			document.form_cad_os.odsdetalhamento.focus();
			return false;
		}
		if(document.form_cad_os.odsdtprevinicio.value == ''){
			alert("Preencha a Previsão de início");
			document.form_cad_os.odsdtprevinicio.focus();
			return false;
		}
		if(document.form_cad_os.odsdtprevtermino.value == ''){
			alert("Preencha a Previsão de Término");
			document.form_cad_os.odsdtprevtermino.focus();
			return false;
		}
		if(document.form_cad_os.odsqtdpfestimada.value == ''){
			alert("Preencha a Qtd. de P.F. Estimado");
			document.form_cad_os.odsqtdpfestimada.focus();
			return false;
		}
		
			 
		if(document.getElementById('odsdtprevinicio').value && document.getElementById('odsdtprevtermino').value) {
		
			var data_an1 = '<? echo $dadosan['ansprevinicio']; ?>';
			var data_an2 = '<? echo $dadosan['ansprevtermino']; ?>';
			
			var data_1 = document.getElementById('odsdtprevinicio').value;
			var data_2 = document.getElementById('odsdtprevtermino').value;
		
			var compara_an1 = parseInt(data_an1.split("/")[2].toString() + data_an1.split("/")[1].toString() + data_an1.split("/")[0].toString());	
			var compara_an2 = parseInt(data_an2.split("/")[2].toString() + data_an2.split("/")[1].toString() + data_an2.split("/")[0].toString());
			var compara01 = parseInt(data_1.split("/")[2].toString() + data_1.split("/")[1].toString() + data_1.split("/")[0].toString());  
			var compara02 = parseInt(data_2.split("/")[2].toString() + data_2.split("/")[1].toString() + data_2.split("/")[0].toString());
			  
			if (compara01 > compara02) {  
				alert('Data de início não pode ser maior que a data de termino');
				return false;  
			}
			
			if (compara_an1 > compara01) {  
				alert('As datas de previsão não pode menor do que a data prevista na analise');
				return false;  
			}
		
			if (compara_an2 < compara02) {  
				alert('As datas de previsão não pode maior do que a data prevista na analise');
				return false;  
			}
			
		}
		
		$('#form_cad_os').submit();
	}
	
	function alterarOSMEC(odsid)
	{
		window.location.href = 'fabrica.php?modulo=principal/popup/OSContratante&acao=A&odsid=' + odsid;
	}
	
	function novaOS()
	{
		window.location.href = 'fabrica.php?modulo=principal/popup/OSContratante&acao=A';
	}
</script>
<form name="form_cad_os" id="form_cad_os" method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="20%" class="SubTituloDireita">Descrição do serviço:</td>
			<td>
				<? echo campo_textarea( 'odsdetalhamento', 'S', $habil, '', '70', '4', '1000'); ?>
				<input type="hidden" name="requisicao" value="salvarOSContratante" />
				<input type="hidden" name="odsid" value="<?=$odsid?>" />
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Previsão de início:</td>
			<td><? echo campo_data2('odsdtprevinicio','S', $habil, 'Previsão de início', 'S' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Previsão de Término:</td>
			<td><? echo campo_data2('odsdtprevtermino','S', $habil, 'Previsão de Término', 'S' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Qtd. de P.F. Estimado:</td>
			<td><? echo campo_texto('odsqtdpfestimada', 'S', $habil, 'Qtd. de P.F. Estimado', 10, 11, '[#].###,##', '', '', '', 0, 'id="odsqtdpfestimada"' ); ?></td>
		</tr>
		<tr>
			<td colspan="2" class="TituloTabela center">
				<? if($habil=='S'): ?>
				<input type="button" onclick="salvarOS2()" value="Salvar" >
				<input type="button" onclick="limparCampos()" value="Limpar Campos" >
				<? endif; ?>
				<?php if($odsid): ?>
					<input type="button" onclick="novaOS()" value="Nova" >
				<?php endif; ?>

			</td>
		</tr>
		<tr>
			<td class="TituloTabela center" colspan="2">Ordens de Seviço da Contratante</td>
		</tr>
	</table>
</form>
<?php
$cabecalho = array("Ação","Nº","Detalhamento","Previsão início","Previsão término", "Qtd. PF");
$sql = "SELECT '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"alterarOSMEC(\''||odsid||'\');\"></center>' as acao, '<center>'||odsid||'</center>', odsdetalhamento, to_char(odsdtprevinicio, 'dd/mm/YYYY') as odsdtprevinicio, to_char(odsdtprevtermino, 'dd/mm/YYYY') as odsdtprevtermino, odsqtdpfestimada FROM fabrica.ordemservico WHERE odscontratada is false and scsid='".$_SESSION['fabrica_var']['scsid']."' ORDER BY odsid";
$db->monta_lista($sql,$cabecalho,100,5,'S','center',$par2);
?>
<?php if($msg): ?>
<script>
	alert('<?php echo $msg ?>');
	window.opener.location.reload();
</script>
<?php endif; ?>