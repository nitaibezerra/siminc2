<?php

include_once(APPRAIZ.'includes/classes/MontaListaAjax.class.inc');
header('content-type: text/html; charset=ISO-8859-1');
	
// Recupera o valor dos checkboxes marcados
$strCodsItens = $_REQUEST['arrIcoId'];

$arrCodItem = explode('|', $strCodsItens, -1);
$arrCodsItens = array();
if (count($arrCodItem) == 0) {
    echo "<script>alert('Selecione no mínimo 1 item para anexa contrato'); window.close(); history.back(-1)</script>";
}else if(count($arrCodItem) == 1) {
    if ($arrCodItem[0] == 0) {
        echo "<script>alert('Selecione um item válido para anexar ao contrato'); window.close(); history.back(-1)</script>";
    }else{
    	$arrCodsItens = $arrCodItem;
    }
}else {
    foreach ($arrCodItem as $item) {
        if ($item != 0) {
            $arrCodsItens[] = $item;
        }
    }
}

$caminhoAtual  = "par.php?modulo=principal/popupAnexoAcompanhamento&acao=A&arrIcoId=" . $strCodsItens;

if( $_GET['requisicao'] == 'excluir' )
{
	// Validacao de perfil ( sem no momento )
	if(true)
	{
		$vinculoExcluido = false;	
		$sql1 = "DELETE FROM
				par.subacaoitenscomposicaocontratos 
			WHERE
				conid = {$_GET['contratoid']}
			AND icoid = {$_GET['icoid']}";
		
		if($db->executar($sql1))
		{
			$vinculoExcluido = true;
			$sqlCount = "
				SELECT
					count(conid) as nvinculos
				FROM
					par.subacaoitenscomposicaocontratos 
				WHERE
					conid = {$_GET['contratoid']}
			";
			$result = $db->carregar($sqlCount);
			
			$totalVinculos = $result[0]['nvinculos'];
		}
		
		if( ($vinculoExcluido) && ($totalVinculos < 1 ) )
		{
			$sql = "DELETE FROM par.contratos WHERE arqid = {$_GET['arqid']} and conid = {$_GET['contratoid']}";
			if($db->executar($sql))
			{
				include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
				$file = new FilesSimec();
				$file->excluiArquivoFisico($_GET['arqid']);
				
				$sql = "DELETE FROM public.arquivo WHERE arqid = {$_GET['arqid']}";
				$db->executar($sql);
			}	
		}
						
		$db->commit();
		echo '<script>
				alert("Contrato excluído com sucesso!");
				document.location.href = \''.$caminhoAtual.'\';
			  </script>';
	}else{
		echo '<script>
				alert("Operação não permitida!");
				document.location.href = \''.$caminhoAtual.'\';
			  </script>';
	}
	exit;
}

// Retira o "|" do final para transformar em array os valores dos checkboxes
$strCodsItens = substr_replace($strCodsItens, '', -1);
// Cria uma substring da passada por parametro para poder trazer as informações do BD.
$strCodsItens = str_replace('|', ", ", $strCodsItens);

if($_REQUEST['download'] == 'S'){
	
	/*
    $param = array();
    $param["arqid"] = $_REQUEST['arqid'];
    DownloadArquivo($param);
    exit;
    */
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$arqid = $_REQUEST['arqid'];
	$file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = '".$_SERVER['HTTP_REFERER']."';</script>";
    exit;
  
}

