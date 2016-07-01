<?php

//recupera o entid
$entid = $_SESSION['pse']['entid'];


if(!$_SESSION['pse']['sse'] || !$_SESSION['pse']['entid'] || !$_SESSION['pse']['comid'] || !$_SESSION['pse']['acaoid']){
	?>
	<script>
		alert("Sua sessão expirou. Por favor, entre novamente!");
		location.href='pse.php?modulo=inicio&acao=C';
	</script>
	<?
	exit;
}


//verifica permissão
$vpflcod = pegaPerfil($_SESSION['usucpf']);
if($vpflcod == SUPER_USUARIO || $vpflcod == ESCOLA_MUNICIPAL || $vpflcod == ESCOLA_ESTADUAL || $vpflcod == EDUCADOR_ESCOLA){
	$vPermissao = 'S';
}
else{
	$vPermissao = 'N';
}

//apos 31 de novembro e diferente de monitoramento de ações, permite so vizualizar os dados
if( date('Ymd') > 20121130 && $_SESSION['pse']['sse'] != 'N' ) $vPermissao = 'N';



//excluir registro
if($_REQUEST['exluir'] && $_REQUEST['avcid']){
	
	$sql = "DELETE FROM pse.avaliacomp2 WHERE avcid = ".$_REQUEST['avcid'];
	$db->executar($sql);					
	$db->commit();
	
	print "<script>
			alert('Operação realizada com sucesso!');
			location.href='pse.php?modulo=principal/cadastroMonitoramentoAcao2&acao=A';
		   </script>";
	exit();
}



//validar registro
if($_REQUEST['validar'] && $_REQUEST['avcid']){
	
	$sql = "UPDATE 
				pse.avaliacomp2
			SET 
	            avcvalidar = '{$_REQUEST['validar']}'
			WHERE 
				avcid = ".$_REQUEST['avcid'];
	$db->executar($sql);					
	$db->commit();
	
	print "<script>
			alert('Operação realizada com sucesso!');
			location.href='pse.php?modulo=principal/cadastroMonitoramentoAcao2&acao=A';
		   </script>";
	exit();
}


//primeiro acesso na ação comp2
//verifica se ja existe
	$sql = "SELECT moeid from pse.monitoraescola
			WHERE entid = ".$entid." and acaoid = ".$_SESSION['pse']['acaoid']." and moeidentificasse = '{$_SESSION['pse']['sse']}'";
	$moeid = $db->pegaUm($sql);
	
	if(!$moeid){
		
		$sql = "select pacquantedutotal, pacquanteduseratend  
						from pse.planoacaoc2
						where entid = $entid";
		$comp2 = $db->pegaLinha($sql);
		$moetotaleducando = ($comp2['pacquantedutotal'] ? $comp2['pacquantedutotal'] : 0);
		$moetotalpactuado = ($comp2['pacquanteduseratend'] ? $comp2['pacquanteduseratend'] : 0);
					
		$entcodent = $db->pegaUm("select entcodent from entidade.entidade where entid = ".$_SESSION['pse']['entid']);
				
		$sql = "INSERT INTO pse.monitoraescola(entid, acaoid, moecodigoinep, moetotaleducando, moetotalpactuado, 
		            moetotalavaliado, moepercentavaliado, moetotalidentificado, moepercentidentificado, 
		            moeidentificasse, moestatus)
		    VALUES (".$entid.",
		    		".$_SESSION['pse']['acaoid'].", 
		    		'".$entcodent."', 
		    		".$moetotaleducando.", 
		    		".$moetotalpactuado.", 
		            0, 
		            0, 
		            0, 
		            0, 
		            '".$_SESSION['pse']['sse']."',
		            'A') returning moeid";
	
		$moeid = $db->pegaUm($sql);		
		$db->commit();
				
	}
	
	//atualiza total educando para semana saude na escola
	if($_SESSION['pse']['sse'] == 'S'){
			$sql = "select 
						moetotaleducando
					from pse.monitoraescola
					where moeid = {$moeid}";
			$monitora = $db->pegaLinha($sql);
			
			if(!$monitora['moetotaleducando'] || $monitora['moetotaleducando'] == 0){
				$sql = "select 
						cenquanteducando
					from pse.censopse
					where entid = {$entid}";
				$moetotaleducando = $db->pegaUm($sql);
				
				if($moetotaleducando > 0){
					$sql = "UPDATE 
								pse.monitoraescola
							SET 
					            moetotaleducando = ".$moetotaleducando."
							WHERE 
								moeid = {$moeid}";
					$db->executar($sql);					
					$db->commit();
				}
			}
				
	}
	
	//atualiza ação: 3.1 - Ação: Formar jovens multiplicadores para atuarem entre pares nas temáticas envolvendo saúde sexual, saúde reprodutiva e prevenção das DST/AIDS e Hepatites Virais
	/*
	if($_SESSION['pse']['acaoid'] == 18){ 
		$moetotalpactuado = 0;
		$sql = "UPDATE 
						pse.monitoraescola
					SET 
			            moetotalpactuado = ".$moetotalpactuado."
					WHERE 
						moeid = {$moeid}";
			$db->executar($sql);					
			$db->commit();
	}
	*/
	//fim primeiro acesso




