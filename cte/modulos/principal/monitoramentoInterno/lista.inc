<style>
<!--
.barra1{
	text-align:left; 
	padding: 0 0 0 0; 
	margin-left:5px; 
	padding:0 0 0 0; 
	height: 12px; 
	max-height: 12px;
	width: 50px; 
	border: 1px solid #888888; 
	background-color:#FFFFFF;
}
-->
</style>			
<?php

include_once( APPRAIZ. "includes/classes/AbaAjax.class.inc" );
include_once( APPRAIZ."includes/classes/MontaListaAjax.class.inc" ); 

$obMontaListaAjax = new MontaListaAjax($db, false);	

if( $_REQUEST["requisicao"] == 'recuperarSubacoesPorPrograma' ){
	
	montarComboSubacoesPorPrograma( $_REQUEST["prgid"] );
	die;
}

if( $_REQUEST["requisicao"] == 'recuperarSubacoesPorProposicao'){
	
	if($_REQUEST["ppsid"]){		
		
		$sql = "select  pps.ppsid, pps.undid, pps.prgid, pps.ppsordem, pps.frmid, pps.ppsdsc, pps.ppsmetodologia,
					d.dimcod, d.dimdsc, ad.ardcod, arddsc, i.indcod, i.inddsc, f.frmdsc, u.undid, u.unddsc,
					case 
						when ( pps.indqtdporescola = true ) then 'Por Escola'
						else 'Global'
					end as indqtdporescola
				from cte.proposicaosubacao pps
					inner join cte.formaexecucao f on f.frmid = pps.frmid
					inner join cte.unidademedida u on u.undid = pps.undid
					inner join cte.proposicaoacao ppa on ppa.ppaid = pps.ppaid
					inner join cte.criterio c on c.crtid = ppa.crtid
					inner join cte.indicador i on i.indid = c.indid
					inner join cte.areadimensao ad on ad.ardid = i.ardid
					inner join cte.dimensao d on d.dimid = ad.dimid
				where pps.ppsid = {$_REQUEST["ppsid"]}";
	
		$arDados = $db->pegaLinha( $sql );
	
		$stDetalheSubacao = '
		<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
			<colgroup>
				<col width="25%" />
				<col width="75%" />
			</colgroup>
		
			<tbody>
				<tr>
					<td class="SubTituloDireita">Dimensão:</td>
					<td>'. $arDados['dimcod'].' - '.$arDados['dimdsc'] .'</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Área:</td>
					<td>'. $arDados['ardcod'].' - '.$arDados['arddsc'] .'</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Indicador:</td>
					<td>'. $arDados['indcod'].' - '.$arDados['inddsc'] .'</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Descricção da Subação:</td>
					<td>'. $arDados['dimcod'].'.'.$arDados['ardcod'].'.'.$arDados['indcod'].' - '.$arDados['ppsordem'].' '.$arDados['ppsdsc'] .'</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Estratégia da Implementação:</td>
					<td>'. $arDados['ppsmetodologia'] .'</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Unidade de Medida:</td>
					<td>'. $arDados['unddsc'] .'</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Forma de Execução:</td>
					<td>'. $arDados['frmdsc'] .'</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Cronograma:</td>
					<td>'. $arDados['indqtdporescola'] .'</td>
				</tr>
			</tbody>
		</table>';
		
		echo $stDetalheSubacao;
		die;
		
	} else {
		
		alert('A variável de sessão não existe, tente novamente.');
		echo "<script>document.location.reload();</script>";
		exit();
	}
}

