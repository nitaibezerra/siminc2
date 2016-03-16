<?php

ini_set("memory_limit","256M");

if ($_REQUEST['test'] == true){

	function carregaNome($cpf){

		global $db;
		//Tratar o CPF - Retirar os caracteres e deixar os numeros
		$cpf	 = str_replace(Array('.','-'),'',$cpf);

		$sql  = "select usunome from seguranca.usuario where usucpf='".$cpf."'";
		$nome = $db->pegaUm($sql);

		echo trim($nome);
	}

	carregaNome($_POST['fdpcpf']);
	die();
		
}

include_once( APPRAIZ . "www/includes/webservice/cpf.php" );
echo '<br>';

$fdpcpf = $_REQUEST['ajaxcpf'];

monta_titulo( 'Advogado', 'Dados Pessoais' );

if( $_REQUEST['ajaxcpf'] ){
	
	$sql = "SELECT
				adv.advid,
				entnumcpfcnpj,
				entnome as usunome,
				advstatus,
				coonid as titular
			FROM
				conjur.advogados adv
			INNER JOIN entidade.entidade ent ON ent.entid = adv.entid
			WHERE
				entnumcpfcnpj = '".$_REQUEST['ajaxcpf']."'";
	$adv = $db->pegaLinha($sql);
	extract($adv);
	$stSqlCarregados = "SELECT
							coo.coonid AS codigo, 
							coo.coodsc  AS descricao
						FROM
							conjur.coordenacao coo
						INNER JOIN conjur.advogadosxcoordenacao adc ON adc.coonid = coo.coonid
						WHERE
							coo.advid = ".$adv['advid'];
}

//////////////////////////////////////////////////////////////////////////////////////////
//Ação Editar
if ($_REQUEST['conjur'] == 'editar'){

	//Receber o CPF e Retirar os caracteres, deixando só os numeros
	$cpf	 = str_replace(Array('.','-'),'',$_REQUEST['fdpcpf']);

	$coonids 	= $_REQUEST['coonid'];
	$advstatus 	= $_REQUEST['advstatus'];
	$titular 	= $_REQUEST['titular'];

	//Editar Advogado

	$sql = "SELECT
				adv.advid
			FROM
				conjur.advogados adv
			INNER JOIN entidade.entidade ent ON ent.entid = adv.entid
			WHERE entnumcpfcnpj = '$cpf'";

	$busca = $db->carregar($sql);
	$resul = $busca[0]["advid"];

	if ($resul){
		$sql = "UPDATE conjur.advogados SET
					advstatus = '$advstatus',
					coonid = {$titular} 
				WHERE
					advid = $resul;";
		$sql .="DELETE FROM conjur.advogadosxcoordenacao WHERE advid = $resul;";
		if($titular){
			$sql .= "UPDATE conjur.coordenacao SET advid = $resul WHERE coonid = {$titular};";
		}
		if( is_array($coonids) ){
			foreach( $coonids as $coonid ){
				$sql.= "INSERT INTO conjur.advogadosxcoordenacao(advid, coonid) VALUES($resul, $coonid);";
			}
		}
	}
	$db->executar($sql);
	$db->commit();

	echo("<script>alert('Alterado com Sucesso.')\n</script>");
	echo("<script>window.parent.opener.location.reload();</script>");
	echo("<script>window.close();</script>");
	exit();
}

