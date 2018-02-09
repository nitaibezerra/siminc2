<?php

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$usucpf = $_SESSION[usucpf];
	
if ($_GET[unrid])
{
	$db->executar("UPDATE emenda.unidnomeresp SET unrstatus = 'I', unrdataalteracao = NOW(), usucpfalteracao = '$usucpf' WHERE unrid = '$_GET[unrid]'");
	$db->commit();	
}

if( isset( $_POST['submetido'] ) ) {

	if($_REQUEST["uniid"] && $_REQUEST["uniid"] != "") {
		// UPDATE
		$db->executar("UPDATE emenda.unidades SET
													unisigla = '".pg_escape_string($_REQUEST["unisigla"])."',
													uninome = '".pg_escape_string($_REQUEST["uninome"])."',
													unidataalteracao = NOW(),
													usucpfalteracao = '$usucpf'
												   WHERE
												   		uniid = ".$_REQUEST["uniid"]);
		
		$uniid = $_REQUEST["uniid"];
		
		if ($_POST["unrcargo"])
		{
			foreach ($_POST["unrcargo"] AS $k => $v)
			{
				$unrordem = $_POST["unrordem"][$k];
				$unrcargo = $_POST["unrcargo"][$k];
				$unrnome = $_POST["unrnome"][$k];
				
				$db->executar("UPDATE emenda.unidnomeresp SET unrordem = '$unrordem', unrcargo = '$unrcargo', unrnome = '$unrnome', unrdataalteracao = NOW(), usucpfalteracao = '$usucpf' WHERE unrid = '$k'");
			}		
		}
	}
	else {
		// INSERT
		$uniid = $db->pegaUm("INSERT INTO emenda.unidades(	unisigla,
															uninome,
															unistatus,
															unidatainclusao,
															unidataalteracao,
															usucpfinclusao,
															usucpfalteracao) 
					   							VALUES( '".pg_escape_string($_REQUEST["unisigla"])."',
					   									'".pg_escape_string($_REQUEST["uninome"])."',
					   									'A',
					   									'NOW()',
					   									'NOW()',
					   									'$usucpf',
					   									'$usucpf') RETURNING uniid");
	}
	
	if ($_POST["unrcargo_novo"])
	{
		// insere responsável
		foreach ($_POST["unrcargo_novo"] AS $k => $v)
		{
			$unrordem = $_POST["unrordem_novo"][$k];
			$unrcargo = $_POST["unrcargo_novo"][$k];
			$unrnome = $_POST["unrnome_novo"][$k];
			
			$db->executar("INSERT INTO emenda.unidnomeresp (uniid, unrordem, unrcargo, unrnome, unrstatus, unrdatainclusao, usucpfinclusao) VALUES('$uniid','$unrordem','$unrcargo','$unrnome','A',NOW(),'$usucpf')");
		}
	}
	
	$db->commit();
	echo "<script type=\"text/javascript\">
			alert(\"Operação realizada com sucesso.\");
			window.location = \"emenda.php?modulo=principal/listaUnidades&acao=A\";
		  </script>";
	die;
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( "Cadastro Unidades", "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios" );

if($_REQUEST["uniid"]) {

	$dados 					= $db->carregar("SELECT * FROM emenda.unidades WHERE uniid = ".$_REQUEST["uniid"]);
		
	$iniid 		= $dados[0]["iniid"];
	$uninome 	= $dados[0]["uninome"];
	$unisigla 	= $dados[0]["unisigla"];	
}

?>

<form id="formCadastroUnidades" name="formCadastroUnidades" action="" method="post" enctype="multipart/form-data">
<input type="hidden" value="1" id="submetido" name="submetido" />
<input type="hidden" value="<?= ($_REQUEST["uniid"]) ? $_REQUEST["uniid"] : "" ?>" id="uniid" name="uniid" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;"><b>Sigla:</b></td>
		<td>
			<?=campo_texto( 'unisigla', 'S', 'S', 'Sigla', 80, 350, '', '','','','','id="sigla"');?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"><b>Unidade:</b></td>
		<td>
			<?=campo_texto( 'uninome', 'S', 'S', 'Unidade', 80, 350, '', '','','','','id="uninome"');?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Responsável:</td>
		<td>
			<a href="#" onclick="inserirResponsavel();"><img src="../imagens/gif_inclui.gif" border="0" style="cursor:pointer;">&nbsp;Vincular Responsável</a>
			<br />
			<table id="tb_responsavel" width="50%" border="0" cellspacing="0" cellpadding="2" class="listagem">
               <thead>
               <tr>
               		<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Cargo</strong></td>
                    <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Responsável</strong></td>
                    <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Excluir</strong></td>
               </tr>
               </thead>
               <tbody id='responsavel'>
               <?
			   	if($_REQUEST["uniid"]) {
					$sql = "  SELECT
								unrid, 
								unrcargo,
								unrnome,
								unrordem
							FROM 
							  emenda.unidnomeresp res
							WHERE
								uniid = ".$_REQUEST['uniid']."
							    and unrstatus = 'A'								
							ORDER BY
								unrnome";
					
					$dados = $db->carregar($sql);
		
					if($dados) {
				   		for($i=0; $i<count($dados); $i++) {
				   			$cor = ($i % 2) ?  "#e0e0e0" : "#f4f4f4";
				   			
				   			$unrid = $dados[$i]["unrid"];
				   			
							echo "<tr bgcolor=\"".$cor."\">
		                               <td align=\"center\">
		                               		<input type=\"text\" name=\"unrordem[$unrid]\" value=\"".$dados[$i]["unrordem"]."\" size=5 />
		                               		</td><td>		                               
		                               		<input type=\"text\" name=\"unrcargo[$unrid]\" value=\"".$dados[$i]["unrcargo"]."\" size=30 />
		                               		</td><td>
		                               		<input type=\"text\" name=\"unrnome[$unrid]\" value=\"".$dados[$i]["unrnome"]."\" size=40/>
		                               		</td><td>
		                               		<a href=\"#\" onclick=\"window.open('emenda.php?modulo=principal/cadastroUnidades&acao=A&uniid=$_GET[uniid]&unrid=$unrid','_top')\" ><img src=\"../imagens/excluir.gif\" border=\"0\" style=\"cursor:pointer;\"></a>
		                               </td>
	                              </tr>";
						}
					}
				}
			   ?>
			   </tbody>
       		</table>
       		<br />
		</td>
	</tr>
	<?
	$perfil = pegaPerfilArray( $_SESSION['usucpf'] );
	if( in_array( GESTOR_EMENDAS, $perfil ) || in_array( SUPER_USUARIO, $perfil ) ){
		$retorno = '';
	} else {
		$retorno = 'disabled="disabled"';
	}
	?>
	<tr bgcolor="#c0c0c0">
		<td>&nbsp;</td>
		<td>
			<input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" onclick="salvarUnidades();" <?=$retorno;?> />
			<input type="button" value="Cancelar" name="btnVoltar" id="btnVoltar" onclick="voltarUnidades();">
		</td>
	</tr>
</table>
</form>

<script type="text/javascript">

form 		= document.getElementById("formCadastroUnidades");
btnSalvar 	= document.getElementById("btnSalvar");
btnVoltar 	= document.getElementById("btnVoltar");

function voltarUnidades() {
	btnSalvar.disabled = true;
	btnVoltar.disabled = true;

	window.location.href = "emenda.php?modulo=principal/listaUnidades&acao=A";
}

function salvarUnidades() {
		
	btnVoltar.disabled = true;

	var campos = document.getElementById('formCadastroUnidades').getElementsByTagName('input');
	var obrigatorio = 0;
	
	for (i = 0; i < campos.length; i++)
	{
		if (!campos[i].value) obrigatorio = 1;
	}

	if (obrigatorio == 1)
	{
		alert('Todos os campos são obrigatórios.');
		return false;
	}
	
	/*
	if(espnome.value == "") {
		alert("O campo 'Especificação' deve ser informado.");
		btnSalvar.disabled = false;
		btnVoltar.disabled = false;
		espnome.focus();
		return;
	}*/

	form.submit();
}

function inserirResponsavel() {

	var inputs = document.getElementById('responsavel').getElementsByTagName('input');

	var inputs_old = new Array;
	
	for (i = 0; i < inputs.length; i++)
	{
		inputs_old[i] = inputs[i].value;
	}
	
	document.getElementById('responsavel').innerHTML += "<tr><td><input type='text' name='unrordem_novo[]' size='10'></td><td><input type='text' name='unrcargo_novo[]' size='30'></td><td><input type='text' name='unrnome_novo[]' size='40'></td><td><a style='cursor:pointer;' onclick='this.parentNode.parentNode.parentNode.removeChild(this.parentNode.parentNode);'><img src='../imagens/excluir.gif' style='cursor:pointer;' border='0'></a></td></tr>"

	for (i = 0; i < inputs.length; i++)
	{
		if (inputs_old[i]) inputs[i].value = inputs_old[i];
	}
}	
</script>