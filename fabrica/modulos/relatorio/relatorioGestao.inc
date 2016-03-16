<?php
ini_set( "memory_limit", "512M" );
set_time_limit(0);

include APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";
$db->cria_aba($abacod_tela,$url,'');

monta_titulo( 'Relatório Gestão', 'Selecione os filtros desejados' );

extract( $_REQUEST );
?>
<form action="" method="post" name="formulario" id="formulario"> 
<input type="hidden" name="requisicao" id="requisicao" value=""/>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td class="SubTituloDireita">SS:</td>
		<td>
			<?=campo_texto( 'scsid', 'N', 'S', '', 20, 50, '[#]', '', 'left', '', 0, 'id="scsid"'); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Sistema:</td>
		<td>
			<?
			$sql = "SELECT DISTINCT
						sid.sidid as codigo,
						sid.siddescricao as descricao
					FROM fabrica.fasedisciplinaproduto fdp
						inner join fabrica.servicofaseproduto s on s.fdpid = fdp.fdpid
						inner join fabrica.analisesolicitacao a on a.ansid = s.ansid
						inner join fabrica.solicitacaoservico sse on sse.scsid = a.scsid
						inner join demandas.sistemadetalhe sid ON sid.sidid=sse.sidid
					order by sid.siddescricao";
			$db->monta_combo("sidid", $sql, "S", "Selecione...", '', '', '', '250', 'N','sidid');
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Disciplina:</td>
		<td>
			<?
			$sql = "SELECT 
					  dspid as codigo,
					  dspdsc as descricao  
					FROM 
					  fabrica.disciplina
					WHERE
						dspstatus = 'A'	
					ORDER BY
						dspdsc";
			
			$db->monta_combo("dspid", $sql, "S", "Selecione...", '', '', '', '250', 'N','dspid');
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Fase:</td>
		<td>
			<?
			$sql = "SELECT 
					  fasid as codigo,
					  fasdsc as descricao  
					FROM 
					  fabrica.fase
					WHERE
						fasstatus = 'A'	
					ORDER BY
						fasdsc";
			$db->monta_combo("fasid", $sql, "S", "Selecione...", '', '', '', '250', 'N','fasid');
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Artefato:</td>
		<td>
			<?
			$sql = "SELECT 
					  prdid as codigo,
					  prddsc as descricao  
					FROM 
					  fabrica.produto p
					WHERE
						prdstatus = 'A'	
					ORDER BY
						prddsc";
			$db->monta_combo("prdid", $sql, "S", "Selecione...", '', '', '', '250', 'N','prdid');
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Situação:</td>
		<td>
			<?
			$sql = "SELECT DISTINCT 
						ed.esdid as codigo,
						ed.esddsc as descricao
					FROM fabrica.fasedisciplinaproduto fdp
						inner join fabrica.servicofaseproduto s on s.fdpid = fdp.fdpid
						inner join fabrica.analisesolicitacao a on a.ansid = s.ansid
						inner join fabrica.solicitacaoservico sse on sse.scsid = a.scsid
						inner join workflow.documento doc on doc.docid = sse.docid
						inner join workflow.estadodocumento ed on ed.esdid = doc.esdid
					ORDER BY ed.esddsc";
			
			$db->monta_combo("esdid", $sql, "S", "Selecione...", '', '', '', '250', 'N','esdid');
			?>
		</td>
	</tr>
	<tr bgcolor="#CCCCCC">
		<td></td>
		<td>				
			 <input type="button" value="Visualizar" onclick="exibeRelatorioGestao('exibir');" style="cursor: pointer;"/>
			 <input type="button" value="Exportar Excel" onclick="exibeRelatorioGestao('exportar');" style="cursor: pointer;"/>
			 <input type="button" value="Limpar Filtros" onclick="exibeRelatorioGestao('limpar');" style="cursor: pointer;"/>
		</td>
	</tr>
