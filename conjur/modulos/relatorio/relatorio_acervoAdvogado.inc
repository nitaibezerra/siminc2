<?php

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo = "Relatório de Acervo por Advogado.";
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
				$_REQUEST['htddata_fim']    = $_REQUEST['htddata_fim'] ? $_REQUEST['htddata_fim'] : '01/'.(date('m')+1).'/'.date('Y');
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
					ent.entnome,
					--adv.advid,
					sum(
					CASE WHEN h1.hstid = hta1.hstid THEN 1 ELSE 0 END
					) as qtd_processos_15dias,
					sum(
					CASE WHEN h1.hstid = hta2.hstid THEN 1 ELSE 0 END
					) as qtd_processos_16dias,
					sum(
					CASE WHEN h1.hstid = hta3.hstid THEN 1 ELSE 0 END
					) as qtd_processos_31dias,
					sum(
					CASE WHEN h1.hstid = hta4.hstid THEN 1 ELSE 0
					END
					) as qtd_processos_60dias
				FROM
					conjur.advogados adv
				INNER JOIN entidade.entidade ent ON ent.entid = adv.entid
				--Coluna 1
				LEFT  JOIN conjur.historicoadvogados had ON had.advid = adv.advid
				LEFT  JOIN workflow.historicodocumento hst ON hst.hstid = had.hstid 
				--Coluna 3 
				LEFT JOIN workflow.documento d1 ON d1.docid = hst.docid 
				LEFT JOIN workflow.historicodocumento h1 ON h1.hstid = d1.hstid
				LEFT JOIN workflow.historicodocumento h2 ON h2.hstid = had.hstid			
				-- até 15 dias
				LEFT  JOIN workflow.historicodocumento hta1 ON hta1.hstid = h2.hstid AND ( (now()::date - hta1.htddata::date) <= 15 ) 
				-- de 16 até 30 dias
				LEFT  JOIN workflow.historicodocumento hta2 ON hta2.hstid = h2.hstid AND ( (now()::date - hta2.htddata::date) BETWEEN 16 AND 30 )
				-- de 31 até 60 dias
				LEFT  JOIN workflow.historicodocumento hta3 ON hta3.hstid = h2.hstid AND ( (now()::date - hta3.htddata::date) BETWEEN 31 AND 60 )
				-- acima de 60 dias
				LEFT  JOIN workflow.historicodocumento hta4 ON hta4.hstid = h2.hstid AND ( (now()::date - hta4.htddata::date) > 60 )
				GROUP BY
					ent.entnome,
					adv.advid
				ORDER BY 
					ent.entnome";
		$cabecalho = Array("Advogado",
							"<center>Até 15 dias</center>",
							"<center>De 16 <br> até 30 dias</center>",
							"<center>De 31 <br> até 60 dias</center>",
							"<center>Acima de 60 dias</center>");
		$celWidth = Array("60%","10%","10%","10%","10%");
		$celAlign = Array("Left","center","center","center","center");
		$db->monta_lista($sql,$cabecalho,100,50,$soma,$alinha,"N","formulario",$celWidth,$celAlign,$tempocache=null,$param=Array());
		?>
	</table>
</form>