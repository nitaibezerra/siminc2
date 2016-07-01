<?php
if($_REQUEST['download'] == 'S'){

	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$arqid = $_REQUEST['arqid'];
	$file = new FilesSimec();
	$arquivo = $file->getDownloadArquivo($arqid);
	echo"<script>window.location.href = '?modulo=principal/acompanhamento/ferramentas/termoContratosNotas&acao=A' ;</script>";
	exit;

}

header('content-type: text/html; charset=ISO-8859-1');
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

// $_REQUEST['dopid'] = 6105;
// $_SESSION['dopid'] = 6105;
// $dopid = '6105';

// Id do documento
// @todo stand by, talvez precise, verficiar no outro arquivo acompanhamentoDocumento
$dopid = $_REQUEST['dopid'];
$_SESSION['dopid'] = $dopid;

function carregaDadosTermos($excel = false)
{
	global $db;

	$filtroUF 			= $_POST['estuf'];
	$filtroMun 			= $_POST['mundescricao'];
	$filtroNtermo 		= $_POST['ntermo'];
	$filtroNprocesso	= $_POST['empnumeroprocesso'];

	$filtroContratosNaoVinculados	= $_POST['convinculado'];

	if($filtroContratosNaoVinculados)
	{
		$filtroContratosNaoVinculados = 'not';
	}
	else
	{
		$filtroContratosNaoVinculados = '';
	}
		
	switch( $_REQUEST['aba'] ){
		case 'termonotas';
			$sqlAba = "SELECT s.sbaid
						FROM 
							par.subacaoitenscomposicaonotasfiscais 	snf -- notas fiscais de cada item
						INNER JOIN par.subacaoitenscomposicao 		sic ON sic.icoid = snf.icoid -- itens
						INNER JOIN par.subacao 						s   ON s.sbaid = sic.sbaid and s.sbastatus = 'A'";
		break;
		default:
			$sqlAba = "SELECT DISTINCT
							si.sbaid
						FROM
							par.contratos con
						INNER JOIN par.subacaoitenscomposicaoContratos sicon ON sicon.conid = con.conid
						INNER JOIN par.subacaoitenscomposicao si ON si.icoid = sicon.icoid";
		break;
	}

	if( ! $excel)
	{
		$sql = "SELECT DISTINCT
					'<center>
						<img 	style=\"cursor:pointer\" id=\"img_subacao_' || dp.dopid || '\" src=\"/imagens/mais.gif\" 
								onclick=\"carregasubacoescontratos(this.id,\'' || dp.dopid || '\');\" border=\"0\" />
					</center>'
					as acao,
					CASE WHEN iu.estuf IS NULL 
						THEN iu.mun_estuf
						ELSE iu.estuf
					END as uf,
					m.muncod || '<input type=\'hidden\'>' as cod_ibge,
					m.mundescricao as municipio,
					CASE WHEN dp2.dopano::boolean 
						THEN dp.dopnumerodocumento::text || '/' || dp2.dopano::text
						ELSE dp.dopnumerodocumento::text
					END || '<input type=\'hidden\'>' as ndocumento,
					substring(p.prpnumeroprocesso from 1 for 5)||'.'||
					substring(p.prpnumeroprocesso from 6 for 6)||'/'||
					substring(p.prpnumeroprocesso from 12 for 4)||'-'||
					substring(p.prpnumeroprocesso from 16 for 2) as numeroprocesso,
					tpddsc
					|| '</td></tr>
					<tr style=\"display:none\" id=\"listaDocumentos2_' || dp.dopid || '\" >
					<td id=\"trI_' || dp.dopid || '\" colspan=8 ></td>
					</tr>'
					as tipo
				FROM par.instrumentounidade iu
				INNER JOIN par.processopar 				p   ON p.inuid = iu.inuid AND p.prpstatus = 'A'
				INNER JOIN par.vm_documentopar_ativos 	dp  ON dp.prpid = p.prpid
				INNER JOIN par.modelosdocumentos 		em  ON dp.mdoid = em.mdoid
				INNER JOIN public.tipodocumento 		tp  ON tp.tpdcod = em.tpdcod
				LEFT JOIN par.documentopar 				dp2 ON dp2.dopid = dp.dopnumerodocumento

				INNER JOIN par.termocomposicao tc ON tc.dopid = dp.dopid
				inner join par.subacaodetalhe sd on tc.sbdid = sd.sbdid
				inner join par.subacao 		s on sd.sbaid = s.sbaid
		
				LEFT JOIN territorios.municipio  m on iu.muncod = m.muncod AND mun_estuf is not null
				LEFT JOIN territorios.estado e on e.estuf = iu.estuf AND mun_estuf is null
				WHERE 
					s.sbaid {$filtroContratosNaoVinculados} IN
						(
						$sqlAba
						)";
	}
	else
	{
		
		if($_POST['checksem'])
		{
			$filtroContratosNaoVinculados = 'not';
		}
		else
		{
			$filtroContratosNaoVinculados = '';
		}
		
		$sql = "SELECT DISTINCT
					dp.dopid
					as acao,
					CASE WHEN iu.estuf IS NULL 
						THEN iu.mun_estuf
						ELSE iu.estuf
					END as uf,
					m.muncod as cod_ibge,
					m.mundescricao as municipio,
					CASE WHEN dp2.dopano::boolean THEN
					dp.dopnumerodocumento::text || '/' || dp2.dopano::text
					ELSE
						dp.dopnumerodocumento::text
						END
							as ndocumento,
					substring(p.prpnumeroprocesso from 1 for 5)||'.'||
					substring(p.prpnumeroprocesso from 6 for 6)||'/'||
					substring(p.prpnumeroprocesso from 12 for 4)||'-'||
					substring(p.prpnumeroprocesso from 16 for 2) as numeroprocesso,
					tpddsc as tipo
				FROM par.instrumentounidade iu
				INNER JOIN par.processopar 				p   ON p.inuid = iu.inuid AND p.prpstatus = 'A'
				INNER JOIN par.vm_documentopar_ativos 	dp  ON dp.prpid = p.prpid
				INNER JOIN par.modelosdocumentos 		em  ON dp.mdoid = em.mdoid
				INNER JOIN public.tipodocumento 		tp  ON tp.tpdcod = em.tpdcod
				LEFT  JOIN par.documentopar 			dp2 ON dp2.dopid = dp.dopnumerodocumento

				INNER JOIN par.termocomposicao tc ON tc.dopid = dp.dopid
				INNER JOIN par.subacaodetalhe sd on tc.sbdid = sd.sbdid
				INNER JOIN par.subacao 		s on sd.sbaid = s.sbaid

				LEFT JOIN territorios.municipio  m on iu.muncod = m.muncod AND mun_estuf is not null
				LEFT JOIN territorios.estado e on e.estuf = iu.estuf AND mun_estuf is null
				WHERE 
					s.sbaid {$filtroContratosNaoVinculados} IN
						(
						$sqlAba
						)";
		}
			
		if( $filtroUF )
		{
			$sql .= " AND CASE WHEN iu.estuf IS NULL 
							THEN iu.mun_estuf = '{$filtroUF}'
							ELSE iu.estuf = '{$filtroUF}' END";
		}
		if($filtroMun)
		{
			$filtroMun = str_replace('/','',$filtroMun);
				
			$sql .= " AND m.mundescricao ilike '%{$filtroMun}%'";
		}
		if( $filtroNtermo )
		{
			$sql .= " AND dp.dopnumerodocumento::text ilike '%{$filtroNtermo}%' ";
		}
		if( $filtroNprocesso )
		{
			$filtroNprocesso = str_replace(".","", $filtroNprocesso);
			$filtroNprocesso = str_replace("/","", $filtroNprocesso);
			$filtroNprocesso = str_replace("-","",$filtroNprocesso );
			
			$sql .= " AND p.prpnumeroprocesso ilike '%$filtroNprocesso%' ";
		}

		if($excel)
		{
			$cabecalho = array("Doc id", "UF", "IBGE", "Município", "Nº termo", "Nº do processo", "Tipo do termo" );
			ob_clean();
			header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
			header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
			header ( "Pragma: no-cache" );
			header ( "Content-type: application/xls; name=Termos_contratos_firmados".date("Ymdhis").".xls");
			header ( "Content-Disposition: attachment; filename=SIMEC_RelatDocente".date("Ymdhis").".xls");
			header ( "Content-Description: MID Gera excel" );
				
			$db->monta_lista_tabulado($sql,$cabecalho,1000000,5,'N','100%',$par2);
			exit;
		}
		$cabecalho = array("&nbsp;Ações&nbsp;", "UF", "IBGE", "Município", "Nº termo", "Nº do processo", "Tipo do termo" );
		$db->monta_lista($sql,$cabecalho,20,5,'N','95%','S');
}

