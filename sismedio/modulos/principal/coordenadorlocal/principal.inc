<?

if($_REQUEST['modulo']=='principal/coordenadorlocal/coordenadorlocalexecucao') {

	$sql = "SELECT atiid,
				   atidesc, 
				   to_char(atidatainicioprev,'dd/mm/YYYY') as atidatainicioprev,
				   to_char(atidatafimprev,'dd/mm/YYYY') as atidatafimprev 
			FROM sispacto.atividades WHERE atistatus='A' AND attitipo='A'";
	
	$iusd_execucao = $_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['iusd'];
	
	
} else {
	
	$wh[] = "fiostatus='A'";
	if($_SESSION['sispacto']['esfera']=='municipal') $wh[] = "fioesfera='M' AND muncod='".$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['muncod']."'";
	elseif($_SESSION['sispacto']['esfera']=='estadual') $wh[] = "fioesfera='E' AND estuf='".$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['estuf']."'";
			
	$sql = "SELECT count(*) as tot FROM sispacto.formacaoinicialouvintes 
			WHERE ".(($wh)?implode(" AND ",$wh):"");
	
	$total_ouvintes = $db->pegaUm($sql);
	
	if($total_ouvintes > 0) $alert_ouvintes = "<p style=color:red;>Existem Orientadores de Estudo substitutos aguardando autorização de troca. Para autorizar basta acessar a aba 'Definir Orientadores de Estudo', selecionar o substituto e clicar no botão autorizar.</p>";
	
	$sql = "SELECT atiid,
				   atidesc, 
				   to_char(atidatainicioprev,'dd/mm/YYYY') as atidatainicioprev,
				   to_char(atidatafimprev,'dd/mm/YYYY') as atidatafimprev 
			FROM sispacto.atividades WHERE atistatus='A' AND attitipo='P'";
	

}

$atividades = $db->carregar($sql);
?>
<script>
function inserirAnexosPrincipalCoordenadorLocal(apaid) {
	window.open('sispacto.php?modulo=principal/coordenadorlocal/inseriranexos&acao=A&apaid='+apaid,'Anexos','scrollbars=no,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}
</script>
<input type="hidden" name="requisicao" value="inserirAnexoPrincipalCoordenadorLocal">
<input type="hidden" name="goto" id="goto" value="">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Principal</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?>
	<?
	if($alert_ouvintes) echo $alert_ouvintes;
	?>
	</td>
</tr>
<tr>
	<td colspan="2">
	
	<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro">Atividade</td>
		<td class="SubTituloCentro">Situação</td>
		<td class="SubTituloCentro" width="7%">Início<br>(Previsto)</td>
		<td class="SubTituloCentro" width="7%">Término<br>(Previsto)</td>
		<td class="SubTituloCentro" width="7%">Início</td>
		<td class="SubTituloCentro" width="7%">Término</td>
		<td class="SubTituloCentro">&nbsp;</td>
	</tr>
	<? 
	if($atividades[0]) :
		foreach($atividades as $atividade) :
		
			$sql = "SELECT suaid, suadesc, suafuncaosituacao, to_char(suadatainicioprev,'dd/mm/YYYY') as suadatainicioprev, to_char(suadatafimprev,'dd/mm/YYYY') as suadatafimprev, suadocumento 
					FROM sispacto.subatividades  
					WHERE suastatus='A' AND atiid='".$atividade['atiid']."'"; 
			$subatividades = $db->carregar($sql);
			
			atualizarInfoSubAtividades($subatividades);

			$execucao_atividade = carregarExecucaoAtividade(array("iusd"=>$iusd_execucao,"picid"=>$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid'],"atiid"=>$atividade['atiid']));
		
		?>
		<tr>
			<td><img src="../imagens/menos.gif"> <b><?=$atividade['atidesc'] ?></b></td>
			<td><?=progressBar($execucao_atividade['apassituacao']); ?></td>
			<td><?=(($atividade['atidatainicioprev'])?$atividade['atidatainicioprev']:"-") ?></td>
			<td><?=(($atividade['atidatafimprev'])?$atividade['atidatafimprev']:"-") ?></td>
			<td><?=(($execucao_atividade['apadatainicio'])?$execucao_atividade['apadatainicio']:"-") ?></td>
			<td><?=(($execucao_atividade['apadatafim'] && $execucao_atividade['apassituacao']==100)?$execucao_atividade['apadatafim']:"-") ?></td>
			<td>&nbsp;</td>
		</tr>
		<? 
			if($subatividades[0]) :
				foreach($subatividades as $subatividade) :
					$atividadepacto = carregarExecucaoSubAcao(array("iusd"=>$iusd_execucao,"suaid"=>$subatividade['suaid'],"picid"=>$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid']));
				?>
				<tr>
					<td>&nbsp;&nbsp;&nbsp;&nbsp;<img src="../imagens/seta_filho.gif"> <?=$subatividade['suadesc'] ?></td>
					<td><? progressBar($atividadepacto['apassituacao']); ?></td>
					<td><?=(($subatividade['suadatainicioprev'])?$subatividade['suadatainicioprev']:"-") ?></td>
					<td><?=(($subatividade['suadatafimprev'])?$subatividade['suadatafimprev']:"-") ?></td>
					<td><?=(($atividadepacto['apadatainicio'])?$atividadepacto['apadatainicio']:"-") ?></td>
					<td><?=(($atividadepacto['apadatafim'] && $atividadepacto['apassituacao']==100)?$atividadepacto['apadatafim']:"-") ?></td>
					<td>
					<? if($subatividade['suadocumento']) : ?>
					<center><input type="button" value="CLIQUE AQUI" onclick="window.location='<?=str_replace(array("{modulo}"),array($_REQUEST['modulo']),$subatividade['suadocumento']); ?>'"></center>
					<? else : ?>
					&nbsp;
					<? endif; ?>
					</td>
				</tr>
				<?
				endforeach;
			endif;
		endforeach;  
	endif; 
	?>
	</table>
	
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td><input type="button" name="proximo" value="Próximo" onclick="divCarregando();window.location='sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=dados';"></td>
</tr>
</table>
</form>