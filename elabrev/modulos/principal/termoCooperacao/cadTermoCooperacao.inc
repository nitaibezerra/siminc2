<?php

#Inclui funções do termo de execução descentralizado
require_once '_funcoes_termoCooperacao.php';

if ($_REQUEST['requisicao'] == 'salvarRelCumprimento') {
    $_REQUEST['requisicao']($_POST);
    die;
}

/**
 * aba relatorio de cumprimento do objeto
 */
if($_REQUEST['requisicao'] == 'excluirNota'){
    $sql = "delete from elabrev.relcumprimentonc where rcnid = {$_REQUEST['rcnid']}";
    $db->executar($sql);
    if($db->commit()){
        echo '1';
        die;
    }
    echo '0';
    die;
}

/**
 * verfica se NC foi efeticada junto ao SIGEF
 */
if ($_POST['req'] == 'verifica_nc_sigef') {

    $sql = "
        select
            sigefid
        from
            monitora.previsaoorcamentaria
        where
            tcpid = {$_SESSION['elabrev']['tcpid']}
            and prostatus = 'A'
            and sigefid is not null
            and codsigefnc is null limit 1
    ";

    $retorno = $db->pegaLinha($sql);
    //ver($retorno);
    if ($retorno) {
        verifica_nc_sigef($retorno['sigefid']);
    }
    die;
}

/**
 * aba relatorio de cumprimento do objeto
 * Faz o download dos arquivos e anexos
 */
if ($_REQUEST['requisicao'] == 'excluirDocAnexo'
    || $_REQUEST['requisicao'] == 'dowloadDocAnexo') {
    $_REQUEST['requisicao']($_REQUEST['arqid']);
    die;
}

#Chama a função da requisição
if ($_REQUEST['req']) {
	$_REQUEST['req']($_REQUEST);
	die();
}

#Faz o download dos arquivos e anexos
if ($_REQUEST['download'] == 's') {
	require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	unset($_REQUEST['download']);
	$arqid = $_REQUEST['arqid'];
	ob_get_clean();
	$file = new FilesSimec(null,null,'monitora');
	$file->getDownloadArquivo($arqid);
	echo "<script> window.close(); </script>";
	die;
}

#Excluir arquivos ou anexos
if($_REQUEST['requisicao'] == 'excluirAnexo'){
    global $db;
	$arqid = $_POST['arqid'];
	if ( $arqid ){
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$file = new FilesSimec("arquivoprevorcamentaria", array(''), "monitora");
		//$file->setRemoveUpload($arqid);

        $sql = "DELETE FROM monitora.arquivoprevorcamentaria WHERE arqid = {$arqid}";
        $executado = $db->executar($sql);
        /*if ($executado) {
            $file->excluiArquivoFisico($arqid);
        }*/
        $db->commit();
		die('1');
	}
	die('0');
}

#Carregar os municipios via ajax pela UF
if($_REQUEST['requisicao'] == 'carregarMunicipios'){
	if( $_REQUEST['habilitado'] == "N"){
		$hab = $_REQUEST['habilitado'];
	}else{
		$hab = "S";
	}
	$sql = "select muncod as codigo, mundescricao as descricao from territorios.municipio where estuf = '{$_REQUEST['estuf']}'";
	$db->monta_combo('muncod', $sql, $hab, 'Selecione...', '', '', '', '', '', '', '', $_REQUEST['muncod']);	
	die;
}

// -- Solicitando o cadastro de notas de créditos - requisição originada
// -- em: <modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=tramite>
if ('solicitarNC' == $_REQUEST['requisicaoNC']) {
    if (solicitarCadastroDeNotasDeCreditoSIGEF($_POST)) {
        $msg = 'Sua solicitação foi realizada com sucesso.';
    } else {
        $msg = 'Não foi possível realizar sua solicitação.';
    }
    ?>
    <script>
        alert('<?php echo $msg; ?>');
        window.location = '<?php echo $_SERVER['REQUEST_URI']; ?>';
    </script>
    <?php
    die();
}

#Verifica se concedente é fnde
$concedente = '';
if( $_SESSION['elabrev']['tcpid'] ){
	$sql = " select ungcodconcedente from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']} ";
	$ungcodconcedente = $db->pegaUm( $sql );
	if( $ungcodconcedente == UG_FNDE )
		$concedente = 'fnde';
}

#Recupera estado atual do documento - integer
if($_SESSION['elabrev']['tcpid'] != ''){
	$estadoAtual = pegarEstadoAtual( $_SESSION['elabrev']['tcpid'] );
}
#Recupera perfis do usuario - array
if($_SESSION['usucpf'] != '' && $_SESSION['sisid'] != ''){
	$perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
}

#Seta as sessões para navegar no termo
$_SESSION['elabrev']['tcpid'] = $_REQUEST['tcpid'] ? $_REQUEST['tcpid'] : $_SESSION['elabrev']['tcpid'];
$_SESSION['elabrev']['termo']['aba'] = $_REQUEST['aba'] ? $_REQUEST['aba'] : $_SESSION['elabrev']['termo']['aba'];

