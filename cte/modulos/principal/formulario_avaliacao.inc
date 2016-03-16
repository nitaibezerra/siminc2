<?php
/****************** DECLARAÇÃO DE VARIVEIS ******************/
cte_verificaSessao();
$itrid 				= cte_pegarItrid( $_SESSION['inuid'] );
$docid 				= cte_pegarDocid($_SESSION['inuid']);
$estado_documento 	= wf_pegarEstadoAtual($docid);
$temPerfil 			= cte_possuiPerfil(array(CTE_PERFIL_ADMINISTRADOR, CTE_PERFIL_SUPER_USUARIO));

if($_POST['excluirMonitoramento']){
	$sql = "DELETE FROM cte.monitoramentosubacao WHERE sbaid in (
			   SELECT sa.sbaid from cte.indicador i
				INNER JOIN cte.pontuacao p on p.indid = i.indid  
				INNER JOIN cte.criterio c on c.crtid = p.crtid
				INNER JOIN cte.acaoindicador ai on ai.ptoid = p.ptoid
				INNER JOIN cte.subacaoindicador sa on sa.aciid = ai.aciid
			  WHERE i.indid = {$_REQUEST['indid']} and p.inuid = {$_SESSION["inuid"]}
			)";
	$db->executar($sql);
	$db->commit();
	die;
}

if($_POST['verificaMonitoramento']){
	$arMonitoramentoSubAcao =  array();
	$pontuacao = $db->carregar( "select pto.crtid from cte.pontuacao pto where pto.ptoid = '".$_POST['objPontuacao']."'");
	if ($pontuacao[0]['crtid'] != $_POST['opcaoPontuacao']) {
		$sql = "SELECT sbaid FROM cte.monitoramentosubacao WHERE sbaid in (
			   SELECT sa.sbaid from cte.indicador i
				INNER JOIN cte.pontuacao p on p.indid = i.indid  
				INNER JOIN cte.criterio c on c.crtid = p.crtid
				INNER JOIN cte.acaoindicador ai on ai.ptoid = p.ptoid
				INNER JOIN cte.subacaoindicador sa on sa.aciid = ai.aciid
			  WHERE i.indid = {$_POST['objIndicador']} and p.inuid = {$_SESSION["inuid"]}
			)";
		$arMonitoramentoSubAcao = $db->carregar($sql);
		if(count($arMonitoramentoSubAcao) && is_array($arMonitoramentoSubAcao)){
			echo true;
			die;
		} else {
			echo false;
			die;
		}
	} else {
		echo false;
	}
	die;
}


foreach ( $_REQUEST as $chave => $valor ) {
	if ( is_string( $valor ) ) {
		$_REQUEST[$chave] = stripslashes( str_replace("'","''", trim( stripslashes( $valor ) ) ) );
	}
}

/****************** REGRAS DE VISUALIZAÇÃO DO INDICADOR ******************/
if ( !cte_podeVerIndicador( $_SESSION["inuid"] ) ) {
	header('Location: ?modulo=principal/estrutura_avaliacao&acao=A' );
	exit();
}
$habil = cte_podeEditarIndicador( $_SESSION["inuid"], $_REQUEST['indid'] ) ? "S" : "N"; 

/****************** INSERÇÃO E EDIÇÃO DE DADOS ******************/
if($_REQUEST['action'] == "addacaosub034" && cte_podeEditarIndicador($_SESSION["inuid"], $_REQUEST['indid'])){	
	$resposta = cte_insereAcoesSubacoesPontuacao034( $_REQUEST['ptoid'] );
	if($resposta){
		$db->commit();
		$db->sucesso('principal/formulario_avaliacao','&indid='.$_REQUEST['indid']);
	}else{
		alert("Não existe ações e subações cadastrada no guia para está pontuação. \n ou as ações já estão cadastradas.");
		 echo '<script> 
					alert( "Não existe ações e subações cadastrada no guia para está pontuação. \n ou as ações já foram incluídas.");
			   </script>';
		$db->rollback();
	}
}

