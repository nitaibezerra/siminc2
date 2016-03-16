<?
include APPRAIZ ."includes/workflow.php";
include "_funcoes_universidade.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sismedio.js"></script>';

echo "<br>";

$perfis = pegaPerfilGeral();

if($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis)) :
?>
<script>
function avaliarCoordenadorIES(uncid) {
	window.location='sismedio.php?modulo=principal/composicaoturmas&acao=A&uncid='+uncid;
}
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloCentro" colspan="2">
	<?
	$sql = "SELECT uncid as codigo, uninome as descricao FROM sismedio.universidadecadastro un INNER JOIN sismedio.universidade uni ON uni.uniid = un.uniid ORDER BY uni.uninome";
	$db->monta_combo('uncid', $sql, 'S', 'Selecione', 'avaliarCoordenadorIES', '', '', '', 'N', '','',$_REQUEST['uncid']);
	?>
	</td>
</tr>
</table>
<?

	$menu[] = array("id" => 1, "descricao" => "Compor Turmas ( Formadores Regionais )", "link" => "/sismedio/sismedio.php?modulo=principal/composicaoturmas&acao=A".(($_REQUEST['uncid'])?"&uncid=".$_REQUEST['uncid']:"")."&aba=fr");
	$menu[] = array("id" => 2, "descricao" => "Compor Turmas ( Orientadores de estudo )", "link" => "/sismedio/sismedio.php?modulo=principal/composicaoturmas&acao=A".(($_REQUEST['uncid'])?"&uncid=".$_REQUEST['uncid']:"")."&aba=oe");
	
	if(!$_REQUEST['aba']):$_REQUEST['aba'] = "fr";endif;


elseif(in_array(PFL_COORDENADORIES,$perfis)) :
	$_REQUEST['uncid'] = $_SESSION['sismedio']['universidade']['uncid'];

	$menu[] = array("id" => 1, "descricao" => "Compor Turmas ( Formadores Regionais )", "link" => "/sismedio/sismedio.php?modulo=principal/composicaoturmas&acao=A".(($_REQUEST['uncid'])?"&uncid=".$_REQUEST['uncid']:"")."&aba=fr");
	$menu[] = array("id" => 2, "descricao" => "Compor Turmas ( Orientadores de estudo )", "link" => "/sismedio/sismedio.php?modulo=principal/composicaoturmas&acao=A".(($_REQUEST['uncid'])?"&uncid=".$_REQUEST['uncid']:"")."&aba=oe");
	
	if(!$_REQUEST['aba']):$_REQUEST['aba'] = "fr";endif;

elseif(in_array(PFL_COORDENADORADJUNTOIES,$perfis)) :
	$_REQUEST['uncid'] = $_SESSION['sismedio']['coordenadoradjuntoies']['uncid'];

	$menu[] = array("id" => 1, "descricao" => "Compor Turmas ( Formadores Regionais )", "link" => "/sismedio/sismedio.php?modulo=principal/composicaoturmas&acao=A".(($_REQUEST['uncid'])?"&uncid=".$_REQUEST['uncid']:"")."&aba=fr");
	$menu[] = array("id" => 2, "descricao" => "Compor Turmas ( Orientadores de estudo )", "link" => "/sismedio/sismedio.php?modulo=principal/composicaoturmas&acao=A".(($_REQUEST['uncid'])?"&uncid=".$_REQUEST['uncid']:"")."&aba=oe");
	
	if(!$_REQUEST['aba']):$_REQUEST['aba'] = "fr";endif;

elseif(in_array(PFL_SUPERVISORIES,$perfis)) :
	$_REQUEST['uncid'] = $_SESSION['sismedio']['supervisories']['uncid'];

	$menu[] = array("id" => 1, "descricao" => "Compor Turmas ( Formadores Regionais )", "link" => "/sismedio/sismedio.php?modulo=principal/composicaoturmas&acao=A".(($_REQUEST['uncid'])?"&uncid=".$_REQUEST['uncid']:"")."&aba=fr");
	$menu[] = array("id" => 2, "descricao" => "Compor Turmas ( Orientadores de estudo )", "link" => "/sismedio/sismedio.php?modulo=principal/composicaoturmas&acao=A".(($_REQUEST['uncid'])?"&uncid=".$_REQUEST['uncid']:"")."&aba=oe");
	
	if(!$_REQUEST['aba']):$_REQUEST['aba'] = "fr";endif;

