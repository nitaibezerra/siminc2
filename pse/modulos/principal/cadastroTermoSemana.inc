<?php
/*
$dataAtual = date('Y').date('m').date('d');
if(($dataAtual < 20120120 || $dataAtual > 20120224)){
	?>
	<script>
		//alert("Semana Saúde na Escola estará disponível apartir de 20/01/2012 até 24/02/2012.");
		//location.href='pse.php?modulo=principal/listaMunicipiosSemana&acao=A';
	</script>
	<?
	//exit;
}
*/

$entid = $_SESSION['pse']['entid'];


if(!$_SESSION['pse']['entid']){
	?>
	<script>
		alert("Sua sessão expirou. Por favor, entre novamente no PSE!");
		location.href='pse.php?modulo=principal/listaMunicipiosSemana&acao=A';
	</script>
	<?
	exit;
}


if($_REQUEST['alterarform']) $_SESSION['alterarform'] = "ok";
if($_POST['chk_acordo']) $_SESSION['alterarform'] = "";



function inserirDados(){
	global $db;
	
	$sql = "INSERT INTO pse.municipioparsse
			(
            	conid,
            	mssano, 
            	mssquantesf,
            	mssdataacordo
	        ) VALUES (
	        	".$_POST['conid'].",
	        	".$_POST['mssano'].",
	        	".($_POST['mssquantesf'] ? $_POST['mssquantesf'] : 'null').",
	        	".($_POST['chk_acordo'] ? "'".date("Y-m-d")."'" : 'null')."
	        ) returning mssid";
	$mssid = $db->pegaUm($sql);
	
	//insere as ações
	if($_POST['chk_acao']){
		foreach($_POST['chk_acao'] as $v){
			$sql = "INSERT INTO pse.municipioacaosse
					(
		            	mssid,
		            	assid
			        ) VALUES (
			        	".$mssid.",
			        	".$v."
			        )";
			$db->executar($sql);
		}
	}	

	//insere as escolas
	unset($v);
	if($_POST['chk_entid']){
		foreach($_POST['chk_entid'] as $v){
			$sql = "INSERT INTO pse.municipioescolasse
					(
		            	mssid,
		            	entid
			        ) VALUES (
			        	".$mssid.",
			        	".$v."
			        )";
			$db->executar($sql);
		}
	}	
	
	$db->commit();
}



function alterarDados(){
	global $db;
	
	$mssid = $_POST['mssid'];
	
	$sql = "UPDATE 
				pse.municipioparsse
			SET 
	            mssquantesf = ".($_POST['mssquantesf'] ? $_POST['mssquantesf'] : 'null').",
	            mssdataacordo = ".($_POST['chk_acordo'] ? "'".date("Y-m-d")."'" : 'null')."
			WHERE 
				mssid = ".$_POST['mssid'];
	$db->executar($sql);

	
	//insere as ações
	$sql = "DELETE FROM pse.municipioacaosse WHERE mssid = ".$mssid;
	$db->executar($sql);
	
	if($_POST['chk_acao']){
		foreach($_POST['chk_acao'] as $v){
			$sql = "INSERT INTO pse.municipioacaosse
					(
		            	mssid,
		            	assid
			        ) VALUES (
			        	".$mssid.",
			        	".$v."
			        )";
			$db->executar($sql);
		}
	}	

	//insere as escolas
	unset($v);
	if($_POST['chk_entid']){
		foreach($_POST['chk_entid'] as $v){
			$sql = "INSERT INTO pse.municipioescolasse
					(
		            	mssid,
		            	entid
			        ) VALUES (
			        	".$mssid.",
			        	".$v."
			        )";
			$db->executar($sql);
		}
	}	
	
	
	$db->commit();
}



if($_REQUEST['excluir'] == '1' && $_REQUEST['mesis']){
	$sql = "DELETE FROM pse.municipioescolasse WHERE mesis = ".$_REQUEST['mesis'];
	$db->executar($sql);
	$db->commit();
	
	print "<script>
				alert('Operação realizada com sucesso!');
				location.href='pse.php?modulo=principal/cadastroTermoSemana&acao=A';
			</script>";
}

