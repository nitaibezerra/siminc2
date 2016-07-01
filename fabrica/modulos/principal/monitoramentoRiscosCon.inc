<?
include APPRAIZ ."includes/workflow.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

echo '<br>';

if($_REQUEST['scsid']) {
	$_SESSION['fabrica_var']['scsid'] = $_REQUEST['scsid']; 
}

if($_REQUEST['mntid']) {
	
	$sql = "SELECT m.mntid, m.mntdsc, m.etpid, m.mntitem, m.mntdsc, e.aatid 
			FROM fabrica.monitorarisco m 
			LEFT JOIN fabrica.etapaareaatuacao e ON m.etpid=e.etpid 
			WHERE scsid='".$_SESSION['fabrica_var']['scsid']."'";
	
	$monitorarisco = $db->pegaLinha($sql);
	$aatid = $monitorarisco['aatid'];
	$etpid = $monitorarisco['etpid'];
	$mntitem = $monitorarisco['mntitem'];
	$mntdsc = $monitorarisco['mntdsc'];

	$estado_wf = wf_pegarEstadoAtual($analisesolicitacao['docid']);
	$requisicao = "atualizarMonitoramentoRiscos";
	$formhabil = "S";
	
	
} else {
	$formhabil = "S";
	$requisicao = "inserirMonitoramentoRiscos";
}

switch($_REQUEST['acao']) {
	case 'A':
		$menu = array(0 => array("id" => 1, "descricao" => "Avaliar / Aprovar Solicitação",      "link" => "/fabrica/fabrica.php?modulo=principal/cadAvaliacaoAprovacaoCon&acao=A&ansid=".$_SESSION['fabrica_var']['ansid']."&scsid=".$_SESSION['fabrica_var']['scsid']),
				  1 => array("id" => 2, "descricao" => "Observações", 						 "link" => "/fabrica/fabrica.php?modulo=principal/cadSSObservacaoCon&acao=A&tipoobs=cadAvaliacaoAprovacao"),
				  2 => array("id" => 3, "descricao" => "Anexos da ordem de serviço",         "link" => "/fabrica/fabrica.php?modulo=principal/cadDetalhamentoAnexosCon&acao=C"),
				  3 => array("id" => 4, "descricao" => "Providências", 		 		 		 "link" => "/fabrica/fabrica.php?modulo=principal/providenciasCon&acao=A&tipoaba=cadAvaliacaoAprovacao")
			  	  );
		break;
	case 'C':
		$menu = array(0 => array("id" => 1, "descricao" => "Executar Solicitação",      		 "link" => "/fabrica/fabrica.php?modulo=principal/cadExecucaoCon&acao=A&ansid=".$_SESSION['fabrica_var']['ansid']."&scsid=".$_SESSION['fabrica_var']['scsid']),
				  1 => array("id" => 2, "descricao" => "Observações", 						 "link" => "/fabrica/fabrica.php?modulo=principal/cadSSObservacaoCon&acao=A&tipoobs=cadExecucao"),
				  2 => array("id" => 3, "descricao" => "Anexos da ordem de serviço",         "link" => "/fabrica/fabrica.php?modulo=principal/cadDetalhamentoAnexosCon&acao=D"),
				  3 => array("id" => 4, "descricao" => "Monitoramento / Controle de riscos", "link" => "/fabrica/fabrica.php?modulo=principal/monitoramentoRiscosCon&acao=C"),
				  4 => array("id" => 5, "descricao" => "Providências", 		 		 	 	 "link" => "/fabrica/fabrica.php?modulo=principal/providenciasCon&acao=A&tipoaba=cadExecucao")
			  	  );
		break;
}

echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

$titulo_modulo = "Fábrica de softwares";
monta_titulo($titulo_modulo, 'Análise da demanda');

//permissao para edição da tela
if(!verificaPermissaoEdicao()) $formhabil = 'N';

$formhabil = 'N';


?>
<!-- (INÍCIO) BIBLIOTECAS - PARA USO DOS COMPONENTES (CALENDÁRIO E SLIDER) -->
<script	language="javascript" type="text/javascript" src="../includes/blendtrans.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/slider/slider.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>

<script language="javascript" type="text/javascript">