//submit
if($_POST['botaoacao'] == '3'){
	
		
	if(!$_POST['pmoid']) {
		
		$sql = "INSERT INTO pse.perguntamonitora
			(
			  	moeid,
			  	acaoid,
			  	comid,
			  	pmopergunta1,
			  	pmopergunta2,
			  	pmopergunta3 
	        ) VALUES (
	        	".$_POST['moeid'].",
	        	".$_SESSION['pse']['acaoid'].",
	        	".$_SESSION['pse']['comid'].",
	        	".($_POST['pmopergunta1'] ? "'".$_POST['pmopergunta1']."'" : 'null').",
	        	".($_POST['pmopergunta2'] ? "'".$_POST['pmopergunta2']."'" : 'null').",
	        	".($_POST['pmopergunta3'] ? "'".$_POST['pmopergunta3']."'" : 'null')."
	        )";	

		$db->executar($sql);	
		
	}
	else{
		
		$sql = "UPDATE 
				pse.perguntamonitora
			SET 
	            pmopergunta1 = ".($_POST['pmopergunta1'] ? "'".$_POST['pmopergunta1']."'" : 'null').",
				pmopergunta2 = ".($_POST['pmopergunta2'] ? "'".$_POST['pmopergunta2']."'" : 'null').",
				pmopergunta3 = ".($_POST['pmopergunta3'] ? "'".$_POST['pmopergunta3']."'" : 'null')."
			WHERE 
				pmoid = ".$_POST['pmoid'];
		
		$db->executar($sql);
		
	}
	
	$db->commit();
	
	unset($_POST);
	
	print "<script>
			alert('Operação realizada com sucesso!');
			location.href='pse.php?modulo=principal/cadastroMonitoramentoAcao2&acao=A';
		   </script>";
	exit();
	
}

if($_POST['botaoacao'] == '1'){
	
	
	//verifica o somatorio do Número de educandos que participaram da ação pela 1a. vez (avcquantprimeiravez)
	if($_POST['avcid']) $andSoma = " and avcid not in ({$_POST['avcid']})";
	$sql = "select sum(avcquantprimeiravez) as total 
			from pse.avaliacomp2 
			where avctipo = '1'
			and moeid = {$_POST['moeid']}
			and acaoid = {$_SESSION['pse']['acaoid']}
			$andSoma";
	$somaAvaliados = $db->pegaUm($sql);
	if(!$somaAvaliados) $somaAvaliados = 0;
	$somaAvaliados += (int) $_POST['avcquantprimeiravez'];
	
	if( (int) $somaAvaliados > (int) $_POST['moetotaleducando'] ){
		print "<script>
				alert('Erro: O somatório do Número de educandos que participaram da ação pela 1a. vez ({$somaAvaliados}) não pode ser maior que o total de educandos da escola ({$_POST['moetotaleducando']})');
				history.back();
		   	  </script>";
		exit();
	}
	//fim verifica
	
	
	//trata trimestre
	if($_POST['triid']){

		//verifica se ja existe
		if($_POST['avcid']) $andEdita = "and avcid not in (".$_POST['avcid'].")";
		
		$sql = "select avcid 
				from pse.avaliacomp2 
				WHERE avctipo = '1' 
				and comid = {$_SESSION['pse']['comid']}
				and acaoid = {$_SESSION['pse']['acaoid']}
				and moeid = {$_POST['moeid']}
				and triid = ".$_POST['triid']."
				$andEdita";
		$existe = $db->pegaUm($sql);
		
		if($existe){
			print "<script>
					alert('Já existe um registro neste período, cadastre corretamente o Período da realização da ação!');
					history.back();
				   </script>";
			exit();
		}
	}
	
	//verifica se preencheu as perguntas
		if(!$_POST['pmoid']){
			print "<script>
						alert('É necessário responder as perguntas do Informe.');
						history.back();
					   </script>";
			exit();
		}
	//fim verifica
	
	//verifica se é numerico
		if($_POST['avcquantidade']){
			if(!is_numeric($_POST['avcquantidade'])){
				print "<script>
					alert('Erro: Número inválido! Informe o número correto.');
					history.back();
			   	  </script>";
				exit();	
			}		
		}
		if($_POST['avcquantprimeiravez']){
			if(!is_numeric($_POST['avcquantprimeiravez'])){
				print "<script>
					alert('Erro: Número inválido! Informe o número correto.');
					history.back();
			   	  </script>";
				exit();	
			}		
		}
	//fim verifica		
		
	if(!$_POST['avcid']) {
		$sql = "INSERT INTO pse.avaliacomp2
			(
			  	moeid,
			  	pmoid,
			  	acaoid,
			  	comid,
			  	avcquantidade,
			  	avcquantprimeiravez,
			  	avctipo,
			  	triid
	        ) VALUES (
	        	".$_POST['moeid'].",
	        	".$_POST['pmoid'].",
	        	".$_SESSION['pse']['acaoid'].",
	        	".$_SESSION['pse']['comid'].",
	        	".$_POST['avcquantidade'].",
	        	".$_POST['avcquantprimeiravez'].",
	        	'".$_POST['botaoacao']."',
	        	".$_POST['triid']."
	        )";	
		$db->executar($sql);
	}
	else{
		
		$sql = "UPDATE 
				pse.avaliacomp2
			SET 
	            triid = ".$_POST['triid'].",
				avcquantidade = ".$_POST['avcquantidade'].",
				avcquantprimeiravez = ".$_POST['avcquantprimeiravez']."
			WHERE 
				avcid = ".$_POST['avcid'];
		$db->executar($sql);
	}
		
	$db->commit();
	
	unset($_POST);
	
	print "<script>
			alert('Operação realizada com sucesso!');
			location.href='pse.php?modulo=principal/cadastroMonitoramentoAcao2&acao=A';
		   </script>";
	exit();
	
}

