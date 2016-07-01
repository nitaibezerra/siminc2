<?php
header('content-type: text/html; charset=ISO-8859-1');
?>
		
<?php 
if( ! temPagamento($_SESSION['dopid']))
{
	echo "	<script>
					window.location=\"par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoContratos&dopid= {$_SESSION['dopid']}\" ;
			</script>";
}
echo'<br>';

if( $perfilTipoAcompanhamento == 3 )
{
	$arrAbas = array(
			0 => array( "descricao" => "Contratos", 					"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoContratos&dopid=" . $_SESSION['dopid'] ),
			1 => array( "descricao" => "Monitoramento", 				"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=monitoramentoContratos&dopid=" . $_SESSION['dopid']),
			2 => array( "descricao" => "Notas fiscais",					"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoNotas&dopid=" . $_SESSION['dopid']),
			3 => array( "descricao" => "Pendências/Finalizar",			"link"	  => "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=finalizarAcompanhamento&dopid=" . $_SESSION['dopid'])
	   );
}
else 
{

	$arrAbas = array(
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

echo montarAbasArray( $arrAbas, "par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=monitoramentoContratos&dopid=" . $_SESSION['dopid'] );

$habil = 'N';

if(  !verificaTermoEmReformulacao( $_SESSION['dopid'] ) ){
	$habil = 'S';
}else{
	$textTermo = '<label style="color:red">Este termo se encontra em reformulação.</label><br>';
}
?>

<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">
	<tr><td width="100%" colspan="3" align="center"><label class="TituloTela" style="color:#000000;">Monitoramento dos itens recebidos</label></td></tr>
	<tr>
		<td bgcolor="#e9e9e9" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b> <?=$local ?> </b></td>
		<td bgcolor="#e9e9e9" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b>Termo de compromisso</b></td>
		<td bgcolor="#e9e9e9" style="width:78%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" > 
			<img src="../imagens/icone_lupa.png" style="cursor:pointer;" title="Visualizar Termo" onclick="carregaTermoMinuta('<?=$dopid; ?>');" border="0">
			<?=getNumDoc( $dopid ) ?> 
		</td>
		<td bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><input id="voltar" type="button" onclick="voltar()" value="Voltar" name="voltar" ></td>
	</tr>
	<tr>
		<td colspan="4" bgcolor="#DCDCDC" align="center" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b>
		<?=$textTermo ?>
		Informar a quantidade recebida para cada um dos	itens.</b>
		</td>
	</tr>
</table>

<?php 
	function carregaDadoSubacao()
	{
		global $db;
		
		if( (! bloqueiaParaConsultaEstMun()) && (getFinalizado() != 'finalizado' ) )
		{
			$btnRepetir = "'<input disabled=\"disabled\" type=". '"' ."button". '"' .  " id= \"btnconformetermo_' || sd.sbdid ||  '\" value=". '"' ."' || 'Bens/Serviços conforme termo' || '". '"' .  " onclick=". '"' ."' || 'carregaConformeTermo(' || s.sbaid ||  ')' ||  '". '"' .  ">'";
		}
		else 
		{
			$btnRepetir = "''";
		}
		
		$sql = "select DISTINCT  
	
			CASE WHEN s.sbaid  IS NOT NULL THEN -- quer dizer que ele perdeu itens.
				'<center><img style=\"cursor:pointer\" id=\"img_subacao_' || sd.sbdid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaItensMonitoramento(this.id,\'' || sd.sbdid || '\', \'' || dp.dopid || '\' );\" border=\"0\" /></center>'
			END
			as acao,  s.sbadsc,
			{$btnRepetir} as repetir,
			'</td></tr>
	            	<tr style=\"display:none\" id=\"listaDocumentos2_' || sd.sbdid || '\" >
	            		<td id=\"trI_' || sd.sbdid || '\" colspan=8 ></td>
	            </tr>' as valor
			from par.subacao s
			inner join par.subacaodetalhe sd on sd.sbaid = s.sbaid -- and sd.sbdano = '2013' -- tabela de detalhe da subação
			inner join par.termocomposicao tc on tc.sbdid = sd.sbdid -- tabela fica a composicao do termo (os itens)
			inner join par.documentopar dp on dp.dopid = tc.dopid -- dopnumerodocumento = 4851 -- fica o termo de compromisso
			where dp.dopid = {$_SESSION['dopid']}
			AND sd.sbdid in (
				SELECT  DISTINCT
				sd.sbdid
																																							  
				from par.subacao s
				INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
				INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
				INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano AND si.icostatus = 'A'
				INNER JOIN par.subacaoitenscomposicaocontratos sicc ON  si.icoid = sicc.icoid
				INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
				LEFT JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = si.sbaid AND emi.epistatus = 'A'
				WHERE sd.sbdid in(
							select DISTINCT  
				
					CASE WHEN s.sbaid  IS NOT NULL THEN -- quer dizer que ele perdeu itens.
						sd.sbdid 
					END
				
					from par.subacao s
					inner join par.subacaodetalhe sd on sd.sbaid = s.sbaid -- and sd.sbdano = '2013' -- tabela de detalhe da subação
					inner join par.termocomposicao tc on tc.sbdid = sd.sbdid -- tabela fica a composicao do termo (os itens)
					inner join par.documentopar dp on dp.dopid = tc.dopid -- dopnumerodocumento = 4851 -- fica o termo de compromisso
					where dp.dopid = {$_SESSION['dopid']}
				
				)
					AND 
						icomonitoramentocancelado = false
			)
			
			
			";
	
		$cabecalho = array("&nbsp;Ações&nbsp;", "Descrição da Subação", "Quantidade Recebida" );
		$db->monta_lista($sql,$cabecalho,50000,5,'N','95%','S');
	
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

<?php 
		function btnSubacaoConformeTermo()
		{
			global $db;
			
			$sqlAcoes = "select DISTINCT  
				s.sbaid
				from par.subacao s
				inner join par.subacaodetalhe sd on sd.sbaid = s.sbaid -- and sd.sbdano = '2013' -- tabela de detalhe da subação
				inner join par.termocomposicao tc on tc.sbdid = sd.sbdid -- tabela fica a composicao do termo (os itens)
				inner join par.documentopar dp on dp.dopid = tc.dopid -- dopnumerodocumento = 4851 -- fica o termo de compromisso
				where dp.dopid = {$_SESSION['dopid']}";
			
			$acoes = $db->carregar($sqlAcoes);
			$functionjs = "
				function carregaConformeTermo( sbaid )
				{
					switch( sbaid )
					{
				";
			
			$acoes = is_array($acoes) ? $acoes : Array();
			
			foreach( $acoes as $k => $v )
			{
				$sqlItens = " SELECT  DISTINCT
					si.icoid,
					'repeteQtd(' || 
						
						(CASE WHEN sbacronograma = 1 THEN
				                               ( SELECT DISTINCT
				                                               CASE WHEN sic.icovalidatecnico = 'S' THEN sum(coalesce(sic.icoquantidadetecnico,0))  END as vlrsubacao
				                               FROM par.subacaoitenscomposicao sic 
				                               WHERE sic.sbaid = s.sbaid
				                               AND sic.icoano = sd.sbdano
				                               and sic.icoid = si.icoid
                                                               AND sic.icostatus = 'A'
				                               GROUP BY sic.sbaid, sic.icovalidatecnico )
				                ELSE 
				                               ( SELECT DISTINCT
				                                                               CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
				                                                                              THEN -- escolas sem itens
				                                                                                              sum(coalesce(se.sesquantidadetecnico,0))
				                                                                              ELSE -- escolas com itens
				                                                                                              CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
				                                                                                                              sum(coalesce(ssi.seiqtdtecnico,0))
				                                                                                              END
				                                                                              END
				                                                               as vlrsubacao
				                                               FROM entidade.entidade t
				                                               inner join entidade.funcaoentidade f on f.entid = t.entid
				                                               left join entidade.entidadedetalhe ed on t.entid = ed.entid
				                                               inner join entidade.endereco d on t.entid = d.entid
				                                               left join territorios.municipio m on m.muncod = d.muncod
				                                               left join par.escolas e on e.entid = t.entid
				                                               INNER JOIN par.subacaoescolas se ON se.escid = e.escid
				                                               INNER JOIN par.subacaoitenscomposicao sic on se.sbaid = sic.sbaid AND se.sesano = sic.icoano AND sic.icostatus = 'A'
				                                               LEFT JOIN  par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
				                                               WHERE sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
				                                               and sic.icoid = si.icoid
				                                               and (t.entescolanova = false or t.entescolanova is null) AND t.entstatus = 'A' and f.funid = 3 --and t.tpcid = v_tpcid
				                                               GROUP BY sic.sbaid, se.sesvalidatecnico, sic.icovalidatecnico )
				                END )
					
					|| ',' || si.icoid ||  ')' as repetir
																																								  
					from par.subacao s
					INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
					INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
					INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano AND si.icostatus = 'A'
					INNER JOIN par.subacaoitenscomposicaocontratos sicc ON  si.icoid = sicc.icoid
					INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
					LEFT JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = si.sbaid AND emi.epistatus = 'A'
					WHERE s.sbaid = ".$v['sbaid'];
				
					
				$itensFunction = $db->carregar($sqlItens);
				if( is_array($itensFunction) )
				{
					$functionjs .= "
						case {$v['sbaid']} :";
					foreach( $itensFunction as $key => $item )
					{
						$functionjs .= "
							{$item[repetir]} ;";
					}
					$functionjs .= "
						break;
					";
				}
			}
			$functionjs .= "
						default:
						  '';
					}
				}
			";
			echo $functionjs ;

		}
		btnSubacaoConformeTermo();
		?>
	
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

function enviar()
{
//	divCarregando();

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
	   		data: "requisicao=SalvarItensMonitoramento&arrParams="+arrParams,
	   		async: false,
	   		success: function( msg ){
   				alert( msg ); 
	   			window.location.href = window.location;	
	   		}
		});
	}
	else
	{
		alert("Escolha no mínimo uma subação");
	}
}
function carregarListaItensMonitoramento(idImg, sbdid, dopid){

	
	if(document.getElementById("btnconformetermo_" + sbdid ) && '<?=$habil ?>' == 'S' )
	{	
		document.getElementById("btnconformetermo_" + sbdid ).disabled = false;
	}
	var img 	 = $( '#'+idImg );
	
	var tr_nome = 'listaDocumentos2_'+ sbdid;
	var td_nome  = 'trI_'+ sbdid;

	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == "")
	{
		$('#'+td_nome).html('Carregando...');
		img.attr ('src','../imagens/menos.gif');
		carregaListaItens(sbdid, td_nome, dopid);
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

function carregaListaItens(sbdid, td_nome, dopid){
	divCarregando();
	var habil = '<?=$habil ?>';
	$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/acompanhamentoDocumentos&acao=A",
	   		data: "requisicao=carregaDadosItensMonitoramento&sbdid="+sbdid+"&dopid="+dopid+"&habil="+habil,
	   		async: false,
	   		success: function(msg){
	   			$('#'+td_nome).html(msg);
	   			divCarregado();
	   		}
		});
}

function abreEscolas(sbaid, ano, icoid, tipo){
	
	url = "par.php?modulo=principal/popupMonitoramentoEscolas&acao=A&ano=" + ano + "&sbaid=" + sbaid + "&icoid=" + icoid + "&tipo=" + tipo;
	janela(url,900,500,"Quantidade por escolas "+tipo);
	
}

</script>
	



<?php
if( (! bloqueiaParaConsultaEstMun() ) && (getFinalizado() != 'finalizado' ) )
{ 
	print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
	print '<tr><td width="100%" colspan="3" align="center"><label class="TituloTela" style="color:#000000;"><input type="button" onclick="enviar()" value="Salvar"></label></td></tr><tr>';
	print '</tr></table>';
}
?>
