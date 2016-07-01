<?php

include_once APPRAIZ . 'includes/workflow.php';
	
me_verificaSessao();

$boExisteEntidade = boExisteEntidade( $_SESSION['meentid'] );
if( !$boExisteEntidade ){
	echo "<script>
			alert('A Entidade informada não existe!');
			history.back(-1);
		  </script>";
	die;
}

if( $_SESSION['memid'] != '' &&  $_REQUEST['rel_estimativa'] != '' ){
	$sql_combo = "SELECT
							ent.entid as codigo 
						FROM 
							entidade.entidade ent
						INNER JOIN
							pdeescola.memaiseducacao mme ON mme.entid = ent.entid AND mme.memanoreferencia = " . $_SESSION["exercicio"] . "
						WHERE mme.memid = ".$_SESSION['memid']."
							ORDER BY
							ent.entnome";
	$entid = $db->pegaUm( $sql_combo );
	echo $entid;
	//	echo "
//		<script>
//			window.open( 'pdeescola.php?modulo=merelatorio/meresultado_estimativa&acao=A&entid=$entid&tipo=html', 
//			'relatorio', 'width=800,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
//		</script>
//		";
	die();
}

// Flag para determinar se a tela será de cadastro ou de consulta
$somenteConsulta = true;

if(!$_SESSION["memid"]){
	echo "<script>
			alert('Não existe escola atribuida para o seu Perfil.');
			window.location.href = '/pdeescola/pdeescola.php?modulo=inicio&acao=C'; 
		  </script>";
	die;
}

$sql = "SELECT docid FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION['memid'];
$existeDocid = $db->pegaUm($sql);

if($existeDocid) {
	// Recupera ou cria o docid.
	$docid = meCriarDocumento( $_SESSION['meentid'] , $_SESSION['memid'] );
	if($docid == false) {
		echo "<script>
				alert('Esta entidade não consta na tabela principal do Mais Educação');
				history.back(-1);
			  </script>";
		die;
	}
}

$sql = "SELECT count(*) FROM seguranca.perfilusuario WHERE usucpf = '".$_SESSION['usucpf']."' AND pflcod in (".PDEESC_PERFIL_CAD_MAIS_EDUCACAO.", ".PDEESC_PERFIL_SUPER_USUARIO.", ".PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO.")";
$vPerfilCadMaisEscola = $db->pegaUm($sql);
 
if((integer)$vPerfilCadMaisEscola > 0) {
	if(!$existeDocid) {
		if((integer)$_SESSION["exercicio"] == meMaxProgramacaoExercicio()) {
			// Recupera ou cria o docid.
			$docid = meCriarDocumento( $_SESSION['meentid'] , $_SESSION['memid'] );
			if($docid == false) {
				echo "<script>
						alert('Esta entidade não consta na tabela principal do Mais Educação');
						history.back(-1);
					  </script>";
				die;
			}
		} else {
			$docid = false;
		}
	}

}


$esdid = mePegarEstadoAtual( $docid );
if((integer)$esdid == CADASTRAMENTO_ME || (integer)$esdid == CORRECAO_CADASTRAMENTO_ME) { 
	$somenteConsulta = false;
}

	
$perfis = arrayPerfil();
if(in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $perfis)||
	in_array(PDEESC_PERFIL_CAD_MAIS_EDUCACAO, $perfis)||
	in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfis)){
	$somenteConsulta = false;
}

$boSalvar = (!$somenteConsulta) ? true : false;


include APPRAIZ . "includes/cabecalho.inc";



echo "<br />";
echo montarAbasArray(carregaAbasMaisEducacao(), "/pdeescola/pdeescola.php?modulo=meprincipal/verificaPendencias&acao=A");

$titulo = "Mais Educação - Verificar pendências";
$subtitulo = "Verifique os itens pendentes abaixo:";

monta_titulo($titulo, $subtitulo);

echo cabecalhoMaisEducacao();

//retorna array de pendencias
$arPendencias = meVerificaPendencias2();


?>

