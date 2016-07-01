<?php
header('content-type: text/html; charset=ISO-8859-1');
	
// Recupera o valor dos checkboxes marcados
$strCodsItens = $_REQUEST['arrIcoId'];

$caminhoAtual  = "par.php?modulo=principal/popupAnexoTOR&acao=A&arrIcoId=" . $strCodsItens;

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


function testaPermissaoAnaliseTOR(){

	$arrPerfil = Array( PAR_PERFIL_SUPER_USUARIO, 
						PAR_PERFIL_ADMINISTRADOR, 
						
						PAR_PERFIL_PREFEITO,
						PAR_PERFIL_EQUIPE_MUNICIPAL,
						PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, 
						
						PAR_PERFIL_EQUIPE_ESTADUAL,
						PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,
						PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO );

	return possui_perfil( $arrPerfil );
}

function excluir()
{
    global $db;

    if( testaPermissaoAnaliseTOR() ) {
        try {
            
            $sqldelete = "DELETE FROM par.itenscomposicao_anexotor WHERE icoid = {$_GET['icoid']} AND atpid = {$_GET['atpid']}";
            $db->executar($sqldelete);
            
            $sqlInsert = "INSERT INTO par.itenscomposicao_anexotor (icoid, atpid, icausucpfexclusao, icadataalteracao) VALUES ({$_GET['icoid']}, {$_GET['atpid']}, {$_SESSION['usucpf']}, 'NOW()')";
            $db->executar($sqlInsert);

            $sql = "UPDATE par.subacaoitenscomposicao SET atpid = NULL WHERE icoid = {$_GET['icoid']}";
            $db->executar($sql);
            
            $sql = "UPDATE par.anexotorpar SET atpstatus = 'I' WHERE atpid = {$_GET['atpid']}";
            $db->executar($sql);
            
            $db->commit();
            echo "<script>
                        alert('Contrato excluído com sucesso!');
                        window.location.href = 'par.php?modulo=principal/tor/popupAnexoTOR&acao=A&arrIcoId={$_REQUEST['arrIcoId']}';
                  </script>";
        } catch (Exception $exc) {
            $db->rollback();
            echo $exc->getTraceAsString();
        }
    }else{
            echo "<script>
                            alert('Operação não permitida!');
                            window.location.href = 'par.php?modulo=principal/tor/popupAnexoTOR&acao=A&arrIcoId={$_REQUEST['arrIcoId']}';
                      </script>";
    }
    exit;
}

function download(){

	ob_clean();
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$arqid = $_REQUEST['arqid'];
	$file = new FilesSimec();
	$arquivo = $file->getDownloadArquivo($arqid);
	echo"<script>window.location.href = '".$_SERVER['HTTP_REFERER']."';</script>";
	exit;
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	die();
}

// Recupera o valor dos checkboxes marcados
$strCodsItens = $_REQUEST['arrIcoId'];
// Retira o "|" do final para transformar em array os valores dos checkboxes
$strCodsItens = substr_replace($strCodsItens, '', -1);

// Cria um array com os códigos dos checkboxes
$arrCodsItens = explode('|', $strCodsItens);
// Cria uma substring da passada por parametro para poder trazer as informações do BD.
$strCodsItens = str_replace('|', ", ", $strCodsItens);