function montaTabelaItensParam()
{
	global $db;
	global $strCodsItens;
	
	$sql = "
			SELECT DISTINCT
				s.sbaid,
			 	s.sbadsc
			
			FROM par.subacao s 
			INNER JOIN par.subacaodetalhe sd on sd.sbaid = s.sbaid 
			INNER JOIN par.subacaoitenscomposicao si on si.sbaid = s.sbaid
			INNER JOIN par.termocomposicao tc on tc.sbdid = sd.sbdid 
			INNER JOIN par.documentopar dp on dp.dopid = tc.dopid
			WHERE 
				si.icoid in ( {$strCodsItens} )
			ORDER BY s.sbaid
		
		";
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
	
	$tabelaSubacaoItens .= 
	"
		<table>
			<tr>
				<td colspan='2'  class='SubTituloEsquerda' >
					O contrato será vinculado aos itens abaixo:
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
if($_REQUEST['salvar'] == '1')
{
	$numContrato	= $_POST['connumerocontrato'];
	$valorContrato	= $_POST['convlrcontrato'];
    $arqDescricao	= $_POST['arqdescricao'];
    $conqtditem     = $_POST['conqtditem'];
    $condatacontrato     = $_POST['condatacontrato'];
    $concnpj	= $_POST['concnpj'];

	if($_FILES['arquivo']['error'] == 0) 
	{
        #array mime types validos
        $arrMimeType = array("application/pdf", "application/x-pdf", "application/acrobat", "applications/vnd.pdf", "text/pdf", "text/x-pdf", "application/save", "application/force-download", "application/download", "application/x-download");
        if(in_array($_FILES['arquivo']['type'], $arrMimeType)) {
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

            $numContrato = str_replace(' ', '', $numContrato);
            $valorContrato = str_replace(Array('.',',',' '),Array('','.',''), $valorContrato);
            $concnpj = str_replace(array('.','/',' ', '-'),array(''), $concnpj);
            $condatacontrato = formata_data_sql($condatacontrato);

            $campos = array(
                "condescricao" => "'$arqDescricao'",
                "condata" => "NOW()",
                "connumerocontrato" => $numContrato,
                "convlrcontrato" => $valorContrato,
                "condatacontrato" => "'{$condatacontrato}'",
                "conqtditem" => $conqtditem,
                "concnpj" => $concnpj,
                "usucpf" => "'{$_SESSION['usucpf']}'"
            );

            $file = new FilesSimec("contratos", $campos ,"par");
            $file->setUpload( $arqDescricao, null, true, 'conid' );
            $conid = $file->getCampoRetorno();

            if( $conid ) {
                $sqInsert = "
                    INSERT INTO
                        par.subacaoitenscomposicaocontratos
                        ( icoid, conid, sccdata )
                    VALUES
                ";
                foreach($arrCodsItens as $k => $v)
                {
                    $sqInsert .= "
                        ( {$v}, {$conid} , 'NOW();'),";
                }

                $sqInsert = substr_replace($sqInsert, '', -1);

                $db->executar($sqInsert);
                $db->commit();
            }
            echo '<script>
                    alert("Operação Realizada com Sucesso!");
                    document.location.href = \''.$caminhoAtual.'\';
                  </script>';
            exit();
        } else {
            echo '<script>
                    alert("Falha ao cadastrar! \nSomente arquivo no formato PDF.");
                    document.location.href = \''.$caminhoAtual.'\';
                  </script>';
		    exit();
        }
	}

}


?>
<body >
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script>

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
		
		window.location='?modulo=principal/popupDownloadAcompanhamento&acao=A&download=S&arqid=' + arqid
		
	}
	
	function enviar()
	{
		jQuery('#connumerocontrato').keyup();
		jQuery('#convlrcontrato').keyup();
		
                var arquivo = trim(jQuery('#arquivo').val());
		if( arquivo === '' )
		{
                    alert('É necessário anexar um arquivo.');
                    return false;
		} else {
                    if (arquivo.substring(arquivo.lastIndexOf('.') + 1) !== 'pdf' 
                        && arquivo.substring(arquivo.lastIndexOf('.') + 1) !== 'PDF'
                    ) {
                        alert('Somente arquivos no formato PDF.');
                        return false;
                    }
                }
		if( trim(jQuery('input[name=concnpj]').val()) == '' )
		{
                    alert('É necessário informar o CNPJ.');
                    return false;
		} else {
                    var concnpj = trim(jQuery('input[name=concnpj]').val());
                    concnpj = concnpj.replace('.', '');
                    concnpj = concnpj.replace('/', '');
                    if (!validarCnpj(concnpj)){
                        alert('CNPJ Inválido.');
                        return false;
                    }
                }
                
		if( trim(jQuery('#connumerocontrato').val()) == '' )
		{
			alert('É necessário informar o número do contrato.');
			return false;
		}
        if( trim(jQuery('#condatacontrato').val()) == '' )
        {
            alert('É necessário informar a Data do contrato.');
            return false;
        }
        if( trim(jQuery('#conqtditem').val()) == '' )
        {
            alert('É necessário informar a Quantidade de itens.');
            return false;
        }
		if( trim(jQuery('#convlrcontrato').val()) == '' || trim(jQuery('#convlrcontrato').val()) == '0,00' )
		{
			alert('É necessário informar o Valor do contrato.');
			return false;
		}
		if( trim(jQuery('#arqdescricao').val()) =='' )
		{
			alert('É necessário inserir uma descrição ao anexo.');		
			return false;
		}
		jQuery('#salvar').val('1');
		jQuery('#formulario').submit();
	}
	
