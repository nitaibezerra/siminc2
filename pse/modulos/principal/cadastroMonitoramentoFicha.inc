<?php

//recupera o entid
$entid = $_SESSION['pse']['entid'];

//cria sessao id da avaliacao
$_SESSION['pse']['avlid'] = $_REQUEST['avlid'] ? $_REQUEST['avlid'] : $_SESSION['pse']['avlid'];


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
if($_REQUEST['exluir'] && $_REQUEST['fieid']){
	
	//verifica se ja existe questionario
	$sql = "SELECT count(acpid) as total from pse.acompanhamento WHERE fieid = ".$_REQUEST['fieid'];
	$totalQuest = $db->pegaUm($sql);
	
	if($totalQuest > 0){
		print "<script>
			alert('Não é possível excluir, pois existe Questionário vinculado nesta Ficha do Educando!');
			history.back();
		   </script>";
	}else{
		$sql = "DELETE FROM pse.fichaeducando WHERE fieid = ".$_REQUEST['fieid'];
		$db->executar($sql);					
		$db->commit();
		
		print "<script>
				alert('Operação realizada com sucesso!');
				location.href='pse.php?modulo=principal/cadastroMonitoramentoFicha&acao=A&avlid={$_SESSION['pse']['avlid']}';
			   </script>";
	}			  	
	
	exit();
	
}


//submit
if($_POST){
	
	if(!$_POST['fiedataavaliacao']){
		?>
		<script>
			alert("Sua sessão expirou. Por favor, entre novamente!");
			location.href='pse.php?modulo=principal/cadastroMonitoramento&acao=A';
		</script>
		<?
		exit;
	}
	
	if(!$_POST['fiedatanascimento']){
		?>
		<script>
			alert("Informe a Data de Nascimento!");
			history.back();
		</script>
		<?
		exit;
	}
	
	//trata campo
	if($_POST['fieoutro'] == 'O'){
		$_POST['ubsid'] = "";
		$_POST['scn_cneid'] = "";
	}elseif($_POST['fieoutro'] == 'E'){
		$_POST['ubsid'] = "";
	}elseif($_POST['fieoutro'] == 'U'){
		$_POST['scn_cneid'] = "";
	}
	
		
	if(!$_POST['fieid']) {
		inserirDados();
	}
	else{
		alterarDados($_POST['fieid']);
	}
	
	unset($_POST);
	
	print "<script>
			alert('Operação realizada com sucesso!');
			location.href='pse.php?modulo=principal/cadastroMonitoramentoFicha&acao=A&avlid={$_SESSION['pse']['avlid']}';
		   </script>";
	exit();
	
}



function inserirDados(){
	global $db;
	
	$sql = "INSERT INTO pse.fichaeducando
			(
			  	avlid,
			  	racid,
			  	ubsid,
			  	cneid,
			  	scn_cneid,
			  	fiedataavaliacao,
				fiedatanascimento,
			  	fiematricula,
			  	fienomecompleto,
			  	fienomedamae,
			  	fieinicialnome,
			  	fiecartaosus,
			  	fienumeronis,
			  	fieoutro,
			  	fiesexo,
			  	fiebolsafamilia,
			  	fieobesidadedesnutricao,
			  	fieequipeescola
	        ) VALUES (
	        	".$_SESSION['pse']['avlid'].",
	        	".($_POST['racid'] ? $_POST['racid'] : 'null').",
	        	".($_POST['ubsid'] ? $_POST['ubsid'] : 'null').",
	        	".($_POST['cneid'] ? $_POST['cneid'] : 'null').",
	        	".($_POST['scn_cneid'] ? $_POST['scn_cneid'] : 'null').",
	        	'".formata_data_sql($_POST['fiedataavaliacao'])."',
	        	'".formata_data_sql($_POST['fiedatanascimento'])."',
	        	".($_POST['fiematricula'] ? "'".$_POST['fiematricula']."'" : 'null').",
	        	'".$_POST['fienomecompleto']."',
	        	'".$_POST['fienomedamae']."',
	        	'".strtoupper(substr($_POST['fienomecompleto'],0,1))."',
	        	'".$_POST['fiecartaosus']."',
	        	'".$_POST['fienumeronis']."',
	        	'".$_POST['fieoutro']."',
	        	'".$_POST['fiesexo']."',
	        	".($_POST['fiebolsafamilia'] ? "'".$_POST['fiebolsafamilia']."'" : 'null').",
	        	".($_POST['fieobesidadedesnutricao'] ? "'".$_POST['fieobesidadedesnutricao']."'" : 'null').",
	        	".($_POST['fieequipeescola'] ? "'".$_POST['fieequipeescola']."'" : 'null')."
	        )";	

	$db->executar($sql);
	$db->commit();
	
}



