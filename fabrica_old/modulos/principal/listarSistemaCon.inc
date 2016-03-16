<?php
include APPRAIZ."fabrica/classes/ContratoSistema.class.inc";


// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
$titulo = "Sistemas / Contratos";
monta_titulo( $titulo, '&nbsp;' );

$cs = new ContratoSistema();

extract($_POST);

?>
<script>
	function alterarSistema(ctsid)
	{
		window.location.href = 'fabrica.php?modulo=principal/cadSistemaCon&acao=A&ctsid=' + ctsid;
	}
	
	
	function pesquisarContratoSistema()
	{
		document.getElementById('form_listar_sistema').submit();
	}
	
</script>
<style>
	.TituloTabela{background-color: #C5C5C5;font-weight:bold}
	.center{text-align:center}
	.link{cursor: pointer;}
	.middle{vertical-align: middle;}
</style>
<form name="form_listar_sistema" id="form_listar_sistema" method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita" width="25%" >Sistema:</td>
			<td>
				<input type="hidden" name="requisicao" value="pesquisarContratoSistema" />
				<?$cs->comboSistema($sidid)?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Contrato:</td>
			<td><?$cs->comboContrato($ctrid)?></td>
		</tr>
		<tr>
			<td colspan="2" class="TituloTabela center">
				<input type="button" onclick="pesquisarContratoSistema()" value="Pesquisar" >
			</td>
		</tr>
	</table>
</form>
<?php $sql = "select
					'<center><img class=\"middle link\" title=\"Visualizar Sistema\" alt=\"Visualizar Sistema\" onclick=\"alterarSistema(\'' || ctsid || '\')\" src=\"../imagens/alterar.gif\" /> </center>' as acao,
					(select siddescricao from demandas.sistemadetalhe sid where cts.sidid = sid.sidid) as sistema,
					(select ctrnumero || ' - ' || to_char(ctrdtinicio,'DD/MM/YYY') from fabrica.contrato ctr where cts.ctrid = ctr.ctrid) as contrato
				from
					fabrica.contratosistema cts
				where
					ctsstatus = 'A' 
					".($sidid ? ' AND sidid = '.$sidid : '')."
					".($ctrid ? ' AND ctrid = '.$ctrid : '')."
				order by
					ctsid";
		
		$cabecalho = array("Ações","Sistema","Contrato");
		
		$db->monta_lista($sql,$cabecalho,100,50,"N","center","N"); 
?>