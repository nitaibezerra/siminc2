<?php 
if(!$_SESSION['par']['inuid']){
	header("Location: par.php?modulo=inicio&acao=C");
	exit();
}
if(!$_SESSION['par']['itrid']){
	header("Location: par.php?modulo=inicio&acao=C");
	exit();
}

$indid = ($_GET['indid']) ? somenteNumeros( $_GET['indid'] ) : somenteNumeros( $_POST['indid'] );
$indid = !$indid ? $_SESSION['par']['indid']: $indid;
if($indid){
	$_SESSION['par']['indid'] = $indid;
}

//include_once APPRAIZ . "includes/workflow.php";
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

include_once APPRAIZ . "includes/workflow.php";

global $db;

$docid = parPegarDocidParaEntidade($_SESSION['par']['inuid']);
$estadoAtual = wf_pegarEstadoAtual( $docid );

$oPontuacaoControle = new PontuacaoControle();

$menu[0] = array("descricao" => "Plano de Trabalho", "link"=> "par.php?modulo=principal/planoTrabalho&acao=A");
$menu[1] = array("descricao" => "Pontuação" , "link"=> $_SERVER["REQUEST_URI"] );
echo montarAbasArray($menu,$_SERVER["REQUEST_URI"]);

$titulo_modulo = "Responder dados do indicador";
monta_titulo( $titulo_modulo, '&nbsp;' );

if($_GET['tipoDiagnostico'] == 'arvore'){
	$tipoDiagnostico = 'arvore';
	$checkArvore = "checked=\"checked\"";	
} else {
	$tipoDiagnostico = 'listaAgrupada';
	$checkListaAgrupada = "checked=\"checked\"";
}