function carregasubacoesComContratos()
{

	global $db;
	
	if( $_REQUEST['convinculado'] )
	{
		$filtroContratosNaoVinculados = 'NOT';
	}
	else
	{
		$filtroContratosNaoVinculados = '';
	}
	
	switch( $_REQUEST['aba'] ){
		case 'termonotas';
			$sqlAba = "SELECT 
							sd.sbdid
						FROM 
							par.notasfiscais  nf 
						INNER JOIN par.subacaoitenscomposicaonotasfiscais 	snf ON snf.ntfid = nf.ntfid-- notas fiscais de cada item
						INNER JOIN par.subacaoitenscomposicao 				sic ON sic.icoid = snf.icoid -- itens
						INNER JOIN par.subacao 								s   ON s.sbaid = sic.sbaid 	AND s.sbastatus = 'A'
						INNER JOIN par.subacaodetalhe 						sd  ON sd.sbaid = s.sbaid 	AND sic.icoano = sd.sbdano
						INNER JOIN par.processoparcomposicao 				ppc ON ppc.sbdid = sd.sbdid and ppc.ppcstatus = 'A'
						INNER JOIN par.processopar 							pp  ON pp.prpid = ppc.prpid AND pp.prpstatus = 'A'
						INNER JOIN par.documentopar 						dop ON dop.prpid = pp.prpid
						WHERE 
							dop.dopid = {$_POST['dopid']}
							AND pp.prpstatus = 'A'";
		break;
		default:
			$sqlAba = "SELECT  DISTINCT
						sd.sbdid
					FROM par.subacao s
					INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
					INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
					INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano
					INNER JOIN par.subacaoitenscomposicaocontratos sicc ON  si.icoid = sicc.icoid
					INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
					LEFT  JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = si.sbaid AND emi.epistatus = 'A'
					WHERE 
						sd.sbdid IN 
							(
							SELECT DISTINCT
								CASE WHEN s.sbaid  IS NOT NULL THEN -- quer dizer que ele perdeu itens.
								sd.sbdid
								END
							FROM par.subacao s
							INNER JOIN par.subacaodetalhe 	sd on sd.sbaid = s.sbaid -- and sd.sbdano = '2013' -- tabela de detalhe da subação
							INNER JOIN par.termocomposicao 	tc on tc.sbdid = sd.sbdid -- tabela fica a composicao do termo (os itens)
							INNER JOIN par.documentopar 	dp on dp.dopid = tc.dopid -- dopnumerodocumento = 4851 -- fica o termo de compromisso
							WHERE 
								dp.dopid = {$_POST['dopid']}
							)";
			break;
	}

	$sql = "SELECT DISTINCT
				'<center>
					<img style=\"cursor:pointer\" id=\"img_contratos_' || sd.sbdid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarcontratositens(this.id,\'' || sd.sbdid || '\');\" border=\"0\" />
				</center>' as acao,
				par.retornacodigosubacao(s.sbaid),
				s.sbadsc
				|| '</td></tr>
				<tr style=\"display:none\" id=\"listaDocumentos3_' || sd.sbdid || '\" >
				<td id=\"trII_' || sd.sbdid || '\" colspan=8 ></td>
				</tr>'
			FROM 
				par.subacao s
			INNER JOIN par.subacaodetalhe sd on sd.sbaid = s.sbaid -- and sd.sbdano = '2013' -- tabela de detalhe da subação
			INNER JOIN par.termocomposicao tc on tc.sbdid = sd.sbdid -- tabela fica a composicao do termo (os itens)
			INNER JOIN par.documentopar dp on dp.dopid = tc.dopid -- dopnumerodocumento = 4851 -- fica o termo de compromisso
			WHERE 
				dp.dopid = {$_POST['dopid']}
				AND sd.sbdid {$filtroContratosNaoVinculados} IN
					(
					$sqlAba
					)";
// 	ver($_REQUEST, simec_htmlentities($sql),d);
	$cabecalho = array("&nbsp;Ações&nbsp;", "Localização", "Descrição da Subação" );

	$db->monta_lista($sql,$cabecalho,50000,5,'N','90%','N');
}
	
