<?php
header('content-type: text/html; charset=ISO-8859-1');
?>
		
<?php 
echo'<br>';
if( ! temPagamento($_SESSION['dopid']))
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
	
echo montarAbasArray( $arrAbas, "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoNotas&dopid=" . $_SESSION['dopid'] );

$habil = 'N';

if(  !verificaTermoEmReformulacao( $_SESSION['dopid'] ) ){
	$habil = 'S';
}else{
	$textTermo = '<label style="color:red">Este termo se encontra em reformulação.</label><br>';
}
?>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">
	<tr><td width="100%" colspan="3" align="center"><label class="TituloTela" style="color:#000000;">Notas Fiscais</label></td></tr>
	<tr>
		<td bgcolor="#e9e9e9" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b> <?=$local ?> </b></td>
		<td bgcolor="#e9e9e9" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b>Termo de compromisso</b></td>
		<td bgcolor="#e9e9e9" style="width:78%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" > 
			<img src="../imagens/icone_lupa.png" style="cursor:pointer;" title="Visualizar Termo" onclick="carregaTermoMinuta('<?=$dopid; ?>');" border="0">
			<?=getNumDoc( $dopid ) ?> 
		</td>
		<td bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >
			<input id="voltar" type="button" onclick="voltar()" value="Voltar" name="voltar" >
		</td>
	</tr>
	<tr>
		<td colspan="4" bgcolor="#DCDCDC" align="center" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b>
		<?=$textTermo ?>
		Necessário informar as notas fiscais para os itens recebidos
                <?php
                    # Verifica Diligências de Notas Fiscais
                    $sqlNFDiligencia = "SELECT DISTINCT
                                                '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"window.location.href=\'par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoNotas&dopid='||dop.dopid||'\'\" border=\"0\">' as acao,
                                                dop.dopnumerodocumento as numero_termo,
                                                par.retornacodigosubacao(sd.sbaid) as subacao,
                                                ntf.ntfnumeronotafiscal,
                                                ntf.ntfdiligencia
                                            FROM par.processopar pp 
                                            INNER JOIN par.vm_documentopar_ativos dop ON dop.prpid = pp.prpid
                                            INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = pp.prpid AND ppc.ppcstatus = 'A'
                                            INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid 
                                            INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sd.sbaid AND sic.icostatus = 'A'
                                            INNER JOIN par.subacaoitenscomposicaonotasfiscais sicntf ON sicntf.icoid = sic.icoid AND sicntf.scnstatus = 'A'
                                            INNER JOIN par.notasfiscais ntf ON ntf.ntfid = sicntf.ntfid AND ntf.ntfstatus = 'A' AND ntf.ntfdiligencia IS NOT NULL
                                            WHERE dop.dopid = {$_REQUEST['dopid']}";
                    $arrNFDiligencia = $db->carregar($sqlNFDiligencia);
                    $arrNFDiligencia = ($arrNFDiligencia) ? $arrNFDiligencia : array();
                    
                    if ($arrNFDiligencia) {
                        echo "<br /><span style='color: #FF0000'>Existe(m) Nota(s) Fiscal(is) em diligência, destacado(s) em vermelho que deve(m) ser corrigido(s)</span>";
                    }
                ?>
                </b>
		</td>
	</tr>
</table>

