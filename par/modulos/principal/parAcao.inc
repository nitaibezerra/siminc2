<?php
function salvarAcao()
{
	
	$nome  = strlen($_POST['acinomeresponsavel']) < 100 ? $_POST['acinomeresponsavel'] : substr($_POST['acinomeresponsavel'], 0, 99);
	$cargo = strlen($_POST['acicargoresponsavel']) < 100 ? $_POST['acicargoresponsavel'] : substr($_POST['acicargoresponsavel'], 0, 99);
	
	$oAcao = new Acao();
	$oAcao->aciid				  = $_POST['aciid'];
	$oAcao->acidsc 			  	  = $_POST['acidsc'];
	$oAcao->acinomeresponsavel	  = $nome;
	$oAcao->acicargoresponsavel   = $cargo;
	$oAcao->acidemandapotencial   = $_POST['acidemandapotencial'];
	$oAcao->aciresultadoesperado  = $_POST['aciresultadoesperado'];
	$oAcao->acidata			 	  = 'now()';
	$oAcao->usucpf				  = $_SESSION['usucpf'];
	$oAcao->salvar();
	$oAcao->commit();
	$_SESSION['par']['msg'] = "Operação realizado com sucesso!";
	header("Location: par.php?modulo=principal/parAcao&acao=A&aciid={$_POST['aciid']}");
	exit;
}

if($_POST['requisicao']){
	$_POST['requisicao']();
}

if(!$_SESSION['par']['inuid']){
	header("Location: par.php?modulo=inicio&acao=C");
}

//include_once APPRAIZ . "includes/workflow.php";
include_once APPRAIZ . "includes/workflow.php";

$docid = parPegarDocidParaEntidade($_SESSION['par']['inuid']);
$estadoAtual = wf_pegarEstadoAtual( $docid );

$oPontuacaoControle = new PontuacaoControle();

if( $_REQUEST['tipo'] == 'atualizacao' ){  ?>
	<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
<?php } else {
	include  APPRAIZ."includes/cabecalho.inc";
	echo'<br>';
	$db->cria_aba( $abacod_tela, $url, "");
}


$titulo_modulo = "Ação";
monta_titulo( $titulo_modulo, '&nbsp;' );

$indid = ($_GET['indid']) ? $_GET['indid'] : $_POST['indid'];
$indid = !$indid ? $_SESSION['par']['indid']: $indid;
if($indid){
	$_SESSION['par']['indid'] = $indid;
}

if($indid && $_SESSION['par']['inuid']){
	$arDados = $oPontuacaoControle->carregaPontuacao($indid);
}

$oAcao = new Acao();

$aciid = $_GET['aciid'] ? $_GET['aciid'] : $_POST['aciid'];
$aciid = !$aciid ? $_SESSION['par']['aciid'] : $aciid;
if($aciid){
	$oAcao->carregarPorId($aciid);
	$oSubacao = new Subacao();
}

if( possuiPerfil( Array(PAR_PERFIL_SUPER_USUARIO, PAR_PERFIL_ADMINISTRADOR, PAR_PERFIL_EQUIPE_TECNICA, PAR_PERFIL_EQUIPE_FINANCEIRA, PAR_PERFIL_EQUIPE_ESTADUAL,PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO) ) ){
	$habilitadoDescAcao = "S";
}else{
	$habilitadoDescAcao = "N";
}

if( possuiPerfil( 
		Array(
			PAR_PERFIL_SUPER_USUARIO, 
			PAR_PERFIL_ADMINISTRADOR, 
			PAR_PERFIL_EQUIPE_TECNICA, 
			PAR_PERFIL_EQUIPE_FINANCEIRA, 
			PAR_PERFIL_EQUIPE_ESTADUAL,
			PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, 
			PAR_PERFIL_EQUIPE_MUNICIPAL,
			PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, 
			PAR_PERFIL_PREFEITO
		) 
	) ){

        if(is_array($arrPerfil)){
            if( !in_array(PAR_PERFIL_ORGAO_DE_CONTROLE,$arrPerfil) )
                $habilitadoAcao = "S";
        }
}else{
	$habilitadoAcao = "N";
}

$arrPerfil = pegaPerfilGeral();

if(in_array(PAR_PERFIL_EQUIPE_ESTADUAL,$arrPerfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,$arrPerfil)){
	$obrig = 'S';
} else {
	$obrig = 'N';
}

//Brasil Pro
$sql = "select 
			itrid 
		from par.indicador ind 
		inner join par.area a on a.areid = ind.areid
		inner join par.dimensao d on d.dimid = a.dimid
		where 
			ind.indid = ".$indid;
