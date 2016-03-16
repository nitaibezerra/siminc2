<?php
require_once APPRAIZ . "tarefa/classes/Tarefa.class.inc";
require_once APPRAIZ . "tarefa/classes/Solicitante.class.inc";
require_once APPRAIZ . "tarefa/classes/Acompanhamento.class.inc";
require_once APPRAIZ . "includes/classes/dateTime.inc";
require_once APPRAIZ . "includes/classes/AbaAjax.class.inc";

if($_GET['tarid']){
	# Verifica se existe tarefa
	boExisteTarefa( $_GET['tarid'], true );
}
# Verifica Sessão Ativa
verificaSessao();

/**
 * AÇÃO GRAVAR
 */
if($_POST['acaoForm']){
	extract($_POST);
	
	$obTarefa = new Tarefa();
	
	$arCampos = array('tarid','unaidsetororigem');
	$obTarefa->popularObjeto($arCampos);
	
	if($tarprioridade){
		$obTarefa->tarprioridade = $tarprioridade;
	}
	
	if($unaidsetorresponsavel){
		$obTarefa->unaidsetorresponsavel = $unaidsetorresponsavel;
	} else {
		$obTarefa->unaidsetorresponsavel = $unaidsetorresponsavelAnterior;
	}
	
	if($sitid){
		$obTarefa->sitid = $sitid;
	} else {
		$obTarefa->sitid = $sitidAnterior;
	}
	
	if($tardepexterna){
		$obTarefa->tardepexterna = $tardepexterna;
	} else {
		$obTarefa->tardepexterna = $tardepexternaAnterior;
	}
	
	$obData = new Data();
	if($tardataprazoatendimento){
		$obTarefa->tardataprazoatendimento = $obData->formataData($tardataprazoatendimento,"YYYY-mm-dd");
	} else {
		$obTarefa->tardataprazoatendimento = $obData->formataData($tardataprazoatendimentoAnterior,"YYYY-mm-dd");		
	}
	
	
	# Verifica responsável
	if($usucpfresponsavel){
		$obTarefa->usucpfresponsavel = $usucpfresponsavel;	
	} else {
		if($boGravaNullCpfRespon>0){
			$obTarefa->usucpfresponsavel = $usucpfresponsavelAnterior;	
		} else {
			$obTarefa->usucpfresponsavel = null;
		}
	}
	
	//echo $boGravaNullCpfRespon;exit;
	
	//ver($obTarefa->usucpfresponsavel,d);
	$arCamposNulo = array('usucpfresponsavel');
	
	$obTarefa->salvar(true, true, $arCamposNulo);
	if($acodsc){
		$obTarefa->salvarAcompanhamento($_POST);
	}
	$obTarefa->commit();

	if($obTarefa->usucpfresponsavel){
		$usucpfresponsavel = $obTarefa->usucpfresponsavel;
		$arAcompanhamento = $obTarefa->recuperaAcompanhamentoTarid($obTarefa->tarid);	
		$email = $obTarefa->recuperaEmailPorCpf($usucpfresponsavel);
		$nrtarefa = $obTarefa->pegaTartarefaPorTarid($obTarefa->tarid);
		enviarEmailTarefa($usucpfresponsavel,$email, $arAcompanhamento, $obTarefa->tartitulo, $nrtarefa );
	}

	
	
/*	$_REQUEST['acao'] = 'A';
	$db->sucesso("principal/cadAcompanhamento","&tarid=".$obTarefa->tarid);
	unset($obTarefa);		
	die;*/
}

if($_GET['tarid'] && $_REQUEST['acao'] == "A"){
	$tarid = $_GET['tarid'];
} elseif($_SESSION['tarid'] && $_REQUEST['acao'] == "A"){
	$tarid = $_SESSION['tarid'];
} elseif($_SESSION['dados_tarefa']['tarid']){
	$tarid = $_SESSION['dados_tarefa']['tarid'];
}

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
	$_SESSION['_tartarefa'] = $_GET['tartarefa'];
} else {
	die("<script>alert('Tarefa não encontrada. Refaça o procedimento.');window.location='tarefa.php?modulo=principal/listaTarefas&acao=A';</script>");
}