//dbg($_REQUEST['alterarform']);
//dbg($_POST['alterarform'],1);

if($_POST){
	
	$_SESSION['permissaoedicaoSemana'] = true;
	
	if($_POST['conid']){
		if(!$_POST['mssid']) {
			inserirDados();
		}
		else{
			alterarDados();
		}
		unset($_POST);
		print "<script>
				alert('Operação realizada com sucesso!');
				location.href='pse.php?modulo=principal/cadastroTermoSemana&acao=A';
			</script>";
		
	}
	else{
		print "<script>
					alert('Erro ao gravar os dados! Favor entrar em contato com o administrador do sistema.');
					location.href='pse.php?modulo=principal/cadastroTermoSemana&acao=A';
			  </script>";
	}
	//header( "Location: demandas.php?modulo=sistema/apoio/origemDemanda&acao=A" );
	exit();
	
}




// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

	$sql = "SELECT mundescricao as municipio, estuf as uf
			FROM territorios.municipio
			WHERE muncod = '".$_SESSION['pse']['muncod']."'";
	$mun = $db->pegaLinha($sql);
	
	
	//pega entid da SECRETARIA MUNICIPAL DE EDUCAÇÃO de um determinado municipio
	$sql = "select et.entid, et.entnome from entidade.entidade et
			inner join entidade.endereco ed on ed.entid=et.entid
			inner join entidade.funcaoentidade fe on  fe.entid = et.entid AND fe.funid = 7
			where fe.fuestatus='A' and muncod = '".$_SESSION['pse']['muncod']."'";
	$secretaria = $db->pegaLinha($sql);
	
	$entidSecretaria = $secretaria['entid'];
	
	
	
	$titulo = "PSE - SEMANA SAÚDE NA ESCOLA";
	$dsctitulo = '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.<br>
	<table width="100%" align="center"  cellpadding="2" cellspacing="1">
	<tr>
		<td width="10%" style="text-align: right;" class="SubTituloDireita">UF:</td>
		<td width="90%" class="SubTituloEsquerda">'.$mun['uf'].'</td>
	</tr>
	<tr>
		<td style="text-align: right;" class="SubTituloDireita">MUNICÍPIO:</td>
		<td class="SubTituloEsquerda">'.$mun['municipio'].'</td>
	</tr>
	<tr>
		<td style="text-align: right;" class="SubTituloDireita">SECRETARIA:</td>
		<td class="SubTituloEsquerda">'.$secretaria['entnome'].'</td>
	</tr>
	</table>';
	

echo "<br>";

/*
$menu = array(0 => array("id" => 1, "descricao" => "Termo de Compromisso", 			"link" => "/pse/pse.php?modulo=principal/cadastroTermoCompromisso&acao=A"),
				  1 => array("id" => 2, "descricao" => "Semana Saúde na Escola", 	"link" => $_SERVER['REQUEST_URI'])
			  	  );
*/			  	  

$menu = array(0 => array("id" => 1, "descricao" => "Adesão Semana Saúde na Escola", "link" => $_SERVER['REQUEST_URI']),
			  1 => array("id" => 2, "descricao" => "Monitoramento de Açoes", 	"link" => "/pse/pse.php?modulo=principal/cadastroMonitoramento&acao=A")
		  	  );
		  	  
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
		


monta_titulo( $titulo, $dsctitulo );

echo "<br>";
	


