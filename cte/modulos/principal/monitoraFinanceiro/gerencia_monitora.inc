<?php
/************************* INCLUDES *************************************************/
include_once(APPRAIZ.'www/cte/monitoraFinanceiro/includes/constantes.php');
include_once(APPRAIZ.'cte/modulos/principal/monitoraFinanceiro/funcoes.inc');
include_once(APPRAIZ.'cte/classes/HistoricoMonitoramentoConvenio.class.inc');
include_once(APPRAIZ.'cte/classes/HistoricoMonitoramentoDadosBancarios.class.inc');

$prsid = $_REQUEST['prsid'];
$ano = $_REQUEST['ano'];

$obMonitoramento		 	= new HistoricoMonitoramentoConvenio();
$meses = $obMonitoramento->carregaUltimoMonitoramento($prsid,$ano );
$meses['codigo'];
$meses['descricao'];

if( $_REQUEST['salvar'] ){
	//$obMonitoramento->carregarPorId( $_REQUEST['mes'] );
	//dbg();
//	$obMonitoramento->excluir($_REQUEST['mes']);
	if($obMonitoramento->excluir($_REQUEST['mes'])){
		$sql = "delete from cte.auditoriafinanceiro where hmcid =".$_REQUEST['mes'];
		$obMonitoramento->executar($sql);
		$obMonitoramento->commit();
		$ultimo = $obMonitoramento->carregaUltimoMonitoramento($prsid,$ano );
		$sql = "update cte.historicomonitoramentoconvenio set hmcstatus = 'I'  where hmcid =".$ultimo['codigo'];
		$obMonitoramento->executar($sql);
		$obMonitoramento->commit();
		 echo '<script> 
					alert( "Operação realizada com sucesso. " );
					parent.closeLyteboxGerencia();
			  </script>';
	}
	
}
?>
<html>
<head>
	<link rel="stylesheet" type="text/css" href="../../cte/monitoraFinanceiro/includes/monitora_financeiro.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
	<script type="text/javascript" src="../../includes/funcoes.js"></script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">
<input type=hidden name="mes" id="mes" value="<?php echo $meses['codigo']; ?>" />
<input type=hidden name="salvar" value="false" />
<table id="form3" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="100%">
		<tr>
			<th colspan="2" >Gestão dos monitoramentos</th>
		</tr>
		<tr>
			<th id="texto" colspan="2" style="display:none;" ><?=$titulo; ?></th>
		</tr>
		<tr>
			<td class="SubTituloDireita">Mês:</td>
			<td><?=$meses['dataformatada']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" id="JustificativaTexto" style="display:none;" >Justificativa:</td>
			<td id="JustificativaCampo" style="display:none;" ><?=campo_textarea('hmsjustificativa', "S", 'S', '', 70, 3, 2000 ); ?></td>
		</tr>
		<tr>
			<td></td>
			<td><input type="button" name="apagar" value="Apagar"  onclick="deletarMonitoramento();" /></td>
		</tr>
	</table>
</form>
	<script type="text/javascript">
	function deletarMonitoramento(){
		var hmcid = document.getElementById("mes").value;
		if(hmcid != ''){
			document.formulario.salvar.value = true;
			document.formulario.submit();
		}else{
			alert("Selecione um mês.");
			return false;
		}
	};
</script>
</body>
</html>