function alterarDados($fieid){
	global $db;
	
	$sql = "UPDATE 
				pse.fichaeducando
			SET 
				racid = ".($_POST['racid'] ? $_POST['racid'] : 'null').",
				ubsid = ".($_POST['ubsid'] ? $_POST['ubsid'] : 'null').",
				cneid = ".($_POST['cneid'] ? $_POST['cneid'] : 'null').",
				scn_cneid = ".($_POST['scn_cneid'] ? $_POST['scn_cneid'] : 'null').",
	            fiedataavaliacao = '".formata_data_sql($_POST['fiedataavaliacao'])."',
	            fiedatanascimento = '".formata_data_sql($_POST['fiedatanascimento'])."',
				fiematricula = ".($_POST['fiematricula'] ? "'".$_POST['fiematricula']."'" : 'null').",
				fienomecompleto = '".$_POST['fienomecompleto']."',
				fienomedamae = '".$_POST['fienomedamae']."',
				fieinicialnome = '".strtoupper(substr($_POST['fienomecompleto'],0,1))."',
				fiecartaosus = '".$_POST['fiecartaosus']."',
				fienumeronis = '".$_POST['fienumeronis']."',
				fieoutro = '".$_POST['fieoutro']."',
				fiesexo = '".$_POST['fiesexo']."',
				fiebolsafamilia = ".($_POST['fiebolsafamilia'] ? "'".$_POST['fiebolsafamilia']."'" : 'null').",
				fieobesidadedesnutricao = ".($_POST['fieobesidadedesnutricao'] ? "'".$_POST['fieobesidadedesnutricao']."'" : 'null').",
				fieequipeescola = ".($_POST['fieequipeescola'] ? "'".$_POST['fieequipeescola']."'" : 'null')."
			WHERE 
				fieid = ".$fieid;

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
			  2 => array("id" => 3, "descricao" => "Ficha de Acompanhamento", 	"link" => $_SERVER['REQUEST_URI'])
			  //3 => array("id" => 4, "descricao" => "Questionário", 	"link" => "/pse/pse.php?modulo=principal/cadastroQuestionario&acao=A")
		  	  );
		
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);


//carrega dados para edição
if($_REQUEST['fieid']){
	$sql = "SELECT  
				fieid,
			  	racid,
			  	ubsid,
			  	cneid,
			  	scn_cneid,
			  	fiedataavaliacao,
			  	fiematricula,
			  	fienomecompleto,
			  	fienomedamae,
			  	fieinicialnome,
			  	fiedatanascimento,
			  	fiecartaosus,
			  	fienumeronis,
			  	fieoutro,
			  	fiesexo,
			  	fiebolsafamilia,
			  	fieobesidadedesnutricao,
			  	fieequipeescola
		     FROM 
		     		pse.fichaeducando
		     WHERE 
		     		fieid = {$_REQUEST['fieid']}";
	$dados = $db->carregar($sql);
	if($dados) extract($dados[0]);
}

$limiteTotalFichas = $db->pegaUm("select avlidentifica1 from pse.avaliacao where avlid = ".$_SESSION['pse']['avlid']);
$totalFichas = $db->pegaUm("select count(fieid) as total from pse.fichaeducando where avlid = ".$_SESSION['pse']['avlid']);
if(!$totalFichas) $totalFichas = 0;

?>

<script src="../includes/calendario.js"></script>


<form id="formulario" name="formulario" action="" method="post">

