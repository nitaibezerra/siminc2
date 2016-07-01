<?php

//recupera o entid
$entid = $_SESSION['pse']['entid'];


//pega id da ficha educando
$fieid = $_REQUEST['fieid'] ? $_REQUEST['fieid'] : $_POST['fieid'];


if(!$_SESSION['pse']['avlid'] || !$_SESSION['pse']['entid'] || !$_SESSION['pse']['comid'] || !$_SESSION['pse']['acaoid']){
	?>
	<script>
		alert("Sua sessão expirou. Por favor, entre novamente!");
		location.href='pse.php?modulo=principal/cadastroMonitoramento&acao=A';
	</script>
	<?
	exit;
}



//verifica permissão
$vpflcod = pegaPerfil($_SESSION['usucpf']);
if($vpflcod == SUPER_USUARIO || $vpflcod == ESCOLA_MUNICIPAL || $vpflcod == ESCOLA_ESTADUAL){
	$vPermissao = 'S';
	
	//verifica se a avaliacao foi validada
	/*
	$sql = "select avlvalidar from pse.avaliacao where avlid = ".$_SESSION['pse']['avlid'];
	$avlvalidar = $db->pegaUm($sql);
	if($avlvalidar == 'S') $vPermissao = 'N';
	*/
	

}
else{
	$vPermissao = 'N';
}


//apos 31 de novembro e diferente de monitoramento de ações, permite so vizualizar os dados
if( date('Ymd') > 20121130 && $_SESSION['pse']['sse'] != 'N' ) $vPermissao = 'N';


//excluir registro
if($_REQUEST['excluir'] && $_REQUEST['fieid']){
	
	$sql = "DELETE FROM pse.acompanhamento WHERE fieid = ".$_REQUEST['fieid'];
	$db->executar($sql);					
	$db->commit();
	
	print "<script>
			alert('Operação realizada com sucesso!');
			location.href='pse.php?modulo=principal/cadastroMonitoramentoQuestionario&acao=A&fieid={$fieid}';
		   </script>";			  	
	
	exit();
	
}


//submit
if($_POST){
	
	//trata campos
	if($_POST['acppergunta1'] == 'N'){
		$_POST['acppergunta2'] = '';
		$_POST['acppergunta3'] = '';
		$_POST['acppergunta4'] = '';
	}
	if($_POST['acppergunta2'] == 'N'){
		$_POST['acppergunta3'] = '';
		$_POST['acppergunta4'] = '';
	}
	if($_POST['acppergunta3'] == 'N'){
		$_POST['acppergunta4'] = '';
	}
	
		
	if(!$_POST['acpid']) {
		inserirDados();
	}
	else{
		alterarDados($_POST['acpid']);
	}
		
	unset($_POST);
	
	print "<script>
			alert('Operação realizada com sucesso!');
			location.href='pse.php?modulo=principal/cadastroMonitoramentoQuestionario&acao=A&fieid={$fieid}';
		   </script>";
	exit();
	
}



function inserirDados(){
	global $db;
	
	$sql = "INSERT INTO pse.acompanhamento
			(
			  	fieid,
			  	acaoid,
			  	comid,
			  	acppergunta1,
			  	acppergunta2,
			  	acppergunta3,
			  	acppergunta4 
	        ) VALUES (
	        	".$_POST['fieid'].",
	        	".$_SESSION['pse']['acaoid'].",
	        	".$_SESSION['pse']['comid'].",
	        	".($_POST['acppergunta1'] ? "'".$_POST['acppergunta1']."'" : 'null').",
	        	".($_POST['acppergunta2'] ? "'".$_POST['acppergunta2']."'" : 'null').",
	        	".($_POST['acppergunta3'] ? "'".$_POST['acppergunta3']."'" : 'null').",
	        	".($_POST['acppergunta4'] ? "'".$_POST['acppergunta4']."'" : 'null')."
	        )";	

	$db->executar($sql);	
	$db->commit();	
	
}



function alterarDados($acpid){
	global $db;
	
	$sql = "UPDATE 
				pse.acompanhamento
			SET 
	            acppergunta1 = ".($_POST['acppergunta1'] ? "'".$_POST['acppergunta1']."'" : 'null').",
				acppergunta2 = ".($_POST['acppergunta2'] ? "'".$_POST['acppergunta2']."'" : 'null').",
				acppergunta3 = ".($_POST['acppergunta1'] ? "'".$_POST['acppergunta3']."'" : 'null').",
				acppergunta4 = ".($_POST['acppergunta1'] ? "'".$_POST['acppergunta4']."'" : 'null')."
			WHERE 
				acpid = ".$acpid;
		
	$db->executar($sql);					
	$db->commit();
}











// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

echo "<br>";

/*
$menu = array(0 => array("id" => 1, "descricao" => "Termo de Compromisso", 			"link" => "/pse/pse.php?modulo=principal/cadastroTermoCompromisso&acao=A"),
			  1 => array("id" => 2, "descricao" => "Monitoramento de Ações", 	"link" => $_SERVER['REQUEST_URI']),
			  2 => array("id" => 3, "descricao" => "Semana Saúde na Escola", 	"link" => "/pse/pse.php?modulo=principal/cadastroTermoSemana&acao=A")
		  	  );
		
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
*/

if($_SESSION['pse']['sse'] == 'S'){
	$titulo = "Monitoramento das Ações da Semana Saúde na Escola";
}elseif($_SESSION['pse']['sse'] == 'N'){
	$titulo = "Monitoramento das Ações do PSE na Escola";
}elseif($_SESSION['pse']['sse'] == 'B'){
	$titulo = "Monitoramento das Ações do Brasil Carinhoso";
}
$dsctitulo = '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.';

monta_titulo( $titulo, $dsctitulo );

Cabecalho($entid,1);

echo "<br>";

//$db->cria_aba( $abacod_tela, $url, '');

$menu = array(0 => array("id" => 1, "descricao" => "Lista de Ações", 			"link" => "/pse/pse.php?modulo=principal/cadastroMonitoramento&acao=A"),
			  1 => array("id" => 2, "descricao" => "Avaliações", 	"link" => "/pse/pse.php?modulo=principal/cadastroMonitoramentoAcao&acao=A"),
			  2 => array("id" => 3, "descricao" => "Ficha de Acompanhamento", 	"link" => "/pse/pse.php?modulo=principal/cadastroMonitoramentoFicha&acao=A"),
			  3 => array("id" => 4, "descricao" => "Questionário", 	"link" => $_SERVER['REQUEST_URI'])
		  	  );
		
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);


//carrega dados para edição
$sql = "SELECT  
			  acpid,
			  acppergunta1,
			  acppergunta2,
			  acppergunta3,
			  acppergunta4,
			  to_char(fiedataavaliacao::date, 'DD/MM/YYYY') as fiedataavaliacao,
			  fiematricula,
			  fienomecompleto,
			  to_char(fiedatanascimento::date, 'DD/MM/YYYY') as fiedatanascimento
	     FROM 
	     		pse.acompanhamento a
	     RIGHT JOIN pse.fichaeducando f ON f.fieid = a.fieid
	     WHERE 
	     		f.fieid = {$fieid}";

$dados = $db->carregar($sql);
if($dados) extract($dados[0]);


$mostraTrPergunta2 = 'none';
$mostraTrPergunta3 = 'none';
$mostraTrPergunta4 = 'none';
	
if($acppergunta1 == 'S') $mostraTrPergunta2 = '';
if($acppergunta2 == 'S') $mostraTrPergunta3 = '';
if($acppergunta3 == 'S') $mostraTrPergunta4 = '';	

?>

  

<form id="formulario" name="formulario" action="" method="post">

<input type="hidden" name="fieid" value="<?=$fieid?>">
<input type="hidden" name="acpid" value="<?=$acpid?>">

<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
<tr>
	<td >
		<table border="0" width="100%" >
			<tr>
				<td class="SubTituloCentro" valign="top" align="center">
					 <br>
					 <font color="red">
				     <b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; O cadastro escola deve ser preenchido pelos diretores ou por representantes do PSE na escola,</b>
				     <br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; por este indicado e/ou por membros das Equipes de Saúde da Família. <br>
				     </font>
				     <br>
				</td>
			</tr>
	    </table>
	     
	</td>
</tr>
<tr>
	<td style="background: rgb(168, 168, 168);" colspan='3'>
		 <center>
		 <span style="font-size: 12">
		 	<b>COMPONENTE I - AVALIAÇÃO CLÍNICA E PSICOSOCIAL</b>
		 </span>
		 </center>
	</td>
</tr>
<tr>
	<td align="center" style="background-color: #ffffff">
		<table width="100%" align="center" cellpadding="2" cellspacing="2">
			<tr>
				<td colspan='4'  class="SubTituloEsquerda">
					<b>
						<?=$db->pegaUm("select acaodescricao from pse.acao where acaoid = ".$_SESSION['pse']['acaoid']);?>	
					</b>
				</td>
			</tr>
		</table>
	</td>
</tr>
</table>

