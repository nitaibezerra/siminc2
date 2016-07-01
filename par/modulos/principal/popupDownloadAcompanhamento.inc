<?php
header('content-type: text/html; charset=ISO-8859-1');
$icoid = $_REQUEST['icoid'];
$dopid = $_REQUEST['dopid'];

if( !empty($dopid) ){
	$_SESSION['dopid'] = $dopid;
} else {
	echo "<script>
				alert('Faltam dados da sessão. Abra novamente a tela.');
				window.close();
		</script>";
	exit();
}

if($_REQUEST['download'] == 'S'){
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$arqid = $_REQUEST['arqid'];
	$file = new FilesSimec();
	$arquivo = $file->getDownloadArquivo($arqid);
	echo"<script>window.location.href = '".$_SERVER['HTTP_REFERER']."';</script>";
	exit;

}

function bloqueiaParaConsultaEstMun()
{

	$perfil = pegaArrayPerfil($_SESSION['usucpf']);
	if (in_array(  PAR_PERFIL_CONSULTA_ESTADUAL, $perfil ) || in_array(  PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL, $perfil ) || in_array( PAR_PERFIL_CONSULTA_MUNICIPAL, $perfil ) || in_array(  PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $perfil ) || in_array( PAR_PERFIL_CONSULTA, $perfil ) )
	{
		
		return true;
	}
	else 
	{
		return false;
	}
		
}

function getFinalizado($dopid = '')
{
	
	global $db;
	
	if($dopid == '')
	{
		$dopid = $_SESSION['dopid'];	
	}
	
	$sqlFinalizado =
		"
		SELECT
			dp.dopacompanhamento
		FROM par.documentopar dp
		INNER JOIN
			(
			SELECT
				si.icoid, $dopid as dopid
			FROM
				par.subacao s
			INNER JOIN par.subacaodetalhe 					sd   ON sd.sbaid = s.sbaid
			INNER JOIN par.termocomposicao 					tc   ON tc.sbdid = sd.sbdid
			INNER JOIN par.subacaoitenscomposicao 			si   ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano
			INNER JOIN par.subacaoitenscomposicaocontratos 	sicc ON  si.icoid = sicc.icoid
			INNER JOIN par.propostaitemcomposicao 			pic  ON pic.picid = si.picid
			WHERE
				s.sbaid in (
					SELECT DISTINCT
						s.sbaid
					FROM
						par.subacao s
					INNER JOIN par.subacaodetalhe 	sd ON sd.sbaid = s.sbaid -- and sd.sbdano = '2013' -- tabela de detalhe da subação
					INNER JOIN par.termocomposicao 	tc ON tc.sbdid = sd.sbdid -- tabela fica a composicao do termo (os itens)
					INNER JOIN par.documentopar 	dp ON dp.dopid = tc.dopid -- dopnumerodocumento = 4851 -- fica o termo de compromisso
					WHERE
						dp.dopid = $dopid
					)
			) as foo ON foo.dopid = dp.dopid
		WHERE
			dp.dopid = $dopid
		";
	
// 	$sqlFinalizado = 
// 	"
// 		SELECT 
// 			dp.dopacompanhamento from par.documentopar dp  
// 		WHERE 
// 			dp.dopid = {$dopid}
// 		AND 
// 			EXISTS
// 			(
// 				SELECT  
// 					 si.icoid
// 				FROM
// 					par.subacao s
// 					INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
// 					INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
// 					INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano 
// 					INNER JOIN par.subacaoitenscomposicaocontratos sicc ON  si.icoid = sicc.icoid
// 					INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
// 				WHERE 
// 					s.sbaid in (
// 						SELECT DISTINCT  
// 								s.sbaid
// 						FROM
// 							par.subacao s
// 						inner join 
// 							par.subacaodetalhe sd on sd.sbaid = s.sbaid -- and sd.sbdano = '2013' -- tabela de detalhe da subação
// 						inner join 
// 							par.termocomposicao tc on tc.sbdid = sd.sbdid -- tabela fica a composicao do termo (os itens)
// 						inner join 
// 							par.documentopar dp on dp.dopid = tc.dopid -- dopnumerodocumento = 4851 -- fica o termo de compromisso
// 						WHERE
// 							dp.dopid = {$dopid}
// 				)

// 			)
// 	";


	$result = $db->carregar($sqlFinalizado);
	if(is_array($result) && count($result))
	{
		$acompanhando = $result[0]['dopacompanhamento'];
		
		// Finalizado
		if( $acompanhando == 'f')
		{
			return true;
			
		}
		else
		{// em acompanhamento
			
			return false;
		}
		
	}
	else
	{
		return false;
	}
	
}


