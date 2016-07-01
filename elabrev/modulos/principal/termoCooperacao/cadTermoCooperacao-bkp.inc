<?php
 
if($_REQUEST['download'] == 's'){
	require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	unset($_REQUEST['download']);
	$arqid = $_REQUEST['arqid'];
	ob_get_clean();
	$file = new FilesSimec(null,null,'monitora');
	$file->getDownloadArquivo($arqid);
	echo "<script> window.close(); </script>";
	die;
}

if($_REQUEST['requisicao'] == 'excluirAnexo'){
	$arqid = $_POST['arqid'];
	if ( $arqid ){
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$file = new FilesSimec("arquivoprevorcamentaria", array(''), "monitora");		
		$file->setRemoveUpload($arqid);		
		die('1');
	}
	die('0');
}

if($_REQUEST['requisicao'] == 'gravaCoordenacao'){
	$sql = "update monitora.termocooperacao set cooid = {$_REQUEST['cooid']} where tcpid = {$_REQUEST['tcpid']}";
	$db->executar($sql);
	if($db->commit()){
		die('1');
	}
	die('0');
}

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

if($_REQUEST['requisicao'] == 'carregaPlanosTrabalho'){
	
	$sql = "SELECT DISTINCT
				p.ptrid as codigo,
 				ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as descricao
			FROM monitora.ptres p
			JOIN public.unidadegestora u 
				ON u.unicod = p.unicod
			WHERE p.ptrano = '2013' 
			AND p.ptrstatus = 'A'
			AND u.unicod IN ( '26101','26298','26291','26290' )";
	
	$arDados = $db->carregar($sql); 
	
	$q = strtolower($_GET["q"]);
	if ($q === '') return;
	foreach ($arDados as $key=>$value){
		echo "{$value['descricao']}|{$value['codigo']}\n";
	}
	die;
}

$perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);

if($_REQUEST['requisicao'] == 'excluirPrevisao'){
	
	if($_REQUEST['id']){
		$sql = "delete from monitora.previsaoorcamentaria where proid = {$_REQUEST['id']}";
		$db->executar($sql);
		if($db->commit()){
			die('1');
		}
	}
	die('0');
}

require_once '_funcoes_termoCooperacao.php';

if( $_REQUEST['req'] ){
	$_REQUEST['req']($_REQUEST);
	die();
}

$_SESSION['elabrev']['tcpid'] = $_REQUEST['tcpid'] ? $_REQUEST['tcpid'] : $_SESSION['elabrev']['tcpid'];
$_SESSION['elabrev']['termo']['aba'] = $_REQUEST['aba'] ? $_REQUEST['aba'] : $_SESSION['elabrev']['termo']['aba'];

if( $_SESSION['favurl'] != 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A' ){
	if( $_REQUEST['aba'] != '' && $_REQUEST['tcpid'] != '' ){
		echo "<script>
				window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=".$_REQUEST['aba']."';
			  </script>";
	}if( $_REQUEST['aba'] == 'gerapdf' && $_REQUEST['tcpid'] != '' ){
		echo "<script>
				window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=".$_REQUEST['aba']."&requisicao=".$_REQUEST['requisicao']."';
			  </script>";
	}
}

switch ($_SESSION['elabrev']['termo']['aba']){
	case 'proponente':
		$abaAnt 	= 'proponente';
		$abaProx 	= 'concedente';
	break;
	case 'concedente':
		$abaAnt 	= 'proponente';
		$abaProx 	= 'descentralizacao';
	break;
	case 'descentralizacao':
		$abaAnt 	= 'concedente';
		$abaProx 	= 'previsao';
	break;
	case 'previsao':
		$abaAnt 	= 'descentralizacao';
		if(in_array(UO_EQUIPE_TECNICA, $perfis)){
			$abaProx 	= 'anexo';
		}else{			
			$abaProx 	= 'parecertecnico';		
		}
	break;
	case 'cronograma':
		$abaAnt 	= 'previsao';
		$abaProx 	= 'parecertecnico';
	break;
	case 'parecertecnico':
		$abaAnt 	= 'previsao';		
		$abaProx 	= 'anexo';
	break;
	case 'anexo':
		$abaAnt 	= 'parecertecnico';
		$abaProx 	= 'tramite';
	break;
	case 'tramite':
		$abaAnt 	= 'anexo';
		$abaProx 	= 'gerapdf';
	break;
	case 'gerapdf':
		$abaAnt 	= 'tramite';
		$abaProx 	= 'gerapdf';
	break;	
}

if( $_SESSION['elabrev']['ungcod'] ){
	$dados = recuperaDadosUG( $_SESSION['elabrev']['ungcod'] );
}

	#Estado atual do documento - tabela monitora.termocooperacao
	if($_SESSION['elabrev']['tcpid'] != ''){
		$estadoAtual = pegarEstadoAtual( $_SESSION['elabrev']['tcpid'] );
	}
	
	// Pega aba atual
	$abaAtual = $_SESSION['elabrev']['termo']['aba'];
	
	// Começa tudo bloqueado
	$habilita 				= 'N';
	$habilita_Und_G 		= 'N';
	$_SESSION['habilita'] 	= 'N';
	$habilitaSalvar 		= 'N';
	$habilitaInserir 		= 'N';
	$habilita_Und_G 		= 'N';
	$habilita_pro_con 		= 'N';
	$habilita_parecer		= 'N';
	$habilita_cumprimento	= 'N';
	
	$boEmAlteracao = false;
	if($_SESSION['elabrev']['tcpid']){
		$sql = "select h.docid from workflow.historicodocumento h
				inner join monitora.termocooperacao t on t.docid = h.docid
				inner join workflow.acaoestadodoc a on a.aedid = h.aedid
				where t.tcpid = {$_SESSION['elabrev']['tcpid']} and a.esdiddestino = ".ALTERAR_TERMO_COOPERACAO;
		
		$boEmAlteracao = $db->pegaUm($sql);
	}
	
	// Verifica se está em cadastramento ou diligência
	$emCadastramentoCorrecao = false;
	if( ($estadoAtual == EM_CADASTRAMENTO || $estadoAtual == '' || $estadoAtual == EM_DILIGENCIA) && !$boEmAlteracao){
		$emCadastramentoCorrecao = true;
	}
	
	// Verifica se pode cadastrar e editar
	if(	$emCadastramentoCorrecao && in_array(UO_EQUIPE_TECNICA, $perfis) ){		
		$habilita 				= 'S';
		$habilita_Und_G 		= 'S';
		$_SESSION['habilita'] 	= 'S';
		$habilitaSalvar 		= 'S';
		$habilitaInserir 		= 'S';
		$habilita_Und_G 		= 'S';
		$habilita_pro_con 		= 'S';
	}
	
	// Verifica se pode editar parecer tecnico
	if(in_array($abaAtual,array('parecertecnico', 'previsao')) && $estadoAtual == EM_ANALISE_OU_PENDENTE && in_array(PERFIL_COORDENADOR_SEC, $perfis)){
		$habilita 				= 'S';
		$habilita_parecer		= 'S';
		$habilitaSalvar 		= 'S';		
		$_SESSION['habilita'] 	= 'S';
	}
	
	// Alteração de termo 
	if( in_array($abaAtual,array('previsao')) && ( ( $estadoAtual == ALTERAR_TERMO_COOPERACAO && in_array(UO_EQUIPE_TECNICA, $perfis)) || ( $boEmAlteracao && $estadoAtual == EM_DILIGENCIA ) ) ){
		$habilita 				= 'S';		
		$_SESSION['habilita'] 	= 'S';
		$habilitaSalvar 		= 'S';
		$habilitaInserir 		= 'S';		
	}
	
	// Habilita salvar na aba previsao orçamentaria
	if(in_array($abaAtual, array('previsao')) && in_array(PERFIL_COORDENADOR_SEC, $perfis)){
		$habilita 				= 'S';
		$habilitaSalvar 		= 'S';
		$_SESSION['habilita'] 	= 'S';		
	}
	
	// Habilita salvar o relatorio de cumprimento
	if(in_array($abaAtual, array('cumprimento')) && ( ( in_array(UO_EQUIPE_TECNICA, $perfis)  && $estadoAtual != RELATORIO_OBJ_AGUARDANDO_APROV_GESTOR)  || in_array(PERFIL_SUPER_USUARIO, $perfis) ) ){
		$habilita 				= 'S';
		$habilitaSalvar 		= 'S';
		$_SESSION['habilita'] 	= 'S';
		$habilita_cumprimento	= 'S';
		
	}
	
	// Verifica se está em cadastramento ou diligência
	if( $estadoAtual == EM_ANALISE_OU_PENDENTE && in_array($_SESSION['elabrev']['termo']['aba'], array('previsao', 'parecertecnico')) ){		
		if( testaRespUGConcedente() ){
			$habilita 				= 'S';
			$habilita_Und_G 		= 'S';
			$habilitaSalvar 		= 'S';
			$habilitaInserir 		= 'S';
			$habilita_Und_G 		= 'S';
			$habilita_pro_con 		= 'S';
			$habilita_parecer		= 'S';			
		}
	}
	
	// Se for super usuário libera tudo
	if(in_array(PERFIL_SUPER_USUARIO, $perfis)){
		$habilita 				= 'S';
		$habilita_Und_G 		= 'S';
		$_SESSION['habilita'] 	= 'S';
		$habilitaSalvar 		= 'S';
		$habilitaInserir 		= 'S';
		$habilita_Und_G 		= 'S';
		$habilita_pro_con 		= 'S';
		$habilita_parecer		= 'S';
	}
	
	$habilita = $habilita ? $habilita : 'N';

require_once APPRAIZ . 'includes/cabecalho.inc';

//Constantes
define("OBJETIVO_HINT", "Favor Preencher com a descrição do objeto a ser executado, indicando, inclusive, o campus em que se localizará o objeto. O objeto é o que deve ser fisicamente entregue à sociedade ao final da execução do Plano de Trabalho.");
define("JUSTIFICATIVA_HINT", "Favor registrar: Contextualização da obra no campus em que o projeto será executado; Motivação da obra, isto é, qual o problema que a obra busca sanar e qual a demanda para o projeto. Caso a proposta tenha recursos a serem descentralizados em mais de um exercício, o proponente deverá inserir no campo da justificativa o comentário de como o recurso deverá ser distribuído ao longo dos exercícios. Ex.: A construção em questão deverá ter aporte de recursos distribuídos em mais de um exercício. Sendo a parcela para 2013 de R$ XX, para 2014 de R$ YY e para 2015 de R$ ZZ.");
?>
<style>
.botao{
	cursor:pointer;
	text-decoration:underline;
}
.botao:hover{
	background-color:#ffffff;
	font-weight:bold;
	text-decoration:none;
}
.fechar_div_cadastro{
	float: top;
	float: right;
	border: none;
	width: 20px;
	cursor: pointer;
}
.div_cadastro{
	width:100%;
	display:none;
}
.formFundo{
	margin-top:-60px;
	margin-left:-10px;
	position:absolute;
	width:100%;
	height:100%;
	background-color:black;
	opacity:0.65;
	-moz-opacity: 0.65;
	filter: alpha(opacity=65);
	z-index:30;
}
.formConteudo{
	background-color:white;
	position:absolute;
	margin-left:25%;
	width:50%;
	height:60%;
	z-index:30;
}
</style>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script type="text/javascript">

function salvaDiretoria(valor){
	if (valor != '')
		window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=tramite&dircodSalvar='+valor;
}

function atualizaComboMunicipio( estuf ){
	$.ajax({
		type: "POST",
		url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
		data: "req=atualizaComboMunicipio&estuf="+estuf,
		async: false,
		success: function(msg){
			jQuery('#td_muncod').html(msg);
		}
	});
	return true;
}

function atualizaUG( ungcod ){
	$.ajax({
		type: "POST",
		url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
		data: "req=dadosUGAjax&ungcod="+ungcod,
		dataType: 'json',
		async: false,
		success: function(msg){	
			$('#divgescod').html(msg.gescod);		
			$('#ungendereco').val(msg.ungendereco);
			$('#ungbairro').val(msg.ungbairro);
			$('#estuf').val(msg.estuf);
			atualizaComboMunicipio( msg.estuf );
			$('#muncod').val(msg.muncod);
			$('#ungcep').val(msg.ungcep);
			$('#ungfone').val(msg.ungfone);
			$('#ungemail').val(msg.ungemail);
			atualizaSecretariaResponsavel( ungcod );
			if(ungcod == '153173'){
				$('#tr_ungpolitica').show();
			}
			else{
				$('#ungcodpoliticafnde').val('');
				$('#tr_ungpolitica').hide();
			}
		}
	});
	return true;
}

