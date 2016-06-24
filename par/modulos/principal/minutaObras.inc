<?php

/*if($_REQUEST['alterarminuta']){
	header('content-type: text/html; charset=ISO-8859-1');
	alterarDadosMinuta($_POST);
	exit;
}*/

if( $_POST['requisicao'] == 'email' ){
	if( !empty($_GET['dopid']) ){
		$arConvenio = $db->pegaLinha( "SELECT empanoconvenio, empnumeroconvenio FROM par.empenho e
  										inner join par.processopar p on p.prpnumeroprocesso = e.empnumeroprocesso WHERE p.prpid = ".$_GET['prpid']." and empstatus = 'A'" );
		
		$strAssunto = 'Publicação - SIMEC Convênio '.$arConvenio['empnumeroconvenio'].'/'.$arConvenio['empanoconvenio'];		
				
		$strMensagem = pegaTermoCompromissoArquivo( $_REQUEST['dopid'], '' );
		
		$remetente = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
		$strMensagem = html_entity_decode($strMensagem);
		if( $_SERVER['HTTP_HOST'] == "simec-local" || $_SERVER['HTTP_HOST'] == "localhost" ){
			$retorno = true;
		} elseif($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br"){
			$strEmailTo = array($_SESSION['email_sistema']);
			$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);		
		} else {
			$strEmailTo = 'dicom_publicacao@fnde.gov.br';
			$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
		}
		if( $retorno ){
			echo "<script>alert('E-mail enviado com sucesso!');
						window.location.href = window.location;
				  </script>";
		} else {
			echo "<script>alert('Não foi possível enviar o e-mail!');
					window.location.href = window.location;
			  </script>";
		}
	} else {
		echo "<script>alert('Não foi possível enviar o e-mail, é necessário cadastrar uma publicação!');
					window.location.href = window.location;
			  </script>";
	}
	exit();
}