<?php 
	function carregaDadoSubacao()
	{
		global $db;
		
		// Seleciona os possíveis itens da lista e sua quantidade recebida
		$sql = "    
			SELECT  DISTINCT
			si.icoid,
			si.icoquantidaderecebida,
			
			CASE WHEN 
			(
				SELECT 
											
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
				
			) in ( 1, 2)
			THEN
				'D'	
			ELSE
				''
			END as detalhamento
																																				  
			from par.subacao s
			INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
			INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
			INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano AND si.icostatus = 'A'
			INNER JOIN par.subacaoitenscomposicaocontratos sicc ON  si.icoid = sicc.icoid
			INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
			LEFT JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = si.sbaid  AND emi.epistatus = 'A'
			WHERE s.sbaid in (
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
			AND si.icoquantidaderecebida > 0
            AND sd.sbdid IN (SELECT sbdid FROM par.processoparcomposicao  WHERE prpid = (SELECT prpid FROM par.vm_documentopar_ativos WHERE dopid = {$_SESSION['dopid']} LIMIT 1))
			";
		
		$itensLista = $db->carregar($sql);
		
		// Caso existam itens na lista 
		if($itensLista)
		{
			// Verifica se a quantidade de renavans inserida é no mínimo igual a recebida
			foreach( $itensLista as $k => $v )
			{
				
				$icoid 			= $v['icoid'];
				$qtdRecebida	= $v['icoquantidaderecebida'];
				$detalhamento	= $v['detalhamento'];
				
				// Trata porque só precisa ser detalhado se for ônibus

				if($detalhamento == "D")
				{
					$sqlItem = " SELECT count(icoid) as qtd  from par.detalhamentoquantidadeitens where icoid = {$icoid} ";
					
					$qtdInseridas = $db->carregar($sqlItem);
					$qtdInseridas = $qtdInseridas[0]['qtd'];
					
					if( $qtdInseridas >= $qtdRecebida )
					{
						$itensComAnexo[] = 	$icoid;	
					}
				}
				else 
				{
					$itensComAnexo[] = 	$icoid;	
				}
			}
			if($itensComAnexo)
			{
				$itensComAnexo = implode(", ", $itensComAnexo);
			}
		}
		
		if( (! bloqueiaParaConsultaEstMun()) && ( getFinalizado() != 'finalizado' )  )
		{
			$checkButton = "CASE WHEN EXISTS  ( SELECT dps.dpsid FROM par.documentoparreprogramacaosubacao dps
										INNER JOIN par.reprogramacao rep ON rep.repid = dps.repid AND rep.repstatus = 'A'
										INNER JOIN par.vm_documentopar_ativos dp ON dp.dopid = dps.dopid
										WHERE dps.sbdid = sd.sbdid AND dps.dpsstatus = 'A' ) 
								THEN '<img src=../imagens/atencao.png style=cursor:pointer; onclick=\"alert(''Este item se encontra em reprogramação.'');\" >'
								ELSE '<input type=\"checkbox\"  id=\"' || si.icoid || '\"  value=\"' || si.icoid || '\">'
							END";
		}
		else 
		{
			$checkButton = "''";			
		}
		// Monta lista da tela
		$sqlLista = "    
		SELECT  DISTINCT
		 {$checkButton} as check,
		si.icodescricao,
		'<center>' || si.icoquantidaderecebida  || '</center>' as icoquantidaderecebida,
		(
                    array_to_string(array(
                        SELECT DISTINCT
                            CASE WHEN (nf.ntfdiligencia) IS NOT NULL 
                                THEN '<span style=\"color: #FF0000\">' || nf.ntfnumeronotafiscal || '</span>'
                                ELSE '<span>' || nf.ntfnumeronotafiscal || '</span>'
                            END
                        FROM par.subacaoitenscomposicaonotasfiscais sicnf
                        INNER JOIN par.subacaoitenscomposicao sic ON sic.icoid=sicnf.icoid AND sic.icostatus = 'A'
                        INNER JOIN par.notasfiscais nf ON nf.ntfid=sicnf.ntfid AND nf.ntfstatus = 'A'
                        WHERE sic.sbaid = s.sbaid AND sic.icoid = si.icoid AND sicnf.scnstatus = 'A'), ', ')
                ) as notas, 
		CASE WHEN
			EXISTS(	
				
				SELECT 
					scn.icoid
				FROM
					
				par.subacaoitenscomposicaonotasfiscais scn 
				INNER JOIN par.subacaoitenscomposicao sit ON sit.icoid = scn.icoid AND sit.icostatus = 'A'
			
				where scn.icoid = si.icoid
			)
		THEN
			'<img src=../imagens/icone_lupa.png style=cursor:pointer; onclick=\"listaDownloadNotas('|| si.icoid ||', {$_SESSION['dopid']} );\"' 
		ELSE
			''
		END as notas_item
																																					  
		from par.subacao s
		INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
		INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
		INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano AND si.icostatus = 'A'
		INNER JOIN par.subacaoitenscomposicaocontratos sicc ON  si.icoid = sicc.icoid
		INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
		LEFT JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = si.sbaid  AND emi.epistatus = 'A'
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
			";
		
		if($itensComAnexo)
		{
			// Caso tenha o mínimo de anexos 
			$sqlLista .= "    AND
				si.icoid in ( {$itensComAnexo} )
			
			";
		}
		else
		{
			// Não retorna nenhum registro caso nenhum dos itens não tenha o mínimo de anexos
			$sqlLista .= "    AND
				1=0
			";
		}
		
		$cabecalho = array("Ações", "Descrição do Item", "Quantidade recebida" , "Notas fiscais", "Visualizar Nota Fiscal" );
		$db->monta_lista($sqlLista,$cabecalho,200,5,'N','90%','N');
	
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

function anexarNota()
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
		window.open('par.php?modulo=principal/popupAcompanhamentoNotas&acao=A&arrIcoId=' + arrIcoId,'','width=780,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left='+ (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);  
	}
	else
	{
		alert('Selecione no mínimo 1 item');
	}	
}

function detalharItem( icoId )
{
	
	window.open('par.php?modulo=principal/popupAnexoDetalhamento&acao=A&icoId=' + icoId,'','width=780,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left='+ (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);  
	
	
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
<?php 
if( (! bloqueiaParaConsultaEstMun() ) && ( getFinalizado() != 'finalizado' ) && $habil == 'S'  )
{
	print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
	print '<tr><td width="100%" colspan="3" align="center"><label class="TituloTela" style="color:#000000;"><input type="button" onclick="anexarNota()" value="Anexar Notas"></label></td></tr><tr>';
	print '</tr></table>';
}
?>