//////////////////////////////////////////////////////////////////////////////////////////
//Ação Salvar
if ($_REQUEST['conjur'] == 'inserir'){

	//Receber o CPF e Retirar os caracteres, deixando só os numeros
	$cpf	 = str_replace(Array('.','-'),'',$_REQUEST['fdpcpf']);

	$usunome 	= $_REQUEST['usunome'];
	$coonids 	= $_REQUEST['coonid'];
	$advstatus 	= $_REQUEST['advstatus'];
	$titular 	= $_REQUEST['titular'];

	//Verificar se está Cadastrado no SIS
	if ($cpf){
		$sql = "SELECT true FROM seguranca.usuario WHERE usucpf = '$cpf'";
		$retorno = $db->pegaUm($sql);
	}

	//Não está cadastrado no SIS
	if(!$retorno){
		echo("<script>alert('Não está Cadastrado no SIS.')\n</script>");
		echo("<script>window.location.href = 'conjur.php?modulo=sistema/geral/cadDadosAdvogado&acao=A'</script>");
		exit();
	}
		

	//Está cadastrado no SIS
	if($retorno){
		//Verificar se o Advogado está Cadastrado na tabela entidade
		if ($cpf){
			$sql1 = "SELECT true FROM entidade.entidade WHERE entnumcpfcnpj = '$cpf'";
			$retorno1 = $db->pegaUm($sql1);
		}

		//Está cadastrado no SIS mas não em entidade
		if(!$retorno1){
			//Cadastrar na tabela Advogado e Entidade
			
			//Busca dados da tabela usuario
			$sql = "SELECT
						usucpf,
						usunome,
						usuemail,
						ususexo,
						usufoneddd,
						usufonenum
					FROM 
						seguranca.usuario
					WHERE usucpf = '$cpf' ";

			$dados = $db->carregar($sql);
			$usucpf = $dados[0]["usucpf"];
			$usunome = $dados[0]["usunome"];
			$usuemail = $dados[0]["usuemail"];
			$ususexo = $dados[0]["ususexo"];
			$usufoneddd = $dados[0]["usufoneddd"];
			$usufonenum = $dados[0]["usufonenum"];

			if ($usucpf){
				//Inserir na Tabela Entidade
				$sql = "INSERT INTO entidade.entidade (
					 		entnumcpfcnpj,
					  	 	entnome,
					  	 	entemail,
					  	 	entstatus,
					  	 	entsexo,
					  	 	entnumdddcomercial,
					  	 	entnumcomercial )
			  	 		VALUES (
					  		'".$usucpf."',
					  	 	'".$usunome."',
					  	 	'".$usuemail."',
					  	 	'A',
					  	 	'".$ususexo."',
						  	'".$usufoneddd."',
						  	'".$usufonenum."' )";
				$db->executar($sql);
				$db->commit();
			}else{
				echo("<script>alert('Falha ao buscar dados do usuario.')\n</script>");
				echo("<script>window.parent.opener.location.reload();</script>");
				echo("<script>window.close();</script>");
			}

			//Busca o ID da Entidade
			$sql = "SELECT
						entid
					FROM 
						entidade.entidade
					WHERE entnumcpfcnpj = '$usucpf' ";

			$entid = $db->pegaUm($sql);

			if ($entid){
				//Inserir na Tabela Advogados
				$sql = "INSERT INTO conjur.advogados (
						 	 entid,
						  	 advstatus,
						  	 coonid
						  	  )
	  	 				VALUES (
				  	 		'$entid',
					  	 	'$advstatus',
					  	 	{$titular} )
					  	RETURNING advid;";
				$advid = $db->pegaUm($sql);
				$sql ="DELETE FROM conjur.advogadosxcoordenacao WHERE advid = $advid;";
				if($titular){
					$sql .= "UPDATE conjur.coordenacao SET advid = $advid WHERE coonid = {$titular};";
				}
				if( is_array($coonids) ){
					foreach( $coonids as $coonid ){
						$sql.= "INSERT INTO conjur.advogadosxcoordenacao(advid, coonid) VALUES($advid, $coonid);";
					}
				}
				$db->executar($sql);
				$db->commit();
				echo("<script>alert('Cadastrado com Sucesso.')\n</script>");
				echo("<script>window.parent.opener.location.reload();</script>");
				echo("<script>window.close();</script>");
			}else{
				echo("<script>alert('Erro ao recuperar usuario da entidade.')\n</script>");
				echo("<script>window.parent.opener.location.reload();</script>");
				echo("<script>window.close();</script>");
			}
			echo("<script>window.location.href = 'conjur.php?modulo=sistema/geral/listaAdvogados&acao=A'</script>");
			exit();
		}

		//Está cadastrado no SIS e em entidade
		if($retorno1){

			//Busca dados das diversas tabelas
			$sql = "SELECT
						ent.entid
					FROM
						entidade.entidade as ent
					INNER JOIN seguranca.usuario usu ON ent.entnumcpfcnpj = usu.usucpf
					WHERE ent.entnumcpfcnpj='".$cpf."' ";

			$entid = $db->pegaUm($sql);

			if ($entid){
				//Inserir na Tabela Advogados
				$sql = "INSERT INTO conjur.advogados (
						 	 entid,
						  	 advstatus,
						  	 coonid
						  	  )
	  	 				VALUES (
				  	 		'$entid',
					  	 	'$advstatus',
					  	 	{$titular} )
					  	RETURNING advid;";
				$advid = $db->pegaUm($sql);
				$sql ="DELETE FROM conjur.advogadosxcoordenacao WHERE advid = $advid;";
				if($titular){
					$sql .= "UPDATE conjur.coordenacao SET advid = $advid WHERE coonid = {$titular};";
				}
				if( is_array($coonids) ){
					foreach( $coonids as $coonid ){
						$sql.= "INSERT INTO conjur.advogadosxcoordenacao(advid, coonid) VALUES($advid, $coonid);";
					}
				}
				$db->executar($sql);
				$db->commit();
				echo("<script>alert('Cadastrado com Sucesso.')\n</script>");
				echo("<script>window.parent.opener.location.reload();</script>");
				echo("<script>window.close();</script>");
			}else{
				echo("<script>alert('Erro ao recuperar usuario da entidade.')\n</script>");
				echo("<script>window.parent.opener.location.reload();</script>");
				echo("<script>window.close();</script>");
			}
			echo("<script>window.location.href = 'conjur.php?modulo=sistema/geral/listaAdvogados&acao=A'</script>");
			exit();
		}
		exit();
	}
}