if($_GET['imprimeWord']){
	header("Content-type: application/vnd.ms-word");
	header("Content-type: application/force-download");
	header("Content-Disposition: attachment; filename=MinutaConvenioDou.doc");
	header("Pragma: no-cache");
	
	$doptexto = pegaTermoCompromissoArquivo( $_REQUEST['dopid'], '' );
	
    echo "<html>".html_entity_decode($doptexto)."</html>";
	exit;
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

if($_REQUEST['pegaDataFinal']){
	header('content-type: text/html; charset=ISO-8859-1');
	if( !empty($_POST['dopdiasvigencia']) )
		getDataFinalConvenio($_POST['dopdiasvigencia'], $_POST['dopdatainicio']);
	else{
		$dopdatainicio = formata_data_sql($_POST['dopdatainicio']);
		$dopdatafim = formata_data_sql($_POST['dopdatafim']);
		$dias = dateDiff1(strtotime($dopdatainicio),strtotime($dopdatafim));
 		echo $dias;
	}		
	exit;
}

function getDataFinalConvenio($dias, $data){
	$arData = explode('/', $data);

	$dia = $arData[0] - 1;
	$mes = $arData[1];
	$ano = $arData[2];
	$dataFinal = mktime(24*$dias, 0, 0, $mes, $dia, $ano);
	$dataFormatada = date('d/m/Y',$dataFinal);
	echo $dataFormatada;
}

function dateDiff1($from,$to){
	$diff = $to - $from;
	$info = array();
	if($diff>86400){
		$info['d'] = ($diff - ($diff%86400))/86400;
		$diff = $diff%86400;
	}
	$f = '';
	foreach($info as $k=>$v){
		if($v>0) $f .= "$v";
	}
	return ($f == 24) ? "1" : $f;
}
$proid = $_GET['proid'];


if( empty($proid) ){
	echo "<script>
			alert('Faltam dados.');
			window.close();
		  </script>";
	die;
}

$sql = "SELECT profinalizado FROM par.processoobraspar WHERE proid = ".$proid;
$finalizado = $db->pegaUm($sql);

function salvarMinutaObras( $post, $mintexto = '', $dopid = '' ){
	global $db;
        
        if($_SESSION['par']['totalVLR']){
                $post['dopvalor'] = $_SESSION['par']['totalVLR'];
                unset($_SESSION['par']['totalVLR']);
        }
        
	$retorno = salvarDadosMinutaObras($post, $mintexto);
	
	if( $retorno ){
		$dopid = $dopid ? $dopid : $post['dopid'];
		if( $_POST['requisicao'] == 'salvar' ){
			enviaEmailNovoTermoObrasPAR($dopid);
		}
		$db->sucesso('principal/minutaObras', "&proid=".$_GET['proid']."&dopid=$dopid");
	} else {
		echo "<script>
				alert('Falha na operação');
				window.location.href = 'par.php?modulo=principal/minutaObras&acao=A&proid=$proid';
			</script>";
		exit();
	}
}

if( $_POST['requisicao'] == 'salvar' ){
	salvarMinutaObras( $_POST );
}

if( $_REQUEST['dopid'] ){
	$sql = "SELECT dp.dopid, dp.prpid, dp.iueid, dp.dopstatus, dp.dopdiasvigencia, dp.dopdatainicio, dp.dopdatafim, 
	  			dp.mdoid, dp.dopdatainclusao, dp.usucpfinclusao, dp.dopdataalteracao, dp.usucpfalteracao, md.tpdcod, odp.omjid, dp.dopjustificativa,
	  			dp.dopnumportaria, dp.doppaginadou, dp.dopdatapublicacao
			FROM par.documentopar dp
	        	left join par.modelosdocumentos md on md.mdoid = dp.mdoid
	            left join par.objetodocumentopar odp on odp.dopid = dp.dopid
			WHERE dp.dopid = ".$_REQUEST['dopid'];
	
	$arMinuta = $db->pegaLinha( $sql );
	$arMinuta = $arMinuta ? $arMinuta : array();
	extract($arMinuta);
}

$boObjeto = $omjid ? true : false;
$boDatas = ($dopdiasvigencia) ? true : false;
$boJustificativa = $dopjustificativa ? true : false;

$mdoid = $_POST['mdoid'] ? $_POST['mdoid'] : $mdoid;
$tpdcod = $_POST['tpdcod'] ? $_POST['tpdcod'] : $tpdcod;

if( !empty($mdoid) ){
	$iutid = $db->pegaUm("select iutid from par.modelosdocumentos where mdoid = $mdoid");
}

unset($_SESSION['par']['dopid']);

if($_POST['mdoid'] != "") {	
	
	$sql = "SELECT mdoconteudo FROM par.modelosdocumentos WHERE mdostatus = 'A' AND mdoid = ".$_POST['mdoid'];
	$mdoconteudo = $db->pegaUm($sql);
	
	$sisid = $db->pegaUm("SELECT sisid FROM par.processoobraspar WHERE prostatus = 'A' and proid = ".$proid);
	
	if(strpos($mdoconteudo, '#Justificativa#')) $boJustificativa = true;
	if(strpos($mdoconteudo, '#Objeto_Convenio#')) $boObjeto = true;
	if(strpos($mdoconteudo, '#Numero_Dias#') || strpos($mdoconteudo, '#Data_Fim_Vigencia') || strpos($mdoconteudo, '#Numero_Dias_Prorrogados#')
		|| strpos($mdoconteudo, '#Nova_Vigencia_Inicio#') || strpos($mdoconteudo, '#Nova_Vigencia_Fim#') || strpos($mdoconteudo, '#Data_Celebracao#')) $boDatas = true;
	
	if($mdoconteudo && $_POST['requisicao'] == 'geradocumento'){
		$dopid = salvarDadosMinutaObras($_POST);
		if( $dopid ){
			$_POST['dopid'] = $dopid;
			$_SESSION['par']['dopid'] = $dopid;
			if( $sisid == 14 ){
				$doptexto = alteraMacrosMinutaObrasBP($mdoconteudo, $proid, $_POST);
			} else {
				$doptexto = alteraMacrosMinutaObras($mdoconteudo, $proid, $_POST);
			}
			if($_SESSION['par']['totalVLR']){
				$_POST['dopvalor'] = $_SESSION['par']['totalVLR'];
				unset($_SESSION['par']['totalVLR']);
			}
			
			salvarMinutaObras($_POST, $doptexto, $dopid);
		} else {
			echo "<script>
				alert('Falha na operação!');
			</script>";
		}
	}
}


$perfil = pegaPerfilGeral();
//regras de acesso passada por Thiago em 24/05/2012
// corrigido por victor no dia 04/09/2012
if(
	in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ||
	in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ||
	in_array(PAR_PERFIL_COORDENADOR_GERAL ,$perfil) ||
	in_array(PAR_PERFIL_GERADOR_DOCUMENTO, $perfil)
){
	$disabled = 'S';
	$readonly 	= '';
}else{
	$disabled = 'N';
	$readonly = 'readonly="readonly"';
}

if( $finalizado == true ){
	$disabled = 'N';
	$readonly = 'readonly="readonly"';
}

if( $_REQUEST['dopid'] ){
	$doptexto = pegaTermoCompromissoArquivo( $_REQUEST['dopid'], '' );
}

$doptexto = $doptexto ? $doptexto : "";

$doptexto = str_replace('"', "'", $doptexto);

monta_titulo( $titulo_modulo, '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório' );

$sql = "SELECT
			p.pronumeroprocesso,
			iu.muncod,
		    m.mundescricao||' / '|| m.estuf as municipio,
		    e.estdescricao||' / '||e.estuf as estado
		FROM par.processoobraspar p
		    inner join par.instrumentounidade iu on iu.inuid = p.inuid
		    inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
		    left join territorios.municipio m on m.muncod = iu.muncod
		    left join territorios.estado e on e.estuf = iu.estuf
		WHERE
			p.prostatus = 'A' and
			p.proid = ".$_REQUEST['proid'];

$arTerritorio = $db->pegaLinha($sql);

if( $_REQUEST['proid'] ){
	$processo = $db->pegaUm("select pronumeroprocesso from par.processoobraspar where proid = {$_REQUEST['proid']}");
	verificaEmpenhoSigef( $processo );
}
 
?>
<style>

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 100%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 300px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
</style>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript" src="../library/jquery/jquery-1.10.2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
	<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="/includes/prototype.js"></script>

<form id="formulario" name="formulario" method="post" action="">
<input type="hidden" name="requisicao" id="requisicao" value="" />
<input type="hidden" name="proid" id="proid" value="<?=$proid; ?>" />
<input type="hidden" name="prpid" id="prpid" value="<?=$prpid; ?>" />
<input type="hidden" name="dopid" id="dopid" value="<?=$dopid; ?>" />
<input type="hidden" name="modelo" id="modelo" value="<?=$mdoid; ?>" />
<input type="hidden" name="iutid" id="iutid" value="<?=$iutid; ?>" />
<?php 

if($dopid){
	echo '<input type="hidden" id="doptexto" value="'.(($doptexto) ? $doptexto : '').'" />';
} else {
	echo '<input type="hidden" id="doptexto" value="'.$doptexto.'" />';
}
?>
<table class="tabela" align="center" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td class="subtitulodireita" style="width: 50%;"><b>Processo:</b></td>
		<td><?php echo formatarProcesso($arTerritorio['pronumeroprocesso']); ?></td>
	</tr>
	<?php
	if( !empty($arTerritorio['muncod']) ){ 
	?>
	<tr>
		<td class="subtitulodireita"><b>Município:</b></td>
		<td><?php echo $arTerritorio['municipio']; ?></td>
	</tr>
	<?} else {?>
	<tr>
		<td class="subtitulodireita"><b>Estado:</b></td>
		<td><?php echo $arTerritorio['estado']; ?></td>
	</tr>
	<?}?>
</table>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<!--  <tr>
		<td>
		<table class="tabela" align="left" border="0" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">-->
			<tr>
				<td class="SubTituloDireita" style="text-align: left;" colspan="3" valign="middle"><b>Documentos</b></td>
			</tr>
			<tr>
				<td class="subtitulodireita"><b>Tipo do Documento:</b></td>
				<td>
					<?php 			
						$sql = "SELECT
									tpdcod as codigo,
									tpddsc as descricao
								FROM
									public.tipodocumento
								WHERE
									tpdstatus = 'A'
								ORDER BY
									descricao";
						//$tpdcod = $_POST['tpdcod'] ? $_POST['tpdcod'] : $tpdcod;
						$db->monta_combo("tpdcod", $sql, $disabled, "Selecione...", 'carregaTipoDoc', '', '', '500', 'N', 'tpdcod');				
					?>
				</td>
			</tr>
		    <tr id="modelotermo" style="display: none">
		        <td class="SubTituloDireita" style="width: 19%;">
		            <b>Modelo de documento:</b>
		        </td> 
		        <td>
			    <?php 
			    //$mdoid = $_POST['mdoid'] ? $_POST['mdoid'] : $mdoid;
		    	$sql = "select 
		    				mod.mdoid as codigo, 
		    				mod.mdonome as descricao
							from par.modelosdocumentos mod
				     		where mod.mdostatus = 'A'
				     		and mod.tpdcod = {$tpdcod}
				     		and mod.mdonome ilike '%obra%'";
		    	if( $tpdcod )
			    	$db->monta_combo("mdoid", $sql, $disabled, 'Selecione...', 'carregarDados', '', '', '500', '', 'mdoid', '', '', 'Lista de modelo(s) vinculado(s) a termo aditivo' );
		        ?>
		        </td>    
		    </tr>
		    <?php
		    if( !empty($iutid) ){
		    ?>
		    <tr>
		        <td class="SubTituloDireita" style="width: 19%;">
		            <b>Assinatura de outra Entidade:</b>
		        </td> 
		        <td>
				    <?php 
			    	$sql = "SELECT iue.iueid as codigo, iue.iuenome as descricao
							FROM par.instrumentounidade iu
								INNER JOIN par.processoobraspar pro ON pro.inuid = iu.inuid and pro.prostatus = 'A'
								INNER JOIN par.instrumentounidadeentidade iue ON iu.inuid = iue.inuid
								INNER JOIN par.instrumentounidadeentidadetipo iut ON iut.iutid = iue.iutid
							WHERE
								iut.iutid = $iutid
								AND pro.proid = $proid";
			    	
				    $db->monta_combo("iueid", $sql, $disabled, 'Selecione...', '', '', '', '500', '', 'iueid', '', '', 'Lista de Entidades' );
			        ?>
		        </td>    
		    </tr>
		    <?
		    } else {
		    	echo '<input type="hidden" name="iueid" id="iueid" value="" />';
		    }
		    
		    if( $boDatas || $boObjeto ){ ?>
		    <tr>
		    	<td class="SubTituloDireita" style="text-align: center;" colspan="2" valign="middle"><b>Dados Adicionais</b></td>
		    </tr>
		    <?} if( $boDatas ){ ?>
			<tr>
				<td class="SubTituloDireita" valign="middle"><b>Data Celebração:</b></td>
				<td >
				<?	
					$dopdatainicio = ($dopdatainicio ? formata_data( $dopdatainicio ) : "");
					echo campo_data2('dopdatainicio', 'N', $disabled,'Data Inicio da Vigência', '', '', '', $dopdatainicio, 'calculaDataInicioVigencia();', '', 'dopdatainicio');
				?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="middle"><b>Dias de Vigência:</b></td>
				<td>
				<?
					echo campo_texto('dopdiasvigencia', 'S', $disabled, '', 5, 10, '[#]', '', '', '', 0, 'id="dopdiasvigencia"','', $dopdiasvigencia, 'calculaDiasVigencia();' );
				?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="middle"><b>Data Final da Vigência:</b></td>
				<td>
				<?
					$dopdatafim = ($dopdatafim ? formata_data($dopdatafim) : "");
					echo campo_data2('dopdatafim', 'N', $disabled,'Data Final da Vigência', '', '', '', $dopdatafim, 'calculaDataFimVigencia();', '', 'dopdatafim');
				?>
				</td>
			</tr>
			<?}
			if( $boObjeto ){ ?>
		    <tr>
				<td class="SubTituloDireita" valign="middle"><b>Objeto:</b></td>
				<td >
					<?php
					$sql = "SELECT oc.objid as codigo, oc.objdsc as descricao
							FROM 
								par.objeto oc
							    inner join par.objetodocumentopar omc on omc.objid = oc.objid
							WHERE
								omc.dopid = $dopid
							    and oc.objstatus = 'A'
							    --and oc.objtpobj = 'A'";
					
					if(  !empty($dopid) ) $objid = $db->carregar( $sql );
					
					$sql = "SELECT oc.objid as codigo, oc.objdsc as descricao FROM par.objeto oc
							WHERE  objstatus = 'A'
							--and oc.objtpobj = 'A'
							ORDER BY oc.objdsc";
								
					combo_popup( 'objid', $sql, '', '400x400', 0, array(), '', $disabled, false, false, 7, 400 );
					echo obrigatorio(); 
					?>
				</td>
			</tr>
			<?} 
			if($boJustificativa){ ?>
		    <tr>
				<td class="SubTituloDireita" valign="middle"><b>Justificativa:</b></td>
				<td><?=campo_textarea('dopjustificativa', 'S', $disabled, 'Justificativa', 80, 5, 4000, '', '', '', '', '');?></td>
			</tr>
			<tr>
		        <td class="SubTituloDireita" style="width: 19%;">
		            <b>ptres:</b>
		        </td> 
		        <td>
			    <?php 
			    	$db->monta_combo("mdoid1", array(), $disabled, 'Selecione...', '', '', '', '', '', 'mdoid1', '', '', '' );
		        ?>
		        </td>    
		    </tr>
			<tr>
		        <td class="SubTituloDireita" style="width: 19%;">
		            <b>PI:</b>
		        </td> 
		        <td>
			    <?php 
			    	$db->monta_combo("mdoid2", array(), $disabled, 'Selecione...', '', '', '', '', '', 'mdoid2', '', '', '' );
		        ?>
		        </td>    
		    </tr>
			<tr>
		        <td class="SubTituloDireita" style="width: 19%;">
		            <b>Natureza de Despesa:</b>
		        </td> 
		        <td>
			    <?php 
			    	$db->monta_combo("mdoid3", array(), $disabled, 'Selecione...', '', '', '', '', '', 'mdoid3', '', '', '' );
		        ?>
		        </td>    
		    </tr>
	<?} ?>
	</table>
	<?if( $_POST['tpdcod'] == '102' || $tpdcod == '102' || $_POST['tpdcod'] == '21' || $tpdcod == '21' ){ ?>
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="5" style="border-bottom:none; width: 95%">
		<tr>
			<th>Adicionar Obras ao Termo</th>
		</tr>
		<tr>
			<td><?
			
				$sisid = $db->pegaUm("SELECT sisid FROM par.processoobraspar WHERE prostatus = 'A' and proid = ".$_REQUEST['proid']);
					//<a href=\"#\" onclick=\"javascript:listarSubacao(\''||sub.sbaid||'\', \'' || sd.sobano || '\', \'' || pon.inuid || '\');\"><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" title=\"Visualizar subação\" border=\"0\"></a>

				if( $sisid == 14 ){ //Brasil pro
					$sql = "select acoes, codigo, descricao, subacao, sum(valorempenho) as valorempenho from (
			                    (
			                    SELECT distinct
									case when tc.preid is not null then
								    	'<center>
								    		<a href=\"#\" onclick=\"javascript:listarSubacao(\''||sub.sbaid||'\', \'' || sd.sobano || '\', \'' || pon.inuid || '\');\"><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" title=\"Visualizar subação\" border=\"0\"></a>
								    		<input type=checkbox name=chk[] id=chk_'||pre.preid||' checked value='||pre.preid||'></center>'
								    else
										'<center>
											<a href=\"#\" onclick=\"javascript:listarSubacao(\''||sub.sbaid||'\', \'' || sd.sobano || '\', \'' || pon.inuid || '\');\"><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" title=\"Visualizar subação\" border=\"0\"></a>
											<input type=checkbox name=chk[] id=chk_'||pre.preid||' value='||pre.preid||'></center>'
								    end as acoes,
								   d.dimcod || '.' || are.ardcod || '.' || i.indcod || '.' || sub.sbaordem||' ' as codigo,
								    pre.predescricao as descricao , 
								    sub.sbadsc as subacao ,
			                        ems.eobvalorempenho as valorempenho
								FROM
									par.processoobraspar prp
								inner join par.processoobrasparcomposicao ppc on ppc.proid = prp.proid
							    --inner join par.subacaoobra sd on sd.preid = ppc.preid
							    inner join cte.subacaoobra sd on sd.preid = ppc.preid
							    INNER JOIN cte.subacaoindicador sub ON sub.sbaid = sd.sbaid 
							    INNER JOIN cte.acaoindicador a ON a.aciid = sub.aciid
							    INNER JOIN cte.pontuacao pon ON pon.ptoid = a.ptoid
							    inner join obras.preobra pre on pre.preid = ppc.preid 
							    inner join par.empenhoobrapar ems on ems.preid = pre.preid and eobstatus = 'A'
							    inner join par.empenho emp on emp.empid = ems.empid and empstatus = 'A'
							    --inner join par.subacao sub on sub.sbaid = sd.sbaid and sub.sbastatus = 'A'
							    --inner join par.acao a on a.aciid = sub.aciid
							    --inner join par.pontuacao pon on pon.ptoid = a.ptoid
							    inner join cte.criterio c on c.crtid = pon.crtid
							    inner join cte.indicador i on i.indid = c.indid
							    inner join cte.areadimensao are on are.ardid = i.ardid
							    inner join cte.dimensao d on d.dimid = are.dimid
							    left join par.termocomposicao tc on tc.preid = sd.preid
								WHERE
									prp.prostatus = 'A' and
									prp.proid = {$_REQUEST['proid']}
									AND prp.sisid = 14
									AND emp.empsituacao ilike '%efetivado%'
								)
								UNION ALL
								(
								SELECT distinct
										case when tc.preid is not null then
									    	'<center>
									    		<a href=\"#\" onclick=\"javascript:listarSubacao(\''||sub.sbaid||'\', \'' || sd.sobano || '\', \'' || pon.inuid || '\');\"><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" title=\"Visualizar subação\" border=\"0\"></a>
									    		<input type=checkbox name=chk[] id=chk_'||pre.preid||' checked value='||pre.preid||'></center>'
									    else
											'<center>
												<a href=\"#\" onclick=\"javascript:listarSubacao(\''||sub.sbaid||'\', \'' || sd.sobano || '\', \'' || pon.inuid || '\');\"><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" title=\"Visualizar subação\" border=\"0\"></a>
												<input type=checkbox name=chk[] id=chk_'||pre.preid||' value='||pre.preid||'></center>'
									    end as acoes,
								    d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sub.sbaordem||' ' as codigo,
								    pre.predescricao as descricao , 
								    sub.sbadsc as subacao ,
			                        ems.eobvalorempenho as valorempenho
								FROM
									par.processoobraspar prp
							    inner join par.processoobrasparcomposicao ppc on ppc.proid = prp.proid
							    inner join par.subacaoobra sd on sd.preid = ppc.preid
		                        inner join obras.preobra pre on pre.preid = ppc.preid 
							    inner join par.empenhoobrapar ems on ems.preid = pre.preid and eobstatus = 'A'
							    inner join par.empenho emp on emp.empid = ems.empid and empstatus = 'A'
							    inner join par.subacao sub on sub.sbaid = sd.sbaid and sub.sbastatus = 'A'
							    inner join par.acao a on a.aciid = sub.aciid
							    inner join par.pontuacao pon on pon.ptoid = a.ptoid
							    inner join par.criterio c on c.crtid = pon.crtid
							    inner join par.indicador i on i.indid = c.indid
							    inner join par.area are on are.areid = i.areid
							    inner join par.dimensao d on d.dimid = are.dimid
							    left join (select t.preid from par.termocomposicao t
                                				inner join par.vm_documentopar_ativos dp on dp.dopid = t.dopid) tc on tc.preid = sd.preid
								WHERE
									prp.proid = {$_REQUEST['proid']}
									AND prp.sisid = 14
									and prp.prostatus = 'A'
								    AND emp.empsituacao in ( '1 - ENVIADO AO SIAFI', '2 - EFETIVADO', '8 - SOLICITAÇÃO APROVADA'  )
								)
			            	) as foo
				            group by 
				            	acoes, codigo, descricao, subacao
				            order by codigo, descricao";
				} else {
					$sql = "select acoes, codigo, descricao, subacao, sum(valorempenho) as valorempenho from (
			                    SELECT distinct
									case when tc.preid is not null then
								    	'<center>
								    		<a href=\"#\" onclick=\"javascript:listarSubacao(\''||sub.sbaid||'\', \'' || sd.sobano || '\', \'' || pon.inuid || '\');\"><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" title=\"Visualizar subação\" border=\"0\"></a>
								    		<input type=checkbox name=chk[] id=chk_'||pre.preid||' checked value='||pre.preid||'></center>'
								    else
										'<center>
											<a href=\"#\" onclick=\"javascript:listarSubacao(\''||sub.sbaid||'\', \'' || sd.sobano || '\', \'' || pon.inuid || '\');\"><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" title=\"Visualizar subação\" border=\"0\"></a>
											<input type=checkbox name=chk[] id=chk_'||pre.preid||' value='||pre.preid||'></center>'
								    end as acoes,
							    d.dimcod || '.' || are.arecod || '.' || i.indcod || '.' || sub.sbaordem||' ' as codigo,
							    pre.predescricao as descricao , 
							    sub.sbadsc as subacao ,
		                        ems.eobvalorempenho as valorempenho
							FROM
								par.processoobraspar prp
							    inner join par.processoobrasparcomposicao ppc on ppc.proid = prp.proid
							    inner join par.subacaoobra sd on sd.preid = ppc.preid
		                        inner join obras.preobra pre on pre.preid = ppc.preid
		                        INNER JOIN workflow.documento doc ON doc.docid = pre.docid  
							    inner join par.empenhoobrapar ems on ems.preid = pre.preid and eobstatus = 'A'
							    inner join par.empenho emp on emp.empid = ems.empid and empstatus = 'A'
							    inner join par.subacao sub on sub.sbaid = sd.sbaid and sub.sbastatus = 'A'
							    inner join par.acao a on a.aciid = sub.aciid
							    inner join par.pontuacao pon on pon.ptoid = a.ptoid
							    inner join par.criterio c on c.crtid = pon.crtid
							    inner join par.indicador i on i.indid = c.indid
							    inner join par.area are on are.areid = i.areid
							    inner join par.dimensao d on d.dimid = are.dimid
							    left join (select t.preid from par.termocomposicao t
                                				inner join par.vm_documentopar_ativos dp on dp.dopid = t.dopid) tc on tc.preid = sd.preid
								WHERE 
									prp.proid = {$_REQUEST['proid']}
									/* Adequado de acordo com regras de solicitação de prorrogação de prazo*/
									AND 
										( 
											doc.esdid = 337 OR
											(
												SELECT 
				                                    true 
				                                FROM obras.preobra po
				                                INNER JOIN workflow.documento doc ON po.docid = doc.docid
				                                WHERE 
				                                	po.preid = pre.preid
					                                AND doc.esdid = 1260 
					                                AND (SELECT popvalidacao FROM obras.preobraprorrogacao WHERE preid=po.preid ORDER BY popid DESC LIMIT 1) ='t'
											)
										)
									/* FIM - Adequado de acordo com regras de solicitação de prorrogação de prazo*/
									AND prp.prostatus = 'A'
								    AND emp.empsituacao in ( '1 - ENVIADO AO SIAFI', '2 - EFETIVADO', '8 - SOLICITAÇÃO APROVADA'  )
			            	) as foo
				            group by 
				            	acoes, codigo, descricao, subacao
				            order by codigo, descricao";
				}