if($_POST['acaoForm'] && $_POST['indid']){
	
	global $db;
	
	$arrPerfil = pegaPerfilGeral();
	if( $db->pegaUm("select itrid from par.indicador i
					inner join par.area a on a.areid = i.areid
					inner join par.dimensao d on d.dimid = a.dimid
					where i.indid = ".$_POST['indid']) == 3 ){
		
		/*$dias = verificaQuantidadeDiasObraPendencias($_SESSION['par']['itrid'], $_SESSION['par']['inuid']);
		if( $dias['qtdbloqueio'] > 0 ){
			//não pode cadastrar a obra
			echo "<script>alert('Você não pode executar essa operação pois existe a necessidade de atualização dos dados no sistema de monitoramento de obras.'); 
							window.location.href = 'par.php?modulo=principal/pontuacao&acao=A&indid={$_POST['indid']}';</script>";
			return false;
		}*/
	}
		
	$arPropostaAcao = $oPontuacaoControle->recuperarPropostaAcoesPorCriterio($_POST['crtid']);
	$arPropostaAcao = ($arPropostaAcao) ? $arPropostaAcao : array();
	
	$arPropostaSubacao = $oPontuacaoControle->recuperarSubacoesPorCriterio($_POST['crtid']);
	$arPropostaSubacao = ($arPropostaSubacao) ? $arPropostaSubacao : array();
	
	$_POST['ptoid'] = $_POST['ptoid'] ? $_POST['ptoid'] : null;
	
	$oPontuacao = new Pontuacao();
	$oPontuacao->ptoid 				 = $_POST['ptoid'];
	$oPontuacao->crtid 				 = $_POST['crtid'];
	$oPontuacao->ptodata 			 = date('Y-m-d H:i:s');
	$oPontuacao->usucpf 			 = $_SESSION['usucpf'];
	$oPontuacao->inuid 				 = $_SESSION['par']['inuid'];
	$oPontuacao->ptostatus 			 = 'A';
	if($_SESSION['par']['itrid'] == 1){
		$oPontuacao->ptodemandaestadual = $_POST['ptodemandaestadual'];
	}
	$oPontuacao->ptodemandamunicipal = $_POST['ptodemandamunicipal'];
	$oPontuacao->ptojustificativa	 = $_POST['ptojustificativa'];
	$oPontuacao->salvar();
	
	$ptoid = $_POST['ptoid'] ? $_POST['ptoid'] : $oPontuacao->ptoid;

	if($arPropostaSubacao && !$arPropostaAcao){
		$oPontuacao->rollback();
		echo "<script>
				alert('Não foi possível salvar esta pontuação, não existe ação.');
				window.location.href = 'par.php?modulo=principal/pontuacao&acao=A&indid={$_POST['indid']}';
			  </script>";
		exit;
	} else {
		
		//atualiza as outras pontuações para 'I'
		$oPontuacao2 = new Pontuacao();
		$oPontuacao2->atualizaPontuacao($ptoid, $_SESSION['par']['inuid'], $_POST['crtid']);
		
		if($_POST['crit'] == ''){
			
			//apaga as obras
			$oSubacaoobra = new SubacaoObra();
			$oSubacaoobra->deletaPorPtoid($ptoid);
			
			//apaga os itens de composição
			$oSubacaoitens = new SubacaoItensComposicao();
			$oSubacaoitens->deletaPorPtoid($ptoid);
			
			//apaga as escolas
			$oSubacaoescola = new SubacaoEscola();
			$oSubacaoescola->deletaPorPtoid($ptoid);
	
			//apaga os beneficiarios
			$oSubacaobeneficiario = new SubacaoBeneficiario();
			$oSubacaobeneficiario->deletaPorPtoid($ptoid);
	
			//apaga os detalhes da subação
			$oSubacaodetalhe = new SubacaoDetalhe();
			$oSubacaodetalhe->deletaPorPtoid($ptoid);
	
			//apaga a subação
			$oSubacao = new Subacao();
			$oSubacao->deletaPorPtoid($ptoid);
			
			//apaga a ação
			$oAcao = new Acao();
			$oAcao->deletaPorPtoid($ptoid);
		
			if($arPropostaAcao){
				$oAcao->ppaid  	  = $arPropostaAcao['ppaid'];
				$oAcao->ptoid  	  = $ptoid;
				$oAcao->acistatus = 'A';
				$oAcao->acidsc 	  = $arPropostaAcao['ppadsc'];
				$aciid 		   	  = $oAcao->salvar();
				$oAcao->commit();
			}
			
			if($aciid && $arPropostaSubacao){
				foreach ($arPropostaSubacao as $propostaSubacao) {
					if($propostaSubacao['frmid'] != 13){
						if($propostaSubacao['ppscarga'] == 't'){
							$listaInstrumento = listaSubacaoExcusivoParaEntidade($propostaSubacao['ppsid'], $_SESSION['par']['itrid'], $_SESSION['par']['inuid']);
							
							if(is_array($listaInstrumento)){
								if(in_array($_SESSION['par']['inuid'],$listaInstrumento)){
									
									$sql = "INSERT INTO workflow.documento (tpdid, esdid, docdsc, docdatainclusao)
											VALUES (62, 451, 'Em Elaboração', now()) returning docid ";
									$docid = $db->pegaUm($sql);
							
									$oSubacao->sbaid 					  = null;
									$oSubacao->sbadsc 					  = $propostaSubacao['ppsdsc'];
									$oSubacao->sbaordem 				  = $propostaSubacao['ppsordem'];
									$oSubacao->sbaobra 					  = $propostaSubacao['sbaobra'];
									$oSubacao->sbaestrategiaimplementacao = $propostaSubacao['ppsestrategiaimplementacao'];
									$oSubacao->sbaptres 				  = $propostaSubacao['ppsptres'];
									$oSubacao->sbanaturezadespesa 		  = $propostaSubacao['ppsnaturezadespesa'];
									$oSubacao->sbamonitoratecnico 		  = $propostaSubacao['ppsmonitora'];
									$oSubacao->sbastatus				  = 'A';
									$oSubacao->docid					  = $docid;
									$oSubacao->frmid 					  = $propostaSubacao['frmid'];
									$oSubacao->ptsid 					  = $propostaSubacao['ptsid'];
									$oSubacao->indid 					  = $propostaSubacao['indid'];
									$oSubacao->foaid 					  = $propostaSubacao['foaid'];
									$oSubacao->undid 					  = $propostaSubacao['undid'];
									$oSubacao->ppsid 					  = $propostaSubacao['ppsid'];
									$oSubacao->sbacronograma 			  = $propostaSubacao['ppscronograma'];
									$oSubacao->aciid 					  = $aciid;
									$oSubacao->salvar();
								}
							} else {
								
									$sql = "INSERT INTO workflow.documento (tpdid, esdid, docdsc, docdatainclusao)
											VALUES (62, 451, 'Em Elaboração', now()) returning docid ";
									$docid = $db->pegaUm($sql);
									
									$oSubacao->sbaid 					  = null;
									$oSubacao->sbadsc 					  = $propostaSubacao['ppsdsc'];
									$oSubacao->sbaordem 				  = $propostaSubacao['ppsordem'];
									$oSubacao->sbaobra 					  = $propostaSubacao['sbaobra'];
									$oSubacao->sbaestrategiaimplementacao = $propostaSubacao['ppsestrategiaimplementacao'];
									$oSubacao->sbaptres 				  = $propostaSubacao['ppsptres'];
									$oSubacao->sbanaturezadespesa 		  = $propostaSubacao['ppsnaturezadespesa'];
									$oSubacao->sbamonitoratecnico 		  = $propostaSubacao['ppsmonitora'];
									$oSubacao->sbastatus				  = 'A';
									$oSubacao->docid					  = $docid;
									$oSubacao->frmid 					  = $propostaSubacao['frmid'];
									$oSubacao->ptsid 					  = $propostaSubacao['ptsid'];
									$oSubacao->indid 					  = $propostaSubacao['indid'];
									$oSubacao->foaid 					  = $propostaSubacao['foaid'];
									$oSubacao->undid 					  = $propostaSubacao['undid'];
									$oSubacao->ppsid 					  = $propostaSubacao['ppsid'];
									$oSubacao->sbacronograma 			  = $propostaSubacao['ppscronograma'];
									$oSubacao->aciid 					  = $aciid;
									$oSubacao->salvar();
							}
						}
					}
				}
			}
		}
		
	}
	$boSucesso = $oPontuacao->commit();
	if($boSucesso){
		echo "<script>
				alert('Operação realizada com sucesso.');
				window.location.href = 'par.php?modulo=principal/pontuacao&acao=A&indid={$_POST['indid']}';
			  </script>";
		exit;
	} else {
		$oPontuacao->rollback();
		echo "<script>
				alert('Ocorreu um erro.');
				window.location.href = 'par.php?modulo=principal/pontuacao&acao=A&indid={$_POST['indid']}';
			  </script>";
		exit;
	}
	
}

if($indid && $_SESSION['par']['inuid']){
	$arDados 	 = $oPontuacaoControle->carregaPontuacao($indid);
	$arPontuacao = $oPontuacaoControle->recuperaPontuacaoPorIndid($indid);
}

$lista 			= $oPontuacaoControle->recuperaProximoAnteriorPorIndid($indid);
$indice 		= array_search( $_GET['indid'], $lista );
$anterior		= $lista[$indice-1];
$proximo  		= $lista[$indice+1];
$indAnterior 	= isset( $anterior ) ? $anterior : null;
$indProximo 	= isset( $proximo ) ? $proximo : null;


#Estrutura condicional mudada em 04/06/2012, para que seja habilitando o questionário somente para os perfis que tem essa permissão, permissão para edição.
#Habilitando ou desabilitando os botões "Salvar anterior, Salvar e Salvar próximo".  
#O valor "false" habilita o formulário.
#O valor "true" desabilita o formulário.

//Pega os perfis do usuário
$arrPerfil = pegaPerfilGeral();

//Desabilita as opções de salvar para todos os perfis que não sejam os "descritos abaixo"
if(	in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrPerfil) ||
	in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrPerfil) ||
	in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrPerfil) ||
	in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrPerfil) ||
	in_array(PAR_PERFIL_PREFEITO, $arrPerfil) ||
	in_array(PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO, $arrPerfil) ||
	in_array(PAR_PERFIL_SUPER_USUARIO, $arrPerfil) ||
	in_array(PAR_PERFIL_ADMINISTRADOR, $arrPerfil) ||
	in_array(PAR_PERFIL_ALTA_GESTAO_MEC, $arrPerfil)
){
	$boFormDesabilitado = false;
}else{
	$boFormDesabilitado = true;
}

