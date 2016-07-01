<?php
//ver($_POST,d);
if( !$_SESSION['par']['inuid'] ){
	print "<script>"
		. "    alert('Problema de acesso!');"
		. "    history.back(-1);"
		. "</script>";
	die;
}

include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
include_once APPRAIZ . "par/classes/controle/CampoExternoControle.class.inc";
include_once APPRAIZ . "par/classes/modelo/QuestoesPontuaisAnexos.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include  	 APPRAIZ . "includes/cabecalho.inc";

if(!possuiPerfil(array(PAR_PERFIL_SUPER_USUARIO, PAR_PERFIL_ADMINISTRADOR)) || $_SESSION['par']['boPerfilConsulta']){
	$oDadosUnidade = new DadosUnidade();
	$oDadosUnidade->verificaPreenchimentoAbas($_SESSION['par']['inuid'], $_SESSION['par']['muncod'], $_SESSION['par']['estuf'], $_SESSION['par']['itrid']);
	unset($oDadosUnidade);
}

header('Content-type: text/html; charset="iso-8859-1"',true);
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre

if( $_REQUEST['requisicao'] == 'download' ){
	ob_get_clean();
	$file = new FilesSimec();
	$file->getDownloadArquivo( $_REQUEST['arqid'] );
	die;
}

if( $_REQUEST['atualiza'] ){
	global $db;
	$sql = "UPDATE par.atualizacaopar SET atpquestoespontuais = 't' WHERE inuid = ".$_SESSION['par']['inuid'];
	$db->executar($sql);
	$db->commit();
}

if( $_REQUEST['deleta'] && $_REQUEST['arqid'] ){
	$obQuest = new QuestoesPontuaisAnexos();
	$obQuest->deletaAnexo( $_REQUEST['arqid'] );
}

if( $_POST['salvar_questionario'] && $_POST['identExterno'] && $_FILES ){
	//ver($_POST, d);
	$obMonta = new CampoExternoControle();
	$obMonta->salvar();
	//die;
}

$itrid = $_SESSION['par']['itrid'];
$inuid = $_SESSION['par']['inuid'];

if( $itrid == 1 ){ //estadual
	$qrpid = pegaQrpid( $inuid, QUESTOES_PONTUAIS_QUEID, $_SESSION['par']['estuf'] );
} else { //municipal
	$qrpid = pegaQrpid( $inuid, QUESTOES_PONTUAIS_QUEID, $_SESSION['par']['estuf'], $_SESSION['par']['muncod'] );
}

//Pega os perfis do usuário
$arrPerfil = pegaPerfilGeral();
//Desabilita as opções de salvar para os perfis de consulta

#Estrutura condicional mudada em 04/06/2012, para que seja habilitando o questionário somente para os perfis que tem essa permissão, permissão para edição.
#Habilitando ou desabilitando os botões "Salvar anterior, Salvar e Salvar próximo".  
#O valor "false" habilita o quetionário.
#O valor "true" desabilita o questionário.

if(	in_array(PAR_PERFIL_SUPER_USUARIO, $arrPerfil) ||
	in_array(PAR_PERFIL_ADMINISTRADOR, $arrPerfil) ||
	in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrPerfil) || 
	in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrPerfil) || 
	in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrPerfil) || 
	in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrPerfil) || 
	in_array(PAR_PERFIL_PREFEITO, $arrPerfil) || 
	in_array(PAR_PERFIL_ANALISTA_QUESTOES_PONTUAIS, $arrPerfil)
){
	$boQuestionarioDesabilitado = false;
}else{
	$boQuestionarioDesabilitado = true;
}

$docid = parPegarDocidParaEntidade($_SESSION['par']['inuid']);
$estadoAtualEntidade = wf_pegarEstadoAtual( $docid );

