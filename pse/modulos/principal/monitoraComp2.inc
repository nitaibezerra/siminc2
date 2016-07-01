<?php

$entid = $_SESSION['pse']['entid'];

$vpflcod = pegaPerfil($_SESSION['usucpf']);

if(!$entid || !$_SESSION['pse']['sse']){
	?>
	<script>
		alert("Sua sessão expirou. Por favor, entre novamente!");
		location.href='pse.php?modulo=inicio&acao=C';
	</script>
	<?php
	exit;
}

//dbg($_SESSION['pse']['sse'],1);

if($_SESSION['pse']['sse'] == 'S'){ //semana saude
	
	//verifica se o municipio tem acesso
	$mssid = $db->pegaUm("select mssid from pse.municipioescolasse where entid = $entid");
	if($mssid){
		$mssdataacordo = $db->pegaUm("select mssdataacordo from pse.municipioparsse where mssid = $mssid");
	}

	if(!$mssid || !$mssdataacordo){
		?>
		<script>
			alert("Acesso Negado! \nEscola não selecionada na adesão.");
			location.href='pse.php?modulo=inicio&acao=C';
		</script>
		<?php
		exit;
	}
	
}elseif($_SESSION['pse']['sse'] == 'N'){ // monitoramento de ações	
	
	//verifica se o municipio tem acesso
	/*
	$sql = "SELECT mueid from pse.municipioexcluido WHERE  mueibge = '".$_SESSION['pse']['muncod']."'";
	$mueid = $db->pegaUm($sql);
	if($mueid){
		?>
		<script>
			alert("Conforme ofício encaminhado pelo Ministério da Saúde, por irregularidade de todas as equipes Saúde da Família do Município, foi anulado o processo de pactuação, assim como o Termo de Compromisso Municipal informados no Sistema de Avaliação e Monitoramento do PSE – SIMEC. \n\nDúvidas entre em contato pelo telefone (61) 3306-8091");
			location.href='pse.php?modulo=inicio&acao=C';
		</script>
		<?
		exit;
	}
	*/
	
	//pega entid da secretaria municipal ou estadual
	$sql = "select et.entid, et.entnome from entidade.entidade et
		inner join entidade.endereco ed on ed.entid=et.entid
		inner join entidade.funcaoentidade fe on  fe.entid = et.entid AND fe.funid = 7
		where fe.fuestatus='A' and muncod = '".$_SESSION['pse']['muncod']."'
		order by 1 limit 1";
	$secretaria = $db->pegaLinha($sql);


	//verifica se foi pactuado
	/*
	if($secretaria['entid']){
		$sql = "SELECT arqid FROM pse.contratualizacao WHERE entid = ".$secretaria['entid'];
		$arqid = $db->pegaUm($sql);
	}
	
	if(!$arqid){
		?>
		<script>
			alert("Acesso Negado! \nEscola não pactuada no Termo de Compromisso.");
			location.href='pse.php?modulo=inicio&acao=C';
		</script>
		<?
		exit;
	}
	*/
	
	/*
	//verifica se tem registro na tabela planoacao1
	$sql = "select count(entid) as total from pse.planoacaoc1 where entid = ".$entid;
	$totalPlanoC1 = $db->pegaUm($sql);
	
	//verifica se tem registro na tabela planoacao2
	$sql = "select count(entid) as total from pse.planoacaoc2 where entid = ".$entid;
	$totalPlanoC2 = $db->pegaUm($sql);
	
	
	if($totalPlanoC1 == 0){

		if($totalPlanoC2 == 0){
			?>
			<script>
				alert("Acesso Negado! \nEscola não pactuada no Termo de Compromisso.");
				location.href='pse.php?modulo=inicio&acao=C';
			</script>
			<?
			exit;
		}
		
	}
	*/
	
}elseif($_SESSION['pse']['sse'] == 'B'){ // brasil carinhoso	
		
}


$tipo = $_REQUEST['tipo'];