// 				ver($sql,0);
				$cabecalho = array('&nbsp;&nbsp;Ação&nbsp;&nbsp;', 'Localiza', 'Nome da Obra', 'Programa', 'Valor Empenhado');
				if( empty($_REQUEST['proid']) ) $sql = array();
				$db->monta_lista_simples($sql, $cabecalho, 50000, 5, 'S', '100%');
			?></td>
		</tr>
		</table>
	<?} ?>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none; width: 95%">
	<tr id="botaotexto" style="display: none;">
		<td align="center" bgcolor="#c0c0c0" colspan="3">
			<?php if($readonly == ''){?>
				<input type="button" id="bt_carregar" value="Carregar Documento" onclick="carregaTexto();" />
			<?php }?>
		</td>
	</tr>
	<tr id="documento" style="display: none;">
		<td colspan="3">
			<div>
				<textarea id="texto" name="texto" rows="30" cols="80" style="width:100%" class="minutatinymce"></textarea>
			</div>
		</td>
	</tr>
	<tr id="dou" style="display: none;">
		<td>
		<?
			echo 'Data de publicação: ';
			echo campo_data2( 'dopdatapublicacao','S', 'S', 'Data do Publicação', 'S', '', '');	
		?>
		</td>
		<td>
		<?
			echo 'Página da publicação: ';
			echo campo_texto( 'doppaginadou', 'N', 'S', 'Página da publicação', 110, 100, '', '','','','','id="pubpagdou"');
		?>
		</td>
		<td>
		<?
			echo 'Secão da publicação: ';
			echo campo_texto( 'dopnumportaria', 'N', 'S', 'Portaria da publicação', 40, 50, '', '','','','','id="pubnumportaria"');
		?>
		</td>
	</tr>
	<tr id="botao" style="display: none;">
		<td align="center" bgcolor="#c0c0c0" colspan="3">
		<?
		if( $dopid && $tpdcod == 100 ){
			echo '<input type="button" value="Imprimir" onclick="imprimirMinutaDOU(\''.$dopid.'\');" />';
			echo '&nbsp;<input type="button" value="Imprime Word" onclick="minutaConvenioWord(\''.$dopid.'\');" />';	
			echo '&nbsp;<input type="button" id="bt_enviaemail" value="Enviar e-mail" onclick="enviarEmailPublicacao(\''.$dopid.'\');" />';
		}
		 ?>
		<?php if($readonly == ''){?>
			<input type="button" id="bt_salvar" value="Salvar" onclick="salvarMinutaTermoAditivo();" />
		<?php }?>
			&nbsp;
			<input type="button" id="bt_cancelar" value="Fechar" onclick="fechaMinuta();" />
		</td>
	</tr>
	</table>
