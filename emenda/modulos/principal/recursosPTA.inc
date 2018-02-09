<?php
//cadastraGuiaPTA('2503', '');
include_once APPRAIZ . 'includes/workflow.php';
//include_once( APPRAIZ . 'emenda/classes/PTA.class.inc');
$obPTA = new PTA();

if ($_REQUEST['carregaEmendasDisponiveis']) {
	header('content-type: text/html; charset=ISO-8859-1');
	$obPTA->carregaEmendasDisponiveisAjax($_POST['mdeid'], $_SESSION["emenda"]["enbid"]);
	exit;
}

if ($_REQUEST['verificaValorGuia']) {
	header('content-type: text/html; charset=ISO-8859-1');
	if( !empty($_POST['guia_check']) ){
		$guiids = $_POST['guia_check'];
		
		$sql = "SELECT 
   				sum(d.dicvalor) as valor
			FROM
            	emenda.guia g
                inner join emenda.guia_guiainiciativa ggi on ggi.guiid = g.guiid
                inner join emenda.guiainiciativa gi on gi.ginid = ggi.ginid
                inner join emenda.guiaespecificacao ge on ge.ginid = gi.ginid                
                inner join emenda.itempar_especificacao ie on ie.iteid = ge.iteid and ie.espid = ge.espid
                inner join emenda.itempar ip on ip.ipaid = ie.ipaid    
                inner join par.propostaitemcomposicao p ON ip.picid = p.picid
                inner join par.detalheitemcomposicao d ON d.picid = p.picid
                inner join par.ufdetalheitemcomposicao ufd ON ufd.dicid = d.dicid
			WHERE
				g.guiid in ($guiids)
			    and gi.ginistatus = 'A'
    			and g.guistatus = 'A'
                and d.dicstatus = 'A'
                and (cast(now() as date) between d.dicdatainicial and d.dicdatafinal) 
                and ufd.estuf in (select estuf from emenda.entidadebeneficiada where enbid = {$_POST['enbid']})";
        $valorGuia = $db->pegaUm($sql);
        
       /* $valorGuia = number_format($valorGuia, 2, ',', '.');
        $valorGuia = str_replace('.','', $valorGuia);
		$valorGuia = str_replace(',','.', $valorGuia);*/
        
        echo (float)$valorGuia; 
	} else {
		echo '0.00';
	}
	exit;
}

if ($_REQUEST['carregaDetalheEmenda']) {
	header('content-type: text/html; charset=ISO-8859-1');
	$obPTA->carregaDetalheEmendaAjax($_POST['emeid']);
	exit;
}
if ($_REQUEST['insereEmendasDetalhe']) {
	header('content-type: text/html; charset=ISO-8859-1');
	$obPTA->inserePlanoTrabalho($_POST);
	exit;
}
if ($_REQUEST['verificaExistenciaEmenda']) {
	header('content-type: text/html; charset=ISO-8859-1');
	$obPTA->verificaExistenciaEmendaAjax($_POST['resid']);
	exit;
}

if ($_REQUEST['carregaPorcentagemContrapartida']) {
	header('content-type: text/html; charset=ISO-8859-1');
	$obPTA->carregaPorcentagemContrapartidaAjax($_POST['mdeid'],$_POST['enbid'], $_POST['emdid']);
	exit;
}

$obEmenda = new Emenda();
unset( $_SESSION['emenda']['federal'] );

if($tipo != 'cadastro') {
	if($_REQUEST['ptrid']){
		$ptrid = $_REQUEST['ptrid'];
		$_SESSION["emenda"]["ptrid"] = $ptrid;
	}else{
		$ptrid = $_SESSION["emenda"]["ptrid"];
	}
	$obPTA->validaSessionPTA( $ptrid );
	$federal = $obEmenda->buscaEmendaDescentraliza( $ptrid );
	$obPTA->validaSessionPTA( $federal );
	$_SESSION['emenda']['federal'] = $federal;
}

unset($_SESSION['emenda']['dadosDetalheEntidade']);
$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptrid"] );
if( $_SESSION['emenda']['tpopup'] != 'popup' ){
	include  APPRAIZ."includes/cabecalho.inc";
} else {
	echo '<script type="text/javascript" src="/includes/funcoes.js"></script>
		  <script type="text/javascript" src="/includes/prototype.js"></script>
		  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		  <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
}
echo $obPTA->exibeInstituicaoBenefiada( $_SESSION["emenda"]["enbid"] );
//print "<br/>";

$sisid = '';