//rerediciona para as ações 1 ou 2
if($_REQUEST['comid'] && $_REQUEST['acaoid']){
	$_SESSION['pse']['comid'] = $_REQUEST['comid'];
	$_SESSION['pse']['acaoid'] = $_REQUEST['acaoid'];
	
	if($_SESSION['pse']['comid'] == '1'){
		?>
			<script>
				location.href="pse.php?modulo=principal/cadastroComp1&acao=A";
			</script>
		<?
	}
	else{
		
		if($tipo == '2'){
			?>
				<script>
					location.href="pse.php?modulo=principal/cadastroComp22&acao=A";
				</script>
			<?
			exit;
		}
		
		if($tipo == '1' && $_SESSION['pse']['acaoid'] == '999'){
			?>
				<script>
					location.href="pse.php?modulo=principal/monitoraComp2&acao=A&tipo=1";
				</script>
			<?
			exit;
		}
		
		if($tipo == '1' && $_SESSION['pse']['acaoid'] != '999'){
			?>
				<script>
					location.href="pse.php?modulo=principal/cadastroComp21&acao=A";
				</script>
			<?
		}
		
		?>
		<script>
			location.href="pse.php?modulo=principal/cadastroComp2&acao=A";
		</script>
		<?
		exit;
		
		
	}
	exit;
}

unset($_SESSION['pse']['comid']);
unset($_SESSION['pse']['acaoid']);
unset($_SESSION['pse']['idave']);



// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

echo "<br>";

/*
$menu = array(0 => array("id" => 1, "descricao" => "Termo de Compromisso", 			"link" => "/pse/pse.php?modulo=principal/cadastroTermoCompromisso&acao=A"),
			  1 => array("id" => 2, "descricao" => "Monitoramento de Ações", 	"link" => "/pse/pse.php?modulo=principal/monitoraComp2&acao=A"),
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
$dsctitulo = 'Selecione uma ação abaixo.';

monta_titulo( $titulo, $dsctitulo );

Cabecalho($entid,1);


//$db->cria_aba( $abacod_tela, $url, '');

?>

<form id="formulario" name="formulario" action="" method="post">

<?
//verifica se possui quantidade de visita na Semana Saude 
/*
if($_SESSION['pse']['sse'] == 'S' && !$_POST){
	$mesquantvisita = $db->pegaUm("select mesquantvisita from pse.municipioescolasse where entid = $entid");
	
	if(!$mesquantvisita){
		?>
		<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
			<tr>
				<td style="background: rgb(168, 168, 168);" colspan='4' align="center"><b>INFORME</b></td>
			</tr>
			<tr>
				<td width="50%" class="SubTituloDireita" colspan='2'>Quantidade de visitas orientadas às unidades básicas de saúde:</td>
				<td colspan='2'>
					<?=campo_texto( 'mesquantvisita', 'S', $vPermissao, '', 6, 4, '####', '', 'right', '', 0, '');?>
					&nbsp;
					<input type="button" class="botao" name="btQtdSalvar" id="btQtdSalvar" value="Salvar" onclick="document.formulario.submit();">
				</td>
			</tr>
		</table>		
		<?
		include APPRAIZ . 'includes/rodape.inc';
		exit;
	}
}
*/
?>


<table  class="Tabela" align="center"  cellpadding="2" cellspacing="1">
<tr>
	<td >
		<table border="0" width="100%" cellpadding="0" cellspacing="0" >
			<tr>
				<td style="background: rgb(168, 168, 168);" >&nbsp;</td>
			</tr>
			<tr>
				<td class="SubTituloCentro" valign="top" align="center">
					 <br>
					 <font color="red">
				     <b> 
				     	COMPONENTE II - PROMOÇÃO DA SAÚDE E PREVENÇÃO
				     	<br>
				     </b>
				     </font>
				     <br>
				</td>
			</tr>
			<tr>
				<td style="background: rgb(168, 168, 168);" >&nbsp;</td>
			</tr>
	    </table>
	</td>
</tr>


<table  class="Tabela" align="center"  cellpadding="2" cellspacing="1">
    <?php if ($vpflcod == SUPER_USUARIO || $vpflcod == EDUCADOR_ESCOLA || $vpflcod == MEC) {?>
<tr>
	<td >
		<table border="0" width="100%" >
			<tr>
				<td class="SubTituloCentro" valign="top" align="center">
					 <br>
					 <font color="red">
				     <b> 
				     	<?if($_SESSION['pse']['sse'] == 'B'){
					     	//echo 'O módulo BRASIL CARINHOSO do PSE deve ser preenchido por profissionais da Atenção Básica.';
					    }else{
					    	//echo 'O Componente I do PSE no módulo monitoramento na escola deve ser preenchido por profissionais da Atenção Básica e validado pelo diretor da escola.';
					     	//echo '<br><br>'; 
					     	//echo 'O Componente II do PSE no módulo monitoramento na escola deve ser preenchido pela equipe da escola ou por profissionais da Atenção Básica e validado pelo diretor da escola.';
					    }
					   // echo 'O Componente II do PSE no módulo monitoramento na escola deve ser preenchido pela equipe da escola ou por profissionais da Atenção Básica e validado pelo diretor da escola.';
					    ?> 
				     	<br>
				     </b>
				     </font>
				     <br>
				</td>
				<td class="SubTituloCentro"  align="center" width="5%">
					<img border="0" title="Imprimir Relatório." src="../imagens/ico_print.jpg" style="cursor: hand" onclick="window.open('pse.php?modulo=relatorio/resultMonitoramentoAcoes&acao=A&comp=2', 'relatorio', 'height=600, width=780, scrollbars=1');">
				</td>
			</tr>
	    </table>
	     
	</td>
</tr>
    <?php }?>