if ($_REQUEST['action'] == "add" && cte_podeEditarIndicador($_SESSION["inuid"], $_REQUEST['indid'])) {
	$checkPontuacao   = $db->carregar("select
                                        pto.ptoid
                                       from
                                        cte.pontuacao pto
                                       inner join cte.criterio c ON c.crtid = pto.crtid
                                       where
                                        pto.inuid = '" . $_SESSION['inuid'] . "' AND
                                        pto.indid = '" . $_REQUEST['indid'] . "'");

	$checkPontuacaoId = $checkPontuacao[0]['ptoid'];

	if ($checkPontuacaoId == "") {
		$campoMunicipal = "ptodemandamunicipal,";
		$valorMunicipal =  "'" . $_REQUEST['ptomunicipal'] . "',";
		$crtid          = (integer) $_REQUEST['crtid'];

		if (trim($crtid) == 0) {
			$crtid = "null";
		}

		$ptojustificativa = $_REQUEST['ptojustificativa'];
		$sql              = "insert into cte.pontuacao (
                                 crtid,
                                 ptojustificativa,
                                 ".$campoMunicipal."
                                 ptodemandaestadual,
                                 usucpf,
                                 inuid,
                                 indid
                             ) values (
                                 $crtid,'".
                                 $ptojustificativa."',".
                                 $valorMunicipal." '".
                                 $_REQUEST['ptoestadual']."','".
                                 $_SESSION['usucpf']."','".
                                 $_SESSION['inuid']."','".
                                 $_REQUEST['indid']."'
                             ) returning ptoid";

		$ptoid = (integer) $db->pegaUm($sql);

		// atualiza plano para pequenos municipios
		cte_atualizarPlanoPequenoMunicipio( $ptoid );

		$db->commit();
		$db->sucesso('principal/formulario_avaliacao','&indid='.$_REQUEST['indid']);
	} else {
		
		echo '<script>alert("Existe pontuação para este critério!");window.history.back();</script>';
	}
}

if ($_REQUEST['action']=="edit" && $_REQUEST['ptoid']!="") {
	$campovalorMunicipal = " ptodemandamunicipal = '". $_REQUEST['ptomunicipal'] ."',";
	$crtid = (integer) $_REQUEST['crtid'];

	if ($crtid == 0 )
        $crtid = "null";

	$ptojustificativa = $_REQUEST['ptojustificativa'];

	if ($_REQUEST['ptoparecertecnico'] != "") {
		$campodscparecer = " ptoparecertecnico='". $_REQUEST['ptoparecertecnico'] ."',";
	} else {
		//$campodscparecer = "";
		$campodscparecer = " ptoparecertecnico='". $_REQUEST['ptoparecertecnico'] ."',";
	}

	$pontuacao = $db->carregar( "select pto.crtid from cte.pontuacao pto where pto.ptoid = '".$_REQUEST['ptoid']."'");

	$sql = "
    update cte.pontuacao set
        crtid              = "  . $crtid . ",
        ptojustificativa   = '" . $ptojustificativa . "', " . $campovalorMunicipal . " " . $campodscparecer . "
        ptodemandaestadual = '" . $_REQUEST['ptoestadual'] . "',
        usucpf             = '" . $_SESSION['usucpf']."',
        ptodata            = CURRENT_DATE
    where
        ptoid              = " . (integer) $_REQUEST['ptoid'];
	$db->executar($sql);

	if ($pontuacao[0]['crtid'] != $_REQUEST['opcaoPontuacao']) {
		$itrid = cte_pegarItrid( $_SESSION['inuid'] );
		$municipio = $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL;
		$grandeMunicipio = $municipio && cte_verificaGrandeMunicipio( $_SESSION['inuid'] );
		$pequenoMunicipio = $municipio && !$grandeMunicipio;
		if ( $pequenoMunicipio ) {
			cte_atualizarPlanoPequenoMunicipio( $_REQUEST['ptoid'] );
		} else {
			cte_atualizarPlanoGrandeMunicipio( $_REQUEST['ptoid'] );
		}
	}

	$db->commit();
	$db->sucesso('principal/formulario_avaliacao','&indid='.$_REQUEST['indid']);
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '&indid='. $_REQUEST['indid'] );
cte_montaTitulo( $titulo_modulo, '<img border="0" src="../imagens/obrig.gif"/> Indica Campo Obrigatório.' );