function montaTabelaItensParam()
{
	global $db, $strCodsItens;
	
	$sql = "SELECT DISTINCT
				s.sbaid,
			 	s.sbadsc
			FROM par.subacao s 
			INNER JOIN par.subacaodetalhe sd on sd.sbaid = s.sbaid 
			INNER JOIN par.subacaoitenscomposicao si on si.sbaid = s.sbaid
			INNER JOIN par.termocomposicao tc on tc.sbdid = sd.sbdid 
			INNER JOIN par.documentopar dp on dp.dopid = tc.dopid
			WHERE 
				si.icoid in ( {$strCodsItens} )
			ORDER BY s.sbaid";
	$arrSubacao = $db->carregar($sql);
	
	if(is_array($arrSubacao) && count($arrSubacao))
	{
		foreach($arrSubacao as $k => $v )
		{
			$arrSubsecaoItem[$v['sbaid']] = $v;
		}
	}
	$sqlItem = "
			SELECT DISTINCT
				s.sbaid,
				si.icoid,
				si.icodescricao
				
			FROM
				par.subacao s -- subação
			INNER JOIN par.subacaodetalhe sd on sd.sbaid = s.sbaid -- and sd.sbdano = '2013' -- tabela de detalhe da subação
			INNER JOIN par.subacaoitenscomposicao si on si.sbaid = s.sbaid  -- and si.icoano = '2013' -- tabela de itens
			INNER JOIN par.termocomposicao tc on tc.sbdid = sd.sbdid -- tabela fica a composicao do termo (os itens)
			INNER JOIN par.documentopar dp on dp.dopid = tc.dopid -- dopnumerodocumento = 4851 -- fica o termo de compromisso
			WHERE 
				si.icoid in ( {$strCodsItens} )
			ORDER BY 
				s.sbaid
		" ;
	
	$arrItem =$db->carregar($sqlItem);
	
	if( is_array($arrItem) && count($arrItem) )
	{
		foreach($arrItem as $k => $v )
		{
			$arrSubsecaoItem[$v['sbaid']]['itens'][] = $v; 
		}
	}
	
	$tabelaSubacaoItens .="
<table>
	<tr>
		<td colspan='2'  class='SubTituloEsquerda' >
			O TR será vinculado aos itens abaixo:
		</td>
	</tr>
	<tr>
		<td  class='SubTituloEsquerda' width='20%' >
			Subação:
		</td>
		<td  class='SubTituloEsquerda' >
			Itens:
		</td>
	</tr>
	";
	if( is_array($arrSubsecaoItem) && count($arrSubsecaoItem) )
	{
		foreach($arrSubsecaoItem as $k => $v )
		{
			$total = count($v[itens])+1;
			
			$tabelaSubacaoItens .= "
				<tr>
					<td class='SubTituloDireita' rowspan='{$total}'>
						{$v['sbadsc']}
					</td>
				</tr>";
		
			if( is_array($v[itens]) && count($v[itens]) )
			{			
				foreach( $v[itens] as $key => $value )
				{
					$tabelaSubacaoItens .= "
					<tr style='background-color: #cccccc'>
						<td >
							{$value['icodescricao']}
						</td>
					</tr>";
				}
			}
		}
	}
	$tabelaSubacaoItens .= "</table>";
	
	return $tabelaSubacaoItens;
}

$cabecalho = array("&nbsp;Ações&nbsp;", "Descrição da Subação" );
//$db->monta_lista($sql,$cabecalho,50000,5,'N','95%','S');
$arrProc =$db->carregar($sql); 

//insere dados do orçamento resultado
if($_REQUEST['salvar'] == '1'){
    if( testaPermissaoAnaliseTOR() ){
        $arqDescricao	= $_POST['arqdescricao'];
	    
        if($_FILES['arquivo']['error'] == 0) {	
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
            $campos = array(
                "atpdsc" =>"'$arqDescricao'",
                "atpdatacriacao" => "NOW()",
                "dopid" => $_SESSION['dopid'],
                "usucpf" => "'{$_SESSION['usucpf']}'"
            );
	
            $file = new FilesSimec("anexotorpar", $campos ,"par");
            $file->setUpload(substr($arqDescricao, 0, 255), null, true, 'atpid');
            $atpid = $file->getCampoRetorno();

            if($atpid){
                require_once APPRAIZ ."includes/workflow.php";
                $docid = wf_cadastrarDocumento( 179, "TR documento {$_SESSION['dopid']}" );
                
		$sql = "UPDATE par.anexotorpar SET docid = $docid WHERE atpid = $atpid;";
                $db->executar($sql);
                $db->commit();
                                
                $sql = "";
                foreach( $arrCodsItens as $icoid ){
                    #Inativa Anexo Tor Ativo Diferente no novo adicionado
                    $sqlUpdate = "UPDATE par.anexotorpar SET atpstatus = 'I' WHERE atpid IN (SELECT 
                                        icat.atpid 
                                    FROM par.itenscomposicao_anexotor icat
                                    INNER JOIN par.anexotorpar atp ON atp.atpid = icat.atpid 
                                    WHERE icat.icoid = {$icoid} AND atp.atpid <> $atpid)";
                    $db->executar($sqlUpdate);
                    
                    #Insere vinculo de Item composicao com anexo Tor
                    $sqlInsert = "INSERT INTO par.itenscomposicao_anexotor (icoid, atpid, icausucpfexclusao, icadataalteracao) VALUES ({$icoid}, {$atpid}, {$_SESSION['usucpf']}, 'NOW()')";
                    $db->executar($sqlInsert);

                    $sqlTeste = "SELECT 
                                    true 
                                FROM par.subacaoitenscomposicao ico
                                INNER JOIN par.anexotorpar 		atp ON atp.atpid = ico.atpid
                                LEFT  JOIN workflow.documento 	doc ON doc.docid = atp.docid
                                WHERE icoid = $icoid
                                    AND esdid = 1097;";

                    $teste = $db->pegaUm( $sqlTeste );
                    if( $teste != 't' ){
                        $sql .= "UPDATE par.subacaoitenscomposicao SET atpid = $atpid WHERE icoid = $icoid;";
                    }
                }
                if($sql != ''){
                    $db->executar($sql);
                    $db->commit();
                }else{
                    echo"<script> alert('Nenhum Item pode receber um Termo!'); window.location.href = window.location.href;</script>";
                    return false;
                }
            }
            echo"<script> alert('Operação realizada com sucesso!'); window.location.href = window.location.href;</script>";
        }
    }else{
        echo "<script>
                alert('Operação não permitida!');
                window.location.href = window.location.href;
            </script>";
    }
}
?>
<body >
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>