<!-- 
<tr>
	<td style="background: rgb(168, 168, 168);" colspan='3'><center>
		 <span style="font-size: 12"><b>COMPONENTE I - AVALIAÇÃO CLÍNICA E PSICOSOCIAL</b></span>
		 </center>
	</td>
</tr>
<tr>
	<td align="center" style="background: rgb(218, 218, 218);"  colspan='3'><span style="font-size: 12"><b>AÇÃO</b></span></td>
</tr>
<tr>
	<td align="center" style="background-color: #f5f5f5">
		<table width="90%" align="center" cellpadding="5" cellspacing="3">
			<?php 
			if($totalPlanoC1 == 0 && $_SESSION['pse']['sse'] == 'N'){
				echo '<tr><td align="center">Não existe ação pactuada para esta escola.</td></tr>';
			}
			else{
				
			
				if($_SESSION['pse']['sse'] == 'S'){
					
					$sql = "select acaoid, acaodescricao, comid
						from pse.acao 
						where comid = 1 and acaoid = 1";
					
				}elseif($_SESSION['pse']['sse'] == 'N'){
					
					$sql = "select acaoid, acaodescricao, comid
							from pse.acao 
							where comid = 1";
				}elseif($_SESSION['pse']['sse'] == 'B'){
					
					$sql = "select acaoid, acaodescricao, comid
							from pse.acao 
							where comid = 1 and acaoid in (1,3,7)";

				}
				
				//$dados = $db->carregar($sql);
				$dados = array();
				foreach($dados as $d){
					
					$acaoid = $d['acaoid'];
					
					//verifica monitora se ja existe
					$sql = "SELECT moeid from pse.monitoraescola
							WHERE entid = ".$entid." and acaoid = ".$acaoid." and moeidentificasse = '".$_SESSION['pse']['sse']."'";
					$moeid = $db->pegaUm($sql);
					
					$cor = 'black';
					$dscPendencia = '';
					
					if($moeid){
					
						//verifica avaliacao 
						$sql = "select count(avlid) as total from pse.avaliacao where comid = 1 and acaoid = ".$acaoid." and moeid = ".$moeid;
						$totalAvaliacao = $db->pegaUm($sql);
						if(!$totalAvaliacao) $totalAvaliacao = 0;
	
						//verifica avaliacao validada 
						$sql = "select count(avlid) as total from pse.avaliacao where avlvalidar='S' and comid = 1 and acaoid = ".$acaoid." and moeid = ".$moeid;
						$totalAvaliacaoValidadas = $db->pegaUm($sql);
						if(!$totalAvaliacaoValidadas) $totalAvaliacaoValidadas = 0;
						
						//verifica avaliacao == avaliacao validada
						if($totalAvaliacao == $totalAvaliacaoValidadas) {
							
							if($totalAvaliacao == 0) $cor = 'black';
							else $cor = 'blue';
							
							//$cor = 'black';
							$dscPendencia = '';
						}
						else {
							$cor = 'red';
							
							if( ($totalAvaliacao - $totalAvaliacaoValidadas) == 1 ){
								$dscPendencia = "Falta ".($totalAvaliacao - $totalAvaliacaoValidadas)." avaliação que não foi validada";
							}
							else{
								$dscPendencia = "Faltam ".($totalAvaliacao - $totalAvaliacaoValidadas)." avaliações que não foram validadas";
							}
						}

						
							
						//verifica acoes com fichas
						if( in_array($acaoid, array(1,2,5,6)) ){

							//verifica total de fichas por acao
							$sql = "select 
										avlid, avlidentifica1, to_char(avldataavaliacao::date, 'DD/MM/YYYY') as dataaval
									from pse.avaliacao 
									where avlidentifica1 > 0 and comid = 1 and acaoid = ".$acaoid." and moeid = ".$moeid;
							$vFichas = $db->carregar($sql);
							
							if($vFichas){
								
								foreach ($vFichas as $v){
									
									// verifica perguntas
									/*
									if( in_array($acaoid, array(5,6)) ){
										
										$sql = "select count(acpid) as total from pse.acompanhamento where fieid in (select fieid from pse.fichaeducando where avlid = ".$v['avlid'].")";
										$totalPerguntas = $db->pegaUm($sql);
										if(!$totalPerguntas) $totalPerguntas = 0;
										
										if($totalPerguntas != $v['avlidentifica1']) {
											$cor = 'red';
											$dscPendencia = 'É necessário responder as perguntas da ficha do educando da avaliação - Data: '.$v['dataaval'];
											break;
										}
										
									}
									*/
									
									
									//verifica fichas
									$sql = "select count(fieid) as total from pse.fichaeducando where avlid = ".$v['avlid'];
									$totalFicha = $db->pegaUm($sql);
									if(!$totalFicha) $totalFicha = 0;
									
									if($totalFicha != $v['avlidentifica1']) {
										$cor = 'red';
										$dscPendencia = 'Total de Fichas deve ter o mesmo total de educandos da avaliação - Data: '.$v['dataaval'];
										break;
									}
									
									
							
								}
							}
							
						}
						
						// verifica acoes com doencas
						if( in_array($acaoid, array(4)) ){
							
							$sql = "select 
										avlid, avlidentifica1, to_char(avldataavaliacao::date, 'DD/MM/YYYY') as dataaval
									from pse.avaliacao 
									where avlidentifica1 > 0 and comid = 1 and acaoid = ".$acaoid." and moeid = ".$moeid;
							$vDoencas = $db->carregar($sql);
							
							if($vDoencas){
								foreach ($vDoencas as $v){
									$sql = "select sum(eddquantidade) as total from pse.educandodoenca where avlid = ".$v['avlid'];
									$totalDoencas = $db->pegaUm($sql);
									if(!$totalDoencas) $totalDoencas = 0;
									
									if($totalDoencas != $v['avlidentifica1']) {
										$cor = 'red';
										$dscPendencia = 'Total de Educandos com doenças deve ter o mesmo total de educandos da avaliação - Data: '.$v['dataaval'];
										break;
									}
								}
							}
							
						}
						
					}
					
					?>
					<tr>
						<td colspan='3'>
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
							<b>
								<a href="pse.php?modulo=principal/monitoraComp2&acao=A&comid=<?=$d['comid']?>&acaoid=<?=$acaoid?>">
									<font title="<?=$dscPendencia?>" color="<?=$cor?>"><?=$d['acaodescricao']?></font>
								</a>
							</b>
						</td>
					</tr>
				<?} // fehca foreach($dados as $d){
			} // fecha if($totalPlanoC1 == 0 && $_SESSION['pse']['sse'] == 'N'){
			?>
		</table>
	</td>
