<?php
monta_titulo('Períodos Aguardando Pagamento','');

$sql = "
		SELECT DISTINCT
			foo.perperiodo,
			SUM(foo.autorizadas) as autorizadas,
			foo.data as data
		FROM
		(
			SELECT 
				per.perperiodo,
				CASE WHEN dia.perid IN(SELECT
										perid
									  FROM
										projovemurbano.periodocurso
									  WHERE
										ppuid = {$_SESSION['projovemurbano']['ppuid']}) 
					AND doc.esdid = 528 
					AND doc2.esdid IN(523,524) 
					AND pge.pgeaptoreceber = 't' --AND (cae.caeqtddireitobolsa - cae.caeqtdbolsaprojovem) > 0 
					THEN 1
					ELSE 0
				END as autorizadas,
				to_char(hst.htddata,'yyyy-MM-DD') as data
			FROM projovemurbano.pagamentoestudante pge
			INNER JOIN workflow.documento doc	 	 ON doc.docid = pge.docid
			INNER JOIN projovemurbano.cadastroestudante cae  ON pge.caeid = cae.caeid
			INNER JOIN workflow.historicodocumento hst	 ON hst.hstid = doc.hstid 	
			INNER JOIN projovemurbano.diario dia             ON pge.diaid = dia.diaid
			INNER JOIN workflow.documento doc2               ON dia.docid = doc2.docid
			INNER JOIN projovemurbano.periodocurso per 	 ON per.perid = dia.perid
			WHERE
				dia.perid in (SELECT
							perid
						  FROM
							projovemurbano.periodocurso
						  WHERE
							ppuid = {$_SESSION['projovemurbano']['ppuid']})
			AND 	doc.esdid = 528
			AND doc2.esdid IN(523,524)
			AND pge.pgeaptoreceber = 't'
			AND (cae.caeqtddireitobolsa - cae.caeqtdbolsaprojovem) > 0 
			GROUP BY
				per.perperiodo,
				hst.htddata,
				dia.perid,
				doc.esdid,
				doc2.esdid,
				pge.pgeaptoreceber,
				cae.caeqtddireitobolsa,
				cae.caeqtdbolsaprojovem
			Order by 
				per.perperiodo
		) as foo
		WHERE
			foo.autorizadas::int <> 0
		GROUP BY
			foo.perperiodo,
			foo.autorizadas,
			foo.data
		ORDER BY
			foo.perperiodo,
			data
		";

$cabecalho = array("Período","Bolsas a pagar","Envio em");
$alinhamento	= array( 'center', 'center','center');
$db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', '', '','',$alinhamento);
?>
 <br>
<?
monta_titulo('','Resumo dos pagamentos');
$sql = "
		SELECT DISTINCT 
			'<img src=../imagens/consultar.gif id=\"'|| per.perid ||'\" value=\"'||per.perid||'\" onclick=\"exibeHistorico('||per.perid||')\"  style=\"cursor: pointer;\">' as acao,
			per.perperiodo,
			('<a perid=\"'|| per.perid ||'\" tipo=\"pendente\" class=\"relatorio\"  style=\"cursor: pointer;\">' || 
			(			
			SELECT DISTINCT
				SUM(dia.diabolsaspendentes)
			FROM  projovemurbano.diario dia             			
			INNER JOIN projovemurbano.periodocurso per1      		ON per1.perid = dia.perid
			WHERE 
				dia.perid = per.perid
			) || '</a>') as pendente,
			('<a perid=\"'|| per.perid ||'\" tipo=\"autorizado\" class=\"relatorio\"  style=\"cursor: pointer;\">' || 
			(
			SELECT DISTINCT
				SUM(dia.diabolsasautorizadas)
			FROM projovemurbano.diario dia             		
			INNER JOIN projovemurbano.periodocurso per1      		ON per1.perid = dia.perid
			WHERE 
				dia.perid = per.perid
			) || '</a>') as autorizadas,
			('<a perid=\"'|| per.perid ||'\" tipo=\"enviado\" class=\"relatorio\"  style=\"cursor: pointer;\">' || 
			(
			SELECT DISTINCT
				SUM(dia.diabolsasenviadas)
			FROM projovemurbano.diario dia             		
			INNER JOIN projovemurbano.periodocurso per1      		ON per1.perid = dia.perid
			WHERE 
				dia.perid = per.perid
			) || '</a>') as enviadas,
			('<a perid=\"'|| per.perid ||'\" tipo=\"rejeitado\" class=\"relatorio\"  style=\"cursor: pointer;\">' || 
			(
			SELECT DISTINCT
				SUM(dia.diabolsasrecusadas)
			FROM 
				projovemurbano.diario dia             		
			INNER JOIN projovemurbano.periodocurso per1      		ON per1.perid = dia.perid
			WHERE 
				dia.perid = per.perid
			) || '</a>') as rejeitadas	
		FROM 	
			projovemurbano.diario dia            
		INNER JOIN projovemurbano.periodocurso per       ON per.perid = dia.perid
		WHERE 
			per.perid in (SELECT
							perid
				  		  FROM
							projovemurbano.periodocurso
				  		  WHERE
							ppuid = {$_SESSION['projovemurbano']['ppuid']})
		GROUP BY
			per.perperiodo,
    		per.perid
		ORDER BY
		per.perperiodo
							
