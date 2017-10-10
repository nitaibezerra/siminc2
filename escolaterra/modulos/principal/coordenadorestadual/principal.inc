<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Principal</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
</tr>
<tr>
	<td colspan="2">
	<? if($execucao) : ?>
	
	<?
	$sql_equipe = sqlEquipeCoordenadorEstadual(array("iusid"=>$_SESSION['escolaterra']['coordenadorestadual']['iusid']));
	$tot_eq = $db->pegaUm("SELECT COUNT(*) as tot FROM (".$sql_equipe.") as foo");
	$tot_eq_at = $db->pegaUm("SELECT COUNT(*) as tot FROM (".$sql_equipe.") as foo WHERE foo.status='A'");
	if(!$tot_eq_at) $tot_eq_at="0";
	
	?>
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloEsquerda" width="80%">1. Ativar tutores no <?php echo SIGLA_SISTEMA; ?></td>
		<td><?=(($tot_eq==$tot_eq_at)?"<span style=\"color:blue;font-size:15px;\"><b>CONCLUÍDO ({$tot_eq_at}/$tot_eq)":"<span style=\"color:red;font-size:15px;\"><b>PENDENTE ({$tot_eq_at}/$tot_eq)") ?></b></span></td>
	</tr>
	</table>
	<? else : ?>
	
	<?
	$goto_pro 		= "escolaterra.php?modulo=principal/coordenadorestadual/coordenadorestadual&acao=A&aba=dados";
	
	$preenchimento  = $db->pegaUm("SELECT iustermocompromisso FROM escolaterra.identificacaousuario WHERE iusid='".$_SESSION['escolaterra']['coordenadorestadual']['iusid']."'");
	
	$esdid = $db->pegaUm("SELECT d.esdid FROM escolaterra.turmas t 
								  INNER JOIN workflow.documento d ON d.docid = t.docid  
								  WHERE iusid='".$_SESSION['escolaterra']['coordenadorestadual']['iusid']."'");
	if($esdid != ESD_EM_CADASTRAMENTO && $esdid) {
		$cadastramento_tut = true;
	}
	
	$tot_eq = $db->pegaUm("SELECT COUNT(*) as tot FROM escolaterra.turmas t 
						  INNER JOIN workflow.documento d ON d.docid = t.docid 
						  INNER JOIN escolaterra.identificacaousuario i ON i.iusid = t.iusid 
						  INNER JOIN escolaterra.tipoperfil tp ON tp.iusid = i.iusid AND tp.pflcod='".PFL_TUTOR."'
						  WHERE i.ufpid='".$_SESSION['escolaterra']['coordenadorestadual']['ufpid']."' AND i.iusstatus='A'");
	
	$tot_eq_vl = $db->pegaUm("SELECT COUNT(*) as tot FROM escolaterra.turmas t 
						  INNER JOIN workflow.documento d ON d.docid = t.docid 
						  INNER JOIN escolaterra.identificacaousuario i ON i.iusid = t.iusid 
						  INNER JOIN escolaterra.tipoperfil tp ON tp.iusid = i.iusid AND tp.pflcod='".PFL_TUTOR."'
						  WHERE i.ufpid='".$_SESSION['escolaterra']['coordenadorestadual']['ufpid']."' AND i.iusstatus='A' AND d.esdid='".ESD_VALIDADO."'");
	if(!$tot_eq_vl) $tot_eq_vl="0";
	
	?>
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloEsquerda" width="80%">1. Preenchimento dos dados cadastrais</td>
		<td><?=(($preenchimento=='t')?"<span style=\"color:blue;font-size:15px;\"><b>CONCLUÍDO":"<span style=\"color:red;font-size:15px;\"><b>PENDENTE") ?></b></span></td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" width="80%">2. Cadastramento de tutores</td>
		<td><?=(($cadastramento_tut)?"<span style=\"color:blue;font-size:15px;\"><b>CONCLUÍDO":"<span style=\"color:red;font-size:15px;\"><b>PENDENTE") ?></b></span></td>
	</tr>
	<? if(habilitarAbaValidacaoProfessores(array())) : ?>
	<tr>
		<td class="SubTituloEsquerda" width="80%">3. Validação do cadastramento dos professores</td>
		<td><?=(($tot_eq==$tot_eq_vl && $tot_eq)?"<span style=\"color:blue;font-size:15px;\"><b>CONCLUÍDO ({$tot_eq_vl}/{$tot_eq})":"<span style=\"color:red;font-size:15px;\"><b>PENDENTE ({$tot_eq_vl}/{$tot_eq})") ?></b></span></td>
	</tr>
	<? endif; ?>
	</table>
	<? endif; ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td><input type="button" value="Próximo" onclick="divCarregando();window.location='<?=$goto_pro ?>';"></td>
</tr>
</table>