if($_POST['botaoacao'] == '2'){
	
	//verifica o somatorio do Número de educandos que participaram da ação pela 1a. vez (avcquantprimeiravez)
	if($_POST['avcid_2']) $andSoma = " and avcid not in ({$_POST['avcid_2']})";
	$sql = "select sum(avcquantprimeiravez) as total 
			from pse.avaliacomp2 
			where avctipo = '2'
			and moeid = {$_POST['moeid']}
			and acaoid = {$_SESSION['pse']['acaoid']}
			$andSoma";
	$somaAvaliados = $db->pegaUm($sql);
	if(!$somaAvaliados) $somaAvaliados = 0;
	$somaAvaliados += (int) $_POST['avcquantprimeiravez_2'];
	
	if( (int) $somaAvaliados > (int) $_POST['moetotaleducando'] ){
		print "<script>
				alert('Erro: O somatório do Número de educandos que participaram da ação pela 1a. vez ({$somaAvaliados}) não pode ser maior que o total de educandos da escola ({$_POST['moetotaleducando']})');
				history.back();
		   	  </script>";
		exit();
	}
	//fim verifica

	
	//trata trimestre
	if($_POST['triid_2']){
		
		//verifica se ja existe
		if($_POST['avcid_2']) $andEdita = "and avcid not in (".$_POST['avcid_2'].")";
		
		$sql = "select avcid 
				from pse.avaliacomp2 
				WHERE avctipo = '2' 
				and comid = {$_SESSION['pse']['comid']}
				and acaoid = {$_SESSION['pse']['acaoid']}
				and moeid = {$_POST['moeid']}
				and triid = ".$_POST['triid_2']."
				$andEdita";
		$existe = $db->pegaUm($sql);
		
		if($existe){
			print "<script>
					alert('Já existe um registro neste período, cadastre corretamente o Período da realização da ação!');
					history.back();
				   </script>";
			exit();
		}
	}
	
	//verifica se preencheu as perguntas
	if(!$_POST['pmoid']){
		print "<script>
					alert('É necessário responder as perguntas do Informe.');
					history.back();
				   </script>";
		exit();
	}
	
	if(!$_POST['avcid_2']) {
		$sql = "INSERT INTO pse.avaliacomp2
			(
			  	moeid,
			  	pmoid,
			  	acaoid,
			  	comid,
			  	avcquantidade,
			  	avcquantprimeiravez,
			  	avctipo,
			  	triid
	        ) VALUES (
	        	".$_POST['moeid'].",
	        	".$_POST['pmoid'].",
	        	".$_SESSION['pse']['acaoid'].",
	        	".$_SESSION['pse']['comid'].",
	        	".$_POST['avcquantidade_2'].",
	        	".$_POST['avcquantprimeiravez_2'].",
	        	'".$_POST['botaoacao']."',
	        	".$_POST['triid_2']."
	        )";	
		$db->executar($sql);
	}
	else{
		$sql = "UPDATE 
				pse.avaliacomp2
			SET 
	            triid = ".$_POST['triid_2'].",
				avcquantidade = ".$_POST['avcquantidade_2'].",
				avcquantprimeiravez = ".$_POST['avcquantprimeiravez_2']."
			WHERE 
				avcid = ".$_POST['avcid_2'];
		$db->executar($sql);
	}
	
	$db->commit();
	
	unset($_POST);
	
	print "<script>
			alert('Operação realizada com sucesso!');
			location.href='pse.php?modulo=principal/cadastroMonitoramentoAcao2&acao=A';
		   </script>";
	exit();
	
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
}
else{
	$titulo = "Monitoramento das Ações do PSE na Escola";
}
$dsctitulo = '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.';


monta_titulo( $titulo, $dsctitulo );

Cabecalho($entid,1);

echo "<br>";

//$db->cria_aba( $abacod_tela, $url, '');

$menu = array(0 => array("id" => 1, "descricao" => "Lista de Ações", 			"link" => "/pse/pse.php?modulo=principal/cadastroMonitoramento&acao=A"),
			  1 => array("id" => 2, "descricao" => "Avaliações", 	"link" => $_SERVER['REQUEST_URI'])
			  //2 => array("id" => 3, "descricao" => "Ficha de Acompanhamento", 	"link" => "/pse/pse.php?modulo=principal/cadastroMonitoramentoFicha&acao=A"),
			  //3 => array("id" => 4, "descricao" => "Questionário", 	"link" => "/pse/pse.php?modulo=principal/cadastroQuestionario&acao=A")
		  	  );
		
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);


//carrega dados para edição
if($_REQUEST['avcid']){
	$sql = "SELECT
				  avcid,
				  moeid,
				  triid,
				  avcquantidade,
				  avcquantprimeiravez
		     FROM 
		     		pse.avaliacomp2
		     WHERE 
		     		avcid = {$_REQUEST['avcid']}";
	$dados1 = $db->carregar($sql);
	if($dados1) extract($dados1[0]);
}

if($_REQUEST['avcid_2']){
	$sql = "SELECT
				  avcid as avcid_2,
				  moeid,
				  triid as triid_2,
				  avcquantidade as avcquantidade_2,
				  avcquantprimeiravez as avcquantprimeiravez_2
		     FROM 
		     		pse.avaliacomp2
		     WHERE 
		     		avcid = {$_REQUEST['avcid_2']}";
	$dados2 = $db->carregar($sql);
	if($dados2) extract($dados2[0]);
}