</tr>
 -->
 
<?if($_SESSION['pse']['sse'] != 'B'){?>
	<!-- 
	<tr>
		<td style="background: rgb(168, 168, 168);" colspan='3'><center>
			 <span style="font-size: 12"><b>COMPONENTE II - PROMOÇÃO E PREVENÇÃO À SAÚDE</b></span>
			 </center></td>
	</tr>
	<tr>
		<td align="center" style="background: rgb(218, 218, 218);"  colspan='3'><span style="font-size: 12"><b>AÇÃO</b></span></td>
	</tr>
	 -->
	<tr>
		<td align="center" style="background-color: #f5f5f5">
			<table width="90%" align="center" cellpadding="5" cellspacing="3">
				<?php 
				if($_SESSION['pse']['sse'] == 'B'){
					echo '<tr><td align="center">Não existe ação para o Programa Brasil Carinhoso.</td></tr>';
				}
				else{
				
					/*
					if($_SESSION['pse']['sse'] == 'S'){
						
						$sql = "select acaoid, acaodescricao, comid
								from pse.acao 
								where comid = 2 and acaoid in (9,11,12,13)
								order by 2";
						
					}
					else{
						$sql = "select acaoid, acaodescricao, comid
								from pse.acao 
								where comid = 2
								order by 2";
						
					}
					*/
					
					if(!$tipo){
						$sql = "select acaoid, acaodescricao, comid
									from pse.acao 
									where comid = 2 
									and acaoano=2013
									and acaoid in (25,26,28,29,30,31,35)
									order by 1";
						$dados = $db->carregar($sql);
					}
					else{
						$sql = "select acaoid, acaodescricao, comid
									from pse.acao 
									where comid = 2 
									and acaoano=2013
									and acaoid in (27,32,33,34)
									order by 1";
						$dados = $db->carregar($sql);
						
						echo '<tr>
							<td colspan="3" class="SubTituloCentro">
								<b>
								Ação - Promoção da Saúde Mental no território escolar
								</b>
							</td>
						</tr>';
					}
					
					foreach($dados as $d){
						
						$cor = 'black';
						/*
						//verifica monitora se ja existe
						$sql = "SELECT moeid from pse.monitoraescola
								WHERE entid = ".$entid." and acaoid = ".$d['acaoid']." and moeidentificasse = '".$_SESSION['pse']['sse']."'";
						
						$moeid = $db->pegaUm($sql);
	
						
						$dscPendencia = '';
						
						if($moeid){
							
							//verifica avaliacao 
							$sql = "select count(avcid) as total from pse.avaliacomp2 where comid = 2 and acaoid = ".$d['acaoid']." and moeid = ".$moeid;
							$totalAvaliacao = $db->pegaUm($sql);
							if(!$totalAvaliacao) $totalAvaliacao = 0;
		
							//verifica avaliacao validada 
							$sql = "select count(avcid) as total from pse.avaliacomp2 where avcvalidar='S' and comid = 2 and acaoid = ".$d['acaoid']." and moeid = ".$moeid;
							$totalAvaliacaoValidadas = $db->pegaUm($sql);
							if(!$totalAvaliacaoValidadas) $totalAvaliacaoValidadas = 0;
							
							//verifica avaliacao == avaliacao validada
							if($totalAvaliacao == $totalAvaliacaoValidadas) {
								
								if($totalAvaliacao == 0) $cor = 'black';
								else $cor = 'blue';
								
								//$cor = 'black';
								$dscPendencia = '';
							}
							else {
								$cor = 'red';
								
								if( ($totalAvaliacao - $totalAvaliacaoValidadas) == 1 ){
									$dscPendencia = "Falta ".($totalAvaliacao - $totalAvaliacaoValidadas)." avaliação que não foi validada";
								}
								else{
									$dscPendencia = "Faltam ".($totalAvaliacao - $totalAvaliacaoValidadas)." avaliações que não foram validadas";
								}
							}
							
						}
						*/
						?>
						<tr>
							<td colspan='3'>
								&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
								<b>
								<a href="pse.php?modulo=principal/monitoraComp2&acao=A&comid=<?=$d['comid']?>&acaoid=<?=$d['acaoid']?>&tipo=<?=$tipo?>">
									<font title="" color="<?=$cor?>"><?=$d['acaodescricao']?></font>
								</a>
								</b>
							</td>
						</tr>
					<?} // fecha foreach($dados as $d){
					
					if(!$tipo){
						?>
						<tr>
							<td colspan='3'>
								&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
								<b>
								<a href="pse.php?modulo=principal/monitoraComp2&acao=A&comid=2&acaoid=999&tipo=1">
									<font title="" color="<?=$cor?>">Promoção da Saúde Mental no Território Escolar</font>
								</a>
								</b>
							</td>
						</tr>
						<tr>
							<td colspan='3'>
								&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
								<b>
								<a href="pse.php?modulo=principal/monitoraComp2&acao=A&comid=2&acaoid=999&tipo=2">
									<font title="" color="<?=$cor?>">Projeto Político Pedagógico</font>
								</a>
								</b>
							</td>
						</tr>
						<?
					}
					
				} // fecha if($totalPlanoC2 == 0 && $_SESSION['pse']['sse'] == 'N'){	
				?>
				
				<?if($tipo==1){?>
					<tr>
						<td colspan='3' align="center">
							<input type="button" class="botao" name="btVoltarC1" id="btVoltarC1" value="Voltar" onclick="location.href='pse.php?modulo=principal/monitoraComp2&acao=A';">
						</td>
					</tr>
				<?}?>
			</table>
		</td>
	</tr>
<?}?>

</table>


</form>

</body>
 </html>