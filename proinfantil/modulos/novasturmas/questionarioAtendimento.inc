<script>
	$(function(){
		perid = $('[name=perid_disabled_perg]').val();
		perid_r = $('[name=perid_disabled_resp]').val();
		$('[name=perg['+perid+']]').click(function(){
			if(this.value == perid_r){
				alert('Neste momento, o município/DF não está apto a solicitar os recursos de que trata a Resolução CD/FNDE nº 28/2012.');
			}
		});
	});
</script>
<?php 
header('content-type: text/html; charset=ISO-8859-1');

if(empty($_SESSION['proinfantil']['muncod']) && isset($_REQUEST['muncod'])){
	$_SESSION['proinfantil']['muncod'] = $_REQUEST['muncod'];
}

//Chamada de programa
include_once APPRAIZ . "includes/cabecalho.inc";
include_once "_funcoes_novasturmas.php";
include_once APPRAIZ . "includes/workflow.php";
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
include_once APPRAIZ . "proinfantil/classes/controle/CampoExternoControle.class.inc";
echo "<br/>"; ?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<table class="tabela" height="100%" align="center" cellspacing="0" cellpadding="3" >
	<tr class="subtituloEsquerda">
		<td height="10" width="45%">
			<input type="button" value="Voltar" id="btnVoltar" />
		</td>
		<td class="TituloTela">Novas Turmas</td>
	</tr>
</table>
<br/>

<script type="text/javascript">
	$('#btnVoltar').click(function(){
		document.location.href = 'proinfantil.php?modulo=novasturmas/listaMunicipios&acao=A';
	});
</script>	

<?php 
$perfis = pegaPerfil($_SESSION['usucpf']);

$docid = criaDocumentoNovasTurmas($_SESSION['proinfantil']['muncod']);
$esdid = pegaEstadoAtualNovasTurmas($docid);

if( $_POST['identExterno'] ){
	$obMonta = new CampoExternoControle();
	$obMonta->salvar();
}

$arMnuid = array();
if($_SESSION['baselogin']=="simec_desenvolvimento") {
	$abacod_tela = 57524;
	if(!$_SESSION['proinfantil']['turid']){
		$arMnuid = array(11766, 11767);
	}
} else {
	$abacod_tela = 57524;
	if(!$_SESSION['proinfantil']['turid']){
		$arMnuid = array(11125);
	}
}

if( /*$esdid != WF_NOVASTURMAS_EM_ANALISE ||*/ in_array(EQUIPE_MUNICIPAL,$perfis) || in_array(SECRETARIO_ESTADUAL,$perfis)){
	$arMnuid[] = 11139;
}

$arMnuid[] = 11125;

// Monta abas
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

$titulo2 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';
monta_titulo('Questionário de Atendimento', $titulo2);
cabecalhoTurma();

$arDados = array(
				'queid' => QUEID_NOVASTURMAS_ATENDIMENTO,
				'muncod' => $_SESSION['proinfantil']['muncod']
			);

$qrpid = getQrpidNovasTurmas( $arDados );

$acesso = verificaAcessoPerfil($_SESSION['usucpf'],$esdid);

if( (int)date('Y') == ANO_TRAVA_SISTEMA ){
	$acesso['cadastro'] = 'N';
}

$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'habilitado' => $acesso['cadastro'], 'descricao' => 'N') );

?>
<input type="hidden" name="perid_disabled_perg" value="<?php echo PERID_NT_ATEND_DESABILITA_PERG; ?>" />
<input type="hidden" name="perid_disabled_resp" value="<?php echo PERID_NT_ATEND_DESABILITA_RESP; ?>" />