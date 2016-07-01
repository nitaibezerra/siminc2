<?php
if($_REQUEST['download'] == 'S'){
	$arqid = $_REQUEST['arqid'];
	$sql = "select arqid from fabrica.anexoordemservico where arqid = $arqid --and taoid != ".TPANEXO_RELATORIO_PF;
	$arqid = $db->pegaUm($sql);
	if(!$arqid){
		echo"<script>alert('Arquivo indisponível!')</script>";
		echo"<script>window.location.href = '".$_SERVER['HTTP_REFERER']."';</script>";
    	exit;
	}
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = '".$_SERVER['HTTP_REFERER']."';</script>";
    exit;
}

$sql = "SELECT os.docid, scs.scsid, ans.ansid, odsdetalhamento, odsqtdpfestimada, odsid, to_char(odsdtprevinicio, 'dd/mm/YYYY') as odsdtprevinicio, to_char(odsdtprevtermino, 'dd/mm/YYYY') as odsdtprevtermino
		FROM fabrica.ordemservico os
		inner join fabrica.solicitacaoservico scs ON scs.scsid = os.scsid
		inner join fabrica.analisesolicitacao ans ON ans.scsid = scs.scsid
		WHERE odsid = (select odsidpai from fabrica.ordemservico where odsid = {$_REQUEST['odsid']} ) ORDER BY odsid";

// buscando dados da OS
$oss = $db->pegaLinha($sql);

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

//abas
$menu[0] = array("descricao" => "Listar OS", "link"=> "fabrica.php?modulo=principal/listarOrdemServico&acao=A");
$menu[1] = array("descricao" => "Dados OS de Origem", "link"=> "fabrica.php?modulo=principal/contagemOS&acao=A&odsid={$_REQUEST['odsid']}&scsid={$_REQUEST['scsid']}");
$menu[2] = array("descricao" => "Contagem P.F.", "link"=> "fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid={$_REQUEST['odsid']}&scsid={$_REQUEST['scsid']}");

echo montarAbasArray($menu, "fabrica.php?modulo=principal/contagemOS&acao=A&odsid={$_REQUEST['odsid']}&scsid={$_REQUEST['scsid']}");

$link = "<span style='cursor:pointer; color: #0066CC;' onclick=\"window.open('fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid=".$oss['odsid']."', 'Observacoes','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');\">".$oss['odsid']."</span>";


$titulo = "Dados de Origem da OS Nº {$link}";
monta_titulo( $titulo, "Fiscal Técnico" );
?>
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
<form name="form_cad_contrato" id="form_cad_contrato" method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubTituloDireita">Número:</td>
			<td><? echo $link; ?></td>
		</tr>
		<tr>
		<td class="SubTituloDireita">Disciplinas e Produtos:</td>
		<td>
		<?php $arrDisciplina = carregarDisciplinasProdutosPorAnalise($oss['ansid']); ?>
			<?php if(is_array($arrDisciplina)): ?>
				<?php foreach($arrDisciplina as $disc): ?>
						<?php
						$arrContratada[$disc['disciplina']]['dpedsc'] = $disc['disciplina'];
						$arrContratada[$disc['disciplina']]['produtos'][] = $disc['produto'];
						?>
				<?php endforeach; ?>
			<?php endif; ?>
			<?php if(is_array($arrContratada)): ?>
				<?php $n=1; ?>
				<?php foreach($arrContratada as $d): ?>
						<b><?php echo $n.") ".$d['dpedsc'] ?></b><?php echo $d['produtos'][0] ? ": ".implode(", ",$d['produtos']).";" : ";"  ?><br />
						<?php $n++; ?>
				<?php endforeach; ?>
			<?php else: ?>
				N/A
			<?php endif; ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Previsão Início:</td>
			<td><? echo $oss['odsdtprevinicio']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Previsão Término:</td>
			<td><? echo $oss['odsdtprevtermino']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Detalhamento:</td>
			<td><?  echo $oss['odsdetalhamento']; ?></td>
		</tr>
		<tr>
			<td colspan="2" class="TituloTabela center">Anexos</td>
		</tr>
		<tr>
			<td colspan="2">
			<?php
			$sql = "SELECT to_char(an.aosdtinclusao,'dd/mm/YYYY'), tp.taodsc,
						'<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'{$_SERVER['REQUEST_URI']}&download=S&arqid=' || ar.arqid || '\';\" />' || ar.arqnome||'.'||ar.arqextensao ||'</a>',
						ar.arqtamanho, an.aosdsc
					FROM fabrica.anexoordemservico an
					LEFT JOIN fabrica.tipoanexoordem tp ON an.taoid=tp.taoid
					LEFT JOIN public.arquivo ar ON ar.arqid=an.arqid
					WHERE odsid='".$oss['odsid']."' AND aosstatus='A'
					--AND tp.taoid != ".TPANEXO_RELATORIO_PF."
					";

			$cabecalho = array("Data inclusão", "Tipo Arquivo", "Nome arquivo", "Tamanho(bytes)", "Descrição");
			$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
			?>
			</td>
		</tr>
	</table>
</form>