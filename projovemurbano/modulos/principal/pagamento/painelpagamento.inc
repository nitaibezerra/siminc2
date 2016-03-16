<?php

monta_titulo('Resumo dos Pagamentos','');
$_SESSION['projovemurbano']['modalidade'] = '';

$bloq = 'S'

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

$(document).ready(function(){
	$('.relatorio').click(function(){
		var perid = $(this).attr('perid');
		var tipo = $(this).attr('tipo');
		formulario.target = 'perid';
        var janela = window.open('projovemurbano.php?modulo=principal/pagamento/relatorio_bolsas&acao=A&perid='+perid+'&tipo='+tipo, 'perid', 
                				 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
        janela.focus();
	});
});

function exibeHistorico(perid) {
    document.getElementById('req').value = perid;
    var formulario = document.formulario;
   	selectAllOptions(document.getElementById('perid'));
        formulario.target = 'perid';
        var janela = window.open('projovemurbano.php?modulo=principal/pagamento/historicopagamento&acao=A&perid='+perid, 'perid', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
        janela.focus();
}

</script>
<style>
.div_muda{
align:top;
}
</style>
<div id="dialog-detalhe"></div>
<form action="" method="get" name="formulario" id="perid">
    <input type="hidden" id="req" name="req" value=""/>	
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="Center" border="0">
             
    </table>
</form>
<?

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
		AND perdtinicio <= now()
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