</form>
<script type="text/javascript">
document.getElementById('texto').value = document.getElementById('doptexto').value;

tinyMCE.init({
	// General options
	mode : "textareas",
	theme : "advanced",
	language: "pt",
	editor_selector : "minutatinymce",
	plugins : "safari,pagebreak,style,layer,table,save,advhr,advimage,advlink,emotions,iespell,inlinepopups,insertdatetime,preview,media,searchreplace,print,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template,wordcount",

	// Theme options
	theme_advanced_buttons1 : "bold,italic,underline,strikethrough,|,justifyleft,justifycenter,justifyright,justifyfull,|,formatselect,fontselect,fontsizeselect",
	theme_advanced_buttons2 : "cut,copy,paste,pastetext,pasteword,|,search,replace,|,bullist,numlist,|,outdent,indent,blockquote,|,undo,redo,|,link,unlink,anchor,image,cleanup,help,code,|,insertdate,inserttime,preview,|,forecolor,backcolor",
	theme_advanced_buttons3 : "tablecontrols,|,hr,removeformat,visualaid,|,sub,sup,|,charmap,emotions,iespell,media,advhr,|,print,|,ltr,rtl,|,fullscreen",
	theme_advanced_buttons4 : "insertlayer,moveforward,movebackward,absolute,|,styleprops,|,cite,abbr,acronym,del,ins,attribs,|,visualchars,nonbreaking,template,pagebreak",
	theme_advanced_toolbar_location : "top",
	theme_advanced_toolbar_align : "left",
	theme_advanced_statusbar_location : "",
	theme_advanced_resizing : true,

	// Example content CSS (should be your site CSS)
	content_css : "css/content.css",

	// Drop lists for link/image/media/template dialogs
	template_external_list_url : "lists/template_list.js",
	external_link_list_url : "lists/link_list.js",
	external_image_list_url : "lists/image_list.js",
	media_external_list_url : "lists/media_list.js",

	// Replace values for the template plugin
	template_replace_values : {
		username : "Some User",
		staffid : "991234"
	}
});

