<?php
include APPRAIZ ."includes/workflow.php";
include_once "_funcoes_proinfancia.php";
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
include_once APPRAIZ . "proinfantil/classes/controle/CampoExternoControle.class.inc";

if($_REQUEST['obrid']){
	$_SESSION['proinfantil']['obrid'] = $_REQUEST['obrid'];
}

if($_REQUEST['popup']){
	$_SESSION['proinfantil']['popup'] = 'S';
}

if( $_POST['identExterno'] ){
	$obMonta = new CampoExternoControle();
	$obMonta->salvarCodInep();
}
$obrid = $_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['proinfantil']['obrid'];

if(!$_SESSION['proinfantil']['obrid']){ ?>
<script>
	alert("Sua sessão expirou. Por favor, entre novamente!");
	location.href='proinfantil.php?modulo=inicio&acao=C';
</script>
<?php
	exit();
}


$pinid = pegaPinid( $obrid );
$_SESSION['proinfantil']['pinid'] = $pinid;
$docid = recuperaDocumentoProInfantil($_SESSION['proinfantil']['pinid']);
$esdid = pegaEstadoAtualDocumento($docid);

if($_REQUEST['requisicao'] == 'validar_preenchimento'){
	ob_clean();	
	$quest = verificarQuestionario($pinid,$esdid);
	
	echo $quest;
	exit();
}

if($_REQUEST['validar_caracter']){
	$sql = "SELECT	 		q.qrpid
			FROM 			proinfantil.questionario q
			INNER JOIN	 	questionario.resposta r ON r.qrpid = q.qrpid
            WHERE 			r.perid = 3578 AND q.pinid = {$_SESSION['proinfantil']['pinid']}";	
	$rs = $db->pegaLinha($sql);	
	
	$sql_del = "DELETE 	FROM 	questionario.resposta
  				WHERE 			perid = 3578 AND qrpid = {$rs['qrpid']}";
	
	if($db->executar($sql_del)){
		echo 'S';
		$db->commit();
	} else {
		echo 'N';
	}
	exit();
}

if($_REQUEST['verificar_pergunta']){
	$resposta = verificarRespostaRadio('1588',$pinid);
	echo $resposta;	
	exit();
}

if($_REQUEST['verificar_questionario']){
	$quest = verificarQuestionario($pinid,$esdid);
	echo $quest;
	exit();
}

if($_REQUEST['requisicao'] == 'validar_inep'){
	ob_clean();
	$respNao = '3043';
	$respSim = '3044';
	$pergAnterior = '1588'; //id da pergunta 2 - Estabelecimento está cadastrado no Educacenso?
	$entcodent = $_POST['entcodent'];
	
	$resposta = $db->pegaUm("select itpid from questionario.resposta where perid = {$pergAnterior} and qrpid = {$_POST['qrpid']}");
	
	if( empty($resposta) ){
		echo 1;
	} else {
		if( $resposta == $respSim ){ 
			//Verifica se existe escola
			$sql_v = "SELECT et.entid FROM
						entidade.entidade et
						INNER JOIN entidade.endereco ed ON et.entid = ed.entid
					WHERE et.entcodent IS NOT NULL
						AND et.entstatus = 'A'
						AND et.entcodent = '{$entcodent}'
						AND ed.muncod = '{$_SESSION['proinfantil']['muncod']}'";
			
			$rs_v = $db->pegaLinha($sql_v);
			
			if(empty($rs_v['entid'])){
				echo 2;//False
			}
		} elseif( $resposta == $respNao ){
			
			if( strlen($entcodent) != 8 ){ 
				echo 3;
			}
		}
	}
	exit();
}

$sql_mun = "SELECT		ende.muncod as muncod,
						mun.mundescricao as descricao,
						mun.estuf as estuf
			FROM		obras2.obras ois
			INNER JOIN 	entidade.endereco ende 	 ON ende.endid = ois.endid
			INNER JOIN 	territorios.municipio mun ON mun.muncod = ende.muncod
			WHERE		obrid = {$_SESSION['proinfantil']['obrid']}";

$mun = $db->pegaLinha($sql_mun);

$muncod    = $mun['muncod'];
$descricao = $mun['descricao'];
$estuf 	   = $mun['estuf'];

$_SESSION['proinfantil']['muncod'] 	  = $muncod;
$_SESSION['proinfantil']['descricao'] = $descricao;
$_SESSION['proinfantil']['estuf'] 	  = $estuf;

if( empty($_SESSION['proinfantil']['popup']) ){
	include APPRAIZ . "includes/cabecalho.inc";
	echo '<br/>';
} else {
	echo '<script type="text/javascript" src="/includes/funcoes.js"></script>
			<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
			<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
}
include_once "termoproinfancia.inc";
$aderiu = verificarAdesaoProInfancia($pinid);