//VERIFICA ENTID DA SECRETARIA DO MUNICIPIO	
	if($entidSecretaria){
		$sql = "SELECT  conid,
			            conesfatuarapse,
			            consenomesecretario,
			            conssnomesecretario,
			            arqid 
			     FROM 
			     		pse.contratualizacao
			     WHERE 
			     		arqid is not null
			     AND	entid = $entidSecretaria";
		//dbg($sql,1);
		$dados = $db->carregar($sql);
		
		if($dados){
			extract($dados[0]);
		}
		else{
				
			print '<script>
						alert("Acesso negado! \nEste Município não concluiu o cadastro do Termo de Compromisso.");
						location.href="pse.php?modulo=principal/listaMunicipiosSemana&acao=A";
					</script>';
			exit;
			
		}
		
		
		if($conid){
			$_SESSION['conid'] = $conid;
			$sql = "SELECT mssid, mssquantesf, to_char(mssdataacordo::timestamp,'DD/MM/YYYY') as mssdataacordo
					FROM pse.municipioparsse WHERE conid = ".$conid;
			$dados2 = $db->carregar($sql);
			
			if($dados2){
				extract($dados2[0]);
			}
			else{
				
				print '<script>
						alert("Acesso negado! \nEste Município não fez a adesão da Semana Saúde na Escola.");
						location.href="pse.php?modulo=principal/listaMunicipiosSemana&acao=A";
					</script>';
				exit;
				
			}
			
			if(!$mssdataacordo) $_SESSION['alterarform'] = 'ok'; 
		}
		
	}
//FIM VERIFICA ENTID DA SECRETARIA DO MUNICIPIO		
	
	
	
	
	//verifica se o municipio preencheu o termo de compromisso 100%
    
		$sql = "SELECT concoberturacomp1 as total1 FROM pse.contratualizacao 
				WHERE conid = $conid";
		$total1 = $db->pegaUm($sql);
		if(!$total1) $total1 = 0;
	
		$sql = "SELECT sum(pamquanteduseratend) as total2 FROM pse.planoacaoc1 
				WHERE conid = $conid";
		$total2 = $db->pegaUm($sql);
		if(!$total2) $total2 = 0;
	     
	    $sql = "SELECT concoberturacomp2 as total3 FROM pse.contratualizacao 
				WHERE conid = $conid";
		$total3 = $db->pegaUm($sql);
		if(!$total3) $total3 = 0;
		
		$sql = "SELECT sum(pacquanteduseratend) as total4 FROM pse.planoacaoc2 
				WHERE conid = $conid";
		$total4 = $db->pegaUm($sql);
		if(!$total4) $total4 = 0;
		
		$sql = "SELECT 
				 	count(mpaid) as total5
				 FROM 
				 	pse.metapactuada m
				 WHERE 
					mpaacao06comp1 is not null
				 and	mpaacao04comp3 is not null
				 and	mpaacao01comp3 is not null
				 and 	mpaacao02comp3 is not null
				 and	mpaacao03comp3 is not null
				 and    m.conid = $conid";
		$total5 = $db->pegaUm($sql);
		
		if( ($total1 == $total2 || $total1 < $total2) && ( $total3 == $total4 || $total3 < $total4) && ($total5 > 0) ){
			
			$_SESSION['permissaoedicaoSemana'] = false;
			
			/*
			if(($dataAtual < 20120120 || $dataAtual > 20120224)){
				$_SESSION['permissaoedicaoSemana'] = false;	
				?>
				<script>
					//alert("Semana Saúde na Escola estará disponível apartir de 20/01/2012 até 24/02/2012. \nApós esta data, só é permitido a visualização.");
					alert("Favor aguardar a liberação do módulo Monitoramento para informar as ações realizadas na Semana Saúde na Escola.");
				</script>
				<?
			}
			else{
				$_SESSION['permissaoedicaoSemana'] = true;
			} 
			*/
			    	
		}
		else{
			$_SESSION['permissaoedicaoSemana'] = false;
			
						print '<script>
						alert("Acesso negado! \nEste Município não concluiu o cadastro do Termo de Compromisso.");
						location.href="pse.php?modulo=principal/listaMunicipiosSemana&acao=A";
					</script>';
			exit;
			
		}
						
	
	// fim verifica
	
		
		
?>
	
<form id="formulario" name="formulario" action="" method="post">
	
	<input type="hidden" name="conid" value="<?=$conid?>">
	<input type="hidden" name="mssid" value="<?=$mssid?>">
	<input type="hidden" name="mssano" value="<?=$_SESSION['exercicio']?>">
	
	
	
