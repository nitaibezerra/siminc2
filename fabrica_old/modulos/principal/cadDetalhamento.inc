<?php
include_once APPRAIZ ."includes/workflow.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

require_once APPRAIZ . "fabrica/classes/HistoricoPausaServico.class.inc";
$oHistoricoPausaServico = new HistoricoPausaServico();

echo '<br>';

if($_REQUEST['scsid']) {
	$_SESSION['fabrica_var']['scsid'] = $_REQUEST['scsid']; 
}

if($_REQUEST['ansid']) {
	$_SESSION['fabrica_var']['ansid'] = $_REQUEST['ansid']; 
}

if($_SESSION['fabrica_var']['ansid']){
	$sql = "SELECT 
				a.ansdtrecebimento, a.ctrid, s.docid, to_char(a.ansprevinicio,'dd/mm/YYYY') as ansprevinicio
				, to_char(a.ansprevtermino,'dd/mm/YYYY') as ansprevtermino, a.mensuravel, a.ansgarantia 
			FROM fabrica.analisesolicitacao a 
			LEFT JOIN fabrica.solicitacaoservico s ON a.scsid=s.scsid 
			WHERE ansid='". $_SESSION['fabrica_var']['ansid']."'";
	
	$dadosan = $db->pegaLinha($sql);
	
	$ansdtrecebimento 	= $dadosan['ansdtrecebimento'];
	$ansctrid 			= $dadosan['ctrid'];
	$ansmensuravel 		= $dadosan['mensuravel'];
	$ansgarantia 		= $dadosan['ansgarantia'];
	
	$estado_wf = wf_pegarEstadoAtual($dadosan['docid']);
}

if($_REQUEST['odsid']) {
	
	$sql = "SELECT * FROM fabrica.ordemservico WHERE scsid='".$_SESSION['fabrica_var']['scsid']."' AND odsid='".$_REQUEST['odsid']."'";
	$ordemservico = $db->pegaLinha($sql);
	
	$odsdetalhamento  = $ordemservico['odsdetalhamento'];
	$odsdtprevinicio  = $ordemservico['odsdtprevinicio'];
	$odsdtprevtermino = $ordemservico['odsdtprevtermino'];
	$odssubtotalpf    = $ordemservico['odssubtotalpf'];
	$odsqtdpfestimada =	$ordemservico['odsqtdpfestimada'];
	$odsenderecosvn   =	$ordemservico['odsenderecosvn'];
	
	
	$sql = "SELECT d.dspid as codigo, d.dspdsc as descricao FROM fabrica.disciplina d 
			LEFT JOIN fabrica.ordemservicodisciplina o ON o.dspid=d.dspid 
			WHERE odsid='".$ordemservico['odsid']."'";
	$dspid = $db->carregar($sql);
	
	$sql = "SELECT p.prdid as codigo, p.prddsc as descricao FROM fabrica.produto p 
			LEFT JOIN fabrica.ordemservicoproduto o ON o.prdid=p.prdid 
			WHERE odsid='".$ordemservico['odsid']."'";
        
        $sql = "SELECT p.prdid as codigo, p.prddsc as descricao FROM fabrica.produto p 
			LEFT JOIN fabrica.ordemservicoproduto o ON o.prdid=p.prdid 
			WHERE odsid='".$ordemservico['odsid']."'";
        
        
	$prdid = $db->carregar($sql);
	
	
	$requisicao = "atualizarDetalhamentoSolicitacaoServico";
	
} else {
	$requisicao = "inserirDetalhamentoSolicitacaoServico";
}

//permissao para edição da tela
$habil = 'S';
if(!verificaPermissaoEdicao()) $habil = 'N';

/**** somente super usuário, fiscal, gestor podem inserir/alterar uma analise ****/
$pfls = arrayPerfil();

if(in_array(PERFIL_SUPER_USUARIO, $pfls) ||
   in_array(PERFIL_PREPOSTO,  	  $pfls) ||
   in_array(PERFIL_ESPECIALISTA_SQUADRA,  	  $pfls) ||
   in_array(PERFIL_ADMINISTRADOR, $pfls)) {
   	
   	$habil = 'S';
   
} else {
   
	$habil = 'N';
}



$menu = carregarMenuDetalhamentoSolicitacao();

