<script>
function recuperaSetorUsuarioLogado(){
	return '<?php echo $_SESSION['tarefa']['setorUsuarioLogado']; ?>';
}
		
function recuperaCpfUsuarioLogado(){
	return '<?php echo $_SESSION['usucpf']; ?>';
}

function verificaSeGerente(){
	return '<?php echo $_SESSION['tarefa']['boPerfilGerente']; ?>';
}

function verificaSeSuperUsuario(){
	return '<?php echo $_SESSION['tarefa']['boPerfilSuperUsuario']; ?>';
}
</script>
<?php 
require_once APPRAIZ . "tarefa/classes/Tarefa.class.inc";
require_once APPRAIZ . "tarefa/classes/Solicitante.class.inc";
require_once APPRAIZ . "includes/classes/dateTime.inc";

//Mata sessão para incluir nova tarefa
if($_GET['acao']=='I' && $_GET['mantemIescod']!='t'){
	unset($_SESSION['arSolicitante']);
	unset($_SESSION['iescodSession']);
}

# Verifica se existe tarefa
if($_GET['tarid']){
	boExisteTarefa( $_GET['tarid'], true );
}

# Verifica Sessão Ativa
verificaSessao();

if($taridSol = $_SESSION['arSolicitante'][0][0]['tarid']){
	# Verifica se existe tarefa
	if( !$boExisteTarefa = boExisteTarefa( $taridSol ) ){
		unset($_SESSION['arSolicitante']);
	}
}
	
/**
 * AÇÃO GRAVAR
 */