#Recupera aba atual
$abaAtual = $_SESSION['elabrev']['termo']['aba'];

#Trata abas
if( $_SESSION['favurl'] != 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A' ){

	if( $_REQUEST['aba'] != '' && $_REQUEST['tcpid'] != '' ){
		echo "<script>
				window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=".$_REQUEST['aba']."';
			  </script>";
        exit;
	}

    if( $_REQUEST['aba'] == 'gerapdf' && $_REQUEST['tcpid'] != '' ){
		echo "<script>
				window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=".$_REQUEST['aba']."&requisicao=".$_REQUEST['requisicao']."';
			  </script>";
        exit;
	}
}

#Verifica se é popup para visualizar termo
$boPopup = false;

# Verifica se está em cadastramento ou correção
$boEmAlteracao = false;
if($_SESSION['elabrev']['tcpid']){
	$sql = "select 
				h.docid 
			from workflow.historicodocumento h
			inner join monitora.termocooperacao t on t.docid = h.docid
			inner join workflow.acaoestadodoc a on a.aedid = h.aedid
			where t.tcpid = {$_SESSION['elabrev']['tcpid']} 
			and a.esdiddestino = ".ALTERAR_TERMO_COOPERACAO;

	$boEmAlteracao = $db->pegaUm($sql);
}
$emCadastramentoCorrecao = false;
if( ($estadoAtual == EM_CADASTRAMENTO || $estadoAtual == '' || $estadoAtual == EM_DILIGENCIA) && !$boEmAlteracao ){
	$emCadastramentoCorrecao = true;
}

// Começa tudo bloqueado
#Outras Abas
$habilita 				= 'N';
$habilita_Und_G 		= 'N';
$habilitaSalvar 		= 'N';
$habilitaInserir 		= 'N';
$habilita_pro_con 		= 'N';
$habilita_parecer		= 'N';
$habilita_cumprimento	= 'N';
#Aba Previsão Orçamentária
$habilita_Plano  		= 'N';
$habilita_Natur  		= 'N';
$habilita_botao  		= 'N';
$habilita_Parc   		= 'N';
$habilita_Acao   		= 'N';
$habilita_Meslib 		= 'N';
#Aba de Anexo
$habilita_botao 		= 'N';


/**
 * OUTRAS ABAS
 */
// Verifica se pode cadastrar e editar
if(	$emCadastramentoCorrecao && in_array(UO_EQUIPE_TECNICA, $perfis) && verificaPodeInserirEditarEquipeTecnica() ){		
	$habilita 				= 'S';
	$habilita_Und_G 		= 'S';
	$habilitaSalvar 		= 'S';
	$habilitaInserir 		= 'S';
	$habilita_Und_G 		= 'S';
	$habilita_pro_con 		= 'S';
}

// Alteração de termo 
if( in_array($abaAtual,array('previsao', 'anexo')) && verificaPodeInserirEditarEquipeTecnica() && ( ( $estadoAtual == ALTERAR_TERMO_COOPERACAO && in_array(UO_EQUIPE_TECNICA, $perfis)) || ( $boEmAlteracao && $estadoAtual == EM_DILIGENCIA ) ) ){
	$habilita 				= 'S';		
	$habilitaSalvar 		= 'S';
	$habilitaInserir 		= 'S';		
}
// Habilita salvar na aba previsao orçamentaria
if(in_array($abaAtual, array('previsao')) && in_array(PERFIL_COORDENADOR_SEC, $perfis)){
	$habilita 				= 'S';
	$habilitaSalvar 		= 'S';
}
// Habilita salvar o relatorio de cumprimento
if(in_array($abaAtual, array('cumprimento')) && verificaPodeInserirEditarEquipeTecnica() && ( ( in_array(UO_EQUIPE_TECNICA, $perfis)  && $estadoAtual != RELATORIO_OBJ_AGUARDANDO_APROV_GESTOR)  || in_array(PERFIL_SUPER_USUARIO, $perfis) ) ){
	$habilita 				= 'S';
	$habilitaSalvar 		= 'S';
	$habilita_cumprimento	= 'S';
	
}
// Verifica se está em cadastramento ou diligência
if( $estadoAtual == EM_ANALISE_OU_PENDENTE && in_array($_SESSION['elabrev']['termo']['aba'], array('previsao', 'parecertecnico')) ){		
	if( testaRespUGConcedente() ){
		$habilita 				= 'S';
		$habilita_Und_G 		= 'S';
		$habilitaSalvar 		= 'S';
		$habilitaInserir 		= 'S';
		$habilita_pro_con 		= 'S';
		$habilita_parecer		= 'S';
	}

    if (in_array(PERFIL_GABINETE_SECRETARIA_AUTARQUIA, $perfis)) {
        $habilitaSalvar 		= 'N';
    }
}

/**
 * ABA PREVISÃO ORÇAMENTÁRIA
 */
