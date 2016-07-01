<?php
header("Content-Type: text/html; charset=ISO-8859-1",true);

global $db;

require_once APPRAIZ . 'includes/cabecalho.inc';

echo '<br/>';
monta_titulo( 'Adesões pendentes para envio manual', '' );
echo '<br/>';
$db->cria_aba($abacod_tela,$url,$parametros);
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
$(function(){
	$('#btnPesquisar').click(function(){	
		if($('[name=estuf]').val() == '' && $('[name=mundescricao]').val() == ''){
			alert('Preencha pelo menos um filtro!');
			return false;
		}	
		$('#formulario').submit();
	});
	$('#btnLimpar').click(function(){
		document.location.href = 'par.php?modulo=principal/acompanhamento/ferramentas/listaAdesoesPendentes&acao=A';
	});
});
</script>
<form method="post" name="formulario" id="formulario" action="">
	<input type="hidden" name="anoatual" value="<?=$_SESSION['exercicio']; ?>">
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>
			<td class="subtituloDireita">UF:</td>
			<td>
				<?php
				$estuf = $_POST['estuf']; 
				$sql = "SELECT		estuf as codigo,
									estdescricao as descricao
						FROM		territorios.estado
						ORDER BY 	estuf";
				$db->monta_combo('estuf', $sql, 'S', 'Selecione...', '', '', ''); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Município:</td>
			<td>
				<?php
					$mundescricao = $_POST['mundescricao']; 
					echo campo_texto('mundescricao', 'N', 'S', '', 70, '', '', ''); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input type="button" value="Pesquisar" id="btnPesquisar" />
				<input type="button" value="Limpar" id="btnLimpar" />
			</td>
		</tr>
	</table>
</form>
<?php

$w = "";
if( $_POST['estuf'] ){
	$w .= " AND uf = '".$_POST['estuf']."' ";
}

if( $_POST['mundescricao'] ){
	$w .= " AND local ilike '%".$_POST['mundescricao']."%' ";
}

$cabecalho = array("Processo", "UF","Local","Esfera", "Tipo");

$sql = "SELECT * FROM (

		SELECT DISTINCT
			prp.prpnumeroprocesso,
			CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE iu.mun_estuf END as uf,
			CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE m.mundescricao END as local,
			CASE WHEN iu.itrid = 1 THEN 'Estadual' ELSE 'Municipal' END as esfera,
			'PAR Genérico' as tipo
		FROM 
			par.processopar prp
		INNER JOIN par.instrumentounidade iu ON iu.inuid = prp.inuid
		LEFT JOIN  territorios.municipio m ON m.muncod = iu.muncod
		WHERE
			prp.prpid in ( SELECT DISTINCT prpid FROM par.empenhopregaoitemperdido )
			AND prp.prpstatus = 'A'
		UNION ALL
		
		SELECT 
			po.pronumeroprocesso, 
			CASE WHEN m.estuf IS NOT NULL THEN m.estuf ELSE po.estuf END as uf,
			CASE WHEN m.mundescricao IS NOT NULL THEN m.mundescricao ELSE po.estuf END as local,
			CASE WHEN m.mundescricao IS NOT NULL THEN 'Municipal' ELSE 'Estadual' END as esfera,
			'Obras MI' as tipo
		FROM 
			par.processoobra  po
		LEFT JOIN territorios.municipio m ON m.muncod = po.muncod
		WHERE 
			po.pronumeroprocesso IN ( SELECT DISTINCT processo FROM par.enviomiitemperdido  )
			and po.prostatus = 'A'
		) as foo
		WHERE 1 = 1 
			".$w."
		ORDER BY
			tipo, esfera, uf, local";

$db->monta_lista($sql,$cabecalho,100,100,'N','center','N','formPendente');
?>