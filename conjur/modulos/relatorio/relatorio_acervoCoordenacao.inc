<?php

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo = "Relatório de Acervo por Coordenação.";
monta_titulo( $titulo_modulo, '' );

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script language="javascript" type="text/javascript">

function validaFormulario(){
	$('#loader-container').slideDown();
	var inicio = document.getElementById('htddata_inicio');
	var fim = document.getElementById('htddata_fim');
	//valida data
	if(inicio.value != '' && fim.value != ''){
		if(!validaData(inicio)){
			alert("Data Início Inválida.");
			inicio.focus();
			$('#loader-container').slideUp();
			return false;
		}		
		if(!validaData(fim)){
			alert("Data Fim Inválida.");
			fim.focus();
			$('#loader-container').slideUp();
			return false;
		}		
	}
	$('#filtro').submit();
}

$(document).ready(function()
{
	$('#loader-container').slideUp();
});

</script>
<div id="loader-container" style="position: absolute; background-color: white; opacity: .6; width:110%; height:2000%; margin-top: -200px; margin-left: -20px; Z-index:22;" >
	<center>
		<div id="loader" style="margin-top:300px;">
			<img src="../imagens/wait.gif" border="0" align="middle">
			<span>Aguarde! Carregando Dados...</span>
		</div>
	</center>
</div>
<form action="" method="post" name="filtro" id="filtro">
	<input type="hidden" name="form" value="1"/>
	<input type="hidden" id="req" name="req" value=""/>	
	<input type="hidden" name="pesquisa" value="1"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<!--<tr>
			<td class="SubTituloDireita">Período do Boletim:</td>
			<td>
				<?php 
				$_REQUEST['htddata_inicio'] = $_REQUEST['htddata_inicio'] ? $_REQUEST['htddata_inicio'] : '01/'.date('m').'/'.date('Y');
				$_REQUEST['htddata_fim']    = $_REQUEST['htddata_fim'] ? $_REQUEST['htddata_fim'] : date('d').'/'.date('m').'/'.date('Y');
				$htddata_inicio = explode('/', $_REQUEST['htddata_inicio']);
				$htddata_inicio = $htddata_inicio[1].'/'.$htddata_inicio[0].'/'.$htddata_inicio[2];
				$htddata_fim = explode('/', $_REQUEST['htddata_fim']);
				$htddata_fim = $htddata_fim[1].'/'.$htddata_fim[0].'/'.$htddata_fim[2];
				?>
				<?= campo_data2( 'htddata_inicio', 'N', 'S', '', 'S' ); ?>
				&nbsp;&nbsp;
				a
				&nbsp;&nbsp;&nbsp;
				<?= campo_data2( 'htddata_fim', 'N', 'S', '', 'S' ); ?>
			</td>
		</tr>
		-->
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Filtrar" style="cursor: pointer;" class="filtrar" onclick="validaFormulario()"/>
			</td>
		</tr>
	</table>
