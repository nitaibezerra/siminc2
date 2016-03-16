<center>
	<div id="aguardando" style="display:none; position: absolute; background-color: white; height:500%; width:100%; opacity:0.5; filter:alpha(opacity=40); z-index:100 " >
		<div style="margin-top:250px; align:center;">
			<img border="0" title="Aguardando" src="../imagens/carregando.gif">
			Carregando...
		</div>
	</div>
</center>
<?php


function getBloqueiaParaConsultaEstMun() 
{
	$perfil = pegaArrayPerfil($_SESSION['usucpf']);
	
	if (in_array(  PAR_PERFIL_CONSULTA_ESTADUAL, $perfil ) || in_array(  PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL, $perfil ) || in_array( PAR_PERFIL_CONSULTA_MUNICIPAL, $perfil ) || in_array( PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $perfil ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

header('content-type: text/html; charset=ISO-8859-1');
echo'<br>';

// Função que retorna os dados dos itens
function carregaItensTOR()
{
	global $db;
	 
	$bloqueiaParaConsultaEstMun = getBloqueiaParaConsultaEstMun();
	
	if( $bloqueiaParaConsultaEstMun )
	{
		
		$chk = "'' as chk,";
		$chk2 = "''";
	}
	else
	{
		$chk = "'<center><input type=checkbox class=todos id={$_POST['sbdid']} /></center>' as chk,";
		$chk2 = "'<center><input type=checkbox name=icoids[] class={$_POST['sbdid']} value='|| si.icoid ||' /></center>'";
	}	
	
	$sql = "(
			SELECT
				{$chk}
				'<b>Marcar Todos</b>' as icodescricao,
				'' as tors
			)
			UNION ALL
			(
			SELECT  DISTINCT
				CASE 
					WHEN esdid = 1097 THEN '<center><img width=\"20px\" src=../imagens/check_checklist.png style=cursor:pointer; tittle=Aprovado onclick=\"wf_exibirHistorico('||doc.docid||')\" /></center>' --97
					ELSE 
					{$chk2}
				END as chk,
				si.icodescricao as icodescricao,
				CASE WHEN atp.atpid IS NOT NULL
					THEN '<center><img src=../imagens/icone_lupa.png style=cursor:pointer; onclick=\"arquivoItem('|| si.icoid ||');\" /></center>'
					ELSE 'Não possui'
				END as tors
			FROM
				par.subacao s
			INNER JOIN par.subacaodetalhe 				sd ON sd.sbaid = s.sbaid
			INNER JOIN par.termocomposicao 				tc ON tc.sbdid = sd.sbdid
			INNER JOIN par.subacaoitenscomposicao 		si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano
			LEFT  JOIN par.anexotorpar 					atp 
				INNER JOIN workflow.documento 	doc ON doc.docid = atp.docid
			ON atp.atpid = si.atpid
			WHERE
				sd.sbdid = {$_POST['sbdid']}
				AND si.picid NOT IN (
					SELECT DISTINCT
						picid
					FROM 
						par.propostaitemcomposicao 
					WHERE
						picpregao IS TRUE
					)
			ORDER BY si.icodescricao
			)";
// ver(simec_htmlentities($sql),d);
	$cabecalho = array("&nbsp;", "Descrição do Item", "TR do item" );
	$db->monta_lista($sql,$cabecalho,5000,5,'N','90%','N');
}

function testaPermissaoAnaliseTOR(){
	 
	$arrPerfil = Array( PAR_PERFIL_SUPER_USUARIO, 
						PAR_PERFIL_ADMINISTRADOR, 
						PAR_PERFIL_ANALISTA_TR );
	
	return possui_perfil( $arrPerfil );
}

function montaAbasTOR(){
	$arrAbas =	array(
			0 => array( "descricao" => "Termos de Referência", "link" => "par.php?modulo=principal/tor/torDocumentos&acao=A&abas=torDocumentosTermoreferencia&dopid=" . $_SESSION['dopid'] )
	);
	
	if( testaPermissaoAnaliseTOR() ){
		array_push($arrAbas, array( "descricao" => "Analise", "link" => "par.php?modulo=principal/tor/torDocumentos&acao=A&abas=torDocumentosAnalise&dopid=" . $_SESSION['dopid']));
	}
	
	$aba = $_REQUEST['abas'] ? $_REQUEST['abas'] : 'torDocumentosTermoreferencia';
	
	return montarAbasArray( $arrAbas, "par.php?modulo=principal/tor/torDocumentos&acao=A&abas=$aba&dopid=" . $_SESSION['dopid'] );
}
/*
function getNumDoc( $dopid )
{
	$sql = "SELECT
				CASE WHEN dp2.dopano::boolean 
					THEN dp.dopnumerodocumento::text || '/' || dp2.dopano::text
					ELSE dp.dopnumerodocumento::text
				END as ndocumento
			FROM 
				par.documentopar dp
			LEFT JOIN par.documentopar dp2 ON dp2.dopid = dp.dopnumerodocumento
			WHERE 
				dp.dopid = $dopid";
	global $db;
	 
	$result = $db->pegaUm( $sql );
	
	return $result ? $result : 'erro';
}
*/
function cabecalhoTOR( $cabecalho, $msg ){
	
	global $db;
	
	//Monta cabeçalho
	if( ! $_SESSION['par']['muncod'] ){
	
		$local = "Estado: ".$_SESSION['par']['estuf'];
	}
	else
	{
		$sql = "SELECT mundescricao FROM territorios.municipio WHERE muncod = '{$_SESSION['par']['muncod']}'";
		$mundescricao = $db->pegaUm($sql);
		$local = "Município: $mundescricao - " . $_SESSION['par']['estuf'];
	}
	
	$html = '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">
				<tr>
					<td width="100%" colspan="3" align="center"><label class="TituloTela" style="color:#000000;">'.$cabecalho.'</label></td>
				</tr>
				<tr>
					<td bgcolor="#e9e9e9" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b> '.$local.' </b></td>
					<td bgcolor="#e9e9e9" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b>Termo de compromisso</b></td>
					<td bgcolor="#e9e9e9" style="width:78%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >
					<img src="../imagens/icone_lupa.png" style="cursor:pointer;" title="Visualizar Termo" onclick="carregaTermoMinuta(\''.$_SESSION['dopid'].'\');" border="0">
					'. getNumDoc( $_SESSION['dopid'] ).'</td>
					<td bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >
						<input id="voltar" type="button" onclick="voltar()" value="Voltar" name="voltar" >
					</td>
				</tr>
				<tr>
					<td colspan="4" bgcolor="#DCDCDC" align="center" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >
						<b>'.$msg.'</b>
					</td>
				</tr>
			</table>';
	
	return $html;
}

function analisar(){

	global $db;

	if( testaPermissaoAnaliseTOR() ){
		require_once APPRAIZ . 'includes/workflow.php';
		
		$docids = array_unique($_REQUEST['docids']);
		
		foreach( $docids as $docid){
			wf_alterarEstado( $docid, $_REQUEST['aedid'], $_REQUEST['parecer'], array( 'docid' => $docid ) );
		}
		
		echo "<script>
				alert('Itens analisados com sucesso!');
				window.location.href = window.location.href;
			  </script>";
	}else{
		
		echo "<script>
				alert('Você não possui permissão para executar essa operação!');
				window.location.href = window.location.href;
			  </script>";
	}
}

function carregaTermos()
{
	global $db;

	$sql = "(
        SELECT
            '<center><input type=checkbox id=todos /></center>' as chk,
            '<b>Marcar Todos</b>' as icodescricao,
            '' as tors
        )
        UNION ALL
        (
        SELECT  DISTINCT
            CASE 
                WHEN esdid = 1096 THEN '<center><input type=checkbox name=docids[] value='|| atp.docid ||' /></center>' --96
                WHEN esdid = 1097 THEN '<center><img width=\"20px\" src=../imagens/check_checklist.png style=cursor:pointer; tittle=Aprovado onclick=\"wf_exibirHistorico('||doc.docid||')\" /></center>' --97
                ELSE '<center><img width=\"20px\" src=../imagens/check_checklist_vermelho.png style=cursor:pointer; tittle=Recusado onclick=\"wf_exibirHistorico('||doc.docid||')\" /></center>'
            END as chk,
            si.icodescricao,
            CASE WHEN atp.atpid IS NOT NULL
                THEN '<center><img src=../imagens/icone_lupa.png style=cursor:pointer; onclick=\"arquivoItem('|| si.icoid ||');\" /></center>'
                ELSE 'Não possui'
            END as tors
        FROM
        par.subacao s
        INNER JOIN par.subacaodetalhe 			sd  ON sd.sbaid = s.sbaid
        INNER JOIN par.termocomposicao 			tc  ON tc.sbdid = sd.sbdid
        INNER JOIN par.subacaoitenscomposicao 	si  ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano
        INNER JOIN par.anexotorpar 				atp ON atp.atpid = si.atpid
        INNER JOIN workflow.documento 			doc ON doc.docid = atp.docid
        WHERE
            tc.dopid = {$_REQUEST['dopid']}
            AND sd.sbdid = {$_REQUEST['sbdid']}
        ORDER BY si.icodescricao
        )";

    $cabecalho = array("&nbsp;", "Descrição do Item", "TR do item" );
    $db->monta_lista_simples($sql,$cabecalho,5000,0,'N','90%','N');
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	die();
}

header('content-type: text/html; charset=ISO-8859-1');
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$perfil = pegaArrayPerfil($_SESSION['usucpf']);

// Id do documento
$dopid = $_REQUEST['dopid'];
$_SESSION['dopid'] = $dopid;

?>
<script type="text/javascript" src="../includes/funcoes.js"></script>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<script>
function carregaTermoMinuta(dopid){
	window.open('par.php?modulo=principal/visualizaTermoCompromisso&acao=A&dopid='+dopid,'visualizatermo','scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes');
}
function voltar()
{
	location.href='par.php?modulo=principal/administracaoDocumentos&acao=A';
}

function arquivoItem( icoid ){
	window.open('par.php?modulo=principal/tor/popupAnexoTOR&acao=A&arrIcoId=' + icoid+'|','','width=780,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left='+ (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);
}
</script>
<?php
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
?>  
<table style="background-color: #F5F5F5;" align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
	<tr>
		<td>
		<?php 
		
		$_REQUEST['abas'] = (empty($_REQUEST['abas']) ? 'torDocumentosTermoreferencia' : $_REQUEST['abas']);
		
		include_once APPRAIZ . "par/modulos/principal/tor/{$_REQUEST['abas']}.inc";
		?>
		</td>
	</tr>
</table>