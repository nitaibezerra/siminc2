<?php
unset($_SESSION['pse']['avlid']);

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
if($vpflcod == SUPER_USUARIO || $vpflcod == ESCOLA_MUNICIPAL || $vpflcod == ESCOLA_ESTADUAL){
	$vPermissao = 'S';
}
else{
	$vPermissao = 'N';
}

//apos 31 de novembro e diferente de monitoramento de ações, permite so vizualizar os dados
if( date('Ymd') > 20121130 && $_SESSION['pse']['sse'] != 'N' ) $vPermissao = 'N';



//excluir registro
if($_REQUEST['exluir'] && $_REQUEST['avlid']){
	
	/*
	$sql = "UPDATE 
				pse.avaliacao
			SET 
	            avlvalidar = 'N'
			WHERE 
				avlid = ".$_REQUEST['avlid'];
	*/

	//verifica se ja existe ficha
	$sql = "SELECT count(fieid) as total from pse.fichaeducando WHERE avlid = ".$_REQUEST['avlid'];
	$totalFichas = $db->pegaUm($sql);

	//verifica se ja existe doenca
	$sql = "SELECT count(eddid) as total from pse.educandodoenca WHERE avlid = ".$_REQUEST['avlid'];
	$totalDoencas = $db->pegaUm($sql);
	
	if($totalFichas > 0 || $totalDoencas > 0){
		print "<script>
			alert('Não é possível excluir, pois existem fichas de educandos ou doenças vinculadas nesta ação!');
			history.back();
		   </script>";
	}else{
		$sql = "DELETE FROM pse.avaliacao WHERE avlid = ".$_REQUEST['avlid'];
		$db->executar($sql);					
		$db->commit();
		
		//recupera moeid
		$sql = "SELECT moeid from pse.monitoraescola
		WHERE entid = '".$entid."' and acaoid = ".$_SESSION['pse']['acaoid']." and moeidentificasse = '".$_SESSION['pse']['sse']."'";
		$moeid = $db->pegaUm($sql);
		if($moeid){
			atualizaTotalPercentual($moeid);
		}
		
		print "<script>
			alert('Operação realizada com sucesso!');
			location.href='pse.php?modulo=principal/cadastroMonitoramentoAcao&acao=A';
		   </script>";
	}
	
	
	exit();
}


//validar registro
if($_REQUEST['validar'] && $_REQUEST['avlid']){
	
	$sql = "UPDATE 
				pse.avaliacao
			SET 
	            avlvalidar = '{$_REQUEST['validar']}'
			WHERE 
				avlid = ".$_REQUEST['avlid'];
	$db->executar($sql);					
	$db->commit();
	
	print "<script>
			alert('Operação realizada com sucesso!');
			location.href='pse.php?modulo=principal/cadastroMonitoramentoAcao&acao=A';
		   </script>";
	exit();
}