if($_POST['acaoForm']){
	header('content-type: text/html; charset=ISO-8859-1');
	extract($_POST);
	$obTarefa = new Tarefa();
	
	//inserção do campos "'tartiponumsidoc','tarnumsidoc'" no array
	$arCampos = array('tarid','tartitulo','tardsc','tartema','tmdid','tartiponumsidoc','tarnumsidoc','tarprioridade','unaidsetororigem','unaidsetorresponsavel','sitid','tardepexterna');
	
	$obTarefa->popularObjeto($arCampos);
	$obTarefa->tartipo		 		  = "T";
	
	$obData = new Data();
	if($tardatarecebimento){
		$obTarefa->tardatarecebimento  = $obData->formataData($tardatarecebimento,"YYYY-mm-dd");
	}
	$obTarefa->tardatainicio 		   = $obData->formataData($tardatainicio,"YYYY-mm-dd");
	$obTarefa->tardataprazoatendimento = $obData->formataData($tardataprazoatendimento,"YYYY-mm-dd");
	$obTarefa->tardataprazolinhadebase = $obData->formataData($tardataprazoatendimento,"YYYY-mm-dd");
	$obTarefa->tarnumsidoc 			   = str_replace(".","", str_replace( "-", "", str_replace( "/", "", $_POST['tarnumsidoc'])));
	$obTarefa->tartiponumsidoc 		   = $_POST['tartiponumsidoc'];
	
	if(!$obTarefa->tarid){
		$sql = "select 
					max(_tarordem)
				from tarefa.tarefa where _tarpai is null ";
		$max_tarordem = $db->pegaUm($sql);
		$tamanho = strlen($max_tarordem);
		$tamanho = ($tamanho) ? $tamanho : 4;
		$soma = intval($max_tarordem) + 1;
		$max_ordem = str_pad($soma, $tamanho, 0, STR_PAD_LEFT);
		$obTarefa->_tarordem = $max_ordem;		
	}
	
	$obTarefa->usucpfresponsavel = ($usucpfresponsavel) ? $usucpfresponsavel : null;
	$arCamposNulo = array('usucpfresponsavel');

	$obTarefa->salvar(true, true, $arCamposNulo);
	if($acodsc){
		$obTarefa->salvarAcompanhamento($_POST, true);
	}
	
	/**
	 * GRAVA SOLICITANTE
	 */
	 //ver($_SESSION['arSolicitante'],d);
	
	if(isset($_SESSION['arSolicitante']) && is_array($_SESSION['arSolicitante']) && count($_SESSION['arSolicitante'][0])){
		$obSolicitante = new Solicitante();
		$obSolicitante->deletaPorTarid($obTarefa->tarid);
		
		foreach($_SESSION['arSolicitante'] as $arSolicitante){
			foreach($arSolicitante as $solicitante){
			   $obSolicitante->solid 			  = null;
			   $obSolicitante->tarid 			  = $obTarefa->tarid;
			   $obSolicitante->soltiposolicitante = ($solicitante['soltiposolicitante']) ? $solicitante['soltiposolicitante'] : null;
			   $obSolicitante->soltipopessoa 	  = ($solicitante['soltipopessoa']) ? $solicitante['soltipopessoa'] : null;
			   $obSolicitante->unaid 			  = ($solicitante['unaid']) ? $solicitante['unaid'] : null;
			   $obSolicitante->estuf 			  = ($solicitante['estuf']) ? $solicitante['estuf'] : null;
			   $obSolicitante->iesid 			  = ($solicitante['iesid']) ? $solicitante['iesid'] : null;
			   $obSolicitante->muncod 			  = ($solicitante['muncod']) ? $solicitante['muncod'] : null;
			   $obSolicitante->solnome 			  = ($solicitante['solnome']) ? $solicitante['solnome'] : null;
			   $obSolicitante->soltelefone 		  = ($solicitante['soltelefone']) ? $solicitante['soltelefone'] : null;
			   $obSolicitante->solemail  		  = ($solicitante['solemail']) ? $solicitante['solemail'] : null;	   
			   $obSolicitante->entid 			  = ($solicitante['entid']) ? $solicitante['entid'] : null;
			   $obSolicitante->solcpf 			  = ($solicitante['solcpf']) ? $solicitante['solcpf'] : null;
			   $obSolicitante->solendereco 		  = ($solicitante['solendereco']) ? $solicitante['solendereco'] : null;
			   $obSolicitante->salvar();
			}
		}
	}else{
		$obSolicitante = new Solicitante();
		$obSolicitante->deletaPorTarid($obTarefa->tarid);
	}
	
	/**
	 * GRAVA INSTITUIÇÕES
	 */
	$sql = "DELETE FROM tarefa.instituicaorelacionada WHERE tarid = {$obTarefa->tarid} ";
	$db->executar($sql);
	
	$instituicoes = (is_array($instituicoes) && $instituicoes[0] != '') ? $instituicoes : array();
	
	if( is_array( $_SESSION['iescodSession'])){
		$arUnique = array_unique( $_SESSION['iescodSession'] ); 
		foreach($arUnique as $iescod){
			$sql = "INSERT INTO tarefa.instituicaorelacionada (tarid, iesid)
		   			VALUES ('".$obTarefa->tarid."','".$iescod."') ";
			$db->executar($sql);
		}
	}
	
	$obTarefa->commit();
	
	if($obTarefa->usucpfresponsavel){
		$usucpfresponsavel = $obTarefa->usucpfresponsavel;
		$arAcompanhamento = $obTarefa->recuperaAcompanhamentoTarid($obTarefa->tarid);	
		$email = $obTarefa->recuperaEmailPorCpf($usucpfresponsavel);
		$nrtarefa = $obTarefa->pegaTartarefaPorTarid($obTarefa->tarid);
		enviarEmailTarefa($usucpfresponsavel,$email, $arAcompanhamento, $obTarefa->tartitulo, $nrtarefa );
	}
	
	$_REQUEST['acao'] = 'A';
	$db->sucesso("principal/cadAcompanhamento","&tarid=".$obTarefa->tarid);
	unset($obTarefa);
	die;
}

if($_SESSION['dados_tarefa']['tarid'] && $_REQUEST['acao'] == "A"){
	$tarid = $_SESSION['dados_tarefa']['tarid'];
} elseif($_SESSION['_tartarefa'] && $_REQUEST['acao'] == "A"){
	$tarid = $_SESSION['_tartarefa'];
} elseif($_GET['tarid'] && $_REQUEST['acao'] == "A"){
	$tarid = $_GET['tarid'];
} elseif($_SESSION['tarid'] && $_REQUEST['acao'] == "A"){
	$tarid = $_SESSION['tarid'];
}

$_SESSION['dados_tarefa']['tarid'] = $tarid;

if($tarid){
	# Verifica se existe tarefa
	boExisteTarefa( $tarid, true );
}

/**
 * OBJETO TAREFA
 */