function selecionarAreaAtuacaoTI(aatid) {

	document.getElementById('tdfase').innerHTML = "Carregando...";
	
	$.ajax({
   		type: "POST",
   		url: "fabrica.php?modulo=principal/monitoramentoRiscos&acao=A",
   		data: "requisicao=pegarFasesPorArea&aatid="+aatid,
   		success: function(msg){
   			document.getElementById('tdfase').innerHTML = msg;
   		}
 		});

	
}

function submeterMonitoramentoRiscos() {
	
	if(document.getElementById('aatid').value == '') {
		alert('Selecione uma área de atuação');
		return false;
	}
	
	if(document.getElementById('etpid').value == '') {
		alert('Selecione uma etapa');
		return false;
	}

	if(document.getElementById('mntitem').value.length == 0) {
		alert('Digite o item');
		return false;
	}

	if(document.getElementById('mntdsc').value.length == 0) {
		alert('Digite uma descrição');
		return false;
	}

	
	divCarregando();
	document.getElementById('formulario_').submit();
}

</script>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<form method="post" name="formulario_" id="formulario_" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="<? echo $requisicao; ?>">
<input type="hidden" name="mntid" value="<? echo $monitorarisco['mntid']; ?>">
<?

telaCabecalhoSolicitacaoServico(array("scsid" => $_SESSION['fabrica_var']['scsid']));

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td width="100%">
	<table class="listagem" width="100%">
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Análise Preliminar da solicitação de serviço</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Área de atuação TI:</td>
		<td><? 
		if($db->testa_superuser()) {
			$sql = "SELECT a.aatid as codigo, a.aatdsc as descricao FROM fabrica.areaatuacao a WHERE aatstatus='A'";			
		}

		$db->monta_combo('aatid', $sql, $formhabil, 'Selecione', 'selecionarAreaAtuacaoTI', '', '', '', 'S', 'aatid');
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Fase:</td>
		<td id="tdfase"><?

		if($formhabil == "S") {
			$habil = "N";
			
			if($aatid) {
				$habil = "S";
				$sql = "SELECT e.etpid as codigo, e.etpdsc as descricao FROM fabrica.etapa e 
						LEFT JOIN fabrica.etapaareaatuacao ea ON ea.etpid=e.etpid 
						WHERE ea.aatid='".$aatid."'";
			}
			
		} else {
			$habil = "N";
		}
		$db->monta_combo('etpid', $sql, $habil, 'Selecione', '', '', '', '', 'S', 'etpid');
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Item de risco:</td>
		<td><? echo campo_texto('mntitem', 'S', $formhabil, 'Item de risco', 66, 200, '', '', '', '', 0, 'id="mntitem"' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Descrição:</td>
		<td><? echo campo_textarea( 'mntdsc', 'S', $formhabil, '', '70', '4', '5000'); ?></td>
	</tr>

	</table>
	
	</td>

</tr>
<tr>
	<td class="SubTituloDireita">
	<? if($formhabil == "S") { ?>
	<input type="button" name="inseriranalise" value="Enviar" onclick="submeterMonitoramentoRiscos();">
	<? } ?>
	<input type="button" name="voltar" value="Voltar" onclick="window.location='fabrica.php?modulo=principal/listarSolicitacoes&acao=A';">
	</td>
</tr>
</table>
</form>
<?
$cabecalho = array("","Área de atuação","Fase","Item de risco");
$sql = "SELECT '<center>".(($formhabil=="S")?"<img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'fabrica.php?modulo=principal/monitoramentoRiscos&acao=A&mntid='||m.mntid||'\';\">":"<img src=\"/imagens/alterar_pb.gif\" border=0 title=\"Editar\">")."</center>' as acao, 
			   aa.aatdsc, e.etpdsc, 
			   m.mntitem 
		FROM fabrica.monitorarisco m 
		LEFT JOIN fabrica.etapa e ON e.etpid = m.etpid 
		LEFT JOIN fabrica.etapaareaatuacao ea ON ea.etpid=e.etpid 
		LEFT JOIN fabrica.areaatuacao aa ON aa.aatid=ea.aatid  
		WHERE m.scsid='".$_SESSION['fabrica_var']['scsid']."' ORDER BY mntid";
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
?>