function listarSubacao(sbaid, ano, inuid){
	var local = "par.php?modulo=principal/subacao&acao=A&sbaid=" + sbaid + "&anoatual=" + ano + "&inuid=" + inuid;
	janela(local,800,600,"Subação");
}

function fechaMinuta(){
	if( window.opener.document.getElementById('tipoacao').value == 'popup' ){
		window.opener.location.href = "par.php?modulo=principal/documentoParObras&acao=A&popup=true&tipoacao=popup";
	} else {
		window.opener.location.href = "par.php?modulo=principal/documentoParObras&acao=A&popup=true";
	}
	window.close();
}

function mostraCampos( tpdcod, mdoid, dopid ){
	if( tpdcod ){
		$('modelotermo').style.display = '';
	} else {
		$('modelotermo').style.display = 'none';
	}
	
	if( mdoid ){
		$('documento').style.display = '';
		$('botao').style.display = '';
		$('botaotexto').style.display = '';
		if( tpdcod == '100' && dopid != '' ) $('dou').style.display = '';
	} else {
		$('documento').style.display = 'none';
		$('botao').style.display = 'none';
		$('botaotexto').style.display = 'none';
		if( tpdcod == '100' ) $('dou').style.display = 'none';
	}
}

function carregaTipoDoc(){
	var formulario = $('formulario');
	
	for(i=0; i<formulario.length; i++){
		if(formulario.elements[i].type == "select-one"){		
			if(formulario.elements[i].name == 'mdoid'){
				document.getElementById("mdoid").value = "";
			}
		}	
	}
	document.getElementById("requisicao").value = "carregadoc";
	formulario.submit();
}