</form>
<form action="" method="post" name="formulario">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td style="text-align: center; background-color: #e0e0e0;" colspan="6">
				<b style="font-size: 10pt;"><br/></b>
			</td>
		</tr>
		<?php 
		/*
		$whereLEFT1 = "";
		
		$dataFim = 'now()';
		
		// período
		if( $_REQUEST['htddata_inicio'] && $_REQUEST['htddata_fim'] ){
			$htddata_inicio = formata_data_sql($_REQUEST['htddata_inicio']);
			$htddata_fim = formata_data_sql($_REQUEST['htddata_fim']);
			$dataFim = "'".$htddata_fim."'";
			$whereLEFT1 = " AND hst.htddata::date BETWEEN '$htddata_inicio'::date AND '$htddata_fim'::date";
			$whereLEFT2 = " AND htddata::date <= '$htddata_fim'::date";
		}
		*/
		
		$sql = "SELECT
					coo.coodsc,
					coalesce('<input type=\"hidden\" value=\"'|| ent.entnome ||'\"/>'||ent.entnome,'<input type=\"hidden\" value=\"zzzzzzz\"/>Sem advogado atribuído.') as entnome,
					sum(
					CASE WHEN htp.hstid = htc1.hstid THEN 1 ELSE 0 END
					) as qtd_processos_15dias,
					sum(
					CASE WHEN htp.hstid = htc2.hstid THEN 1 ELSE 0 END
					) as qtd_processos_16dias,
					sum(
					CASE WHEN htp.hstid = htc3.hstid THEN 1 ELSE 0 END
					) as qtd_processos_31dias,
					sum(
					CASE WHEN htp.hstid = htc4.hstid THEN 1 ELSE 0
					END
					) as qtd_processos_60dias
				FROM
					conjur.coordenacao coo
				INNER JOIN conjur.historicocoordenacoes hco ON hco.coonid = coo.coonid
				INNER JOIN workflow.historicodocumento  hst ON hst.hstid  = hco.hstid 
				LEFT  JOIN (SELECT max(hstid) as hstid, docid 
						FROM workflow.historicodocumento 
						WHERE aedid in (".WF_AEDID_ENCAMINHAR_ADVIGADO.",
										".WF_AEDID_RETORNAR_ADVIGADO.",
										".WF_AEDID_ENCAMINHAR_CHEFE_DIVISAO_CGAE.",
										".WF_AEDID_ENCAMINHAR_COORD_CGAE.",
										".WF_AEDID_RETORNAR_CHEFE_DIVISAO_CGAE.",
										".WF_AEDID_RETORNAR_COORD_CGAE.") 
						GROUP BY
							docid) hpa ON hpa.docid = hst.docid
				LEFT  JOIN conjur.historicoadvogados    had ON had.hstid = hpa.hstid
				LEFT  JOIN conjur.advogadosxcoordenacao aco ON aco.advid = had.advid AND aco.coonid = coo.coonid 
				LEFT  JOIN conjur.advogados 		adv ON adv.advid = aco.advid
				LEFT  JOIN entidade.entidade 		ent ON ent.entid = adv.entid
				LEFT  JOIN (SELECT max(hstid)as hstid, docid FROM workflow.historicodocumento GROUP BY docid) htp ON htp.docid = hst.docid
				LEFT  JOIN (SELECT max(hstid)as hstid, docid FROM workflow.historicodocumento GROUP BY docid) htc ON htc.hstid = had.hstid
				-- até 15 dias
				LEFT  JOIN workflow.historicodocumento htc1 ON htc1.hstid = htc.hstid AND ( (now()::date - htc1.htddata::date) <= 15 ) 
				-- de 16 até 30 dias
				LEFT  JOIN workflow.historicodocumento htc2 ON htc2.hstid = htc.hstid AND ( (now()::date - htc2.htddata::date) BETWEEN 16 AND 30 )
				-- de 31 até 60 dias
				LEFT  JOIN workflow.historicodocumento htc3 ON htc3.hstid = htc.hstid AND ( (now()::date - htc3.htddata::date) BETWEEN 31 AND 60 )
				-- acima de 60 dias
				LEFT  JOIN workflow.historicodocumento htc4 ON htc4.hstid = htc.hstid AND ( (now()::date - htc4.htddata::date) > 60 )
				GROUP BY
					coo.coodsc,
					ent.entnome,
					adv.advid
				ORDER BY 
					1,2";
		$coords = $db->carregar($sql);
		$cor = 'white';
		$tot15 = 0;
		$tot16 = 0;
		$tot31 = 0;
		$tot60 = 0;
		foreach( $coords as $k => $coord ){
			$cor = ($k%2)==0 ? 'white' : '#F7F7F7';
			$tot15 = $tot15 + $coord['qtd_processos_15dias'];
			$tot16 = $tot16 + $coord['qtd_processos_16dias'];
			$tot31 = $tot31 + $coord['qtd_processos_31dias'];
			$tot60 = $tot60 + $coord['qtd_processos_60dias'];
			if( $cordAnt == '' || $cordAnt != $coord['coodsc'] ){ 
				$cor = 'white';
				if($cordAnt != '') echo "</table></td></tr>";
				$cordAnt = $coord['coodsc'];
		?>
		<tr>
			<td style="text-align: center; background-color: white;" colspan="6">
				<b style="font-size: 10pt;"><?=$coord['coodsc'] ?></b>
			</td>
		</tr>
		<tr>
			<td style="text-align: center; colspan="6">
				<table cellspacing="0" cellpadding="2" width="95%" border="0" align="center" class="listagem">
					<tr>
						<td valign="top" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; 
							border-left: 1px solid #ffffff;" class="title"><strong>Advogado</strong></td>
						<td valign="top" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; 
							border-left: 1px solid #ffffff;" class="title"><strong><center>Até 15 dias</center></strong></td>
						<td valign="top" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; 
							border-left: 1px solid #ffffff;" class="title"><strong><center>De 16 <br> até 30 dias</center></strong></td>
						<td valign="top" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; 
							border-left: 1px solid #ffffff;" class="title"><strong><center>De 31 <br> até 60 dias</center></strong></td>
						<td valign="top" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; 
							border-left: 1px solid #ffffff;" class="title"><strong><center>Acima de 60 dias</center></strong></td>
					</tr> 
		<?php 
			}
		?>
					<tr bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor ?>';" onmouseover="this.bgColor='#ffffcc';">
						<td width="60%" align="Left" title="Advogado"><?=$coord['entnome'] ?></td>
						<td width="10%" align="center" title="Até 15 dias" style="color:#0066cc;"><?=$coord['qtd_processos_15dias'] ?><br></td>
						<td width="10%" align="center" title="De 16  até 30 dias" style="color:#0066cc;"><?=$coord['qtd_processos_16dias'] ?><br></td>
						<td width="10%" align="center" title="De 31  até 60 dias" style="color:#0066cc;"><?=$coord['qtd_processos_31dias'] ?><br></td>
						<td width="10%" align="center" title="Acima de 60 dias" style="color:#0066cc;"><?=$coord['qtd_processos_60dias'] ?><br></td>
					</tr>
		<?php 
			if( $coord['coodsc'] != $coords[$k+1]['coodsc'] ){
				$cor = $cor=='#F7F7F7' ? 'white' : '#F7F7F7';
		?>
					<tr bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor ?>';" onmouseover="this.bgColor='#ffffcc';">
						<td width="60%" align="right" title="Advogado" ><b>Totais:</b></td>
						<td width="10%" align="center" title="Até 15 dias" style="color:#0066cc;"><?=$tot15 ?><br></td>
						<td width="10%" align="center" title="De 16  até 30 dias" style="color:#0066cc;"><?=$tot16 ?><br></td>
						<td width="10%" align="center" title="De 31  até 60 dias" style="color:#0066cc;"><?=$tot31 ?><br></td>
						<td width="10%" align="center" title="Acima de 60 dias" style="color:#0066cc;"><?=$tot60 ?><br></td>
					</tr>
		<?php 
				$tot15 = 0;
				$tot16 = 0;
				$tot31 = 0;
				$tot60 = 0;
			}
		}
		?>	
				</table>
			</td>
		</tr>
	</table>
</form>