if( $_REQUEST["mudarAba"] ){

	$arUnidades = cte_recuperarArUnidadesExigemCPF();
	$sql = "select undid from cte.proposicaosubacao where ppsid = '{$_REQUEST["ppsid"]}'";
	$undid = $db->pegaUm( $sql );
	
	if( $undid == CTE_UNIDADE_MEDIDA_UNIDADES_ESCOLARES_MUN ){

		$stSelect = "	count( distinct qfa.qfaid ) as qtdpar,
						( select sum( ees.eesentqtd )
						from cte.entidadeexecucaosubacao ees
							inner join cte.execucaosubacao es on es.exeid = ees.exeid
						where es.exeid = (
							select coalesce(es.exeid, 0) as exeqtd
							from cte.monitoramentosubacao m
								inner join cte.execucaosubacao es on es.mntid = m.mntid
								inner join cte.periodoreferencia pr on pr.perid = es.perid
							where m.sbaid = s.sbaid and pr.perano = '{$_REQUEST["mniano"]}' order by pr.perid desc limit 1
						) ) as qtdMonitoramento";
		
		$stGroupBy = "group by acao, iu.mun_estuf, m.mundescricao, qtdMonitoramento, mniqtd, atendimento, valorInvestimento";
		
	}
	elseif( in_array( $undid, $arUnidades ) ){
		
		$stSelect = "	count( cspid ) as qtdpar,
						( select count( ees.eesid )
						from cte.entidadeexecucaosubacao ees
							inner join cte.execucaosubacao es on es.exeid = ees.exeid
						where es.exeid = (
							select coalesce(es.exeid, 0) as exeqtd
							from cte.monitoramentosubacao m
								inner join cte.execucaosubacao es on es.mntid = m.mntid
								inner join cte.periodoreferencia pr on pr.perid = es.perid
							where m.sbaid = s.sbaid and pr.perano = '{$_REQUEST["mniano"]}' order by pr.perid desc limit 1
						) ) as qtdMonitoramento";
		
		$stGroupBy = "group by acao, iu.mun_estuf, m.mundescricao, qtdMonitoramento, mniqtd, atendimento, valorInvestimento, iu.inuid";
	}
	else{
		
		$stSelect = "	coalesce( spt.sptunt, 0 ) as qtdpar,
						coalesce( 
						(select coalesce(es.exeqtd, 0) as exeqtd
						from cte.monitoramentosubacao m
							inner join cte.execucaosubacao es on es.mntid = m.mntid
							inner join cte.periodoreferencia pr on pr.perid = es.perid
						where m.sbaid = s.sbaid and pr.perano = '{$_REQUEST["mniano"]}' order by pr.perid desc limit 1 ), 0
						) as qtdMonitoramento";
		 	
		$stGroupBy = '';

	}	

	$arWhere[] = "s.ppsid = '{$_REQUEST["ppsid"]}'";

	$arWhere = count( $arWhere ) ? " and ". implode( " and ", $arWhere ) : "";
	
	$sql = "SELECT estuf, muncod FROM cte.usuarioresponsabilidade WHERE usucpf = '".$_SESSION['usucpf']."'";
	$usuResponsabilidade = $db->carregar($sql);	

	if($usuResponsabilidade){
		foreach ($usuResponsabilidade as $dados){
			if($dados['estuf'] != ''){
				$usuEstuf = true;
			} elseif ($dados['muncod'] != '') {
				$usuMuncod = true;
			}
		}
	}
	
	$stInnerResponsabilidade = "";
	
	if(!verificaPerfisAdiminMonitoramentoInterno()){			

		if($usuEstuf && !$usuMuncod){
			
			$stInnerResponsabilidade = "inner join cte.usuarioresponsabilidade ur ON ur.estuf = m.estuf";
			
		} elseif (!$usuEstuf && $usuMuncod) {
			
			$stInnerResponsabilidade = "inner join cte.usuarioresponsabilidade ur ON ur.muncod = m.muncod";
				
		} elseif ($usuEstuf && $usuMuncod) {
			
			$stInnerResponsabilidade = 	"inner join cte.usuarioresponsabilidade ur ON ur.estuf = m.estuf or ur.muncod = m.muncod";
		}
		
		$stWhereResponsabilidade 	= "and ur.usucpf = '".$_SESSION['usucpf']."'";		
	}	
	
	if(verificaPerfisAdiminMonitoramentoInterno()){
		$btEnviaEmail = "<a style=\"margin: 0 -20px 0 20px;\" ><img src=\"/imagens/email.gif\" onclick=\"abrePopup(\'../geral/envia_email_equimun.php?muncod=\',673,456,' || '''' || m.muncod || '''' || ')\" border=0 title=\"Enviar e-mail\"></a>";		
	}else{
		$btEnviaEmail = "";		
	}
	
	$sqlLista = "select 	acao, estuf, mundescricao, qtdpar, qtdMonitoramento, qtdMonitoramentoInterno,
					case
						when coalesce( qtdMonitoramentoInterno, 0 ) > 0 AND coalesce(qtdPar, 0) > 0 then cast(cast(qtdMonitoramentoInterno as decimal(10,2))*100/cast(qtdPar as decimal(10,2)) as decimal(10,2))
						else 0
					end as atendimento,
	 				 
					valorInvestimento,
					inuid
			from (
				select 	distinct					 
				  	'<a style=\"margin: 0 -20px 0 20px;\" href=\"cte.php?modulo=principal/monitoramentoInterno/lista&acao=A&mniano={$_REQUEST["mniano"]}&ppsid={$_REQUEST["ppsid"]}&evento=selecionar&muncod='|| m.muncod ||'\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Monitoramento Interno\"></a>
					 <a style=\"margin: 0 -20px 0 20px;\" href=\"cte.php?modulo=lista&acao=M&evento=selecionar&muncod='|| m.muncod ||'\"><img src=\"/imagens/alterar.gif\" border=0 title=\"PAR - Plano de Metas\"></a>
					 <a style=\"margin: 0 -20px 0 20px;\" href=\"cte.php?modulo=principal/monitora_lista&acao=A&evento=selecionar&muncod='|| m.muncod ||'\"><img src=\"/imagens/consultar.gif\" border=0 title=\"Monitoramento Técnico\"></a>
					 $btEnviaEmail' as acao,
					iu.mun_estuf as estuf, 
					m.mundescricao,					
					$stSelect,
					coalesce( mniqtd, 0 ) as qtdMonitoramentoInterno, 
					0 as atendimento, 
					( ppsvaluni * mniqtd ) as valorInvestimento,
					'' as par,
					'' as monitora,
					iu.inuid as inuid
				from cte.instrumentounidade as iu
					inner join territorios.municipio m on m.muncod = iu.muncod
					$stInnerResponsabilidade
					inner join cte.pontuacao p on p.inuid = iu.inuid
					inner join cte.acaoindicador a on a.ptoid = p.ptoid
					inner join cte.subacaoindicador s on s.aciid = a.aciid
					inner join cte.proposicaosubacao pps on pps.ppsid = s.ppsid
					left  join cte.monitoramentointerno mni on mni.inuid = iu.inuid and mniano = '{$_REQUEST["mniano"]}'
					left  join cte.subacaoparecertecnico spt on spt.sbaid = s.sbaid and sptano = '{$_REQUEST["mniano"]}'
					inner join cte.monitoramentosubacao mnt on mnt.sbaid = s.sbaid
					left  join cte.execucaosubacao ex on ex.mntid  = mnt.mntid
					left  join cte.composicaopessoa cp on cp.sbaid = s.sbaid  and cspano = '{$_REQUEST["mniano"]}'
					left  join cte.qtdfisicoano qfa on qfa.sbaid = s.sbaid and qfaano = '{$_REQUEST["mniano"]}'
				where iu.itrid = 2 
				$stWhereResponsabilidade
				and p.ptostatus = 'A'
				$arWhere 
				$stGroupBy
				order by iu.mun_estuf, m.mundescricao
			) as painelMonitoramentoInterno";
			
//			$arrListas = $db->carregar($sqlLista);			
			
			$registrosPorPagina = 20;		
			
			$cabecalho = array( "Ação", "UF", "Município", "<div align='right'>PAR</div>", "<div align='right'>Monitoramento</div>", "<div align='right'>Programa</div>", "<div align='right'>% Atendimento</div>", "<div align='right'>Investimento</div>", "<center>Monitoramento</center>", "<center>PAR</center>");									
			$funcoes = array( 
								"monitora" => 'recuperarPorcentagemMonitoramentoPorInuid', 
								"par" => 'recuperarPorcentagemParPorInuid' 
							);				
			$parametrosFuncoes 	= array('inuid');			
							
			$obMontaListaAjax->montaLista($sqlLista, $cabecalho,$registrosPorPagina, 10, 'S', '', '', '', '', '', $funcoes, $parametrosFuncoes );						
			die();
}