//////////////////////////////EXCLUIR//////////////////////////////

$cpf = $_REQUEST['cpf'];

if ($cpf){

	$sql = "SELECT
				adv.advid
			FROM
				conjur.advogados as adv
			INNER JOIN entidade.entidade  as ent ON ent.entid = adv.entid
			INNER JOIN seguranca.usuario  as usu ON ent.entnumcpfcnpj = usu.usucpf
			INNER JOIN conjur.coordenacao as coo ON coo.coonid = adv.coonid
			WHERE ent.entnumcpfcnpj = '$cpf'";

	$advid = $db->pegaUm($sql);

	$sql = "SELECT true FROM conjur.processoconjur WHERE advid = ".$advid;
	$cont = $db->pegaUm($sql);

	if(!$cont)
	{
		if ($advid)
		{
			$sql = "UPDATE conjur.coordenacao SET advid = NULL WHERE advid = $advid;";
			$sql .= "DELETE FROM conjur.advogadosxcoordenacao WHERE advid =	'$advid';";
			$sql .= "DELETE FROM conjur.advogados WHERE advid =	'$advid';";
			$db->executar($sql);
			$db->commit();

			echo("<script>alert('Excluído com Sucesso.')\n</script>");
			echo("<script>window.location.href = 'conjur.php?modulo=sistema/geral/listaAdvogados&acao=A'</script>");
			exit();
		}
		else
		{
			echo("<script>alert('Erro ao excluir. Tente novamente')\n</script>");
			echo("<script>window.location.href = 'conjur.php?modulo=sistema/geral/listaAdvogados&acao=A'</script>");
			exit();
		}
	}
	else
	{
		echo("<script>alert('O advogado não pode ser excluído pois possui referência em outras tabelas.')\n</script>");
		echo("<script>window.location.href = 'conjur.php?modulo=sistema/geral/listaAdvogados&acao=A'</script>");
		exit();
	}
}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script src="../includes/prototype.js"></script>
<script src="../includes/calendario.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css' />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<center>Preencha os Dados do Advogado</center>
<form name="formulario" action="" method="post" id="formulario" enctype="multipart/form-data"><input type="hidden" name="conjur">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<?php
	if ( $entnumcpfcnpj == ''){ $ncpf="S"; }
	if ( $entnumcpfcnpj != ''){ $ncpf="N"; }
	?>
	<tr>
		<td class="SubTituloDireita" align="right">CPF:</td>
		<td>
			<?=campo_texto('fdpcpf', 'S', $ncpf, '', 20, 14, '', '', 'left', '',  0, 'id="fdpcpf" onkeyup="this.value=mascaraglobal(\'###.###.###-##\',this.value);" ','','', 'test(this.value);'  ); ?>
		</td>
	</tr>
	<?php
	if ( $usunome == ''){ $nome="S"; }
	if ( $usunome != ''){ $nome="N"; }
	?>
	<tr>
		<td class="SubTituloDireita" align="right">Nome:</td>
		<td><?= campo_texto('usunome', 'S', $nome, '', 80, 200, '', '', 'left', '',  0, 'id="usunome" onblur="MouseBlur(this);"' ); ?>
		</td>
	</tr>
	<?
	$sql = "SELECT
				coonid AS codigo, 
				coodsc  AS descricao
			FROM
				conjur.coordenacao ";	                
	mostrarComboPopup( 'Coordecação:', 'coonid',  $sql, $stSqlCarregados, 'Selecione a(s) Coordenação(ões)' ,null,null,null,null,$arrVisivel,$arrOrdem);
	?>
	<tr>
		<td class="SubTituloDireita" align="right">Titular:</td>
		<td>
			<?php 
			$sql = "SELECT
						coonid AS codigo, 
						coodsc  AS descricao
					FROM
						conjur.coordenacao ";
			
			$db->monta_combo('titular', $sql, 'S', '', '', '', '');
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" align="right">Status:</td>
		<td>
			<input type="radio" name="advstatus" id="advstatus[A]" <?=($advstatus = 'A' ? 'checked="checked"' : '' )?> value="A"> Ativo 
			<input type="radio" name="advstatus" id="advstatus[I]" <?=($advstatus = 'A' ? '' : 'checked="checked"' )?> value="I"> Inativo
		</td>
	</tr>
	<?php if ( $entnumcpfcnpj == ''){ ?>
		<td class="SubTituloDireita" align="left"></td>
		<td>
			<input type="button" name="btSalvar" id="btSalvar" onclick="validaForm1('grava');" value="Salvar"> 
			<input type="button" value="Fechar" style="cursor: pointer" id="botaovoltar" onclick="window.close();">
		</td>
	<?php 
		  }
		  if ( $entnumcpfcnpj != ''){
	?>
		<td class="SubTituloDireita" align="left"></td>
		<td>
			<input type="button" name="btEditar" id="btEditar" onclick="validaForm2('altera');" value="Alterar"> 
			<input type="button" value="Fechar" style="cursor: pointer" id="botaovoltar" onclick="window.close();">
		</td>
	<?php } ?>
	</tr>