//primeiro acesso na ação comp1
//verifica se ja existe
	$entcodent = $db->pegaUm("select entcodent from entidade.entidade where entid = ".$entid);
	
	$sql = "SELECT moeid from pse.monitoraescola
			WHERE entid = '".$entid."' and acaoid = ".$_SESSION['pse']['acaoid']." and moeidentificasse = '".$_SESSION['pse']['sse']."'";
	$moeid = $db->pegaUm($sql);
	
	if(!$moeid){
		
		$sql = "select pamquantedutotal, pamquanteduseratend  
				from pse.planoacaoc1
				where entid = $entid";
		$comp1 = $db->pegaLinha($sql);
		$moetotaleducando = ($comp1['pamquantedutotal'] ? $comp1['pamquantedutotal'] : 0);
		$moetotalpactuado = ($comp1['pamquanteduseratend'] ? $comp1['pamquanteduseratend'] : 0);
		
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
	
	//atualiza total educando para semana saude na escola ($_SESSION['pse']['sse'] == 'S')
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
	
	//atualiza total educando para brasil carinhoso ($_SESSION['pse']['sse'] == 'B')
	if($_SESSION['pse']['sse'] == 'B'){
			
			$sql = "select 
					ebcquanteducando
				from pse.escolabrasilcarinhoso
				where ebccodigoinep = '".$entcodent."'";
			$ebcquanteducando = $db->pegaUm($sql);
			if(!$ebcquanteducando) $ebcquanteducando = 0;
			
			$sql = "UPDATE 
						pse.monitoraescola
					SET 
			            moetotaleducando = ".$ebcquanteducando."
					WHERE 
						moeid = {$moeid}";
			$db->executar($sql);					
			$db->commit();

	}
	
	//atualiza ação: 7 - Ação: Realizar triagem da acuidade auditiva dos educandos e identificar educandos com problemas auditivos
	if($_SESSION['pse']['acaoid'] == 6){ 
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
	
//fim primeiro acesso





//submit
if($_POST){
	
	if(!$_POST['moeid']){
		?>
		<script>
			alert("Sua sessão expirou. Por favor, entre novamente!");
			location.href='pse.php?modulo=inicio&acao=C';
		</script>
		<?
		exit;	
	}
	
	//verifica o somatorio do Número de educandos avaliados (avleducandoavaliado)
	if($_POST['avlid']) $andSoma = " and avlid not in ({$_POST['avlid']})";
	$sql = "select sum(avleducandoavaliado) as total 
			from pse.avaliacao 
			where moeid = {$_POST['moeid']}
			and acaoid = {$_SESSION['pse']['acaoid']}
			$andSoma";
	$somaAvaliados = $db->pegaUm($sql);
	if(!$somaAvaliados) $somaAvaliados = 0;
	$somaAvaliados += (int) $_POST['avleducandoavaliado'];
	
	if( (int) $somaAvaliados > (int) $_POST['moetotaleducando'] ){
		print "<script>
				alert('Erro: O somatório do número de educandos avaliados ({$somaAvaliados}) não pode ser maior que o total de educandos da escola ({$_POST['moetotaleducando']})');
				history.back();
		   	  </script>";
		exit();
	}
	//fim verifica
	
	//verifica se é numerico
		if($_POST['avleducandoavaliado']){
			if(!is_numeric($_POST['avleducandoavaliado'])){
				print "<script>
					alert('Erro: Número inválido! Informe o número correto.');
					history.back();
			   	  </script>";
				exit();	
			}		
		}
		if($_POST['avlidentifica1']){
			if(!is_numeric($_POST['avlidentifica1'])){
				print "<script>
					alert('Erro: Número inválido! Informe o número correto.');
					history.back();
			   	  </script>";
				exit();	
			}		
		}
		if($_POST['avlidentifica2']){
			if(!is_numeric($_POST['avlidentifica2'])){
				print "<script>
					alert('Erro: Número inválido! Informe o número correto.');
					history.back();
			   	  </script>";
				exit();	
			}		
		}
	//fim verifica
		
	if(!$_POST['avlid']) {
		inserirDados();
	}
	else{
		alterarDados($_POST['avlid']);
	}
	
	atualizaTotalPercentual($_POST['moeid']);
		
	unset($_POST);
	
	print "<script>
			alert('Operação realizada com sucesso!');
			location.href='pse.php?modulo=principal/cadastroMonitoramentoAcao&acao=A';
		   </script>";
	exit();
	
}



function inserirDados(){
	global $db;
	
	$sql = "INSERT INTO pse.avaliacao
			(
            	moeid,
			  	acaoid,
			  	comid,
			  	avldataavaliacao,
			  	avleducandoavaliado,
			  	avlidentifica1,
			  	avlidentifica2,
			  	cneid,
			  	avlequipeescola,
			  	avlvalidar 
	        ) VALUES (
	        	".$_POST['moeid'].",
	        	".$_SESSION['pse']['acaoid'].",
	        	".$_SESSION['pse']['comid'].",
	        	'".formata_data_sql($_POST['avldataavaliacao'])."',
	        	".$_POST['avleducandoavaliado'].",
	        	".$_POST['avlidentifica1'].",
	        	".($_POST['avlidentifica2'] ? $_POST['avlidentifica2'] : 'null').",
	        	".($_POST['cneid'] ? $_POST['cneid'] : 'null').",
	        	".($_POST['avlequipeescola'] ? "'".$_POST['avlequipeescola']."'" : 'null').",
	        	null
	        )";	

	$db->executar($sql);
	$db->commit();
	
}



function alterarDados($avlid){
	global $db;
	
	$sql = "UPDATE 
				pse.avaliacao
			SET 
	            avldataavaliacao = '".formata_data_sql($_POST['avldataavaliacao'])."',
				avleducandoavaliado = ".$_POST['avleducandoavaliado'].",
				cneid = ".($_POST['cneid'] ? $_POST['cneid'] : 'null').",
				avlequipeescola = ".($_POST['avlequipeescola'] ? "'".$_POST['avlequipeescola']."'" : 'null').",
				avlidentifica1 = ".$_POST['avlidentifica1'].",
				avlidentifica2 = ".($_POST['avlidentifica2'] ? $_POST['avlidentifica2'] : 'null')."
			WHERE 
				avlid = ".$avlid;
		
	$db->executar($sql);					
	$db->commit();
	
}




function atualizaTotalPercentual($moeid){
	global $db;
	
	//atualiza o total e percentual dos educandos avaliados
	
	//totalavaliado
	$sql = "select sum(avleducandoavaliado) as total 
			from pse.avaliacao 
			where moeid = {$moeid}
			and acaoid = {$_SESSION['pse']['acaoid']}";
	$totalAvaliados = $db->pegaUm($sql);
	if(!$totalAvaliados) $totalAvaliados = 0;

	//totalidentificado
	$sql = "select sum(avlidentifica1) as total 
			from pse.avaliacao 
			where moeid = {$moeid}
			and acaoid = {$_SESSION['pse']['acaoid']}";
	$totalaIdentificado = $db->pegaUm($sql);
	if(!$totalaIdentificado) $totalaIdentificado = 0;
	
	//totalpactuados
	$sql = "select moetotalpactuado as total 
			from pse.monitoraescola 
			where entid = {$_SESSION['pse']['entid']} and acaoid = ".$_SESSION['pse']['acaoid']." and moeidentificasse = '".$_SESSION['pse']['sse']."'";
	$totalPactuados = $db->pegaUm($sql);
	if(!$totalPactuados) $totalPactuados = 0;
	
	//totaleducandos
	$sql = "select moetotaleducando as total 
			from pse.monitoraescola 
			where entid = {$_SESSION['pse']['entid']} and acaoid = ".$_SESSION['pse']['acaoid']." and moeidentificasse = '".$_SESSION['pse']['sse']."'";
	$totalEducandos = $db->pegaUm($sql);
	if(!$totalEducandos) $totalEducandos = 0;
	
	
	
	if($_SESSION['pse']['sse'] == 'B'){ //brasil carinhoso
		if($totalEducandos > 0) $moepercentavaliado = ($totalAvaliados * 100) / $totalEducandos;
		else $moepercentavaliado = 0;
	}else{
		if($totalPactuados > 0) $moepercentavaliado = ($totalAvaliados * 100) / $totalPactuados;
		else $moepercentavaliado = 0;
	}
	
	if($totalAvaliados > 0) $moepercentidentificado = ($totalaIdentificado * 100) / $totalAvaliados;
	else $moepercentidentificado = 0;

	//if($moepercentavaliado > 100) $moepercentavaliado = 100;
	//if($moepercentidentificado > 100) $moepercentidentificado = 100;
	
	$sql = "UPDATE 
			pse.monitoraescola
		SET 
            moetotalavaliado = {$totalAvaliados},
            moepercentavaliado = ".number_format($moepercentavaliado,2, '.', '').",
            moetotalidentificado = {$totalaIdentificado},
            moepercentidentificado = ".number_format($moepercentidentificado,2, '.', '')."
		WHERE 
			entid = {$_SESSION['pse']['entid']} and acaoid = {$_SESSION['pse']['acaoid']} and moeidentificasse = '{$_SESSION['pse']['sse']}'";
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
			  1 => array("id" => 2, "descricao" => "Avaliações", 	"link" => $_SERVER['REQUEST_URI'])
			  //2 => array("id" => 3, "descricao" => "Ficha de Acompanhamento", 	"link" => "/pse/pse.php?modulo=principal/cadastroMonitoramentoFicha&acao=A"),
			  //3 => array("id" => 4, "descricao" => "Questionário", 	"link" => "/pse/pse.php?modulo=principal/cadastroQuestionario&acao=A")
		  	  );
		
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);


//carrega dados para edição
if($_REQUEST['avlid']){
	$sql = "SELECT  
				  avlid,
				  moeid,
				  avldataavaliacao,
				  avleducandoavaliado,
				  avlidentifica1,
				  avlidentifica2,
				  avlvalidar,
				  cneid,
				  avlequipeescola
		     FROM 
		     		pse.avaliacao
		     WHERE 
		     		avlid = {$_REQUEST['avlid']}";
	$dados = $db->carregar($sql);
	if($dados) extract($dados[0]);
}


//configura titulos e img para pagina
//COMP 1
if( in_array($_SESSION['pse']['acaoid'], array(1)) ){ 
	$labelTd1 = "educandos avaliados";
	$labelTd2 = "educandos com sinal de obesidade/desnutrição";
}elseif( in_array($_SESSION['pse']['acaoid'], array(2)) ){
	$labelTd1 = "educandos avaliados";
	$labelTd2 = "educandos identificados com pressão arterial alterada";
}elseif( in_array($_SESSION['pse']['acaoid'], array(3)) ){
	$labelTd1 = "educandos com carteira de vacina verificada";
	$labelTd2 = "educandos identificados com carteira de vacinação desatualizada";
}elseif( in_array($_SESSION['pse']['acaoid'], array(4)) ){
	$labelTd1 = "educandos avaliados";
	$labelTd2 = "educandos identificados com sinais de agravos de saúde negligenciados";
}elseif( in_array($_SESSION['pse']['acaoid'], array(5)) ){
	$labelTd1 = "educandos avaliados";
	$labelTd2 = "educandos com problemas visuais";
}elseif( in_array($_SESSION['pse']['acaoid'], array(6)) ){ 
	$labelTd1 = "educandos avaliados";
	$labelTd2 = "educandos com problemas auditivos";
}elseif( in_array($_SESSION['pse']['acaoid'], array(7)) ){
	$labelTd1 = "educandos avaliados";
	$labelTd2 = "educandos com necessidade de cuidado em saúde bucal";
}elseif( in_array($_SESSION['pse']['acaoid'], array(8)) ){
	$labelTd1 = "educandos avaliados";
	$labelTd2 = "educandos identificados sem registro civil";
}

//recupera o moeid
$moeid = $db->pegaUm("select moeid from pse.monitoraescola where entid = {$entid} and acaoid = {$_SESSION['pse']['acaoid']} and moeidentificasse = '{$_SESSION['pse']['sse']}'");
?>

<script src="../includes/calendario.js"></script>


<form id="formulario" name="formulario" action="" method="post">

<input type="hidden" name="moeid" value="<?=$moeid?>">
<input type="hidden" name="avlid" value="<?=$avlid?>">

<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
<tr>
	<td >
		<table border="0" width="100%" >
			<tr>
				<td class="SubTituloCentro" valign="top" align="center">
					 <br>
					 <font color="red">
				     <b> 
				     	<?if($_SESSION['pse']['sse'] == 'B'){?>
					     	O módulo BRASIL CARINHOSO do PSE deve ser preenchido por profissionais da Atenção Básica.
					    <?}else{?>
					    	O Componente I do PSE no módulo monitoramento na escola deve ser preenchido por profissionais da Atenção Básica e validado pelo diretor da escola.
					     	<br><br> 
					     	O Componente II do PSE no módulo monitoramento na escola deve ser preenchido pela equipe da escola ou por profissionais da Atenção Básica e validado pelo diretor da escola.
					    <?}?> 
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
			<tr>
				<td class="SubTituloDireita">
					Total de educandos da escola: 
					<?
						if($entid){
							$sql = "select 
										moetotaleducando
									from pse.monitoraescola
									where entid = {$entid} and acaoid = {$_SESSION['pse']['acaoid']} and moeidentificasse = '{$_SESSION['pse']['sse']}'";
							$moetotaleducando = $db->pegaUm($sql);
							if(!$moetotaleducando) $moetotaleducando = 0;
						}
						
						echo campo_texto( 'moetotaleducando', 'N', 'N', '', 10, 6, '', '', 'right', '', 0, '');
					?>
				</td>
				
				<?if($_SESSION['pse']['sse'] != 'B'){ // diferente de brasil carinhoso 
					?>
					<td class="SubTituloDireita">
						Total de educandos pactuados: 
						<?
							if($entid){
								$sql = "select 
											moetotalpactuado
										from pse.monitoraescola
										where entid = {$entid} and acaoid = {$_SESSION['pse']['acaoid']} and moeidentificasse = '{$_SESSION['pse']['sse']}'";
								$moetotalpactuado = $db->pegaUm($sql);
								if(!$moetotalpactuado) $moetotalpactuado = 0;
							}
							
							echo campo_texto( 'moetotalpactuado', 'N', 'N', '', 10, 6, '', '', 'right', '', 0, '');
						?>
					</td>
				<?}?>
				
				<?if( in_array($_SESSION['pse']['acaoid'], array(1,2,3,4,5,6,7,8)) ){ ?>
					<td class="SubTituloDireita">
						<?
							echo "Total de " . $labelTd1 .":";
							
							if($entid){
								$sql = "select moetotalavaliado 
										from pse.monitoraescola
										where entid = {$entid} and acaoid = {$_SESSION['pse']['acaoid']} and moeidentificasse = '{$_SESSION['pse']['sse']}'";
								$moetotalavaliado = $db->pegaUm($sql);
								if(!$moetotalavaliado) $moetotalavaliado = 0;
							}
							//ver($sql);
							echo campo_texto( 'moetotalavaliado', 'N', 'N', '', 10, 6, '', '', 'right', '', 0, '');
							
							echo "&nbsp;&nbsp;";
							
							
							if($_SESSION['pse']['acaoid'] != 6){ 
								if($entid){
									$sql = "select moepercentavaliado 
											from pse.monitoraescola
											where entid = {$entid} and acaoid = {$_SESSION['pse']['acaoid']} and moeidentificasse = '{$_SESSION['pse']['sse']}'";
									$moepercentavaliado = $db->pegaUm($sql);
									if(!$moepercentavaliado) $moepercentavaliado = 0;
								}
								
								echo campo_texto( 'moepercentavaliado', 'N', 'N', '', 10, 6, '', '', 'right', '', 0, '');
								echo "%";
							}
						?>
					</td>
				<?}?>
				
				<?if( in_array($_SESSION['pse']['acaoid'], array(1,4,5,6,7,8)) ){ ?>
					<td class="SubTituloDireita">
						<?
							echo "Total de " . $labelTd2 .":";
							
							if($entid){
								$sql = "select moetotalidentificado 
										from pse.monitoraescola
										where entid = {$entid} and acaoid = {$_SESSION['pse']['acaoid']} and moeidentificasse = '{$_SESSION['pse']['sse']}'";
								$moetotalidentificado = $db->pegaUm($sql);
								if(!$moetotalidentificado) $moetotalidentificado = 0;
							}
							
							echo campo_texto( 'moetotalidentificado', 'N', 'N', '', 10, 6, '', '', 'right', '', 0, '');
							
							echo "&nbsp;&nbsp;";
							
							if($entid){
								$sql = "select moepercentidentificado 
										from pse.monitoraescola
										where entid = {$entid} and acaoid = {$_SESSION['pse']['acaoid']} and moeidentificasse = '{$_SESSION['pse']['sse']}'";
								$moepercentidentificado = $db->pegaUm($sql);
								if(!$moepercentidentificado) $moepercentidentificado = 0;
							}
							
							echo campo_texto( 'moepercentidentificado', 'N', 'N', '', 10, 6, '', '', 'right', '', 0, '');
							
							echo "%";
						?>
					</td>
				<?}?>
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
		<td width="50%" class="SubTituloDireita" colspan='2'>Data da avaliação:</td>
		<td colspan='2'>
			<?=campo_data('avldataavaliacao', 'S', $vPermissao, '', 'S' );?>
		</td>
	</tr>
	<?if( in_array($_SESSION['pse']['acaoid'], array(5,8)) ){ ?>
	<tr>
		<td width="50%" class="SubTituloDireita" colspan='2'>Equipe que Realizou a Ação:</td>
		<td colspan='2'>
			<input type="radio" name="avlequipeescola" id="avlequipeescola" value="S" <?if($avlequipeescola == 'S') echo "checked";?> onclick="document.getElementById('combo_cneid').style.display='';"> Equipe ESF
			&nbsp;&nbsp;
			<span id="combo_cneid" style="display: <?if($avlequipeescola == 'S'){echo "";}else{echo "none";}?>">
			<?
				$sql_combo = "SELECT
								cneid as codigo,
								cnecodigocnes ||' - '|| cnenomefantasia ||' - '|| cneseqequipe  as descricao
							  FROM pse.scnes
							  where muncod = '".$_SESSION['pse']['muncod']."'
							  order by 2";
				$responsavelesf = $db->carregar($sql_combo);
				$db->monta_combo("cneid", $sql_combo, $vPermissao, "Selecione a Equipe ESF", '', '', '', '', 'S', 'cneid');
			?>
			</span>
			
			<br>
			
			<input type="radio" name="avlequipeescola" id="avlequipeescola" value="N" <?if($avlequipeescola == 'N') echo "checked";?> onclick="document.formulario.cneid.value='';document.getElementById('combo_cneid').style.display='none';"> Equipe da Escola
		</td>
	</tr>
	<?}else{?>
	<tr>
		<td width="50%" class="SubTituloDireita" colspan='2'>ESF - Equipe Saúde da Família:</td>
		<td colspan='2'>
			<?
				$sql_combo = "SELECT
								cneid as codigo,
								cnecodigocnes ||' - '|| cnenomefantasia ||' - '|| cneseqequipe as descricao
							  FROM pse.scnes
							  where muncod = '".$_SESSION['pse']['muncod']."'
							  order by 2";
				$db->monta_combo("cneid", $sql_combo, $vPermissao, "Selecione a Equipe ESF", '', '', '', '', 'S', 'cneid');
			?>
		</td>
	</tr>
	<?}?>
	<tr>
		<td width="50%" class="SubTituloDireita" colspan='2'>Número de <?=$labelTd1?>:</td>
		<td colspan='2'>
			<?=campo_texto( 'avleducandoavaliado', 'S', $vPermissao, '', 12, 10, '##########', '', 'right', '', 0, '');?>
		</td>
	</tr>
	<tr>
		<td width="50%" class="SubTituloDireita" colspan='2'>Número de <?=$labelTd2?>:</td>
		<td colspan='2'>
			<?=campo_texto( 'avlidentifica1', 'S', $vPermissao, '', 12, 10, '##########', '', 'right', '', 0, '');?>
		</td>
	</tr>
	<?if( in_array($_SESSION['pse']['acaoid'], array(1)) ){ ?>
		<tr>  
			<td width="50%" class="SubTituloDireita" colspan='2'>Número de educandos com sobrepeso:</td>
			<td colspan='2'>
				<?=campo_texto( 'avlidentifica2', 'S', $vPermissao, '', 12, 10, '##########', '', 'right', '', 0, '');?>
			</td>
		</tr>
	<?}?>	
	<tr>
		<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='4'>
			<table width="100%">
				<tr>
					<td width="55%" align="right">
						<?if($vPermissao == 'S'){?>
							<input type="button" class="botao" name="btSalvarC1" id="btSalvarC1" value="Salvar" onclick="validaFormPse();">
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
					$sql = "SELECT
								'' as acao, 
								to_char(a.avldataavaliacao::date, 'DD/MM/YYYY') as dataaval,
								(CASE WHEN a.avlequipeescola = 'N' THEN
										'Equipe da Escola'
									  ELSE
									  	s.cnecodigocnes ||' - '|| s.cnenomefantasia ||' - '|| s.cneseqequipe
								END) as nomeequipe,
				            	a.avleducandoavaliado,
				            	a.avlidentifica1, 
				            	a.avlidentifica2,
				            	a.avlid,
				            	a.avlvalidar
							FROM pse.avaliacao a
							INNER JOIN pse.monitoraescola m on m.moeid=a.moeid
							LEFT JOIN pse.scnes s on s.cneid=a.cneid
							WHERE m.moeidentificasse = '{$_SESSION['pse']['sse']}' 
							and a.comid = {$_SESSION['pse']['comid']}
							and a.acaoid = {$_SESSION['pse']['acaoid']}
							and m.entid = {$entid}
							order by 2";
					
					$dados = $db->carregar($sql); 
				?>
				
				<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
					<thead>
						<tr>
							<td  align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Ação</strong></label></td>
							<td  align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Data da avaliação</strong></label></td>
							<?if( in_array($_SESSION['pse']['acaoid'], array(5,8)) ){ ?>
								<td  align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Equipe que Realizou a Ação</strong></label></td>
							<?}else{?>
								<td  align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>ESF - Equipe Saúde da Família</strong></label></td>
							<?}?>
							<td  align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Número de <?=$labelTd1?></strong></label></td>
							<td  align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Número de <?=$labelTd2?></strong></label></td>
							<?if( in_array($_SESSION['pse']['acaoid'], array(1)) ){?>
								<td  align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Número de educandos com sobrepeso</strong></label></td>
							<?}?>
						</tr>
					</thead>
					<?if(!$dados){?>
						<tr>
							<td align="center" colspan="6">Não existem registros.</td>
						</tr>
					<?}else{
						
						foreach($dados as $d){
							
							//verifica pendencias
							$cor = 'white';
							$dscPendencia = '';
							if($d['avlidentifica1'] > 0){

								
								//verifica perguntas
								/*
								if( in_array($_SESSION['pse']['acaoid'], array(5,6)) ){
										
									$sql = "select count(acpid) as total from pse.acompanhamento where fieid in (select fieid from pse.fichaeducando where avlid = ".$d['avlid'].")";
									$totalPerguntas = $db->pegaUm($sql);
									if(!$totalPerguntas) $totalPerguntas = 0;
									
									if($totalPerguntas != $d['avlidentifica1']) {
										$cor = '#FF6666';
										$dscPendencia = 'É necessário responder as perguntas da ficha do educando da avaliação - Data: '.$d['dataaval'];
									}
										
								}
								*/
								
								
								//verifica fichas
								if( in_array($_SESSION['pse']['acaoid'], array(1,2,5,6)) ){
									$sql = "select count(fieid) as total from pse.fichaeducando where avlid = ".$d['avlid'];
									$totalFicha = $db->pegaUm($sql);
									if(!$totalFicha) $totalFicha = 0;
									
									if($totalFicha != $d['avlidentifica1']) {
										$cor = '#FF6666';
										$dscPendencia = 'Total de Fichas deve ter o mesmo total de '.$labelTd2.' da avaliação - Data: '.$d['dataaval'];
									}
								}
								
								
								if( in_array($_SESSION['pse']['acaoid'], array(4)) ){
									$sql = "select sum(eddquantidade) as total from pse.educandodoenca where avlid = ".$d['avlid'];
									$totalDoencas = $db->pegaUm($sql);
									if(!$totalDoencas) $totalDoencas = 0;
									
									if($totalDoencas != $d['avlidentifica1']) {
										$cor = '#FF6666';
										$dscPendencia = 'Total de Educandos com doenças deve ter o mesmo total de educandos da avaliação - Data: '.$d['dataaval'];
									}
								}
								
								
							}
							
							?>
							<tr bgcolor="<?=$cor?>" title="<?=$dscPendencia?>">
								<td align="center" width="10%">
									<?
										if($vPermissao == 'S' && $d['avlvalidar'] != 'S'){
											echo "<a href=\"#\" onclick=\"alterarComp('". $d['avlid'] . "');\" title=\"Alterar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\" onclick=\"excluirComp('". $d['avlid'] . "');\" title=\"Excluir\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;";
										} else {
											echo "<img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\">&nbsp;<img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">&nbsp;";
										}
										
										if( $d['avlidentifica1'] > 0 ){ 
											if( in_array($_SESSION['pse']['acaoid'], array(1,2,5,6)) ){ 
												echo "<a href=\"#\" onclick=\"ficha('". $d['avlid'] . "');\" title=\"Cadastrar/Editar Ficha do Educando\"><img src=\"../imagens/editar_conteudo_caixa.png\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;";
											}
											elseif( in_array($_SESSION['pse']['acaoid'], array(4)) ){ 
												echo "<a href=\"#\" onclick=\"cadDoenca('". $d['avlid'] . "');\" title=\"Cadastrar/Editar Doenças\"><img src=\"../imagens/editar_conteudo_caixa.png\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;";
											}
										}
										
										if($vpflcod == SUPER_USUARIO || $vpflcod == DIRETOR_ESCOLA){
											if($d['avlvalidar'] == 'S'){
												echo "<a href=\"#\" onclick=\"validarComp('". $d['avlid'] . "','N');\" title=\"Clique aqui para Desvalidar\"><img src=\"../imagens/cadiado.png\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>";
											}
											else{
												//if($dscPendencia){
												//	echo "<a href=\"#\" onclick=\"alert('Não é possível validar, pois existe pendência!');\" title=\"Clique aqui para Validar\"><img src=\"../imagens/cadeadoAberto.png\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>";
												//}else{
												if($_SESSION['pse']['sse'] != 'B'){
													echo "<a href=\"#\" onclick=\"validarComp('". $d['avlid'] . "','S');\" title=\"Clique aqui para Validar\"><img src=\"../imagens/cadeadoAberto.png\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>";
												}
												//}
											}
										}
										else{
											if($d['avlvalidar'] == 'S'){
												echo "<img src=\"../imagens/cadiado_p.png\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\" title=\"Validada\">";
											}
											else{
												echo "<img src=\"../imagens/cadeadoAberto_p.png\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\" title=\"Não Validada\">";
											} 
										}
										
									?>
								</td>
								<td align="center" width="10%"><?=$d['dataaval']?></td>
								<td align="left" width="30%"><?=$d['nomeequipe']?></td>
								<td align="center"><?=$d['avleducandoavaliado']?></td>
								<td align="center"><?=$d['avlidentifica1']?></td>
								<?if( in_array($_SESSION['pse']['acaoid'], array(1)) ){?>
									<td align="center"><?=$d['avlidentifica2']?></td>
								<?}?>
							</tr>
						<?}?>
					<?}?>
				</table>
			<div>
		</td>
	</tr>
</table>


</form>

</body>
</html>

 
<script>

function validaFormPse(){

	var d = document.formulario;
	
	if(d.avldataavaliacao.value == ''){
		alert('Favor Preencher o campo obrigatório: Data da avaliação.');
		d.avldataavaliacao.focus();
		return false;
	}
	
	var dtavaliacao = d.avldataavaliacao.value.substr(6,4) + d.avldataavaliacao.value.substr(3,2) + d.avldataavaliacao.value.substr(0,2);
	var dtatual = "<?=date('Ymd')?>";
	
	if( parseFloat(dtavaliacao) >  parseFloat(dtatual) ){
		alert('Data da avaliação não pode ser maior que a data de hoje.');
		d.avldataavaliacao.focus();
		return false;
	}
	if( parseFloat(dtavaliacao) <  parseFloat('20111201') ){
		alert('Data da avaliação não pode ser menor que 01/12/2011.');
		d.avldataavaliacao.focus();
		return false;
	}
	
	<?if( in_array($_SESSION['pse']['acaoid'], array(5,8)) ){ ?>
		if(d.avlequipeescola[0].checked == false && d.avlequipeescola[1].checked == false){
			alert('Favor Preencher o campo obrigatório: Equipe que Realizou a Ação.');
			d.avlequipeescola[0].focus();
			return false;
		}
		if(d.avlequipeescola[0].checked == true && d.cneid.value == ''){
			alert('Favor Preencher o campo obrigatório: ESF - Equipe Saúde da Família.');
			d.cneid.focus();
			return false;
		}
	<?}else{?>
		if(d.cneid.value == ''){
			alert('Favor Preencher o campo obrigatório: ESF - Equipe Saúde da Família.');
			d.cneid.focus();
			return false;
		}
	<?}?>

	if(d.avleducandoavaliado.value == ''){
		alert('Favor Preencher o campo obrigatório: Número de <?=$labelTd1?>.');
		d.avleducandoavaliado.focus();
		return false;
	}
	if(d.avlidentifica1.value == ''){
		alert('Favor Preencher o campo obrigatório: Número de <?=$labelTd2?>.');
		d.avlidentifica1.focus();
		return false;
	}
	<?if( in_array($_SESSION['pse']['acaoid'], array(1)) ){ ?>
		if(d.avlidentifica2.value == ''){
			alert('Favor Preencher o campo obrigatório: Número de educandos com sobrepeso.');
			d.avlidentifica2.focus();
			return false;
		}
	<?}?>

	d.submit();

}


function alterarComp(id){
	location.href='pse.php?modulo=principal/cadastroMonitoramentoAcao&acao=A&avlid='+id;
}

function validarComp(id,status){
	location.href='pse.php?modulo=principal/cadastroMonitoramentoAcao&acao=A&validar='+status+'&avlid='+id;
}

function ficha(id){
	location.href='pse.php?modulo=principal/cadastroMonitoramentoFicha&acao=A&avlid='+id;
}

function cadDoenca(id){
	location.href='pse.php?modulo=principal/cadastroMonitoramentoDoenca&acao=A&avlid='+id;
}

function excluirComp(id){

	if(confirm('Deseja realmente excluir este item?')){
		location.href='pse.php?modulo=principal/cadastroMonitoramentoAcao&acao=A&exluir=1&avlid='+id;
	}
}
</script>