$caminhoAtual  = "par.php?modulo=principal/popupDownloadAcompanhamento&acao=A&icoid=" . $icoid . "&dopid=" . $dopid;


if((!bloqueiaParaConsultaEstMun()) && (!getFinalizado($dopid))) {
    if( $_GET['requisicao'] == 'excluir' ) {
        $vinculoExcluido = false;
        $sql1 = "UPDATE par.subacaoitenscomposicaocontratos SET sccstatus = 'I' WHERE conid = {$_GET['contratoid']} AND icoid = {$_GET['icoid']}";

        if($db->executar($sql1)) {
           $sql = "UPDATE par.contratos SET constatus = 'I' WHERE arqid = {$_GET['arqid']} and conid = {$_GET['contratoid']}";
            if($db->executar($sql)) {
               $db->commit();
                echo '<script>
                            alert("Contrato excluído com sucesso!");
                            document.location.href = \''.$caminhoAtual.'\';
                      </script>';
            }	
        } else {
            echo '<script>
                        alert("Falha ao excluir Contrato!");
                        document.location.href = \''.$caminhoAtual.'\';
                  </script>';
        }
	exit;
    }
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>

<script type="text/javascript" src="/includes/funcoes.js"></script>

<script>
$(window).unload( function () { window.parent.opener.location.reload() } );
function excluirContrato(url, arqid, contratoid, icoid)
{
	if( boAtivo = 'S' )
	{
		if(confirm("Deseja realmente excluir este Contrato ?")){
			window.location = url+'&contratoid='+contratoid+'&arqid='+arqid+'&icoid='+icoid;
		}
	}
}

function downloadArquivo( arqid )
{
	
	window.location='?modulo=principal/popupDownloadAcompanhamento&acao=A&download=S&arqid=' + arqid + '&dopid=' + <?php echo $dopid ?>
	
}
</script>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<input type="hidden" name="salvar" value="0">


<?php
	$icoid = $_REQUEST['icoid'];
	
	$sqlIconDesc = "select icodescricao from par.subacaoitenscomposicao sit where sit.icoid = {$icoid}";
	
	$descIcone = $db->carregar($sqlIconDesc);
	$descIcone = $descIcone[0]['icodescricao'];
	
	
?>

<table width="95%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td class="SubTitulocentro" colspan="2">LISTA DE CONTRATOS</td>		
	</tr>
	
	<tr>
		<td class="SubTituloEsquerda" >Descrição do Item: </td>
		<td class="SubTitulocentro">
			<?=$descIcone ?>
		</td>
		
	</tr>
</table>	

<?php 
	if( !bloqueiaParaConsultaEstMun() && ( !getFinalizado($dopid)  ) && !verificaTermoEmReformulacao( $dopid ) )
	{
		$btnExcluir = "'<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\" onclick=\"javascript:excluirContrato(\'" . $caminhoAtual . "&requisicao=excluir" . "\',' || con.arqid || ',' || con.conid ||',' || si.icoid ||');\" >'";
	}
	else
	{
		$btnExcluir = "''";
	}

	$sql = "
		SELECT 
			'<img src=../imagens/icone_lupa.png style=cursor:pointer; onclick=\"downloadArquivo('|| con.arqid ||');\">' ||
			 {$btnExcluir}
			 as acao,
			con.connumerocontrato,
			con.condescricao,
			to_char(con.condatacontrato, 'DD/MM/YYYY') as data,
			con.conqtditem,
                        coalesce(con.condiligencia, '-')
		FROM
			par.contratos con
			INNER JOIN par.subacaoitenscomposicaoContratos sicon ON sicon.conid = con.conid
			INNER JOIN par.subacaoitenscomposicao si ON si.icoid = sicon.icoid
			
		WHERE 
			si.icoid = {$icoid}
			and con.constatus = 'A'
		";
		
        $cabecalho = array( 'Ação', 'Nº do Contrato' , 'Descricão', 'Data de contrato', 'Quantidade de itens', 'Diligência' );
        
        $obMontaListaAjax = new MontaListaAjax($db, true);   
		$registrosPorPagina = 10; 
				            
        $obMontaListaAjax->montaLista($sql, $cabecalho,$registrosPorPagina, 50, 'N', '', '', '', '', '', '', '' );
        
?>

<div id="debug"></div>
<script type="text/javascript">

</script>