if($_REQUEST['indid']==""){
$db->sucesso('modulo=inicio','&');
}

$indicador = $db->carregar("select ind.indid,ind.ardid,ind.inddsc,ind.indstatus,ind.indcod,ind.indtipo from cte.indicador ind
where ind.indid='".$_REQUEST['indid']."'");

$indicadorTipo      = $indicador[0]['indtipo'];
$indicadorDescricao = $indicador[0]['inddsc'];
$indicadorArea 		= $indicador[0]['ardid'];

$areaDimensao  =  $db->carregar("select are.ardid,are.dimid,are.arddsc,are.ardcod,are.ardstatus 
from cte.areadimensao are where are.ardid='" . $indicadorArea . "'");

$areaDimensaoDescricao = $areaDimensao[0]['arddsc'];
$areaDimensaoId 	   = $areaDimensao[0]['dimid'];

$dimensao  =  $db->carregar("select dim.dimid,dim.itrid,dim.dimcod,dim.dimdsc,dim.dimstatus
 from cte.dimensao dim
where dim.dimid='".$areaDimensaoId."'");
$dimensaoDescricao = $dimensao[0]['dimdsc'];

$criterios      = $db->carregar("select ctr.crtid,ctr.indid,ctr.tpcid,ctr.crtdsc,ctr.ctrpontuacao,ctr.ctrord 
from cte.criterio ctr where ctr.indid='".$_REQUEST['indid']."' order by ctr.ctrpontuacao DESC");


if($criterios[0]["ctrpontuacao"] == 4){
	$semPontuacao = array_pop($criterios);
	array_unshift($criterios, $semPontuacao);
	
}
$criteriosTotal = count($criterios);

$pontuacao = $db->carregar( "
	select
		pto.ptoid,
		pto.crtid,
		pto.ptojustificativa,
		pto.ptodemandamunicipal,
		pto.ptodemandaestadual,
		pto.ptodata,
		pto.usucpf,
		pto.inuid,
		ptoparecertecnico,
		c.ctrpontuacao  
	from cte.pontuacao pto
	INNER JOIN cte.criterio c ON c.crtid = pto.crtid
	where
		pto.inuid = '".$_SESSION['inuid']."' and
		pto.indid='".$_REQUEST['indid']."' and
		pto.ptostatus = 'A'
");

$pontuacaoCriterioId  = $pontuacao[0]['crtid'];
$pontuacaoId 		  = $pontuacao[0]['ptoid'];
$ptojustificativa 	  = $pontuacao[0]['ptojustificativa'];
$ptomunicipal 		  = $pontuacao[0]['ptodemandamunicipal'];
$ptoestadual		  = $pontuacao[0]['ptodemandaestadual'];
$ptoparecertecnico	  = $pontuacao[0]['ptoparecertecnico'];

// calcula próximo e anterior

$sql = "
	select
		i.indid
	from cte.indicador i
		inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
		inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
		inner join cte.instrumento ins on ins.itrid = d.itrid
		inner join cte.instrumentounidade iu on iu.itrid = ins.itrid
	where
		iu.inuid = " . ( (integer) $_SESSION['inuid'] ) . " and
		i.indstatus = 'A'
	order by
		d.dimcod,
		ad.ardcod,
		i.indcod
";
$lista 			= $db->carregarColuna( $sql, 'indid' );
$indice 		= array_search( $indicador[0]['indid'], $lista );
$anterior 		= isset( $lista[$indice-1] ) ? $lista[$indice-1]: null;
$proximo 		= isset( $lista[$indice+1] ) ? $lista[$indice+1]: null;
$pontuacaoAtual = $pontuacao[0]["ctrpontuacao"];

?>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<?php

$alertas = array();

if ( $indicador[0]['indid'] && $estado_documento['esdid'] != CTE_ESTADO_DIAGNOSTICO ) {
	$sql = sprintf(
		"select
			i.indid,
			length(p.ptodemandaestadual) as demandaestadual, length(p.ptodemandamunicipal) as demandamunicipal,
			c.ctrpontuacao,
			aie.aciid as acaoestadual, aim.aciid as acaomunicipal,
			saie.aciid as subacaoestadual, saim.aciid as subacaomunicipal
		from cte.indicador i
		left join cte.pontuacao p on p.indid = i.indid and p.inuid = %d
		left join cte.criterio c on c.crtid = p.crtid
		left join cte.acaoindicador aie on aie.ptoid = p.ptoid and aie.acilocalizador = 'E'
		left join cte.acaoindicador aim on aim.ptoid = p.ptoid and aim.acilocalizador = 'M'
		--left join cte.acaoindicador saie on saie.ptoid = p.ptoid and saie.acilocalizador = 'E' and exists ( select sbaid from cte.subacaoindicador where aciid = saie.aciid )
		--left join cte.acaoindicador saim on saim.ptoid = p.ptoid and saim.acilocalizador = 'M' and exists ( select sbaid from cte.subacaoindicador where aciid = saim.aciid )
		left join cte.subacaoindicador saie on saie.aciid=aie.aciid
		left join cte.subacaoindicador saim on saim.aciid=aim.aciid
		where i.indid = %d",
		$_SESSION['inuid'],
		$indicador[0]['indid']
	);
	$registro = $db->pegaLinha( $sql );
	$alertas = array();
	
	if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) {
		if( $estado_documento['esdid'] != CTE_ESTADO_DIAGNOSTICO ) {
			if ( !$registro['subacaoestadual'] && in_array( $registro['ctrpontuacao'], array(1,2) ) ) {
				array_push( $alertas, "A pontuação está baixa." );
			}
			if ( !$registro['subacaoestadual'] && $registro['demandaestadual'] > 0 ) {
				array_push( $alertas, "Há demanda estadual." );
			}
			if ( !$registro['subacaomunicipal'] && $registro['demandamunicipal'] > 0 ) {
				array_push( $alertas, "Há demanda municipal." );
			}
		}
	}
	
	if ( $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ) {
		if( $estado_documento['esdid'] != CTE_ESTADO_DIAGNOSTICO ) {
			if ( !$registro['subacaomunicipal'] && in_array( $registro['ctrpontuacao'], array(1,2) ) ) {
				array_push( $alertas, "A pontuação está baixa." );
			}
		}
	}
	
	if ( count( $alertas ) > 0 ) {
		array_push( $alertas, "É necessário cadastrar um plano de ação." );
	}
	
}


?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<?php if( !empty( $alertas ) ): ?>
	<table cellspacing="1" cellpadding="3" border="0" align="center" style="border-bottom: 0pt none ! important;" class="tabela">
		<colgroup>
			<col/>
		</colgroup>
		<tbody>
			<tr>
				<td style="padding: 15px; background-color: rgb(250, 250, 250); color: red; text-align: right;">
					<b>Alerta:</b> <?php foreach( $alertas as $alerta ): ?>
						<?= $alerta ?> 
					<?php endforeach; ?>
				</td>
			</tr>
		</tbody>
	</table>
<?php endif; ?>
<form action="" method="post" name="formulario">
<!-- div id="divTeste">aqui</div-->
<input type="hidden" id="crtid" name="crtid" value="">
<input type="hidden" id="opcaoPontuacao_old" name="opcaoPontuacao_old" value="<? echo $pontuacaoCriterioId; ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">Dimensão:</td>
		<td><?= $dimensao[0]['dimcod'] ?>. <?=$dimensaoDescricao; ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Área:</td>
		<td><?= $areaDimensao[0]['ardcod'] ?>. <?=$areaDimensaoDescricao; ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Indicador:</td>
		<td><?= $indicador[0]['indcod'] ?>. <?=$indicadorDescricao; ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita"><input type="hidden" name="inuid" value="<?=$_SESSION['inuid'] ?>">Critérios:</td>
		
		<td>
		
		
			
			<!-- Critérios do indicador -->
			<?php
			
			$itrid = cte_pegarItrid( $_SESSION['inuid'] );
			$municipio = $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL;
			$grandeMunicipio = $municipio && cte_verificaGrandeMunicipio( $_SESSION['inuid'] );
			$pequenoMunicipio = $municipio && !$grandeMunicipio;
			if ( $pequenoMunicipio )
			{
				$habil_criterio = $habil == "S";
			}
			else
			{
				$habil_criterio = $habil == "S"; 
				//$habil == "S" && !cte_possuiAcao( $_SESSION['inuid'], $_REQUEST['indid'] );
			}
			
			?>	
			
			<input type="hidden" id="sitmunicipio" name="sitmunicipio" value="<? echo ($pequenoMunicipio)?'pequenomun':'grandemun'; ?>">
			
			<? if( $criteriosTotal > 1 ) : ?>
				<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<thead>
						<tr>
							<td><b>Pontuação</b></td>
							<td><b>Critérios (Preenchimento Obrigatório)</b></td>
						</tr>
					</thead>
					<? 
					for($i= 0; $i < $criteriosTotal; $i++) : 
					?>
						<?  $marcado = fmod( $i, 2 ) == 0 ? "#e7e7e7" : "#f7f7f7"; ?>
						<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
							<td>
								<input
									id="criterio_<?= $criterios[$i]['crtid'] ?>"
									type="radio"
									<?php if ( $habil_criterio ): ?>
										name="opcaoPontuacao"
										value="<?= $criterios[$i]['crtid'] ?>"
										onclick="marcarAlteracao();"
									<?php else: ?>
										name="opcaoPontuacao_fake"
										value=""
										disabled="disable"
									<?php endif; ?>
									<?php if( $pontuacaoCriterioId == $criterios[$i]['crtid'] ) : ?>
										checked="checked"
									<?php endif; ?>
								/>
							</td>
							<td>
								<!-- <label for="criterio_<?=$criterios[$i]['crtid'] ?>">  -->
									<b><?= $criterios[$i]['ctrord'] > 0 ? $criterios[$i]['ctrpontuacao'].". " : "" ?></b>
									<?=$criterios[$i]['crtdsc'] ?>
								<!-- </label>  -->
							</td>	
						</tr>
					<? endfor; ?>
				</table>
				<? if ( !$habil_criterio ) : ?>
					<?php
					$sql = "
						select
							crtid
						from cte.pontuacao
						where
							ptostatus = 'A' and
							inuid = " . ( (integer) $_SESSION['inuid'] ) . " and
							indid = " . ( (integer) $_REQUEST['indid'] ) .  "
					";
					$_crtid = (integer) $db->pegaUm( $sql );
					?>
					<input type="hidden" name="opcaoPontuacao" value="<?= $_crtid ?>"/>
				<? endif; ?>
			<? else: ?>
				&nbsp;Não há critérios definidos
			<? endif; ?>
			<!-- Critérios do indicador -->	
			
			<?php if( 	($pontuacaoAtual == 3 || $pontuacaoAtual == 4 || $pontuacaoAtual == 0)  
						&& ($estado_documento["esdid"] == CTE_ESTADO_ANALISE || $estado_documento["esdid"] == CTE_ESTADO_ANALISE_FIN  ) 
						&& ($temPerfil) ){ ?>
	<tr>
		<td class="SubTituloDireita">Proposta:</td>
		<td>
		<a style="cursor:pointer;" onclick="propostaAcoesOutrasPontuacoes(<?=$_REQUEST['indid']; ?>, <?=$pontuacaoId; ?>);">Proposta de ações para a pontuação <?php echo $pontuacaoAtual; ?></a>
		</td>
	</tr>
	<?php } ?>
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita">Justificativa:</td>
		<td>
			<?=campo_textarea('ptojustificativa',true,$habil,'',100,5,500,' onkeyup="marcarAlteracao();" ' ); ?>
		</td>
	</tr>
    <!-- Parecer Tecnico -->
    <!--
 	<? //if( cte_podeVerParecer( $_SESSION['inuid'] ) == true || $ptoparecertecnico != "" ) { ?>
	<? //$habilita = cte_podeEditarParecer( $_SESSION['inuid'] ) ? "S" : "N"; ?>
 	 
 	<tr>
		<td class="SubTituloDireita">Parecer Técnico:</td>
		<td>
			<?//=campo_textarea('ptoparecertecnico','N',$habilita,'',100,5,500,' onkeyup="marcarAlteracao();" ' ); ?>
		</td>
	</tr>
 	<? //} ?>
 	 -->
 	<!-- Parecer Tecnico -->
 	
 	<!-- Demanda Potencial -->
 	<tr>
		<td class="SubTituloEsquerda" align="right" colspan="2"><b>Demandas&nbsp;potenciais:</b></td>
	</tr>
	<?php if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ): ?>
		<tr>
			<td class="SubTituloDireita">Rede&nbsp;estadual:</td>
			<td>
			<?if ($editajustificativa=="" ) {?>
				<?=campo_textarea('ptoestadual','N',$habil,'',100,3,500,' onkeyup="marcarAlteracao();" '); ?>
			<? }?>
			</td>
		</tr>
	<?php endif; ?>
	
	<? if( $indicadorTipo == "0"  || $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ){ ?>
	<tr>
		<td class="SubTituloDireita" align="right" valign="top">Redes&nbsp;municipais:</td>
		<td>
		
	<? if ( $editajustificativa == "" ) {?>
			<?=campo_textarea('ptomunicipal','N',$habil,'',100,3,500,' onkeyup="marcarAlteracao();" '); ?>
	<?	}?>

		</td>
	</tr>
	
	<? } ?>
	<!-- Demanda Potencial -->
	
	<tr bgcolor="#C0C0C0">
			<td ></td>
			<td>
			<div style="float: left;">
			<?if ($habil=='S'){ ?>
			<? if($pontuacaoId <= 0){ ?>
			<input type='button' class="botao" name='cadastra' value='Salvar' onclick="salvarCriterio(<?=$_REQUEST['indid']; ?>,this);"/>
			<? }else{ ?>
			<input type='button' class="botao" name='editar' value='Salvar' onclick="editarCriterio(<?=$_REQUEST['indid']; ?>,<?=$pontuacaoId; ?>,this);"/>
			<? } ?>
			<?} ?>
			</div>
			<div style="float: right;">
				<input  <?= !$anterior ? 'disabled="disabled"' : '' ?> type='button' class="botao" name='anterior' value='Anterior' onclick="indicadorAnterior();" title="Indicador anterior"/>
				<input <?= !$proximo ? 'disabled="disabled"' : '' ?> type='button' class="botao" name='proximo' value='Próximo' onclick="proximoIndicador();" title="Próximo indicador"/>
			</div>
			</td>
		</tr>
	
	</table>
</form>

<br>
<?php if($estado_documento['esdid'] != CTE_ESTADO_DIAGNOSTICO) {?>
	<?php
		$podeManipularAcao =
			$habil == 'S' &&
			(
				!cte_pegarMuncod( $_SESSION['inuid'] ) ||
				cte_verificaGrandeMunicipio( $_SESSION['inuid'] )
			);
	?>
	<!--  Lista de Ações Cadastradas -->
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		
		<tbody>
			<tr>
				<td class="SubtituloEsquerda">Lista de Ações Cadastradas Neste Indicador</td>
			</tr>
		</tbody>
	</table>
	<?php
	
	$cabecalho = array( "Ação", "Nome da Ação", "Demanda", "Data Inicial", "Data Final","Qtd. Subações");
	if ( $podeManipularAcao )
	{
		$sql = " SELECT
			'<a href=\"javascript:editar_par('|| a.aciid ||', \' ' || a.acilocalizador || ' \' );\"><img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\"></a>'".
			( $habil == 'S' ? " || '&nbsp;<a href=\"javascript:excluir_par('|| a.aciid ||');\"><img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\"></a>'" : "" )
			."
				as acao,
				a.acidsc, 
				case when a.acilocalizador='E' then 'Estadual' else 'Municipal' end as demanda,
				to_char(a.acidtinicial,'dd/mm/YYYY') as datainicial, 
				to_char(a.acidtfinal,'dd/mm/YYYY') as datafinal,
				(select count(*) from cte.subacaoindicador where subacaoindicador.aciid=a.aciid) as qtd
				from cte.acaoindicador a where a.ptoid = '". ( (integer) $pontuacaoId ) . "'
			";
	} else {
		$sql = " SELECT
			'<a href=\"javascript:editar_par('|| a.aciid ||', \' ' || a.acilocalizador || ' \' );\"><img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\"></a>'
				as acao,
				a.acidsc, 
				case when a.acilocalizador='E' then 'Estadual' else 'Municipal' end as demanda,
				to_char(a.acidtinicial,'dd/mm/YYYY') as datainicial, 
				to_char(a.acidtfinal,'dd/mm/YYYY') as datafinal,
				(select count(*) from cte.subacaoindicador where subacaoindicador.aciid=a.aciid) as qtd
				from cte.acaoindicador a where a.ptoid = '". ( (integer) $pontuacaoId ) . "'
			";
		
	}
	$db->monta_lista( $sql, $cabecalho, 100, 20, '', '' ,'' );
}
?>

<script language="javascript" type="text/javascript">
function propostaAcoesOutrasPontuacoes(objindicador, objPontuacao){
	if(confirm('Deseja adicionar ações e subaçãoes para está pontuação?')){
		document.formulario.action = "../cte/cte.php?modulo=principal/formulario_avaliacao&acao=A&ptoid="+objPontuacao+"&indid="+objindicador+"&action=addacaosub034";
		document.formulario.submit();
	}
}


function editar_par(id,localizador){

	window.location = "?modulo=principal/par_acao&acao=A&aciid="+id+"&ptoid=<?=$pontuacaoId?>&acilocalizador="+trim(localizador);

}

function excluir_par(id){
	if ( confirm( "Deseja excluir a ação?" ) ) {
		window.location = "?modulo=principal/par_acao&acao=A&aciid="+id+"&action=remov&origem=<?= urlencode( $_SERVER['QUERY_STRING'] ) ?>";
	}
}


function pegaRadio(objRadio){
	if ( objRadio.value )
	{
		return objRadio.value;
	}
	if ( objRadio.length )
	{
		for ( var i = 0 ; i < objRadio.length ; i++ ){
			if (objRadio[i].checked) {
				
				return objRadio[ i ].value;
				break ;
			}
		}
	}
	return objRadio.value;
	//return false;
}

function pegaIndiceRadio(objRadio){
	if ( objRadio.value )
	{
		return objRadio.value;
	}
	if ( objRadio.length )
	{
		for ( var i = 0 ; i < objRadio.length ; i++ ){
			if (objRadio[i].checked) {
				
				return i;
				break ;
			}
		}
	}
	return 0;
}



function editarCriterio(indicador,pontuacao,botao)
{

	var objRadio  	            = "";
	var valorRadio 			    = "";
	var objCampoJustificativa   = "";
	var objCampoEstadual        = "";
	var objCampoMunicipal	    = "";
	var objValida				= "true";
	var objindicador			= indicador;
	var objPontuacao			= pontuacao;
	
	
	valorRadio_old		  = document.formulario.opcaoPontuacao_old.value;
	objRadio              = document.formulario.opcaoPontuacao;
	objCampoJustificativa = document.formulario.ptojustificativa.value;
	if( objCampoJustificativa =="" )
	{
		alert('Preenchimento Obrigatório da Justificativa!');
		    objValida = 'false';
	}
	
	valorRadio = pegaRadio( objRadio );
	if( valorRadio == false ) {
		    alert( 'Selecione um critério!' );
		    objValida = 'false';
	}




	// Definindo regras - PEQUENO MUNICIPIO e ESTADO
	switch(document.formulario.sitmunicipio.value) {
		case 'pequenomun':
			if(valorRadio) {
				if(valorRadio_old != valorRadio) {
					if(!confirm('ATENÇÃO! A alteração do critério irá redefinir o preenchimento das ações e subações de acordo com o GUIA. Os dados preenchidos serão perdidos. Confirma a alteração?')) {
						return false;
					}
				}
			}	
			break;
		case 'grandemun':
			if(valorRadio) {
				if(valorRadio_old != valorRadio) {
				if( pegaIndiceRadio( objRadio ) < 3 ) {
					if(!confirm('ATENÇÃO! A alteração do critério irá deletar as ações e subações cadastradas no indicador. Confirma a alteração?')) {
						return false;
					}
				}
				}
			}	
			break;
	}

	if(!verificaMonitoramento(objPontuacao,objindicador)){
		return false;
	}

	//verificaMonitoramento(objPontuacao,objindicador);

	document.formulario.crtid.value = valorRadio;
	
	if( objValida=="true" )
	{
		botao.disabled = true; 
		document.formulario.action = "../cte/cte.php?modulo=principal/formulario_avaliacao&acao=A&ptoid="+objPontuacao+"&indid="+objindicador+"&action=edit";
		document.formulario.submit();
	}
	
}

function salvarCriterio(indicador,botao)
{

	var objRadio  	            = "";
	var valorRadio 			    = "";
	var objCampoJustificativa   = "";
	var objCampoEstadual        = "";
	var objCampoMunicipal	    = "";
	var objValida				= "false";
	var objindicador			= indicador;
	
		objRadio      			=  document.formulario.opcaoPontuacao;
		objCampoJustificativa   =  document.formulario.ptojustificativa.value;
		
	
		valorRadio = pegaRadio(objRadio);
	
		if(valorRadio==false){
		    alert('Selecione um critério!');
			}else if(objCampoJustificativa ==""){
				  alert('Preenchimento Obrigatório da Justificativa!');
			      }else{
				      //setando o campo hidden de crtid com o valor selecionado do radio
				      document.formulario.crtid.value = valorRadio;
				      objValida = "true";
			      }
	if(objValida=="true"){
	botao.disabled=true;
	document.formulario.action="../cte/cte.php?modulo=principal/formulario_avaliacao&acao=A&indid="+objindicador+"&action=add";
	document.formulario.submit();
	
	}		
}

var houveModificacao = false;

function marcarAlteracao(){
	houveModificacao = true;
}

function verificarAlteracao(){
	return !houveModificacao || confirm( 'Atenção! Caso confirme a ação as alterações não gravadas serão perdidas.' );
}

function indicadorAnterior(){
	if( !verificarAlteracao() ){
		return;
	}
	window.location = "?modulo=principal/formulario_avaliacao&acao=A&indid=<?= $anterior ?>";
}

function proximoIndicador(){
	if( !verificarAlteracao() ){
		return;
	}
	window.location = "?modulo=principal/formulario_avaliacao&acao=A&indid=<?= $proximo ?>";
} 

function verificaMonitoramento(objPontuacao,objIndicador){
	objRadio = document.formulario.opcaoPontuacao;
	valorRadio = pegaRadio(objRadio);
	var parametros = "&opcaoPontuacao="+valorRadio+"&objPontuacao="+objPontuacao+"&objIndicador="+objIndicador ;  

	var retorno = true;
	
	new Ajax.Request(window.location.href, {
        method: 'post',
        asynchronous: false,
        parameters: 'verificaMonitoramento=true'+parametros,
        onComplete: function(res)
        {
        	if(res.responseText){
        		if(confirm('Existe subações em monitoramento. Deseja exclui-las?')){
            		if(excluirMonitoramento(parametros)){
            			retorno = true;
                	}
                } else {
                	retorno = false;
                }
            }
        }
	});
	return retorno;
}

function excluirMonitoramento(parametros){
	var retorno = true;
	
	new Ajax.Request(window.location.href, {
        method: 'post',
        asynchronous: false,
        parameters: 'excluirMonitoramento=true'+parametros,
        onComplete: function(res)
        {
        	retorno = true;
        }
	});
	return retorno;
}
</script>