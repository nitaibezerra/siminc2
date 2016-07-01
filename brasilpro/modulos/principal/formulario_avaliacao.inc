<?php

cte_verificaSessao();
$itrid = cte_pegarItrid( $_SESSION['inuid'] );

foreach ( $_REQUEST as $chave => $valor ) {
	if ( is_string( $valor ) ) {
		$_REQUEST[$chave] = stripslashes( str_replace("'","''", trim( stripslashes( $valor ) ) ) );
	}
}


$estado_documento = wf_pegarEstadoAtual( $_SESSION['docid'] );

if ( !cte_podeVerIndicador( $_SESSION["inuid"] ) ) 
{
	header('Location: ?modulo=principal/estrutura_avaliacao&acao=A' );
	exit();
}
$habil = cte_podeEditarIndicador( $_SESSION["inuid"], $_REQUEST['indid'] ) ? "S" : "N"; 

if( $_REQUEST['action']=="add" && cte_podeEditarIndicador( $_SESSION["inuid"], $_REQUEST['indid'] ) )
{
	
	$checkPontuacao = $db->carregar(" select pto.ptoid
	from cte.pontuacao pto
	where  pto.inuid = '".$_SESSION['inuid']."' and pto.indid='".$_REQUEST['indid']."'");	
	$checkPontuacaoId = $checkPontuacao[0]['ptoid'];
	
	if($checkPontuacaoId==""){	
		$campoMunicipal = "ptodemandamunicipal,";
		$valoMunicipal  =  "'" . str_replace( "'", "\\'", $_REQUEST['ptomunicipal'] ) . "',";
		$crtid = (integer) $_REQUEST['crtid'];
		if ( $crtid == 0 )
		{
			$crtid = "null";
		}
		
		$ptojustificativa = addslashes($_REQUEST['ptojustificativa']);
		
		$sql= " insert into cte.pontuacao (crtid,ptojustificativa,".$campoMunicipal."ptodemandaestadual,usucpf,inuid,indid) values($crtid,'".$ptojustificativa."',".$valoMunicipal." '".$_REQUEST['ptoestadual']."','".$_SESSION['usucpf']."','".$_SESSION['inuid']."','".$_REQUEST['indid']."') returning ptoid";
		$ptoid = (integer) $db->executar($sql);
			
		// atualiza plano para pequenos municipios
		cte_atualizarPlanoPequenoMunicipio( $ptoid );
		
		$db->commit();
		$db->sucesso('principal/formulario_avaliacao','&indid='.$_REQUEST['indid']);
	
	}else{
		
		echo '<script>alert("Existe Pontuação, \n para este Critério !");window.history.back();</script>';
	}
		
}
if($_REQUEST['action']=="edit" && $_REQUEST['ptoid']!=""){
	
	//dump($_REQUEST['ptojustificativa']).die;
	
	$campovalorMunicipal = " ptodemandamunicipal='". str_replace( "'", "\\'", $_REQUEST['ptomunicipal'] ) ."',";
	$crtid = $_REQUEST['crtid'] != "" ? (integer) $_REQUEST['crtid'] : "null";
	//dump($crtid).die;
	$ptojustificativa = addslashes($_REQUEST['ptojustificativa']);
	
	if($_REQUEST['ptoparecertecnico']!=""){
		$campodscparecer = " ptoparecertecnico='". str_replace( "'", "\\'", $_REQUEST['ptoparecertecnico'] ) ."',";
	}else{
		$campodscparecer = "";
	}
		
	$sql= " update cte.pontuacao set crtid=".$crtid.",ptojustificativa='".$ptojustificativa."',".$campovalorMunicipal." ".$campodscparecer." ptodemandaestadual='".$_REQUEST['ptoestadual']."',usucpf='".$_SESSION['usucpf']."',ptodata='".date('Y-m-d H:i:s')."' where ptoid='".$_REQUEST['ptoid']."' ";

	//dump($sql).die;
	
	$db->executar($sql);
	
	
	// atualiza plano para pequenos municipios
	cte_atualizarPlanoPequenoMunicipio( $_REQUEST['ptoid'] );
	
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
		ptoparecertecnico 
	from cte.pontuacao pto
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
$lista = $db->carregarColuna( $sql, 'indid' );
$indice = array_search( $indicador[0]['indid'], $lista );
$anterior = isset( $lista[$indice-1] ) ? $lista[$indice-1]: null;
$proximo = isset( $lista[$indice+1] ) ? $lista[$indice+1]: null;

?>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<?php

$alertas = array();
$estado_documento = wf_pegarEstadoAtual( $_SESSION['docid'] );

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
<input type="hidden" id="crtid" name="crtid" value="">
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
				$habil_criterio =
					$habil == "S";
			}
			else
			{
				$habil_criterio =
					$habil == "S" &&
					!cte_possuiAcao( $_SESSION['inuid'], $_REQUEST['indid'] );
			}
			
			?>	
			<? if( $criteriosTotal > 1 ) : ?>
				<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<thead>
						<tr>
							<td><b>Pontuação</b></td>
							<td><b>Critérios (Preenchimento Obrigatório)</b></td>
						</tr>
					</thead>
					<? for($i= 0; $i < $criteriosTotal; $i++) : ?>
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
                                    onclick="checarPontuacao(this, <?php echo $criterios[$i]['ctrpontuacao']; ?>)"
                                    onchange="checarPontuacao(this, <?php echo $criterios[$i]['ctrpontuacao']; ?>)"
								/>
							</td>
							<td>
								<label for="criterio_<?=$criterios[$i]['crtid'] ?>">
									<b><?= $criterios[$i]['ctrord'] > 0 ? $criterios[$i]['ctrpontuacao'].". " : "" ?></b>
									<?=$criterios[$i]['crtdsc'] ?>
								</label>
							</td>	
						</tr>
<?php
$formAtivo = false;

if ($pontuacaoCriterioId == $criterios[$i]['crtid'] && !$formAtivo) {
    echo '<script  type="text/javascript">var formAtivo = ' , ($criterios[$i]['ctrpontuacao'] == 0 ? 'true' : 'false') , ';</script>';
    $formAtivo = true;
} else {
    if (!$formAtivo) {
        echo '<script  type="text/javascript">var formAtivo = true;</script>';
        $formAtivo = true;
    }
}

?>
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
					$ctrid = (integer) $db->pegaUm( $sql );
					?>
					<input type="hidden" name="opcaoPontuacao" value="<?= $ctrid ?>"/>
				<? endif; ?>
			<? else: ?>
				&nbsp;Não há critérios definidos
			<? endif; ?>
			<!-- Critérios do indicador -->	
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
		</td>
	</tr>

	<tr>
		<td class="SubTituloDireita">Justificativa:</td>
		<td>
			<?=campo_textarea('ptojustificativa',true,$habil,'',100,5,1200,' onkeyup="marcarAlteracao();" ' ); ?>
		</td>
	</tr>
    <!-- Parecer Tecnico -->
    
 	<? if( cte_podeVerParecer( $_SESSION['inuid'] ) == true || $ptoparecertecnico != "" ) { ?>
	<? $habilita = cte_podeEditarParecer( $_SESSION['inuid'] ) ? "S" : "N"; ?>
 	<tr>
		<td class="SubTituloDireita">Parecer Técnico:</td>
		<td>
			<?=campo_textarea('ptoparecertecnico','N',$habilita,'',100,5,500,' onkeyup="marcarAlteracao();" ' ); ?>
		</td>
	</tr>
 	<? } ?>
 	
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

<?if($estado_documento['esdid'] != CTE_ESTADO_DIAGNOSTICO) {?>
	<?
		$podeManipularAcao =
			$habil == 'S'
			//&& (
			//	!cte_pegarMuncod( $_SESSION['inuid'] ) ||
			//	cte_verificaGrandeMunicipio( $_SESSION['inuid'] )
			//);
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
	for ( var i = 0 ; i < objRadio.length ; i++ ){
		if (objRadio[i].checked) {
			
			return objRadio[ i ].value;
			break ;
		}
	}
	return false;
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
	
	
	
	objRadio              = document.formulario.opcaoPontuacao;
	objCampoJustificativa = document.formulario.ptojustificativa.value;
	
	
	//if( objCampoJustificativa =="")
	if( objCampoJustificativa =="" && document.formulario.ptojustificativa.disabled == false)	
	{
		alert('Preenchimento Obrigatório da Justificativa!');
		    objValida = 'false';
	}
	
		valorRadio = pegaRadio( objRadio );
		
		if( valorRadio == false )
		{
		    alert( 'Selecione um critério!' );
		    objValida = 'false';
		}else{
			//setando o campo hidden de crtid com o valor selecionado do radio
			document.formulario.crtid.value = valorRadio;
		}
	
	if(objValida=="true"){
	botao.disabled=true; 
	document.formulario.action="../brasilpro/brasilpro.php?modulo=principal/formulario_avaliacao&acao=A&ptoid="+objPontuacao+"&indid="+objindicador+"&action=edit";
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
			}else if(objCampoJustificativa =="" && document.formulario.ptojustificativa.disabled == false){
				  alert('Preenchimento Obrigatório da Justificativa!');
			      }else{
			      //setando o campo hidden de crtid com o valor selecionado do radio
			      document.formulario.crtid.value = valorRadio;
			      
			      objValida = "true";
			      
			      }
	if(objValida=="true"){
	botao.disabled=true;
	document.formulario.action="../brasilpro/brasilpro.php?modulo=principal/formulario_avaliacao&acao=A&indid="+objindicador+"&action=add";
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

function desativarForm()
{
    var ptojustificativa  = document.getElementById('ptojustificativa');
    var ptoparecertecnico = document.getElementById('ptoparecertecnico');
    var ptoestadual       = document.getElementById('ptoestadual');
    var ptomunicipal      = document.getElementById('ptomunicipal');

    /*if (ptojustificativa) {
        ptojustificativa.setAttribute('disabled', 'disabled');
        ptojustificativa.value = '';
    }*/

    if (ptoparecertecnico) {
        ptoparecertecnico.setAttribute('disabled', 'disabled');
    }

    if (ptoestadual) {
        ptoestadual.setAttribute('disabled', 'disabled');
        ptoestadual.value = '';
    }

    if (ptomunicipal) {
        ptomunicipal.setAttribute('disabled', 'disabled');
    }
}

function ativarForm()
{
    var ptojustificativa  = document.getElementById('ptojustificativa');
    var ptoparecertecnico = document.getElementById('ptoparecertecnico');
    var ptoestadual       = document.getElementById('ptoestadual');
    var ptomunicipal      = document.getElementById('ptomunicipal');

    if (ptojustificativa) {
        ptojustificativa.removeAttribute('disabled');
    }

    if (ptoparecertecnico) {
        ptoparecertecnico.removeAttribute('disabled');
    }

    if (ptoestadual) {
        ptoestadual.removeAttribute('disabled');
    }

    if (ptomunicipal) {
        ptomunicipal.removeAttribute('disabled');
    }
}

function checarPontuacao(e, ctrpontuacao)
{
    if (ctrpontuacao == '0') {
        desativarForm();
    } else {
        ativarForm();
    }    
}

if (document.getElementById('criterio_4947') != null && document.getElementById('criterio_4947').checked)
	document.getElementById('ptoestadual').setAttribute('disabled', 'disabled');

/*
if (typeof(formAtivo) != 'undefined') {
    if (formAtivo) {
        ativarForm();
    } else {
        desativarForm();
    }
}
*/
</script>