function carregaContratos()
{

	global $db;
	
	$sql = "SELECT DISTINCT
				'<span style=\"display:none\">' || con.conid || '</span>' || si.icodescricao,
				con.conid,
				con.connumerocontrato,
				con.condescricao,
				'<img src=../imagens/icone_lupa.png style=cursor:pointer; class=download arqid='|| con.arqid ||' >' as acao
			FROM
				par.contratos con
			INNER JOIN par.subacaoitenscomposicaoContratos sicon ON sicon.conid = con.conid
			INNER JOIN par.subacaoitenscomposicao si ON si.icoid = sicon.icoid
			WHERE
				si.icoid  IN
					(
						SELECT DISTINCT
							si.icoid
						from par.subacao s
						INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
						INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
						INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano AND si.icostatus = 'A'
						INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
						LEFT JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = si.sbaid   AND emi.epistatus = 'A'
						WHERE
						 	sd.sbdid IN ( {$_POST['sbdid']} )
					)
				AND icomonitoramentocancelado = false	
					";

	$cabecalho = array('Descrição Item', 'ID do contrato', 'Nº do Contrato' ,'Descricão Contrato',  'Download' );
	
	$db->monta_lista($sql,$cabecalho,50000,5,'N','90%','N');
}
	
function carregaNotas()
{

	global $db;
	
	$sql = "SELECT 
				sic.icodescricao,
				nf.ntfdescricao,
				'<img src=../imagens/icone_lupa.png style=cursor:pointer; class=download arqid='|| nf.arqid ||' >' as acao
			FROM 
				par.notasfiscais  nf 
			INNER JOIN par.subacaoitenscomposicaonotasfiscais 	snf ON snf.ntfid = nf.ntfid-- notas fiscais de cada item
			INNER JOIN par.subacaoitenscomposicao 				sic ON sic.icoid = snf.icoid -- itens
			INNER JOIN par.subacao 								s   ON s.sbaid = sic.sbaid 	AND s.sbastatus = 'A'
			INNER JOIN par.subacaodetalhe 						sd  ON sd.sbaid = s.sbaid 	AND sic.icoano = sd.sbdano
			INNER JOIN par.processoparcomposicao 				ppc ON ppc.sbdid = sd.sbdid and ppc.ppcstatus = 'A'
			INNER JOIN par.processopar 							pp  ON pp.prpid = ppc.prpid AND pp.prpstatus = 'A'
			WHERE 
				sd.sbdid = {$_POST['sbdid']}";

	$cabecalho = array('Descrição Item', 'Descrição da Nota',  'Download' );
	
	$db->monta_lista($sql,$cabecalho,50000,5,'N','90%','N');
}