<input type="hidden" name="fieid" value="<?=$fieid?>">



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
		 	<b>
		 		<?
		 		if($_SESSION['pse']['comid'] == '1'){
		 			echo "COMPONENTE I - AVALIAÇÃO CLÍNICA E PSICOSOCIAL";
		 		}
		 		else{
		 			echo "COMPONENTE II - PROMOÇÃO E PREVENÇÃO À SAÚDE";
		 		}
		 		?>
		 	</b>
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
			 <span style="font-size: 12"><b>FICHA DO EDUCANDO</b></span>
			 </center>
		</td>
	</tr>
	<tr>
		<td width="40%" class="SubTituloDireita" colspan='2'>Data da avaliação:</td>
		<td colspan='2'>
			<?
			$fiedataavaliacao = $db->pegaUm("select to_char(avldataavaliacao::date, 'DD/MM/YYYY') as dataaval from pse.avaliacao where avlid = ".$_SESSION['pse']['avlid']);
			echo $fiedataavaliacao;
			?>
			<input type="hidden" name="fiedataavaliacao" value="<?=$fiedataavaliacao?>">
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan='2'>Matrícula:</td>
		<td colspan='2'>
			<?
				echo campo_texto( 'fiematricula', 'N', $vPermissao, '', 24, 20, '', '', '', '', 0, '');
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan='2'>Nome Completo do Educando:</td>
		<td colspan='2'>
			<?
				echo campo_texto( 'fienomecompleto', 'S', $vPermissao, '', 60, 100, '', '', '', '', 0, '');
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan='2'>Data de Nascimento:</td>
		<td colspan='2'>
			<?=campo_data('fiedatanascimento', 'S', $vPermissao, '', 'S' );?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan='2'>Nome da Mãe:</td>
		<td colspan='2'>
			<?
				echo campo_texto( 'fienomedamae', 'S', $vPermissao, '', 60, 100, '', '', '', '', 0, '');
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan='2'>Raça / Cor:</td>
		<td colspan='2'>
			<?
				$sql_combo = "SELECT
								racid as codigo,
								racdescricao as descricao
							  FROM pse.racacor
							  order by 2";
				$racacor = $db->carregar($sql_combo);
				$db->monta_combo("racid", $sql_combo, $vPermissao, "Selecione a Raça/Cor", '', '', '', '', 'N', 'racid');
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan='2'>Número do Cartão SUS:</td>
		<td colspan='2'>
			<?
				echo campo_texto( 'fiecartaosus', 'N', $vPermissao, '', 24, 20, '', '', '', '', 0, '');
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan='2'>Número do NIS:</td>
		<td colspan='2'>
			<?
				echo campo_texto( 'fienumeronis', 'N', $vPermissao, '', 24, 20, '', '', '', '', 0, '');
			?>
		</td>
	</tr>
	<?if( in_array($_SESSION['pse']['acaoid'], array(5,8)) ){ ?>
	<tr>
		<td class="SubTituloDireita" colspan='2'>Equipe que Realizou a Ação:</td>
		<td colspan='2'>
			<?
				$avlequipeescola = $db->pegaUm("select avlequipeescola from pse.avaliacao where avlid = ".$_SESSION['pse']['avlid']);
				
				if($avlequipeescola == 'N'){
			
					echo "Equipe da Escola";
					echo '<input type="hidden" name="cneid" value="">';

				}else{
					
					$dadosEq = $db->pegaLinha("select cneid, cnenomefantasia, cnecodigocnes, cneseqequipe from pse.scnes where cneid in (select cneid from pse.avaliacao where avlid = ".$_SESSION['pse']['avlid'].")");
					echo $dadosEq['cnecodigocnes'] .' - '. $dadosEq['cnenomefantasia'] .' - '. $dadosEq['cneseqequipe'];
					echo '<input type="hidden" name="cneid" value="'.$dadosEq["cneid"].'">'; 
					
				}
				
				echo '<input type="hidden" name="fieequipeescola" value="'.$avlequipeescola.'">';
				
				/*
				$sql_combo = "SELECT
								cneid as codigo,
								cnenomefantasia as descricao
							  FROM pse.scnes
							  where muncod = '".$_SESSION['pse']['muncod']."'
							  order by 2";
				$equipeesf = $db->carregar($sql_combo);
				$db->monta_combo("cneid", $sql_combo, $vPermissao, "Selecione a Equipe ESF", '', '', '', '', 'S', 'cneid');
				*/
			?>
		</td>
	</tr>
	<?}else{?>
	<tr>
		<td class="SubTituloDireita" colspan='2'>ESF - Equipe de Saúde da Família que Realizou a Ação:</td>
		<td colspan='2'>
			<?
				$dadosEq = $db->pegaLinha("select cneid, cnenomefantasia, cnecodigocnes, cneseqequipe from pse.scnes where cneid in (select cneid from pse.avaliacao where avlid = ".$_SESSION['pse']['avlid'].")");
				echo $dadosEq['cnecodigocnes'] .' - '. $dadosEq['cnenomefantasia'] .' - '. $dadosEq['cneseqequipe'];
				echo '<input type="hidden" name="cneid" value="'.$dadosEq["cneid"].'">'; 

				/*
				$sql_combo = "SELECT
								cneid as codigo,
								cnenomefantasia as descricao
							  FROM pse.scnes
							  where muncod = '".$_SESSION['pse']['muncod']."'
							  order by 2";
				$equipeesf = $db->carregar($sql_combo);
				$db->monta_combo("cneid", $sql_combo, $vPermissao, "Selecione a Equipe ESF", '', '', '', '', 'S', 'cneid');
				*/
			?>
		</td>
	</tr>
	<?}?>
	<tr>
		<td class="SubTituloDireita" colspan='2'>Responsável pelo Cuidado do Educando:</td>
		<td colspan='2'>
			<input type="radio" name="fieoutro" id="fieoutro" value="E" <?if($fieoutro == 'E') echo "checked";?> onclick="document.formulario.ubsid.value='';document.getElementById('combo_scn_cneid').style.display='';document.getElementById('combo_ubsid').style.display='none';"> ESF
			&nbsp;&nbsp;
			<span id="combo_scn_cneid" style="display: <?if($fieoutro != 'E') echo "none";?>">
			<?
				$sql_combo = "SELECT
								cneid as codigo,
								cnecodigocnes ||' - '|| cnenomefantasia ||' - '|| cneseqequipe as descricao
							  FROM pse.scnes
							  where muncod = '".$_SESSION['pse']['muncod']."'
							  order by 2";
				$responsavelesf = $db->carregar($sql_combo);
				$db->monta_combo("scn_cneid", $sql_combo, $vPermissao, "Selecione a Equipe ESF", '', '', '', '', 'N', 'scn_cneid');
			
			?>
			</span>
			
			<br>
			
			<input type="radio" name="fieoutro" id="fieoutro" value="U" <?if($fieoutro == 'U') echo "checked";?> onclick="document.formulario.scn_cneid.value='';document.getElementById('combo_ubsid').style.display='';document.getElementById('combo_scn_cneid').style.display='none';"> UBS
			&nbsp;&nbsp;
			<span id="combo_ubsid" style="display: <?if($fieoutro != 'U') echo "none";?>">
			<?
				$sql_combo = "SELECT
								ubsid as codigo,
								ubsnomefantasia as descricao
							  FROM pse.unidadebasicasaude
							  where ubscodigoibge = '".$_SESSION['pse']['muncod']."'
							  order by 2";
				$responsavelubs = $db->carregar($sql_combo);
				$db->monta_combo("ubsid", $sql_combo, $vPermissao, "Selecione a Equipe UBS", '', '', '', '', 'N', 'ubsid');
			
			?>
			</span>
			
			<br>
			
			<input type="radio" name="fieoutro" id="fieoutro" value="O" <?if($fieoutro == 'O') echo "checked";?> onclick="document.formulario.ubsid.value='';document.formulario.scn_cneid.value='';document.getElementById('combo_scn_cneid').style.display='none';document.getElementById('combo_ubsid').style.display='none';"> ESF ou UBS de outro município
			
			
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan='2'>Sexo:</td>
		<td colspan='2'>
			<input type="radio" name="fiesexo" id="fiesexo" value="M" <?if($fiesexo == 'M') echo "checked";?>> MASCULINO
			<br>
			<input type="radio" name="fiesexo" id="fiesexo" value="F" <?if($fiesexo == 'F') echo "checked";?>> FEMININO
			&nbsp;
			<?echo obrigatorio();?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan='2'>É Beneficiário do Programa Bolsa Família:</td>
		<td colspan='2'>
			<input type="radio" name="fiebolsafamilia" id="fiebolsafamilia" value="S" <?if($fiebolsafamilia == 'S') echo "checked";?>> SIM
			<br>
			<input type="radio" name="fiebolsafamilia" id="fiebolsafamilia" value="N" <?if($fiebolsafamilia == 'N') echo "checked";?>> NÃO
		</td>
	</tr>
	<?if( in_array($_SESSION['pse']['acaoid'], array(1)) ){ ?>
		<tr>
			<td class="SubTituloDireita" colspan='2'>O Educando Apresenta:</td>
			<td colspan='2'>
				<input type="radio" name="fieobesidadedesnutricao" id="fieobesidadedesnutricao" value="O" <?if($fieobesidadedesnutricao == 'O') echo "checked";?>> Obesidade
				<br>
				<input type="radio" name="fieobesidadedesnutricao" id="fieobesidadedesnutricao" value="D" <?if($fieobesidadedesnutricao == 'D') echo "checked";?>> Desnutrição
				&nbsp;
				<?echo obrigatorio();?>
			</td>
		</tr>
	<?}?>
	<tr>
		<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='4'>
			<table width="100%">
				<tr>
					<td width="55%" align="right">
						<?if($vPermissao == 'S'){?>
						<input type="button" class="botao" name="btSalvarC1" id="btSalvarC1" value="Salvar" onclick="validaFormFicha();">
						<?}?>
						&nbsp;
						<input type="button" class="botao" name="btVoltarC1" id="btVoltarC1" value="Voltar" onclick="location.href='pse.php?modulo=principal/cadastroMonitoramentoAcao&acao=A';">
					</td>
					<td align="right">&nbsp;</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td colspan='4'>
			<div id="listaC1" style="height: 250px; overflow-y: auto; overflow-x: hidden;">
				<?php 
					if($vPermissao == 'S'){
						$img = "<a href=\"#\" onclick=\"alterarFicha(\'' || fe.fieid || '\');\" title=\"Alterar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\" onclick=\"excluirFicha(\'' || fe.fieid || '\');\" title=\"Excluir\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>";
						
						if( in_array($_SESSION['pse']['acaoid'], array(5,6)) ){
							$img .= "&nbsp;<a href=\"#\" onclick=\"cadQuest(\'' || fe.fieid || '\');\" title=\"Responder Questionário\"><img src=\"../imagens/atencao.png\" style=\"cursor:pointer;\" border=\"0\" align=\"top\"></a>";
						}
						
					} else {
						$img = '<img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\">&nbsp;<img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">';
						
						if( in_array($_SESSION['pse']['acaoid'], array(5,6)) ){
							$img .= "&nbsp;<a href=\"#\" onclick=\"cadQuest(\'' || fe.fieid || '\');\" title=\"Responder Questionário\"><img src=\"../imagens/atencao.png\" style=\"cursor:pointer;\" border=\"0\" align=\"top\"></a>";
						}
					}
					
					
					if( in_array($_SESSION['pse']['acaoid'], array(1)) ){

						$sql = "SELECT
								'<center>$img</center>' as acao, 
								to_char(fiedataavaliacao::date, 'DD/MM/YYYY') as c0,
								fiematricula as c1,
				            	fienomecompleto as c2,
				            	to_char(fiedatanascimento::date, 'DD/MM/YYYY') as c3,
				            	fienomedamae as c11,
				            	racdescricao as c4,
								fiecartaosus as c5,
								fienumeronis as c6,
								sc.cnecodigocnes ||' - '|| sc.cnenomefantasia ||' - '|| sc.cneseqequipe as c7,
								(CASE WHEN fiesexo = 'F' THEN 'FEMININO'
									  ELSE 'MASCULINO' 
								END) as c8,
								(CASE WHEN fiebolsafamilia = 'S' THEN 'SIM'
									  ELSE 'NÃO' 
								END) as c9,
								(CASE WHEN fieobesidadedesnutricao = 'O' THEN 'OBESIDADE'
									  ELSE 'DESNUTRIÇÃO' 
								END) as c10
							FROM pse.fichaeducando fe
							LEFT JOIN pse.racacor rc on rc.racid = fe.racid
							LEFT JOIN pse.scnes sc on sc.cneid = fe.cneid
							LEFT JOIN pse.scnes sc2 on sc2.cneid = fe.scn_cneid
							LEFT JOIN pse.unidadebasicasaude ub on ub.ubsid = fe.ubsid
							WHERE fe.avlid = {$_SESSION['pse']['avlid']}
							order by fienomecompleto";
						
						$cabecalho 	= array( "Ação", "Data da Avaliação", "Matrícula","Nome Completo", "Data de Nascimento","Nome da Mãe","Raça / Cor","Número do Cartão SUS","Número do NIS","ESF - Equipe de Saúde da Família que Realizou a Ação","Sexo","É Beneficiário do Programa Bolsa Família","Obesidade ou Desnutrição");
					}
					else{
						
						$sql = "SELECT
								'<center>$img</center>' as acao, 
								to_char(fiedataavaliacao::date, 'DD/MM/YYYY') as c0,
								fiematricula as c1,
				            	fienomecompleto as c2,
				            	to_char(fiedatanascimento::date, 'DD/MM/YYYY') as c3,
				            	fienomedamae as c10,
				            	racdescricao as c4,
								fiecartaosus as c5,
								fienumeronis as c6,
								(CASE WHEN av.avlequipeescola = 'N' THEN
										'Equipe da Escola'
									  ELSE
									  	sc.cnecodigocnes ||' - '|| sc.cnenomefantasia ||' - '|| sc.cneseqequipe
								END) as c7,
								(CASE WHEN fiesexo = 'F' THEN 'FEMININO'
									  ELSE 'MASCULINO' 
								END) as c8,
								(CASE WHEN fiebolsafamilia = 'S' THEN 'SIM'
									  ELSE 'NÃO' 
								END) as c9
							FROM pse.fichaeducando fe
							INNER JOIN pse.avaliacao av on av.avlid = fe.avlid
							LEFT JOIN pse.racacor rc on rc.racid = fe.racid
							LEFT JOIN pse.scnes sc on sc.cneid = av.cneid
							LEFT JOIN pse.scnes sc2 on sc2.cneid = fe.scn_cneid
							LEFT JOIN pse.unidadebasicasaude ub on ub.ubsid = fe.ubsid
							WHERE fe.avlid = {$_SESSION['pse']['avlid']}
							order by fienomecompleto";
						
						if( in_array($_SESSION['pse']['acaoid'], array(5,8)) ){
							$cabecalho 	= array( "Ação", "Data da Avaliação", "Matrícula","Nome Completo", "Data de Nascimento","Nome da Mãe","Raça / Cor","Número do Cartão SUS","Número do NIS","Equipe que Realizou a Ação","Sexo","É Beneficiário do Programa Bolsa Família");
						}
						else{
							$cabecalho 	= array( "Ação", "Data da Avaliação", "Matrícula","Nome Completo", "Data de Nascimento","Nome da Mãe","Raça / Cor","Número do Cartão SUS","Número do NIS","ESF - Equipe de Saúde da Família que Realizou a Ação","Sexo","É Beneficiário do Programa Bolsa Família");
						}
					}
					
					
					$alinha = array("center","center","left","left","center","left","left","left","left","left","center","center","center");
					$tamanho = array("5%","10%","10%","10%","10%","10%","10%","10%","10%","10%","10%","10%","10%");
					$db->monta_lista( $sql, $cabecalho, 500, 10, 'N', 'center', '', '',$tamanho,$alinha);

														
				?>
			<div>
		</td>
	</tr>