<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
	<tr>
		<td style="background: rgb(168, 168, 168);" colspan='4'>
		 	<center>
			 <span style="font-size: 12"><b>FICHA DE ACOMPANHAMENTO DO EDUCANDO <BR> <FONT style="font-size: 9px">ACOMPANHE SEU EDUCANDO. INFORME-SE SOBRE:</FONT></b></span>
			 </center>
		</td>
	</tr>
	<tr>
		<td width="40%" class="SubTituloDireita" colspan='2'>Data da avaliação:</td>
		<td colspan='2'>
			<?=$fiedataavaliacao?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan='2'>Matrícula:</td>
		<td colspan='2'>
			<?=$fiematricula?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan='2'>Nome do Educando:</td>
		<td colspan='2'>
			<?=$fienomecompleto?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan='2'>Data de Nascimento:</td>
		<td colspan='2'>
			<?=$fiedatanascimento?>
		</td>
	</tr>

	<?
	if( in_array($_SESSION['pse']['acaoid'], array(5)) ){
	?>
		<tr>
			<td width="50%" class="SubTituloDireita" colspan='2'>O educando foi atendido pelo oftalmologista?</td>
			<td colspan='2'>
				<input type="radio" name="acppergunta1" value="S" onclick="mostraLinha(1);" <?if($acppergunta1 == 'S') echo "checked";?>> SIM
				&nbsp;&nbsp;&nbsp;
				<input type="radio" name="acppergunta1" value="N" onclick="escondeLinha(1);" <?if($acppergunta1 == 'N') echo "checked";?>> NÃO
			</td>
		</tr>
		<tr id="trPergunta2" style="display: <?=$mostraTrPergunta2?>;">
			<td width="50%" class="SubTituloDireita" colspan='2'>Havia necessidade de óculos?</td>
			<td colspan='2'>
				<input type="radio" name="acppergunta2" value="S" onclick="mostraLinha(2);" <?if($acppergunta2 == 'S') echo "checked";?>> SIM
				&nbsp;&nbsp;&nbsp;
				<input type="radio" name="acppergunta2" value="N" onclick="escondeLinha(2);" <?if($acppergunta2 == 'N') echo "checked";?>> NÃO
			</td>
		</tr>
		<tr id="trPergunta3" style="display: <?=$mostraTrPergunta3?>;">
			<td width="50%" class="SubTituloDireita" colspan='2'>Recebeu óculos?</td>
			<td colspan='2'>
				<input type="radio" name="acppergunta3" value="S" onclick="mostraLinha(3);" <?if($acppergunta3 == 'S') echo "checked";?>> SIM
				&nbsp;&nbsp;&nbsp;
				<input type="radio" name="acppergunta3" value="N" onclick="escondeLinha(3);" <?if($acppergunta3 == 'N') echo "checked";?>> NÃO
			</td>
		</tr>
		<tr id="trPergunta4" style="display: <?=$mostraTrPergunta4?>;">
			<td width="50%" class="SubTituloDireita" colspan='2'>A consulta foi realizada no consultório itinerante do PSE(carreta)?</td>
			<td colspan='2'>
				<input type="radio" name="acppergunta4" value="S" <?if($acppergunta4 == 'S') echo "checked";?>> SIM
				&nbsp;&nbsp;&nbsp;
				<input type="radio" name="acppergunta4" value="N" <?if($acppergunta4 == 'N') echo "checked";?>> NÃO
			</td>
		</tr>
	<?
	}elseif( in_array($_SESSION['pse']['acaoid'], array(6)) ){
	?>
		<tr>
			<td width="50%" class="SubTituloDireita" colspan='2'>O educando foi atendido pelo especialista?</td>
			<td colspan='2'>
				<input type="radio" name="acppergunta1" value="S" onclick="mostraLinha(1);" <?if($acppergunta1 == 'S') echo "checked";?>> SIM
				&nbsp;&nbsp;&nbsp;
				<input type="radio" name="acppergunta1" value="N" onclick="escondeLinha(1);" <?if($acppergunta1 == 'N') echo "checked";?>> NÃO
			</td>
		</tr>
		<tr id="trPergunta2" style="display: <?=$mostraTrPergunta2?>;">
			<td width="50%" class="SubTituloDireita" colspan='2'>Necessidade de órtese / prótese?</td>
			<td colspan='2'>
				<input type="radio" name="acppergunta2" value="S" onclick="mostraLinha(2);" <?if($acppergunta2 == 'S') echo "checked";?>> SIM
				&nbsp;&nbsp;&nbsp;
				<input type="radio" name="acppergunta2" value="N" onclick="escondeLinha(2);" <?if($acppergunta2 == 'N') echo "checked";?>> NÃO
			</td>
		</tr>
		<tr id="trPergunta3" style="display: <?=$mostraTrPergunta3?>;">
			<td width="50%" class="SubTituloDireita" colspan='2'>Recebeu órtese / prótese?</td>
			<td colspan='2'>
				<input type="radio" name="acppergunta3" value="S" <?if($acppergunta3 == 'S') echo "checked";?>> SIM
				&nbsp;&nbsp;&nbsp;
				<input type="radio" name="acppergunta3" value="N" <?if($acppergunta3 == 'N') echo "checked";?>> NÃO
			</td>
		</tr>
	<?}?>
	<tr>
		<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='4'>
			<table width="100%">
				<tr>
					<td width="55%" align="right">
						<?if($vPermissao == 'S'){?>
							<input type="button" class="botao" name="btSalvarC1" id="btSalvarC1" value="Salvar" onclick="validaFormQuest();">
							<?if($acpid){ ?>
								&nbsp;
								<input type="button" class="botao" name="btExcluirC1" id="btExcluirC1" value="Excluir" onclick="location.href='pse.php?modulo=principal/cadastroMonitoramentoQuestionario&acao=A&fieid=<?=$fieid?>&excluir=ok';">
							<?}?>
						<?}?>
						&nbsp;
						<input type="button" class="botao" name="btVoltarC1" id="btVoltarC1" value="Voltar" onclick="history.back();">
					</td>
					<td align="right">&nbsp;</td>
				</tr>
			</table>
		</td>
	</tr>