elseif(in_array(PFL_FORMADORREGIONAL,$perfis)) :
	$_REQUEST['uncid'] = $_SESSION['sismedio']['formadorregional']['uncid'];

	$menu[] = array("id" => 1, "descricao" => "Compor Turmas ( Orientadores de estudo )", "link" => "/sismedio/sismedio.php?modulo=principal/composicaoturmas&acao=A".(($_REQUEST['uncid'])?"&uncid=".$_REQUEST['uncid']:"")."&aba=oe");
	
	if(!$_REQUEST['aba']):$_REQUEST['aba'] = "oe";endif;
	
	$iusds = $db->carregarColuna("SELECT ot.iusd FROM sismedio.orientadorturma ot INNER JOIN sismedio.turmas tu ON tu.turid = ot.turid WHERE tu.iusd='".$_SESSION['sismedio']['formadorregional']['iusd']."'");

endif;


if($_REQUEST['uncid']) :

	$_SESSION['sismedio']['uncid'] = $_REQUEST['uncid'];
	
	
	$universidadecadastro = $db->pegaLinha("SELECT docidturmaformadoresregionais, docidturmaorientadoresestudo FROM sismedio.universidadecadastro WHERE uncid='".$_SESSION['sismedio']['uncid']."'");
	
	switch($_REQUEST['aba']) {
		case 'fr':
			
			if(!$universidadecadastro['docidturmaformadoresregionais']) {
				$_SESSION['sismedio']['docid'] = wf_cadastrarDocumento(TPD_COMPOSICAOTURMA,"SISMédio ".$_SESSION['sismedio']['uncid']);
				$db->executar("UPDATE sismedio.universidadecadastro SET docidturmaformadoresregionais='".$_SESSION['sismedio']['docid']."' WHERE uncid='".$_SESSION['sismedio']['uncid']."'");
				$db->commit();
			} else {
				$_SESSION['sismedio']['docid'] = $universidadecadastro['docidturmaformadoresregionais'];
			}
			
			$perfil_turma = PFL_FORMADORREGIONAL;
			$btn_inserir  = "Inserir Orientadores";
			break;
		case 'oe':
			
			if(!$universidadecadastro['docidturmaorientadoresestudo']) {
				$_SESSION['sismedio']['docid'] = wf_cadastrarDocumento(TPD_COMPOSICAOTURMA,"SISMédio ".$_SESSION['sismedio']['uncid']);
				$db->executar("UPDATE sismedio.universidadecadastro SET docidturmaorientadoresestudo='".$_SESSION['sismedio']['docid']."' WHERE uncid='".$_SESSION['sismedio']['uncid']."'");
				$db->commit();
			} else {
				$_SESSION['sismedio']['docid'] = $universidadecadastro['docidturmaorientadoresestudo'];
			}
			
			$perfil_turma = PFL_ORIENTADORESTUDO;
			$btn_inserir  = "Inserir Professores";
			break;
	}
	
	$estado = wf_pegarEstadoAtual( $_SESSION['sismedio']['docid'] );
	
	if($estado['esdid'] != ESD_TURMA_ABERTA) {
		$consulta = true;
	}
	
	
	$abaativa = "/sismedio/sismedio.php?modulo=principal/composicaoturmas&acao=A".(($_REQUEST['uncid'])?"&uncid=".$_REQUEST['uncid']:"")."&aba=".$_REQUEST['aba'];
	
	echo montarAbasArray($menu, $abaativa);
	
?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function carregarAlunosTurma() {
	ajaxatualizar('requisicao=carregarAlunosTurma&turid=<?=$_REQUEST['turid'] ?>','td_alunosturmas');
}