if($_GET['taridExcluir']){
	$obTarefa = new Tarefa();
	$obTarefa->excluir($_GET['taridExcluir']);
	$obTarefa->commit();
	unset($_GET['taridExcluir']);
	unset($obTarefa);
	$db->sucesso("principal/cadAcompanhamento");
	die;
}
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
$titulo_modulo = "Lista Tarefa";
monta_titulo( $titulo_modulo, '' );
require_once 'cabecalhoIncludesListaTarefa.inc';
if($_SESSION['tarefa']['filtroArvore']){
	extract($_SESSION['tarefa']['filtroArvore']);
}
?>
<script src="./js/validacaoAtendimento.js" type="text/javascript"></script>
<form method="post" name="formulario" id="formulario" action="/tarefa/tarefa.php?modulo=principal/cadAcompanhamento&acao=A">
<input type="hidden" name="cadAcompanhamento" id="cadAcompanhamento" value="1" />
<input type="hidden" name="boGravaNullCpfRespon" id="boGravaNullCpfRespon" value="1" />
<input type="hidden" name="acaoForm" id="acaoForm" value="1" />
<input type="hidden" name="tarid" id="tarid" value="<? echo $obTarefa->tarid; ?>" />
<input type="hidden" name="tardataprazoatendimentoTarefa" id="tardataprazoatendimentoTarefa" value="<? echo $_SESSION['dados_tarefa']['tardataprazoatendimento']; ?>" />
<input type="hidden" name="boAtividade" id="boAtividade" value="<? echo $obTarefa->boAtividade(); ?>" />
<div style="display: none;">
<?php
/**
 * FEITO ISSO POR CAUSA DA PRESSA
 */
	/*$sql = "SELECT unaid as codigo, unasigla||' - '|| unadescricao as descricao FROM tarefa.unidade";
	$db->monta_combo( "filtrounaidsetororigem", $sql, 'S', 'Selecione...', '', '', '', '', 'N', 'filtrounaidsetororigem',false,null,'Setor de Origem');
	
	$sql = "SELECT unaid as codigo, unasigla||' - '|| unadescricao as descricao FROM tarefa.unidade";
	$db->monta_combo( "filtrounaidsetorresponsavel", $sql, 'S', 'Selecione...', '', '', '', '', 'N', 'filtrounaidsetorresponsavel',false,null,'Setor de Responsável');
	
	if($filtrounaidsetorresponsavel){
		$sql = "select distinct ur.usucpf as codigo, u.usunome as descricao 
					from tarefa.usuarioresponsabilidade ur
   					inner join seguranca.usuario u on ur.usucpf = u.usucpf
   					where ur.unaid = {$filtrounaidsetorresponsavel} and ur.rpustatus = 'A' ";
	
		$db->monta_combo('filtrousucpfresponsavel', $sql, $habilitado, "Selecione um Setor Responsável", '', '', '', '405', 'N', 'filtrousucpfresponsavel',false,null,'Responsável pela Tarefa');
	} else {
		$db->monta_combo('filtrousucpfresponsavel', array(), 'S', "Selecione um Setor Responsável", '', '', '', '405', 'N', 'filtrousucpfresponsavel',false,null,'Responsável pela Tarefa');
	}
	
	$sql = "SELECT 
                            sitid AS codigo, 
                            sitdsc AS descricao
                        FROM
                            tarefa.situacaotarefa order by codigo";
	$arSituacao = $db->carregar($sql);
	$filtrosituacao = ($filtrosituacao) ? $filtrosituacao : array(); 
	foreach($arSituacao as $situacao){
		if(in_array($situacao['codigo'], $filtrosituacao)){
			$ckecked = "checked=\"checked\"";
		}
		echo "<input type=\"checkbox\" $ckecked name=\"filtrosituacao[]\" value=\"{$situacao['codigo']}\"> ".$situacao['descricao'];
		$ckecked = "";					
	}*/