if(!$ansdtrecebimento || ($habil=='N')) {
	unset($menu[2],$menu[3],$menu[4]);
}

echo montarAbasArray($menu, "/fabrica/fabrica.php?modulo=principal/abrirSolicitacao&acao=A&ansid=".$_SESSION['fabrica_var']['ansid']."&scsid=".$_SESSION['fabrica_var']['scsid']);

$titulo_modulo = "Detalhamento da solicitação";
monta_titulo($titulo_modulo, 'Empresa Item 1');


//recupera sigla do item da metrica
$sigla = recuperaMetrica( $_SESSION['fabrica_var']['ansid'] );

?>
<!-- (INÍCIO) BIBLIOTECAS - PARA USO DOS COMPONENTES (CALENDÁRIO E SLIDER) -->
<script	language="javascript" type="text/javascript" src="../includes/blendtrans.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/slider/slider.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script type="text/javascript" src="./js/jquery-ui-1.8.16.custom.min.js"></script>
<script type="text/javascript" src="./js/jquery.validate.min.js"></script>
<script type="text/javascript" src="./js/fabrica.js"></script>

<script language="javascript" type="text/javascript">

function validaForm(){

	if(document.getElementById('odsdetalhamento').value.length == 0) {
		alert('Preencha a descrição detalhada');
		return false;
	}
	
	if(document.getElementById('odsdtprevinicio').value.length == 0) {
		alert('Preencha a previsão de início');
		return false;
	}

	if(document.getElementById('odsdtprevtermino').value.length == 0) {
		alert('Preencha a previsão de término');
		return false;
	}
	<?if($sigla == 'PF'){?>
	if(document.getElementById('odssubtotalpf2').value == '') {
		alert('Preencha a Qtd. de Subtotal de PF');
		return false;
	}
	<?}?>
	if(document.getElementById('odsqtdpfestimada').value.length == 0) {
		alert('Preencha a Qtd. Estimada de <?=$sigla?>');
		return false;
	}
	if(document.getElementById('odsenderecosvn').value.length == 0) {
		alert('Preencha o Endereço do Repositório (SVN)');
		return false;
	}



	var dataInicio = $("#odsdtprevinicio").val();
	var dataInicioConvertida = dataInicio.substring(6,10) + dataInicio.substring(3,5) + dataInicio.substring(0,2);
	
	var dataFim = $("#odsdtprevtermino").val();
	var dataFimConvertida = dataFim.substring(6,10) + dataFim.substring(3,5) + dataFim.substring(0,2);
	
	if (dataInicioConvertida > dataFimConvertida){
		alert('Período informado inválido');
		return false;
	}

	/*
	// SS-982 - REQ002 - Solicitação de Serviço - Detalhamento da Solicitação
	var dataFimTerminoAnaliseSolicitacao = $("#prevterminoAnaliseSolicitacao").text();
	var dataFimTerminoAnaliseSolicitacaoConvertida = dataFimTerminoAnaliseSolicitacao.substring(6,10) + dataFimTerminoAnaliseSolicitacao.substring(3,5) + dataFimTerminoAnaliseSolicitacao.substring(0,2);

	if (dataFimConvertida > dataFimTerminoAnaliseSolicitacaoConvertida){
		alert('Período informado inválido');
		return false;
	}*/
	
	<? 
	if(in_array(PERFIL_SUPER_USUARIO, 		 $pfls) ||
	   in_array(PERFIL_PREPOSTO,  	  		 $pfls) ||
	   in_array(PERFIL_FISCAL_CONTRATO,  	 $pfls) ||
	   in_array(PERFIL_ESPECIALISTA_SQUADRA, $pfls) ||
	   in_array(PERFIL_ADMINISTRADOR, $pfls)) { ?>
	   	
	if(document.getElementById('odsdtprevinicio').value && document.getElementById('odsdtprevtermino').value) {
		
		var data_an1 = '<? echo $dadosan['ansprevinicio']; ?>';
		var data_an2 = '<? echo $dadosan['ansprevtermino']; ?>';
		
		var data_1 = document.getElementById('odsdtprevinicio').value;
		var data_2 = document.getElementById('odsdtprevtermino').value;
	
		var compara_an1 = parseInt(data_an1.split("/")[2].toString() + data_an1.split("/")[1].toString() + data_an1.split("/")[0].toString());	
		var compara_an2 = parseInt(data_an2.split("/")[2].toString() + data_an2.split("/")[1].toString() + data_an2.split("/")[0].toString());
		var compara01   = parseInt(data_1.split("/")[2].toString()   + data_1.split("/")[1].toString()   + data_1.split("/")[0].toString()  );  
		var compara02   = parseInt(data_2.split("/")[2].toString()   + data_2.split("/")[1].toString()   + data_2.split("/")[0].toString()  );
		  
		if (compara01 > compara02) {  
			alert('Data de início não pode ser maior que a data de termino');
			return false;  
		}

		if (compara_an1 > compara01 || compara01 > compara_an2 ) {  
			alert('A data de previsão de início deve estar dentro do período cadastrado na análise');
			return false;  
		}

		/*
		// SS-982 - REQ002 - Solicitação de Serviço - Detalhamento da Solicitação
		if (compara_an2 < compara02) {  
			alert('As datas de previsão  de início e término devem estar dentro do período cadastrado na análise');
			return false;  
		}
		*/
		
	}
	<? } ?>

	
	$.ajax({
   		type: "POST",
   		url: "fabrica.php?modulo=principal/abrirSolicitacao&acao=A",
   		data: "requisicao=verificaPontosFuncao",
   		success: function(msg){
   			if(msg == 'N'){
   				alert('O contrato não possui pontos de função suficiente');
   				return false;
   			}
   			else{
   				document.getElementById('formulario_').submit();   	   			
   			}
   		}
 		});
	
	
} 
function IsMensuravel(valor) {

	if(valor == "TRUE") {
		if($('#sidid').val() == "") {
			alert('Selecione um sistema');
			document.getElementsByName('ansmensuravel')[1].checked='checked';
			return false;
		}
        }
}
$(document).ready(function() {
	var isMensuravel = '<?php echo $ansmensuravel; ?>';

	if (isMensuravel == "t"){
		IsMensuravel('TRUE');
	}
});