// 	jQuery(window).unload( function () { window.parent.opener.location.reload() } );
</script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<form name=formulario id=formulario method=post enctype="multipart/form-data">
	<input type="hidden" name="salvar" id="salvar" value="0">
	<table width="95%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTitulocentro" colspan="2">Anexar Contratos</td>		
		</tr>
		<tr>
			<td class="SubTitulocentro" >
				<?= montaTabelaItensParam();?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda" colspan="2">Selecione os contratos referentes aos itens selecionados:</td>
		</tr>
		<tr>
			<td > 
				<table style="width:100%" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">	
				    <tr>
					        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
				        <td width='50%'>
				            <input type="file" name="arquivo" id="arquivo"/>
				            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/><br />
                                            <font face="Verdana" size="1" color="red">Somente arquivo no formato PDF.</font>
				        </td>      
				    </tr>
                    <tr>
				        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">CNPJ:</td>
				        <td> <?php echo campo_texto('concnpj', 'N', 'S', '[#]', 50, 50, '##.###.###/####-##', '', 'left', '', 0, '', '', $_REQUEST['concnpj']);?><img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /></td>
				    </tr> 
				    <tr>
				        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Número do contrato:</td>
				        <td>
				        	<input type="text" style="text-align:left;" name="connumerocontrato" id="connumerocontrato" size="13" maxlength="10" value="" onKeyUp= "this.value=mascaraglobal('##########',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : left; width:15ex;"  title='' class='obrigatorio normal' /> <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />
				        </td>
				    </tr>
                    <tr>
                        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Data do contrato:</td>
                        <td>
                            <?=campo_data2('condatacontrato', 'N', 'S', '', '' ); ?> <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />
                        </td>
                    </tr>
                    <tr>
                        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Quantidade de itens:</td>
                        <td>
                            <input type="text" style="text-align:left;" name="conqtditem" id="conqtditem" size="13" maxlength="10" value="" onKeyUp= "this.value=mascaraglobal('##########',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : left; width:15ex;"  title='' class='obrigatorio normal' /> <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />
                        </td>
                    </tr>
				    <tr>
				        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Valor do contrato:</td>
				        <td>
				        	<input type="text" style="text-align:left;" name="convlrcontrato" id="convlrcontrato" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeyup="this.value=mascaraglobal('###.###.###,##',this.value);" value="0,00" maxlength="15" size="15" class='obrigatorio normal' /> <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />
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
			<tr>
				<td class="SubTitulocentro" colspan="2">Contratos anexados</td>	
			</tr>
		</tr>
	</table>
</form>
<?php 
	
		$sql = "
		SELECT 
                    '<img src=../imagens/icone_lupa.png style=cursor:pointer; onclick=\"downloadArquivo('|| con.arqid ||');\">'
                    ||
                    '<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\" onclick=\"javascript:excluirContrato(\'" . $caminhoAtual . "&requisicao=excluir" . "\',' || con.arqid || ',' || con.conid || ',' || si.icoid || ');\" >'
                    as acao,
                    si.icodescricao,
                    coalesce(to_char(con.concnpj::bigint, 'FM00\".\"000\".\"000\"/\"0000\"-\"00'), '-'),
                    con.connumerocontrato,
                    to_char(con.condatacontrato, 'DD/MM/YYYY') as data,
                    con.conqtditem,
                    con.convlrcontrato,
                    con.condescricao
		FROM par.contratos con
                    INNER JOIN par.subacaoitenscomposicaoContratos sicon ON sicon.conid = con.conid
                    INNER JOIN par.subacaoitenscomposicao si ON si.icoid = sicon.icoid
		WHERE si.icoid in ( {$strCodsItens} )";
		
        $cabecalho = array('Ação', 'Descrição do Item', 'CNPJ',  'Nº do Contrato', 'Data do Contrato', 'Quantidade de Itens' , 'Valor do Contrato', 'Descricão' );
        
         $obMontaListaAjax = new MontaListaAjax($db, true);   
		$registrosPorPagina = 5; 
				            
        $obMontaListaAjax->montaLista($sql, $cabecalho,$registrosPorPagina, 50, 'N', '', '', '', '', '', '', '' );
?>	
	
<div id="debug"></div>
<script type="text/javascript">
jQuery(document).ready(function(){
	jQuery('#btnSalvar').click(function(){			
		var prpdocumenta = jQuery('#prpdocumenta').val();
		var pagina = jQuery('#pagina').val();
		
		if( prpdocumenta == '' ){
			alert('É obrigatório informar o número do documento!');
			jQuery('#prpdocumenta').focus();
			return false;
		}
		
		divCarregando();
		var dados = jQuery('#formularioDoc').serialize();
		jQuery.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/popupCadastraDocumenta&acao=A",
	   		data: "requisicao=salvarDocumenta&"+dados,
	   		async: true,
	   		success: function(msg){
	   			
	   			if(msg != 'erro'){
	   				window.location.href = 'par.php?modulo=principal/'+pagina+'&acao=A';
	   				closeMessage();
	   				tramitar(msg);
	   			}else{
	   				alert('Falha na operação');
	   			}
	   		}
		});	
		divCarregado();
	});
});
</script>
</body>