<script type="text/javascript" src="/includes/funcoes.js"></script>

<script>
function excluirContrato( icoid, atpid )
{
    if(confirm("Deseja realmente excluir este TR?")){
        window.location.href = window.location.href+'&req=excluir&icoid='+icoid+'&atpid='+atpid;
    }
}
function downloadArquivo( arqid )
{
	
	window.location = window.location.href+'&req=download&arqid=' + arqid
	
}

function enviar()
{

	var arq   = document.getElementById('arquivo');
	var arqdesc = document.getElementById('arqdescricao');

	if( arq.value == '' )
	{
		alert('É necessário anexar um arquivo.');
		return false;
	}
	if(arqdesc.value =='' )
	{
		alert('É necessário inserir uma descrição ao anexo.');		return false;
	}
	
	//location.href= window.location+'&salvar=1';
	document.formulario.salvar.value=1;
	document.formulario.submit();
}

function wf_exibirHistorico( docid )
{
	var url = 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/historico.php' +
		'?modulo=principal/tramitacao' +
		'&acao=C' +
		'&docid=' + docid;
	window.open(
		url,
		'alterarEstado',
		'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
	);
}

// $(window).unload( function () { window.parent.opener.location.reload() } );

</script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<form name=formulario id=formulario method=post enctype="multipart/form-data">
	<input type="hidden" name="salvar" value="0">
	<table width="95%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<?php 
			$bloqueiaParaConsultaEstMun = getBloqueiaParaConsultaEstMun();
			if( ! $bloqueiaParaConsultaEstMun)
			{
			
		?>
		
		<tr>
			<td class="SubTitulocentro" colspan="2">Anexar Termos de Referência</td>		
		</tr>
		<tr>
			<td class="SubTitulocentro" >
				<?=montaTabelaItensParam();?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda" colspan="2">Selecione os TRs referentes aos itens selecionados:</td>
		</tr>
		<tr>
			<td > 
				<table style="width:100%" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">	
				    <tr>
					        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
				        <td width='50%'>
				            <input type="file" name="arquivo" id="arquivo"/>
				            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
				        </td>      
				    </tr>
				    <tr>
				        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
				        <td><?= campo_textarea( 'arqdescricao', 'S', 'S', '', 60, 2, 250 ); ?></td>
				    </tr>   
				    <tr style="background-color: #cccccc">
				        <td>&nbsp;</td>
				        <td><input type="button" name="botao" value="Salvar" onclick="enviar()"></td>
				    </tr> 
				</table>
			
			
			</td>
		</tr>
		
		<?php 
			}
		?>
		<tr>
			<td class="SubTitulocentro" colspan="2">Termos de referência anexados</td>	
		</tr>
	</table>
</form>
<?php 

	if($bloqueiaParaConsultaEstMun)
	{
		$excluir = "''";
	}
	else
	{
		$excluir = "'<center><img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirContrato('|| ico.icoid ||', '||atp.atpid||');\" /></center>'";		
	}
	$sql = "SELECT
                    CASE WHEN esdid = 1097 
                        THEN '<center><img width=\"20px\" src=../imagens/check_checklist.png style=cursor:pointer; tittle=Aprovado onclick=\"wf_exibirHistorico('||doc.docid||')\" /></center>' --97
                            ELSE CASE WHEN atp.atpstatus = 'A' THEN {$excluir} ELSE '<center>-</center>' END
                    END as excluir,
                    ico.icodescricao,
                    atp.atpdsc,
                    TO_CHAR(atp.atpdatacriacao, 'DD/MM/YYYY') as dt_inclusao,
                    CASE WHEN atpstatus = 'A' THEN 'Ativo' ELSE 'Inativo' END AS situacao,
                    '<center><img src=../imagens/icone_lupa.png style=cursor:pointer; onclick=\"downloadArquivo('|| atp.arqid ||');\" /></center>' as download 
                FROM par.itenscomposicao_anexotor icat
                LEFT JOIN par.subacaoitenscomposicao ico ON ico.icoid = icat.icoid
                INNER JOIN par.anexotorpar atp ON atp.atpid = icat.atpid 
                INNER JOIN workflow.documento doc ON doc.docid = atp.docid
                WHERE ico.icoid in ( {$strCodsItens} )
                ORDER BY atpdatacriacao desc";
//		ver(simec_htmlentities($sql), d);
	$cabecalho = array('&nbsp;', 'Descrição do Item',  'Descricão do TR' , 'Data Inclusão', 'Situação', 'Baixar TR' );
	$obMontaListaAjax = new MontaListaAjax($db, true);   
	$registrosPorPagina = 5; 
				            
	$obMontaListaAjax->montaLista($sql, $cabecalho,$registrosPorPagina, 50, 'S', '', '', '', '', '', '', '' );
?>	