if($tipo == 'cadastro') {
	
	if($_SESSION["emenda"]["enbid"]){
		$enbid = $_SESSION["emenda"]["enbid"];
	}else{
		echo "<script>
				window.location.href = 'emenda.php?modulo=principal/listaInstituicoes&acao=A&retorno=definirRecursosPTA';
			  </script>";
		exit();
	}
	montaAbasPTA( $abacod_tela, $url, '', $estadoAtual );
	
	monta_titulo( 'Elaborar Plano de Trabalho', 'Selecione as emendas que deseja utilizar para criar um plano de trabalho (PTA) e informe o valor pretendido da contrapartida. Durante a elaboração do plano de trabalho, o valor informado da contrapartida poderá ser alterado.<br>
Obs.: Ao selecionar emendas de autores diferentes para compor um mesmo plano de trabalho, a aprovação deste ficará condicionada à concordância dos parlamentares.
	');
	echo $obPTA->cabecalhoPTADefinirRecurso($enbid);
} else {
	if($ptrid){
		$obPTA->verificaExistePTA($ptrid);				
		$arDadosPTA = $obPTA->carregaDadosDefinirRecursos( $ptrid );
		$arDadosPTA = $arDadosPTA ? $arDadosPTA : array(); 
		extract($arDadosPTA);
	}
	
	montaAbasPTA( $abacod_tela, $url, '', $estadoAtual, $sisid );
	monta_titulo( 'Recursos Orçamentários', '');
	echo $obPTA->cabecalhoPlanoTrabalho($ptrid);
}
if(empty($enbid)) $enbid = $db->pegaUm("select enbid from emenda.planotrabalho where ptrid = $ptrid");
$sql = "SELECT enbcertificado, enbcertificadovalido FROM emenda.entidadebeneficiada WHERE enbid = $enbid";
$arRegistro = $db->pegaLinha( $sql );
$arRegistro = $arRegistro ? $arRegistro : array();
extract($arRegistro);
$bocontrapartida = validaContraPartidaPTA( $enbid );

$arTema = $obPTA->NivelEnsinoBeneficiado( $ptrid, $enbid, $tipo );

if($tipo == 'cadastro') {
	echo montaGuiaPlanoTrabalho($arTema[0][0]['resid'], $enbid);
}

$arrPerfil = pegaPerfilArray($_SESSION['usucpf']);

/*$_SESSION["emenda"]["ptridpai"] = ($ptridpai ? $ptridpai : '');
$_SESSION["emenda"]["refid"] = ($refid ? $refid : '');*/

unset($_SESSION['emenda']['cadGuia']);

$etoid = $db->pegaUm("select e.etoid from emenda.v_emendadetalheentidade v 
								inner join emenda.emenda e on e.emeid = v.emeid
							where v.entid = $enbid");
$etoid = ( !empty($etoid) ? $etoid : '3');
if( $etoid == 4 ){
	$btnSalvarAcao = "salvarFederal";
} else {
	$btnSalvarAcao = "salvarPTA";
}
?>
<html>
<head>
<!-- <script type="text/javascript" src="/includes/funcoes.js"></script> -->
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="js/pta.js"></script>
<style>

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 100%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 300px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
</style>
</head>
<body>
<form id="formulario" name="formulario" action="#" method="post" enctype="multipart/form-data" >
	<div id="loader-container" style="display: none">
	   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
	</div>
	<input type="hidden" name="ptrid" id="ptrid" value="<?=$ptrid; ?>">
	<input type="hidden" name="enbid" id="enbid" value="<?=$enbid; ?>">
	<input type="hidden" name="ptrcod" id="ptrcod" value="<?=$ptrcod; ?>">
	<input type="hidden" name="sisid" id="sisid" value="<?=$sisid; ?>">
	<input type="hidden" name="retorno" id="retorno" value="">
	
	<input type="hidden" name="etoid" id="etoid" value="<?=$etoid ?>">
	
	<input type="hidden" name="guia_check" id="guia_check" value="">
	
	<input type="hidden" name="exercicio" id="exercicio" value="<?=$_SESSION['exercicio'];?>">
	<input type="hidden" name="enbcertificadovalido" id="enbcertificadovalido" value="<?=$enbcertificadovalido; ?>">
	<input type="hidden" name="enbcertificado" id="enbcertificado" value="<?=$enbcertificado; ?>">	
	<input type="hidden" name="bocontrapartida" id="bocontrapartida" value="<?=$bocontrapartida; ?>">
	
	<table align="center" id="recurso" class="Tabela" cellpadding="10" cellspacing="1">
	 <tbody>
		<tr>
			<td width="100%" style="text-align: left;" class="SubTituloEsquerda">Recursos do Concedente</td>
		</tr>
		<tr>
			<td width="100%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita" style="width:20%; text-align: right;" bgcolor="FFFFC4"><b>Nível de Ensino a ser Beneficiado:</b></td>
			<td style="text-align: left;">
			<?php
				echo $arTema[1];
				
				$hab = (empty($arTema[0]) ? 'N' : 'S');
				if($tipo == 'cadastro') {
					$obEmenda->buscaEmendaDescentraliza( '', $enbid, $arTema[0][0]['resid'] ? $arTema[0][0]['resid'] : $resid );
				}
			?>
			<input type="hidden" name="mdeid" id="mdeid" value="<?=$arTema[0][0]['mdeid']; ?>">
			<input type="hidden" name="resid" id="resid" value="<?=$arTema[0][0]['resid']; ?>">
			</td>
		</tr>
		<tr>
			<td colspan="2"></td>
		</tr>
	</table>
	<div id="listaEmenda"></div>
	</td>
	</tr>
 </tbody>
</table>
	<?
	if( $sisid == 23 ){ ?>
		 <table align="center" class="Tabela" cellpadding="10" cellspacing="1">
		 <tbody>
			<tr>
				<td width="100%" style="text-align: left;" class="SubTituloEsquerda">Emendas Vinculados as Subações do PAR</td>
			</tr>
		</tbody>
		<tr>
			<td colspan="2"><?
			$sql = "SELECT distinct
						'<span style=\"color:#0066cc;\">'||vede.emecod||'</span>' as emecod,
						par.retornacodigosubacao(s.sbaid) as codigo,
						s.sbadsc,
					    sep.sepvalor
					FROM
						par.subacaoemendapta sep
					    inner join par.subacaodetalhe sd on sd.sbdid = sep.sbdid
					    inner join par.subacao s on s.sbaid = sd.sbaid
					    inner join emenda.v_emendadetalheentidade vede on vede.emdid = sep.emdid
					WHERE
						sep.ptrid = $ptrid
						and sep.sepstatus = 'A'";
			
			$cabecalho = array ("Localizador", "Descrição", "Valor");			
			echo $db->monta_lista_grupo ( $sql, $cabecalho, 20, 5, '', '', '', '', 'emecod', '', true );
			?></td>
		</tr>
		<?
	} 
	?>
	<table align="center" class="Tabela" cellpadding="10" cellspacing="1">
	 <tbody>
	 	<?if( $_SESSION['emenda']['federal'] == 'N' && $etoid != 4 ){ ?>
		<tr>
			<td width="100%" style="text-align: left;" class="SubTituloEsquerda">Recursos do Proponente</td>
		</tr>
		<tr>
			<td width="100%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
		
		<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="SubTituloDireita" style="width:20%;"><b>Valor Total do Proponente:</b></td>
				<td><?=campo_texto( 'ptrvalorproponente', 'N', $hab, 'Valor Total do Proponente', 30, 13, '[###.]###,##', '','','','','id="ptrvalorproponente"', '', '', "this.value=mascaraglobal('[###.]###,##',this.value);"); ?>
				<input type="hidden" name="porcentagem_contrapartida" id="porcentagem_contrapartida" value="" />
				 <span id="porcentagem"></span>
				</td>
			</tr>
			<?			
			if( (in_array( ADMINISTRADOR_INST, $arrPerfil ) || in_array( SUPER_USUARIO, $arrPerfil )) && $etoid == 3 ){ ?>
			<tr>
				<td class="SubTituloDireita" style="width:20%;"><b>Rendimento de Aplicação:</b></td>
				<td><?=campo_texto( 'ptrrendimentoaplicacao', 'N', $hab, 'Rendimento de Aplicação', 30, 13, '[###.]###,##', '','','','','id="ptrrendimentoaplicacao"', '', '', "this.value=mascaraglobal('[###.]###,##',this.value);"); ?>
			</tr>
			<?
			}
			if( $_SESSION['exercicio'] >= '2011' && !empty($arTema[0]) && $etoid == 3 ){ ?>
			<tr>
				<td class="SubTituloDireita" style="width:20%;"><b>Possui Certificação de Entidade Beneficente – CEBAS?:</b></td>
				<td><input type="radio" onclick="verificaCertificado('S');" <?=($enbcertificado == 'S' ? 'checked="checked"' : ''); ?> name="enbcertificado_" id="enbcertificado_sim" value="S">Sim
					<input type="radio" onclick="verificaCertificado('N');" name="enbcertificado_" <?=($enbcertificado == 'N' ? 'checked="checked"' : ''); ?> id="enbcertificado_nao" value="N">Não</td>
			</tr>
			<?} ?>
		</table>
	  </td>
	</tr>
	<?} else {?>
		<tr>
			<td width="100%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
				<input type="hidden" name="ptrvalorproponente" id="ptrvalorproponente" value="0" />
				<input type="hidden" name="porcentagem_contrapartida" id="porcentagem_contrapartida" value="" />
				 <span id="porcentagem"></span></td>
		</tr>
		<?if( $_SESSION['exercicio'] >= '2011' && !empty($arTema[0]) && $etoid == 3 ){?>
		<tr>
			<td class="SubTituloDireita" style="width:20%;"><b>Possui Certificação de Entidade Beneficente – CEBAS?:</b></td>
			<td><input type="radio" onclick="verificaCertificado('S');" <?=($enbcertificado == 'S' ? 'checked="checked"' : ''); ?> name="enbcertificado_" id="enbcertificado_sim" value="S">Sim
				<input type="radio" onclick="verificaCertificado('N');" name="enbcertificado_" <?=($enbcertificado == 'N' ? 'checked="checked"' : ''); ?> id="enbcertificado_nao" value="N">Não</td>
		</tr>
		<?} ?>
	<?} ?>
	<tr>
		<?
		if( $sisid == 23 ){ 
			$disbButton = 'disabled="disabled"';
		}
		?>
		<td align="center" style="background-color:#c0c0c0;" colspan="2" id="tbButton">
			<? if ($tipo == 'cadastro') { ?>
				<input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" onclick="<?=$btnSalvarAcao?>('<?=$tipo?>');" <?=verificaPermissaoPerfil('pta'); echo (empty($arTema[0]) ? 'disabled="disabled"' : ''); echo $disbButton; ?>>				
				<input type="button" value="Voltar" name="btnVoltar" id="btnVoltar" onclick="history.back(-1);">
			<? } else { ?>	
				<input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" onclick="<?=$btnSalvarAcao?>('<?=$tipo?>');" <?=verificaPermissaoPerfil('pta', '', $estadoAtual, true); echo $disbButton; ?>>
				<input type="button" value="Voltar" name="btnVoltar" id="btnVoltar" onclick="history.back(-1);">
			<? }?>
		</td>		
		<td align="center" style="background-color:#c0c0c0; display: none;" colspan="2" id="tbVoltar">
			<input type="button" value="Voltar" name="btnVoltar" id="btnVoltar" onclick="history.back(-1);">
		</td>
	</tr>
 </tbody>
</table>
<input type="hidden" name="emdid" id="emdid" value="<?=$arTema[0][0]['emdid']; ?>">
</form>
<div id="erro"></div>
<div id="dialog_turma" title="Lista de Turmas" style="display: none; text-align: center;">
	<form name="formTurmaCad" id="formTurmaCad" action="" enctype="multipart/form-data" method="post">
		<div style="padding:5px;text-align:justify;" align="left" id="marcartodos"><input type="checkbox" name="todos" id="todos" value="" class="marcar-todos" marcar="check"><b>Selecionar Todos</b></div>
		<div style="padding:5px;text-align:justify;" id="mostraTurma"></div>
	</form>
</div>
</body>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

jQuery.noConflict();
jQuery(document).ready(function(){
	calculaVrlContraPartida();
});

function salvarPTA(tipo) {
	
	if( document.getElementById('sisid').value != '23' && document.getElementById('etoid').value != 1 ){
		
		if( $F('exercicio') >= '2011' && document.getElementById('etoid').value == '3' ){
			
			if( $F('enbcertificadovalido') == '' ){ 
				$('enbcertificado_nao').checked = true;
				verificaCertificado( 'N' );
			}
			if($F('porcentagem_contrapartida') == "" && ($F('bocontrapartida') == 'true' || $F('enbcertificado') == 'N') ) {
				alert('Não é possível criar este Plano de Trabalho pois não foi configurada a contrapartida\n              mínima do Proponente. Contacte o Administrador do Sistema.');
				return;
			}
		} else {
			if($F('porcentagem_contrapartida') == "" ) {
				alert('Não é possível criar este Plano de Trabalho pois não foi configurada a contrapartida\n              mínima do Proponente. Contacte o Administrador do Sistema.');
				return;
			}
		}
	
		var arDados = "";
		var check = false;
		
		for(i=0; i<formulario.length; i++){
			if(formulario.elements[i].id.indexOf('resp_') != -1 ){
			//if(formulario.elements[i].type == 'radio'){
				if(formulario.elements[i].checked){
					check = true;
				}
			}
		}	
		
		if(!check){
			alert('Selecione um nível de ensino!');
			return false;
		}
	
		for(i=0; i<formulario.length; i++) {
			if(formulario.elements[i].type == 'checkbox') {
				if(formulario.elements[i].checked && !formulario.elements[i].disabled ){
					if(arDados == "") {
						arDados = formulario.elements[i].value;
					} else {
						arDados = arDados + '|' +formulario.elements[i].value;
					}
				}
			}
		}
		
		if(tipo == 'cadastro') {
			if(!arDados) {
				alert('Nenhum recurso foi selecionada. Para prosseguir, clique na opção +, ao lado da(s) recurso(s) que deseja visualizar, e marque aquela(s) que pretende utilizar no plano de trabalho.');
				return false;
			}
		}
		
		$('ptrvalorproponente').value = (($F('ptrvalorproponente') != "") ? $F('ptrvalorproponente') : "0");
		
		var valorProponente 		= $F('ptrvalorproponente');
		var valoConcedente			= $F('vrlTotalEmenda');
		var porcentagemContrapartida= $F('porcentagem_contrapartida');
		//Tratando valores
		valorProponente 			= 	valorProponente.replace(/\./gi, '');
		valorProponente 			= 	Number(valorProponente.replace(/,/gi, "."));
		porcentagemContrapartida	= 	porcentagemContrapartida.replace(/,/, ".");
		var valorPTA 					= Number(valoConcedente) + Number(valorProponente);
		
		//Cálculo de porcentagem:
		var valorCalculoContrapartida = (100 - Number(porcentagemContrapartida));
		porcentagemContrapartida	=	(Number(porcentagemContrapartida)/100);
		var valorContrapartidaEsperada 	= valoConcedente / valorCalculoContrapartida
	
		//valorContrapartidaEsperada 		= (Number(valorContrapartidaEsperada) + 0.01);
		
		//var valorContrapartidaEsperada = (valorPTA/100) * porcentagemContrapartida;
		valorContrapartidaEsperada		= Number(valorContrapartidaEsperada.toString().substr(0, (valorContrapartidaEsperada.toString().search(/\./)+3)));
		
		if( $F('exercicio') >= '2011' && document.getElementById('etoid').value == '3' ){
			if(Number(valorProponente) < Number(valorContrapartidaEsperada) && $F('porcentagem_contrapartida') != "S" && ($F('bocontrapartida') == 'true' || $F('enbcertificado_nao') == 'N') ){
				//erro
				alert("Valor Total do Proponente menor do que a Contrapartida mínima exigida.\n\nMemória de cálculo:\n\nValor total do PTA = R$" + mascaraglobal('###.###.###.###,##', parseFloat($F('emdvalorTotal')).toFixed(2)) + " + R$" + $F('ptrvalorproponente') + "\nValor total do PTA = R$" + mascaraglobal('###.###.###.###,##', parseFloat(valorPTA).toFixed(2)) + "\n\nValor da contrapartida mínima = R$ " + mascaraglobal('###.###.###.###,##', parseFloat(valorContrapartidaEsperada).toFixed(2)) + " (" + $F('porcentagem_contrapartida') + "% do valor total do PTA)\nValor Informado = R$" + $F('ptrvalorproponente') + "");
				$('ptrvalorproponente').focus();
				return;
			}
		} else {
			if(Number(valorProponente) < Number(valorContrapartidaEsperada) && $F('porcentagem_contrapartida') != "S" ){
				//erro
				alert("Valor Total do Proponente menor do que a Contrapartida mínima exigida.\n\nMemória de cálculo:\n\nValor total do PTA = R$" + mascaraglobal('###.###.###.###,##', parseFloat($F('emdvalorTotal')).toFixed(2)) + " + R$" + $F('ptrvalorproponente') + "\nValor total do PTA = R$" + mascaraglobal('###.###.###.###,##', parseFloat(valorPTA).toFixed(2)) + "\n\nValor da contrapartida mínima = R$ " + mascaraglobal('###.###.###.###,##', parseFloat(valorContrapartidaEsperada).toFixed(2)) + " (" + $F('porcentagem_contrapartida') + "% do valor total do PTA)\nValor Informado = R$" + $F('ptrvalorproponente') + "");
				$('ptrvalorproponente').focus();
				return;
			}
		}
		
		if(valoConcedente == '0,00'){
			alert('O Valor Total do Concedente não pode ser zerado.');
			return false;
		}
		
		//if(arDados) {	
			if($('ptrvalorproponente').value == ""){
				alert('O campo Valor Total do Proponente é obrigatório!');
				$('ptrvalorproponente').focus();
				return false;
			}else if(parseInt($('ptrvalorproponente').value) <= 0 && $F('porcentagem_contrapartida') != "S" && ($F('bocontrapartida') == 'true' || $F('enbcertificado_nao') == 'N') ) {
				alert('A quantidade não pode se menor ou igual a 0!');
				$('ptrvalorproponente').focus();
				return false;
			}else{
				valor = $('ptrvalorproponente').value; 
				valor = valor.replace(/\./gi,"");
				valor = valor.replace(/\,/gi,".");
							
				var retornoGuia = verificaValorTotalGuia(valoConcedente, valorProponente);
				
				if( retornoGuia == 'false' ){
					$('loader-container').show();
					var myAjax = new Ajax.Request('emenda.php?modulo=principal/definirRecursosPTA&acao=A', {
								        method:     'post',
								        asynchronous: false,
								        parameters: '&insereEmendasDetalhe=true&dados=' + arDados + '&' + $('formulario').serialize(),
								        onComplete: function (res){
								        	if( res.responseText.indexOf('/') != -1 ){
								        		if($('ptrid').value == ""){
								        			alert('Dados do PTA inserido com sucesso! \nO número do seu PTA é ' + res.responseText)+'.';
								        		}else{
								        			alert('Dados do PTA alterado com sucesso!');
								        		}
								        		$('retorno').value = "false";
								        		//carregaEmendasDisponiveis($('resid').value, $('entid').value);
								        		$('btnSalvar').value = 'Salvar';
								        		window.location.href = 'emenda.php?modulo=principal/acoesPlanoTrabalho&acao=A';
								        	}else{
								        		if( res.responseText.length > 10 ){
								        			alert(res.responseText);
								        		} else {
								        			alert('Operação não realizada!');
								        		}
								        	}
											//$('erro').innerHTML = (res.responseText);
								        }
								  });
					$('loader-container').hide();
				}
			}
		} else {
			alert('Esse plano de trabalho foi elaborado pelo PAR - Plano de Metas, não sendo possivel realizar alterações.');
		}
}

function salvarFederal( tipo ){
	var arDados = "";
	var check = false;
	
	for(i=0; i<formulario.length; i++){
		if(formulario.elements[i].id.indexOf('resp_') != -1 ){
		//if(formulario.elements[i].type == 'radio'){
			if(formulario.elements[i].checked){
				check = true;
			}
		}
	}	
	
	if(!check){
		alert('Selecione um nível de ensino!');
		return false;
	}

	for(i=0; i<formulario.length; i++) {
		if(formulario.elements[i].type == 'checkbox') {
			if(formulario.elements[i].checked && !formulario.elements[i].disabled ){
				if(arDados == "") {
					arDados = formulario.elements[i].value;
				} else {
					arDados = arDados + '|' +formulario.elements[i].value;
				}
			}
		}
	}
	
	if(tipo == 'cadastro') {
		if(!arDados) {
			alert('Nenhum recurso foi selecionada. Para prosseguir, clique na opção +, ao lado da(s) recurso(s) que deseja visualizar, e marque aquela(s) que pretende utilizar no plano de trabalho.');
			return false;
		}
	}
	var valoConcedente	= document.getElementById('emdvalorTotal').value;
	
	if(valoConcedente == '0,00'){
		alert('O Valor Total do Concedente não pode ser zerado.');
		return false;
	}
	
	$('loader-container').show();
	var myAjax = new Ajax.Request('emenda.php?modulo=principal/definirRecursosPTA&acao=A', {
				        method:     'post',
				        asynchronous: false,
				        parameters: '&insereEmendasDetalhe=true&dados=' + arDados + '&' + $('formulario').serialize(),
				        onComplete: function (res){
				        	if( res.responseText.indexOf('/') != -1 ){
				        		if($('ptrid').value == ""){
				        			alert('Dados do PTA inserido com sucesso! \nO número do seu PTA é ' + res.responseText)+'.';
				        		}else{
				        			alert('Dados do PTA alterado com sucesso!');
				        		}
				        		$('retorno').value = "false";
				        		$('btnSalvar').value = 'Salvar';
				        		window.location.href = 'emenda.php?modulo=principal/acoesPlanoTrabalho&acao=A';
				        	}else{
				        		if( res.responseText.length > 10 ){
				        			alert(res.responseText);
				        		} else {
				        			alert('Operação não realizada!');
				        		}
				        	}
				        }
				  });
	$('loader-container').hide();
}

function carregaEmendasDisponiveis(valor, acao, enbid, emdid, resid){
	//$('loader-container').show();

	if(acao){
		$('resp_'+valor).checked = true;
		$('btnSalvar').value = 'Salvar';	
	}	
	$('mdeid').value = valor;
	$('resid').value = resid;
	
	var myAjax = new Ajax.Request('emenda.php?modulo=principal/definirRecursosPTA&acao=A', {
				        method:     'post',
				        asynchronous: false,
				        parameters: '&carregaEmendasDisponiveis=true&mdeid='+valor,
				        onComplete: function (res){
							$('listaEmenda').update(res.responseText);
				        }
				  });
		  
	if( emdid == '' ){
		$('btnSalvar').disabled= true;
	} else {
		if( $F('exercicio') >= '2011' && document.getElementById('etoid').value == '3' ){
			if( $F('enbcertificadovalido') == '' ){ 
				$('enbcertificado_nao').checked = true;
				verificaCertificado( 'N' );
			}
			
			if( $F('bocontrapartida') == 'true' || $F('enbcertificado') == 'N' )
				carregaPorcentagemContrapartida(valor,enbid, emdid);
		} else {
			carregaPorcentagemContrapartida(valor,enbid, emdid);
		}
	}
	//$('loader-container').hide();
}
 
function carregaPorcentagemContrapartida(mdeid, enbid, emdid) {

	var myAjax = new Ajax.Request('emenda.php?modulo=principal/definirRecursosPTA&acao=A', 
	{
        method: 'post',
        parameters: '&carregaPorcentagemContrapartida=true&mdeid='+mdeid+'&enbid='+enbid+'&emdid='+emdid,
        asynchronous: false,
        onComplete: function (res) {
        	if(res.responseText == 'erro') {
        		$('porcentagem_contrapartida').value = '';
				$('porcentagem').update('Contrapartida mínima: Nenhuma contrapartida mínima configurada.');
        	} else if( res.responseText == 'federal' ){
        		$('porcentagem_contrapartida').value = 'S';
        		$('porcentagem').update('');
        	} else {
	        	$('porcentagem_contrapartida').value = res.responseText;
				$('porcentagem').update('Contrapartida mínima:' + res.responseText + '%');
				if(document.getElementById('etoid').value == '4') $('porcentagem').style.display = 'none';
			}
        }
	});
}
 
function verificaExistenciaEmenda(resid){
	var myAjax = new Ajax.Request('emenda.php?modulo=principal/definirRecursosPTA&acao=A', {
				        method:     'post',
				        parameters: '&verificaExistenciaEmenda=true&resid='+resid,
				        onComplete: function (res){	
							if(Number(res.responseText) > 0){
								$('retorno').value = "true";
							}else{
								$('retorno').value = "false";
							}
				        }
				  });
}

function somaEmendaDetalhe(check, valor, emeid, boTipoOrigem, ano){
	var soma = 0;
	
	if( (boTipoOrigem == 0 || boTipoOrigem == 1) && ano >= '2014' ){
		var msg = 'Emenda sem tipo de origem';
		if( boTipoOrigem == 1 ) var msg = 'Emenda de origem PAR';
		
		alert(msg);		
		$(check.id).checked = false;
	} else {	
		if(check.checked){
			var total = $('emdvalorTotal').value;
			soma = parseFloat(valor) + parseFloat(total);
			$('emdvalorTotal').value = soma;
			$('emdvalorTotalDiv').innerHTML = 'R$ ' + mascaraglobal('###.###.###.###,##', soma.toFixed(2));
		} else {
			var total = $('emdvalorTotal').value;
			soma = parseFloat(total) - parseFloat(valor);
			$('emdvalorTotal').value = soma;
			$('emdvalorTotalDiv').innerHTML = 'R$ ' + mascaraglobal('###.###.###.###,##', soma.toFixed(2));
		}
	}
	calculaVrlContraPartida();
}

function calculaVrlContraPartida(){
	var valor = 0;
	jQuery('[name="vlrrecurso[]"]').each(function(){
		var arRecurso = jQuery(this).attr('id').split('_');
		var emdid = arRecurso[1];
		
		if( jQuery('[name="emdid_'+emdid+'"]').attr('checked') == true ){
			valor = parseFloat( valor ) + parseFloat( jQuery(this).val() );
		}
	});
	
	jQuery('[name="vrlTotalEmenda"]').val(valor);
}

function carregaDetalheEmenda(idImg, emeid, div_nome, tr_nome){	
	var img = document.getElementById( idImg );

	if($(div_nome).style.display == 'none'){
		$(div_nome).style.display = '';
		$(tr_nome).style.display = '';
		img.src = '../imagens/menos.gif';
	} else {
		$(div_nome).style.display = 'none';
		$(tr_nome).style.display = 'none';
		img.src = '/imagens/mais.gif';
	}
}

function verificaValorTotalGuia(valoConcedente, valorProponente){
	var boretorno = '';
	var myAjax = new Ajax.Request('emenda.php?modulo=principal/definirRecursosPTA&acao=A', {
						        method:     'post',
						        asynchronous: false,
						        parameters: '&verificaValorGuia=true&' + $('formulario').serialize(),
						        onComplete: function (res){
						        
						        	var retornoGuia = res.responseText;
						        	var valorPta = parseFloat(valoConcedente) + parseFloat(valorProponente);
						        	
									//$('erro').innerHTML = res.responseText;
						        	if( retornoGuia > valorPta ){
						        		alert('Não é possivel criar o plano de trabalho!\nValor proposto no guia R$ '+mascaraglobal('###.###.###.###,##', parseFloat(retornoGuia).toFixed(2))+' é maior que o valor disponivél R$ '+mascaraglobal('###.###.###.###,##', parseFloat(valorPta).toFixed(2))+' para a criação do seu plano de trabalho.');
						        		boretorno = 'true';
						        	}else{
						        		boretorno = 'false';
						        	}
						        }
						  });
	return boretorno;
}

function verificaCertificado( check ){
	window.document.getElementById('enbcertificado').value = check
	var enbcertificadovalido = window.document.getElementById('enbcertificadovalido').value;
	if( check == 'S' ){
		if( $F('bocontrapartida') == 'false' && $F('enbcertificado') != 'N' ){
			$('porcentagem_contrapartida').value = 'S';
	        $('porcentagem').update('');
        }
		var janela = window.open("emenda.php?modulo=principal/popupMostraCertificado&acao=A&enbcertificado="+check+"&enbcertificadovalido="+enbcertificadovalido,"certificado", "scrollbars=yes,height=400,width=500");
		janela.focus();
	} else {
		var resid = window.document.getElementById('mdeid').value;
		var enbid = window.document.getElementById('enbid').value;
		var emdid = window.document.getElementById('emdid').value;
		
		carregaPorcentagemContrapartida(resid, enbid, emdid);
	}
}
</script>
<?
	if ( count($arTema[0]) == 1 && !empty($arTema[0]) ){
		echo "<script>carregaEmendasDisponiveis({$arTema[0][0]['mdeid']}, '', {$enbid}, {$arTema[0][0]['emdid']}, {$arTema[0][0]['resid']});</script>";
	}
	if($arDadosPTA){
		echo "<script>carregaEmendasDisponiveis($mdeid, 'altera', $enbid, {$arTema[0][0]['emdid']}, $resid);</script>";
	}
	
if( $tipo == 'cadastro' ){
	
	if( $etoid == 3){
		$obEntidade = new entidadeFNDE();
		$entnumcnpjcpf = $obEntidade->pegaCnpj( $_SESSION["emenda"]["enbid"] );
		$obEntidade  = $obEntidade->buscaEntidadeBaseFNDE_WS( $entnumcnpjcpf );
		
		if( !is_object( $obEntidade ) ){
			$texto = "<center>
				<img src=\"../imagens/alerta_sistema.gif\" />
			    <p><font size=3 color=red><b>Nota do Sistema!</b></font></p>
           	     <p style=\"font-weight:bold\" ><font size=3>ATENÇÃO<br>
Entidade não cadastrada no FNDE.<br> 
Para efetuar o cadastramento é necessário o encaminhamento da documentação constante na Resolução nº 10 de 2012, em conformidade com a natureza jurídica da entidade, para a Coordenação de Habilitação de Projetos – COHAP/FNDE.<br>
Endereço: SBS Quadra 02 Bloco “F” Ed. FDNE – Sobreloja Sala 03<br>
CEP: 70-070-929  Brasília/DF<br> 
Telefone: (61) 2022.4294<br><br>

IMPORTANTE<br>
O cadastramento da entidade não implica a celebração do convênio, mas é condição indispensável para tanto.</p>
           	     <input type=\"button\" value=\"Fechar\" style=\"cursor:pointer;font:16px Trebuchet Ms,Arial,Tahoma,Verdana,Helvetica,Sans-Serif;height:33px\" onclick=\"document.getElementById('id_popup_alerta').style.display='none';\" />
          	  </center>";
		popupAlertaGeral($texto,"680px","430px");
		echo "<script>
				$('btnSalvar').disabled = true;
				$('tbButton').style.display = 'none';
				$('tbVoltar').style.display = '';
			</script>";
		}
	}
	
	$sql = "";
}
?>
</html>
