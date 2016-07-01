<?php
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

verificaPermissao();

switch( $_REQUEST["requisicao"] ){
	case "buscaagencia":
			if($_REQUEST["bcoid"] == '1'){
				$banco = '001';
			}else{
				$banco = '104';
			}
			 $sql ="SELECT
							codagencia ||'-'|| dv_agencia as codigo,
							'(' || codagencia || ' - ' || dv_agencia || ') - ' || nomeagencia as descricao
					FROM
							financeiro.bancoagencia
					WHERE 
							codbanco = '{$banco}' AND 
							siglauf =	'{$_REQUEST["estuf"]}'
					ORDER BY
							codagencia ASC"; 
			 
			$db->monta_combo( "codagencia", $sql, 'S', 'Selecione...', '', '', '', '215', 'N','codagencia','',$codagencia);
		
		die;
		
	break;
}

		//Ajax
		if ($_REQUEST['filtraEmendaAjax'] && $_REQUEST['proid']) {
			header('content-type: text/html; charset=ISO-8859-1');
			filtraEmendas($_REQUEST['proid']);
			exit;
		}

$enbid = $_SESSION["emenda"]["enbid"];

$ptrid = $_SESSION["emenda"]["ptrid"];

if( !verificaExistePTA($_SESSION["emenda"]["ptrid"]) ){
	echo "<script>
			alert('Número do Plano de Trabalho inválido.');
			window.location.href = 'emenda.php?modulo=principal/listaPlanoTrabalho&acao=A';
		  </script>";
	die;
}

if($_REQUEST["submetido"]) {
	$emeid = (($_REQUEST["emeid"] != "") ? $_REQUEST["emeid"] : 'NULL');
	$ptrjustificativa = (($_REQUEST["ptrjustificativa"] != "") ? stripcslashes(pg_escape_string( str_replace( "'", '"', $_REQUEST["ptrjustificativa"]))) : 'NULL');
	$bcoid = (($_REQUEST["bcoid"] != "") ? $_REQUEST["bcoid"] : 'NULL');
	$ptrtipodirigente = (($_REQUEST["dirigente_representante"] != "") ? $_REQUEST["dirigente_representante"] : 'NULL');
	$entiddirigente = (($_REQUEST["entid_dirigente_representante"] != "") ? $_REQUEST["entid_dirigente_representante"] : 'NULL');
	$ptragenciabancaria = (($_REQUEST["codagencia"] != "") ? $_REQUEST["codagencia"] : 'NULL');
	$carid				= (($_REQUEST["carid"] != "") ? $_REQUEST["carid"] : 'NULL');
	
	$sql = "UPDATE 
				emenda.planotrabalho
			SET
				ptrjustificativa 	=	'" . $ptrjustificativa . "',
				ptrtipodirigente 	=	" . $ptrtipodirigente . ",
				entiddirigente 		= 	" . $entiddirigente .",
				bcoid				= 	" . $bcoid .",
				ptragenciabancaria	= 	'" . $ptragenciabancaria . "',
				carid				= 	" . $carid . "
			WHERE
				ptrid				=	" . $_SESSION["emenda"]["ptrid"] . "";

	$db->executar($sql);

	$xml  = "";
	$xml .= "<ptrjustificativa>$ptrjustificativa</ptrjustificativa>";
	$xml .= "<ptrtipodirigente>$ptrtipodirigente</ptrtipodirigente>";
	$xml .= "<entiddirigente>$entiddirigente</entiddirigente>";
	$xml .= "<bcoid>$bcoid</bcoid>";
	$xml .= "<ptragenciabancaria>$ptragenciabancaria</ptragenciabancaria>";
	$xml .= "<carid>$carid</carid>";

	$sql = "INSERT INTO 
				emenda.historicogeral
				(
					tabid,
					histidtabela,
					histtxtalteracao,
					histdatainclusao,
					usucpfinclusao,
					ptrid
				)
			VALUES
				(
					'1',
					'{$_SESSION[emenda][ptrid]}',
					'{$xml}',
					NOW(),
					'{$_SESSION[usucpf]}',
					'{$_SESSION[emenda][ptrid]}'
				)";

	$db->executar($sql);	
	
	
	$db->commit();
	$db->sucesso("principal/alterarPlanoTrabalho");
}

if( $_SESSION['emenda']['tpopup'] != 'popup' ){
	include  APPRAIZ."includes/cabecalho.inc";
} else {
	echo '<script type="text/javascript" src="/includes/funcoes.js"></script>
		  <script type="text/javascript" src="/includes/prototype.js"></script>
		  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		  <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
}
print "<br/>";
validaSessionPTA( $_SESSION['emenda']['federal'] );

