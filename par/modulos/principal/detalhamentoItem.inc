<?php
header('content-type: text/html; charset=ISO-8859-1');
?>
		
<?php 
echo'<br>';
//ver($_SESSAO, d);
if( ( ! temPagamento($_SESSION['dopid'])) || ($perfilTipoAcompanhamento == 3) )
{
	echo "	<script>
					window.location=\"par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoContratos&dopid= {$_SESSION['dopid']}\" ;
			</script>";
}
if( $perfilTipoAcompanhamento == 3 )
{
	$arrAbas =	array
		(	 
			0 => array( "descricao" => "Contratos", 					"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoContratos&dopid=" . $_SESSION['dopid'] ),
			1 => array( "descricao" => "Monitoramento", 				"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=monitoramentoContratos&dopid=" . $_SESSION['dopid']),
			2 => array( "descricao" => "Notas fiscais",					"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoNotas&dopid=" . $_SESSION['dopid']),
			3 => array( "descricao" => "Pendências/Finalizar",			"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=finalizarAcompanhamento&dopid=" . $_SESSION['dopid'])
		);
}
else 
{
	$arrAbas =	array
		(	 
			0 => array( "descricao" => "Contratos", 						"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoContratos&dopid=" . $_SESSION['dopid'] ),
			1 => array( "descricao" => "Monitoramento", 					"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=monitoramentoContratos&dopid=" . $_SESSION['dopid']),
			2 => array( "descricao" => "Detalhamento do Serviço / Item",	"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=detalhamentoItem&dopid=" . $_SESSION['dopid']),
			3 => array( "descricao" => "Notas fiscais",						"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoNotas&dopid=" . $_SESSION['dopid']),
			4 => array( "descricao" => "Pendências/Finalizar",				"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=finalizarAcompanhamento&dopid=" . $_SESSION['dopid'])
		);
}
	
if( temPermissaoAnaliseFNDE() ){
	array_push($arrAbas, array( "descricao" => "Analise FNDE",				
								"link"	  	=> "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoAnaliseFNDE&dopid=" . $_SESSION['dopid']));
}

echo montarAbasArray( $arrAbas, "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=detalhamentoItem&dopid=" . $_SESSION['dopid'] );

$habil = 'N';

if(  !verificaTermoEmReformulacao( $_SESSION['dopid'] ) ){
	$habil = 'S';
}else{
	$textTermo = '<label style="color:red">Este termo se encontra em reformulação.</label><br>';
}
?>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">
	<tr><td width="100%" colspan="3" align="center"><label class="TituloTela" style="color:#000000;">Detalhamento do Serviço / Item</label></td>
	</tr>
	<tr>
		<td bgcolor="#e9e9e9" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b> <?=$local ?> </b></td>
		<td bgcolor="#e9e9e9" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b>Termo de compromisso</b></td>
		<td bgcolor="#e9e9e9" style="width:78%; FILTER: progid:DXIma 	geTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><?=getNumDoc( $dopid ) ?></td>
		<td bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><input id="voltar" type="button" onclick="voltar()" value="Voltar" name="voltar" ></td>
	</tr>
	<tr>
		<td colspan="3" bgcolor="#DCDCDC" align="center" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b>
		<?=$textTermo; ?>
		Inserir foto do ônibus e informar número do renavam.</b>
	</td>
	</tr>
</table>