?>
</div>
<div id="divCabecalho">
	<!-- CABECALHO -->
	<?php
		echo cabecalhoTarefa($obTarefa->tarid);
	?>
</div>
<br />
<!-- LISTA TAREFA -->
<div id="montaArvore">
<?php
	$tarTarefa = $obTarefa->pegaTartarefaPorTarid($obTarefa->tarid);
	montarArvore($tarTarefa,true,true,true);
	echo "<script type=\"text/javascript\">
			filtroPesquisa('$tarTarefa', 1);
		  </script>";
	
?>
</div>
<br />
<?php
/**
 * CRIA ABA
 */
$obAbaAjax = new AbaAjax(null,false, true, true);

# DEFINE ABA PADRÃO
$padraoAtendimento = $padraoRestricao = false;
if(!$_GET['boPadraoRetricao']){
	$padraoAtendimento = true;
} else {
	$padraoRestricao = true;
}
$arAba = array(
			  0 => array("descricao" => "Atendimento", 
		   				 "padrao" => $padraoAtendimento,
		   				 "elemento" => array( "tarid" ),
			  			 "url" => "ajax.php",
		   				 "parametro" => "tipo=abaAtendimento"),
			  1 => array("descricao" => "Restricao", 
		   				 "padrao" => $padraoRestricao,
			  			 "elemento" => array( "tarid" ),
		   				 "url" => "ajax.php",
		   				 "parametro" => "tipo=abaRestricao")
	  	      );
