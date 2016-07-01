<?php

if(!$_SESSION['pse']['entid']){
	?>
	<script>
		alert("Sua sessão expirou. Por favor, entre novamente!");
		location.href='pse.php?modulo=principal/listaMunicipiosSemana&acao=A';
	</script>
	<?
	exit;
}

//verifica se o municipio tem acesso
$sql = "SELECT mueid from pse.municipioexcluido WHERE  mueibge = '".$_SESSION['pse']['muncod']."'";
$mueid = $db->pegaUm($sql);
if($mueid){
	?>
	<script>
		alert("Conforme ofício encaminhado pelo Ministério da Saúde, por irregularidade de todas as equipes Saúde da Família do Município, foi anulado o processo de pactuação, assim como o Termo de Compromisso Municipal informados no Sistema de Avaliação e Monitoramento do PSE – SIMEC. \n\nDúvidas entre em contato pelo telefone (61) 3306-8091");
		location.href='pse.php?modulo=principal/listaMunicipiosSemana&acao=A';
	</script>
	<?
	exit;
}

//verifica se tem registro na tabela planoacao1
$sql = "select count(entid) as total from pse.planoacaoc1 where entid = ".$_SESSION['pse']['entid'];
$total = $db->pegaUm($sql);
if($total == 0){
	?>
	<script>
		alert("Conforme ofício encaminhado pelo Ministério da Saúde, por irregularidade de todas as equipes Saúde da Família do Município, foi anulado o processo de pactuação, assim como o Termo de Compromisso Municipal informados no Sistema de Avaliação e Monitoramento do PSE – SIMEC. \n\nDúvidas entre em contato pelo telefone (61) 3306-8091");
		location.href='pse.php?modulo=principal/listaMunicipiosSemana&acao=A';
	</script>
	<?
	exit;
}


//verifica se tem registro na tabela planoacao2
$sql = "select count(entid) as total from pse.planoacaoc2 where entid = ".$_SESSION['pse']['entid'];
$total = $db->pegaUm($sql);
if($total == 0){
	?>
	<script>
		alert("Conforme ofício encaminhado pelo Ministério da Saúde, por irregularidade de todas as equipes Saúde da Família do Município, foi anulado o processo de pactuação, assim como o Termo de Compromisso Municipal informados no Sistema de Avaliação e Monitoramento do PSE – SIMEC. \n\nDúvidas entre em contato pelo telefone (61) 3306-8091");
		location.href='pse.php?modulo=principal/listaMunicipiosSemana&acao=A';
	</script>
	<?
	exit;
}



if($_REQUEST['comid'] && $_REQUEST['acaoid']){
	$_SESSION['pse']['comid'] = $_REQUEST['comid'];
	$_SESSION['pse']['acaoid'] = $_REQUEST['acaoid'];
	
	if($_SESSION['pse']['comid'] == '1'){
		?>
			<script>
				location.href="pse.php?modulo=principal/cadastroSemanaAcao&acao=A";
			</script>
		<?
	}
	else{
		?>
			<script>
				location.href="pse.php?modulo=principal/cadastrSemanaAcao2&acao=A";
			</script>
		<?
	}
	exit;
}

unset($_SESSION['pse']['comid']);
unset($_SESSION['pse']['acaoid']);
unset($_SESSION['pse']['avlid']);
unset($_SESSION['pse']['avcid']);



$entid = $_SESSION['pse']['entid'];




// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

echo "<br>";

/*
$menu = array(0 => array("id" => 1, "descricao" => "Termo de Compromisso", 			"link" => "/pse/pse.php?modulo=principal/cadastroTermoCompromisso&acao=A"),
			  1 => array("id" => 2, "descricao" => "Monitoramento de Ações", 	"link" => "/pse/pse.php?modulo=principal/cadastroMonitoramento&acao=A"),
			  2 => array("id" => 3, "descricao" => "Semana Saúde na Escola", 	"link" => "/pse/pse.php?modulo=principal/cadastroTermoSemana&acao=A")
		  	  );
		
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
*/


$titulo = "Monitoramento das Ações da Semana Saúde na Escola";
$dsctitulo = 'Selecione uma ação abaixo.';

monta_titulo( $titulo, $dsctitulo );

Cabecalho($entid,1);


//$db->cria_aba( $abacod_tela, $url, '');

?>

<form id="formulario" name="formulario" action="" method="post">

<table  class="Tabela" align="center"  cellpadding="2" cellspacing="1">
<tr>
	<td >
		<table border="0" width="100%" >
			<tr>
				<td class="SubTituloCentro" valign="top" align="center">
					 <br>
					 <font color="red">
				     <b> 
				     	O Componente I do PSE no módulo monitoramento na escola deve ser preenchido por profissionais da Atenção Básica e validado pelo diretor da escola.
				     	<br><br> 
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
				$sql = "select acaoid, acaodescricao, comid
						from pse.acao 
						where comid = 1 and acaoid = 1";
				$dados = $db->carregar($sql);
				foreach($dados as $d){
					
					$acaoid = $d['acaoid'];
					
					//verifica monitora se ja existe
					$sql = "SELECT moeid from pse.monitoraescola
							WHERE entid = ".$entid." and acaoid = ".$acaoid;
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
									$sql = "select count(eddid) as total from pse.educandodoenca where avlid = ".$v['avlid'];
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
								<a href="pse.php?modulo=principal/cadastroMonitoramento&acao=A&comid=<?=$d['comid']?>&acaoid=<?=$acaoid?>">
									<font title="<?=$dscPendencia?>" color="<?=$cor?>"><?=$d['acaodescricao']?></font>
								</a>
							</b>
						</td>
					</tr>
				<?}?>
		</table>
	</td>
</tr>


<tr>
	<td style="background: rgb(168, 168, 168);" colspan='3'><center>
		 <span style="font-size: 12"><b>COMPONENTE II - PROMOÇÃO E PREVENÇÃO À SAÚDE</b></span>
		 </center></td>
</tr>
<tr>
	<td align="center" style="background: rgb(218, 218, 218);"  colspan='3'><span style="font-size: 12"><b>AÇÃO</b></span></td>
</tr>
<tr>
	<td align="center" style="background-color: #f5f5f5">
		<table width="90%" align="center" cellpadding="5" cellspacing="3">
			<?php 
				$sql = "select acaoid, acaodescricao, comid
						from pse.acao 
						where comid = 2 and acaoid in (9,11,12,13)";
				$dados = $db->carregar($sql);
				foreach($dados as $d){
					
					//verifica monitora se ja existe
					$sql = "SELECT moeid from pse.monitoraescola
							WHERE entid = ".$entid." and acaoid = ".$d['acaoid'];
					$moeid = $db->pegaUm($sql);

					$cor = 'black';
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
					?>
					<tr>
						<td colspan='3'>
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
							<b>
							<a href="pse.php?modulo=principal/cadastroMonitoramento&acao=A&comid=<?=$d['comid']?>&acaoid=<?=$d['acaoid']?>">
								<font title="<?=$dscPendencia?>" color="<?=$cor?>"><?=$d['acaodescricao']?></font>
							</a>
							</b>
						</td>
					</tr>
				<?}?>
		</table>
	</td>
</tr>


</table>


</form>

</body>
 </html>