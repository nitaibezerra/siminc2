<?php


function mudaEnd($cpf){
	global $db;
 
	$sql = sprintf("SELECT 
					 u.usdgabinete, u.usdramal, u.usdcelular, u.usdsala, u.unaid, u.laaid, l.lcaid
					FROM 
					 demandas.usuariodetalhes u
					 LEFT JOIN demandas.localandaratendimento l on l.laaid = u.laaid
					WHERE
					 u.usucpf = '%s' 
					", 
			$cpf);
	$dados = $db->PegaLinha($sql);
	
	if($dados){
		echo $dados['unaid'] ."|". $dados['lcaid'] ."|". $dados['laaid'] ."|". $dados['usdsala'] ."|". $dados['usdgabinete'] ."|". $dados['usdramal'] ."|". $dados['usdcelular'];
	}
	else{
		echo "";
	}
	exit;
	 
}

//filtra tipo para mostrar qtd
if ($_REQUEST['cpfAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	mudaEnd($_POST['cpfAjax']);
	exit;
}


if($_REQUEST['novousuario'] == '1')
{
	$_SESSION = array();
	?>
		<script>
			window.opener.location.href="http://simec.mec.gov.br/demandas/cadastrar_usuario.php";
			self.close();
		</script>
	<?
	exit;
}


monta_titulo( 'Pesquisa de Usuário', '' );

$usucpf   = $_GET['usucpf'];
$usernome = $_GET['usunome']; 
//$unaid	  = $_GET['unaid'];
$usermail = $_GET['usermail'];

$perfil = arrayPerfil();


?>
<html>
 <head>
  <script type="text/javascript" src="/includes/prototype.js"></script>
  <script type="text/javascript" src="../includes/funcoes.js"></script>
  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
  <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
  <script type="text/javascript">
  	d = document;
  	
  	/*
  	 * Habilita opções para cadastro de usuários não cadastrados no sistema
  	*/ 	
	function outroUsuario(obj){		
		//var tr1    = d.getElementById('tr_user1');	
		var tr2    = d.getElementById('tr_user2');
		var tr3	   = d.getElementById('tr_user3');	
		var select = d.getElementsByName('usucpf')[0];
					
		if (obj.checked){
			//tr1.style.display 		   = navigator.appName == 'Netscape' ? 'table-row' : 'block';	
			tr2.style.display 		   = navigator.appName == 'Netscape' ? 'table-row' : 'block';	
			tr3.style.display 		   = navigator.appName == 'Netscape' ? 'table-row' : 'block';				
			select.options[0].selected = true;	
			select.disabled   		   = true;			
		}else{
			//tr1.style.display = 'none';
			tr2.style.display = 'none';
			tr3.style.display = 'none';			
			select.disabled   = false;
		}
	}	
	
	/*
	 * Valida e envia para página pai
	*/
	function enviar(){
		if (d.getElementById('outrouser').checked){
			//if (d.getElementsByName('unaid')[0].value == '' || d.getElementsByName('usernome')[0].value == '' || d.getElementsByName('usermail')[0].value == ''){
			if (trim(d.getElementsByName('usernome')[0].value) == '' || trim(d.getElementsByName('usermail')[0].value) == ''){
				alert('Os campos "Demandante" e "E-mail" são obrigatórios!');
				return false;
			}

			if (!validaEmail(d.getElementsByName('usermail')[0].value)){
				alert('"E-mail" Inválido!');
				return false;
			}else{
				//window.opener.d.getElementById('unaid').value  			  = d.getElementsByName('unaid')[0].value;
				window.opener.d.getElementsByName('usudemandante')[0].value   = d.getElementsByName('usernome')[0].value.toUpperCase();
				window.opener.d.getElementById('outrouser').value 			  = d.getElementsByName('outrouser')[0].value;					
				window.opener.d.getElementById('usermail').value			  = d.getElementsByName('usermail')[0].value;	
				window.opener.d.getElementById('usucpf').value    			  = "";
				window.close();									
			}
		}else if (d.getElementsByName('usucpf')[0].value == ''){
			alert('Nenhum usuário foi selecionado!');
			return false;
			//window.close();
		}else{
			//window.opener.d.getElementById('unaid').value  			= "";
			window.opener.d.getElementsByName('usudemandante')[0].value = d.getElementsByName('usucpf')[0].options[d.getElementsByName('usucpf')[0].selectedIndex].text.toUpperCase();
			window.opener.d.getElementById('outrouser').value 			= 0;
			window.opener.d.getElementById('usermail').value			= "";					
			window.opener.d.getElementById('usucpf').value    			= d.getElementsByName('usucpf')[0].value;

			window.opener.d.forms['formDemanda'].submit();											
		}
		//alert('Operação realizada com sucesso!');
		
		//mudaend(d.getElementsByName('usucpf')[0].value);

		
		
		
	}	

	/*
	function mudaend(cpf){

		trs	   = window.opener.d.getElementById('tr_sala');
		tr 	   = window.opener.d.getElementById('tr_andar');
		
		if(cpf){
			// Faz uma requisição ajax, passando o parametro 'cpf', via POST
			var req = new Ajax.Request('demandas.php?modulo=principal/popup/pesqUser&acao=A', {
									        method:     'post',
									        parameters: '&cpfAjax=' + cpf,
									        onComplete: function (res)
									        {		
												var result = res.responseText;

												if(result){

													tr.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
													trs.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
													
													result = result.split("|");
													
													if(result[0]) window.opener.d.getElementById('unaid').value = result[0];
													if(result[1]) window.opener.d.getElementById('lcaid').value = result[1];
													//if(result[2]) window.opener.d.getElementById('laaid').value = result[2];
													if(result[3]) window.opener.d.getElementById('dmdsalaatendimento').value = result[3]; 
													if(result[4] == 't') window.opener.d.getElementById('usdgabinete')[0].checked = true;
													if(result[5]) window.opener.d.getElementById('usdramal').value = result[5];
													if(result[6]) window.opener.d.getElementById('usdcelular').value = result[6].substring(0,4) +'-'+ result[6].substring(4,8);

													filtraAndar(result[1],result[2]);
												}
													
												
												
												
									        }
									  });
		}
		
		//window.close();
	}

*/
	/*
	* Faz requisição via ajax
	* Filtra o andar, atravéz do parametro passado 'lcaid'
	*/
	/*
	function filtraAndar(lcaid,laaid) {

		if(!lcaid) lcaid = '999999';
		
		trs	   = window.opener.d.getElementById('tr_sala');
		tr 	   = window.opener.d.getElementById('tr_andar');	
		td     = window.opener.d.getElementById('andardemanda');
		campolaaid = window.opener.d.getElementById('laaid');	
			
		// Faz uma requisição ajax, passando o parametro 'ordid', via POST
		var req = new Ajax.Request('demandas.php?modulo=principal/cadDemanda&acao=A', {
								        method:     'post',
								        parameters: '&lcaidAjax=' + lcaid,
								        onComplete: function (res)
								        {			
											td.innerHTML = res.responseText;
											campolaaid.value = laaid;
											tr.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
											trs.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
											window.close();
								        }
								  });
	}	
	*/
	function cadnovousu()
	{
		location.href="demandas.php?modulo=principal/popup/pesqUser&acao=A&usucpf=<?=$_SESSION['usucpf']?>&novousuario=1"
	}
	
  </script>
 </head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<form id="formDemanda" action="" method="post" onsubmit="return validaForm();">
<table id="tblFormDemanda" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr bgcolor="#F2F2F2">
		<td class="subtitulodireita">Usuário:</td>
		<td>
			<?php
				$sql = "SELECT
						 u.usucpf AS codigo,
						 u.usunome AS descricao
						FROM
						 seguranca.usuario u
						 inner join seguranca.usuario_sistema us on
						 u.usucpf = us.usucpf
						 where
						 us.sisid = ".$_SESSION['sisid']." AND
						 us.susstatus = 'A' AND
						 us.suscod = 'A'
						ORDER BY
						 TRANSLATE(u.usunome, ' ','0')";
				$db->monta_combo("usucpf",$sql, 'S' ,"-- Selecione um usuário --",'','');
				if ($usernome){
					echo "<script>d.getElementsByName('usucpf')[0].disabled = true;</script>";
				}	
			?>
			<?= obrigatorio(); ?>
		</td>
	</tr>
	<tr bgcolor="#F2F2F2">
		<td class="subtitulodireita">&nbsp;</td>
		<td>
			
			<input type='checkbox' <?=$usernome ? 'checked=checked' : ''; ?> value='1' name='outrouser' id='outrouser' onclick="outroUsuario(this)"/> Marque, caso o usuário não esteja cadastrado no sistema.
			
			<?if ( in_array(DEMANDA_PERFIL_TECNICO1, $perfil)  ){ ?>
				<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				ou <a href="javascript:cadnovousu();"><font color="blue"><b>clique aqui</b></font></a> para cadastrar um novo usuário.
			<?}?>  
		</td>
	</tr>
	<!-- 	
	<tr bgcolor="#F2F2F2" id="tr_user1" style="display:<?=$usernome ? '' : 'none'; ?>;">
		<td class="subtitulodireita">Setor:</td>
		<td>
			<?php
				$sql = " SELECT
						  unaid AS codigo, 
					  	  upper(unasigla)||' - '||unadescricao as descricao 
						 FROM
						  demandas.unidadeatendimento 
						 WHERE
						  unastatus = 'A'";
				$db->monta_combo("unaid",$sql,'S',"-- Selecione um setor --",'','');
			?>
			<?= obrigatorio(); ?>
		</td>
	</tr>
	 -->
	<tr id="tr_user2" style="display:<?=$usernome ? '' : 'none'; ?>;">
		<td class="SubTituloDireita">Demandante:</td>
		<td><?= campo_texto( 'usernome', 'S', 'S', '', 40, 50, '', ''); ?></td>
	</tr>	
	<tr id="tr_user3" style="display:<?=$usernome ? '' : 'none'; ?>;">
		<td class="SubTituloDireita">E-mail:</td>
		<td><?= campo_texto( 'usermail', 'S', 'S', '', 40, 50, '', ''); ?></td>
	</tr>	
	<tr bgcolor="#C0C0C0">
		<td>&nbsp;</td>
		<td>
	    	<input type='button' class='botao' value='Enviar' name='cadastrar' onclick='enviar()' />&nbsp;
	    	<input type='button' class='botao' value='Fechar' name='fechar' onclick='window.close();' />	    	
		</td>			
	</tr>				
</table>
</form>
</body>
</html>