$sql = "SELECT
				  pmoid,
				  pmopergunta1,
				  pmopergunta2,
				  pmopergunta3
		     FROM 
		     		pse.perguntamonitora
		     WHERE 
		     		moeid = '{$moeid}'";

$dados3 = $db->carregar($sql);
if($dados3) extract($dados3[0]);


//configura titulos e img para pagina
//COMP 2
if( in_array($_SESSION['pse']['acaoid'], array(9)) ){
	$labelP1 = "A temática de promoção da alimentação e modos de vida saudáveis com a comunidade escolar, considerando os alimentos regionais está contemplada no Projeto Político Pedagógico da escola";
	$labelP2 = "O ambiente escolar favorece as práticas de alimentação saudável";
	$labelP3 = "O ambiente escolar favorece as atividades físicas e corporais";
	$labelTitulo1 = "Número de educandos contemplados com as atividades educativas sobre promoção da alimentação e modos de vida saudáveis com a comunidade escolar, valorizando os alimentos regionais";
}elseif( in_array($_SESSION['pse']['acaoid'], array(10)) ){
	$labelP1 = "Atividades de práticas corporais orientadas, relacionadas à realidade da comunidade, incluídas no cotidiano escolar estão contempladas no Projeto Político Pedagógico da escola";
	$labelP2 = "Adesão dos alunos aos programas de esporte e lazer desenvolvidos nas escolas de jornada ampliada";
	$labelP3 = "Realização de atividades em parceria com as Academias de Saúde";
	$labelTitulo1 = "Número de educandos contemplados em práticas corporais orientadas, relacionadas à realidade da comunidade, incluídas no cotidiano escolar";
}elseif( in_array($_SESSION['pse']['acaoid'], array(11)) ){
	$labelP1 = "Temática da saúde sexual, saúde reprodutiva e prevenção das DST/AIDS e Hepatites Virais contempladas nos Projetos Político-Pedagógicos (PPP)";
	$labelP2 = "Disponibilização de preservativos nas escolas (requer acordo com os pais, responsáveis e comunidade escolar)";
	$labelP3 = "Houve adolescentes e jovens grávidas orientados a procurar a Unidade de Saúde de referência para acompanhamento pré-natal";
	$labelTitulo1 = "Número de educandos contemplados pelas atividades abordando as temáticas da saúde sexual, saúde reprodutiva e prevenção das DST/AIDS e Hepatites Virais no cotidiano da escola";
}elseif( in_array($_SESSION['pse']['acaoid'], array(12)) ){
	$labelP1 = "Inclusão das temáticas dos riscos e danos do uso de álcool, tabaco, crack e outras drogas nos Projetos Político-Pedagógicos (PPP)";
	$labelP2 = "Adolescentes e jovens orientados quanto à rede pública de apoio ao usuário de álcool, tabaco, crack e outras drogas";
	$labelTitulo1 = "Número de educandos que participaram das atividades no cotidiano escolar abordando a temática dos riscos e danos do uso de álcool, tabaco, crack e outras drogas";
}elseif( in_array($_SESSION['pse']['acaoid'], array(13)) ){
	$labelP1 = "Inclusão das temáticas de diversidade sexual, bullyng, homofobia, discriminação e preconceito nos Projetos Político-Pedagógicos (PPP)";
	$labelP2 = "Educandos orientados em relação à importância das relações interpessoais saudáveis no território escolar";
	$labelTitulo1 = "Número de educandos que participaram das atividades no cotidiano da escola abordando as temáticas da diversidade sexual, bullyng, discriminação e preconceito da família e da comunidade";
}elseif( in_array($_SESSION['pse']['acaoid'], array(14)) ){
	$labelP1 = "Educandos orientados à adoção de práticas sustentáveis no ambiente escolar";
	$labelP2 = "Espaço escolar sem depredação do patrimônio público";
	$labelTitulo1 = "Número de educandos envolvidos nas atividades de sensibilização, responsabilização e intervenção do cuidado consigo mesmo e com o ambiente escolar";
}


?>

<script src="../includes/calendario.js"></script>


<form id="formulario" name="formulario" action="" method="post">

<input type="hidden" name="moeid" value="<?=$moeid?>">
<input type="hidden" name="avcid" value="<?=$avcid?>">
<input type="hidden" name="avcid_2" value="<?=$avcid_2?>">
<input type="hidden" name="pmoid" value="<?=$pmoid?>">

<input type="hidden" name="botaoacao" value="">

<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
<tr>
	<td >
		<table border="0" width="100%" >
			<tr>
				<td class="SubTituloCentro" valign="top" align="center">
					 <br>
					 <font color="red">
				     <b> 
				     	O Componente II do PSE no módulo monitoramento na escola deve ser preenchido pela equipe da escola ou por profissionais da Atenção Básica e validado pelo diretor da escola. 
				     	<br>
				     </b>
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
		 	<b>COMPONENTE II - PROMOÇÃO E PREVENÇÃO À SAÚDE</b>
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
			<tr>
				<td class="SubTituloDireita">
					Total de educandos da escola: 
					<?
						if($entid){
							$sql = "select 
										moetotaleducando
									from pse.monitoraescola
									where entid = ".$entid." and acaoid = ".$_SESSION['pse']['acaoid']." and moeidentificasse = '{$_SESSION['pse']['sse']}'";
							$moetotaleducando = $db->pegaUm($sql);
							if(!$moetotaleducando) $moetotaleducando = 0;
						}
						
						echo campo_texto( 'moetotaleducando', 'N', 'N', '', 10, 6, '', '', 'right', '', 0, '');
					?>
				</td>
				<td class="SubTituloDireita">
					Total de educandos pactuados: 
					<?
						if($entid){
							$sql = "select 
										moetotalpactuado
									from pse.monitoraescola
									where entid = ".$entid." and acaoid = ".$_SESSION['pse']['acaoid']." and moeidentificasse = '{$_SESSION['pse']['sse']}'";
							$moetotalpactuado = $db->pegaUm($sql);
							if(!$moetotalpactuado) $moetotalpactuado = 0;
						}
						
						echo campo_texto( 'moetotalpactuado', 'N', 'N', '', 10, 6, '', '', 'right', '', 0, '');
					?>
				</td>
			</tr>
		</table>
	</td>
