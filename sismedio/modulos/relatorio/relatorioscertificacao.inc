<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

include_once "_funcoes_universidade.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

$perfis = pegaPerfilGeral();

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( "Relatório de recomendação para certificação", "" );

if(in_array(PFL_COORDENADORIES,$perfis)) {
	$sql = "SELECT uncid FROM sismedio.identificacaousuario WHERE iuscpf='".$_SESSION['usucpf']."'";
	$uncid = $db->pegaUm($sql);
	
	echo "<p align=\"center\" style=\"font-size:large;\">";
	echo $db->pegaUm("SELECT unisigla||' - '||uninome FROM sismedio.universidadecadastro uc 
				 	  INNER JOIN sismedio.universidade un ON un.uniid = uc.uniid 
				 	  WHERE uc.uncid='".$uncid."'");
	echo "</p>";
} else {
	$uncid = $_REQUEST['uncid'];
	$sql = "(
			SELECT 9999 as codigo, 'Listar TODOS' as descricao
			) UNION ALL (
			SELECT uc.uncid as codigo, un.unisigla||' - '||un.uninome as descricao FROM sismedio.universidadecadastro uc 
			INNER JOIN sismedio.universidade un ON un.uniid = uc.uniid
			)";
	echo "<p align=center>";
	$db->monta_combo('uncid', $sql, 'S', 'Selecione', 'selecionarUniversidade', '', '', '', 'S', 'uncid');
	echo "</p>";
}


?>
<script type="text/javascript">
function selecionarUniversidade(uncid) {
	if(uncid) {
		window.location='sismedio.php?modulo=relatorio/relatorioscertificacao&acao=A&uncid='+uncid;
	}
}
</script>

<? $perfis = $db->carregar("SELECT * FROM seguranca.perfil WHERE pflcod IN(".PFL_ORIENTADORESTUDO.",".PFL_PROFESSORALFABETIZADOR.",".PFL_COORDENADORPEDAGOGICO.")"); ?>


<? if($uncid) : ?>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<? if($perfis[0]) : ?>
	<? foreach($perfis as $pf) : ?>		
    <tr>
    	<td class="SubTituloEsquerda">-------- <?=$pf['pfldsc'] ?> -------- <img src="../imagens/excel.gif" style="cursor: pointer;" onclick="window.location='sismedio.php?modulo=relatorio/relatorioscertificacao&acao=A&requisicao=exibirCursistasRelatorioFinal&uncid=<?=$uncid ?>&pflcod=<?=$pf['pflcod'] ?>&xls=1';"></td>
    </tr>		
	<tr>
		<td>
		<? 
		exibirCursistasRelatorioFinal(array('uncid' => $uncid, 'pflcod' => $pf['pflcod']));
		?>
		</td>
	</tr>
	<? endforeach; ?>
	<? endif; ?>
</table>
<? endif; ?>