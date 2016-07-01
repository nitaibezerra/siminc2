<?php
$preid = $_SESSION['par']['preid'] ? $_SESSION['par']['preid'] : $_REQUEST['preid'];

echo carregaAbasProInfancia("par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&preid=".$preid."&tipoAba=CronogramaContrapartida", $preid, $descricaoItem );

monta_titulo( 'Cronograma de Contrapartida', $obraDescricao  ); 
echo cabecalho();

$sql = "select
		    cast(p.prevalorobra as numeric(20, 2)) as vlrobra,
		    vm.saldo as valor_empenhado,
		    coalesce(sr.sfocontrapartidainformada, 0) as valor_contrapartida,
		    cast((p.prevalorobra - coalesce(sr.sfocontrapartidainformada, 0)) as numeric(20, 2)) as valor_fnde
		from
			obras.preobra p
		    inner join workflow.documento d on d.docid = p.docid
		    inner join par.solicitacaoreformulacaoobras sr on sr.preid = p.preid and sr.sfostatus = 'A'
		    inner join par.v_saldo_empenho_por_obra vm on vm.preid = p.preid
		    /*inner join par.termoobraspaccomposicao tc on tc.preid = p.preid
		    inner join par.termocompromissopac ter on ter.terid = tc.terid*/
		where
			p.preid = $preid";
			
 $cabecalho = array('Valor Total da Obra', 'Valor Empenhado', 'Valor de Contrapartida', 'Valor do FNDE',);
 $db->monta_lista_simples($sql, $cabecalho, 500, 5, 'N','100%', '');
 
 $sql = "select
		    (sr.sfocontrapartidainformada * (20.0/100)) as execucao_20,
		    (sr.sfocontrapartidainformada * (25.0/100)) as execucao_25,
		    (sr.sfocontrapartidainformada * (35.0/100)) as execucao_35,
		    (sr.sfocontrapartidainformada * (20.0/100)) as execucao_201,
		    sr.sfocontrapartidainformada
		from
			par.solicitacaoreformulacaoobras sr 
		where
			sr.preid = $preid";
 $arContra = $db->pegaLinha($sql);
?>
<form name="formulario" id="formulario" method="post">   
  	<input type="hidden" name="requisicao" id="requisicao" value="">
    	
	<table border="0" class="tabela" align="center"  bgcolor="#f5f5f5" style="width: 100%" cellspacing="2" cellpadding="3">
	<tr>
		<th colspan="4" width="25%"><b>CRONOGRAMA DE CONTRAPARTIDA</b></th>
	</tr>
	<tr>
		<th>Fase da obra</th>
		<th>% do valor da  contrapartida a ser depositado</th>
		<th>Valor da Contrapartida</th>
	</tr>
	<tr>
		<td>Até 20% da execução</td>
		<td align="center">20%</td>
		<td><?php echo simec_number_format( $arContra['execucao_20'], 2, ',', '.' ); ?></td>
	</tr>
	<tr>
		<td>Até 40% da execução</td>
		<td align="center">25%</td>
		<td><?php echo simec_number_format( $arContra['execucao_25'], 2, ',', '.' ); ?></td>
	</tr>
	<tr>
		<td>Até 60% da execução</td>
		<td align="center">35%</td>
		<td><?php echo simec_number_format( $arContra['execucao_35'], 2, ',', '.' ); ?></td>
	</tr>
	<tr>
		<td>Até 80% da execução</td>
		<td align="center">20%</td>
		<td><?php echo simec_number_format( $arContra['execucao_201'], 2, ',', '.' ); ?></td>
	</tr>
	<tr>
		<td><b>Total</b></td>
		<td align="center"><b>100%</b></td>
		<td><b><?php echo simec_number_format( $arContra['sfocontrapartidainformada'], 2, ',', '.' ); ?></b></td>
	</tr>
	</table>
</form>