function atualizaSecretariaResponsavel(ungcod){
	
	$('[name=ungendereco], [name=ungbairro], [name=estuf], [name=muncod], [name=ungcep], [name=ungfone], [name=ungemail]').attr('disabled', false);
	
	$.ajax({
		type: "POST",
		url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
		data: "req=litaResponsavelUgConc&ungcod="+ungcod,
		async: false,
		success: function(msg){
			jQuery('#responsaveis').html(msg);
			var usucpf = $('#usucpf').val();
			if(typeof usucpf != "undefined"){
				$('#volta').show();
				$('#salva').show();
				$('#continua').show();
			}else if(typeof usucpf == "undefined"){
				$('#volta').hide();
				$('#salva').hide();
				$('#continua').hide();
			}
			$('[name=ungendereco], [name=ungbairro], [name=estuf], [name=muncod], [name=ungcep], [name=ungfone], [name=ungemail]').attr('disabled', true);
		}
	});
	return true;
}

function validarCampos(){
	var erro = 0;
	
	$.each($("input[type=text], select, textarea"), function(i,v){
		if( ( $(v).val() == '' && $(v).attr('id') != 'pliid' && $(v).attr('id') != 'tcpobsrelatorio' && $(v).attr('id') != 'dircodpoliticafnde' ) ){
			erro = 1;
		}
	});
	
	if($('#ungcod').val() == '153173' && $('#dircodpoliticafnde').val() == ''){
		erro = 1;
	}
	
	var abaAtual = '<?=$_SESSION['elabrev']['termo']['aba']?>';
	if(abaAtual == 'cronograma'){
		var valor = 0;
		$.each($('input[type=text]'), function(i,v){
			if( ( $(v).attr('id') == 'crdvalor' ) ){
				valor = valor + parseFloat($(v).val().replace(',00', '').replace('.',''));
			}
		});
		$.ajax({
			type	: "POST",
			url		: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
			data	: "req=validarValorCronograma",
			dataType: 'json',
			async	: false,
			success: function(msg){
				var valorPrev = parseFloat(msg.valor_prev);
				var valorCron = valor; //parseFloat(msg.valor_crono);
				if(valorCron > valorPrev){
					alert('O valor total Cronograma de Desembo R$ '+mascaraglobal('[.###],##',valorCron.toFixed(2))+' é maior que o total da Previsão Orçamentaria R$ '+mascaraglobal('[.###],##',valorPrev.toFixed(2)));
					erro = 2;
				}
			}
		}); 
	}
	

	if(erro == 1){
		alert('Existem campos em branco no formulário, todos os campos devem ser preenchidos!');
		validacao = false;
	}else if(erro == 2) {
		validacao = false;
	}else{
		validacao = true;
	}
	return validacao;
}