$obTarefa = new Tarefa($tarid);
if($obTarefa->tarid){
	$_SESSION['tarid'] = $obTarefa->tarid;
	$_SESSION['_tartarefa'] = $obTarefa->tarid;
} elseif(isset($_SESSION['cadTarefa'])) {
	$arCadTarefa = $_SESSION['cadTarefa'];
	//inserção dos campos "'tartiponumsidoc','tarnumsidoc'" no array
	$arAtributos = array(
					  	'tarid' => null, 
					  	'tartipo' => null, 
					  	'_tartarefa' => null, 
					  	'_tarpai' => null, 
					  	'_tarordem' => null, 
					  	'_tarprofundidade' => null,
					  	'tartitulo' => null, 
					  	'tardsc' => null, 
					  	'tartema' => null, 
					  	'tmdid' => null,
						'tartiponumsidoc' => null,
						'tarnumsidoc' => null, 
					  	'tarprioridade' => null, 
					  	'unaidsetororigem' => null, 
					  	'unaidsetorresponsavel' => null, 
					  	'usucpfresponsavel' => null, 
					  	'sitid' => null, 
					  	'tardatarecebimento' => null, 
					  	'tardatainicio' => null, 
					  	'tardataprazolinhadebase' => null, 
					  	'tardataprazoatendimento' => null, 
					  	'tardataconclusao' => null,
					  	'tardepexterna' => null
					  );
	
	foreach ($arCadTarefa as $key => $value) {
		if(array_key_exists( $key, $arAtributos )){
			$obTarefa->$key = iconv( "UTF-8", "ISO-8859-1", $value);			
		}
		if($key == 'acodsc'){
			$acodsc = iconv( "UTF-8", "ISO-8859-1", $value);
		}
	}
	unset($_SESSION['cadTarefa']);
}