if(trim($aderiu) == 'S'){
	$perfis = pegaPerfil($_SESSION["usucpf"]);

	$qrpid = pegaQrpid($pinid); 
	$quest = verificarQuestionario($pinid,$esdid);

	//$db->cria_aba($abacod_tela,$url,$parametros,$arMnuid);
	
	montaAbasProinfantil( $abacod_tela, $url, '', $quest );
	cabecalhoProInfantil($obrid);

	$habilitado = verificaLiberacao();
	
?>
<input type="hidden" name="dataOld" value="">
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
		jQuery.noConflict();

		jQuery(document).ready(function(){
			/*jQuery('#bloco').css('height', '350px');
			jQuery('#tablecentral').css('height', '350px');
			jQuery('#telaarvore > fieldset').css('height', '380px');*/
		/*	
			jQuery('#salvar_proximo').attr('disabled', 'disabled');
			
			jQuery('#proximo, .dTreeNode > a, .node').live('click', function(){
				jQuery('#salvar_proximo').attr('disabled', 'disabled');
				jQuery('#salvar_anterior').attr('disabled', 'disabled');
			});
			
			jQuery("[href^='javascript: quest.atualiza']").live('click',function(){
				jQuery('#salvar_proximo').attr('disabled', 'disabled');
				jQuery('#salvar_anterior').attr('disabled', 'disabled');
			})
		*/	
			jQuery('input[name=dataOld]').val(jQuery('input[name=perg[1587]]').val());

			jQuery('#btn_qst_salvar, #salvar_proximo, #salvar_anterior').live('click', function(){
			
				if( $('#perid').val() == 3578 ){
					if( $('#codigoinep').val() == '' ){
						alert('O campo "3 - Informar código INEP" é obrigatório.');
						return false;
					}
				} else {
					jQuery.ajax({
				   		type: "POST",
				   		url: "proinfantil.php?modulo=principal/questionarioMunicipio&acao=A",
				   		data: "requisicao=validar_preenchimento&qrpid="+$('#qrpid').val(),
				   		async: false,
				   		success: function(data){
							if( data == 'N' && $('#habilitaAba').val() != 'N' ){
								window.location.href = window.location;
							}
				   		}
					});
				} 
				
			});
			
		});
		
		function validaInep(e){
			var inep = e.value;
			$('#btn_qst_salvar').attr('disabled', true);
			$('#salvar_anterior').attr('disabled', true);
			$('#salvar_proximo').attr('disabled', true);
			
			if( inep != '' ){
				jQuery.ajax({
			   		type: "POST",
			   		url: "proinfantil.php?modulo=principal/questionarioMunicipio&acao=A",
			   		data: "requisicao=validar_inep&entcodent="+inep+"&qrpid="+$('#qrpid').val()+"&perid="+$('#perid').val(),
			   		async: false,
			   		success: function(data){
						
						/*
						Tipo de Retorno:
							1 - Não foi respondido a pergunta anterior.
							2 - Não existe codigo inep na base de dados por municipio.
							3 - Codigo inep menor que 8.
						*/
						
						if(trim(data) == '1'){
							alert('E necessário informar se o estabelecimento está cadastrado no Educacenso.');
							e.value = '';
							e.focus();
						} else if( trim(data) == '2' ){
							alert("O código do INEP informado não foi encontrado na Base do sistema. Para prosseguir favor corrigir o código INEP.");
							e.value = '';
							e.focus();
						} else if( trim(data) == '3' ){
							alert("Informe o código do INEP com 8 caracteres.");
							e.value = '';
							e.focus();
						}
			   		}
				});
			}
			$('#btn_qst_salvar').attr('disabled', false);
			$('#salvar_anterior').attr('disabled', false);
			$('#salvar_proximo').attr('disabled', false);
		}	
		
		function qvdata(obj){
			var dataAtual = "<?php echo date('Ymd')?>";
			var d = jQuery("input[name=perg[1587]]").val();
			var datax  = parseInt(d.split("/")[2].toString() + d.split("/")[1].toString() + d.split("/")[0].toString());
			if(datax){
				if(datax > dataAtual){
					alert("Os recursos financeiros serão repassados para unidade escolar de educação infantil construída com recursos do Governo Federal (Proinfância) que esteja em pleno funcionamento, ou seja, atendendo às crianças.");
					jQuery("input[name=perg[1587]]").val(jQuery('input[name=dataOld]').val());
					jQuery("input[name=perg[1587]]").focus();
				}
		 	}		 	
		}
	</script>
	<?php
	if( !empty($_SESSION['proinfantil']['popup']) ) $habilitado = 'N';
	
	$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'altDivPx' => 330, 'habilitado' => $habilitado, 'descricao' => 'N') );
}
?>
<input type="hidden" name="habilitaAba" id="habilitaAba" value="<?=$quest; ?>">
<div id="debug"></div>