$(document).ready(function(){
	$('.baixar').click(function(){
		var arqid = $(this).attr('id');
		return windowOpen( 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&download=s&arqid='+arqid,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	});
	
	$('[name="ptrid[]"]').live('change',function(){		
		linha = $(this).parent().attr('id');
		linha = linha.split('_');
		var ptrid = $(this).val();	
		$.ajax({
			type: "POST",
			url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
			data: "req=atualizaNomeAcao&ptrid="+ptrid,
			async: false,
			success: function(msg){
				jQuery('#td_acao_'+linha[2]).html(msg);
			}
		});
		$.ajax({
			type: "POST",
			url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
			data: "req=atualizaPI&ptrid="+ptrid,
			async: false,
			success: function(msg){
				jQuery('#td_pi_'+linha[2]).html(msg);
			}
		});		
		$.ajax({
			type: "POST",
			url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
			data: "req=atualizaDescAcao&ptrid="+ptrid,
			async: false,
			success: function(msg){				
				jQuery('#td_acaodsc_'+linha[2]).html(msg);
			}
		});
		return true;
	});
	
	$('.excluirLinha').live('click',function(){
		var id = $(this).attr('id');		
		$.ajax({
			url		: 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=parecertecnico',
			type	: 'post',
			data	: 'requisicao=excluirAnexo&arqid='+id,
			success	: function(e){
				if(e=='1'){
					$('#tr_'+id).remove();
					alert('Operação realizada com sucesso.');					
				}else{
					alert('Falha ao tentar excluir o anexo!');
				}
			}
		});
	});

	$('.excluirPrevisao').live('click',function(){

		if(confirm('Deseja deletar este registro?')){
			var id = $(this).attr('id');		
			$.ajax({
				url		: 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=previsao',
				type	: 'post',
				data	: 'requisicao=excluirPrevisao&id='+id,
				success	: function(e){
					if(e=='1'){
						alert('Operação realizada com sucesso.');
						$('#tr_'+id).remove();
					}else{
						alert('Falha ao tentar excluir o anexo!');
					}
				}
			});
		}
	});
	
	$('.inserirCro').click(function(){
		var tamanho = $('#cronograma tr:last').attr('id').replace('tr_','');
		if( tamanho == 'titulo' ){
			tamanho = 0;
		}
		tamanho = parseInt(tamanho)+1;
		$.ajax({
			type: "POST",
			url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
			data: "req=novaLinhaCronograma&cod="+tamanho,
			async: false,
			success: function(msg){
					$('#cronograma tr:last').after(msg);
			}
		});
	});
	
	$('.inserirAnex').click(function(){
		$('.voltar, .salvar, .continuar').show();
		$('.navegar').hide();
		var tamanho = $('#anexos tr:last').attr('id').replace('tr_','');
		if( tamanho == 'titulo' ){
			tamanho = 0;
		}
		tamanho = parseInt(tamanho)+1;
		$.ajax({
			type: "POST",
			url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
			data: "req=novaLinhaAnexo&cod="+tamanho,
			async: false,
			success: function(msg){
					$('#anexos tr:last').after(msg);
			}
		});
	});
	
	$('.inserirAnex2').click(function(){
		$('.voltar, .salvar, .continuar').show();
		$('.navegar').hide();
		var tamanho = $('#anexos tr:last').attr('id').replace('tr_','');
		if( tamanho == 'titulo' ){
			tamanho = 0;
		}
		tamanho = parseInt(tamanho)+1;
		$.ajax({
			type: "POST",
			url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
			data: "req=novaLinhaAnexo2&cod="+tamanho,
			async: false,
			success: function(msg){
					$('#anexos tr:last').after(msg);
			}
		});
	});
	
	$('.inserirPrev').click(function(){
		var tamanho = $('#previsao tr:last').attr('id').replace('tr_','');
		
		if( tamanho == 'titulo' ){
			tamanho = 0;
		}
		
		tamanho = parseInt(tamanho)+1;

		$.ajax({
			type: "POST",
			url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
			data: "req=novaLinhaPrevisao&cod="+tamanho,
			async: false,
			success: function(msg){
				
					$('#previsao tr:last').after(msg);

					/*
					$.each($("[name=crdmesliberacao[]]"), function(i, v){
						if(i==0){
							valor = $(v).val();
						}else{
							$(v).val(valor).attr('disabled', true);
						}
					});
					*/	

					$.each($("[name=crdmesexecucao[]]"), function(i, v){
						if(i==0){
							valor = $(v).val();
						}else{
							$(v).val(valor).attr('disabled', true);
						}
					});	
					
			}
		});
	});
	
	$('.inserirResp').click(function(){
		return windowOpen( '?modulo=principal/termoCooperacao/inserir_representante&acao=A&busca=entnumcpfcnpj','blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	});
	
	$('.excluir').live('click',function(){
		if ($(this).attr('id') == $('#rdProponente:checked').val() ){
			alert('Não é possível excluir o proponente atual');
		}else{
			if (confirm('Deseja realmente desvincular o representante da unidade gestora ?')){
				//apaga o removido da lista de entidades
				var valores =  $('#entidades').val();
				valores = valores.replace($(this).attr('id'), "");
				valores =valores.replace(",,",",");
				$('#entidades').val(valores);

				$('#linha_'+$(this).attr('id')).remove();
				$('#tb_inserirResp').show();

			}
		}
	});
	
	$('.alterar').live('click',function(){
		var index = 'linha_'+entid;
		var entid = $(this).attr('id');
		return windowOpen('?modulo=principal/termoCooperacao/inserir_representante&acao=A&busca=entnumcpfcnpj&entid='+entid+'&tr='+index,'blank','height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
	});

	$('.voltar').click(function(){
		$('#acaoAba').val('<?=$abaAnt ?>');
		$('#req').val('salvarTermo');
		var valida = validarCampos();
		if(valida){
			$('#formulario').submit();
			return false;
		}
	});
	
	$('.continuar').click(function(){
		
		$('#acaoAba').val('<?=$abaProx ?>');
		$('#req').val('salvarTermo');
				
		var valida = validarCampos();
		
		if(valida){
			$('#formulario').submit();
		}
	});
	
	$('.salvar').click(function(){
		
		$('#req').val('salvarTermo');
		
		var valida = validarCampos();
		
		if($('[name=crdmesliberacao[]]') != 'undefined'){
			$('[name=crdmesliberacao[]]').attr('disabled', false);
		}
		if($('[name=crdmesexecucao[]]') != 'undefined'){
			$('[name=crdmesexecucao[]]').attr('disabled', false);
		}				
		
		if(valida){
			$('#formulario').submit();
		}
	});

	$('.gerarPDF').click(function(){
		$('#requisicao').val('gerarPDF');		
		$('#formulario').submit();
	});
	
	/*
	$('[name=crdmesliberacao[]]').change(function(){
		valor = this.value;
		$.each($('[name=crdmesliberacao[]]'), function(i,v){
			if(i>0){
				$(v).attr('disabled', false).val(valor).attr('disabled', true);
			}
		});
	});

	$('[name=crdmesliberacao[]]').live('change',function(){
		valor = this.value;
		$.each($('[name=crdmesliberacao[]]'), function(i,v){
			if(i>0){
				$(v).attr('disabled', false).val(valor).attr('disabled', true);
			}
		});
	});
	*/

	$('[name=crdmesexecucao[]]').change(function(){
		valor = this.value;
		$.each($('[name=crdmesexecucao[]]'), function(i,v){	
			if(i>0){
				$(v).attr('disabled', false).val(valor).attr('disabled', true);
			}
		});
	});

	$('[name=crdmesexecucao[]]').live('change',function(){
		valor = this.value;
		$.each($('[name=crdmesexecucao[]]'), function(i,v){	
			if(i>0){
				$(v).attr('disabled', false).val(valor).attr('disabled', true);
			}
		});
	});

	$('.navegar').click(function(){
		var aba;
		if(this.id == 'proximaAba'){
			aba = '<?php echo $abaProx ?>';
		}else{			
			aba = '<?php echo $abaAnt ?>';
		}
		document.location.href = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba='+aba;
	});

});

function salvaCoordenacao(id){
	tcpid = $('[name=tcpid]').val();
	$.ajax({
		url		: 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A',
		type	: 'post',
		data	: 'requisicao=gravaCoordenacao&cooid='+id+'&tcpid='+tcpid,
		success	: function(e){
			if(e == '1'){
				alert('Operação realizada com sucesso.');
				document.location.reload();
			}else{
				alert('Falha ao gravar a coordenação!');
			}
		}
	});	
}
</script>
<?php

	echo "<br>";
	$db->cria_aba( $abacod_tela, $url, '' );	
	monta_titulo( "Termo de Execução Descentralizado", '' );
	montaCabecalhoUG(Array('termo'=>true));	
	echo "<br>";	
	montaAbaTermoCooperacao();
	
	$aba = $_SESSION['elabrev']['termo']['aba'];
	
	if( $_SESSION['elabrev']['termo']['aba'] == 'proponente' || $_SESSION['elabrev']['termo']['aba'] == 'concedente' ){

		if($_SESSION['elabrev']['tcpid']){
			
			$sqlA = "SELECT	
						ungcodconcedente,
						ungcodproponente,
						unridconcedente,
						ungcodpoliticafnde,
						dircodpoliticafnde
					FROM 
						monitora.termocooperacao
					WHERE 
						tcpid = ".$_SESSION['elabrev']['tcpid'];
			
			$dadosTermo = $db->pegaLinha($sqlA);
		}
?>

<form method="post" name="formulario" id="formulario">
	<input type="hidden" id="tcpid" name="tcpid" value="<?=$_SESSION['elabrev']['tcpid'] ?>" />
	<input type="hidden" id="req" name="req" value="" />
	<input type="hidden" id="requisicao" name="requisicao" value="" />
	<input type="hidden" id="aba" name="aba" value="<?=$_REQUEST['aba'] ?>" />
	<input type="hidden" id="acaoAba" name="acaoAba" value="<?=$_REQUEST['aba'] ?>" />
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" valign="bottom">
				<center><b>Unidade Gestora <?=($aba =='concedente' ? 'Concedente' : 'Proponente')?></b></center>
			</td>
		</tr>
	</table>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<?php if( $_REQUEST['aba'] != 'proponente' ){ ?>
		<tr>
			<td class="SubTituloDireita" width="15%">Código Unidade Gestora</td>
			<td>
			<?php 
								
				$dados = recuperaDadosUG( $dadosTermo['ungcodconcedente'] );
				
				$sqlB = "
					select	gun.ungcodresponsavel
					FROM public.unidadegestora ung
					inner join public.unidade uni on ung.unicod = uni.unicod
					inner join public.grupounidade gun on gun.gunid = uni.gunid
					WHERE ung.ungcod = '{$dados['ungcod']}'
				";
				$registro = $db->pegaUm($sqlB);
				
				if($dados){
					$ungcod = $dados['ungcod'];
				}
				
				$where = "";
				
				if($registro != ''){
					$where = "and ungcod = '{$registro}'"; 
				}
				
				$sql = "
					SELECT	ungcod as codigo,
							ungabrev||' / '||ungdsc as descricao
					FROM public.unidadegestora
					WHERE ungstatus = 'A' 
					".$where."
					and ungcod in ('150001','150003','150004','150019','150016','150011','150028','152390','152389','153173','154003','153978')
					ORDER BY 2
				";
				
				$db->monta_combo('ungcod',$sql, $habilita_Und_G, $registro != '' ? '' :  'Selecione...','atualizaUG',$opc,'','445','S', 'ungcod', '', $dadosTermo['ungcodconcedente'], $title= null);
			?>
			</td>
		</tr>
		<tr id="tr_ungpolitica" style="display: <?if($ungcod != '153173'){echo 'none';}?>">
			<td class="SubTituloDireita" width="15%">Responsável pela Política</td>
			<td>
			<?php 
// 				$sql = "
// 					SELECT	ungcod as codigo,
// 							ungabrev||' / '||ungdsc as descricao
// 					FROM public.unidadegestora
// 					WHERE ungstatus = 'A' 
// 					and ungcod in ('150001','150003','150019','150016','150011','150028','152390','152389','152734','153173','154003','153978')
// 					ORDER BY 2";

				$sql = "SELECT 
							dircod as codigo, 
							'('||ug.ungabrev||') / ' || dirdsc as descricao
						FROM public.unidadegestora ug
							inner join elabrev.diretoria d ON d.ungcod = ug.ungcod
						WHERE ungstatus='A' and dirstatus = 'A'
						AND d.dircod IN (38,39,41,42,43)
						ORDER BY 2";
				
				$db->monta_combo('dircodpoliticafnde',$sql, 'S', 'Selecione...','',$opc,'','445','S', 'dircodpoliticafnde', '', $dadosTermo['dircodpoliticafnde'], $title= null);
			?>
			</td>
		</tr>
		
	<?php }else{ ?>
	
		<?php if($dadosTermo['ungcodproponente']) $dados = recuperaDadosUG( $dadosTermo['ungcodproponente'] ); ?>
		<?php $ungcod = $dados['ungcod']; ?>
		
		<tr>
			<td class="SubTituloDireita" width="15%">Código Unidade Gestora</td>
			<td><?=$dados['ungcod'] ?><input type="hidden" name="ungcod" value="<?=$dados['ungcod'] ?>"/></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Razão Social</td>
			<td><?=$dados['ungdsc'] ?></td>
		</tr>
	<?php }?>
	
		<?php
		if($_SESSION['elabrev']['termo']['aba'] == 'concedente'){
			$habilita_pro_con = 'N';
		}
		?>
	
		<tr>
			<td class="SubTituloDireita" width="15%">Código da Gestão</td>
			<td><div id="divgescod"><?=$dados['gescod'] ?></div><?//=$dados['unicod'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Endereço</td>
			<td><?=campo_texto('ungendereco','S',$habilita_pro_con,'',70,100,'','', '', '', '', 'id="ungendereco"', '', $dados['ungendereco']);?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Bairro</td>
			<td><?=campo_texto('ungbairro','S',$habilita_pro_con,'',70,100,'','', '', '', '', 'id="ungbairro"', '', $dados['ungbairro']);?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">UF</td>
			<td>
			<?php 
			$sql = "SELECT
						estuf as codigo,
						estdescricao as descricao
					FROM
						territorios.estado
					ORDER BY
						2";
			$db->monta_combo('estuf',$sql, $habilita_pro_con,'Selecione...','atualizaComboMunicipio','','',200,'S', 'estuf', '', $dados['estuf'], '');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Município</td>
			<td id="td_muncod">
			<?php 
			if( $dados['estuf'] != '' ){
				$sql = "
					SELECT 	muncod as codigo,
							mundescricao as descricao
					FROM territorios.municipio
					WHERE estuf ilike '".$dados['estuf']."'
					ORDER BY 2
				";
			}else{
				$sql = "SELECT
							'' as codigo,
							'Selecione uma UF' as descricao";
			}
			$db->monta_combo('muncod',$sql, $habilita_pro_con,'Selecione...','','','',200,'S', 'estuf', '', $dados['muncod'], '');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">CEP</td>
			<td >
				<?=campo_texto('ungcep','S',$habilita_pro_con,'',15,10,'##.###-###','', '', '', '', 'id="ungcep"', '', $dados['ungcep']);?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Telefone</td>
			<td >
				<?=campo_texto('ungfone','S',$habilita_pro_con,'',15,14,'##-####-####','', '', '', '', 'id="ungfone"', '', $dados['ungfone']);?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">E-mail</td>
			<td >
				<?=campo_texto('ungemail','S',$habilita_pro_con,'',70,100,'','', '', '', '', 'id="ungemail"', '', $dados['ungemail']);?>
			</td>
		</tr>
	</table>
	<? 
		$repProp = buscarRepreProponente($ungcod);
		if($repProp['rpuid'] == '' && $_REQUEST['aba'] == 'proponente'){
			$habilitaSalvar = 'N';
		}elseif($repProp['rpuid'] != '' && $_REQUEST['aba'] == 'proponente'){
			$habilitaSalvar = 'S';
		}
		
		//$repProp['rpuid'] != '' &&
		if( $_REQUEST['aba'] == 'proponente' ){
	?>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloCentro" style="background-color: #CFCFCF;">Representante Legal</td>
		</tr>
	</table>
	<table id="responsaveis" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">	
		<?php
				
        	if( $ungcod == '' ){
        		
        		if(!empty($_SESSION['elabrev']['unicod'])){	
        			
	        		$sql = "SELECT
								ungcod
							FROM
								public.unidadegestora u
							WHERE 
								".(is_array($_SESSION['elabrev']['unicod']) ? "u.unicod IN ('".implode("','", $_SESSION['elabrev']['unicod'])."')" : "u.unicod = '".$_SESSION['elabrev']['unicod']."'");
	        		        		
	        		$ungcod = $db->carregarColuna($sql);
        		}
        	}	
			litaResponsavelUgProp($ungcod);
		?>
	</table>
	<?php }elseif( $repProp['rpuid'] == '' && $_REQUEST['aba'] == 'proponente' ){ ?>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloCentro" style="background-color: #CFCFCF;">Representante Legal</td>
		</tr>
	</table>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloCentro" style="color:red;">
				Não há Representante Legal vinculado. <br> É necessário que haja um representante vinculado para dar continuidade ao cadastramento do termo.
			</td>
		</tr>
	</table>
	<?php }
	
		$repConc = buscarRepreConcedente($ungcod); 

		if( $repConc['usucpfconcedente'] != '' && $_REQUEST['aba'] == 'concedente' ){ ?>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloCentro" style="background-color: #CFCFCF;">Representante Legal</td>
		</tr>
	</table>
	<table id="responsaveis" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">	
		<?php
			litaResponsavelUgConc($ungcod);
		?>
	</table>
	<?php }elseif( $repConc['usucpfconcedente'] == '' && $_REQUEST['aba'] == 'concedente' ){ ?>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloCentro" style="background-color: #CFCFCF;">Representante Legal</td>
		</tr>
	</table>	
	<table id="responsaveis" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
		<tr>
			<td class="SubTituloCentro" style="color:red;">Selecione um Representante Legal definindo uma unidade gestora.</td>
		</tr>
	</table>
	<!-- 
				<center><b>Não há Representante Legal vinculado. <br> É necessário que haja um representante vinculado para dar continuidade ao cadastramento do termo.</b></center>
				-->	
	<?
	}
}elseif( $_SESSION['elabrev']['termo']['aba'] == 'descentralizacao' ){
	
	verificaCodigoTermoSessao();
	
	$sql = "SELECT  ungcodproponente,
					ungcodconcedente,
					tcpdscobjetoidentificacao,
					tcpobjetivoobjeto,
					tcpobjetojustificativa,
					ungcodgestaorecebedora as ungcodrec
			FROM monitora.termocooperacao
			WHERE tcpid = ".$_SESSION['elabrev']['tcpid'];
	
	$dadoObjetivo = $db->pegaLinha($sql);
?>
<form method="post" name="formulario" id="formulario">

	<input type="hidden" id="tcpid" name="tcpid" value="<?=$_SESSION['elabrev']['tcpid'] ?>" />
	<input type="hidden" id="req" name="req" value="" />
	<input type="hidden" id="aba" name="aba" value="<?=$_REQUEST['aba'] ?>" />
	<input type="hidden" id="acaoAba" name="acaoAba" value="<?=$_REQUEST['aba'] ?>" />
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" valign="bottom">
				<center><b>Objeto e Justificativa da Descentralização do Crédito</b></center>
			</td>
		</tr>
	</table>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="15%">Identificação (Título / Objeto da despesa):</td>
			<td>
				<?=campo_textarea( 'tcpdscobjetoidentificacao', 'S', $habilita, '', 150, 3, 70, '', '', '', '', '', $dadoObjetivo['tcpdscobjetoidentificacao']) ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Objetivo: </td>
			<td>
				<table>
					<tr>
						<td>
							<?=campo_textarea( 'tcpobjetivoobjeto', 'S', $habilita, '', 150, 5, 490, '', '', OBJETIVO_HINT, '', '', $dadoObjetivo['tcpobjetivoobjeto']) ?>							
						</td>
						<td>
							<img src="../imagens/info.gif" height="15" alt="<?php echo OBJETIVO_HINT ?>" title="<?php echo OBJETIVO_HINT ?>" border="0" />
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">UG/Gestão Repassadora: </td>
			<td>
			<?php
				if($dadoObjetivo['ungcodconcedente']){
					$sql = "SELECT
								ungcod as codigo,
								ungcod||' / '||ungdsc as descricao
							FROM
								public.unidadegestora
							WHERE
								ungstatus = 'A' and ungcod = '".$dadoObjetivo['ungcodconcedente']."'
							ORDER BY
							2";
					//$db->monta_combo('ungcodrec', $sql, 'S', '', 'atualizaUG', $opc, '', '445', 'N', 'ungcodrec', '', $dados['ungcodrec'], $title= null);
					$dado = $db->pegaLinha($sql);
					echo $dado['descricao'];
				}
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">UG/Gestão Recebedora: </td>
			<td>
			<?php 
				if($dadoObjetivo['ungcodproponente']){
					$sql = "
						SELECT	ungcod as codigo,
								ungcod||' / '||ungdsc as descricao
						FROM public.unidadegestora
						WHERE ungstatus = 'A' and ungcod = '".$dadoObjetivo['ungcodproponente']."'
						ORDER BY 2
					";
					//$db->monta_combo('ungcoddes', $sql, 'S', '', 'atualizaUG', $opc, '', '445', 'N', 'ungcoddes', '', $dados['ungcoddes'], $title= null);
					$dados = $db->pegaLinha($sql);
					echo $dados['descricao'];
				}
			?>
			
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">
				Justificativa (Motivação / Clientela / Cronograma físico):
			</td>
			<td>
				<table>
					<tr>
						<td>
							<?=campo_textarea( 'tcpobjetojustificativa', 'S', $habilita, '', 150, 4, 350, '', '', JUSTIFICATIVA_HINT, '', '', $dadoObjetivo['tcpobjetojustificativa']);?>
						</td>
						<td>
							<img src="../imagens/info.gif" height="15" alt="<?php echo JUSTIFICATIVA_HINT ?>" title="<?php echo JUSTIFICATIVA_HINT ?>" border="0" />
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Relações entre as Partes</td>
			<td>
				<p>
				I - Integra este termo, independentemente de transcrição, o Plano de Trabalho e o Termo de Referência, cujos dados ali contidos acatam os partícipes e se comprometem em cumprir, sujeitando-se às normas da Lei no 8.666, de 21 de junho de 1993, no que couber, Decreto no  93.872/1986 e o de nº 6.170, de 25 de julho de 2007, Portaria Interministerial no 507, de 24 de novembro de 2011 e Portaria Conjunta MP/MF/CGU nº 8, de 7 de novembro de 2012.
				</p>
				<p>
				II - constituem obrigações da CONCEDENTE:<br>
				a) efetuar a transferência dos recursos financeiros previstos para a execução deste Termo, na forma estabelecida no Cronograma de Desembolso constante do Plano de Trabalho;<br>
				b) orientar, supervisionar e cooperar com a implantação das ações objeto deste Termo;<br>
				c) acompanhar as atividades de execução, avaliando os seus resultados e reflexos;<br>
				d) analisar o relatório de cumprimento do objeto do presente Termo;
				</p>
				<p>
				III - constituem obrigações da PROPONENTE:<br>
				a) promover a execução do objeto do Termo na forma e prazos estabelecidos no Plano de Trabalho;<br>
				b) aplicar os recursos discriminados exclusivamente na consecução do objeto deste Termo;<br>
				c) permitir e facilitar ao Órgão Concedente o acesso a toda documentação, dependências e locais do projeto;<br>
				d) observar e exigir, na apresentação dos serviços, se couber, o cumprimento das normas específicas que regem o Programa;<br>
				e) manter o órgão Concedente informado sobre quaisquer eventos que dificultem ou interrompam o curso normal de execução do Termo;<br>
				f) devolver os saldos dos créditos orçamentários descentralizados e não empenhados, bem como os recursos financeiros não utilizados, conforme norma de encerramento do correspondente exercício financeiro;<br>
				g) a prestação de contas dos créditos descentralizados devem integrar as contas anuais do órgão Proponente a serem apresentadas aos órgãos de controle interno e externo, conforme normas vigentes;<br>
				h) apresentar relatório de cumprimento do objeto pactuado até 60 dias após o término do prazo para cumprimento do objeto estabelecido no Termo.
				</p>
			</td>
		</tr>
	</table>