function voltar2(url){
	window.location = url;
}

</script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<?

telaCabecalhoSolicitacaoServico(array("scsid" => $_SESSION['fabrica_var']['scsid']));
telaCabecalhoAnaliseSolicitacaoServico(array("ansid" => $_SESSION['fabrica_var']['ansid']));
//listaDisciplinaArtefato($_SESSION['fabrica_var']['ansid'], null, true, 1);
editaDisciplinaArtefato($_SESSION['fabrica_var']['ansid']);

if( $ansdtrecebimento ) {
?>

<? if($habil=='S') : ?>
<form method="post" name="formulario_" id="formulario_" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="<? echo $requisicao; ?>">
<input type="hidden" name="scsid" value="<? echo $_SESSION['fabrica_var']['scsid']; ?>">
<input type="hidden" name="odsqtdpfestimada_" value="<? echo $odsqtdpfestimada; ?>">
<input type="hidden" name="odssubtotalpf" value="<? echo $odssubtotalpf; ?>">
<input type="hidden" name="ctrid" value="<? echo $ansctrid; ?>">
<input type="hidden" name="sigla" value="<? echo $sigla; ?>">


<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td width="95%">
	<table class="listagem" width="100%">
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Análise Preliminar da solicitação de serviço</td>
	</tr>
	<tr>
		<td width="40%" class="SubTituloDireita">Descrição do serviço:</td>
		<td><? echo campo_textarea( 'odsdetalhamento', 'S', $habil, '', '70', '4', '5000'); ?></td>
	</tr>
        
	<tr>
		<td class="SubTituloDireita">Previsão de início:</td>
		<td><? echo campo_data2('odsdtprevinicio','S', empty($_GET['odsid']) ? $habil : 'N', 'Previsão de início', 'S' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Previsão de Término:</td>
		<td>
                    <? echo campo_data2('odsdtprevtermino','S', empty($_GET['odsid']) ? $habil : 'N', 'Previsão de Término', 'S' ); ?>

                    <?php if(!empty($_GET['odsid']) && $habil == 'S'): ?>
                    <script type="text/javascript">
                        function alterarPrevisao(){

                            window.open('?modulo=principal/popup/alterarPrevisaoTermino&acao=A&odsid=<?php echo $_GET['odsid']; ?>&alteracao=OSitem1', 'alterarPrevisaoTermino', 'top=350, left=100, align=center,toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=850,height=320');
                            
                        }
                    </script>
                    <a href="javascript: alterarPrevisao()"
                       style="text-decoration: underline;">
                        Redefinir Previs&atilde;o de T&eacute;rmino
                    </a>
                    <?php endif; ?>
                </td>
	</tr>
	<?if($sigla == 'PF'){?> 
	<tr>
		<td class="SubTituloDireita">Subtotal de PF:</td>
		<td>
			<? if($odssubtotalpf) $odssubtotalpf = number_format($odssubtotalpf,2,",",".");?>
			<? echo campo_texto('odssubtotalpf', 'S', ($ansgarantia == 't' ? "N" : "S"), 'Subtotal de PF', 15, 14, '###.###.###,##', '', 'right', '', 0, 'id="odssubtotalpf2"', '', ($ansgarantia == 't' ? '0,00' : "") ); ?>
		</td>
	</tr>
	<?}?>
	<tr>
		<td class="SubTituloDireita">Qtd. Estimada de <?=$sigla?>:</td>
		<td>
			<?php if($odsqtdpfestimada) $odsqtdpfestimada = number_format($odsqtdpfestimada,2,",",".");?>
			<? echo campo_texto('odsqtdpfestimada', 'S', ($ansgarantia == 't' ? "N" : "S"), 'Qtd. de P.F. Estimado', 15, 14, '###.###.###,##', '', 'right', '', 0, 'id="odsqtdpfestimada"', '', ($ansgarantia == 't' ? '0,00' : "") ); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Endereço do Repositório (SVN):</td>
		<td>
			<? echo campo_texto('odsenderecosvn', 'S', $habil, 'Endereço do Repositório (SVN)', 65, 200, '', '', '', '', 0, 'id="odsenderecosvn"' ); ?>
			<input type="hidden" value="<?php echo($dadosan['ansprevtermino']); ?>" name="ansprevtermino" id="ansprevtermino"/>
		</td>
	</tr>
        <tr>
            <td class="SubTituloDireita">Tipo:
            	<?php if( $ansgarantia=='t' ) $ansmensuravel = "f" ;?>
                <input type="radio" name="ansmensuravel" value="TRUE" <? echo (($ansmensuravel=="t"||!$ansmensuravel)?"checked ":""); echo $disabled_; ?> <? echo (($habil=='S')?"":"disabled"); ?> onclick="if(this.checked){IsMensuravel(this.value);}" > Mensurável  
                <input type="radio" name="ansmensuravel" value="FALSE" <? echo (($ansmensuravel=="f")?"checked ":""); echo $disabled_; ?> <? echo (($habil=='S')?"":"disabled"); ?> onclick="if(this.checked){IsMensuravel(this.value);}" > Não Mensurável
            </td>
        </tr>

	</table>
	
	</td>
	<td valign="top" width="5%">
	<? 
	$dados_wf = array('scsid' => $_SESSION['fabrica_var']['scsid']);
	wf_desenhaBarraNavegacao(pegarDocidSolicitacaoServico($dados_wf), $dados_wf);

    $_GET['scsid'] = empty($_GET['scsid']) ? $_SESSION['fabrica_var']['scsid'] : $_GET['scsid'];

    if( $estado_wf['esdid'] == WF_ESTADO_DETALHAMENTO )
	{
		?>
	    <table border="0" cellpadding="3" cellspacing="0"
	        style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
	        <tr style="background-color: #c9c9c9; text-align: center;">
	            <td style="font-size: 7pt; text-align: center;">
	                <span title="estado atual">
	                    <b>outras ações</b>
	                </span>
	            </td>
	        </tr>
	        <tr>
	            <td 
	                style="font-size: 7pt; text-align: center; border-bottom: 2px solid #c9c9c9;"
	                onmouseover="this.style.backgroundColor='#ffffdd';"
	                onmouseout="this.style.backgroundColor='';">
	                <a href="javascript: popPausarServico();">
	                    Enviar para Pausa
	                </a>
	            </td>
	        </tr>
	    </table>
	    <?php 
	}
    ?>

	</td>
</tr>
<tr>
	<td class="SubTituloDireita">
	<!-- 
	<input type="button" name="inseriranalise" value="Salvar" onclick="submeterDetalhamentoSolicitacaoServico();">
	 -->
	<input type="button" name="inseriranalise" value="Salvar" onclick="validaForm();">

	
	<? if($ordemservico['odsid']) { ?>
	<input type="button" name="novoos" value="Novo" onclick="window.location='fabrica.php?modulo=principal/cadDetalhamento&acao=A';">
	<? } ?>

	<input type="button" name="voltar" value="Voltar" onclick="voltar2('?modulo=principal/listarSolicitacoes&acao=A&manterPesquisa=true');">
	</td>
</tr>
</table>
</form>
<? endif; ?>

<?
	if($sigla == 'PF'){
		$cabecalho = array("Ações","Nº","Detalhamento","SVN","Previsão início","Previsão término","Subtotal de PF","Qtd. Estimada de PF");
		$alinhamento   = array("center","center","left","left","center","center","right","right");
		$sql = "SELECT 
					'<center>".(($habil=='S')?"<img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'fabrica.php?modulo=principal/cadDetalhamento&acao=A&odsid='||odsid||'\';\">":"")."</center>' as acao,
					 odsid, 
					 odsdetalhamento, 
					 odsenderecosvn,
					 to_char(odsdtprevinicio, 'dd/mm/YYYY') as odsdtprevinicio, 
					 to_char(odsdtprevtermino, 'dd/mm/YYYY') as odsdtprevtermino, 
					 odssubtotalpf,
					 odsqtdpfestimada
				FROM fabrica.ordemservico 
				WHERE scsid='".$_SESSION['fabrica_var']['scsid']."' 
				and tosid = ".TIPO_OS_GERAL." 
				ORDER BY odsid";
		$db->monta_lista($sql,$cabecalho,100,5,'S','center',$par2,"",$tamanho,$alinhamento);
	}
	else{
		$cabecalho = array("Ações","Nº","Detalhamento","SVN","Previsão início","Previsão término","Qtd. Estimada de UST");
		$alinhamento   = array("center","center","left","left","center","center","right");
		$sql = "SELECT 
					'<center>".(($habil=='S')?"<img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'fabrica.php?modulo=principal/cadDetalhamento&acao=A&odsid='||odsid||'\';\">":"")."</center>' as acao,
					 odsid, 
					 odsdetalhamento, 
					 odsenderecosvn,
					 to_char(odsdtprevinicio, 'dd/mm/YYYY') as odsdtprevinicio, 
					 to_char(odsdtprevtermino, 'dd/mm/YYYY') as odsdtprevtermino, 
					 odsqtdpfestimada
				FROM fabrica.ordemservico 
				WHERE scsid='".$_SESSION['fabrica_var']['scsid']."' 
				and tosid = ".TIPO_OS_GERAL." 
				ORDER BY odsid";
		$db->monta_lista($sql,$cabecalho,100,5,'S','center',$par2,"",$tamanho,$alinhamento);
	}


} else {
?>
<form method="post" name="formulario_" id="formulario_" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="confirmarRecebimentoAnalise">
<input type="hidden" name="confrecebimento" value="sim">

<input type="hidden" name="ansid" value="<? echo $_SESSION['fabrica_var']['ansid']; ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloEsquerda">
	<? if($habil=='S'): ?>
	<input type="button" name="confirmar" id="confirmar" value="Iniciar detalhamento da solicitação?" onclick="document.getElementById('formulario_').submit();"> 
	<input type="button" name="voltar" value="Voltar" onclick="history.back(-1);">
	<? else : ?>
	**** Detalhamento não iniciado
	<? endif; ?>
	</td>
</tr>
</table>
</form>
<?
}
?>

<script language="javascript" type="text/javascript">
$(document).ready(function() {
	//removendo os totais endesejados
	$('.listagem > thead:eq(1) > tr > td:eq(1)').text('');
});
function popPausarServico(){
    window.open('?modulo=principal/popup/pausarServico&acao=A&scsid=<?php echo $_GET['scsid']; ?>', 'pausarServico', 'top=350, left=100, align=center,toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=850,height=320');
}
</script>