function carregarDados(){
	//document.getElementById("requisicao").value = "carregadoc";
	$('formulario').submit();
}

function calculaDataInicioVigencia(){
	var datainicio = $('dopdatainicio').value;
	var datafim = $('dopdatafim').value;
	var diavigencia = $('dopdiasvigencia').value;
		 
	if( datafim != '' ){
		var myajax = new Ajax.Request('par.php?modulo=principal/minutaObras&acao=A', {
		        method:     'post',
		        parameters: '&pegaDataFinal=true&dopdatafim='+datafim+'&dopdatainicio='+datainicio,
		        asynchronous: false,
		        onComplete: function (res){
		        	//$('erro').innerHTML = res.responseText;
		        	$('dopdiasvigencia').value = res.responseText;
		        }
		  });
	} else if( diavigencia != '' ){
		var myajax = new Ajax.Request('par.php?modulo=principal/minutaObras&acao=A', {
		        method:     'post',
		        parameters: '&pegaDataFinal=true&dopdiasvigencia='+diavigencia+'&dopdatainicio='+datainicio,
		        asynchronous: false,
		        onComplete: function (res){
		        	$('dopdatafim').value = res.responseText;
		        }
		  });
	}	
}

function calculaDiasVigencia(){
	var datainicio = $('dopdatainicio').value;
	var diavigencia = $('dopdiasvigencia').value;	 
	
	if( diavigencia != '' && datainicio != '' ){
		var myajax = new Ajax.Request('par.php?modulo=principal/minutaObras&acao=A', {
		        method:     'post',
		        parameters: '&pegaDataFinal=true&dopdiasvigencia='+diavigencia+'&dopdatainicio='+datainicio,
		        asynchronous: false,
		        onComplete: function (res){
		        	//$('erro').innerHTML = res.responseText;
					$('dopdatafim').value = res.responseText;
		        }
		  });
	}	
}