<table class="tabela" align="center">
	<tr>
		<td>
		
			<table width="95%" align="center">
		
				<?php //ver($pacDocumentos, d) ?>
				<?php $x=0 ?>
				<?php foreach($arPendencias as $k => $v): ?>
					<?php
						$cor = ($x % 2) ? 'white' : '#d9d9d9;'; 
						if(  ( $k == 'Atividades 2012' ) ||
							 ( $k == 'Atividades 2013' ) ||
							 ( $k == 'Relação Escola-comunidade' ) || 
							 ( $k == 'Jovens de 15 a 17' ) ||  
							 ( $k == 'Espaços PME' ) || 
							 ( $k == 'Questionário Monitoramento Físico-Financeiro' )  
							 ): ?>
						<?php if(!$boMsg){
						?>
							<tr>
								<td colspan="3" style="text-align:center;font-size:14px;font-weight:bold;color:#900;height:50px;">
									O sistema verificou que alguns dados não foram preenchidos:
								</td>
							</tr>
							<?php $boMsg = true; ?>
						<?php }else{
							  } ?>
						<tr style="background-color: <?php echo $cor ?>;">
							<td>
								<?php 
								switch($k){
									case 'Atividades 2012':
										$link = 'pdeescola.php?modulo=meprincipal/atividades_ano_anterior&acao=A';
										break;
									case 'Atividades 2013':
										$link = 'pdeescola.php?modulo=meprincipal/atividades_ano_atual&acao=A';
										break;
									case 'Relação Escola-comunidade':
										$link = 'pdeescola.php?modulo=meprincipal/dados_programa&acao=A';
										break;
									case 'Jovens de 15 a 17':
										$link = 'pdeescola.php?modulo=meprincipal/jovens15a17&acao=A';
										break;
									case 'Espaços PME':
										$link = 'pdeescola.php?modulo=meprincipal/documentos_anexos&acao=A';
										break;
									case 'Questionário Monitoramento Físico-Financeiro':
										$link = 'pdeescola.php?modulo=meprincipal/me_questionario_fisico_financeiro&acao=A';
										break;
								}
								?>
								<a href="<?php echo $link ?>">
								<img border="0" src='/imagens/consultar.gif' onclick='javascript:void(0)'>
								</a>
							</td>
							<td>
								<b><?php echo $k ?></b>
								<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - 
								<?php echo str_replace("{valor}","R$ ".formata_valor($boPlanilhaOrcamentaria['valor']), $v) ?><br />
							</td>
							<td style="background:white;width:100px;"></td>
						</tr>
						<?php $x++ ?>
					<?php endif; ?>			
				<?php endforeach; 
					
				?>
					
				<?php if(!$boMsg): ?>
					<tr>
						<td colspan="3" style="text-align:center;font-size:14px;font-weight:bold;color:#900;height:50px;">
							O sistema não encontrou pendências de preenchimento. 
							Por favor, tramite o documento no lado direito.
						</td>
					</tr>
				<?php endif; ?>
				<tr>
					<td colspan="3" height="270px;" style="position:; marmargin-bottom: 100%; "></td>
				</tr>		
			</table>
		
		
		</td>
		<td valign="top" width="7%">
			<br><br>
			<!-- WORKFLOW -->
			
			<? 
				if($docid) {
					// Barra de estado atual e ações e Historico
					wf_desenhaBarraNavegacao( $docid , array( 'memid' => $_SESSION['memid'] ) );
				}
				if(($esdid) && ((integer)$esdid == FINALIZADO_ME)) {
					// Recupera o perfil do usuário no Mais Educação.
					$usuPerfil = arrayPerfil();
					
					if(((integer)$vPerfilCadMaisEscola > 0) || in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $usuPerfil) || in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil)) {
				?>	 
					<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
						<tr style="background-color: #c9c9c9; text-align:center;">
							<td style="font-size:7pt; text-align:center;">
								<span title="relatorio_plano_escola"><b>relatório</b></span>
							</td>
						</tr>
						<tr style="text-align:center;">
							<td style="font-size:7pt; text-align:center;">
								<span title="relatorio_plano_escola"><a href="#" alt="Ver o relatório Plano de Atendimento da Escola" title="Ver o relatório Plano de Atendimento da Escola" onclick="abreRelatorio();">Plano de Atendimento da Escola</a></span>
							</td>
						</tr>
					</table>
					
					<script type="text/javascript">
					function abreRelatorio() {
						var janela = window.open('pdeescola.php?modulo=merelatorio/plano_atendimento_escola&acao=A', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
						janela.focus();
					}
					</script>
				<? 
					}
				} 
				?>
				<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
					<tr  style="text-align:center;">
						<td>
							<a style="cursor: pointer"  onclick="abrirRelEstimativa();" >
								<img src="/imagens/print.png" border="0" alt="Versão para impressão" >  
								<br> Relatório de Estimativa
							</a>
						</td>
					</tr> 
				</table>
				
			
		</td>
	</tr>
</table>

<script>

function abrirRelEstimativa(){
	window.open("pdeescola.php?modulo=merelatorio/meresultado_estimativa&acao=A&entid=<?=$_SESSION['meentid']?>&tipo=html", "Relatorio", "height=420,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes");
} 

function popupMapa(entid){
	window.open('pdeescola.php?modulo=principal/mapaEntidade&acao=A&entid=' + entid,'Mapa','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
}
</script>