if ( $_REQUEST['muncod'] && $_REQUEST['evento'] == 'selecionar' ) {
	
	if( cte_selecionarMunicipio( INSTRUMENTO_DIAGNOSTICO_MUNICIPAL, WF_TIPO_CTE, $_REQUEST['muncod'] ) ) {
		$db->commit();
		header( "Location: ?modulo=principal/monitoramentoInterno/cadastrarMonitoramentoInterno&acao=A&mniano={$_REQUEST["mniano"]}&ppsid={$_REQUEST["ppsid"]}" );
		exit();
	}
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba( $abacod_tela, $url, '' );
cte_montaTitulo( $titulo_modulo, '&nbsp;' );
?>
<script type="text/javascript" src="../../includes/remedial.js"></script>
<script type="text/javascript" src="../../includes/funcoes.js"></script>
<script type="text/javascript" src="../../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>
<script type="text/javascript" src="../../includes/JQuery/jquery.js"></script>
<script type="text/javascript">
	var jQuery = jQuery.noConflict();
</script>
<script type="text/javascript" src="../../includes/prototype.js"></script>
<script type="text/javascript">
	function forceInt( Text, leftZeros )
	{
		leftZeros = (leftZeros == undefined) ? false : leftZeros;
	
		Numbers = "1234567890\n";
		NewText = "";
		for( i = 0 ; i < Text.length ; i++ ){
			if( Numbers.indexOf( Text.charAt(i) ) != -1 ){
				NewText += Text.charAt(i);
			}
		}
		if (NewText == ""){
			return "";
		}
	
		if(!leftZeros){
			// a base tem de ser forcada para 10
			// porque o default ? base 8 caso NewText comece com 0
			NewText = parseInt(NewText,10);
		}
	
		return( NewText );
	}

	function abrePopup(pagina,largura,altura,muncod){	
	
		//pega a resolução do visitante
		w = screen.width;
		h = screen.height;
		
		//divide a resolução por 2, obtendo o centro do monitor
		meio_w = w/2;
		meio_h = h/2;
		
		//diminui o valor da metade da resolução pelo tamanho da janela, fazendo com q ela fique centralizada
		altura2 = altura/2;
		largura2 = largura/2;
		meio1 = meio_h-altura2;
		meio2 = meio_w-largura2;
		
		window.open(pagina + muncod,'','scrollbars=1, height=' + altura + ', width=' + largura + ', top='+meio1+', left='+meio2+'' );		
	}
	
	function pesquisar(){
		formulario.submit();
	}
	
	function excluirEscolaAtiva( inuid ){
 
		if( confirm( "Deseja realmente excluir todos os dados referentes ao Programa Escola Ativa deste Município, bem como cancelar sua adesão ao programa?" ) ){
			
			return windowOpen('/cte/cte.php?modulo=principal/escolaAtivaExclusao&acao=A&inuid=' + inuid, 'escolaAtivaExclusao',
	                          'height=200,width=300,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes'); 			
			
		}
	}
	
	function recuperarSubacoesPorPrograma( prgid ){
		jQuery('#divSubacoes').load( window.location.href + '&requisicao=recuperarSubacoesPorPrograma&prgid=' + prgid );
		jQuery('#divDetalheSubacao').html( '' );
	}
	
	function recuperarSubacoesPorProposicao( ppsid ){
		jQuery('#divDetalheSubacao').load( window.location.href + '&requisicao=recuperarSubacoesPorProposicao&ppsid=' + ppsid );
	}
	
</script>


<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<form action="" method="post" name="formulario">
					<input type="hidden" name="modulo" value="<?= $_REQUEST['modulo'] ?>"/>
					<div style="float: left;">
						
						<table border="0" cellpadding="3" cellspacing="0">
							<tr>
								<td valign="bottom">
									Programa<br/>
									<?php								
									$sql = "SELECT prgid as codigo, prgdsc as descricao 
											FROM cte.programa 
											WHERE prgmonitorainterno = true";
									
									$db->monta_combo( "prgid", $sql, 'S', 'Selecione', 'recuperarSubacoesPorPrograma', '', '', '300', '', '', '', $_REQUEST['prgid'] );
									 
									if(verificaPerfisAdiminMonitoramentoInterno()){ 
									?>
										&nbsp;<img src="/imagens/gif_inclui.gif" align="top" style="border: none" />&nbsp;<a onclick="abrePopup('cte.php?modulo=principal/monitoramentoInterno/listaProgramasMonitoraInterno&acao=A',673,520,'')" style="cursor: pointer;">Adicionar programas</a>
									<?php } ?>									
								</td>
							</tr>
							<tr>
								<td valign="bottom">
									Subação
									<br/>
									<div id="divSubacoes">
										<?php montarComboSubacoesPorPrograma( $_REQUEST["prgid"] ); ?>
									</div>
								</td>
							</tr>
						</table>
					</div>
					<div style="float: right;">
						<input type="button" name="Pesquisar" value="Pesquisar" onclick="pesquisar();"/>
					</div>
				</form>
			</td>
		</tr>
	</tbody>
</table>
<div id="divDetalheSubacao"></div>
<br />
<?php

if( $_REQUEST['ppsid'] ){
	
	echo '<script> recuperarSubacoesPorProposicao( "'. $_REQUEST['ppsid'] .'" ); </script>';	
	
	// Cria Aba Ajax	
	$obAbaAjax = new AbaAjax( null, false, false, false, false );
	
	$url = $_SERVER['REQUEST_URI'];
		  	      
	$arAnos = array( 2008, 2009, 2010, 2011 );
	$anoSelecionado = isset( $_REQUEST["mniano"] ) ? $_REQUEST["mniano"] : date( 'Y' );
	
	foreach( $arAnos as $count => $ano ){
		$arAbas[$count]['descricao'] = $ano;
		if( $anoSelecionado == $ano ){
			$arAbas[$count]['padrao'] = true;
		}
		$arAbas[$count]['url'] = $url;
		$arAbas[$count]['parametro'] = "mudarAba=true&mniano=$ano&ppsid={$_REQUEST['ppsid']}";
	}
	
	$obAbaAjax->criaAba($arAbas, 'divMonitoramentoInterno');
}

?>
<div id="divMonitoramentoInterno"></div>
<div id="divDetalharValidacao"></div>

<?php

if( !isset($_REQUEST["requisicao"]) && 
	!$_REQUEST['ppsid'] && verificaPerfisAdiminMonitoramentoInterno() ){ 

	echo '<script>
			function detalharValidacao( usucpf ){
			
				document.location.href = "?modulo=principal/monitoramentoInterno/lista&acao=A&detalhar=true&usucpf="+usucpf;
			}
			
			function validar( usucpf ){
				if(!confirm("Deseja validar estas informações?")){
					return false;
				} else {			
					document.location.href = "?modulo=principal/monitoramentoInterno/lista&acao=A&detalhar=true&usucpf="+usucpf+"&valida=true";
				}
			}			
		  </script>';
		
	include_once( APPRAIZ . "cte/classes/MonitoramentoInterno.class.inc" );
	include_once( APPRAIZ . "cte/classes/MonitoramentoInternoEntidade.class.inc" );
	
	if($_REQUEST['valida']){		
		
		$obMonitoramentoInterno = new MonitoramentoInterno();
		$obMonitoramentoInterno->validarRegistrosInseridosUsuario($_REQUEST['usucpf'], $_SESSION['usucpf']);

		alert("Informações validadas com sucesso!");
		echo "<script> document.location.href = '?modulo=principal/monitoramentoInterno/lista&acao=A';</script>";
	}	
	
	if($_REQUEST["detalhar"]) {
		
		$stCampos = "usu.usunome,
					 mi.usucpf,					 
					 ent.entnome,
					 '<div onmouseover=\"SuperTitleOn( this, \'<strong>Subação:</strong> ' || pps.ppsdsc || '<br><strong>Qtd.:</strong> ' || mi.mniqtd || '\' )\" onmouseout=\"SuperTitleOff( this )\"><a style=\"cursor: pointer;\" >' 
					 || SUBSTRING(pps.ppsdsc, 1, 55) || 
					 '</a>...</div>' 
					 as ppsdsc,
					 mun.mundescricao,
					 inu.mun_estuf, 
					 mi.mniano";
		
		$srWhere = "AND mi.usucpf = '".$_REQUEST['usucpf']."'"; 
		
	} else {
		
		 $stCampos = " '<img src=\"/imagens/consultar.gif\" onclick=\"detalharValidacao(\'' || mi.usucpf || '\');\" style=\"cursor: pointer;\" title=\"Detalhar\">
		 				<img src=\"/imagens/valida1.gif\" onclick=\"validar(\'' || mi.usucpf || '\');\" style=\"cursor: pointer;\" title=\"Validar\">' as acoes,
		 			   '<a onclick=\"detalharValidacao(\'' || mi.usucpf || '\');\" style=\"cursor:pointer;\" title=\"Detalhes\">' || usu.usunome || '</a>' as nome, 
		 			   mi.usucpf";
		 
		 $srWhere = "";
	}
	
	$sql = "SELECT DISTINCT				
				$stCampos 	
			FROM cte.monitoramentointerno mi
			INNER JOIN cte.monitoramentointernoentidade mie ON mie.mniid = mi.mniid
			INNER JOIN cte.instrumentounidade inu 			ON inu.inuid = mi.inuid
			INNER JOIN entidade.entidade ent 				ON ent.entid = mie.entid
			INNER JOIN territorios.municipio mun			ON mun.muncod = inu.muncod
			INNER JOIN cte.proposicaosubacao pps			ON pps.ppsid = mi.ppsid
			LEFT JOIN seguranca.usuario usu					ON usu.usucpf = mi.usucpf
			WHERE mi.usucpf IS NOT NULL 
			$srWhere
			AND mi.mnidtvalidacao IS NULL";			
			
	$infoValidar = $db->carregar($sql);	
	
	monta_titulo("Validar Monitoramento Interno", "");
	
	if( verificaPerfisAdiminMonitoramentoInterno() && $infoValidar[0]['usucpf'] != '' ) {
				
		if(!$_REQUEST["detalhar"]){
			
			$celAlign = array(
								"usucpf" => "left",
							 );						 
			$cabecalho = array("Ações","Nome","<div align=\"right\">CPF</div>");		
			//$db->monta_lista($infoValidar, $cabecalho, 20, 5, '', '', '', '', '', $celAlign);
			$obMontaListaAjax->montaLista($infoValidar, $cabecalho, 20, 5, 'N', '', '', '', '', '' );
			
		} else {
			
			$html = '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
						<tr>
							<td class="SubTituloDireita" width="150px">Nome:</td>
							<td>'.$infoValidar[0]['usunome'].'</td>
						</tr>
						<tr>
							<td class="SubTituloDireita">CPF:</td>
							<td>'.$infoValidar[0]['usucpf'].'</td>
						</tr>';
			
			$html .= '</table>';
			
			echo $html;
			
			for($x=0; $x<count($infoValidar); $x++){
				unset($infoValidar[$x]['usunome']);
				unset($infoValidar[$x]['usucpf']);
				unset($infoValidar[$x]['data']);
			}
	
			$celAlign = array(
								"mniqtd" => "left",
							 );			
			$cabecalho = array("Entidade", "Subação", "Município", "UF", "Ano");
			//$db->monta_lista($infoValidar, $cabecalho, 20, 5, '', '', '', '', '', $celAlign);
			$obMontaListaAjax->montaLista($infoValidar, $cabecalho,20, 5, 'N', '', '', '', '', '' );
			echo '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">	
					 <tr>
						<td class="SubTituloCentro" colspan="2">
							<input type="button" value="Validar" onclick="validar(\''.$_REQUEST['usucpf'].'\');">
							<input type="button" value="Voltar" onclick="javascrip:history.go(-1)">
						</td>
					 </tr>
				 </table>';
		}
	} else {
		echo '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">	
					 <tr>
						<td class="SubTituloCentro">
							Sem registros para validar.
						</td>
					</tr>
			  </table>';
	}

}

function montarComboSubacoesPorPrograma( $prgid ){
	
	global $db;
	
	$sql = array();
	
	if( $prgid ){
		$sql = "select 	ppsid as codigo, 
					dimcod || '.' || ardcod || '.' || indcod || ' - ' || ppsordem || ' p('|| ctrpontuacao ||') ' || ' - ' || ppsdsc as descricao, *
				from cte.proposicaosubacao pps
					inner join cte.proposicaoacao pa on pa.ppaid = pps.ppaid
					inner join cte.criterio c on c.crtid = pa.crtid
					inner join cte.indicador i on i.indid = c.indid
					inner join cte.areadimensao a on a.ardid = i.ardid
					inner join cte.dimensao d on d.dimid = a.dimid
				where prgid = '$prgid'
				and pps.ppaid is not null
				order by dimcod, ardcod, indcod, ppsordem, ppsdsc";				
	}	 

	$db->monta_combo( "ppsid", $sql, 'S', 'Selecione', 'recuperarSubacoesPorProposicao', '', '', '300', '', '', '', $_REQUEST["ppsid"] );	
}
?>