function calculaDataFimVigencia(){
	var datainicio = $('dopdatainicio').value;
	var datafim = $('dopdatafim').value;	 
	
	if( datainicio != '' && datafim != '' ){
		var myajax = new Ajax.Request('par.php?modulo=principal/minutaObras&acao=A', {
		        method:     'post',
		        parameters: '&pegaDataFinal=true&dopdatafim='+datafim+'&dopdatainicio='+datainicio,
		        asynchronous: false,
		        onComplete: function (res){
		        	$('dopdiasvigencia').value = res.responseText;
					//$('erro').innerHTML = res.responseText;
		        }
		  });
	}	
}

function validaFormulario(){
	var formulario = $('formulario');
	var checkVazio = true;
	
	for(i=0; i<formulario.length; i++){
		if(formulario.elements[i].type == "text"){		
			if(formulario.elements[i].name == 'dopdatainicio' && $('tpdcod').value == 100 ){ //se tipo de documento for DOU
				if( $('dopdatainicio').value == "" ){
					alert( 'O campo Data Celebração é de preenchimento obrigatório!' );
					$('dopdatainicio').focus();
					return false;
				}
				if(!validaData(document.getElementById('dopdatainicio'))) {
					alert('Data Celebração incorreta!');
					return false;
				}
			}else if( formulario.elements[i].name == 'dopdiasvigencia' ){
				if( $('dopdiasvigencia').value == "" ){
					alert( 'O campo Dias de Vigência é de preenchimento obrigatório!' );
					$('dopdiasvigencia').focus();
					return false;
				}
			}else if( formulario.elements[i].name == 'dopdatafim' && $('tpdcod').value == 100 ){ //se tipo de documento for DOU
				if( $('dopdatafim').value == "" ){
					alert( 'O campo Data Final da Vigência é de preenchimento obrigatório!' );
					$('dopdatafim').focus();
					return false;
				}
				if(!validaData(document.getElementById('dopdatafim'))) {
					alert('Data Final da Vigência incorreta!');
					return false;
				}
			}
		}
		if(formulario.elements[i].type == "textarea"){
			if( formulario.elements[i].name == 'dopjustificativa' ){
				if( $('dopjustificativa').value == "" ){
					alert( 'O campo Justificativa é de preenchimento obrigatório!' );
					$('Justificativa').focus();
					return false;
				}
			}
		}
		if(formulario.elements[i].type == "select-multiple"){
			if( formulario.elements[i].name == 'objid[]' ){
				selectAllOptions( document.getElementById( 'objid' ) );
				var objid	  = document.getElementById("objid");
				var j = objid.options.length;
				if( objid.options[0].value == '' ){
					alert('Objeto é de preenchimento obrigatório');
					return false;
				}
			}
		}
		if(formulario.elements[i].type == "checkbox"){
			if( formulario.elements[i].name == 'chk[]' ){
				if( formulario.elements[i].checked == true ){
					checkVazio = false;
				}
			}
		}
	}
	if( checkVazio && ($('tpdcod').value == '102' || $('tpdcod').value == '21') ){
		alert('Selecione as obras para adicionar ao documento');
		return false;
	}
	return true;
}