<?php 
	function carregaDadoSubacao()
	{
		global $db, $habil;
		
		$sql = "    
	SELECT  DISTINCT
	si.icodescricao,
	'<center>' || si.icoquantidaderecebida  || '</center>' as icoquantidaderecebida,
	'<center>' || (select count(icoid) from par.detalhamentoquantidadeitens where icoid = si.icoid )|| '</center>'	as qtd_detalhada,								 
	CASE WHEN 
	(

		select 
									
		CASE WHEN 	( COUNT( foo.sbaid ) = COUNT(foo.onibus) ) AND ( COUNT( foo.sbaid ) > 0 ) THEN -- somente onibus
			1
		WHEN 		( COUNT( foo.sbaid ) > COUNT(foo.onibus) ) AND ( COUNT( foo.sbaid ) > 0 ) AND (COUNT(foo.onibus) > 0) THEN
			2	
		ELSE
			3
		END
		as onibus
		from (
			select   
				( SELECT  s1.sbaid  FROM par.subacao s1 WHERE s1.sbaid = sa.sbaid and s1.prgid in (81,50,153,164,158)) as onibus,
				 sa.sbaid
			from par.documentopar  dp
			inner join  par.termocomposicao tc on tc.dopid = dp.dopid
			inner join par.subacaodetalhe sd on sd.sbdid = tc.sbdid
			inner join par.subacao sa on sa.sbaid = sd.sbaid
			WHERE dp.dopid = {$_SESSION['dopid']}
			AND sa.sbaid = s.sbaid
		) as foo
		
	) = 1
	THEN
		CASE WHEN '$habil' = 'N' 
			THEN '<img src=../imagens/atencao.png style=cursor:pointer; onclick=\"alert(''Este item se encontra em reprogramação.'');\" >'
			ELSE '<input type= \"button\" onclick=\"detalharItem( ' || si.icoid || ', {$_SESSION['dopid']} )\" value=\"Detalhar\">'
		END
	ELSE
		'<center style=\" color:red; \">Não é necessário detalhar</center>'
	END as btn
																											  
	FROM par.subacao s
	INNER JOIN par.subacaodetalhe 					sd  ON sd.sbaid = s.sbaid
	INNER JOIN par.termocomposicao 					tc  ON tc.sbdid = sd.sbdid
	INNER JOIN par.subacaoitenscomposicao 			si  ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano
	INNER JOIN par.subacaoitenscomposicaocontratos 	sicc ON  si.icoid = sicc.icoid
	INNER JOIN par.propostaitemcomposicao 			pic ON pic.picid = si.picid
	LEFT JOIN par.empenhopregaoitensenviados 		emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = si.sbaid AND emi.epistatus = 'A'
	WHERE 
		s.sbaid in (
			SELECT DISTINCT  
					s.sbaid
			FROM
				par.subacao s
			inner join 
				par.subacaodetalhe sd on sd.sbaid = s.sbaid -- and sd.sbdano = '2013' -- tabela de detalhe da subação
			inner join 
				par.termocomposicao tc on tc.sbdid = sd.sbdid -- tabela fica a composicao do termo (os itens)
			inner join 
				par.documentopar dp on dp.dopid = tc.dopid -- dopnumerodocumento = 4851 -- fica o termo de compromisso
			WHERE
				dp.dopid = {$_SESSION['dopid']}
	)
	AND
		si.icoquantidaderecebida > 0
	AND
		si.icomonitoramentocancelado = FALSE
	AND
		si.icovalidatecnico = 'S'
        AND sd.sbdid = (select sbdid from par.termocomposicao where dopid = {$_SESSION['dopid']} LIMIT 1)
	";
 	//ver(simec_htmlentities($sql),d);
	$cabecalho = array("Descrição do Item", "Quantidade recebida", "Quantidade detalhada", "Detalhe do Serviço / item" );
        $db->monta_lista($sql,$cabecalho,50,5,'N','90%','N');
	
	}        
	carregaDadoSubacao();

?>
<script>

</script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
    <style type="">
    .ui-dialog-titlebar{
    text-align: center;
    }
    </style>

<div id="debug"></div>
<script type="text/javascript">

function voltar()
{
	location.href='par.php?modulo=principal/administracaoDocumentos&acao=A';
}

function anexarContrato()
{
	var arrIcoId = '';
	$('input:checkbox').each(function() {
	   if( this.checked)
	   {
		   arrIcoId =  arrIcoId + $(this).val() + '|';
	   }
	});
	if( arrIcoId != '')
	{
		window.open('par.php?modulo=principal/popupAnexoAcompanhamento&acao=A&arrIcoId=' + arrIcoId,'','width=780,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left='+ (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);  
	}
	else
	{
		alert('Selecione no mínimo 1 item');
	}	
}

function detalharItem( icoId , dopid)
{
	
	window.open('par.php?modulo=principal/popupAnexoDetalhamento&acao=A&icoId=' + icoId + "&dopid=" + dopid,'','width=780,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left='+ (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);  
	
	
}

function enviar()
{
//	divCarregando();

/*	
*/

	var arrParams = '';
	var inputs = 0;
	$( "input:text" ).each(function( index ) 
	{
		  valor = ($(this).val()) ? $(this).val() : '';
		  id = this.id;
		  arrParams +=  id + ',' + valor  + '|';
		  
		  inputs++;
	});
	if( inputs > 0 )
	{

		$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/acompanhamentoDocumentos&acao=A",
	   		data: "requisicao=SalvarDetalhamentoItem&arrParams="+arrParams,
	   		async: false,
	   		success: function(msg){
   				alert('Dados Atualizados com Sucesso!'); 
	   			window.location.href = window.location;	
	   		}
		});
	}
	else
	{
		alert("Escolha no mínimo uma subação");
	}
}
function carregarListaItensMonitoramento(idImg, sbaid){
	
	var img 	 = $( '#'+idImg );
	
	var tr_nome = 'listaDocumentos2_'+ sbaid;
	var td_nome  = 'trI_'+ sbaid;

	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == "")
	{
		$('#'+td_nome).html('Carregando...');
		img.attr ('src','../imagens/menos.gif');
		carregaListaItens(sbaid, td_nome);
	}
	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != "")
	{
		$('#'+tr_nome).css('display','');
		img.attr('src','../imagens/menos.gif');
	} else 
	{
		$('#'+tr_nome).css('display','none');
		img.attr('src','/imagens/mais.gif');
	}
}

function carregaListaItens(sbaid, td_nome){
	divCarregando();
	$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/acompanhamentoDocumentos&acao=A",
	   		data: "requisicao=carregaDadosItensMonitoramento&sbaid="+sbaid,
	   		async: false,
	   		success: function(msg){
	   			$('#'+td_nome).html(msg);
	   			divCarregado();
	   		}
		});
}

</script>
	