</tr>
</table>

<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
	<tr>
		<td style="background: rgb(168, 168, 168);" colspan='4'>&nbsp;</td>
	</tr>
	<tr>
		<td width="50%" class="SubTituloEsquerda" colspan='2'><b>&nbsp;Informe:</b></td>
		<td class="SubTituloEsquerda">
			<b><div align="center">SIM</div></b>
		</td>
		<td class="SubTituloEsquerda">
			<b><div align="center">NÃO</div></b>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan='2'><?=$labelP1?>.</td>
		<td class="SubTituloCentro">
			<input type="radio" name="pmopergunta1" value="S" <?if($pmopergunta1 == 'S') echo "checked";?>>
		</td>
		<td class="SubTituloCentro">
			<input type="radio" name="pmopergunta1" value="N" <?if($pmopergunta1 == 'N') echo "checked";?>>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan='2'><?=$labelP2?>.</td>
		<td class="SubTituloCentro">
			<input type="radio" name="pmopergunta2" value="S" <?if($pmopergunta2 == 'S') echo "checked";?>>
		</td>
		<td class="SubTituloCentro">
			<input type="radio" name="pmopergunta2" value="N" <?if($pmopergunta2 == 'N') echo "checked";?>>
		</td>
	</tr>
	<?if( in_array($_SESSION['pse']['acaoid'], array(9,10,11)) ){?>
		<tr>
			<td class="SubTituloDireita" colspan='2'><?=$labelP3?>.</td>
			<td class="SubTituloCentro">
				<input type="radio" name="pmopergunta3" value="S" <?if($pmopergunta3 == 'S') echo "checked";?>>
			</td>
			<td class="SubTituloCentro">
				<input type="radio" name="pmopergunta3" value="N" <?if($pmopergunta3 == 'N') echo "checked";?>>
			</td>
		</tr>
	<?}?>
	<tr>
		<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='4'>
			<table width="100%">
				<tr>
					<td width="55%" align="right">
						<?if($vPermissao == 'S'){?>
							<input type="button" class="botao" name="btGravar3" id="btGravar3" value="Gravar" onclick="validaFormPse(3);">
						<?}?>
					</td>
					<td align="right">&nbsp;</td>
				</tr>
			</table>
		</td>
	</tr>
	
	
	<tr>
		<td style="background: rgb(168, 168, 168);" colspan='4'>&nbsp;</td>
	</tr>
	<tr>
		<td colspan='4'><b><?=$labelTitulo1?>.</b></td>
	</tr>
	<tr>
		<td width="50%" class="SubTituloDireita" colspan='2'>Período da realização da ação:</td>
		<td colspan='2'>
			<?
				$andEditaCombo  = "";
				if($avcid) $andEditaCombo = "UNION
											 SELECT
												triid as codigo,
												tridescricao as descricao
											 FROM pse.trimestre
											 where triid in(select triid from pse.avaliacomp2 where avcid = $avcid)";
		
				$sqlc = "select distinct triid 
						from pse.avaliacomp2 
						WHERE avctipo = '1' 
						and comid = {$_SESSION['pse']['comid']}
						and acaoid = {$_SESSION['pse']['acaoid']}
						and moeid = {$moeid}";
				
				if($_SESSION['pse']['sse'] == 'S'){
					$andSse = "and triid in (5)";
				}
				else{
					$andSse = "and triid in (1,2)";
					
					if( in_array(date('m'), array('07','08','09')) ) $andSse = "and triid in (1,2,3)";
					if( in_array(date('m'), array('10','11','12')) ) $andSse = "and triid in (1,2,3,4)";
					
				}
				
				$sql_combo = "SELECT
								triid as codigo,
								tridescricao as descricao
							  FROM pse.trimestre
							  where triid not in ($sqlc)
							  $andSse
							  $andEditaCombo
							  order by 2";
				
				$db->monta_combo("triid", $sql_combo, $vPermissao, "Selecione a Período", '', '', '', '', 'S', 'triid');
			?>
		</td>
	</tr>
	<tr>
		<td width="50%" class="SubTituloDireita" colspan='2'>Número de educandos que participaram da ação:</td>
		<td colspan='2'>
			<?=campo_texto( 'avcquantidade', 'S', $vPermissao, '', 12, 10, '##########', '', 'right', '', 0, '');?>
		</td>
	</tr>
	<tr>
		<td width="50%" class="SubTituloDireita" colspan='2'>Número de educandos que participaram da ação pela 1a. vez:</td>
		<td colspan='2'>
			<?=campo_texto( 'avcquantprimeiravez', 'S', $vPermissao, '', 12, 10, '##########', '', 'right', '', 0, '');?>
		</td>
	</tr>
	<tr>
		<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='4'>
			<table width="100%">
				<tr>
					<td width="55%" align="right">
						<?if($vPermissao == 'S'){?>
							<input type="button" class="botao" name="btSalvar1" id="btSalvar1" value="Salvar" onclick="validaFormPse(1);">
						<?}?>
						&nbsp;
						<input type="button" class="botao" name="btVoltarC1" id="btVoltarC1" value="Voltar" onclick="location.href='pse.php?modulo=principal/cadastroMonitoramento&acao=A';">
					</td>
					<td align="right">&nbsp;</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td colspan='4'>
			<div id="listaC1" style="height: 150px; overflow-y: auto; overflow-x: hidden;">
				<?php 
					//if($_SESSION['permissaoedicao'] == true){
						//$img = "<a href=\"#\" onclick=\"alterarComp(\'' || a.avcid || '\');\" title=\"Alterar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\" onclick=\"excluirComp(\'' || a.avcid || '\');\" title=\"Excluir\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>";
					//} else {
						//$img = '<img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\">&nbsp;<img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">';
					//}
					
					if($moetotalpactuado > 0){
						$campoPorcentoContemp = "(a.avcquantidade * 100) / $moetotalpactuado as porcentocontemp";
					}
					else{
						$campoPorcentoContemp = "'0' as porcentocontemp";
					}
					
					$sql = "SELECT
								'<center>' ||
								(CASE WHEN a.avcvalidar = 'S' THEN
										CASE WHEN ".$vpflcod." = ".SUPER_USUARIO." or ".$vpflcod." = ".DIRETOR_ESCOLA." THEN
											 '<img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\">&nbsp;<img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">&nbsp;<a href=\"#\" onclick=\"validarComp(\'' || a.avcid || '\',\'N\');\" title=\"Clique aqui para Desvalidar\"><img src=\"../imagens/cadiado.png\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'
											 ELSE
											 '<img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\">&nbsp;<img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">&nbsp;<a href=\"#\"><img src=\"../imagens/cadiado_p.png\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'
										END
									ELSE
										CASE WHEN ".$vpflcod." = ".SUPER_USUARIO." THEN
											 '<a href=\"#\" onclick=\"alterarComp(\'' || a.avcid || '\');\" title=\"Alterar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\" onclick=\"excluirComp(\'' || a.avcid || '\');\" title=\"Excluir\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\" onclick=\"validarComp(\'' || a.avcid || '\',\'S\');\" title=\"Clique aqui para Validar\"><img src=\"../imagens/cadeadoAberto.png\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'
											 WHEN ".$vpflcod." = ".DIRETOR_ESCOLA." THEN
											 '<a href=\"#\"><img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\"><img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\" onclick=\"validarComp(\'' || a.avcid || '\',\'S\');\" title=\"Clique aqui para Validar\"><img src=\"../imagens/cadeadoAberto.png\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'
											 WHEN '{$vPermissao}' = 'S' THEN
											 '<a href=\"#\" onclick=\"alterarComp(\'' || a.avcid || '\');\" title=\"Alterar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\" onclick=\"excluirComp(\'' || a.avcid || '\');\" title=\"Excluir\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\"><img src=\"../imagens/cadeadoAberto_p.png\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'
											 ELSE
											 '<a href=\"#\"><img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\"><img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\"><img src=\"../imagens/cadeadoAberto_p.png\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'
										END
								END) || '</center>'  as acao,
								'<center>'||t.tridescricao||'</center>' as dsc, 
				            	'<center>'||a.avcquantidade||'</center>' as qtd,
				            	'<center>'||a.avcquantprimeiravez||'</center>' as qtd2,
				            	$campoPorcentoContemp 
							FROM pse.avaliacomp2 a
							INNER JOIN pse.monitoraescola m on m.moeid=a.moeid
							INNER JOIN pse.trimestre t on t.triid = a.triid
 							WHERE m.moeidentificasse = '{$_SESSION['pse']['sse']}' 
 							and a.avctipo = '1' 
							and a.comid = {$_SESSION['pse']['comid']}
							and a.acaoid = {$_SESSION['pse']['acaoid']}
							and m.entid = {$entid}
							order by 2";
				    //dbg($sql,1);
	
					$cabecalho 	= array( "Ação", "Período da realização da ação","Número de educandos que participaram da ação", "Número de educandos que participaram da ação pela 1a. vez", "% De Contemplados");
					$alinha = array("center","center","center","center","center","center");
					$tamanho = array("10%","20%","20%","20%","20%","20%");
					
					//$db->monta_lista( $sql, $cabecalho, 100, 10, 'N', 'center', '', '',$tamanho,$alinha);
					$db->monta_lista_simples( $sql, $cabecalho, 1000, 10, 'N', '', '', '', '', '', true);
					
				?>
			<div>
		</td>
	</tr>		
	
	
	<?if( in_array($_SESSION['pse']['acaoid'], array(11)) ){?>
		<tr>
			<td style="background: rgb(168, 168, 168);" colspan='4'>
				<div align="right">
					Total de educandos pactuados: 
					<?
						/*
						if($entid){
							$sql = "select mpaacao04comp3  
									from pse.metapactuada m
									inner join pse.planoacaoc2 p on p.conid = m.conid
									where p.entid = $entid";
							$mpaacao04comp3 = $db->pegaUm($sql);
							$mpaacao04comp3 = ($mpaacao04comp3 ? $mpaacao04comp3 : '0');
						}
						*/
						$mpaacao04comp3 = '0';
						
						echo campo_texto( 'mpaacao04comp3', 'N', 'N', '', 10, 6, '', '', 'right', '', 0, '');
					?>
					 
				</div>
			</td>
		</tr>
		<tr>
			<td colspan='4'><b>Número de jovens multiplicadores formados para atuarem entre pares nas temáticas envolvendo saúde sexual, saúde reprodutiva e prevenção das DST/AIDS e Hepatite Virais.</b></td>
		</tr>
		<tr>
			<td width="50%" class="SubTituloDireita" colspan='2'>Data da realização da ação:</td>
			<td colspan='2'>
				<?
					$andEditaCombo  = "";
					if($avcid_2) $andEditaCombo = "UNION
												 SELECT
													triid as codigo,
													tridescricao as descricao
												 FROM pse.trimestre
												 where triid in(select triid from pse.avaliacomp2 where avcid = $avcid_2)";
			
					$sqlc = "select distinct triid 
							from pse.avaliacomp2 
							WHERE avctipo = '2' 
							and comid = {$_SESSION['pse']['comid']}
							and acaoid = {$_SESSION['pse']['acaoid']}
							and moeid = {$moeid}";
					
					if($_SESSION['pse']['sse'] == 'S'){
						$andSse = "and triid in (5)";
					}
					else{
						$andSse = "and triid in (1,2)";
					
						if( in_array(date('m'), array('07','08','09')) ) $andSse = "and triid in (1,2,3)";
						if( in_array(date('m'), array('10','11','12')) ) $andSse = "and triid in (1,2,3,4)";
					}
				
					$sql_combo = "SELECT
									triid as codigo,
									tridescricao as descricao
								  FROM pse.trimestre
								  where triid not in ($sqlc)
								  $andSse
								  $andEditaCombo
								  order by 2";
								  
					$db->monta_combo("triid_2", $sql_combo, $vPermissao, "Selecione a Período", '', '', '', '', 'S', 'triid_2');
				?>
			</td>
		</tr>
		<tr>
			<td width="50%" class="SubTituloDireita" colspan='2'>Número de educandos que participaram da ação:</td>
			<td colspan='2'>
				<?=campo_texto( 'avcquantidade_2', 'S', $vPermissao, '', 12, 10, '##########', '', 'right', '', 0, '');?>
			</td>
		</tr>
		<tr>
			<td width="50%" class="SubTituloDireita" colspan='2'>Número de educandos que participaram da ação pela 1a. vez:</td>
			<td colspan='2'>
				<?=campo_texto( 'avcquantprimeiravez_2', 'S', $vPermissao, '', 12, 10, '##########', '', 'right', '', 0, '');?>
			</td>
		</tr>
		<tr>
			<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='4'>
				<table width="100%">
					<tr>
						<td width="55%" align="right">
							<?if($vPermissao == 'S'){?>
								<input type="button" class="botao" name="btSalvar2" id="btSalvar2" value="Salvar" onclick="validaFormPse(2);">
							<?}?>
							
							&nbsp;
							<input type="button" class="botao" name="btVoltar2" id="btVoltar2" value="Voltar" onclick="location.href='pse.php?modulo=principal/cadastroMonitoramento&acao=A';">
						</td>
						<td align="right">&nbsp;</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td colspan='4'>
				<div id="listaC1" style="height: 150px; overflow-y: auto; overflow-x: hidden;">
					<?php 
						//if($_SESSION['permissaoedicao'] == true){
							//$img = "<a href=\"#\" onclick=\"alterarComp2(\'' || a.avcid || '\');\" title=\"Alterar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\" onclick=\"excluirComp(\'' || a.avcid || '\');\" title=\"Excluir\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>";
						//} else {
							//$img = '<img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\">&nbsp;<img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">';
						//}
						
						if($moetotalpactuado > 0){
							$campoPorcentoContemp = "(a.avcquantidade * 100) / $moetotalpactuado as porcentocontemp";
						}
						else{
							$campoPorcentoContemp = "'0' as porcentocontemp";
						}
						
						$sql = "SELECT
									'<center>' ||
									(CASE WHEN a.avcvalidar = 'S' THEN
											CASE WHEN ".$vpflcod." = ".SUPER_USUARIO." or ".$vpflcod." = ".DIRETOR_ESCOLA." THEN
												 '<img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\">&nbsp;<img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">&nbsp;<a href=\"#\" onclick=\"validarComp(\'' || a.avcid || '\',\'N\');\" title=\"Clique aqui para Desvalidar\"><img src=\"../imagens/cadiado.png\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'
												 ELSE
												 '<img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\">&nbsp;<img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">&nbsp;<a href=\"#\"><img src=\"../imagens/cadiado_p.png\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'
											END
										ELSE
											CASE WHEN ".$vpflcod." = ".SUPER_USUARIO." THEN
												 '<a href=\"#\" onclick=\"alterarComp2(\'' || a.avcid || '\');\" title=\"Alterar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\" onclick=\"excluirComp(\'' || a.avcid || '\');\" title=\"Excluir\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\" onclick=\"validarComp(\'' || a.avcid || '\',\'S\');\" title=\"Clique aqui para Validar\"><img src=\"../imagens/cadeadoAberto.png\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'
												 WHEN ".$vpflcod." = ".DIRETOR_ESCOLA." THEN
												 '<a href=\"#\"><img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\"><img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\" onclick=\"validarComp(\'' || a.avcid || '\',\'S\');\" title=\"Clique aqui para Validar\"><img src=\"../imagens/cadeadoAberto.png\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'
												 WHEN '{$vPermissao}' = 'S' THEN
												 '<a href=\"#\" onclick=\"alterarComp2(\'' || a.avcid || '\');\" title=\"Alterar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\" onclick=\"excluirComp(\'' || a.avcid || '\');\" title=\"Excluir\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\"><img src=\"../imagens/cadeadoAberto_p.png\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'
												 ELSE
												 '<a href=\"#\"><img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\"><img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\"><img src=\"../imagens/cadeadoAberto_p.png\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'
											END
									END) || '</center>'  as acao,
									'<center>'||t.tridescricao||'<center>' as dsc, 
					            	'<center>'||a.avcquantidade||'<center>' as qtd,
					            	'<center>'||a.avcquantprimeiravez||'<center>' as qtd2,
					            	$campoPorcentoContemp
								FROM pse.avaliacomp2 a
								INNER JOIN pse.monitoraescola m on m.moeid=a.moeid
								INNER JOIN pse.trimestre t on t.triid = a.triid
								WHERE m.moeidentificasse = '{$_SESSION['pse']['sse']}' 
								and a.avctipo = '2' 
								and a.comid = {$_SESSION['pse']['comid']}
								and a.acaoid = {$_SESSION['pse']['acaoid']}
								and m.entid = {$entid}
								order by 2";
		
						$cabecalho 	= array( "Ação", "Período da realização da ação","Número de educandos que participaram da ação", "Número de educandos que participaram da ação pela 1a. vez", "% De Contemplados");
						$alinha = array("center","center","center","center","center","center");
						$tamanho = array("10%","20%","20%","20%","20%","20%");
						
						//$db->monta_lista( $sql, $cabecalho, 100, 10, 'N', 'center', '', '',$tamanho,$alinha);
						$db->monta_lista_simples( $sql, $cabecalho, 1000, 10, 'N', '', '', '', '', '', true);
					?>
				<div>
			</td>
		</tr>		
	<?}?>
	
	

	