$itridBP = $db->pegaUm($sql);
if( $itridBP == 3 ){
	$sql = "SELECT terid FROM par.protocolo WHERE terid = 1 AND inuid = ".$_SESSION['par']['inuid'];
	$testa = $db->pegaUm($sql);
	
	if( !$testa ){
		$elaboracaoBP = true;
		$habilitadoAcao = "S";
	}
}

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
	$(function() {
		<?php if($_SESSION['par']['msg']): ?>
			alert('<?php echo $_SESSION['par']['msg'] ?>');
			<?php unset($_SESSION['par']['msg']); ?>
		<?php endif; ?>
	});
	
	function salvarAcao()
	{
		var erro = 0;
		nome = $("[name='acinomeresponsavel']").val();
		tamanho = nome.length;
		if(tamanho > 100 ){
			alert('O nome do responsável deve ter menos que 100 caracteres.');
			this.focus();
			return false;
		}
		$("[class~=obrigatorio]").each(function() { 
			if(!this.value){
				erro = 1;
				alert('Favor preencher todos os campos obrigatórios!');
				this.focus();
				return false;
			}
		});
		
		if(erro == 0){
			$("#form_acao").submit();
		}
	}
	
	function listarSubacao(sbaid)
	{
		var local = "par.php?modulo=principal/subacao&acao=A&sbaid=" + sbaid;
		janela(local,800,600,"Subação");
	}
	
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
	<tr>
		<td style="color: blue; font-size: 22px">
			<a href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore">
				<?php $entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod']; ?>
				<?php echo EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar, $_SESSION['par']['itrid']); ?>
			</a>
		</td>
	</tr>
</table>
<form name="form_acao" id="form_acao" method="post" action="">
	<input type="hidden" name="requisicao" value="salvarAcao" >
	<input type="hidden" name="aciid" value="<?php echo $aciid ?>" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align="left" colspan="2"><strong>Localização do Critério</strong></td>
		</tr>
		<tr>
			<td width="200px" class="SubTituloDireita">Dimensão:</td>
			<td><?php echo $arDados[0]['codigodimensao'].'. '.$arDados[0]['descricaodimensao']; ?></td>
		<tr>
			<td class="SubTituloDireita">Área:</td>
			<td><?php echo $arDados[0]['codigoarea'].'. '.$arDados[0]['descricaoarea']; ?></td>
		<tr>
			<td class="SubTituloDireita">Indicador:</td>
			<td><?php echo $arDados[0]['codigoindicador'].'. '.$arDados[0]['descricaoindicador']; ?></td>
		</tr>
		 <tr>
			<td align='right' class="SubTituloDireita">Descrição da Ação:</td>
		    	<td><?php $acidsc = $oAcao->acidsc; echo campo_textarea( 'acidsc', 'S', $habilitadoDescAcao, '', 100, 5, 500 ); ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Nome do Responsável:</td>
			<td><?php $acinomeresponsavel = $oAcao->acinomeresponsavel; echo campo_texto( 'acinomeresponsavel', 'S', $habilitadoAcao, '', 120, 100, '', '' );?></td>
	    </tr>
		<tr>
			<td align='right' class="SubTituloDireita">Cargo do Responsável:</td>
			<td><?php $acicargoresponsavel= $oAcao->acicargoresponsavel; echo campo_texto( 'acicargoresponsavel', 'S', $habilitadoAcao, '', 120, 100, '', '' );?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Resultado Esperado:</td>
			<td><?php $aciresultadoesperado = $oAcao->aciresultadoesperado; echo campo_textarea( 'aciresultadoesperado', 'S', $habilitadoAcao, '', 100, 5, 500 ); ?></td>
		</tr>
		<?php if($estadoAtual['esdid'] != WF_ANALISE || $_REQUEST['tipo'] == 'atualizacao' || $elaboracaoBP){ ?>
		<tr style="background-color: #cccccc">
			<td>&nbsp;</td>
			<td>
				<?php if( $habilitadoAcao == "S" ){ ?>
				<input type="button" name="btn_salvar" value="Salvar" onclick="salvarAcao();"/>
				<?php } ?>
				<input type="button" name="btn_voltar" value="Voltar" onclick="history.back(-);"/>
			</td>
		</tr>
		<?php } ?>
	</table>
</form>
<?php 
	$arrPerfil = pegaPerfilGeral();
	
	if( ($estadoAtual['esdid'] != WF_DIAGNOSTICO) || (in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil)) ){
		$aciid ? $oSubacao->listaSubAcaoPorAcao($aciid) : "";
	} 
?>