function salvarMinutaTermoAditivo() {
	var texto = tinyMCE.get('texto').getContent();

	document.getElementById("bt_salvar").disabled = true;
	
	if( texto != '' ){
		if( validaFormulario() ){	
			document.getElementById("requisicao").value = "salvar";
			$('formulario').submit();
		}
	} else {
		alert('É necessário carregar o documento antes de salvar.');
		document.getElementById("bt_salvar").disabled = false;
		return false;
	}
	document.getElementById("bt_salvar").disabled = false;
}

function carregaTexto( mdoid ) {

	document.getElementById("bt_carregar").disabled = true;
	
	if( validaFormulario() ){
		if( document.getElementById('iutid').value != '' && document.getElementById('iueid').value == '' ){
			alert('Campo obrigatório.');
			document.getElementById('iueid').focus();
			document.getElementById("bt_carregar").disabled = false;
			return false;
		}
		selectAllOptions( document.getElementById( 'objid' ) );
		document.getElementById("requisicao").value = "geradocumento";
		$('formulario').submit();
	} else {
		document.getElementById("bt_carregar").disabled = false;
	}
}

function imprimirMinutaDOU(dopid) {
	var janela = window.open( '/par/par.php?modulo=principal/popImprimeMinutaDOU&acao=A&dopid='+dopid+'', 'imprimeminuta', 'width=450,height=400,status=0,menubar=1,toolbar=0,scrollbars=1,resizable=0' );
	janela.focus();
}

function enviarEmailPublicacao(dopid){
	if( confirm('Deseja enviar e-mail para o responsável da publicação?') ){
		document.getElementById("requisicao").value = "email";
		$('formulario').submit();
	}
}

function minutaConvenioWord(dopid){
	if(dopid){
		window.location.href='par.php?modulo=principal/minutaObras&acao=A&dopid='+dopid+'&imprimeWord=1';
	}
}

mostraCampos('<?=$tpdcod; ?>', '<?=$mdoid; ?>', '<?=$dopid ?>');
</script>
<div id="erro"></div>
<?  
$_POST['texto'] = '';
//listaMinuta($prpid);
?>