if( $estadoAtualEntidade['esdid'] != WF_DIAGNOSTICO ){
	$boQuestionarioDesabilitado = true;
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<script>
jQuery.noConflict();
</script>
<script type="text/javascript">

jQuery(document).ready(function(){
	jQuery('[name^="perg["]').live('click',function() {
		itpid = this.value;
		if(jQuery(this).is(':checked')){
			jQuery('#arquivo_itpid_'+itpid).css('display', 'block');
		} else {
			jQuery('#arquivo_itpid_'+itpid).css('display', 'none');
		}
	});
});

function excluirArquivo( arqid ){

	divCarregando();
		if (confirm('Tem certeza que deseja excluir esse anexo?')){
			jQuery.ajax({
			   type: "POST",
			   url: "par.php?modulo=principal/questoesPontuais&acao=A",
			   data: "&deleta=true&arqid="+arqid,
			   success: function(msg){
			   		alert('Anexo excluído com sucesso!');
					quest.atualizaTela();
					divCarregado();
			   }
			 });
		} else {
			divCarregado();
			return false;
	   }
}

function atualiza(){
	if (confirm('Tem certeza que deseja validar o preechimento das Questões Pontuais?')){
		jQuery.ajax({
		   type: "POST",
		   url: "par.php?modulo=principal/questoesPontuais&acao=A",
		   data: "&atualiza=true",
		   success: function(msg){
		   		alert('Operação realizada com sucesso!');
				window.location='par.php?modulo=principal/questoesPontuaisAtualizacaoPAR&acao=A';
		   }
		 });
	} else {
		return false;
	}
}

</script>
<br />
<?php 
echo cabecalho();

$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];
?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
	<tr>
		<td style="color: blue; font-size: 22px">
			<a href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore">
				<?php echo EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar, $_SESSION['par']['itrid']); ?>
			</a>
		</td>
		<td style="color: blue; font-size: 12px; text-align:right">
			<a href="par.php?modulo=principal/planoTrabalho&acao=A">Voltar à Árvore |</a>
			<a href="par.php?modulo=principal/orgaoEducacao&acao=A">Dados da Unidade | </a>
			<a href="par.php?modulo=principal/listaObrasParUnidade&acao=A"> Lista de Obras |</a>
				<label style="color:black; text-align:center">
					<b>Questões Pontuais</b>
				</label>
		</td>
	</tr>
</table>
<table bgcolor="#f5f5f5" align="center" class="tabela" >
<?php if( $_REQUEST['atualizacaopar'] == 'true' ){ ?>
	<tr>
		<td>
			<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" width="60%" class="tabela">
				<tbody>
					<tr bgcolor="#cccccc">
						<td>
							<div style="float: left; width:100%; text-align: center;">
								<b>ATUALIZAÇÃO DO PAR</b><BR>
								<b>DECLARO QUE OS DADOS ESTÃO ATUALIZADOS E VALIDADOS</b><BR> <input value="Retornar para a Atualização do PAR" type="button" id="atualizacaopar" onclick="atualiza()">
							</div>
						</td>
					</tr>
				</tbody>
			</table>
		</td>
	</tr>
<? } ?>
	<tr>
		<td>
		<fieldset style="width: 94%; background: #fff;"  >
			<legend>Questionário</legend>
			<?php
				if($_SESSION['par']['boPerfilConsulta']){
					$habilitado = 'N';
				} else {
					$habilitado = 'S';					
				}
				
				if($boQuestionarioDesabilitado){
					$habilitado = 'N';
				}
				
				if( $_REQUEST['atualizacaopar'] == 'true' ){
					$habilitado = 'S';
				}
				
				$msgDesabilitado = 'As questões pontuais só estarão disponíveis para edição no periodo de {dt_inicio} a {dt_fim}';

				$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'relatorio' => 'modulo=relatorio/relatorioQuestionario&acao=A', 'habilitado' => $habilitado, 'msgDesabilitado' => $msgDesabilitado  ) );
			?>
		</fieldset>
	</tr>
</table>
