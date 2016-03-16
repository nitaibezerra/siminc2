<?php
header('content-type: text/html; charset=ISO-8859-1');

// Recupera item atual
$itemAtual 	= $_REQUEST['icoId'];
$dopid 		= $_REQUEST['dopid'];

if( !$itemAtual || !$dopid ){
	echo "
		<script>
			alert('Problemas de sessão.');
			window.close();
		</script>";
	die();
}

function bloqueiaParaConsultaEstMun()
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

//insere dados do orçamento resultado
if($_REQUEST['salvar'] == '1')
{
	$numRenavam	= $_POST['dqirenavan'];
    $itemAtual = $_REQUEST['icoId'];
    $muncod    = $_POST['municipio'];
    $arqDescricao = $numRenavam;
    
	if($_FILES['arquivo']['error'] == 0) 
	{
		if( substr($_FILES["arquivo"]["type"],0,5) == 'image' )
		{
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
			
			if($_POST['dqivistoriadopeloinmetro'])
			{
				$vistoriadoInmetro = 'TRUE';
			}
			else 
			{
				$vistoriadoInmetro = 'FALSE';
			}
			
			$campos = array("dqirenavan"         		=> "$numRenavam",
							"icoid"						=> "$itemAtual",
							"dqivistoriadopeloinmetro"	=> $vistoriadoInmetro,
							"muncod"					=> $muncod
			);

			$file = new FilesSimec("detalhamentoquantidadeitens", $campos ,"par");
			
		 	$arquivoSalvo = $file->setUpload( $arqDescricao, null, true );
			
		 	if($arquivoSalvo)
		 	{
		    	echo"<script> alert('Operação realizada com sucesso!');</script>";
		    	
		 	}
		    	
		}
		else
		{
			echo '<script type="text/javascript"> 
						alert("Tipo de arquivo inválido. \n O arquivo deve ser uma imagem. ");
				  </script>';
		}
	    
	}
	else
	{
		echo"<script> alert('Erro ao inserir arquivo!');</script>";
	}

}


if( $_GET['requisicao'] == 'excluir' )
{
	// Validacao de perfil ( sem no momento )
	if(true)
	{
		$sql = "DELETE FROM par.detalhamentoquantidadeitens WHERE arqid = {$_GET['arqid']} and dqiid = {$_GET['fotid']}";

		if($db->executar($sql))
		{
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
			$file = new FilesSimec();
			$file->excluiArquivoFisico($_GET['arqid']);
			
			$sql = "DELETE FROM public.arquivo WHERE arqid = {$_GET['arqid']}";
			$db->executar($sql);
				
		}				
		$db->commit();
		echo '<script>
				alert("Foto excluída com sucesso!");
				document.location.href = \'par.php?modulo=principal/popupAnexoDetalhamento&acao=A&icoId=' . $itemAtual .'&dopid='.$dopid.'&qtdRecebida='. $qtdRecebida .'\';
			  </script>';
	}else{
		echo '<script>
				alert("Operação não permitida!");
				document.location.href = \'par.php?modulo=principal/popupAnexoDetalhamento&acao=A&icoId=' . $itemAtual .'&dopid='.$dopid.'&qtdRecebida='. $qtdRecebida .'\';
			  </script>';
	}
	exit;
}


$caminhoAtual  = "par.php?modulo=principal/popupAnexoDetalhamento&acao=A&icoId=" . $itemAtual."&dopid=".$dopid;

$sqlQtdRecebida = "select si.icoquantidaderecebida as total from par.subacaoitenscomposicao si where icoid = {$itemAtual}";
$resultQtdRecebida = $db->carregar($sqlQtdRecebida);
$qtdRecebida = $resultQtdRecebida[0]['total'];

$sqlQtdDetalhada = "select count(icoid) as total from par.detalhamentoquantidadeitens where icoid = {$itemAtual}";
$resultQtdDetalhada = $db->carregar($sqlQtdDetalhada);

$qtdDetalhada = $resultQtdDetalhada[0]['total'];

$testaReprogramado = verificaItemEmReformulacao( $itemAtual );

$disableSalvar = false;
if( $testaReprogramado )
{
	$disableSalvar = 'Este item se encontra em reprogramação.';
}elseif( $qtdRecebida <= $qtdDetalhada ){
	$disableSalvar = 'O número máximo de detalhamento para este item já foi atingido.';
}


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