$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptrid"] );

echo exibeInstituicaoBenefiada( $_SESSION["emenda"]["enbid"] );
print "<br/>";
montaAbasPTA( $abacod_tela, $url, '', $estadoAtual );

monta_titulo( 'Descrição do Projeto', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório');

// Recupera os dados do Plano de Trabalho
$sql = "SELECT 
			ptr.ptrjustificativa,
			ptr.ptrtipodirigente,
			ptr.entiddirigente,
			ptr.bcoid,
			ptr.ptragenciabancaria,
			ptr.carid
		FROM
			emenda.planotrabalho ptr
		WHERE
            ptr.ptrstatus = 'A' 
            AND ptr.ptrid = ".$_SESSION["emenda"]["ptrid"];
$dadosPlanoTrabalho = $db->carregar($sql);

$ptragenciabancaria = $dadosPlanoTrabalho[0]["ptragenciabancaria"];
		// Junta Digito da Agencia Bancaria, com o Código da Agencia
		if (strlen($ptragenciabancaria) <= 4 ){
			$codagencia  = $ptragenciabancaria;
			$sql="SELECT DISTINCT
			 				dv_agencia
			 	  FROM 
			 	  			financeiro.bancoagencia
			 	  WHERE
			 	  			codagencia = '{$codagencia}'";
			$dv_complemento = $db->carregar($sql);
			$dv_agencia = $dv_complemento[0]["dv_agencia"];
			
		}
		else
		{
		// Separa o código e o dígito da agência
			if(  strpos($ptragenciabancaria, "-") === false ) {
			$codagencia  = substr($ptragenciabancaria, 0, 4);
			$dv_agencia	 = substr($ptragenciabancaria, 4, 1);
			} else {
			$codagencia  = substr($ptragenciabancaria, 0, 4);
			$dv_agencia	 = substr($ptragenciabancaria, 5, 1);
			}

		}
				
if( $dadosPlanoTrabalho[0]["bcoid"] === null ){
	$sql = "SELECT DISTINCT
				bco.codbanco,
				bco.siglauf
			FROM
				financeiro.bancoagencia bco";
}else{
	
	$sql = "SELECT DISTINCT
				bco.codbanco,
				bco.siglauf,
				bc.bcoid
			FROM
				financeiro.bancoagencia bco
			INNER JOIN
				emenda.banco bc ON bc.bcoid = {$dadosPlanoTrabalho[0]["bcoid"]}
							   AND bc.bcocod = bco.codbanco
			WHERE
				bco.codagencia = '{$codagencia}'
			";
}
	
	$dadosBancoAgencia = $db->carregar($sql);
	
	echo cabecalhoPlanoTrabalho($ptrid);

?>

<form id="formAlteracaoPlanoTrabalho" method="post" action="">
<input type="hidden" name="submetido" value="1" />
<input type="hidden" name="preid" value="1" />
<input type="hidden" name="ptrid" id="ptrid" value="<?=$_SESSION["emenda"]["ptrid"];?>">
<input type="hidden" name="federal" id="federal" value="<?=$_SESSION['emenda']['federal'];?>">

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td colspan="2"><b>Dados Adicionais do PTA</b></td>
	</tr> 
	<tr>
		<td class="SubTituloDireita" valign="middle"><b>Justificativa do Projeto:</b></td>
		<td>
			<?
				$ptrjustificativa = $dadosPlanoTrabalho[0]["ptrjustificativa"];
				echo campo_textarea('ptrjustificativa', 'S', 'S', '', 100, 10, 4000);
				
			?>
		</td>
	</tr>
	<!-- <tr>
		<td class="SubTituloDireita" valign="middle"><b>CPF do Dirigente ou Representante Legal:</b></td>
		<td>
			<input type="radio" name="dirigente_representante" value="D" <?/*=(($dadosPlanoTrabalho[0]["ptrtipodirigente"]=="D" || !$dadosPlanoTrabalho[0]["ptrtipodirigente"]) ? "checked=\"checked\"" : "")?> /> Dirigente
			<input type="radio" name="dirigente_representante" value="R" <?=(($dadosPlanoTrabalho[0]["ptrtipodirigente"]=="R") ? "checked=\"checked\"" : "")?> /> Representante Legal
			<br /><br />
			&nbsp;
			<?if(!disabledPTA( $_SESSION['emenda']['ptrid'] )){ ?>
			<a href="#" onclick="popUpIncluirCPF();" style="cursor:pointer">
				<label id="labelCPF"><img src="../imagens/gif_inclui.gif" border="0">
				Clique aqui para incluir o CPF</label>
			</a>
			<?} else { ?>
				<span style="cursor:pointer">
				<label id="labelCPF"><img src="../imagens/gif_inclui_d.gif" border="0">
				Clique aqui para incluir o CPF</label>
			</span>
			<?} ?>
			<br /><br />
			&nbsp;
			<?
				// Preenchendo os campos com os dados relativos ao 
				// 'Dirigente / Representante Legal' cadastrado para o Plano de Trabalho
				/*if($dadosPlanoTrabalho[0]["entiddirigente"]) {
					$usucpfdirigente  = 	$db->pegaUm("SELECT entnumcpfcnpj FROM entidade.entidade WHERE entid = ".$dadosPlanoTrabalho[0]["entiddirigente"]."");
				}
                //$dadosPlanoTrabalho[0]["entiddirigente"];
				
				if($dadosPlanoTrabalho[0]["entiddirigente"]) {
					$usunome						= 	$db->pegaUm("SELECT entnome FROM entidade.entidade WHERE entid = '".$dadosPlanoTrabalho[0]["entiddirigente"]."'");
				}
				$entid_dirigente_representante	=	$dadosPlanoTrabalho[0]["entiddirigente"];//$db->pegaUm("SELECT entid   FROM entidade.entidade WHERE entnumcpfcnpj = '".$usucpfdirigente."'");
			?>
			<input type="hidden" name="entid_dirigente_representante" value="<?=$entid_dirigente_representante?>" />
			<?=campo_texto( 'usucpfdirigente', 'S', 'N', '', 20, 20, '', '', '', '', 0, '' );?>
			<br /><br />
			&nbsp;
			<?=campo_texto( 'usunome', 'N', 'N', '', 60, 100, '', '', '', '', 0, '','',$usunome); ?><br /><br />
			<?
			$carid = $dadosPlanoTrabalho[0]["carid"];
			$sql = "SELECT 
						c.carid as codigo,
						f.fundsc as descricao
					FROM 
						emenda.cargos c
					    inner join entidade.funcao f
					    	on f.funid = c.funid
					        and f.funstatus = 'A'
					ORDER BY
						f.fundsc";
			?>
			Cargo: <?=$db->monta_combo( "carid", $sql, 'S', 'Selecione...', '', '', '', '315', 'S','carid','',$carid);*/ ?>
		</td>
	</tr> -->
	<?if( $_SESSION['emenda']['federal'] == 'N' ){ ?>
	<tr>
		<td class="SubTituloDireita" valign="middle"><b>Banco e Agência onde a conta<br>do convênio deverá ser criada</b></td>
		<td>
			<?
			$bcoid = $dadosBancoAgencia[0]["bcoid"];	
			$sql = "SELECT 
	    					bcoid as codigo, 
	    					bconome as descricao
	    					    					
	    			FROM 
	    					emenda.banco
	    			ORDER BY
	    					bconome ASC";
			?>
			Banco: <?=$db->monta_combo( "bcoid", $sql, 'S', 'Selecione...', "emeBuscaAgencia(document.getElementById('bcoid').value, this.value);", '', '', '215', 'S','bcoid','',$bcoid);?>
			<br /><br />
			<? 
			if ($bcoid <> "" ){
				$estuf = $dadosBancoAgencia[0]["siglauf"];	
			}
			$sql = "SELECT 
	    					estuf as codigo, 
	    					estuf as descricao
	    			FROM 
	    					territorios.estado
	    			ORDER BY
	    					estuf ASC";
			?>
			UF: <?=$db->monta_combo( "estuf", $sql, 'S', 'Selecione...', "emeBuscaAgencia(document.getElementById('bcoid').value, this.value);" , '', '', '100', 'S','estuf','',$estuf);
			?>
			<br /><br />
			Agência:
			<span style="color: #BEBEBE;" id="agencia"> 
			<?php 
			if( !$ptragenciabancaria ){ ?>
			
				Selecione o Banco e a Unidade Federativa
			
			<?php 
			 }else{ 
			 
			 	if($dadosBancoAgencia[0]["bcoid"] == '1'){
					$banco = '001';
				}else{
					$banco = '104';
				}
			 	$sql = "SELECT
							codagencia ||'-'|| dv_agencia as codigo,
							'(' || codagencia || ' - ' || dv_agencia || ') - ' || nomeagencia as descricao
					FROM
							financeiro.bancoagencia
					WHERE 
							codbanco = '{$banco}' AND
							siglauf =	'{$estuf}'
					ORDER BY
							codagencia ASC";

				$codagencia = $codagencia . '-' . $dv_agencia;
				
				$db->monta_combo( "codagencia", $sql, 'S', 'Selecione...', '', '', '', '215', 'S','codagencia','',$codagencia);
			}		
				?>
			</span>
		</td>
	</tr>
	<?} ?>
	<tr>
		<td bgcolor="#c0c0c0"></td>
		<td align="left" bgcolor="#c0c0c0">
			<input type="button" id="bt_alterar" value="Salvar" onclick="alterarPlanoTrabalho()" <?=verificaPermissaoPerfil( 'pta', '', $estadoAtual, true ); ?> />
			&nbsp;
			<input type="button" id="bt_cancelar" value="Cancelar" onclick="cancelarPlanoTrabalho()" />
		</td>
	</tr>
</table>
</form> 

<script type="text/javascript" src="/includes/prototype.js"></script>	
<script type="text/javascript">
// formata o CNPJ
/*var cnpj	=	document.getElementsByName("entnumcpfcnpj")[0];
cnpj.value	=	mascaraglobal('##.###.###/####-##', cnpj.value);*/

/*var cpf		=	document.getElementsByName("usucpfdirigente")[0];
cpf.value	=	mascaraglobal('###.###.###-##', cpf.value);*/

var btAlterar	= document.getElementById("bt_alterar");
var btCancelar	= document.getElementById("bt_cancelar");


function emeBuscaAgencia( bcoid, estuf ){
	estuf = document.getElementById('estuf').value;
	if( estuf != '' ){
		var url = '?modulo=principal/alterarPlanoTrabalho&acao=A&requisicao=buscaagencia&bcoid=' + bcoid + '&estuf=' + estuf;
			
		var myAjax = new Ajax.Updater(
			"agencia",
			url,
			{
				method: 'post',
				asynchronous: false
			});
	} else {
		document.getElementById('estuf').value = '';
		document.getElementById('codagencia').value = '';
	}
}

function alterarPlanoTrabalho() {
	var form				= 	document.getElementById("formAlteracaoPlanoTrabalho");
	var bcoid				=	document.getElementsByName("bcoid")[0];
	//var usucpfdirigente		=	document.getElementsByName("usucpfdirigente")[0];
	var ptrjustificativa	=	document.getElementsByName("ptrjustificativa")[0];
	var ptragenciabancaria	=	document.getElementsByName("codagencia")[0];
	var estuf				=	document.getElementsByName("estuf")[0];
	var federal				=	document.getElementsByName("federal")[0];
	
	if (trim( ptrjustificativa.value ) == '') {
		alert("O campo 'Justificativa do Projeto' deve ser informado.");
		ptrjustificativa.focus();
    	return false;
	}
	if( federal.value == 'N' ){
		if (trim( bcoid.value ) == '') {
			alert("O campo 'Banco' deve ser informado.");
			bcoid.focus();
	    	return false;
		}
		if (trim( estuf.value ) == '') {
			alert("O campo 'UF' deve ser informado.");
			estuf.focus();
	    	return false;
		}
		if (trim( ptragenciabancaria.value ) == '') {
			alert("O campo 'Agência' deve ser informado.");
			ptragenciabancaria.focus();
	    	return false;
		}
	}
	form.submit();
}

function cancelarPlanoTrabalho() {
	/*btAlterar.disabled 	= true;
	btCancelar.disabled = true;*/
	window.location = "emenda.php?modulo=principal/listaPlanoTrabalho&acao=A";
}

/*function popUpIncluirCPF() {
	var entid = document.getElementsByName("entid_dirigente_representante")[0];
	
	var janela = window.open( 'emenda.php?modulo=principal/popInsereDirigenteRepresentante&acao=A&entid=' + entid.value + '', 'popup_cpf', 'width=800,height=700,status=0,menubar=0,toolbar=0,scrollbars=1,resizable=0' );
	janela.focus();
}*/
function filtraEmendas(proid) {
	if(!proid)
		//return false;
		params = "filtraEmendaAjax=true&proid=999";
	else
		params = "filtraEmendaAjax=true&" + "proid=" + proid;
	
	var destino = document.getElementById("td_emendas");
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: params,
			asynchronous: false,
			onComplete: function(resp) {
				if(destino) {
					destino.innerHTML = resp.responseText;
				} 
			},
			onLoading: function(){
				destino.innerHTML = 'Carregando...';
			}
		});
}

</script>