</table>


</form>

</body>
</html>

 
<script>

function validaFormPse(acao){

	var d = document.formulario;
	
	if(acao != 3){
		var pmoid = "<?=$pmoid?>";
		if(pmoid == ''){
			alert('É necessário responder as perguntas do Informe.');
			return false;
		}
	}	
	
	if(acao == 3){
		if(d.pmopergunta1[0].checked == false && d.pmopergunta1[1].checked == false){
			alert('Favor Responder a Pergunta 1');
			d.pmopergunta1[0].focus();
			return false;
		}
		if(d.pmopergunta2[0].checked == false && d.pmopergunta2[1].checked == false){
			alert('Favor Responder a Pergunta 2');
			d.pmopergunta2[0].focus();
			return false;
		}
		<?if( in_array($_SESSION['pse']['acaoid'], array(9,10,11)) ){?>
		if(d.pmopergunta3[0].checked == false && d.pmopergunta3[1].checked == false){
			alert('Favor Responder a Pergunta 3');
			d.pmopergunta3[0].focus();
			return false;
		}
		<?}?>
	}
		
	if(acao == 1){
	
		if(d.triid.value == ''){
			alert('Favor Preencher o campo obrigatório: Período da realização da ação.');
			d.triid.focus();
			return false;
		}
		
		d.avcquantidade.value=mascaraglobal('##########',d.avcquantidade.value);
		if(d.avcquantidade.value == ''){
			alert('Favor Preencher o campo obrigatório: Número de educandos que participaram da ação.');
			d.avcquantidade.focus();
			return false;
		}
		d.avcquantprimeiravez.value=mascaraglobal('##########',d.avcquantprimeiravez.value);
		if(d.avcquantprimeiravez.value == ''){
			alert('Favor Preencher o campo obrigatório: Número de educandos que participaram da ação pela 1a. vez.');
			d.avcquantprimeiravez.focus();
			return false;
		}
		
	}


	if(acao == 2){

		if(d.triid_2.value == ''){
			alert('Favor Preencher o campo obrigatório: Período da realização da ação.');
			d.triid_2.focus();
			return false;
		}
		
		d.avcquantidade_2.value=mascaraglobal('##########',d.avcquantidade_2.value);
		if(d.avcquantidade_2.value == ''){
			alert('Favor Preencher o campo obrigatório: Número de educandos que participaram da ação.');
			d.avcquantidade_2.focus();
			return false;
		}
		
		d.avcquantprimeiravez_2.value=mascaraglobal('##########',d.avcquantprimeiravez_2.value);
		if(d.avcquantprimeiravez_2.value == ''){
			alert('Favor Preencher o campo obrigatório: Número de educandos que participaram da ação pela 1a. vez.');
			d.avcquantprimeiravez_2.focus();
			return false;
		}
		
	}
	
	d.botaoacao.value = acao;

	d.submit();

}


function alterarComp(id){
	location.href='pse.php?modulo=principal/cadastroMonitoramentoAcao2&acao=A&avcid='+id;
}

function alterarComp2(id){
	location.href='pse.php?modulo=principal/cadastroMonitoramentoAcao2&acao=A&avcid_2='+id;
}


function excluirComp(id){

	if(confirm('Deseja realmente excluir este item?')){
		location.href='pse.php?modulo=principal/cadastroMonitoramentoAcao2&acao=A&exluir=1&avcid='+id;
	}
}

function validarComp(id,status){
	location.href='pse.php?modulo=principal/cadastroMonitoramentoAcao2&acao=A&validar='+status+'&avcid='+id;
}

</script>