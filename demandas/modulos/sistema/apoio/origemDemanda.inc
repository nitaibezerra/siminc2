<?php

		//unset($_SESSION['ordid']);
		//unset($_SESSION['op3']);

function responsavel_email($email){
	global $db;
 
	$sql = "SELECT u.usuemail FROM seguranca.usuario AS u WHERE usucpf = '$email'";
	$email = $db->carregar($sql);
	print campo_texto( 'usuemail', 'N', 'N', '', 80, 250, '', '','','','','','',$email[0]['usuemail']);
	
}

function responsavel_tel($tel){
	global $db;
 
	$sql = "SELECT '(' || u.usufoneddd || ') '|| u.usufonenum AS tel FROM seguranca.usuario AS u WHERE usucpf = '$tel'";
	$tel = $db->carregar($sql);
	print campo_texto( 'tel', 'N', 'N', '', 80, 250, '', '','','','','','',$tel[0]['tel']);
	
}

function inserirOrigem(){
	global $db;
	
	$sql = " INSERT INTO demandas.origemdemanda
			 (
			 	orddescricao, 
			 	ordstatus
			 ) VALUES (
			 	'".$_POST['orddescricao']."', 
			 	'".$_POST['ordstatus']."'
			 );";
	$db->executar($sql);
	$db->commit();
}

function selecionarOrigem(){
	global $db;
	$sql = "SELECT 
				o.ordid, 
				o.orddescricao,
				CASE ordstatus
					WHEN 'A' THEN 'Ativo'   
					ELSE 'Inativo' 
				END AS ordstatus
			FROM  
				demandas.origemdemanda o 
			WHERE 
				o.ordid = '".$_SESSION['ordid']."'
	";
	return $db->carregar($sql);
}


function alterarOrigem(){
	global $db;
	
	$sql = "UPDATE 
				demandas.origemdemanda 
			SET 
				orddescricao = '".$_POST['orddescricao']."',
				ordstatus = '".$_POST['ordstatus']."' 
			WHERE 
				ordid = '".$_POST['ordid']."';";

	$db->executar($sql);					
	$db->commit();
}


function deletarOrigem(){
	global $db;
	
	$sql = " DELETE FROM demandas.origemdemanda 
				WHERE ordid = ".$_SESSION['ordid']."";
	
	$db->executar($sql);
	$db->commit();
}