if( $db->pegaUm("select itrid from par.indicador i
					inner join par.area a on a.areid = i.areid
					inner join par.dimensao d on d.dimid = a.dimid
					where i.indid = ".$_SESSION['par']['indid']) == 3 ){
	if( in_array(PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO, $arrPerfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrPerfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrPerfil)  || in_array(PAR_PERFIL_SUPER_USUARIO, $arrPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $arrPerfil)){
		$arqid = $db->pegaUm( "SELECT arqid FROM par.protocolo WHERE terid = 1 AND inuid = ".$_SESSION['par']['inuid'] );
		if( $arqid ){
			$bp = false;
		}else{
			$bp = true;
			/*$dias = verificaQuantidadeDiasObraPendencias($_SESSION['par']['itrid'], $_SESSION['par']['inuid']);

			if( $dias['qtdbloqueio'] > 0 ){
				//não pode cadastrar a obra
				echo "<script>alert('Existe a necessidade de atualização dos dados no sistema de monitoramento de obras.');</script>";
			}*/
		}
	}
}
?>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css"/>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript">
<!--

//jQuery.noConflict();

$.metadata.setType("attr", "validate");

// TESTE FAVOR NÃO APAGAR 
/*$(document).ready(function(){
	$('#formulario').desabilitaform();
});*/

//-->
</script>
<?php $entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod']; ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
	<tr>
		<td style="color: blue; font-size: 22px">
			<a href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore">
				<?php echo EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar, $_SESSION['par']['itrid']); ?>
			</a>
		</td>
	</tr>