</table>
</form>
<script>
function test(valor){
	new Ajax.Request('?modulo=sistema/geral/cadDadosAdvogado&acao=A&test=true',{
		method: 'post',
		parameters: $('formulario').serialize('true'),
		onComplete: function(res){
			document.getElementById('usunome').value = res.responseText; 
			//alert(res.responseText);
		}
	});
}



//Cadastra
function validaForm1(type){ 

	var fdpcpf = document.getElementById('fdpcpf'); 
	var usunome = document.getElementById('usunome'); 
	var coonid = document.getElementById('coonid'); 
	var advstatus = document.getElementById('advstatus');
	
	var type;
	if( type == 'grava' ) {

	if( fdpcpf.value == '' ){
		alert( 'O campo CPF não pode ser vazio' );
		return false;
	}

	if( usunome.value == '' ){
		alert( 'O campo Nome não pode ser vazio' );
		return false;
	}

	if( coonid.options[0].value == '' ){
		alert( 'O campo Coordenação não pode ser vazio' );
		return false;
	}

	if( document.getElementById('advstatus[A]').checked == 0 && 
			document.getElementById('advstatus[I]').checked == 0 ){
			alert( 'O campo "Status" deve ser preenchido' );
			return false;
	}

	selectAllOptions( document.getElementById( 'coonid' ) );
	document.formulario.conjur.value = "inserir";
		
	}

	document.formulario.submit();
		
}



//Edita
function validaForm2(type){ 

	var fdpcpf = document.getElementById('fdpcpf'); 
	var usunome = document.getElementById('usunome'); 
	var coonid = document.getElementById('coonid'); 
	var advstatus = document.getElementById('advstatus');
	alert(coonid.options[0].value);
	var type;
	if( type == 'altera' ) {

		if( fdpcpf.value == '' ){
			alert( 'O campo CPF não pode ser vazio' );
			return false;
		}
	
		if( usunome.value == '' ){
			alert( 'O campo Nome não pode ser vazio' );
			return false;
		}
	
		if( coonid.options[0].value == '' ){
			alert( 'O campo Coordenação não pode ser vazio' );
			return false;
		}
	
		if( document.getElementById('advstatus[A]').checked == 0 && 
				document.getElementById('advstatus[I]').checked == 0 ){
				alert( 'O campo "Status" deve ser preenchido' );
				return false;
		}
		document.formulario.conjur.value = "editar";
	}

	selectAllOptions( document.getElementById( 'coonid' ) );
	
	document.formulario.submit();
		
}

/* Função para substituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}

			
				
/**
 * Alterar visibilidade de um bloco.
 * 
 * @param string indica o bloco a ser mostrado/escondido
 * @return void
 */
function onOffBloco( bloco )
{
	var div_on = document.getElementById( bloco + '_div_filtros_on' );
	var div_off = document.getElementById( bloco + '_div_filtros_off' );
	var img = document.getElementById( bloco + '_img' );
	var input = document.getElementById( bloco + '_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '0';
		img.src = '/imagens/menos.gif';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '1';
		img.src = '/imagens/mais.gif';
	}
}

/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}
</script>