<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
	<tr>
		<td class="TituloTela" style="background: rgb(238, 238, 238)" align="center" colspan='3'>
		     <b>SEMANA SAÚDE NA ESCOLA (05 À 09 DE MARÇO DE 2012)</b>
		</td>
	</tr>
	<tr><td style="height: 10px; background-color: #DCDCDC;" colspan='3'></td></tr>
	<tr>
		<td style="background: rgb(238, 238, 238)" colspan='3'>
			<p style="margin-left: 30px">
				<b>
					A Semana Saúde na Escola acontecerá anualmente&nbsp; a partir&nbsp; de 2012&nbsp; com&nbsp; um&nbsp; tema&nbsp; de&nbsp; mobilização&nbsp; nacional.&nbsp; Para&nbsp; o&nbsp; ano&nbsp; de&nbsp; 2012&nbsp; foi&nbsp; escolhido&nbsp; o&nbsp; tema&nbsp; de prevenção&nbsp; da obesidade.&nbsp; Essa&nbsp; Semana é uma&nbsp; parceria entre os Ministérios da Saúde e da Educação e acontecerá entre os dias 05 à 09 de março de 2012 para os&nbsp; escolares&nbsp; e&nbsp; suas&nbsp; famílias, com&nbsp; o&nbsp; intuito&nbsp; de&nbsp; alertar&nbsp; para&nbsp; a&nbsp; importância&nbsp; das&nbsp; boas&nbsp; condições&nbsp; de&nbsp; saúde&nbsp; para&nbsp; a&nbsp; melhoria&nbsp; no&nbsp; desenvolvimento&nbsp; pleno&nbsp; da criança/adolescente tanto na escola como na formação da cidadania, visando a aproximação da população com a Atenção Básica à Saúde. <br><br>Os municípios que desejarem participar da Semana Saúde na Escola 2012 devem se comprometer a realizar ações de avaliação antropométrica e orientar visitas das famílias às Unidades Básicas de Saúde (UBS). Além disso, poderão ser escolhidas outras duas ações, elencadas a seguir.<br><br>A&nbsp; adesão à&nbsp;&nbsp; Semana&nbsp; Saúde&nbsp; na&nbsp; Escola&nbsp; é&nbsp; voluntária, devendo&nbsp; ser&nbsp; realizada&nbsp; pelo&nbsp; Grupo&nbsp; de Trabalho&nbsp; Intersetorial&nbsp; Municipal&nbsp; (GTI-M)&nbsp; do&nbsp;&nbsp; PSE até o dia 24/02/2012,&nbsp; considerando as&nbsp; parcerias&nbsp; entre&nbsp; a equipe da escola e a equipe de saúde&nbsp; da&nbsp; família (equipes parceiras). <br><br>Está&nbsp; previsto&nbsp; repasse de&nbsp; incentivo&nbsp; financeiro&nbsp; extra&nbsp; visando&nbsp; a&nbsp; continuidade&nbsp; do&nbsp; desenvolvimento&nbsp; das&nbsp; ações&nbsp; com os enfoques da campanha ao longo do ano letivo. O incentivo&nbsp; será&nbsp; repassado&nbsp; de&nbsp; acordo&nbsp; com a&nbsp; quantidade&nbsp; de equipes de&nbsp; saúde da&nbsp; família informadas&nbsp; no&nbsp; Sistema de Monitoramento&nbsp; do PSE (<?php echo SIGLA_SISTEMA; ?>), na aba Semana Saúde na Escola 2012 . O&nbsp; valor&nbsp; desse&nbsp; incentivo&nbsp; é&nbsp; de&nbsp; 1/12&nbsp; da&nbsp; parcela mensal da equipe de saúde da família, cabendo ao GTI-M sua gestão.<br><br>O resultado&nbsp; das ações&nbsp; realizadas ao&nbsp; longo da Semana&nbsp; deverá ser registrado no <?php echo SIGLA_SISTEMA; ?> na aba Semana Saúde na&nbsp; Escola 2012 até 30/04/2012 e contará para o alcance das metas pactuadas no Termo de Compromisso.<br><br>Para aderir à Semana Saúde na Escola 2012 o município deve preencher as informações a seguir e ao final clicar em DE ACORDO e CONFIRMAR.
					<br><br>
					É importante que o GTI-M faça um planejamento conjunto das ações locais que serão desenvolvidas na Semana Saúde na Escola, o qual resultará na construção de uma agenda de ações a serem realizadas ao longo do ano. Esse planejamento/agenda de ações deve envolver as equipes da escola e as equipes da rede de Atenção Básica.
				</b>		
			</p>
		</td>
	</tr>
	<tr><td style="height: 10px; background-color: #DCDCDC;" colspan='3'></td></tr>
	<tr>
		<td style="background: rgb(238, 238, 238)" colspan='3'>
			<p style="margin-left: 30px">
			    O município de <b><?=$mun['municipio']?></b> se compromete a realizar as <b>ações obrigatórias <u>de avaliação antropométrica e orientação de visitas das famílias às Unidades Básicas de Saúde (UBS)</u>, </b>bem como as <b>ações opcionais a seguir (escolher no mínimo 2):</b>
			    
			    <br><br>
			    
			    <?php 
			    $sql = "select assid, assdescricao from pse.acaosse";
			    $acaosse = $db->carregar($sql);
			    ?>
			    <div id="chk_acaosse">
			    <table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
			    	<?foreach($acaosse as $a){

			    		$checado = "";
			    		if($mssid){
							$existeAcao = $db->carregarColuna('SELECT masid FROM pse.municipioacaosse where mssid = '.$mssid.' and assid = '.$a['assid']);
							if($existeAcao) $checado = "checked"; 
						}
						
						if(!$_SESSION['alterarform']) $acaoDesabilitado = "disabled";
						 
			    		
			    		?>
				    	<tr>
				    		<td align="center" style="background-color: #DCDCDC;">
				    			<input type="checkbox" class="normal"  name="chk_acao[]" value="<?=$a['assid']?>" <?=$checado?> <?=$acaoDesabilitado?>>
				    		</td>
				    		<td>
				    			<b><?=$a['assdescricao']?></b>
				    		</td>
				    	</tr>
				    <?}?>
			    </table>
			    </div>
		    </p>
		</td>
	</tr>
	<tr><td style="height: 10px; background-color: #DCDCDC;" colspan='3'></td></tr>
	<tr>
		<td style="background: rgb(238, 238, 238)" colspan='3'>
			<p style="margin-left: 30px">
				Informe que, das
				&nbsp;
				<?=campo_texto( 'conesfatuarapse', 'N', 'N', 'Total de Equipes da Saúde da Família', 3, 3, '###', '', 'right', '', 0, '');?>
				&nbsp;	
				Equipes de Saúde da Família pactuadas no termo de Compromisso do PSE
				&nbsp;
				<?=campo_texto( 'mssquantesf', 'N', ($_SESSION['alterarform'] ? 'S' : 'N'), 'Informe a quantidade de Equipes da Saúde da Família', 3, 3, '###', '', 'right', '', 0, '');?>
				&nbsp;
				participarão da Semana Saúde na Escola.	
			</p>
		</td>
	</tr>
	<tr><td style="height: 10px; background-color: #DCDCDC;" colspan='3'></td></tr>
	<tr>
		<td style="background: rgb(238, 238, 238)" colspan='3'>
			<?
				if($_SESSION['permissaoedicaoSemana'] == true && $_SESSION['alterarform']){
					$img = "'<center><input type=\"checkbox\" class=\"normal\" name=\"chk_entid[]\" value=\"' || pc.entid || '\"></center>'";
				} else {
					$img = "'<center><input type=\"checkbox\" class=\"normal\" name=\"chk_entid[]\" value=\"' || pc.entid || '\" disabled></center>'";
				}
				
				//verifica se escola inserida na semana saude
				if($mssid){
					$existeEscola = $db->carregarColuna('SELECT entid FROM pse.municipioescolasse where mssid = '.$mssid);
					if($existeEscola) $andEntid = "and pc.entid not in (".implode(",",$existeEscola).")"; 
				}

				$sql = "SELECT
							$img as acao, 
							ent.entnome
						FROM pse.planoacaoc1 pc
						INNER JOIN entidade.entidade ent on ent.entid=pc.entid
						WHERE pc.conid = {$conid}
						$andEntid
						
						UNION
						
						SELECT
							$img as acao, 
							ent.entnome
						FROM pse.planoacaoc2 pc
						INNER JOIN entidade.entidade ent on ent.entid=pc.entid
						WHERE pc.conid = {$conid}
						$andEntid
						
						order by 2";
				$dados = $db->carregar($sql);
				
				
				$sqlTotal = "SELECT
								ent.entid
							FROM pse.planoacaoc1 pc
							INNER JOIN entidade.entidade ent on ent.entid=pc.entid
							WHERE pc.conid = {$conid}
							UNION
							SELECT
								ent.entid
							FROM pse.planoacaoc2 pc
							INNER JOIN entidade.entidade ent on ent.entid=pc.entid
							WHERE pc.conid = {$conid}";
				$totalescolas = count($db->carregarColuna($sqlTotal));				
				
			?>
			<p style="margin-left: 30px">
				E que das
				&nbsp;
				<?=campo_texto( 'totalescolas', 'N', 'N', 'Total de escolas do município que foram pactuadas no Plano de Ação', 3, 3, '###', '', 'right', '', 0, '');?>
				&nbsp;	
				escolas pactuadas no Termo de Compromisso do PSE, as escolas assinaladas a seguir participarão da Semana Saúde na Escola.
				
				<br><br>
			    
			    <center><b>Escolas que não irão participar da Semana Saúde</b></center>
				<div id="lista1" style="height: 100px; overflow-y: auto; overflow-x: hidden;">
					<?php 
						
						/*
						$sql = "SELECT
									$img as acao, 
									ent.entnome
								FROM pse.planoacaoc1 pc
								INNER JOIN entidade.entidade ent on ent.entid=pc.entid
								WHERE pc.conid = {$_SESSION['conid']}";
						*/
						$cabecalho 	= array( "Ação", "Escola");
						//$alinha = array("center","left","center","center","left","center");
						//$tamanho = array("5%","25%","10%","20%","20%","10%");
						//$db->monta_lista( $sql, $cabecalho, 500, 10, 'N', 'center', '', '',$tamanho,$alinha);
						
						
						//$db->monta_lista_simples( $sql, $cabecalho, 5000, 10, 'N', '100%', 'N' );
						/*
						$dados = $db->carregar($sql);
						if($dados){
							for($i = 0; $i<=count($dados)-1; $i++ ){
								
								$sqlx = "SELECT
													s.cnecodigocnes ||' - '|| s.cnenomefantasia ||' - '|| s.cneseqequipe as descricao 
											   FROM
											   		pse.escolaesf e
											   INNER JOIN
											   		pse.scnes s ON s.cneid = e.cneid
											   WHERE
											   		e.pamid = ".$dados[$i]['equipe']."
											   ORDER BY s.cnenomefantasia";
								$dadosx = $db->carregarColuna($sqlx);
								if($dadosx){
									$dados[$i]['equipe'] = implode( ";<br>", $dadosx );
								}
								else{ 
									$dados[$i]['equipe'] = '';
								}
							}
						}
						
						*/
						$db->monta_lista_array($dados,$cabecalho,5000,10,'N','center','','',false);									
					?>
				</div>
				<br>	
					
				<p style="margin-left: 90px">
					<!-- 									    			
			    	<input type="button" class="botao" name="btn_incluir" value="Incluir" onclick="incluirEscola();" <?if($mssdataacordo && !$_SESSION['alterarform']) echo "disabled";?>>
			    	 -->
			    </p>
				
				<center><b>Escolas que irão participar da Semana Saúde</b></center>
				<div id="lista2" style="height: 100px; overflow-y: auto; overflow-x: hidden;">
					<?php 
						if($_SESSION['permissaoedicaoSemana'] == true && $_SESSION['alterarform']){
							$img = "<a href=\"#\" onclick=\"excluirEscola(\'' || me.mesis || '\');\" title=\"Excluir\" name=\"imgExcluir\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>";
						} else {
							$img = '<img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">';
						}
					
						$sql = "SELECT
									'<center>$img</center>' as acao, 
									ent.entnome
								FROM pse.municipioescolasse me
								INNER JOIN entidade.entidade ent on ent.entid=me.entid
								WHERE me.mssid = " . ($mssid ? $mssid : 0);
						//dbg($sql);
						$cabecalho 	= array( "Ação", "Escola");
						//$alinha = array("center","left","center","center","left","center");
						//$tamanho = array("5%","25%","10%","20%","20%","10%");
						//$db->monta_lista( $sql, $cabecalho, 500, 10, 'N', 'center', '', '',$tamanho,$alinha);
						
						
						//$db->monta_lista_simples( $sql, $cabecalho, 5000, 10, 'N', '100%', 'N' );
						/*
						$dados = $db->carregar($sql);
						if($dados){
							for($i = 0; $i<=count($dados)-1; $i++ ){
								
								$sqlx = "SELECT
													s.cnecodigocnes ||' - '|| s.cnenomefantasia ||' - '|| s.cneseqequipe as descricao 
											   FROM
											   		pse.escolaesf e
											   INNER JOIN
											   		pse.scnes s ON s.cneid = e.cneid
											   WHERE
											   		e.pamid = ".$dados[$i]['equipe']."
											   ORDER BY s.cnenomefantasia";
								$dadosx = $db->carregarColuna($sqlx);
								if($dadosx){
									$dados[$i]['equipe'] = implode( ";<br>", $dadosx );
								}
								else{ 
									$dados[$i]['equipe'] = '';
								}
							}
						}
						$db->monta_lista_array($dados,$cabecalho,5000,10,'N','center','','',false);
						*/	
						unset($dados);
						$dados = $db->carregar($sql);
						$db->monta_lista_array($dados,$cabecalho,5000,10,'N','center','','',false);								
					?>
				</div>											    			
			    
			    	
			    <br><br>
			    
			    <table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
			    	<tr>
			    		<td width="20%">
			    			<b><?
			    				if($mssdataacordo){
			    					echo $mssdataacordo;
			    				}
			    				else{
			    					echo date("d/m/Y");		
			    				}
			    				?>
			    			</b>			
			    		</td>
			    		<td width="40%">
			    			<?=$consenomesecretario?>
			    			<br>
			    			<b>Secretário(a) Municipal de Educação</b> 		
			    		</td>
			    		<td width="40%">
			    			<?=$conssnomesecretario?>
			    			<br>
			    			<b>Secretário(a) Municipal de Saúde</b> 		
			    		</td>
			    	</tr>
			    </table>
			    		
	    		<br><br>
	    		
	    		<p style="margin-left: 30px">

		    		<input type="checkbox" class="normal" name="chk_acordo" <?if($mssdataacordo && !$_SESSION['alterarform']) echo "checked";?> <?if(!$_SESSION['alterarform']) echo "disabled";?>>
		    		
		    		&nbsp;&nbsp;&nbsp;
		    		De acordo. 
		    		&nbsp;&nbsp;&nbsp;
		    		<!-- 
			    		<input type="button" class="botao" name="btn_confirmar" value="Confirmar" onclick="validaForm();" <?if($mssdataacordo && !$_SESSION['alterarform']) echo "disabled";?>>
			    		
			    		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			    		<?if($mssdataacordo && !$_SESSION['alterarform']){?>
			    			<input type="button" class="botao" name="btn_alterar" value="Alterar" onclick="alteraForm();">
			    		<?}?>
			    	 -->
				</p>
					    				
			</p>
		</td>
	</tr>	