</table>
<form method="post" name="formulario" id="formulario" action="par.php?modulo=principal/pontuacao&acao=A" class="formulario" >
<input type="hidden" name="acaoForm" id="acaoForm" value="1" />
<input type="hidden" name="crit" id="crit" value="" />
<input type="hidden" name="indid" value="<?php echo $indid; ?>">
<input type="hidden" name="ptoid" value="<?php echo $arPontuacao['ptoid'] ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td align="left" colspan="2"><strong>Localização do Critério</strong></td>
	</tr>
	<tr>
		<td width="200px" class="SubTituloDireita">Dimensão:</td>
		<td><?php echo $arDados[0]['codigodimensao'].'. '.$arDados[0]['descricaodimensao']; ?></td>
	<tr>
		<td class="SubTituloDireita">Área:</td>
		<td><?php echo $arDados[0]['codigoarea'].'. '.$arDados[0]['descricaoarea']; ?></td>
	<tr>
		<td class="SubTituloDireita">Indicador:</td>
		<td><?php echo $arDados[0]['codigoindicador'].'. '.$arDados[0]['descricaoindicador']; ?></td>
	</tr>
	<tr>
		<td align="left" colspan="2"><strong>Critério de Pontuação</strong></td>
	</tr>
	<tr>
		<td width="200px" class="SubTituloDireita">Critério:</td>
		<td>
			<div id="div_tabela_solicitante">
				<table class="tabela_listagem" width="800px" id="listaSolicitante">
					<tr>
						<th>Pontuação</th>
						<th>Critérios Preenchimento Obrigatório</th>
					</tr>
					<?php $i = 0; ?>
					<?php
					$disabCheck = ''; 
					if( $_SESSION['par']['inuid'] == 31 && !$bp ){
						$disabCheck = 'disabled="disabled"';
					}
					//ver($arDados);
					if($arDados[0]) :
					foreach ($arDados as $dados): ?>
						<?php $fundo = (($i % 2) == 1) ? "#f7f7f7" : "#f0f0f0"; 
							if($dados['ptoid']){
								$valor = $dados['idcriterio']; 
							}
						?>
						<tr style="background:<?php echo $fundo; ?>" onmouseover="this.style.background='#ffffcc';" onmouseout="this.style.background='<?php echo $fundo ?>';" >
							<td align="center">
								<input <?php echo ($dados['ptoid']) ? "checked=\"checked\"" : ""; ?> type="radio" id="" title="<?php echo $dados['descricaocriterio']; ?>" name="crtid" value="<?php echo $dados['idcriterio']; ?>" align="bottom" <?php echo ($i === 0) ? "validate=\"required:true\"" : ""; echo $disabCheck ?> />
							</td>
							<td>
								<?php echo $dados['pontuacao'].'. '.$dados['descricaocriterio']; ?>
							</td>
						</tr>
						<?php $i++; ?>
					<?php 
					endforeach; 
					endif; ?>
						<tr>
							<td colspan="2"><label for="crtid" class="error" >Este campo é requerido</label></td>
						</tr>
				</table>
			</div>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Justificativa:</td>
		<input type="hidden" name="criterio" id="criterio" value="<?php echo $valor; ?>">
		<td><?php
				$ptojustificativa = $arPontuacao['ptojustificativa']; 
				echo campo_textarea( 'ptojustificativa', 'S', 'S', 'Justificativa', 65 , 5, 1000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Justificativa' ); 
			?>
		</td>
	</tr>
	<tr>
		<td align="left" colspan="2"><strong>Demandas Potenciais</strong></td>
	</tr>
	<?php
		$obrig = "S"; 
		if($_SESSION['par']['itrid'] == 1){ 
			$obrig = "N";
		?>
		<tr>
			<td class="SubTituloDireita" valign="top">Redes Estaduais:</td>
			<td><?php 
					$ptodemandaestadual = $arPontuacao['ptodemandaestadual']; 
					echo campo_textarea( 'ptodemandaestadual', 'S', 'S', 'Redes estaduais', 65 , 5, 1000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Redes estaduais' );
				?>
			</td>
		</tr>
	<?php } ?>
		<tr>
			<td class="SubTituloDireita" valign="top">Redes Municipais:</td>
			<td><?php 
					$ptodemandamunicipal = $arPontuacao['ptodemandamunicipal']; 
					echo campo_textarea( 'ptodemandamunicipal', $obrig, 'S', 'Redes municipais', 65 , 5, 1000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Redes municipais' );
				?>
			</td>
		</tr>
</table>
<table  class="tabela" bgcolor="#dcdcdc" cellSpacing="1" cellPadding="3" align="center" border="0" >
	<tr>
		<td width="260px" style="text-align:right">
		<?php 
			// regra exclusiva para o Rio grande do Norte em caso de ser Brasil Pró -> (($itrid == 3) && ($uf === 'RN'))
			$itrid = $db->pegaUm("select itrid from par.indicador i
					inner join par.area a on a.areid = i.areid
					inner join par.dimensao d on d.dimid = a.dimid
					where i.indid = ".$indid);
			$uf = ( $_SESSION['par']['estuf'] ) ? $_SESSION['par']['estuf'] :  false;
		 
			if( ( (($itrid == 3) && ($uf === 'RN')) || ($estadoAtual['esdid'] == WF_ELABORACAO || $estadoAtual['esdid'] == WF_DIAGNOSTICO) ) && ( 
				in_array(PAR_PERFIL_SUPER_USUARIO, $arrPerfil)  
                || in_array(PAR_PERFIL_ADMINISTRADOR, $arrPerfil)  
				|| $bp
				|| !in_array(PAR_PERFIL_ORGAO_DE_CONTROLE,$arrPerfil))
			){ 
				if( $_SESSION['par']['inuid'] != 31 || $bp ){
					?><input type="button" value="Salvar" onclick="validaForm(<?php echo $arPontuacao['ptoid']; ?>)" /><?php
				} 
			} ?>
		</td>
		<td style="text-align:right">
			<!--<input type="hidden" name="ptoid" value="<?php echo $arPontuacao['ptoid'] ?>">
			-->
			<?php if($indAnterior): ?>
				<input type="button" onclick="window.location.href='par.php?modulo=principal/pontuacao&acao=A&indid=<?php echo $indAnterior; ?>'" value="Anterior" />
			<?php else: ?>
				<input type="button" disabled="disabled" value="Anterior" />
			<?php endif;?>
			<?php if($indProximo): ?>
				<input type="button" onclick="window.location.href='par.php?modulo=principal/pontuacao&acao=A&indid=<?php echo $indProximo; ?>'" value="Próximo" />
			<?php else: ?>
				<input type="button" disabled="disabled" value="Próximo" />
			<?php endif;?>
		</td>
	</tr>
</table>
</form>
<!-- Listagem de ações e subações.
Criada por Afonso Alves em 06/05/2011-->
<?php if( in_array(PAR_PERFIL_SUPER_USUARIO,$arrPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR,$arrPerfil) || in_array(PAR_PERFIL_MANUTENCAO_TABELAS_APOIO,$arrPerfil) || in_array(PAR_PERFIL_EQUIPE_TECNICA,$arrPerfil) ): ?>
<?php $ptoid = $arPontuacao['ptoid']; if($ptoid): ?>
	<div id="listagem"> 
		<table class="tabela_listagem" align ="center">
		<tr>
				<td align="center" colspan="2"><strong>Lista de Ações</strong></td>
		</tr>
		<?
		//Verifica se há uma resposta para Pontuação
		if ($ptoid){
		//Caso haja, a listagem é montada
		$sql = "SELECT
						'<center>
							<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"carregaAcao(\'' || a.aciid || '\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar Ação\"></a>
						</center>' as acao,
						a.acidsc, count(s.sbaid)
				   FROM
				   		par.pontuacao p 
				   INNER JOIN
				   		par.acao a ON p.ptoid = a.ptoid AND a.acistatus = 'A' 
				   LEFT JOIN
				   		par.subacao s ON  s.aciid = a.aciid AND s.sbastatus = 'A'
				   WHERE
				   	    p.ptoid =  $ptoid
				   	    AND p.ptostatus = 'A'
				   		AND  p.inuid	= ".$_SESSION['par']['inuid']."
				   GROUP BY a.aciid, a.acidsc";
		//ver($sql);
		$cabecalho = array("Ações","Descrição","Quantidade de Subações");
		$db->monta_lista($sql,$cabecalho,50,5,'N','95%');
		}?>
		</table>
	</div>
<?php endif; ?>
<?php endif; ?>

<script type="text/javascript">
jQuery(function(){
	//validação
	jQuery(".formulario").validate({
		ignoreTitle: true,
		rules: {
			ptojustificativa: "required"
		    //ptodemandamunicipal: "required";
		}
	});
	
	<?php if($boFormDesabilitado): ?>
		jQuery('[value=Salvar]').remove();
		jQuery('[type=radio]').attr("disabled", true);
		jQuery('input').attr("readonly", true);
		jQuery('select').attr("disabled", true);
		jQuery('textarea').attr("readonly", true);
		jQuery('#formulario').attr("onsubmit","return false");
		jQuery('#formulario').submit(function(){
			alert('Ação Indisponível!');
			return false;
		});
	<?php endif; ?>

});

function validaForm(ptoid)
{
	var erro = 0;
	jQuery("[class~=obrigatorio]").each(function() { 
		if(!this.value){
			erro = 1;
			alert('Favor preencher todos os campos obrigatórios!');
			this.focus();
			return false;
		}
	});

	if( erro == 0 ){
		if( ptoid ){
			criterio = jQuery('[name=criterio]').val();
			criterioNovo = jQuery('[name=crtid]:checked').val();
			if(criterio != criterioNovo){
				if( confirm("Em caso de alteração do critério, todos os dados cadastrados\nna Ação e Subação serão apagados! Deseja continuar?") ){
					jQuery('#formulario').submit();
				} else {
					return false;
				}
			} else {
				jQuery('[name=crit]').val( 'atualiza' );
				jQuery('#formulario').submit();
			}
		} else {
			jQuery('#formulario').submit();
		}
	}
}

function carregaAcao(acao)
{
	window.location.href = "par.php?modulo=principal/parAcao&acao=A&aciid=" + acao;
}
</script>
<div id="divDebug"></div>