// Verifica se está em cadastramento ou diligência
$emCadastramentoCorrecao = false;
if($estadoAtual == EM_CADASTRAMENTO || $estadoAtual == '' || $estadoAtual == EM_DILIGENCIA){
	$emCadastramentoCorrecao = true;
}
// Em cadastramento libera somente os campos da unidade tecnica
if(in_array($estadoAtual, array(EM_DILIGENCIA, EM_CADASTRAMENTO, ALTERAR_TERMO_COOPERACAO)) && in_array(UO_EQUIPE_TECNICA, $perfis) && verificaPodeInserirEditarEquipeTecnica()){
	$habilita_Natur  = 'S';
	$habilita_botao  = 'S';
	$habilita_Parc   = 'S';
}else
	// Se estiver em diligencia libera tudo pra equipe tecnica
	if(in_array($estadoAtual, array(EM_DILIGENCIA, EM_ANALISE_OU_PENDENTE, ALTERAR_TERMO_COOPERACAO)) && (in_array(PERFIL_COORDENADOR_SEC, $perfis) || ( in_array(UO_EQUIPE_TECNICA, $perfis) && verificaPodeInserirEditarEquipeTecnica() ) ) ){
	$habilita_Plano  = 'S';
	$habilita_Meslib = 'S';
}
// Verifica se está em cadastramento ou diligência
if( $estadoAtual == EM_ANALISE_OU_PENDENTE && $_SESSION['elabrev']['termo']['aba'] == 'previsao'){
    if (testaRespUGConcedente()) {
        $habilita_Plano  = 'S';
// 	    $habilita_Natur  = 'S';
// 	    $habilita_botao  = 'S';
// 	    $habilita_Parc   = 'S';
// 	    $habilita_Acao   = 'S';
        $habilita_Meslib = 'S';
        if (in_array(UO_EQUIPE_TECNICA, $perfis)) {
            $habilitaSalvar 		= 'N';
        }
    } else {
        if (in_array(UO_EQUIPE_TECNICA, $perfis)) {
            $habilitaSalvar 		= 'N';
        }
    }
}

// Verifica se pode editar parecer tecnico
if(in_array($abaAtual,array('parecertecnico', 'previsao')) && $estadoAtual == EM_ANALISE_OU_PENDENTE && in_array(PERFIL_COORDENADOR_SEC, $perfis)){
    $habilita 				= 'S';
    $habilita_parecer		= 'S';
    $habilitaSalvar 		= 'S';
}

/**
 * ABA DE ANEXO 
 */
// Habilita se estiver em cadastramento ou diligencia para equipe tecnica
if( $estadoAtual == RELATORIO_OBJ_AGUARDANDO_ANALISE_COORD && in_array(PERFIL_COORDENADOR_SEC, $perfis)){
	$habilita_botao = 'S';
}else if(in_array($estadoAtual, array(EM_CADASTRAMENTO, EM_DILIGENCIA)) && in_array(UO_EQUIPE_TECNICA, $perfis) && verificaPodeInserirEditarEquipeTecnica() ){
	$habilita_botao = 'S';
}

/**
 * HABILITA TUDO SE FOR SUPER USUÁRIO
 */
if(in_array(PERFIL_SUPER_USUARIO, $perfis)){
	#OUTRAS ABAS
	$habilita 				= 'S';
	$habilita_Und_G 		= 'S';
	$habilitaSalvar 		= 'S';
	$habilitaInserir 		= 'S';
	$habilita_pro_con 		= 'S';
	$habilita_parecer		= 'S';
	#ABA PREVISÃO ORÇAMENTÁRIA
	$habilita_Plano  		= 'S';
	$habilita_Natur  		= 'S';
	$habilita_botao  		= 'S';
	$habilita_Parc   		= 'S';
	$habilita_Acao   		= 'S';
	$habilita_Meslib 		= 'S';
	# ABA ANEXO
	$habilita_botao 		= 'S';
}

$habilita = $habilita ? $habilita : 'N';

#Faz o include do arquivo referente a aba
if(!empty($abaAtual)){
	include APPRAIZ."elabrev/modulos/principal/termoCooperacao/aba".ucfirst($abaAtual).".inc";
}	
?>
<div id="dialog" title="Basic dialog" style="display:none;">
  <p></p>
  <div id="_response_">
      <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td colspan="2" class="subtitulocentro">
                Informe o Login e Senha do sistema Documenta
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita">Login</td>
            <td><input type="text" id="logindoc" name="logindoc" /></td>
        </tr>
        <tr>
            <td class="subtitulodireita">Senha</td>
            <td><input type="password" id="senhadoc" name="senhadoc" /></td>
        </tr>
      </table>
  </div>
  <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
  	<tr>
  		<td colspan="2" class="subtitulocentro">
  			<input type="button" id="logar_dialog" name="logar_dialog" value="Logar">
  			<input type="button" id="fechar_dialog" name="fechar_dialog" value="Fechar">
  			<input type="hidden" id="tcpiddoc" name="tcpiddoc" value="">
  		</td>  		
  	</tr>
  </table>
</div>