";

$cabecalho = array("Histórico","Período","Bolsas em Estado Pendente","Bolsas Autorizadas para pagamento","Bolsas Enviadas para Pagamento","Bolsas Rejeitadas");
$alinhamento	= array( 'center', 'center','center', 'center','center', 'center');                   
$db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', '', '','',$alinhamento);

?>
<?php

monta_titulo('','Dados da Transferência');
$_SESSION['projovemurbano']['modalidade'] = '';

$bloq = 'S'

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="./js/projovemurbano.js"></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script language="javascript" type="text/javascript">
jQuery(document).ready(function() {
    
    jQuery('.pesquisar').click(function(){
        jQuery('#perid  option').attr('selected',true);
        jQuery('#pjuesfera  option').attr('selected',true);
        jQuery('#estuf  option').attr('selected',true);
        jQuery('#endmuncod  option').attr('selected',true);
        jQuery('#requisicao').val('');
        jQuery('#form').submit();
    });
});
function carregarMunicipios2(estuf) {
	var pjuesfera = jQuery('#pjuesfera').val();
	estuf = jQuery('#estuf').val();
	if( pjuesfera == 'M' && estuf != '' ){
		jQuery('#tr_muncod').show();
		jQuery('#td_muncod').html('Carregando...');
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location,
	   		data: "req=carregarMunicipios2&estuf="+estuf,
	   		async: false,
	   		success: function(msg){jQuery('#td_muncod').html(msg);}
	 	});
	}else if( pjuesfera == 'E'){
		jQuery('#tr_muncod').hide();
		jQuery('#td_muncod').html('');
		if( estuf != '' ){
			carregarPolo(estuf);
		}
	}else if( estuf != '' ){
		alert('Escolha uma esfera.');
	}
}   
</script>
<style>
.div_muda{
align:top;
}
</style>
<div id="dialog-detalhe"></div>
<form id="form_busca" name="form_busca" method="POST">
	<input type="hidden" name="req" value="">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2" width="30%">
				<b>Filtros de pesquisa</b>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Período</td>
			<td>
				<? 
				$sql = "SELECT
								perid as codigo,
								perperiodo||'° Período' as descricao
							FROM
								projovemurbano.periodocurso
							WHERE
								ppuid = {$_SESSION['projovemurbano']['ppuid']}";
				$db->monta_combo('perid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'perid');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Esfera</td>
			<td>
				<? 
				$sql = "SELECT DISTINCT 
							CASE 
								WHEN estuf IS NULL  THEN 'M' 
								WHEN muncod IS NULL THEN 'E' 
							END as codigo, 
							CASE 
								WHEN estuf IS NULL  THEN 'Municipal' 
								WHEN muncod IS NULL THEN 'Estadual' 
							END as descricao
						FROM 
							projovemurbano.projovemurbano";
				$db->monta_combo('pjuesfera', $sql, 'S', 'Selecione', 'carregarMunicipios2', '', '', '', 'S', 'pjuesfera');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">UF</td>
			<td>
				<? 
				$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
				$db->monta_combo('estuf', $sql, $bloq, 'Selecione', 'carregarMunicipios2', '', '', '', 'S', 'estuf');
				?>
			</td>
		</tr>
		<tr id="tr_muncod">
			<td class="SubTituloDireita">Munícipio</td>
			<td id="td_muncod">
				<? $db->monta_combo('endmuncod', array(), 'S', 'Selecione', '', '', '', '', 'S', 'endmuncod'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2">
				<input type="button" name="pesquisar" value="Pesquisar" onclick="document.getElementById('form_busca').submit();">
			</td>
		</tr>
	</table>
</form>
<?
if($_POST['perid']){

monta_titulo('','Pesquisa de pagamento');

	pesquisapagamento($_POST);
	
}
?>