if ($_REQUEST['emailAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	responsavel_email($_POST['emailAjax']);
	exit;
}

if ($_REQUEST['telAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	responsavel_tel($_POST['telAjax']);
	exit;
}


if($_POST['orddescricao'] && $_POST['ordstatus'] && !$_POST['ordid']){
	inserirOrigem();
	unset($_SESSION['ordid']);
	unset($_SESSION['op3']);
	print "<script>alert('Operação realizada com sucesso!');</script>";
	//header( "Location: demandas.php?modulo=sistema/apoio/origemDemanda&acao=A" );
	//exit();
	
}

if($_POST['orddescricao'] && $_POST['ordstatus'] && $_POST['ordid']){
	alterarOrigem();
	unset($_SESSION['ordid']);
	unset($_SESSION['op3']);
	print "<script>alert('Operação realizada com sucesso!');</script>";
	//header( "Location: demandas.php?modulo=sistema/apoio/origemDemanda&acao=A" );
	//exit();
}

if($_GET['ordid'] && $_GET['op3']){
	session_start();
	$_SESSION['ordid'] = $_GET['ordid'];
	$_SESSION['op3'] = $_GET['op3'];
	header( "Location: demandas.php?modulo=sistema/apoio/origemDemanda&acao=A" );
	exit();
}

if($_SESSION['ordid'] && $_SESSION['op3']){
	if($_SESSION['op3'] == 'delete'){
		deletarOrigem();	
		unset($_SESSION['ordid']);
		unset($_SESSION['op3']);
		print "<script>alert('Operação realizada com sucesso!');</script>";
	}
	if($_SESSION['op3'] == 'update'){
		$dados = selecionarOrigem();	
		unset($_SESSION['ordid']);
		unset($_SESSION['op3']);
	}

}

include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

monta_titulo( 'Origem da Demanda', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<body>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="formOrigem" name="formOrigem" action="" method="post" onsubmit="processaCombosPopup();return validaForm();" >
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<input type="hidden" name="ordid" value="<? echo $dados[0]['ordid']; ?>" />
	<tr>
	<td align='right' class="SubTituloDireita">Origem da Demanda</td>
	<td>
	<? $orddescricao = $dados[0]['orddescricao'];  ?>
		<?= campo_texto( 'orddescricao', 'N', 'S', '', 94, 250, '', '','','','','',''); echo obrigatorio();?>
	</td>
	</tr>
	<tr>
	<td align='right' class="SubTituloDireita">Situação:</td>
	<td>
	<select id="ordstatus" name="ordstatus" class='CampoEstilo' >
		<option value=''>-- Informe o Status --</option>
		<option <?php ($dados[0]['ordstatus'] == 'Ativo')? $s = "selected=\"selected\"" : $s= ""; echo $s; ?> value="A">Ativo</option>
		<option <?php ($dados[0]['ordstatus'] == 'Inativo')? $s2 = "selected=\"selected\"" : $s2 =""; echo $s2; ?> value="I">Inativo</option>
		</select>
		<? echo obrigatorio(); ?>
	</td>
	</tr>
<? 
/*
	<tr>
	<td align='right' class="SubTituloDireita">Nome do Responsável</td>
	<td>
	<?php
	                     
        $sql = "SELECT
						 usucpf AS codigo,
						 usunome AS descricao
						FROM
						 seguranca.usuario
						WHERE 
						 ungcod IN(SELECT
									ungcod 
								   FROM
									unidadegestora 
								   WHERE
									ungstatus = 'A' AND 
									unitpocod = 'U' AND 
									unicod = '26101')
						ORDER BY
						 usunome;";		
					
				$db->monta_combo( 'usucpf', $sql, 'S', '-- Informe o nome do responsável --', 'resp_email(this.value);resp_tel', '','','','','tipid1','',$dados[0]['usucpf']);
			  echo obrigatorio();	?>
		</td>
	</tr>
	<td align='right' class="SubTituloDireita" >Ramal / Telefone de Contato da Equipe
	</td>
	<td>
	<div id="novotel">
	
	</div>
	<div id="antigotel" >
		<? $tel = $dados[0]['tel']; ?>
		<?= campo_texto( 'tel', 'N', 'N', '', 80, 250, '', '','','','','',''); ?>
		</div>
		</td>
	</tr>
	<td align='right' class="SubTituloDireita" >E-mail de Contato</td>
	<td>
	<div id="novoemail">
	
	</div>
	<div id="antigoemail" >
		<? $usuemail = $dados[0]['usuemail']; ?>
		<?= campo_texto( 'usuemail', 'N', 'N', '', 80, 250, '', '','','','','',''); ?>
		</div>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Equipe:</td>
		<td>
				<?php
					$sqlequipe = "SELECT
						 e.usucpf AS codigo,
						 u.usunome AS descricao
						FROM
							demandas.origemdemandaequipe AS e 
						LEFT JOIN seguranca.usuario AS u ON u.usucpf = e.usucpf 
						WHERE 
						 ordid = '".$dados[0]['ordid']."'
						;";
					
					$sql_combo = "SELECT
									 u.usucpf as codigo,
									 u.usucpf || ' - ' || u.usunome as descricao
									FROM
									 seguranca.usuario u
									WHERE
									 u.usustatus = 'A' OR
									 u.usustatus IS NULL
									ORDER BY
									 u.usunome";

			if($dados[0]['ordid'] != '' ){
			               $equipe = $db->carregar($sqlequipe);
			            }
					combo_popup(
						'equipe',
						$sql_combo,
						'Selecione o(s) Membro(s) da Equipe',
						'400x400',
						0,
						array(),
						'',
						'S',
						true,
						false,
						5
					);
				echo obrigatorio();?>
		</td>
	</tr>	
	<tr>
*/
?>
	<tr bgcolor="#C0C0C0">
		<td width="25%">&nbsp;</td>
		<td>
		<input type="submit" class="botao" name="btalterar" value="Salvar">
		<?php
		if (isset($dados)){
			echo '<input type="button" class="botao" name="del" value="Novo" onclick="javascript:location.href = window.location;">';	
		}
		?>
		</td>
	</tr>
	</table>
	<?php
	$sql = "SELECT
				'<a href=\"demandas.php?modulo=sistema/apoio/origemDemanda&acao=A&ordid=' || o.ordid || '&op3=update\">
				   <img border=0 src=\"../imagens/alterar.gif\" />
				 </a> 
				 <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'demandas.php?modulo=sistema/apoio/origemDemanda&acao=A&ordid=' || o.ordid || '&op3=delete\');\" >
				   <img border=0 src=\"../imagens/excluir.gif\" />
				 </a>', 
				o.orddescricao,				
				CASE
				 WHEN ordstatus = 'A' THEN 'Ativo'
				 ELSE 'Inativo'
				END AS status
			FROM 
				demandas.origemdemanda o
		  	ORDER BY 
				 o.ordid";
	
	$cabecalho = array( "Opções","Origem da Demanda","Situação");
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
	?>
	</td>
	</tr>
</table>
</form>
<script type="text/javascript">
function validaForm(){
	
	if(document.formOrigem.orddescricao.value == ''){
		alert ('O campo Origem da Demanda deve ser preenchido.');
		document.formOrigem.orddescricao.focus();
		return false;
	}
	if(document.formOrigem.ordstatus.value == ''){
		alert ('Informe o Status.');
		document.formOrigem.ordstatus.focus();
		return false;
	}
/*	
	if(document.formOrigem.usucpf.value == ''){
		alert ('Informe o Nome do Responsável.');
		document.formOrigem.usucpf.focus();
		return false;
	}
	if(document.formOrigem.equipe.value == ''){
		alert ('Informe o(s) Membro(s) da Equipe.');
		document.formOrigem.equipe.focus();
		return false;
	}
*/	
}

function resp_tel(y) {
	var tel = y;
	if(tel){
	ntel = document.getElementById('novotel');
	atel = document.getElementById('antigotel');
	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('demandas.php?modulo=sistema/apoio/origemDemanda&acao=A', {
							        method:     'post',
							        parameters: '&telAjax=' + tel,
							        onComplete: function (res)
							        {			
										ntel.innerHTML = res.responseText;
										atel.style.display = 'none';
							        }
							  });
	}
}

function resp_email(x) {
	var cpf = x;
	if(cpf){
	nemail = document.getElementById('novoemail');
	aemail = document.getElementById('antigoemail');
	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('demandas.php?modulo=sistema/apoio/origemDemanda&acao=A', {
							        method:     'post',
							        parameters: '&emailAjax=' + cpf,
							        onComplete: function (res)
							        {			
										nemail.innerHTML = res.responseText;
										aemail.style.display = 'none';
							        }
							  });
	}
}

function processaCombosPopup()
{
   selectAllOptions( document.getElementById( 'equipe' ) );
}

function exclusao(url) {
	var questao = confirm("Tem Certeza?")
	if (questao){
		window.location = url;
	}
}
</script>
</body>