<?php 
}elseif( $_SESSION['elabrev']['termo']['aba'] == 'previsao' ){
?>
<form method="post" name="formulario" id="formulario">
	<input type="hidden" id="tcpid" name="tcpid" value="<?=$_SESSION['elabrev']['tcpid'] ?>" />
	<input type="hidden" id="req" name="req" value="" />
	<input type="hidden" id="aba" name="aba" value="<?=$_REQUEST['aba'] ?>" />
	<input type="hidden" id="acaoAba" name="acaoAba" value="<?=$_REQUEST['aba'] ?>" />
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" valign="bottom">
				<center><b>Previsão Orçamentária</b></center>
			</td>
		</tr>
	</table>
	
	<table id="previsao" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
		<tr id="tr_titulo">
			<td class="subtitulocentro" >&nbsp;</td>
			<td class="subtitulocentro" >Ano</td>
			<td class="subtitulocentro" >Ação</td>
			<td class="subtitulocentro" >Programa de Trabalho</td>
			<td class="subtitulocentro" >Plano Interno</td>
			<td class="subtitulocentro" >Descrição da Ação Constante da LOA</td>
			<td class="subtitulocentro" >Nat.da Despesa</td>
			<td class="subtitulocentro" >Valor (em R$ 1,00)</td>
			<td class="subtitulocentro" >Mês da Liberação</td>
			<td class="subtitulocentro" >Prazo para o cumprimento do objeto</td>
		</tr>
		<?php listaPrevisao(); ?>
	</table>
	<?php if($habilitaInserir == 'S'):?>
		<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" id="">
			<tr>
				<td colspan="3">
					<a href="#" class="inserirPrev"> 
						<img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Inserir Contatos">
						<?if($estadoAtual == ALTERAR_TERMO_COOPERACAO){?> 
							Inserir Nova Previsão
						<?}else{?>
							Inserir Previsão
						<?}?>
					</a>
				</td>
			</tr>
		</table>
	<?php endif; ?>
	<script>
		$(function(){
			$.each($("[name=crdmesexecucao[]]"), function(i, v){
				if(i==0){
					valor = $(v).val();
				}else{
					$(v).val(valor).attr('disabled', true);
				}
			});
		});
	</script>
<?php 
}elseif( $_SESSION['elabrev']['termo']['aba'] == 'cronograma' ){
?>
<form method="post" name="formulario" id="formulario">

	<input type="hidden" id="tcpid" name="tcpid" value="<?=$_SESSION['elabrev']['tcpid'] ?>" />
	<input type="hidden" id="req" name="req" value="" />
	<input type="hidden" id="aba" name="aba" value="<?=$_REQUEST['aba'] ?>" />
	<input type="hidden" id="acaoAba" name="acaoAba" value="<?=$_REQUEST['aba'] ?>" />
	
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" valign="bottom"><center><b>Cronograma de Desembo</b></center></td>
		</tr>
	</table>
	<table id="cronograma" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="2" align="center" width="95%">
		<tr id="tr_titulo">
			<td class="subtitulocentro" ></td>
			<td class="subtitulocentro" >N° da Parcela</td>
			<td class="subtitulocentro" >Ação</td>
			<td class="subtitulocentro" ></td>
			<td class="subtitulocentro" >Valor</td>
			<td class="subtitulocentro" ></td>
		</tr>
		<?php listaCronograma(); ?>
	</table>
	<?php if($habilitaInserir == 'S'):?>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="2" cellspacing="1" id="">
		<tr>
			<td colspan="2">
				<a href="#" class="inserirCro"> 
					<img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Inserir Contatos"> Inserir Cronograma
				</a>
			</td>
		</tr>
	</table>
<?php 
	endif;

	}elseif( $_SESSION['elabrev']['termo']['aba'] == 'parecertecnico'){
		$sql = "
			select 
				tcpparecertecnico,
				tcpconsidentproponente,
				tcpconsidproposta,
				tcpconsidobjeto,
				tcpconsidobjetivo,
				tcpconsidjustificativa,
				tcpconsidvalores,
				tcpconsidcabiveis
			from 
				monitora.termocooperacao 
			where 
				tcpid = ". $_SESSION['elabrev']['tcpid'];
		 
		$dadoParecer = $db->pegaLinha($sql);
		?>
	<form action="" method="post" enctype="multipart/form-data" id="formulario" name="formulario">
		<input type="hidden" id="tcpid" name="tcpid" value="<?=$_SESSION['elabrev']['tcpid'] ?>" />
		<input type="hidden" id="req" name="req" value="" />
		<input type="hidden" id="aba" name="aba" value="<?=$_REQUEST['aba'] ?>" />
		<input type="hidden" id="acaoAba" name="acaoAba" value="<?=$_REQUEST['aba'] ?>" />
		<input type="hidden" id="arptipo" name="arptipo" value="P">
		
		<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td class="SubTituloCentro" valign="bottom">Parecer técnico</td>
			</tr>
		</table>
		<input type="hidden" name="tcpparecertecnico" id="tcpparecertecnico" value="" />
		<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<!--  
			<tr>
				<td class="SubTituloDireita" width="15%">Parecer Técnico:</td>
				<td>
					<?=campo_textarea( 'tcpparecertecnico', 'S', $habilita_parecer, '', 150, 5, 1000, '', '', '', '', '', $dadoParecer['tcpparecertecnico']);?>
				</td>
			</tr>
			-->			
			<tr>
				<td class="SubTituloDireita" width="15%">Considerações sobre a entidade proponente:</td>
				<td>
					<?=campo_textarea( 'tcpconsidentproponente', 'S', $habilita_parecer, '', 150, 5, 1000, '', '', '', '', '', $dadoParecer['tcpconsidentproponente']);?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="15%">Considerações sobre a proposta:</td>
				<td>
					<?=campo_textarea( 'tcpconsidproposta', 'S', $habilita_parecer, '', 150, 5, 1000, '', '', '', '', '',$dadoParecer['tcpconsidproposta']);?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="15%">Considerações sobre o objeto:</td>
				<td>
					<?=campo_textarea( 'tcpconsidobjeto', 'S', $habilita_parecer, '', 150, 5, 1000, '', '', '', '', '',$dadoParecer['tcpconsidobjeto']);?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="15%">Considerações sobre o objetivo:</td>
				<td>
					<?=campo_textarea( 'tcpconsidobjetivo', 'S', $habilita_parecer, '', 150, 5, 1000, '', '', '', '', '',$dadoParecer['tcpconsidobjetivo']);?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="15%">Considerações sobre a justificativa:</td>
				<td>
					<?=campo_textarea( 'tcpconsidjustificativa', 'S', $habilita_parecer, '', 150, 5, 1000, '', '', '', '', '',$dadoParecer['tcpconsidjustificativa']);?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="15%">Considerações sobre os valores:</td>
				<td>
					<?=campo_textarea( 'tcpconsidvalores', 'S', $habilita_parecer, '', 150, 5, 1000, '', '', '', '', '',$dadoParecer['tcpconsidvalores']);?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="15%">Outras considerações cabíveis:</td>
				<td>
					<?=campo_textarea( 'tcpconsidcabiveis', 'S', $habilita_parecer, '', 150, 5, 1000, '', '', '', '', '',$dadoParecer['tcpconsidcabiveis']); ?>
				</td>
			</tr>
			
		</table>
		
		<table><tr><td>&nbsp;</td></tr></table>
		
		<table id="anexos" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
			<tr>
				<td colspan="4" class="subtitulocentro" style="background-color: #CFCFCF;">Anexos</td>
			</tr>
			<tr id="tr_titulo">
				<td class="subtitulocentro" >Ação</td>
				<td class="subtitulocentro" >Arquivo</td>
				<td class="subtitulocentro" >Descrição</td>
				<td class="subtitulocentro" >Visualizar arquivo</td>			
			</tr>
			<?php listaAnexos('P'); ?>
		</table>
		<?php if($habilitaInserir == 'S'):?>
		<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" id="">
			<tr>
				<td colspan="3">
					<a href="javascript:void(0)" class="inserirAnex"> 
						<img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Inserir Anexo"> Inserir Anexo
					</a>
				</td>
			</tr>
		</table>
		
	<?php
	endif;
}elseif( $_SESSION['elabrev']['termo']['aba'] == 'anexo' ){
?>
<form action="" method="post" enctype="multipart/form-data" id="formulario" name="formulario">	
	<input type="hidden" id="tcpid" 	name="tcpid" 	value="<?=$_SESSION['elabrev']['tcpid'] ?>" />
	<input type="hidden" id="req" 		name="req" 		value="" />
	<input type="hidden" id="aba" 		name="aba" 		value="<?=$_REQUEST['aba'] ?>" />
	<input type="hidden" id="acaoAba" 	name="acaoAba" 	value="<?=$_REQUEST['aba'] ?>" />
	<input type="hidden" id="arptipo" name="arptipo" value="A">
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="arqidSubTituloDireita" valign="bottom"><center><b>Anexos</b></center></td>
		</tr>
	</table>
	<table id="anexos" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
		<tr id="tr_titulo">
			<td class="subtitulocentro" ></td>
			<td class="subtitulocentro" >Arquivo</td>
			<td class="subtitulocentro" >Descrição</td>
			<td class="subtitulocentro" >Visualizar arquivo</td>			
		</tr>
		<?php listaAnexos('A'); ?>
	</table>
	<?php if($habilitaInserir == 'S'):?>
	<script type="text/javascript">
		$(function(){
			//anexoPrincipal
			
			if($('.excluirLinha')){				
				$.each($('.excluirLinha'), function(i,v){
					if(i==0){
						$('[name="arpdsc[]"]').val(1);										
						$('.anexoPrincipal, .voltar, .salvar, .continuar').hide();
						$('.navegar').show();
					}
				});
			}
		});
	</script>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" id="">
		<tr>
				<td colspan="3">
					<a href="#" class="inserirAnex"> 
						<img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Inserir Anexo"> Demais Arquivos
					</a>
				</td>
		</tr>
		<!--  <tr id="tr_<?=$request['cod'] ?>" class="anexoPrincipal">  -->
		<tr id="tr_1" class="anexoPrincipal">
			<td align="center"> Projeto básico ou termo de referência
				<!--  <input type="hidden" name="anexoCod[]" value="<?=$request['cod'] ?>"/>  -->
				<input type="hidden" name="anexoCod[]" value="1"/>
				<!-- <input type="file" name="anexo_<?=$request['cod'] ?>"/> -->
				<input type="file" name="anexo_1"/>
			</td>
			<td align="center">
				<?=campo_textarea( 'arpdsc[]', 'S', 'S', '', 150, 2, 255, '', '', '', '', '', $dados['arpdsc'], array("id"=>"arpdsc")) ?>
			</td>
		</tr>
		
	</table>
<?php 
	endif;
}elseif( $_SESSION['elabrev']['termo']['aba'] == 'tramite' ){

	if ( $_GET['dircodSalvar']){
		$sql = 'update monitora.termocooperacao set dircod ='.$_GET['dircodSalvar']. ' where tcpid = '.$_SESSION['elabrev']['tcpid'] ;
		$db->executar($sql);
		$db->commit();
	}
	
	$sql = "select tcpobsrelatorio from monitora.termocooperacao where tcpid = ". $_SESSION['elabrev']['tcpid'];
	$tcpobsrelatorio = $db->pegaUm($sql);

	?>
	<form method="post" name="formulario" id="formulario" enctype="multipart/form-data">
		<input type="hidden" id="tcpid" name="tcpid" value="<?=$_SESSION['elabrev']['tcpid'] ?>" />
		<input type="hidden" id="req" name="req" value="" />
		<input type="hidden" id="aba" name="aba" value="<?=$_REQUEST['aba'] ?>" />
		<input type="hidden" id="acaoAba" name="acaoAba" value="<?=$_REQUEST['aba'] ?>" />
		<input type="hidden" id="arptipo" name="arptipo" value="T">
		<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td class="SubTituloDireita" valign="bottom"><center><b>Trâmite</b></center></td>
			</tr>
		</table>
		<table id="cronograma" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
			<tr id="tr_titulo">
				<td class="subtitulocentro" >Pendências</td>
			</tr>
			<?php listaPendencias(); ?>
		</table>
		<?php
		if($_SESSION['elabrev']['tcpid'] != ''){
			$estadoAtual = pegarEstadoAtual( $_SESSION['elabrev']['tcpid'] );
			$perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
		}
		
		?>
		
		<?if($estadoAtual == RELATORIO_OBJ_AGUARDANDO_ANALISE_COORD):?>
				<table><tr><td>&nbsp;</td></tr></table>
				
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
					<tr>
						<td colspan="4" class="subtitulocentro" style="background-color: #CFCFCF;">Observação do Parecer Técnico do Termo de Execução Descentralizado</td>
					</tr>
					<tr>
						<td class="subtitulocentro" >
							<?
							 if( in_array(PERFIL_COORDENADOR_SEC, $perfis) ){
							 	$hab = "S";	
							 }else{
							 	$hab = "N";
							 }
								echo campo_textarea("tcpobsrelatorio","N",$hab,"Observação",120,15,5000);
							 ?>
						</td>
					</tr>
				</table>
				
				<table id="anexos" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
					<tr>
						<td colspan="4" class="subtitulocentro" style="background-color: #CFCFCF;">Parecer Técnico do Termo de Execução Descentralizado</td>
					</tr>
					<?php listaAnexos('T'); ?>
				</table>
				
				<?php
				
				$perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
				if( $estadoAtual == RELATORIO_OBJ_AGUARDANDO_ANALISE_COORD ){
					if( in_array(PERFIL_COORDENADOR_SEC, $perfis) ){
						
						?>
						<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" id="">
							<tr>
								<td colspan="3">
									<a href="javascript:void(0)" class="inserirAnex2"> 
										<img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Inserir Anexo"> Inserir Anexo
									</a>
								</td>
							</tr>
						</table>
						<?php
					}
				}else{
					?>
					<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" id="">
						<tr>
							<td colspan="3">
								<a href="javascript:void(0)" class="inserirAnex2"> 
									<img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Inserir Anexo"> Inserir Anexo
								</a>
							</td>
						</tr>
					</table>
					<?php	
				}
				?>
				
				
		<?endif;?>
<?
}elseif( $_SESSION['elabrev']['termo']['aba'] == 'gerapdf'){
	
	$dadosUG = buscarDadosUndProp();

	$html = '
	<form method="post" name="formulario" id="formulario">
		<input type="hidden" id="tcpid" name="tcpid" value="'.$_SESSION['elabrev']['tcpid'].'" />
		<input type="hidden" id="req" name="req" value="" />
		<input type="hidden" id="requisicao" name="requisicao" value="" />
		<input type="hidden" id="aba" name="aba" value="'.$_REQUEST['aba'].'" />
		<input type="hidden" id="acaoAba" name="acaoAba" value="'.$_REQUEST['aba'].'" />

		<!-- DADOS DO ÕRGÃO OU ENTIDADE PROPONENTE -->
			
		<table id="gerapdf" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%">
			<tr id="tr_titulo">
				<td class="subtitulocentro" >Termo de Execução Descentralizado - Gerar PDF</td>
			</tr>
		</table>
		<table id="dados_entidade_prop" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%">
		  <tr>
		    <td height="30" colspan="5" style="background-color: #CFCFCF; text-align:center; font-weight: bold;">DADOS DO ÕRGÃO OU ENTIDADE PROPONENTE</td>
		  </tr>
		  <tr>
		    <td width="180" style="font-weight: bold;">1. Cód. Und. Gestora</td>
		    <td colspan="2" style="font-weight: bold;">2. Cód. da Gestão</td>
		    <td colspan="2" style="font-weight: bold;">3. Razão Social</td>
		  </tr>
		  <tr>
		    <td>'.$dadosUG['ungcod'].'</td>
		    <td colspan="2">'.$dadosUG['gescod'].'</td>
		    <td colspan="2">'.$dadosUG['ungdsc'].'</td>
		  </tr>
		  <tr>
		    <td height="21" colspan="2" style="font-weight: bold;">4. Endereço</td>
		    <td colspan="2" style="font-weight: bold;">5. Bairro ou Distrito</td>
		    <td style="font-weight: bold;">6. Município</td>
		  </tr>
		  <tr>
		    <td colspan="2">'.$dadosUG['ungendereco'].'</td>
		    <td colspan="2">'.$dadosUG['ungbairro'].'</td>
		    <td>'.$dadosUG['muncod'].'</td>
		  </tr>
		  <tr>
		    <td height="21" style="font-weight: bold;">7. UF</td>
		    <td style="font-weight: bold;">8. CEP</td>
		   
		    <td colspan="2" style="font-weight: bold;">9. Telefone</td>
		    <td style="font-weight: bold;">10. E-Mail</td>
		  </tr>
		  <tr>
		    <td>'.$dadosUG['estuf'].'</td>
		    <td>'.$dadosUG['ungcep'].'</td>
		    <td colspan="2">'.$dadosUG['ungfone'].'</td>
		    <td>'.$dadosUG['ungemail'].'</td>
		  </tr>
		</table>';
?>		
<!-- REPRESENTANTE LEGAL DO ÕRGÃO OU ENTIDADE PROPONENTE -->
<?php 
	$dadosProp = buscaResponsavelProp();
		
	$html .= '
		<table id="dados_proponente" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%">
		  <tr>
		    <td colspan="6" style="background-color: #CFCFCF; text-align:center; font-weight: bold;">REPRESENTANTE LEGAL DO ÕRGÃO OU ENTIDADE PROPONENTE</td>
		  </tr>
		  <tr>
		    <td width="214" style="font-weight: bold;">11. CPF </td>
		    <td colspan="5" style="font-weight: bold;">12. Nome do Representante Legal</td>
		  </tr>
		  <tr>
		    <td>'.$dadosProp['usucpf'].'</td>
		    <td colspan="5">'.$dadosProp['usunome'].'</td>
		  </tr>
		  <tr>
		    <td colspan="3" style="font-weight: bold;">13. Endereço</td>
		    <td style="font-weight: bold;">14. Bairro ou Distrito</td>
		    <td colspan="2" style="font-weight: bold;">15. Município</td>
		  </tr>
		  <tr>
		    <td colspan="3">'.$dadosProp['endereco'].'</td>
		    <td>'.$dadosProp['bairro'].'</td>
		    <td colspan="2">'.$dadosProp['municipio'].'</td>
		  </tr>
		  <tr>
		    <td style="font-weight: bold;">16. UF</td>
		    <td colspan="2" style="font-weight: bold;">17. CEP</td>
		    <td style="font-weight: bold;">18. Telefone</td>
		    <td colspan="2" style="font-weight: bold;">19. E-Mail</td>
		  </tr>
		  <tr>
		    <td>'.$dadosProp['estado'].'</td>
		    <td colspan="2">'.$dadosProp['endcep'].'</td>
		    <td>'.$dadosProp['fone'].'</td>
		    <td colspan="2">'.$dadosProp['usuemail'].'</td>
		  </tr>
		  <tr>
		    <td style="font-weight: bold;">20. N&ordm; da Cédula da CI</td>
		    <td colspan="2" style="font-weight: bold;">21. Õrgão Expeditor</td>
		    <td colspan="3" style="font-weight: bold;">22. Cargo</td>
		  </tr>
		  <tr>
		    <td>'.$dadosProp['numeroidentidade'].'</td>
		    <td colspan="2">'.$dadosProp['entorgaoexpedidor'].'</td>
		    <td colspan="3">'.$dadosProp['usufuncao'].'</td>
		  </tr>
		</table>';
?>
<!-- DADOS DO ÕRGÃO OU ENTIDADE CONCEDENTE -->		
<?php 
	$dadoConc = buscarDadosUndConc();
		
	$html .= '
		<table id="dados_entidade_conc" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%">
		  <tr>
		    <td height="30" colspan="5" style="background-color: #CFCFCF; text-align:center; font-weight: bold;">DADOS DO ÕRGÃO OU ENTIDADE CONCEDENTE</td>
		  </tr>
		  <tr>
		    <td width="180" style="font-weight: bold;">23. Cód. Und. Gestora</td>
		    <td colspan="2" style="font-weight: bold;">24. Cód. da Gestão</td>
		    <td colspan="2" style="font-weight: bold;">25. Razão Social</td>
		  </tr>
		  <tr>
		    <td>'.$dadoConc['ungcod'].'</td>
		    <td colspan="2">'.$dadoConc['gescod'].'</td>
		    <td colspan="2">'.$dadoConc['ungdsc'].'</td>
		  </tr>
		  <tr>
		    <td height="21" colspan="2" style="font-weight: bold;">26. Endereço</td>
		    <td colspan="2" style="font-weight: bold;">27. Bairro ou Distrito</td>
		    <td style="font-weight: bold;">28. Município</td>
		  </tr>
		  <tr>
		    <td colspan="2">'.$dadoConc['ungendereco'].'</td>
		    <td colspan="2">'.$dadoConc['ungbairro'].'</td>
		    <td>'.$dadoConc['muncod'].'</td>
		  </tr>
		  <tr>
		    <td height="21" style="font-weight: bold;">29. UF</td>
		    <td style="font-weight: bold;">30. CEP</td>
		    <td colspan="2" style="font-weight: bold;">31. Telefone</td>
		    <td style="font-weight: bold;">32. E-Mail</td>
		  </tr>
		  <tr>
		    <td>'.$dadoConc['estuf'].'</td>
		    <td>'.$dadoConc['ungcep'].'</td>
		    <td colspan="2">'.$dadoConc['ungfone'].'</td>
		    <td>'.$dadoConc['ungemail'].'</td>
		  </tr>
		</table>';		
?>		
<!-- REPRESENTANTE LEGAL DO ÕRGÃO OU ENTIDADE CONCEDENTE -->		
<?php 
	$dadosRespConced = buscaResponsavelCons();

	$html .= '
		<table id="dados_concedente" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%">
		  <tr>
		    <td colspan="6" style="background-color: #CFCFCF; text-align:center; font-weight: bold;">REPRESENTANTE LEGAL DO ÕRGÃO OU ENTIDADE CONCEDENTE</td>
		  </tr>
		  <tr>
		    <td width="214" style="font-weight: bold;">33. CPF </td>
		    <td colspan="5" style="font-weight: bold;">34. Nome do Representante Legal</td>
		  </tr>
		  <tr>
		    <td>'.$dadosRespConced['usucpf'].'</td>
		    <td colspan="5">'.$dadosRespConced['usunome'].'</td>
		  </tr>
		  <tr>
		    <td colspan="3" style="font-weight: bold;">35. Endereço</td>
		    <td style="font-weight: bold;">36. Bairro ou Distrito</td>
		    <td colspan="2" style="font-weight: bold;">37. Município</td>
		  </tr>
		  <tr>
		    <td colspan="3">'.$dadosRespConced['endereco'].'</td>
		    <td>'.$dadosRespConced['bairro'].'</td>
		    <td colspan="2">'.$dadosRespConced['municipio'].'</td>
		  </tr>
		  <tr>
		    <td style="font-weight: bold;">38. UF</td>
		    <td colspan="2" style="font-weight: bold;">39. CEP</td>
		    <td style="font-weight: bold;">40. Telefone</td>
		    <td colspan="2" style="font-weight: bold;">41. E-Mail</td>
		  </tr>
		  <tr>
		    <td>'.$dadosRespConced['estado'].'</td>
		    <td colspan="2">'.$dadosRespConced['endcep'].'</td>
		    <td>'.$dadosRespConced['fone'].'</td>
		    <td colspan="2">'.$dadosRespConced['usuemail'].'</td>
		  </tr>
		  <tr>
		    <td style="font-weight: bold;">42. N&ordm; da Cédula da CI</td>
		    <td colspan="2" style="font-weight: bold;">43. Õrgão Expeditor</td>
		    <td colspan="3" style="font-weight: bold;">44. Cargo</td>
		  </tr>
		  <tr>
		    <td>'.$dadosRespConced['numeroidentidade'].'</td>
		    <td colspan="2">'.$dadosRespConced['entorgaoexpedidor'].'</td>
		    <td colspan="3">'.$dadosRespConced['usufuncao'].'</td>
		  </tr>
		</table>';
?>		
<!-- OBJETO E JUSTIFICATIVA DA DESCENTRALIZAÇÃO DO CRÉDITO -->
<?php 
	$dadosObjeto = buscarObjetoTermo();
	
	$html .= '
		<table id="objeto_justificativa" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%">
		  <tr>
		    <td colspan="2" style="background-color: #CFCFCF; text-align:center; font-weight: bold;">OBJETO E JUSTIFICATIVA DA DESCENTRALIZAÇÃO DO CRÉDITO</td>
		  </tr>
		  <tr>
		    <td colspan="2" style="font-weight: bold;">45. Identificação (Título/Objeto da Despesa)</td>
		  </tr>
		  <tr>
		    <td colspan="2">'.$dadosObjeto['tcpdscobjetoidentificacao'].'</td>
		  </tr>
		  <tr>
		    <td colspan="2" style="font-weight: bold;">46. Objetivo</td>
		  </tr>
		  <tr>
		    <td colspan="2">'.$dadosObjeto['tcpobjetivoobjeto'].'</td>
		  </tr>
		  <tr>
		    <td width="350" style="font-weight: bold;">47. UG/Gestão Repassadora</td>
		    <td width="350" style="font-weight: bold;">48. UG/Gestão Recebedora</td>
		  </tr>
		  <tr>
		    <td style="font-weight: bold;">152734 / CGSO-SPO </td>
		    <td>'.$dadosUG['ungcod'].' / '.$dadosUG['ungdsc'].'</td>
		  </tr>
		  <tr>
		    <td colspan="2" style="font-weight: bold;">49. Justificativa (Motivação/Clientela/Cronograma Fisico)</td>
		  </tr>
		  <tr>
		    <td colspan="2">'.$dadosObjeto['tcpobjetojustificativa'].'</td>
		  </tr>
		  <tr>
		    <td colspan="2">
			    <p>
					I - Integra este termo, independentemente de transcrição, o Plano de Trabalho e o Termo de Referência, cujos dados ali contidos acatam os partícipes e se comprometem em cumprir, sujeitando-se às normas da Lei no 8.666, de 21 de junho de 1993, no que couber, Decreto no  93.872/1986 e o de nº 6.170, de 25 de julho de 2007, Portaria Interministerial no 507, de 24 de novembro de 2011 e Portaria Conjunta MP/MF/CGU nº 8, de 7 de novembro de 2012.
				</p>
				<p>
					II - constituem obrigações da CONCEDENTE:<br>
					a) efetuar a transferência dos recursos financeiros previstos para a execução deste Termo, na forma estabelecida no Cronograma de Desembolso constante do Plano de Trabalho;<br>
					b) orientar, supervisionar e cooperar com a implantação das ações objeto deste Termo;<br>
					c) acompanhar as atividades de execução, avaliando os seus resultados e reflexos;<br>
					d) analisar o relatório de cumprimento do objeto do presente Termo;
				</p>
				<p>
					III - constituem obrigações da PROPONENTE:<br>
					a) promover a execução do objeto do Termo na forma e prazos estabelecidos no Plano de Trabalho;<br>
					b) aplicar os recursos discriminados exclusivamente na consecução do objeto deste Termo;<br>
					c) permitir e facilitar ao Órgão Concedente o acesso a toda documentação, dependências e locais do projeto;<br>
					d) observar e exigir, na apresentação dos serviços, se couber, o cumprimento das normas específicas que regem o Programa;<br>
					e) manter o órgão Concedente informado sobre quaisquer eventos que dificultem ou interrompam o curso normal de execução do Termo;<br>
					f) devolver os saldos dos créditos orçamentários descentralizados e não empenhados, bem como os recursos financeiros não utilizados, conforme norma de encerramento do correspondente exercício financeiro;<br>
					g) a prestação de contas dos créditos descentralizados devem integrar as contas anuais do órgão Proponente a serem apresentadas aos órgãos de controle interno e externo, conforme normas vigentes;<br>
					h) apresentar relatório de cumprimento do objeto pactuado até 60 dias após o término do prazo para cumprimento do objeto estabelecido no Termo.
				</p>
		    </td>
		  </tr>
		</table>';