?>
<body >
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>

<script type="text/javascript" src="/includes/funcoes.js"></script>

<script>

function excluirFoto(url, arqid, fotid){
	if( boAtivo = 'S' ){
		if(confirm("Deseja realmente excluir esta foto ?")){
			window.location = url+'&fotid='+fotid+'&arqid='+arqid;
		}
	}
}


function downloadArquivo( arqid )
{
	
	window.location='?modulo=principal/popupDownloadAcompanhamento&acao=A&download=S&arqid=' + arqid
	
}

function enviar()
{

	var arq   = document.getElementById('arquivo');
	var nrenavam = document.getElementById('dqirenavan');
	var municipio = document.getElementById('municipio');
	var arqdesc = document.getElementById('arqdescricao');

	if( nrenavam.value == '' )
	{
		alert('É necessário informar um número de renavam.');
		return false;
	}
	if( municipio.value == '' )
	{
		alert('É necessário informar um município.');
		return false;
	}
	if( arq.value == '' )
	{
		alert('É necessário anexar um arquivo.');
		return false;
	}

	document.formulario.salvar.value=1;
	document.formulario.submit();
}


$(window).unload( function () { window.parent.opener.location.reload() } );
</script>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<form name=formulario id=formulario method=post enctype="multipart/form-data">
<input type="hidden" name="salvar" value="0">
<?php if( (! $disableSalvar) && ( ! bloqueiaParaConsultaEstMun()) && ( ! getFinalizado($dopid)  )  )
{?> 
	<table width="95%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTitulocentro" colspan="2">Dados do item recebido</td>		
		</tr>
	
		<tr>
			
			<td class="SubTitulocentro" >
				
			</td>
			
		</tr>
	
		<tr>
			<td > 
				<table style="width:100%" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">	
					
					<tr>
				        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Renavam:</td>
				        <td>
				        	<input type="text" style="text-align:left;" name="dqirenavan" id="dqirenavan" size="13" maxlength="11" value="" onKeyUp= "this.value=mascaraglobal('###########',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : left; width:15ex;"  title='' class='obrigatorio normal' /> <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />
				        </td>
				    </tr>
				    <tr>
					        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Fotos:</td>
				        <td width='50%'>
				            <input type="file" name="arquivo" id="arquivo"/>
				            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
				        </td>      
				    </tr>  
				     <tr>
					        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Vistoriado pelo Inmetro:</td>
				        <td width='50%'>
				            <input type="checkbox" name="dqivistoriadopeloinmetro" id="dqivistoriadopeloinmetro" >
				        </td>      
				    </tr>  
				    
				    <tr>
					    <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Municipio:</td>
				        <td width='50%'>
				    <?php
						if( ! $_SESSION['par']['muncod'] ){
				    	
					    	$estado = $_SESSION['par']['estuf'];
							$sqlMun = "select e.muncod as codigo, e.mundescricao as descricao from territorios.municipio e where e.estuf = '".$estado."' order by e.mundescricao asc";
							
							$db->monta_combo("municipio", $sqlMun, 'S', 'Selecione um Municípios', '', '', '', '', '', 'municipio' );
					            
					    }
				    	else 
				    	{
				    		$codMun = $_SESSION['par']['muncod'];
							$sqlMun = "select e.muncod as codigo, e.mundescricao as descricao from territorios.municipio e where e.muncod = '".$codMun."' order by e.mundescricao asc";
							$munAtual = $db->carregar($sqlMun);
							$munAtual = $munAtual[0]['descricao'];
							
							echo "<input type='text' style='text-align:left;' name='municipio_somente_leitura' id='municipio_somente_leitura' size='13' maxlength='11' value='{$munAtual}'  onKeyUp= 'this.value=mascaraglobal('###########',this.value);' onmouseover='MouseOver(this);' onfocus='MouseClick(this);this.select();' onmouseout='MouseOut(this);' onblur='MouseBlur(this);' style='text-align : left; width:15ex;'  title='' disabled class='normal' /> ";
							echo "<input type='hidden' name='municipio' id='municipio' value='{$codMun}' /> ";
				    	}
				    ?>
				    
				        </td>      
					 </tr>
				    <tr style="background-color: #cccccc">
				        <td>&nbsp;</td>
				        <td><input type="button" name="botao" value="Salvar"  onclick="enviar()" <?=$disableSalvar ?>></td>
				    </tr> 
				</table>
			
			
			</td>
			
		</tr>
			<tr>
				<td class="SubTitulocentro" colspan="2">Fotos</td>	
			</tr>
	<tr>
				<td colspan="2" class="SubTituloCentro">
					<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
					<script type="text/javascript" src="../includes/remedial.js"></script>
					<script type="text/javascript" src="../includes/superTitle.js"></script>
					<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
					<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
					<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
					<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
					<script type="text/javascript">
					messageObj = new DHTML_modalMessage();	// We only create one object of this class
					messageObj.setShadowOffset(5);	// Large shadow
					
					function displayMessage(url) {
						messageObj.setSource(url);
						messageObj.setCssClassMessageBox(false);
						messageObj.setSize(690,400);
						messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
						messageObj.display();
					}
					function displayStaticMessage(messageContent,cssClass) {
						messageObj.setHtmlContent(messageContent);
						messageObj.setSize(600,150);
						messageObj.setCssClassMessageBox(cssClass);
						messageObj.setSource(false);	// no html source since we want to use a static message here.
						messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
						messageObj.display();
					}
					function closeMessage() {
						messageObj.close();	
					}
	
					</script>
					
				</td>
			</tr>
		
	</table>
<?php 
} else
{?>
	<table style="width:100%" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">	
					
					<tr>
				        <td align='center' style=" background-color#DCDCDC; vertical-align:top; width:25%"><?=$disableSalvar ?></td>
				    </tr>
	
<?php 

}?>

