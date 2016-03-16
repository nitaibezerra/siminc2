<?php if($_REQUEST['apcid']) $_SESSION['projovemcampo']['apcid']=$_REQUEST['apcid']; ?>
<html>
<body style="font-size:7pt;" leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">
<script language="JavaScript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
<br>

<br>
<?
if($_REQUEST['diaid']) {
	
	$sql = "select 
				datahora,
				stddesc,
				usunome 
			from 
				projovemcampo.historico_diario as hist
			inner join projovemcampo.status_diario as status on status.stdid = hist.stdid 
			inner join seguranca.usuario us on us.usucpf = hist.usucpfquemfez
			where hist.diaid = ".$_REQUEST['diaid']." order by hist.hidid";
	$dadosHistorico = $db->carregar( $sql );
}
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td colspan="3"><b>Histórico do Diário:</b></td>
	</tr>
	<tr>
		<th><b>Data</b></th>
		<th>
			<b>Ação</b>
		</th>
		<th>
			<b>Quem fez</b>
		</th>
	</tr>
	<?php if( $dadosHistorico ){ 
			foreach ( $dadosHistorico as &$dado ){
				$dataHora = explode( ' ', $dado['datahora'] );
				$hora = substr( $dataHora[1], 0, 8 );
				$data = explode( '-', $dataHora[0] );
				$data = $data[2] . "/" . $data[1] . "/" . $data[0];
				$dado['datahora'] = $data . " " . $hora;
			} 
			foreach( $dadosHistorico as $dados ){ ?>
			<tr>
				<td>
					<?php echo $dados['datahora']; ?>
				</td>
				<td>
					<?php echo $dados['stddesc']; ?>
				</td>
				<td>
					<?php echo $dados['usunome']; ?>
				</td>
			</tr>
		<?php } ?>
	<?php }else{ ?>
		<tr>
			<td colspan="3">
				Não existe histórico de tramitações para o diário selecionado
			</td>
		</tr>
	<?php } ?>
	<tr>
		<td class="SubTituloCentro" colspan="3">
			<input type="button" name="Fechar" value="Fechar" onclick="window.close();"> 
		</td>
	</tr>
</table>
