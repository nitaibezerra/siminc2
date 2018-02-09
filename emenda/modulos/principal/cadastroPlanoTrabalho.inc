<?php
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");


//Ajax
if ($_REQUEST['filtraEmendaAjax'] && $_REQUEST['proid']) {
	header('content-type: text/html; charset=ISO-8859-1');
	filtraEmendas($_REQUEST['proid']);
	exit;
}

if($_SESSION["emenda"]["entid"]){
	$entid = $_SESSION["emenda"]["entid"];
}else{
	echo "<script>
			window.location.href = 'emenda.php?modulo=principal/listaInstituicoes&acao=A';
		  </script>";
	exit();
}

if($_REQUEST["submetido"]) {
	$sql = "INSERT INTO
				emenda.planotrabalho(entid,
									 ptrexercicio,
									 ptrjustificativa,
									 ptrtipodirigente,
									 entiddirigente,
									 ptrstatus)
			VALUES
				(" . $entid . ",
				 " . $_SESSION["exercicio"] . ",
				 " . (($_REQUEST["ptrjustificativa"] != "") ? "'".pg_escape_string($_REQUEST["ptrjustificativa"])."'" : 'NULL') . ",
				 '" . $_REQUEST["dirigente_representante"] . "',
				 '" . $_REQUEST["entid_dirigente_representante"] . "',
				 'A')
			RETURNING
				ptrid"; 
	$ptrid = $db->pegaUm($sql);
	
	$db->commit();
	echo "<script type=\"text/javascript\">
			alert(\"Plano de Trabalho gravado com sucesso.\");
			window.location = \"emenda.php?modulo=principal/alterarPlanoTrabalho&acao=A&ptrid=$ptrid\";
		  </script>";
	die;
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Descrição do Projeto', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório');

$sql = "SELECT 
			ent.entnumcpfcnpj,
			ent.entnome,
			mun.mundescricao,
			est.estuf
		FROM
			entidade.entidade ent
		INNER JOIN
			entidade.endereco ende ON ende.entid = ent.entid
		INNER JOIN
			territorios.municipio mun ON mun.muncod = ende.muncod
		INNER JOIN
			territorios.estado est ON est.estuf = mun.estuf
		WHERE
			ent.entstatus = 'A' AND
			ende.endstatus = 'A' AND
			ent.entid = ".$entid;

$dadosPlanoTrabalho = $db->carregar($sql);

?>

<form id="formCadastroPlanoTrabalho" method="post" action="">
<input type="hidden" name="submetido" value="1" />
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top"><b>CNPJ:</b></td>
		<td>
		<?
			$entnumcpfcnpj = $dadosPlanoTrabalho[0]["entnumcpfcnpj"];
			echo campo_texto( 'entnumcpfcnpj', 'N', 'N', '', 40, 40, '', '', '', '', 0, '' );
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top"><b>Nome do Órgão ou Entidade:</b></td>
		<td>
			<?
				$entnome = $dadosPlanoTrabalho[0]["entnome"];
				echo campo_texto( 'entnome', 'N', 'N', '', 80, 20, '', '', '', '', 0, '' );
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top"><b>Município:</b></td>
		<td>
			<?
				$mundescricao = $dadosPlanoTrabalho[0]["mundescricao"];
				echo campo_texto( 'mundescricao', 'N', 'N', '', 40, 40, '', '', '', '', 0, '' );
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top"><b>UF:</b></td>
		<td>
			<?
				$estuf = $dadosPlanoTrabalho[0]["estuf"];
				$sql = "SELECT
							estuf AS codigo,
							estdescricao AS descricao
						FROM
							territorios.estado";
				$db->monta_combo( "estuf", $sql, 'N', 'Selecione...', '', '', '', '215' );
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top"><b>Exercício:</b></td>
		<td>
			<?=$_SESSION["exercicio"]?>
		</td>
	</tr>
	<!-- <tr>
		<? 
			$sql = "SELECT 
						eme.emeid AS codigo,
						eme.emecod AS descricao
					FROM
						emenda.emenda eme
					/*WHERE
						eme.emeexercicio = 0*/";
		?>
		<td class="SubTituloDireita" valign="top"><b>Emenda N°:</b></td>
		<td id="td_emendas">
			<?	$db->monta_combo( "emeid", $sql, 'N', 'Selecione...', '', '', '', '215', 'N');?>
		</td>
	</tr>-->
	<tr>
		<td class="SubTituloDireita" valign="middle"><b>Justificativa do Projeto:</b></td>
		<td>
			<?=campo_textarea('ptrjustificativa', 'N', 'S', '', 100, 10, 4000);?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="middle"><b>CPF do Dirigente ou Representante Legal:</b></td>
		<td>
			<input type="radio" name="dirigente_representante" value="D" checked="checked" /> Dirigente
			<input type="radio" name="dirigente_representante" value="R" /> Representante Legal
			<br /><br />
			&nbsp;
			<a href="#" onclick="popUpIncluirCPF();" style="cursor:pointer">
				<div id="labelCPF"><img src="../imagens/gif_inclui.gif" border="0">
				Clique aqui para incluir o CPF</div>
			</a>
			<br /><br />
			&nbsp;
			<input type="hidden" name="entid_dirigente_representante" value="" />
			<?= campo_texto( 'usucpfdirigente', 'S', 'N', '', 20, 20, '', '', '', '', 0, '' ); ?>
			<br /><br />
			&nbsp;
			<?= campo_texto( 'usunome', 'N', 'N', '', 60, 100, '', '', '', '', 0, '' ); ?>
		</td>
	</tr>
	<tr>
		<td bgcolor="#c0c0c0"></td>
		<td align="left" bgcolor="#c0c0c0">
			<input type="button" id="bt_salvar" value="Salvar" onclick="javascript:salvarPlanoTrabalho();" />
			&nbsp;
			<input type="button" id="bt_cancelar" value="Cancelar" onclick="javascript:cancelarPlanoTrabalho();" />
		</td>
	</tr>
</table>
</form>

<script type="text/javascript" src="/includes/prototype.js"></script>	
<script type="text/javascript">
	
if(document.getElementsByName("usucpfdirigente")[0].value != ""){
	window.document.getElementById('labelCPF').innerHTML = '<img src=\"../imagens/gif_inclui.gif\" border=\"0\"> Clique aqui para alterar o CPF';
}

// formata o CNPJ
var cnpj	=	document.getElementsByName("entnumcpfcnpj")[0];
cnpj.value	=	mascaraglobal('##.###.###/####-##', cnpj.value);

var btSalvar	= document.getElementById("bt_salvar");
var btCancelar	= document.getElementById("bt_cancelar");

function salvarPlanoTrabalho() {
	var form			= 	document.getElementById("formCadastroPlanoTrabalho");
	//var proid			=	document.getElementsByName("proid")[0];
	var usucpfdirigente	=	document.getElementsByName("usucpfdirigente")[0];
	
	btSalvar.disabled 	= true;
	btCancelar.disabled = true;
	
	/*if(proid.value == "") {
		alert("O campo 'Abrangência do Projeto / Programa' deve ser preenchido.");
		proid.focus(); 
		btSalvar.disabled 	= false;
		btCancelar.disabled = false;
		return;
	}*/
	if(usucpfdirigente.value == "") {
		alert("O 'CPF do Dirigente ou Representante Legal' deve ser informado.");
		btSalvar.disabled 	= false;
		btCancelar.disabled = false;
		return;
	}

	form.submit();
}

function cancelarPlanoTrabalho() {
	btSalvar.disabled 	= true;
	btCancelar.disabled = true;
	window.location = "emenda.php?modulo=principal/listaPlanoTrabalho&acao=A";
}

function popUpIncluirCPF() {
	var entid = document.getElementsByName("entid_dirigente_representante")[0];
	
	var janela = window.open( 'emenda.php?modulo=principal/popInsereDirigenteRepresentante&acao=A&entid=' + entid.value + '', 'popup_cpf', 'width=800,height=700,status=0,menubar=0,toolbar=0,scrollbars=1,resizable=0' );
	janela.focus();
}

function filtraEmendas(proid) {
	if(!proid){
		return false;
	}
	var destino = document.getElementById("td_emendas");
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: "filtraEmendaAjax=true&" + "proid=" + proid,
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