$obAbaAjax->criaAba($arAba, 'divAcompanhamento');
?>
</form>
<script type="text/javascript">
<!--	
    function enviaForm(){
	 	var nomeform 		= 'formulario';
		var submeterForm 	= true;
		var campos 			= new Array();
		var tiposDeCampos 	= new Array();
		
		campos[0] 			= "unaidsetorresponsavel";
		campos[1] 			= "usucpfresponsavel";
		campos[2] 			= "tarprioridade";
		campos[3] 			= "tardatainicio";
		campos[4] 			= "tardataprazoatendimento";
		campos[5] 			= "sitid";
		campos[6] 			= "acodsc";
		
		tiposDeCampos[0] 	= "select";
		tiposDeCampos[1] 	= "select";
		tiposDeCampos[2] 	= "radio";
		tiposDeCampos[3] 	= "data";
		tiposDeCampos[4] 	= "data";
		tiposDeCampos[5] 	= "select";
		tiposDeCampos[6] 	= "textarea";
		
		if(document.formulario.usucpfresponsavel.disabled == true){
			if(document.formulario.usucpfresponsavel.value == document.formulario.usucpfresponsavelAnterior.value){
				if(document.formulario.usucpfresponsavel.value == ''){
					document.formulario.boGravaNullCpfRespon.value = 1;
				} else {
					document.formulario.boGravaNullCpfRespon.value = '';
				}
			} else {
				document.formulario.usucpfresponsavel.value = '';
			}
			
		}
		//return false;
		validaForm(nomeform, campos, tiposDeCampos, submeterForm )
	}
	
	function excluirTarefaAtividade(tarid){
		if(confirm('Deseja excluir este registro')){
			window.location.href = "/tarefa/tarefa.php?modulo=principal/cadAcompanhamento&acao=A&taridExcluir="+tarid;
			return true;
		} else {
			return false;
		}
	}
	
	function carregaCabecalhoAtendimento(tarid, celula){
		$('tarid').value = tarid;
		$('aguarde_').show();  
		
		/***
		* SE ESTIVER NA TELA DO DADOS DO ATENDIMENTO CARREGA BLOCO DE ATENDIMENTO
		*/
		if($('boRestricao') == null){
			var data = 'tipo=carregaCabecalho&tarid='+tarid;
			new Ajax.Request('ajax.php',
			{  
				method: 'post',   
				parameters: data,
				asynchronous: false,
				onComplete: function(r)
				{
					$('divCabecalho').update(r.responseText);
				}
			});
			
			var data = 'tipo=carregaBlocoAtendimento&tarid='+tarid;
			new Ajax.Request('ajax.php',
			{  
				method: 'post',   
				parameters: data,
				asynchronous: false,   
				onComplete: function(r)
				{
					$('divBlocoAtendimento').update(r.responseText);
					var tabela = $('tabela_tarefa');
					for(i=1; i<tabela.rows.length; i++){
						if(i % 2){
							tabela.rows[i].style.backgroundColor = '#fafafa';
						} else {
							tabela.rows[i].style.backgroundColor = '#f0f0f0';
						}
					}
					celula.style.background = '#ffffcc';
				}
			});
	
			var data = 'tipo=carregaListaAtendimento&tarid='+tarid;
			new Ajax.Request('ajax.php',
			{  
				method: 'post',   
				parameters: data,  
				asynchronous: false, 
				onComplete: function(r)
				{
					$('divListaAtendimento').update(r.responseText);
				}
			});
		
		} else { // SENAO CARREGA DADOS E LISTA DA RESTRICAO
			var data = 'tipo=abaRestricao&tarid='+tarid+'&boNaoMostraTitulo=false';
			new Ajax.Request('ajax.php',
			{  
				method: 'post',   
				parameters: data,
				asynchronous: false,   
				onComplete: function(r)
				{
					$('divRestricao').update(r.responseText);
					var tabela = $('tabela_tarefa');
					for(i=1; i<tabela.rows.length; i++){
						if(i % 2){
							tabela.rows[i].style.backgroundColor = '#fafafa';
						} else {
							tabela.rows[i].style.backgroundColor = '#f0f0f0';
						}
					}
					celula.style.background = '#ffffcc';
				}
			});
		}
		
		$('aguarde_').hide();
		return false;
	}
	
	function gravarRestricao(resid, _tartarefa){
		var residTemp  = ''; 
		var ressolucao = 'f';
		if(resid){
			residTemp = '_'+resid;
			var ressolucaoCheck = document.getElementsByName('ressolucao'+residTemp);
			for (var j = 0; j < ressolucaoCheck.length; j++) {
				if(ressolucaoCheck[j].checked == true){
					ressolucao = ressolucaoCheck[j].value;
				}
			}
		}
		
		var resdescricao = $('resdescricao'+residTemp).value;
		var resmedida    = $('resmedida'+residTemp).value;
		var tarid        = $('tarid').value
		var data 		 = 'tipo=salvarRestricao&resid='+resid+'&resdescricao='+resdescricao+'&resmedida='+resmedida+'&tarid='+tarid+'&ressolucao='+ressolucao;
		var boGravar = false;
		new Ajax.Request('ajax.php',
		{  
			method: 'post',   
			parameters: data,
			asynchronous: false, 
			onLoading: $('aguarde_').show(),  
			onComplete: function(r)
			{
				$('resdescricao').value = "";
				$('resmedida').value = "";
				$('divListaRestricao').update(r.responseText);
				boGravar = true;
			}
		});
		
		if(boGravar){
			montaArvoreAberta(_tartarefa, '', 1);
			$('aguarde_').hide();
			alert('Dados gravados com sucesso.');
		}
		
		
	}
	
	function excluirRestricao(resid, tarid){
		if(confirm('Deseja excluir este registro')){
			var data = 'tipo=excluirRestricao&tarid='+tarid+'&resid='+resid;
			new Ajax.Request('ajax.php',
			{  
				method: 'post',   
				parameters: data,
				asynchronous: false,
				onLoading: $('aguarde_').show(),
				onComplete: function(r)
				{
					$('divListaRestricao').update(r.responseText);
				}
			});
			$('aguarde_').hide();
			return true;
		} else {
			return false;
		}
	}
	
	function alteraCampoDescricao(resid){
		var idDiv1 = "divDesc1_"+resid;
		var idDiv2 = "divDesc2_"+resid;
		var bntAlterar = "bntAltera_"+resid;
		
		if($(idDiv1).style.display == 'none'){
			$(idDiv1).style.display = "";
			$(idDiv2).style.display = "none";
			$(bntAlterar).disabled = true;
		}
	}
	
		
	jQuery(document).ready(
		function () {
			validacaoInicial();	
		});
	
-->
</script>