</table>


</form>

</body>
</html>

 
<script>

function validaFormQuest(){

	var d = document.formulario;
	
	<?if( in_array($_SESSION['pse']['acaoid'], array(5)) ){ ?>
		if(d.acppergunta1[0].checked == false && d.acppergunta1[1].checked == false){
			alert('Favor Preencher o campo obrigatório: O educando foi atendido pelo oftalmologista?.');
			d.acppergunta1[0].focus();
			return false;
		}
		if(d.acppergunta1[0].checked == true){
			if(d.acppergunta2[0].checked == false && d.acppergunta2[1].checked == false){
				alert('Favor Preencher o campo obrigatório: Havia necessidade de óculos?.');
				d.acppergunta2[0].focus();
				return false;
			}
		}
		if(d.acppergunta2[0].checked == true){
			if(d.acppergunta3[0].checked == false && d.acppergunta3[1].checked == false){
				alert('Favor Preencher o campo obrigatório: Recebeu óculos?.');
				d.acppergunta3[0].focus();
				return false;
			}
		}
		if(d.acppergunta3[0].checked == true){
			if(d.acppergunta4[0].checked == false && d.acppergunta4[1].checked == false){
				alert('Favor Preencher o campo obrigatório: A consulta foi realizada no consultório itinerante do PSE(carreta)?.');
				d.acppergunta4[0].focus();
				return false;
			}
		}
	<?}elseif( in_array($_SESSION['pse']['acaoid'], array(6)) ){ ?>
		if(d.acppergunta1[0].checked == false && d.acppergunta1[1].checked == false){
			alert('Favor Preencher o campo obrigatório: O educando foi atendido pelo especialista?.');
			d.acppergunta1[0].focus();
			return false;
		}
		if(d.acppergunta1[0].checked == true){
			if(d.acppergunta2[0].checked == false && d.acppergunta2[1].checked == false){
				alert('Favor Preencher o campo obrigatório: Necessidade de órtese / prótese?.');
				d.acppergunta2[0].focus();
				return false;
			}
		}
		if(d.acppergunta2[0].checked == true){
			if(d.acppergunta3[0].checked == false && d.acppergunta3[1].checked == false){
				alert('Favor Preencher o campo obrigatório: Recebeu órtese / prótese?.');
				d.acppergunta3[0].focus();
				return false;
			}
		}
	<?}?>
	

	d.submit();

}


function mostraLinha(id){

	if(id == 1){
		document.getElementById("trPergunta2").style.display = '';
	}
	if(id == 2){
		document.getElementById("trPergunta3").style.display = '';
	}
	if(id == 3){
		document.getElementById("trPergunta4").style.display = '';
	}
}

function escondeLinha(id){

	if(id == 1){
		document.getElementById("trPergunta2").style.display = 'none';
		document.getElementById("trPergunta3").style.display = 'none';
		document.getElementById("trPergunta4").style.display = 'none';
	}
	if(id == 2){
		document.getElementById("trPergunta3").style.display = 'none';
		document.getElementById("trPergunta4").style.display = 'none';
	}
	if(id == 3){
		document.getElementById("trPergunta4").style.display = 'none';
	}
}



</script>