if(isset($_SESSION['iescodSession'])){
	$instituicoesSelecionadas = implode(',',$_SESSION['iescodSession']);
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery.js"></script>
<script type="text/javascript">
var jQuery = jQuery.noConflict();
jQuery(document).ready(
	function () {
			validacaoInicial();	
	});
</script>
<script src="../includes/prototype.js" type="text/javascript"></script>
<script src="./js/tarefa.js" type="text/javascript"></script>
<script src="./js/ajax.js" type="text/javascript"></script>
<center>
	<div id="aguarde_" style="display: none;position:absolute;color:#000033;top:50%;left:35%; width:300;font-size:12px;z-index:0;">
		<br><img src="../imagens/carregando.gif" border="0" align="middle"><br>Carregando...<br>
	</div>
</center>
<script src="./js/validacaoAtendimento.js" type="text/javascript"></script>
<form method="post" name="formulario" id="formulario" action="/tarefa/tarefa.php?modulo=principal/cadTarefa&acao=A">
<input type="hidden" name="acaoForm" id="acaoForm" value="1" />
<input type="hidden" name="REQUEST_URI" id="REQUEST_URI" value="<?=$_SERVER['REQUEST_URI'];?>" />
<input type="hidden" name="cadTarefa" id="cadTarefa" value="1" />
<input type="hidden" name="tipo" id="tipo" value="addSolicitante" />
<input type="hidden" name="tarid" id="tarid" value="<? echo $obTarefa->tarid; ?>" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<!-- DADOS DA TAREFA -->
		<?php 
			echo blocoDadosTarefa($obTarefa, $arCadTarefa,$instituicoesSelecionadas);
		?>
		<!-- DADOS SOLICITANTES -->
		<?php
 			if( !$_GET['mantemIescod'] ){
 				$_SESSION['iescodSession'] = array();		 
 			}else{
 				$_SESSION['iescodSession'] = array_unique( $_SESSION['iescodSession'] );
 			}
 			unset( $_SESSION['iescodSessionEntidade']); 
 			getInstituicaoCadastrada(); 
			echo blocoDadosSolicitante($obTarefa, $arCadTarefa);
		?>
		<!-- DADOS VINCULADAS -->
		<?php
			echo blocoDadosAtendimento($obTarefa,'N',false,true,$acodsc,$arCadTarefa);
		?>
	</table>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-top:none">
		<tr>
			<td class="SubTituloDireita" colspan="2" style="text-align:center">
				<?php $disabled = ($_SESSION['tarefa']['boPerfilGerente'] == 'N') ? 'disabled="disabled"' : ''; ?> 
				<input type="button" value="Salvar" onclick="enviaForm();" <?php echo $disabled; ?> />
				<?php $stNomeBotao = ($_REQUEST['acao'] == 'A') ? "Incluir Novo" : "Limpar"; ?> 
				<input type="button" value="<?php echo $stNomeBotao; ?>" onclick="novaTarefa();" />
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">
<!--
    function enviaForm(){
	 	var nomeform 		= 'formulario';
		var submeterForm 	= true;
		var campos 			= new Array();
		var tiposDeCampos 	= new Array();
		
		campos[0] 			= "tartitulo";
		campos[1] 			= "tmdid";
		campos[2] 			= "tartema";
		campos[3] 			= "tardatarecebimento";
		campos[4] 			= "unaidsetororigem";
		campos[5] 			= "unaidsetorresponsavel";
		campos[6] 			= "tarprioridade";
		campos[7] 			= "tardatainicio";
		campos[8] 			= "tardataprazoatendimento";
		campos[9] 			= "tardataprazolinhadebase";
		campos[10] 			= "sitid";
		
		tiposDeCampos[0] 	= "texto";
		tiposDeCampos[1] 	= "radio";
		tiposDeCampos[2] 	= "texto";
		tiposDeCampos[3] 	= "data";
		tiposDeCampos[4] 	= "select";
		tiposDeCampos[5] 	= "select";
		tiposDeCampos[6] 	= "radio";
		tiposDeCampos[7] 	= "data";
		tiposDeCampos[8] 	= "data";
		tiposDeCampos[9] 	= "data";
		tiposDeCampos[10] 	= "select";
		
		var msg = "";
		var solicitante = '<?php echo $_SESSION['arSolicitante'][0][0]['solnome']; ?>';
		var instituicoes = document.getElementById('instituicoes');
		
		if(solicitante == ''){
			msg += "É necessário preencher ao menos um Solicitante.\n";
		}
		
		/*if(instituicoes.length > 0){
			if(instituicoes.length == 1){
				var valor = instituicoes.options[0].value;
				if(valor == ""){
					msg += "O campo Instituição Avaliada é obrigatório. \n";
				}
			}
		} else {
			msg += "O campo Instituição Avaliada é obrigatório. \n";
		}*/
		
		/*if(($('sitid').value == 3 || $('sitid').value == 4 || $('sitid').value == 5) && ($('acodsc').value == "" && ($('sitidAnterior').value != $('sitid').value)) ){
			msg += "O campo Mensagem é obrigatório. \n";
		}*/
		
		if(msg != ""){
			alert(msg);
			return false;		
		}
//		if(instituicoes.length > 1){
//			selectAllOptions( instituicoes );
//		}
		validaForm(nomeform, campos, tiposDeCampos, submeterForm )
	}
	
	function abrirPopupSolicitante(solid, tarid){
//		var instituicoes = document.getElementById('instituicoes');
//		selectAllOptions( instituicoes );

		var urlPai = '<?php echo $_SERVER['REQUEST_URI']; ?>';
		//alert(urlPai);
		//return false;

		var data = $('formulario').serialize(true);
		new Ajax.Request('ajax.php',
		{  
			method: 'post',   
			parameters: data,   
			onComplete: function(r)
			{
				//$('teste').update(r.responseText);
				if(solid){
					window.open('tarefa.php?modulo=principal/popupSolicitante&acao=A&formAcao=1&solid='+solid+'&tarid='+tarid+'&urlPai='+urlPai,'','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=700,height=315');
				} else {
					window.open('tarefa.php?modulo=principal/popupSolicitante&acao=A&tarid='+tarid+'&urlPai='+urlPai,'','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=700,height=315');
				}
			}
		});
	}
	
	function excluirSolicitante(solid){
		if (!confirm('Atenção! O item selecionado será apagado!\nDeseja continuar?')) {
			return false;
		}
		var data = 'tipo=excluir_solicitante&solid='+solid;
		new Ajax.Request('ajax.php',
		{  
			method: 'post',   
			parameters: data,   
			onComplete: function(r)
			{
				//$('teste').update(r.responseText);
				var tabela 	= document.getElementById( "listaSolicitante" );
				var linha 	= document.getElementById( 'tr_sol'+solid );
				tabela.deleteRow( linha.rowIndex );
			}
		});
	}
	
	function novaTarefa(){
		var data = 'tipo=novaTarefa';
		new Ajax.Request('ajax.php',
		{  
			method: 'post',   
			parameters: data,
			asynchronous: false,
			onComplete: function(r)
			{
				window.location.href='tarefa.php?modulo=principal/cadTarefa&acao=I';
			}
		});
	}
	validacaoInicial();
	//var jQuery = jQuery.noConflict();
	/*jQuery(function() {
		jQuery('#lista a').tooltip({
			track: true,
			delay: 0,
			showURL: false,
			showBody: " - ",
			fade: 250
		});
		
	});*/
	
-->
</script>
<div id="teste"></div>