</table>
</form>
<script type="text/javascript">
function exibeRelatorioGestao(acao){
	document.getElementById('requisicao').value = acao;
	if(acao == 'limpar'){
		window.location.href = window.location
	} else {
		document.getElementById('formulario').submit();
	}
}
</script>
<?
$filtro = '';
if( $_POST['scsid'] ) $filtro .= ' and a.scsid = '.$_POST['scsid'];
if( $_POST['sidid'] ) $filtro .= ' and sid.sidid = '.$_POST['sidid'];
if( $_POST['dspid'] ) $filtro .= ' and d.dspid = '.$_POST['dspid'];
if( $_POST['fasid'] ) $filtro .= ' and f.fasid = '.$_POST['fasid'];
if( $_POST['prdid'] ) $filtro .= ' and p.prdid = '.$_POST['prdid'];
if( $_POST['esdid'] ) $filtro .= ' and ed.esdid = '.$_POST['esdid'];

$sql = "SELECT 
			a.scsid as ss, 
			sid.siddescricao as sistema, 
			d.dspdsc as disciplina, 
			f.fasdsc as fase, 
			p.prddsc as artefato,
			ed.esddsc as situacao
		FROM fabrica.fasedisciplinaproduto fdp
			inner join fabrica.fasedisciplina 		fd  on fd.fsdid  = fdp.fsdid
			inner join fabrica.disciplina 			d   on fd.dspid  = d.dspid
			inner join fabrica.fase 				f   on f.fasid   = fd.fasid
			inner join fabrica.produto 				p   on p.prdid   = fdp.prdid
			inner join fabrica.servicofaseproduto 	s   on s.fdpid   = fdp.fdpid
			inner join fabrica.analisesolicitacao 	a   on a.ansid   = s.ansid
			inner join fabrica.solicitacaoservico 	sse on sse.scsid = a.scsid
			inner join workflow.documento 			doc on doc.docid = sse.docid
			inner join workflow.estadodocumento 	ed  on ed.esdid  = doc.esdid
			left join demandas.sistemadetalhe 		sid ON sid.sidid = sse.sidid
		WHERE
			1=1
			$filtro
		order by 1,2,3,4";
$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();
	
if( $requisicao == 'exportar' ){
	ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');
	$cabecalho = array("SS", "Sistema", "Disciplina", "Fase", "Artefato", "Situação");
	$db->sql_to_excel($arrDados, 'RelatorioGestao_'.date('Ydm'), $cabecalho, $formato);
	exit;
} else {
	if( $arrDados ){
	$html = '<table id="tblform" class="listagem" width="95%" bgcolor="#f5f5f5"	cellSpacing="1" cellPadding="3" align="center">
			<thead>
				<tr>
					<td align="Center" class="title" width="05%"
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
						onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>SS</strong></td>
					<td align="Center" class="title" width="30%"
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
						onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Sistema</strong></td>
					<td align="Center" class="title" width="15%"
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
						onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Disciplina</strong></td>
					<td align="Center" class="title" width="10%"
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
						onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Fase</strong></td>
					<td align="Center" class="title" width="30%"
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
						onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Artefato</strong></td>
					<td align="Center" class="title" width="10%"
						style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
						onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Situação</strong></td>
				</tr>
			</thead>';
		
		foreach ($arrDados as $key => $v) {
			$key % 2 ? $cor = "#dedfde" : $cor = "";
			
			$html .= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
						<td style="text-align: right; color: rgb(0, 102, 204);">'.$v['ss'].'</td>
						<td style="text-align: left;">'.$v['sistema'].'</td>
						<td style="text-align: left;">'.$v['disciplina'].'</td>
						<td style="text-align: left;">'.$v['fase'].'</td>
						<td style="text-align: left;">'.$v['artefato'].'</td>
						<td style="text-align: left;">'.$v['situacao'].'</td>';
		}
		$html.= '<table class="listagem" cellspacing="0" cellpadding="2" border="0" align="center" width="95%">
				<tbody>
					<tr bgcolor="#ffffff">
						<td><b>Total de Registros: '.sizeof($arrDados).'</b></td>
					</tr>
				</tbody>
		  		</table>';
	} else {
		$html = '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
				<tbody>
					<tr>
						<td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td>
					</tr>
				</tbody>
				</table>';
	}
	echo $html;
}
?>