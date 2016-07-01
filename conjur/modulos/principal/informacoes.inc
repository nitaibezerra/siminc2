<table align="center" border="0" cellpadding="5" cellspacing="1"  width="100%" class="tabela" cellpadding="0" cellspacing="0">
	<tr>
			<td  bgcolor="#CCCCCC" colspan="4"><b>Cadastro da Processo</b></td>
	</tr>
	
	<tr>
		<td bgcolor="#CCCCCC" align="right" width="25%">Nº do Processo:</td>
		<td bgcolor="#e9e9e9" align="left" width="25%"><? echo $processoConjur->pcjnumerosidoc;?></td>
		<td bgcolor="#CCCCCC" align="right" width="25%">Situação do Processo:</td>
		<td bgcolor="#e9e9e9" align="left" width="25%"><? echo $processoConjur->esddsc;?></td>
	<td>
	
	<tr>
		<td bgcolor="#CCCCCC" align="right" width="25%">Interessado:</td>
		<td bgcolor="#e9e9e9" align="left" width="25%">
		<?
		$sqlInterressados = " SELECT * FROM conjur.processointeressados as i, entidade.entidade as e where i.entid = e.entid AND i.pcjid = ".$_SESSION[pcjid]."";
		$interessado = $db->executar($sqlInterressados);
		while($info = pg_fetch_assoc($interessado)) {
			echo $info[entnome]."</br>";
		}
		?>
		</td>
		<td bgcolor="#CCCCCC" align="right" width="25%">Situação na CONJUR:</td>
		<td bgcolor="#e9e9e9" align="left" width="25%"><? echo $processoConjur->stcnome;?></td>
	<td>

	<tr>
		<td bgcolor="#CCCCCC" align="right" width="25%">Localização:</td>
		<td bgcolor="#e9e9e9" align="left" width="25%" colspan="3"><? echo $processoConjur->pcjnumerosidoc;?></td>
	<td>

	<tr>
		<td bgcolor="#CCCCCC" align="right" width="25%">Procedência:</td>
		<td bgcolor="#e9e9e9" align="left" width="25%" colspan="3"><? echo $processoConjur->ungdsc;?></td>
	<td>	
			
</table>