</table>
	
</form>

<?php 

	
	//$existeEscolaRural = $db->pegaUm("SELECT count(memid) FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION["memid"]." AND memstatus = 'A' AND memclassificacaoescola = 'R' AND memanoreferencia = ".((integer)$_SESSION["exercicio"]));
	
	//if((integer)$existeEscolaRural > 0) { 

		/*
		$texto = "<div style=\"font-size:12px\" >
					<center><b><font color=red>AVISO</font></b></center>
					<br>
					O PRAZO PARA PREENCHIMENTO DAS INFORMAÇÕES DAS AÇÕES REALIZADAS NA SEMANA SAÚDE NA ESCOLA ENCERRA DIA 30 DE NOVEMBRO DE 2012 !
					<br><br> Atenciosamente,
					<br> Equipe Saúde na Escola.
		 		  </div>";
		popupAlertaGeral($texto,'450px',"150px");
		*/
	//}
?>

	
<script type="text/javascript">
	
	var d = document.formulario;

	function incluirEscola(){
		
		var obj = document.getElementsByName("chk_entid[]");
		var total = obj.length;
		var chkentid = 0;

		for(i=0; i<total; i++){
			if(obj[i].checked == true){
				chkentid++;
				break;
			}
			
		}

		if(chkentid == 0){
			alert ('Favor inserir pelo menos uma escola.');
			return false;
		}
		else{ 

			if( trim(d.mssquantesf.value) != '' ){
				var conesfatuarapse = "<?=$conesfatuarapse?>";
				if(!conesfatuarapse) conesfatuarapse = "0";
				if( parseFloat(trim(d.mssquantesf.value)) > parseFloat(conesfatuarapse) ){ 
					alert('A quantidade de equipes que participarão não pode ser maior que a quantidade de equipes pactuadas.'); 
					d.mssquantesf.focus();
					return false; 
				}
			}

			d.submit();
		}
	}
	
	function validaForm(){

		var obj = document.getElementsByName("chk_acao[]");
		var total = obj.length;
		var chkacao = 0;

		for(i=0; i<total; i++){
			if(obj[i].checked == true){
				chkacao++;
				if(chkacao == 2) break;
			}
			
		}

		if(chkacao < 2){
			alert ('Favor selecionar pelo menos duas ações.');
			return false;
		}

		if( trim(d.mssquantesf.value) == '' ){ 
			alert('Favor informar a quantidade de equipes.'); 
			d.mssquantesf.focus();
			return false; 
		}

		var conesfatuarapse = "<?=$conesfatuarapse?>";
		if(!conesfatuarapse) conesfatuarapse = "0";
		if( parseFloat(trim(d.mssquantesf.value)) > parseFloat(conesfatuarapse) ){ 
			alert('A quantidade de equipes que participarão não pode ser maior que a quantidade de equipes pactuadas.'); 
			d.mssquantesf.focus();
			return false; 
		}

		 		
		
		var existeEscola = "<?=$andEntid?>";
		if(!existeEscola){
			alert ('Favor inserir pelo menos uma escola no botão "Inserir".');
			return false;
		}

		if(d.chk_acordo.checked == false ){ 
			alert('Favor marcar o campo "De acordo".'); 
			d.chk_acordo.focus();
			return false; 
		}
		

		
		d.submit();
	
	}

	function alteraForm(){
		/*
		d.btn_alterar.disabled = true;
		d.btn_confirmar.disabled = false;
		d.btn_incluir.disabled = false;

		var obj = document.getElementsByName("imgExcluir");
		var total = document.getElementsByName("imgExcluir").length;
	
		for(i=0; i<total; i++){
			obj[i].style.display = '';
		}
		*/	
		//document.getElementById("alterarform").value = '1';
		//d.submit();
		location.href='pse.php?modulo=principal/cadastroTermoSemana&acao=A&alterarform=1';	
	}

	function excluirEscola(mesis)
	{
		if(confirm('Deseja excluir esta escola da lista?')){
			location.href='pse.php?modulo=principal/cadastroTermoSemana&acao=A&excluir=1&mesis='+mesis;
		}
	}


	
	<?php if($_SESSION['permissaoedicaoSemana'] == false){?>
		var obj = document.getElementsByTagName("input");
		var total = document.getElementsByTagName("input").length;
	
		for(i=0; i<total; i++){
			obj[i].disabled = true;
		}
	<?php }?>

	<?php if($mssdataacordo){?>
	/*
		var obj = document.getElementsByName("imgExcluir");
		var total = document.getElementsByName("imgExcluir").length;
	
		for(i=0; i<total; i++){
			obj[i].style.display = 'none';
		}
		*/
	<?php }?>
	
</script>
		




	