function abrirAlunosTurma(turid) {
	window.open('sismedio.php?modulo=principal/inserircomposicaoturma&acao=A&aba=<?=$_REQUEST['aba'] ?>&turid='+turid,'Turmas','scrollbars=yes,height=600,width=1100,status=no,toolbar=no,menubar=no,location=no');
}


function comporTurma(turid) {
	divCarregando();
	window.location='sismedio.php?modulo=principal/composicaoturmas&acao=A&aba=<?=$_REQUEST['aba'] ?><?=(($_REQUEST['uncid'])?"&uncid=".$_REQUEST['uncid']:"") ?>&turid='+turid;
}

function carregarListaTurmas() {
	ajaxatualizar('requisicao=carregarTurmas&uncid=<?=$_SESSION['sismedio']['uncid'] ?>','td_turmas');
}

function excluirAlunoTurma(iusd) {
	var conf = confirm("Deseja realmente excluir o aluno da turma?");
	
	if(conf) {
		ajaxatualizar('requisicao=excluirAlunoTurma&turid=<?=$_REQUEST['turid'] ?>&iusd='+iusd,'');
		carregarAlunosTurma();
	}
}


<? if($consulta) : ?>
jQuery(function() {
jQuery("[id='inserirturma']").remove();
});
<? endif; ?>

</script>

<div id="modalOrientacaoAdm" style="display:none;">
<form method="post" id="formulario_orientacao" name="formulario_orientacao">
<input type="hidden" name="abaid" id="abaid">
<input type="hidden" name="requisicao" value="salvarOrientacaoAdm">
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloCentro" colspan="2">Orientação</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%"></td>
	<td><? echo campo_textarea( 'oabdesc', 'S', 'S', '', '70', '4', '5000'); ?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="salvarorientacao" value="Salvar Orientação" onclick="salvarOrientacaoAdm();"></td>
</tr>
</table>
</form>
</div>


<input type="hidden" name="goto" id="goto" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Turmas</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao("/sismedio/sismedio.php?modulo=principal/composicaoturmas&acao=A"); ?></td>
	</tr>
	<? if($_REQUEST['turid']) : ?>
	<? $turma = carregarDadosTurma(array("turid"=>$_REQUEST['turid'])); ?>
	<tr>
		<td class="SubTituloEsquerda" colspan="2"><a href="sismedio.php?modulo=principal/composicaoturmas&acao=A&aba=<?=$_REQUEST['aba'] ?><?=(($_REQUEST['uncid'])?"&uncid=".$_REQUEST['uncid']:"") ?>">Lista de turmas</a> >> <?=$turma['turdesc'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Nome da turma</td>
		<td><?=$turma['turdesc'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Responsável</td>
		<td><?=$turma['iusnome'] ?></td>
	</tr>
	<tr>
		<td colspan="2"><input type="button" name="inseriraluno" id="inseriraluno" value="<?=$btn_inserir ?>" onclick="abrirAlunosTurma('<?=$turma['turid'] ?>');"></td>
	</tr>
	<tr>
		<td colspan="2" id="td_alunosturmas">
		<?=carregarAlunosTurma(array("turid"=>$turma['turid'])) ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Voltar para lista" onclick="divCarregando();window.location='sismedio.php?modulo=principal/composicaoturmas&acao=A&aba=<?=$_REQUEST['aba'] ?><?=(($_REQUEST['uncid'])?"&uncid=".$_REQUEST['uncid']:"") ?>';">
		</td>
	</tr>
	<? else : ?>
	<tr>
		<td colspan="2" id="td_turmas">
		
		<table width="100%">
		<tr>
			<td width="95%"><?=carregarTurmas(array("consulta" => $consulta, "uncid" => $_SESSION['sismedio']['uncid'], "pflcod" => $perfil_turma, "r_turma" => $iusds)) ?></td>
			<td valign="top"><? wf_desenhaBarraNavegacao( $_SESSION['sismedio']['docid'], array('tipo' => $_REQUEST['aba']) ); ?></td>
		</tr>
		</table>
		
		</td>
	</tr>
	<? endif; ?>
</table>
<?

endif;


?>