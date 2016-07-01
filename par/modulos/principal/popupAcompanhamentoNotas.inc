<?php
header('content-type: text/html; charset=ISO-8859-1');
	
// Recupera o valor dos checkboxes marcados
$strCodsItens = $_REQUEST['arrIcoId'];

$caminhoAtual  = "par.php?modulo=principal/popupAcompanhamentoNotas&acao=A&arrIcoId=" . $strCodsItens;

// Retira o "|" do final para transformar em array os valores dos checkboxes
$strCodsItens = substr_replace($strCodsItens, '', -1);

// Cria um array com os códigos dos checkboxes
$arrCodsItens = explode('|', $strCodsItens);
// Cria uma substring da passada por parametro para poder trazer as informações do BD.
$strCodsItens = str_replace('|', ", ", $strCodsItens);

if( $_GET['requisicao'] == 'excluir' )
{
	// Validacao de perfil ( sem no momento )
	if(true)
	{
		$vinculoExcluido = false;
		$sql1 = "DELETE FROM
					par.subacaoitenscomposicaonotasfiscais
				WHERE
					ntfid = {$_GET['notaid']}
				AND 
					icoid = {$_GET['icoid']}";
		
		if($db->executar($sql1))
		{
			$vinculoExcluido = true;
			$sqlCount = "
				SELECT
					count(ntfid) as nvinculos
				FROM
					par.subacaoitenscomposicaonotasfiscais
				WHERE
					ntfid = {$_GET['notaid']}
			";
			$result = $db->carregar($sqlCount);
			
			$totalVinculos = $result[0]['nvinculos'];
		}
		
		
		if( ( $vinculoExcluido ) && ($totalVinculos < 1 ) )
		{
			$sql = "DELETE FROM par.notasfiscais WHERE arqid = {$_GET['arqid']} and ntfid = {$_GET['notaid']}";
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
				alert("Nota excluída com sucesso!");
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

if($_REQUEST['download'] == 'S')
{
	
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
	
	foreach($arrSubacao as $k => $v )
	{
		$arrSubsecaoItem[$v['sbaid']] = $v;
	}
	
	$sqlItem = "
			SELECT
				distinct
				si.icodescricao, 
				s.sbaid,
				si.icoid
				
				
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
	
	foreach($arrItem as $k => $v )
	{
		$arrSubsecaoItem[$v['sbaid']]['itens'][] = $v; 
	}
	
	$tabelaSubacaoItens .= 
	"
		<table>
			<tr>
				<td colspan='2'  class='SubTituloEsquerda' >
				 	As notas fiscais serão vinculadas aos itens abaixo:
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
	
	foreach($arrSubsecaoItem as $k => $v )
	{
		$total = count($v[itens])+1;
		
		$tabelaSubacaoItens .= "
			<tr>
				<td class='SubTituloDireita' rowspan='{$total}'>
					{$v['sbadsc']}
				</td>
			</tr>";
	
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
	$tabelaSubacaoItens .= "</table>";
	
	return $tabelaSubacaoItens;
}
$cabecalho = array("&nbsp;Ações&nbsp;", "Descrição da Subação" );
//$db->monta_lista($sql,$cabecalho,50000,5,'N','95%','S');
$arrProc =$db->carregar($sql); 

// Salva
if($_REQUEST['salvar'] == '1')
{

	$numNota = str_replace(array('.', ',', '/'), '', $_POST['ntfnumeronotafiscal']);
	$valorNota = $_POST['ntfvlrnota'];
    $ntfdescricao = substr($_POST['ntfdescricao'],0,255);
    $ntfdatanota = $_POST['ntfdatanota'];
    $ntfqtditem = $_POST['ntfqtditem'];
    
	if($_FILES['arquivo']['error'] == 0) 
	{
        $arrMimeType = array("application/pdf", "application/x-pdf", "application/acrobat", "applications/vnd.pdf", "text/pdf", "text/x-pdf", "application/save", "application/force-download", "application/download", "application/x-download");
        if(in_array($_FILES['arquivo']['type'], $arrMimeType)) {
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

            $numNota 	= str_replace(' ', '', $numNota);
            $valorNota 	= str_replace('.', '', $valorNota);
            $valorNota 	= str_replace(',', '.', $valorNota);
            
            if( !validaData($ntfdatanota) ){
	            $ntfdatanota = formata_data_sql($ntfdatanota);
            }
            if( !validaData($ntfdatanota) ){
            	echo '<script>
	                    alert("Falha ao cadastrar! \nData inválida.");
	                    window.location.href = window.location.href;
	                  </script>';
            	exit();
            }

            $campos = array(
                "ntfdescricao" 	=>"'$ntfdescricao'",
                "ntfdata" => "NOW()",
                "ntfnumeronotafiscal" => $numNota,
                "ntfdatanota" => "'{$ntfdatanota}'",
                "ntfqtditem" => $ntfqtditem,
                "ntfvlrnota" => $valorNota,
                "usucpf" => "'{$_SESSION['usucpf']}'"
            );

            $file = new FilesSimec("notasfiscais", $campos ,"par");
            $file->setUpload( $ntfdescricao, null, true, 'ntfid' );
            $ntfid = $file->getCampoRetorno();
            if( $ntfid )
            {
                $sqInsert = "
                    INSERT INTO
                        par.subacaoitenscomposicaonotasfiscais
                        ( icoid, ntfid, scndata )
                    VALUES
                ";

                foreach($arrCodsItens as $k => $v)
                {
                    $sqInsert .= "
                        ( {$v}, {$ntfid} , 'NOW();'),";
                }

                $sqInsert = substr_replace($sqInsert, '', -1);

                $db->executar($sqInsert);
                $db->commit();
            }

			echo "	<script> 
						alert('Operação realizada com sucesso!');
	                    window.location.href = window.location.href;
					</script>";
			exit;
        } else {
            echo '<script>
                    alert("Falha ao cadastrar! \nSomente arquivo no formato PDF.");
                    window.location.href = window.location.href;
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

function excluirNota(url, arqid, notaid, icoid)
{
	if( boAtivo = 'S' )
	{
		if(confirm("Deseja realmente excluir esta Nota ?")){
			window.location = url+'&notaid='+notaid+'&arqid='+arqid+'&icoid='+icoid;
		}
	}
}

function downloadArquivo( arqid )
{
	
	window.location='?modulo=principal/popupAcompanhamentoNotas&acao=A&download=S&arqid=' + arqid
	
}

function enviar()
{
	var nnota = document.getElementById('ntfnumeronotafiscal');
	var arqdesc = document.getElementById('ntfdescricao');

	$('#ntfnumeronotafiscal').keyup();
	$('#ntfvlrnota').keyup();

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
	if( nnota.value == '' )
	{
		alert('É necessário informar o número da nota.');
		return false;
	}
    if( trim(jQuery('#ntfdatanota').val()) == '' )
    {
        alert('É necessário informar a Data da nota.');
        return false;
    }
    if( trim(jQuery('#ntfqtditem').val()) == '' )
    {
        alert('É necessário informar a Quantidade de itens.');
        return false;
    }
	if( arqdesc.value =='' )
	{
		alert('É necessário inserir uma descrição ao anexo.');		
		return false;
	}
	
	//location.href= window.location+'&salvar=1';
	document.formulario.salvar.value=1;
	document.formulario.submit();
}


//$(window).unload( function () { window.parent.opener.location.reload() } );
</script>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<form name=formulario id=formulario method=post enctype="multipart/form-data">
<input type="hidden" name="salvar" value="0">
<table width="95%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td class="SubTitulocentro" colspan="2">Anexar notas fiscais</td>		
	</tr>

		<tr>
		
		<td class="SubTitulocentro" >
			<?= montaTabelaItensParam();?>
		</td>
		
	</tr>
		
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Selecione as notas referentes aos itens selecionados:</td>
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
			        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Número da nota:</td>
			        <td>
			        	<input type="text" style="text-align:left;" name="ntfnumeronotafiscal" id="ntfnumeronotafiscal" size="13" maxlength="10" value="" onKeyUp= "this.value=mascaraglobal('##########',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : left; width:15ex;"  title='' class='obrigatorio normal' /> <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />
			        </td>
			    </tr>
                <tr>
                    <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Data da nota:</td>
                    <td>
                        <?=campo_data2('ntfdatanota', 'N', 'S', '', '' ); ?> <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />
                    </td>
                </tr>
                <tr>
                    <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Quantidade de itens:</td>
                    <td>
                        <input type="text" style="text-align:left;" name="ntfqtditem" id="ntfqtditem" size="13" maxlength="10" value="" onKeyUp= "this.value=mascaraglobal('##########',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : left; width:15ex;"  title='' class='obrigatorio normal' /> <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />
                    </td>
                </tr>
			    <tr>
			        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Valor da nota:</td>
			        <td>
			        	<input type="text" style="text-align:left;" name="ntfvlrnota" id="ntfvlrnota"  onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeyup="this.value=mascaraglobal('[.###],##',this.value);" value="0,00" maxlength="18" size="17" class='obrigatorio normal' /> <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />
			        	
			        </td>
			    </tr> 
			    <tr>
			        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
			        <td><?= campo_textarea( 'ntfdescricao', 'S', 'S', '', 60, 2, 250 ); ?></td>
			    </tr> 
			    <tr style="background-color: #cccccc">
			        <td>&nbsp;</td>
			        <td><input type="button" name="botao" value="Salvar" onclick="enviar()"></td>
			    </tr> 
			</table>
		
		
		</td>
		<tr>
			<td class="SubTitulocentro" colspan="2">Notas fiscais anexadas</td>	
		</tr>
	</tr>
	

	
</table>
</form>
<?php 
	
		$sql = "
		SELECT
			'<img src=../imagens/icone_lupa.png style=cursor:pointer; onclick=\"downloadArquivo('|| ntf.arqid ||');\">
			<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\" onclick=\"javascript:excluirNota(\'" . $caminhoAtual . "&requisicao=excluir" . "\',' || ntf.arqid || ',' || ntf.ntfid || ',' || si.icoid || ');\" >'
			as acao,
			si.icodescricao,
			ntf.ntfnumeronotafiscal,
			to_char(ntf.ntfdatanota, 'DD/MM/YYYY') as data,
            ntf.ntfqtditem,
			ntf.ntfvlrnota,
			ntf.ntfdescricao
		FROM
			
			par.notasfiscais ntf
			INNER JOIN par.subacaoitenscomposicaonotasfiscais scn ON scn.ntfid = ntf.ntfid
			INNER JOIN par.subacaoitenscomposicao si ON si.icoid = scn.icoid
			
		WHERE 
			si.icoid in ( {$strCodsItens} )
		";
		
        $cabecalho = array('Ação', 'Descrição do Item',  'Nº da nota' , 'Data da nota', 'Quantidade de Itens', 'Valor da nota', 'Descricão' );
        $db->monta_lista( $sql, $cabecalho, 5, 10, 'N', '', '' );
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
	   			//document.getElementById('debug').innerHTML = msg;
	   			//alert(msg);
	   		}
		});	
		divCarregado();
	});
});
</script>
</body>