</table>


</form>

</body>
</html>

<script>

function validaFormFicha()
{
	var d = document.formulario;
	
	var totalFichas = "<?=$totalFichas?>";
	var limiteTotalFichas = "<?=$limiteTotalFichas?>";
	
	if(totalFichas == limiteTotalFichas && d.fieid.value==''){
		alert('Quantidade de fichas de educandos já é igual a quantidade informada na avaliação.');
		return false;
	}
	
	/*
	if(d.fiedataavaliacao.value == ''){
		alert('Favor Preencher o campo obrigatório: Data da avaliação.');
		d.fiedataavaliacao.focus();
		return false;
	}
	*/
	/*
	if(d.fiematricula.value == ''){
		alert('Favor Preencher o campo obrigatório: Matrícula.');
		d.fiematricula.focus();
		return false;
	}
	*/
	if(d.fienomecompleto.value == ''){
		alert('Favor Preencher o campo obrigatório: Nome Completo do Educando.');
		d.fienomecompleto.focus();
		return false;
	}
	if(d.fiedatanascimento.value == ''){
		alert('Favor Preencher o campo obrigatório: Data de Nascimento.');
		d.fiedatanascimento.focus();
		return false;
	}
	
	var dtnascimento = d.fiedatanascimento.value.substr(6,4) + d.fiedatanascimento.value.substr(3,2) + d.fiedatanascimento.value.substr(0,2);
	var dtatual = "<?=date('Ymd')?>";

	//Validação de data retirada a pedido do Analista Juvenal na data de 01/11/2012. 
	//De acordo com o analista, é necessário somente a validação de preenchimento da data. Já realizado no trecho de código acima.
	/*
	if( parseFloat(dtnascimento) <  parseFloat('19900101') ){
		alert('Data de Nascimento não pode ser menor que 01/01/1990.');
		d.fiedatanascimento.focus();
		return false;
	}
	*/
	/*
	if( parseFloat(dtavaliacao) >  parseFloat(dtatual) ){
		alert('Data da avaliação não pode ser maior que a data de hoje.');
		d.avldataavaliacao.focus();
		return false;
	}
	*/
	
	if(d.fienomedamae.value == ''){
		alert('Favor Preencher o campo obrigatório: Nome da Mãe.');
		d.fienomedamae.focus();
		return false;
	}
	/*
	if(d.racid.value == ''){
		alert('Favor Preencher o campo obrigatório: Raça / Cor.');
		d.racid.focus();
		return false;
	}
	*/
	/*
	if(d.fiecartaosus.value == ''){
		alert('Favor Preencher o campo obrigatório: Número do Cartão SUS.');
		d.fiecartaosus.focus();
		return false;
	}
	if(d.fienumeronis.value == ''){
		alert('Favor Preencher o campo obrigatório: Número do NIS.');
		d.fienumeronis.focus();
		return false;
	}
	*/
	<?if( !in_array($_SESSION['pse']['acaoid'], array(5,8)) ){ ?>
		if(d.cneid.value == ''){
			alert('Favor Preencher o campo obrigatório: ESF - Equipe de Saúde da Família que Realizou a Ação.');
			d.cneid.focus();
			return false;
		}
	<?}?>	
	
	/*
	if(d.fieoutro[0].checked == false && d.fieoutro[1].checked == false && d.fieoutro[2].checked == false){
		alert('Favor Preencher o campo obrigatório: Responsável pelo Cuidado do Educando.');
		d.fieoutro[0].focus();
		return false;
	}
	*/
	if(d.fieoutro[0].checked == true && d.scn_cneid.value == ''){
		alert('Favor Preencher o campo obrigatório: Responsável pelo Cuidado do Educando - Selecione a Equipe ESF.');
		d.scn_cneid.focus();
		return false;
	}
	if(d.fieoutro[1].checked == true && d.ubsid.value == ''){
		alert('Favor Preencher o campo obrigatório: Responsável pelo Cuidado do Educando - Selecione a Equipe UBS.');
		d.ubsid.focus();
		return false;
	}
	
	if(d.fiesexo[0].checked == false && d.fiesexo[1].checked == false){
		alert('Favor Preencher o campo obrigatório: Sexo.');
		d.fiesexo[0].focus();
		return false;
	}
	/*
	if(d.fiebolsafamilia[0].checked == false && d.fiebolsafamilia[1].checked == false){
		alert('Favor Preencher o campo obrigatório: É Beneficiário do Programa Bolsa Família.');
		d.fiebolsafamilia[0].focus();
		return false;
	}
	*/
	<?if( in_array($_SESSION['pse']['acaoid'], array(1)) ){ ?>
		if(d.fieobesidadedesnutricao[0].checked == false && d.fieobesidadedesnutricao[1].checked == false){
			alert('Favor Preencher o campo obrigatório: O Educando Apresenta.');
			d.fieobesidadedesnutricao[0].focus();
			return false;
		}
	<?}?>

	d.submit();
	
}

function cadQuest(id){
	location.href='pse.php?modulo=principal/cadastroMonitoramentoQuestionario&acao=A&fieid='+id;
}


function alterarFicha(id){
	location.href='pse.php?modulo=principal/cadastroMonitoramentoFicha&acao=A&fieid='+id;
}


function excluirFicha(id){

	if(confirm('Deseja realmente excluir este item?')){
		location.href='pse.php?modulo=principal/cadastroMonitoramentoFicha&acao=A&exluir=1&fieid='+id;
	}
}

</script>