// @todo acao do mais
// Caso seja a requisição ajax redireciona para a função carregaDadoItens
if( $_REQUEST['requisicao'] == 'carregaDadosSubacoes' )
{
	carregaSubacoesComContratos();
	exit();
}
if( $_REQUEST['requisicao'] == 'carregaDadosContrato' )
{
	carregaContratos();
	exit();
}
if( $_REQUEST['requisicao'] == 'carregaDadosNotas' )
{
	carregaNotas();
	exit();
}

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
?>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">
	<tr>
		<td width="100%" colspan="3" align="center"><label class="TituloTela" style="color:#000000;">Contratos e Notas de Termos</label></td>
	</tr>
	<tr>
		<td bgcolor="#e9e9e9" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b></b></td>
		<td bgcolor="#e9e9e9" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b></b></td>
		<td bgcolor="#e9e9e9" style="width:78%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ></td>
		<td bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >
			<input id="voltar" type="button" onclick="voltar()" value="Voltar" name="voltar" >
		</td>
	</tr>
</table>
<script type="text/javascript" src="js/par.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script> 
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css" />

<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<style>
	.ui-dialog-titlebar {
		text-align: center;
	}
</style>
<script>
$(document).ready(function(){

	$('.download').live('click',function(){
		window.location.href = window.location.href+'&download=S&arqid='+$(this).attr('arqid');
	});
});
</script>
<?php

$abas = Array(
			Array(	"descricao" => "Termos com Contratos", 	
					"link" => "par.php?modulo=principal/acompanhamento/ferramentas/termoContratosNotas&acao=A&aba=termocontratofirmado"),
			Array(	"descricao" => "Termos com Notas",
					"link" => "par.php?modulo=principal/acompanhamento/ferramentas/termoContratosNotas&acao=A&aba=termonotas"),
			Array(	"descricao" => "Validação de Inf de Execução Financeira",
					"link" => "par.php?modulo=principal/acompanhamento/ferramentas/termoContratosNotas&acao=A&aba=validacaoExecucaoFinanceira")
			);

if( $_REQUEST['aba'] != '' ){
	$link = "par.php?modulo=principal/acompanhamento/ferramentas/termoContratosNotas&acao=A&aba=".$_REQUEST['aba'];
}else{
	$link = "par.php?modulo=principal/acompanhamento/ferramentas/termoContratosNotas&acao=A&aba=termocontratofirmado";
}

echo montarAbasArray($abas, $link);

if( $_REQUEST['aba'] != '' ){
	include $_REQUEST['aba'].'.inc';
}else{
	include 'termocontratofirmado.inc';
}
?>