?>	
<!-- PREVISÃO ORÇAMENTÁRIA -->		
<?php 
	$dadosPrevisao = buscarPrevisaoOrca();
	$html .= '		
		<table id="previsao_orc" border="1"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%">
		  <tr>
		    <td height="21" colspan="6"style="background-color: #CFCFCF; text-align:center; font-weight: bold;">PREVISÃO ORÇAMENTÁRIA</td>
		  </tr>
		  <tr>
		    <td style="font-weight: bold;">50. Programa de Trabalho</td>
		    <td style="font-weight: bold;">51. Ação</td>
		    <td style="font-weight: bold;">52. Plano Interno</td>
		    <td style="font-weight: bold;">53. Descrição da Ação constante da LOA</td>
		    <td style="font-weight: bold;">54. Nat. da Despesa</td>
		    <td style="font-weight: bold;">55. Valor (em R$ 1,00)</td>
		  </tr>';
	
			if($dadosPrevisao): 
	  			foreach($dadosPrevisao as $d):
					$html .= '		  
				 		  <tr>
						    <td>'.$d['plano_trabalho'].'</td><td>'.$d['acao'].'</td><td>'.$d['plano_interno'].'</td><td>'.$d['acao_loa'].'</td><td>'.$d['nat_despesa'].'</td><td>R$ '.$d['provalor'].'</td>
						  </tr>
						';
				endforeach;
			endif;
			
		$sql = "select * from monitora.previsaoorcamentaria where tcpid = {$_SESSION['elabrev']['tcpid']} order by proid limit 1";
		$rsPOmes = $db->pegaLinha($sql);
			
		$html .= '
		  <tr>
		  	<td><b>56. Mês da Liberação</b></td>
		  	<td>'.mes_extenso($rsPOmes['crdmesliberacao']).'&nbsp;</td>
		  	<td><b>57. Prazo para o cumprimento do objeto</b></td>
		  	<td>'.($rsPOmes['crdmesexecucao'] ? $rsPOmes['crdmesexecucao'].'&nbsp;meses' : '').'&nbsp;</td>
		    <td style="font-weight: bold;">58. TOTAL</td>
		    <td>R$ '.$d['total'].'</td>
		  </tr>
		</table>';
