<?php

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// carregando o menu
montaAbaInicio("conjur.php?modulo=principal/statusProcesso&acao=A");

$titulo = "Situação dos Processos";
monta_titulo( $titulo, '&nbsp;' );

?>
<script>
	function visualizarProcessos(esdid)
	{
		if(esdid){
			document.getElementById('esdid').value = esdid;
			if(document.getElementById('esdid').value){
				document.getElementById('formulario2').submit();
			}
		}
	}
</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td>
		<?php $sql = "	SELECT
							'<img src=\"../imagens/consultar.gif\" title=\"Visualizar Processos\" onclick=\"visualizarProcessos(\'' || esd.esdid || '\')\" style=\"cursor:pointer;vertical-align:middle\" > ' || esd.esddsc as esddsc,
							count(doc.docid)
						FROM
							conjur.processoconjur prc
						LEFT JOIN 
							conjur.estruturaprocesso esp ON prc.prcid = esp.prcid 
						LEFT JOIN 
							workflow.documento doc ON doc.docid = esp.docid
						LEFT JOIN 
							workflow.estadodocumento esd ON esd.esdid = doc.esdid
						WHERE
							esd.esddsc is not null
						AND 
							prcstatus = 'A'
						GROUP BY
							esd.esddsc,
							esd.esdid
						ORDER BY
							esd.esddsc"; ?>
		<?php $cabecalho = array("Situação","Qtde Processos") ?>
		<?php $db->monta_lista($sql, $cabecalho, 50, 20, '', '100%', ''); ?>
		</td>
	</tr>
</table>
<form method="post" id="formulario2" name="formulario2" action="conjur.php?modulo=inicio&acao=S">
<input type="hidden" id="esdid" name="esdid" value="" />
<input type="hidden" id="esdid" name="evento" value="pesquisa" />
<input type="hidden" id="esdid" name="origem" value="avancado" />
</form>