</form>
	<?php 
		// Carrega parametros para o slideshow
					$_SESSION['imgparams'] = array("filtro" => "cnt.icoid={$itemAtual}", 
												   "tabela" => "par.detalhamentoquantidadeitens");
					//title=\"". $fotos[$k]["arqdescricao"] ."\"
					
			if( (! bloqueiaParaConsultaEstMun()) && ( ! getFinalizado($dopid)  ) )
			{			
				$btnExcluir = "'<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\" onclick=\"javascript:excluirFoto(\'" . $caminhoAtual . "&requisicao=excluir" . "\',' || arq.arqid || ',' || dqi.dqiid || ');\" >'";			
			}
			else 
			{
				$btnExcluir = "''";
			}			
								
	
			$acao = "
					'<img src=../imagens/icone_lupa.png style=cursor:pointer; id=\" ' || arq.arqid || ' \"  onclick=\"javascript:window.open(\'../slideshow/slideshow/index.php?pagina=". $_REQUEST['pagina'] ."&_sisarquivo=par&arqid=\'+this.id+\'\' , \'imagem\',\'width=850,height=600,resizable=yes\')\" >' 
					||
					{$btnExcluir}
					
					as acao
					";
	$sql = "SELECT 
					$acao,
					dqi.dqirenavan as renavam,
					CASE WHEN 
						   dqivistoriadopeloinmetro = TRUE
						THEN
							'Sim'
						ELSE
							'Não'
					END as dqivistoriadopeloinmetro,
					m.mundescricao
					
				FROM 
					public.arquivo arq 
				INNER JOIN 
					par.detalhamentoquantidadeitens dqi ON dqi.arqid = arq.arqid 
				INNER JOIN
					territorios.municipio m on dqi.muncod = m.muncod
				WHERE 
					dqi.icoid = {$itemAtual}";
		$cabecalho = array("Ações", "Renavam",  "Vistoriado pelo Inmetro", "Município" );
		$obMontaListaAjax = new MontaListaAjax($db, true);   
		$registrosPorPagina = 10; 
				            
        $obMontaListaAjax->montaLista($sql, $cabecalho,$registrosPorPagina, 10, 'N', '', '', '', '', '', '', '' );

	?>
	
<div id="debug"></div>
<script type="text/javascript">
jQuery(document).ready(function(){
    
    jQuery('#dqirenavan').change(function(){
        var cont = 0;
        var valor = jQuery(this).val();
        var msg = '';
        
        if (valor.length !== 11) {
            msg +='O Renavam deve possuir 11 números. \n';
            cont++;
        }
        
        if (valor.search(" ") !== -1){
            msg +='O Renavam não deve possuir espaços em branco. \n';
            cont++;
        }
  
        if (cont > 0) {
            alert(msg);
            jQuery(this).val('');
        }
    });
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