?>
<!--  ASSINATURAS -->
<?php 
	$mes = date("m");
	
	switch ($mes){
		case 1: $mes = "Janeiro"; break;
		case 2: $mes = "Fevereiro"; break;
		case 3: $mes = "Março"; break;
		case 4: $mes = "Abril"; break;
		case 5: $mes = "Maio"; break;
		case 6: $mes = "Junho"; break;
		case 7: $mes = "Julho"; break;
		case 8: $mes = "Agosto"; break;
		case 9: $mes = "Setembro"; break;
		case 10: $mes = "Outubro"; break;
		case 11: $mes = "Novembro"; break;
		case 12: $mes = "Dezembro"; break;
	
	}
	
	$sqlReitor = "select 
				u.usunome, 
				u.usucpf, 
				to_char(h.htddata, 'DD/MM/YYYY') as htddata,
				to_char(h.htddata, 'HH:II:SS') as hora,
				g.ungdsc
			from monitora.termocooperacao t
			inner join workflow.historicodocumento h on h.docid = t.docid
			inner join workflow.acaoestadodoc a on a.aedid = h.aedid
			inner join seguranca.usuario u on u.usucpf = h.usucpf
			left join unidadegestora g on g.ungcod = t.ungcodconcedente
			where t.tcpid = {$_SESSION['elabrev']['tcpid']}
			and a.esdiddestino = ".EM_ANALISE_DA_SECRETARIA."
			order by hstid asc";
	
	$rsReitor = $db->pegaLinha($sqlReitor);
	$stAnaliseReitor = '';
	if($rsReitor){
		$stAnaliseReitor = "Validado e encaminhado pelo reitor {$rsReitor['usunome']} no dia {$rsReitor['htddata']} às {$rsReitor['hora']} <br/>";
	}
	
	$sqlSecretario = "select
				u.usunome,
				u.usucpf,
				to_char(h.htddata, 'DD/MM/YYYY') as htddata,
				to_char(h.htddata, 'HH:II:SS') as hora,
				g.ungdsc
			from monitora.termocooperacao t
			inner join workflow.historicodocumento h on h.docid = t.docid
			inner join workflow.acaoestadodoc a on a.aedid = h.aedid
			inner join seguranca.usuario u on u.usucpf = h.usucpf
			left join unidadegestora g on g.ungcod = t.ungcodconcedente
			where t.tcpid = {$_SESSION['elabrev']['tcpid']}
			and a.esdiddestino = ".EM_ANALISE_PELA_CGSO."
			order by hstid asc";
	
	$rsSecretario = $db->pegaLinha($sqlSecretario);
	$stAnaliseSecretaria = '';
	if($rsSecretario){
		$stAnaliseSecretaria = "Validado e encaminhado pelo secretário {$rsSecretario['usunome']} no dia {$rsSecretario['htddata']} às {$rsSecretario['hora']}";
	}
	
	$html .= '		
		<table id="assinatura" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">		  
		  <tr>
		    <td>&nbsp;</td>
		  </tr>
		  <tr>
		    <td align="center">
		    	'.$stAnaliseReitor.'
		    	'.$stAnaliseSecretaria.'
		    </td>
		  </tr>
		  <tr>
		    <td>&nbsp;</td>
		  </tr>
		  <tr>
		    <td colspan="2" style="text-align:center; font-weight: bold;">Brasília, '.date("d").' de '.$mes.' de '.date("Y").'</td>
		  </tr>
		  <tr>
		    <td>&nbsp;</td>
		  </tr>		  		  
		</table>';
	
	}elseif( $_SESSION['elabrev']['termo']['aba'] == 'cumprimento' ){
		
		if($_REQUEST['requisicao'] == 'salvarDados'){
			
			if(!empty($_REQUEST['recid'])){
				
				$sql = "update elabrev.relatoriocumprimento set 
							tcpid = ".($_REQUEST['tcpid'] ? $_REQUEST['tcpid'] : $_SESSION['elabrev']['tcpid']).",
							reccnpj = ".($_REQUEST['reccnpj'] ? "'".str_replace(array('.','/','-', '\\'), '', $_REQUEST['reccnpj'])."'" : "''").",  
							recnome = ".($_REQUEST['recnome'] ? "'".$_REQUEST['recnome']."'" : "''").",  
							recendereco = ".($_REQUEST['recendereco'] ? "'".$_REQUEST['recendereco']."'" : "''").",
							muncod = ".($_REQUEST['muncod'] ? "'".$_REQUEST['muncod']."'" : "''").",  
							estuf = ".($_REQUEST['estuf'] ? "'".$_REQUEST['estuf']."'" : "''").",  
							reccep = ".($_REQUEST['reccep'] ? "'".str_replace(array('-','.'), '', $_REQUEST['reccep'])."'" : "''").",  
							rectelefone = ".($_REQUEST['rectelefone'] ? "'".$_REQUEST['rectelefoneddd'].str_replace('-', '', $_REQUEST['rectelefone'])."'" : "''").",  
							uocod = ".($_REQUEST['uocod'] ? "'".$_REQUEST['uocod']."'" : "''").",  
							ugcod = ".($_REQUEST['ugcod'] ? "'".$_REQUEST['ugcod']."'" : "''").",  
							gestaocod = ".($_REQUEST['gestaocod'] ? "'".$_REQUEST['gestaocod']."'" : "''").",  
							recnomeresponsavel = ".($_REQUEST['recnomeresponsavel'] ? "'".$_REQUEST['recnomeresponsavel']."'" : "''").",  
							reccpfresponsavel = ".($_REQUEST['recsiaperesponsavel'] ? "'".$_REQUEST['recsiaperesponsavel']."'" : "''").",  
							recsiaperesponsavel = ".($_REQUEST['recsiaperesponsavel'] ? "'".$_REQUEST['recsiaperesponsavel']."'" : "''").",  
							recrgresponsavel = ".($_REQUEST['recrgresponsavel'] ? "'".$_REQUEST['recrgresponsavel']."'" : "''").",  
							recdtemissaorgresposavel = ".($_REQUEST['recdtemissaorgresposavel'] ? "'".formata_data_sql($_REQUEST['recdtemissaorgresposavel'])."'" : "''").",  
							recexpedidorrgresposavel = ".($_REQUEST['recexpedidorrgresposavel'] ? "'".$_REQUEST['recexpedidorrgresposavel']."'" : "''").",  
							reccargo = ".($_REQUEST['reccargo'] ? "'".$_REQUEST['reccargo']."'" : "''").",  
							recemailresposavel = ".($_REQUEST['recemailresposavel'] ? "'".$_REQUEST['recemailresposavel']."'" : "''").",
							recnumportaria = ".($_REQUEST['recnumportaria'] ? "'".$_REQUEST['recnumportaria']."'" : "''").",  
							recdtpublicacao = ".($_REQUEST['recdtpublicacao'] ? "'".formata_data_sql($_REQUEST['recdtpublicacao'])."'" : "''").",  
							recnumnotacredito = ".($_REQUEST['recnumnotacredito'] ? "'".$_REQUEST['recnumnotacredito']."'" : "''").",
							recexecucaoobjeto = ".($_REQUEST['recexecucaoobjeto'] ? "'".$_REQUEST['recexecucaoobjeto']."'" : "''").",
							recatividadesprevistas = ".($_REQUEST['recmetaprevista'] ? "'".$_REQUEST['recmetaprevista']."'" : "''").",
							recmetaprevista = ".($_REQUEST['recmetaprevista'] ? "'".$_REQUEST['recmetaprevista']."'" : "''").",
							recatividadesexecutadas = ".($_REQUEST['recatividadesexecutadas'] ? "'".$_REQUEST['recatividadesexecutadas']."'" : "''").",
							recmetaexecutada = ".($_REQUEST['recmetaexecutada'] ? "'".$_REQUEST['recmetaexecutada']."'" : "''").",
							recdificuldades = ".($_REQUEST['recdificuldades'] ? "'".$_REQUEST['recdificuldades']."'" : "''").",
							recmetasadotadas = ".($_REQUEST['recmetasadotadas'] ? "'".$_REQUEST['recmetasadotadas']."'" : "''").",
							reccomentarios = ".($_REQUEST['reccomentarios'] ? "'".$_REQUEST['reccomentarios']."'" : "''").",
							recvlrrecebido = ".($_REQUEST['recvlrrecebido'] ? "'".formata_valor_sql($_REQUEST['recvlrrecebido'])."'" : "''").",
							recvlrutilizado = ".($_REQUEST['recvlrutilizado'] ? "'".formata_valor_sql($_REQUEST['recvlrutilizado'])."'" : "''").",
							recvlrdevolvido = ".($_REQUEST['recvlrdevolvido'] ? "'".formata_valor_sql($_REQUEST['recvlrdevolvido'])."'" : "''").",
							recnumncdevolucao = ".($_REQUEST['recnumncdevolucao'] ? "'".$_REQUEST['recnumncdevolucao']."'" : "''")."
					where recid = {$_REQUEST['recid']}";
				
			}else{
			
				$sql = "insert into elabrev.relatoriocumprimento 
						(						
							tcpid,
							reccnpj,  
							recnome,  
							recendereco,
							muncod,  
							estuf,  
							reccep,  
							rectelefone,  
							uocod,  
							ugcod,  
							gestaocod,  
							recnomeresponsavel,  
							reccpfresponsavel,  
							recsiaperesponsavel,  
							recrgresponsavel,  
							recdtemissaorgresposavel,  
							recexpedidorrgresposavel,  
							reccargo,  
							recemailresposavel,
							recnumportaria,  
							recdtpublicacao,  
							recnumnotacredito,
							recexecucaoobjeto,
							recatividadesprevistas,
							recmetaprevista,
							recatividadesexecutadas,
							recmetaexecutada,
							recdificuldades,
							recmetasadotadas,
							reccomentarios,
							recvlrrecebido,
							recvlrutilizado,
							recvlrdevolvido,
							recnumncdevolucao
						)
						values
						(
							".($_REQUEST['tcpid'] ? $_REQUEST['tcpid'] : $_SESSION['elabrev']['tcpid']).",
							".($_REQUEST['reccnpj'] ? "'".str_replace(array('.','/','-', '\\'), '', $_REQUEST['reccnpj'])."'" : "''").",
							".($_REQUEST['recnome'] ? "'".$_REQUEST['recnome']."'" : "''").",
							".($_REQUEST['recendereco'] ? "'".$_REQUEST['recendereco']."'" : "''").",
							".($_REQUEST['muncod'] ? "'".$_REQUEST['muncod']."'" : "''").",
							".($_REQUEST['estuf'] ? "'".$_REQUEST['estuf']."'" : "''").",					
							".($_REQUEST['reccep'] ? "'".str_replace(array('-','.'), '', $_REQUEST['reccep'])."'" : "''").",
							".($_REQUEST['rectelefone'] ? "'".$_REQUEST['rectelefoneddd'].str_replace('-', '', $_REQUEST['rectelefone'])."'" : "''").",
							".($_REQUEST['uocod'] ? "'".$_REQUEST['uocod']."'" : "''").",
							".($_REQUEST['ugcod'] ? "'".$_REQUEST['ugcod']."'" : "''").",
							".($_REQUEST['gestaocod'] ? "'".$_REQUEST['gestaocod']."'" : "''").",
							".($_REQUEST['recnomeresponsavel'] ? "'".$_REQUEST['recnomeresponsavel']."'" : "''").",							
							".($_REQUEST['reccpfresponsavel'] ? "'".$_REQUEST['reccpfresponsavel']."'" : "''").",						
							".($_REQUEST['recsiaperesponsavel'] ? "'".$_REQUEST['recsiaperesponsavel']."'" : "''").",
							".($_REQUEST['recrgresponsavel'] ? "'".$_REQUEST['recrgresponsavel']."'" : "''").",
							".($_REQUEST['recdtemissaorgresposavel'] ? "'".formata_data_sql($_REQUEST['recdtemissaorgresposavel'])."'" : "''").",
							".($_REQUEST['recexpedidorrgresposavel'] ? "'".$_REQUEST['recexpedidorrgresposavel']."'" : "''").",
							".($_REQUEST['reccargo'] ? "'".$_REQUEST['reccargo']."'" : "''").",
							".($_REQUEST['recemailresposavel'] ? "'".$_REQUEST['recemailresposavel']."'" : "''").",						
							".($_REQUEST['recnumportaria'] ? "'".$_REQUEST['recnumportaria']."'" : "''").",				
							".($_REQUEST['recdtpublicacao'] ? "'".formata_data_sql($_REQUEST['recdtpublicacao'])."'" : "''").",
							".($_REQUEST['recnumnotacredito'] ? "'".$_REQUEST['recnumnotacredito']."'" : "''").",
							".($_REQUEST['recexecucaoobjeto'] ? "'".$_REQUEST['recexecucaoobjeto']."'" : "''").",						
							".($_REQUEST['recatividadesprevistas'] ? "'".$_REQUEST['recatividadesprevistas']."'" : "''").",
							".($_REQUEST['recmetaprevista'] ? "'".$_REQUEST['recmetaprevista']."'" : "''").",			
							".($_REQUEST['recatividadesexecutadas'] ? "'".$_REQUEST['recatividadesexecutadas']."'" : "''").",
							".($_REQUEST['recmetaexecutada'] ? "'".$_REQUEST['recmetaexecutada']."'" : "''").",
							".($_REQUEST['recdificuldades'] ? "'".$_REQUEST['recdificuldades']."'" : "''").",					
							".($_REQUEST['recmetasadotadas'] ? "'".$_REQUEST['recmetasadotadas']."'" : "''").",
							".($_REQUEST['reccomentarios'] ? "'".$_REQUEST['reccomentarios']."'" : "''").",
							".($_REQUEST['recvlrrecebido'] ? "'".formata_valor_sql($_REQUEST['recvlrrecebido'])."'" : "''").",					
							".($_REQUEST['recvlrutilizado'] ? "'".formata_valor_sql($_REQUEST['recvlrutilizado'])."'" : "''").",
							".($_REQUEST['recvlrdevolvido'] ? "'".formata_valor_sql($_REQUEST['recvlrdevolvido'])."'" : "''").",
							".($_REQUEST['recnumncdevolucao'] ? "'".$_REQUEST['recnumncdevolucao']."'" : "''")."
						)";
			
			}
			
			$db->executar($sql);
			if($db->commit()){
				$db->sucesso('principal/termoCooperacao/cadTermoCooperacao', '&aba=cumprimento');
			}else{
				$db->insucesso('Falha na operação!', 'principal/termoCooperacao/cadTermoCooperacao', '&aba=cumprimento');
			}
		}
		
		echo '<script language="JavaScript" src="../includes/calendario.js"></script>';
		
		if($_SESSION['elabrev']['tcpid']){
			
			$sql = "
				Select	recid, tcpid, reccnpj, recnome, recendereco, muncod, estuf, reccep, 
						substr(rectelefone, 1, 2) as rectelefoneddd,
						substr(rectelefone, 3, 9) as rectelefone,
						uocod, ugcod, gestaocod, recnomeresponsavel, reccpfresponsavel, 
						recsiaperesponsavel, recrgresponsavel, recdtemissaorgresposavel, 
						recexpedidorrgresposavel, reccargo, recemailresposavel, recnumportaria, 
						recdtpublicacao, recnumnotacredito, recexecucaoobjeto, recatividadesprevistas, 
						recmetaprevista, recatividadesexecutadas, recmetaexecutada, recdificuldades, 
						recmetasadotadas, reccomentarios, recvlrrecebido, recvlrutilizado, 
					    recvlrdevolvido, recnumncdevolucao, recstatus
				From elabrev.relatoriocumprimento
				Where tcpid = ".$_SESSION['elabrev']['tcpid']."
			";
			//ver($sql, d);
			$rs = $db->pegaLinha($sql);
			if($rs) extract($rs);
		}
		
		echo "
			<script>
			function carregaMunicipios(id)
			{				
				jQuery.ajax({
					url		: 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A',
					type	: 'post',
					data	: 'requisicao=carregarMunicipios&estuf='+id+'&muncod={$muncod}&habilitado={$habilita_cumprimento}',
					success	: function(e){
						jQuery('#td_municipios').html(e);
					}
				});
			}
			
			$('.salvar').click(function(){
				$('#formulario').submit();
			});
			
			</script>
		";
	
		if( $habilita_cumprimento == "N" ){
			$habilita = "disabled=disabled";
		}else{
			$habilita = "";
		}
		
		$reccnpj = $reccnpj ? formatar_cnpj($reccnpj) : '';
		$recdtemissaorgresposavel = $recdtemissaorgresposavel ? formata_data($recdtemissaorgresposavel) : '';
		$recdtpublicacao = $recdtpublicacao ? formata_data($recdtpublicacao) : '';		
		$recvlrrecebido = $recvlrrecebido ? formata_valor($recvlrrecebido) : '';
		$recvlrutilizado =$recvlrutilizado ? formata_valor($recvlrutilizado) :'';
		$recvlrdevolvido =$recvlrdevolvido ? formata_valor($recvlrdevolvido) : '';
		$reccep = $reccep ? formata_cep($reccep) : '';		
		
		echo '<form method="post" name="formulario" id="formulario">
			<input type="hidden" name="tcpid" id="tcpid" value="'.$_SESSION['elabrev']['tcpid'].'" />
			<input type="hidden" name="recid" id="recid" value="'.$recid.'" />	
			<input type="hidden" name="requisicao" id="requisicao" value="salvarDados" />	
			<table id="assinatura" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
				<tr>
					<td colspan="2" class="subtituloCentro">Dados da Entidade Proponente</td>					
				</tr>
				<tr>
					<td class="subtituloDireita" width="240">CNPJ</td>
					<td>'.campo_texto('reccnpj', 'S', $habilita_cumprimento, '', 25, '', '##.###.###/####-##', '').'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Nome da Entidade</td>
					<td>'.campo_texto('recnome', 'S', $habilita_cumprimento, '', 45, '', '', '').'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Endereço</td>
					<td>'.campo_texto('recendereco', 'S', $habilita_cumprimento, '', 45, '', '', '').'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">UF</td>
					<td>';
					
					$sql = "select estuf as codigo, estdescricao as descricao from territorios.estado order by estuf";
					$rsUfs = $db->carregar($sql);
					
					echo '<select name="estuf" id="estuf" class="CampoEstilo"  '.$habilita.' onchange="carregaMunicipios(this.value)" '.($habilita_cumprimento == 'S' ? '' : 'selected="selected"').'><option value="">Selecione...</option>';
					foreach($rsUfs as $data){
						echo '<option value="'.$data['codigo'].'" '.($estuf == $data['codigo'] ? 'selected="selected"' : '').'>'.$data['descricao'].'</option>';
					}
					echo '</select>';
					echo '&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';
					
					
				echo '</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Município</td>
					<td id="td_municipios">Selecione um estado primeiro.</td>
				</tr>
				<tr>
					<td class="subtituloDireita">CEP</td>
					<td>'.campo_texto('reccep', 'S', $habilita_cumprimento, '', 15, '', '##.###-###', '').'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Telefone com DDD</td>
					<td>'.campo_texto('rectelefoneddd', 'S', $habilita_cumprimento, '', 4, '', '##', '').'&nbsp;
					'.campo_texto('rectelefone', 'S', $habilita_cumprimento, '', 20, '', '####-####', '').'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Código da UO</td>
					<td>'.campo_texto('uocod', 'S', $habilita_cumprimento, '', 20, '', '########', '').'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Código da UG</td>
					<td>'.campo_texto('ugcod', 'S', $habilita_cumprimento, '', 20, '', '########', '').'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Código da Gestão</td>
					<td>'.campo_texto('gestaocod', 'S', $habilita_cumprimento, '', 20, '', '########', '').'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Nome do Responsável</td>
					<td>'.campo_texto('recnomeresponsavel', 'S', $habilita_cumprimento, '', 45, '', '', '').'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">CPF do Responsável</td>
					<td>'.campo_texto('reccpfresponsavel', 'S', $habilita_cumprimento, '', 20, '', '###.###.###-##', '').'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">SIAPE do Responsável</td>
					<td>'.campo_texto('recsiaperesponsavel', 'S', $habilita_cumprimento, '', 15, '', '##########', '').'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Identidade do Responsável</td>
					<td>'.campo_texto('recrgresponsavel', 'S', $habilita_cumprimento, '', 10, '', '', '').'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Data Emissão</td>
					<td>'.campo_data('recdtemissaorgresposavel', 'S', $habilita_cumprimento, '', 10, '', '', '').'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Orgão Expedidor</td>
					<td>'.campo_texto('recexpedidorrgresposavel', 'S', $habilita_cumprimento, '', 10, '', '', '').'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Cargo</td>
					<td>'.campo_texto('reccargo', 'S', $habilita_cumprimento, '', 45, '', '', '').'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">E-mail do Responsável</td>
					<td>'.campo_texto('recemailresposavel', 'S', $habilita_cumprimento, '', 45, '', '', '').'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Nº da Portaria ou Decreto de Nomeação</td>
					<td>'.campo_texto('recnumportaria', 'S', $habilita_cumprimento, '', 25, '', '', '').'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Data de Publicação</td>
					<td>'.campo_data('recdtpublicacao', 'S', $habilita_cumprimento, '', 10, '', '', '').'</td>
				</tr>
				<tr>
					<td colspan="2" class="subtituloCentro">Dados do Objeto da Descentralização do Crédito</td>					
				</tr>
				<!--
				<tr>
					<td class="subtituloDireita">Nº do Termo de Execução Descentralizado</td>
					<td>'.campo_texto('recnumportaria', 'S', $habilita_cumprimento, '', 25, '', '', '').'</td>
				</tr>
				-->
				<tr>
					<td class="subtituloDireita">Nº da Nota de Crédito (NC)</td>
					<td>'.campo_texto('recnumnotacredito', 'S', $habilita_cumprimento, '', 25, '', '', '').'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Execução do Objeto</td>
					<td>';
									
					$arExecucao = array(
								array('codigo'=>1,'descricao'=>'Houve cumprimento TOTAL'),
								array('codigo'=>2,'descricao'=>'Houve cumprimento PARCIAL'),
								array('codigo'=>3,'descricao'=>'Houve devolução integral')
							);
					
					echo '<select name="recexecucaoobjeto" id="recexecucaoobjeto"  '.$habilita.' class="CampoEstilo" '.($habilita_cumprimento == 'S' ? '' : 'selected="selected"').'><option value="">Selecione...</option>';
					foreach($arExecucao as $data){
						echo '<option value="'.$data['codigo'].'" '.($recexecucaoobjeto == $data['codigo'] ? 'selected="selected"' : '').'>'.$data['descricao'].'</option>';
					}
					echo '</select>';
					echo '&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';
					
				
				echo '</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Atividades Previstas</td>
					<td>'.campo_textarea('recatividadesprevistas', 'S', $habilita_cumprimento, '', 75, 3, 5000).'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Meta Prevista</td>
					<td>'.campo_textarea('recmetaprevista', 'S', $habilita_cumprimento, '', 75, 3, 5000).'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Atividades Executadas</td>
					<td>'.campo_textarea('recatividadesexecutadas', 'S', $habilita_cumprimento, '', 75, 3, 5000).'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Meta Executada</td>
					<td>'.campo_textarea('recmetaexecutada', 'S', $habilita_cumprimento, '', 75, 3, 5000).'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Dificuldades Encontradas na Execução da Descentralização</td>
					<td>'.campo_textarea('recdificuldades', 'S', $habilita_cumprimento, '', 75, 3, 5000).'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Medidas Adotadas para Sanar as Dificuldades de Modo a Assegurar o Cumprimrnto do Objeto</td>
					<td>'.campo_textarea('recmetasadotadas', 'S', $habilita_cumprimento, '', 75, 3, 5000).'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Comentários Adicionais</td>
					<td>'.campo_textarea('reccomentarios', 'S', $habilita_cumprimento, '', 75, 3, 5000).'</td>
				</tr>
				<tr>
					<td colspan="2" class="subtituloCentro">Detalhamento do Crédito Orçamentário Recebido</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Valor Recebido (R$ 1,00)</td>
					<td>'.campo_texto('recvlrrecebido', 'S', $habilita_cumprimento, '', 25, '', '[###.],##', '').'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Valor Utilizado (R$ 1,00)</td>
					<td>'.campo_texto('recvlrutilizado', 'S', $habilita_cumprimento, '', 25, '', '[###.],##', '').'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Valor Devolvido (R$ 1,00)</td>
					<td>'.campo_texto('recvlrdevolvido', 'S', $habilita_cumprimento, '', 25, '', '[###.],##', '').'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Nº da NC de Devolução</td>
					<td>'.campo_texto('recnumncdevolucao', 'S', $habilita_cumprimento, '', 25, '', '', '').'</td>
				</tr>
			</table>			
		';	

				if($estuf){
					echo "
					<script>
						carregaMunicipios('{$estuf}');	
					</script>
					";
				}
		
	}
	
	echo $html;

	if($_REQUEST['requisicao'] == 'gerarPDF'){
		
		ob_clean();
	
		$content = http_build_query( array('conteudoHtml'=> utf8_encode($html) ) );
		
		$context = stream_context_create( array( 'http'=>array( 'method'=>'POST', 'content'=> $content )  ) );
		
		$contents = file_get_contents('http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context );
		
		header('Content-Type: application/pdf');
		header("Content-Disposition: attachment; filename=termo_cooperacao");
		echo $contents;
		exit;
	}
?>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td colspan="3" align="center">
			<input type="button" id="voltarAba" value="Voltar" class="navegar" style="display:none;"/>						
			<?php			
			
			//if($habilitaSalvar == 'S'): 
				if( $_SESSION['elabrev']['termo']['aba'] != 'tramite' && $_SESSION['elabrev']['termo']['aba'] != 'gerapdf'){
					
			?>
				<?php if( $_SESSION['elabrev']['termo']['aba'] != '' && $_SESSION['elabrev']['termo']['aba'] != 'proponente'){?>
					<input type="button" id="volta" value="Gravar e Voltar" class="voltar"/> 
				<?php }?>
				<?php if($habilita == 'S'): ?>
					<input type="button" id="salva" value="Gravar" class="salvar"/>
				<?php if( $_SESSION['elabrev']['termo']['aba'] != 'tramite' ){?>
					<input type="button" id="continua" value="Gravar e Continuar" class="continuar"/>
				<?php }?>
				<?php endif; ?>
			<?php }elseif( $_SESSION['elabrev']['termo']['aba'] == 'tramite' && $_SESSION['elabrev']['termo']['aba'] != 'gerapdf'){ 
			
					$perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
					if( $_SESSION['elabrev']['termo']['aba'] == 'tramite' && $estadoAtual == RELATORIO_OBJ_AGUARDANDO_ANALISE_COORD && (in_array(PERFIL_COORDENADOR_SEC, $perfis))){
						echo '<input type="button" id="salva" value="Gravar" class="salvar">';
					}
					?>
					<input type="button" value="Voltar" class="voltarT" onclick="history.back();">
			<?php }elseif( $_SESSION['elabrev']['termo']['aba'] == 'gerapdf'){ ?>
					<input type="button" value="Gerar PDF" class="gerarPDF"/>
			<?php }
			//endif;

			#Para que seja possível a execução da regra de olcutar os botoes.
			if($repConc['usucpfconcedente'] == '' && $_REQUEST['aba'] == 'concedente'){
				echo "<script type=\"text/javascript\">";
				echo "jQuery('#volta').hide();";
				echo "jQuery('#salva').hide();";
				echo "jQuery('#continua').hide();";
				echo "</script>";
			}
			
			?>
			<input type="button" id="proximaAba" value="Continuar" class="navegar" style="display:none;"/>
			</td>
		</tr>
	</table>
</form>