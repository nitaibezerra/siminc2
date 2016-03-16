<?php 
//Carregar as Funções Gerais
include "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";	

function pegaQrpidEventoPRONATEC($evpcpf, $evpnome, $queid){
	$db = new cls_banco();
    	
   	$aryWhere[] = "qr.queid = {$queid}";
   	$aryWhere[] = "pr.evpcpf = '{$evpcpf}'";
   	
    $sql = "SELECT			qr.qrpid
            FROM		   	avalpronatec.eventopronatec pr
            INNER JOIN     	questionario.questionarioresposta qr ON qr.qrpid = pr.qrpid
            				".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."";

    $qrpid = $db->pegaUm($sql);
    
    if(!$qrpid){
    	$sql = "SELECT 	DISTINCT 'true'
    			FROM	questionario.questionario 		  
            	WHERE	queid = {$queid}";
            	
    	$testaQueid = $db->pegaUm($sql);
    	
    	if($testaQueid){
	        $titulo = "Evento PRONATEC - ".$evpnome;
	        $arParam = array ("queid" => $queid, "titulo" => $titulo);
	        $qrpid = GerenciaQuestionario::insereQuestionario($arParam);
	        
      		$sql = "INSERT INTO avalpronatec.eventopronatec (evpcpf, qrpid) VALUES ('{$evpcpf}', {$qrpid})";
	        $db->executar($sql);
	        $db->commit();
    	} else {
    		return false;
    	}
    }
    return $qrpid;
}

if($_SESSION['baselogin'] == 'simec_espelho_producao'){	
	define("QUEID_EVT_PRONATEC", 90);	
} else {
	define("QUEID_EVT_PRONATEC", 90);	
} ?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js" language="JavaScript"></script>
<script type="text/javascript" src="../includes/prototype.js" language="JavaScript"></script>
<script type="text/javascript" src="../includes/funcoes.js" language="JavaScript"></script>
<script language="javascript" type="text/javascript">
function finalizar(){
	return window.location.href = 'msg_pronatec.php'; 
}
</script>
<link rel="stylesheet" href="http://spp.mec.gov.br/public/js/libs/jquery-ui/css/custom-theme/jquery-ui-1.8.20.custom.css" media="screen" type="text/css">
<link rel="stylesheet" href="http://pronatec.mec.gov.br/templates/pronatec/barra_governo3/css/barra_do_governo.css" media="all" type="text/css" />
<link rel="stylesheet" href="http://pronatec.mec.gov.br/templates/pronatec/css/template.css" type="text/css"/>
<link rel="alternate stylesheet" href="http://pronatec.mec.gov.br/templates/pronatec/css/altocontraste.css" title="altoContraste" type="text/css" />
<link rel="stylesheet" href="css/style.css" type="text/css"></link>
<link rel="stylesheet" href="css/dtree.css" type="text/css"></link>

<div id="barra-brasil-v3" class="barraGovernoPreto ">
	<div id="barra-brasil-v3-marca">
	 Brasil &ndash; Governo Federal &ndash; Minist&eacute;rio da Educa&ccedil;&atilde;o
	</div>
</div>
<div id="main"> 
    <div id="logomarca">
   		<a href="http://pronatec.mec.gov.br/index.php" title="Pronatec Portal" alt="Pronatec - Programa Nacional de Acesso ao Ensino Técnico e Emprego" tabindex="1" accesskey="1">
    	<img src="http://pronatec.mec.gov.br/templates/pronatec/images/logo.png" alt="Pronatec - Programa Nacional de Acesso ao Ensino Técnico e Emprego" border="0"/></a>
    </div>
    <div id="sair"><a href="index.php"><img border="0" style="vertical-align: middle" src="../includes/layout/azul/img/bt_logoff.png">Sair</a></div>
	<div id="usuario" align="center"> Evento Inscrição PRONATEC - <?php echo $_POST['evpnome']; ?></div>
	<div id="telacentral" style="float:left !important; width:100%;">
		<?php 	
		if($_REQUEST['requisicao'] == 'exibir_questionario'){ 
			$db = new cls_banco();
			if($_SESSION['session_textoCaptcha'] == $_POST['txt_captcha']){
				$_SESSION['session_acessoPermitido'] = true;
			} else {
				$_SESSION['session_acessoPermitido'] = false;
				ob_start();
				header("Location:index.php?erro=1");
				die();
			}	
			
			if($_SESSION['session_acessoPermitido']){
				extract($_POST);
				$evpcpf = corrige_cpf($evpcpf);
				$qrpid = pegaQrpidEventoPRONATEC($evpcpf, $evpnome, QUEID_EVT_PRONATEC);
				$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'habilitado' => 'S'));
			} 
		}
		?>
	</div>
	<div id="telacentral" style="float:left !important; width:100%; height:35px;">
		<div align="center" style="height:15px;"><input type="button" value=Finalizar id="btnFinalizar" onclick="finalizar();"/></div><br>
		<div id="rodape">© 2012 Ministério da Educação. Todos os direitos reservados.</div>
	</div>
</div>	