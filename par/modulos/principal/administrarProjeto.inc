<?php

if( $_SESSION['par']['inuid'] == '' ){
	echo "
		<script>
			alert('Erro de sessão.');
			window.location.href = 'par.php?modulo=principal/listaMunicipios&acao=A';
		</script>";
	die();
}

include_once APPRAIZ.'par/classes/Projeto.class.inc';
$obProjeto = new Projeto( $_REQUEST );
header('content-type: text/html; charset=ISO-8859-1');

if( $_REQUEST['requisicao'] == 'inativaProcessoPar' ){
	ob_clean();
	$obProjeto->inativaProcessoPar( $_REQUEST['proid'], $_REQUEST['nprocesso'] );
	exit();
}

if( $_REQUEST['requisicao'] == 'inativaProcessoPac' ){
	ob_clean();
	$obProjeto->inativaProcessoPac( $_REQUEST['proid'], $_REQUEST['nprocesso'] );
	exit();
}

if( $_REQUEST['requisicao'] == 'inativaProcessoParObras' ){
	ob_clean();
	$obProjeto->inativaProcessoParObras( $_REQUEST['proid'], $_REQUEST['nprocesso'] );
	exit();
}

if( $_REQUEST['requisicao'] == 'cancelarProcesso' ){
	$obProjeto->cancelarProcesso();
	exit();
}

if( $_REQUEST['requisicaoFinalizar'] == 'finaliza' ){
	finalizaProcesso();
	exit();
}

if( $_REQUEST['requisicao'] == 'carregaDadosSubacao' ){
	carregaDadoSubacao();
	exit();
}

if( $_REQUEST['requisicao'] == 'carregaDadosSubacaoObrasPar' ){
	carregaDadoSubacaoObrasPar();
	exit();
}

if( $_REQUEST['requisicao'] == 'carregaDadosSubacaoObras' ){
	carregaDadoSubacaoObras();
	exit();
}

if( $_REQUEST['requisicao'] == 'carregaDadosItens' ){
	carregaDadoItens();
	exit();
}

if( $_REQUEST['requisicao'] == 'carregaDadosObrasPar' ){
	carregaDadoObrasPar();
	exit();
}

if( $_REQUEST['requisicao'] == 'aderirPregao' ){
	include_once(APPRAIZ."par/classes/WSSigarp.class.inc");
	$oWSSigarp = new WSSigarp();
	$oWSSigarp->aderirPregao($_REQUEST);
	exit();
}

if( $_REQUEST['requisicao'] == 'historicoReformulacao' ){

ob_clean();	
$prpidLog = $_REQUEST['prpid'];
if($prpidLog)
{
	$sql = " SELECT
				'<img src=\"../imagens/icone_lupa.png\" style=\"cursor:pointer;\" onclick=\"abreProtocolo('||hsr.hsrid||');\" border=\"0\">' as acao, 
				hsrprotocolo,
				to_char(hsrdata, 'DD/MM/YYYY HH24:MI') as data_solicitacao,
				usucpf || ' - ' || usunome as nome_usuario,
				CASE WHEN hsrtipo = 'S' THEN
					'Subação'
				ELSE
					'Prazo'
				END AS tipo,
				CASE WHEN hsrtipo = 'S' THEN
					array_to_string( 
						array(SELECT par.retornacodigosubacao(s.sbaid) || ' - ' ||  sbadsc  FROM par.historicosolicitacaoreprogramacaosubacao hrs
							INNER JOIN par.subacao s ON s.sbaid = hrs.sbaid where hsr.hsrid = hrs.hsrid )
							, '</br>' 
					) 
				ELSE
					''
				END
				as subacoes,
				(SELECT
					CASE WHEN dp2.dopano::boolean THEN
					dp.dopnumerodocumento::text || '/' || dp2.dopano::text
				ELSE
					dp.dopnumerodocumento::text
				END
					as ndocumento
				FROM 
					par.documentopar dp
			
				LEFT JOIN par.documentopar dp2 ON dp2.dopid = dp.dopnumerodocumento
				WHERE dp.dopid = hsr.dopid) as numero_doc,
				
				hsrjustificativa as justificativa
			
			FROM par.historicosolicitacaoreprogramacao hsr
			INNER JOIN seguranca.usuario u ON hsr.hsrcpf = u.usucpf where  prpid = {$prpidLog}";
		echo "	<table align=\"center\" cellspacing=\"0\" cellpadding=\"5\" border=\"0\" bgcolor=\"#DCDCDC\" style=\"border-top: none; border-bottom: none;\" class=\"tabela\">
				<tr>
					<td align=\"center\" bgcolor=\"white\"><b>Histórico de Reprogramação/Prorrogação</b>
					</td>
				</tr>
			</table>";
		$cabecalho = array("Ação", "Nº Protocolo", "Data Solicitação", "CPF/Nome Solicitante", "Tipo", "Subações", "Nº do Documento", "Justificativa" );
		$db->monta_lista($sql,$cabecalho,50000,5,'N','90%','N');
}
exit();
}

if( $_REQUEST['requisicao'] == 'aderirPregao1' ){
	include_once(APPRAIZ."par/classes/WSSigarp.class.inc");
	$oWSSigarp = new WSSigarp();
	$oWSSigarp->aderirPregao($_REQUEST, 1, 'processo');
	exit();
}

if( $_REQUEST['requisicao'] == 'carregaDadoItensReformular' ){
	carregaDadoItensReformular();
	exit();
}


function carregaDadoItensReformular(){
	global $db;
	
	$arDetalhe = $db->pegaLinha("select sd.sbaid, sd.sbdano from par.subacaodetalhe sd where sd.sbdid = {$_POST['sbdid']}");
	
	$cronograma = $db->pegaUm( "SELECT sbacronograma FROM par.subacao WHERE sbaid = ".$arDetalhe['sbaid'] );
	
	if( $cronograma == 1 ){ // GLOBAL
	
	$sql = "SELECT 
				sic.icodescricao,
				sic.icoquantidadetecnico as quantidade,
				TO_CHAR( sic.icovalor, '999G999G999D99'),
				TO_CHAR( ( sic.icoquantidadetecnico * sic.icovalor), '999G999G999D99') as valortotal
			FROM
				par.subacaoitenscomposicao sic
			WHERE
				sic.icovalidatecnico = 'S' AND
				sic.icoano = '".$arDetalhe['sbdano']."' and
				sic.sbaid = ".$arDetalhe['sbaid'];

	} else { // ESCOLA
		
	$sql = "SELECT 
				sic.icodescricao,
				par.recuperaquantidadeitemvalidado(sic.icoid) as quantidade,
				TO_CHAR( sic.icovalor, '999G999G999D99'),
				TO_CHAR(SUM( sse.seiqtdtecnico * sic.icovalor ), '999G999G999D99') as valortotal
			FROM
				par.subacaoitenscomposicao sic
			INNER JOIN par.subescolas_subitenscomposicao  sse ON sse.icoid = sic.icoid
			WHERE
				sic.icovalidatecnico = 'S' AND
				sic.sbaid = ".$arDetalhe['sbaid']." and
				sic.icoano = '".$arDetalhe['sbdano']."'
			GROUP BY
				sic.icodescricao, sic.icovalor, sic.icoquantidadetecnico, sic.icoid";

	}
	
	$cabecalho = array("Descrição do Item", "Quantidade", "Valor", "Valor Total" );
	$db->monta_lista($sql,$cabecalho,50000,5,'N','90%','N');

}

function pegaCnpj($inuid, $prpid){
	global $db;
	$cnpj = $db->pegaUm("SELECT prpcnpj FROM par.processopar WHERE prpid = ".$prpid);
	if( $cnpj ){
		return $cnpj;
	} else {
		return $db->pegaUm("SELECT iue.iuecnpj 
		                     FROM par.instrumentounidade iu
		                     inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
		                     WHERE
		                     	iu.inuid = {$inuid}
		                     	and iue.iuestatus = 'A'
		                        and iue.iuedefault = true");
	}
}

function finalizaProcesso(){
	global $db;
	
	// PAR
	if( $_POST['tipo'] == 1 ){
		$sql = "UPDATE par.processopar SET prpfinalizado = true WHERE prpid = ".$_POST['processo'];
	}
	// Obras do PAR
	if( $_POST['tipo'] == 2 ){
		$sql = "UPDATE par.processoobraspar SET profinalizado = true WHERE proid = ".$_POST['processo'];
	}
	// Obras do PAC
	if( $_POST['tipo'] == 3 ){
		$sql = "UPDATE par.processoobra SET profinalizado = true WHERE proid = ".$_POST['processo'];
	}
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
				alert('Processo finalizado com sucesso'); 
				window.location.href = window.location;
		</script>";
	die();
}


function carregaDadoSubacao(){
	global $db;
	
	$sql = "SELECT DISTINCT
                                '<img style=\"cursor:pointer\" title=\"Visualizar Subação\" src=\"/imagens/icone_lupa.png\" onclick=\"janela(\'par.php?modulo=principal/subacao&acao=A&sbaid=' || sd.sbaid || '\',800,600,\'Subação\');\" border=\"0\" />' || (
				CASE WHEN epi.prpid IS NOT NULL THEN -- quer dizer que ele perdeu itens.
					'' --'<center><img style=\"cursor:pointer\" id=\"img_subacao_' || sd.sbdid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaItens(this.id,\'' || sd.sbdid || '\');\" border=\"0\" />&nbsp;<a href=\"javascript:aderirPregao(\'' || sd.sbdid || '\', \'' || pp.inuid || '\', \'' || ppc.prpid || '\')\" ><span title=\"Aderir ao pregão\" class=\"glyphicon glyphicon-transfer\" style=\"color:#228B22;font-size:15px\"></span></a></center>'
				ELSE 
				--	CASE WHEN s.prgid = 153 THEN
				--		'<center><img style=\"cursor:pointer\" id=\"img_subacao_' || sd.sbdid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaItens(this.id,\'' || sd.sbdid || '\');\" border=\"0\" />&nbsp;<a href=\"javascript:aderirPregao(\'' || sd.sbdid || '\', \'' || pp.inuid || '\', \'' || ppc.prpid || '\')\" ><span title=\"Aderir ao pregão\" class=\"glyphicon glyphicon-transfer\" style=\"color:#228B22;font-size:15px\"></span></a></center>'
				--	ELSE
						'<center><img style=\"cursor:pointer\" id=\"img_subacao_' || sd.sbdid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaItens(this.id,\'' || sd.sbdid || '\');\" border=\"0\" /></center>'
				--	END
				END ) AS acao,
				s.sbadsc,
				sd.sbdano||'&nbsp;' as sbdano,
				TO_CHAR( (SELECT par.recuperavalorvalidadossubacaoporano(sd.sbaid, sd.sbdano)), '999G999G999D99') || '</td></tr>
								            	<tr style=\"display:none\" id=\"listaDocumentos2_' || sd.sbdid || '\" >
								            		<td id=\"trI_' || sd.sbdid || '\" colspan=8 ></td>
								            </tr>' as valor
			FROM 
				par.processoparcomposicao ppc
			INNER JOIN par.processopar pp ON pp.prpid = ppc.prpid AND pp.prpstatus = 'A'
			INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
			INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
			LEFT JOIN par.empenhosubacao ems ON ems.sbaid = sd.sbaid AND ems.eobano = sd.sbdano and eobstatus = 'A' 
			LEFT JOIN  par.empenhopregaoitemperdido epi ON epi.sbdid = sd.sbdid AND epi.prpid = ppc.prpid
			WHERE
				 ppc.ppcstatus = 'A' and
				ppc.prpid = ".$_POST['prpid']
				
				;

	$cabecalho = array("&nbsp;Ações&nbsp;", "Descrição da Subação", "Ano da Subação", "Valor da Subação" );
	$db->monta_lista($sql,$cabecalho,50000,5,'N','95%','S');

}

function carregaDadoSubacaoObrasPar(){
	global $db;
	
	/*$sql = "SELECT DISTINCT
                                '<center><img style=\"cursor:pointer\" title=\"Visualizar Subação\" src=\"/imagens/icone_lupa.png\" onclick=\"janela(\'par.php?modulo=principal/subacao&acao=A&sbaid=' || sd.sbaid || '\',800,600,\'Subação\');\" border=\"0\" />'
                                ||
				'<img style=\"cursor:pointer\" id=\"img_subacao_' || sd.sbaid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaObrasPar(this.id,\'' || sd.sbdid || '\');\" border=\"0\" /></center>' as acao,
				s.sbadsc,
				sd.sbdano||'&nbsp;' as sbdano,
				TO_CHAR( SUM(pre.prevalorobra), '999G999G999D99') || '</td></tr>
								            	<tr style=\"display:none\" id=\"listaDocumentos5_' || sd.sbdid || '\" >
								            		<td id=\"trO5_' || sd.sbdid || '\" colspan=8 ></td>
								            </tr>' as valor
			FROM 
				par.processoobrasparcomposicao ppc
			INNER JOIN par.subacaoobra so ON so.preid = ppc.preid
			INNER JOIN par.subacaodetalhe sd ON sd.sbaid = so.sbaid AND sd.sbdano = so.sobano
			INNER JOIN par.subacao s ON s.sbaid = sd.sbaid AND s.sbastatus <> 'C'
			INNER JOIN obras.preobra pre ON pre.preid = ppc.preid
			WHERE
				ppc.proid = ".$_POST['proid']."
				and ppc.pocstatus = 'A'
			GROUP BY
				sd.sbaid,
				sd.sbdid,
				s.sbadsc,
				sd.sbdano"
				//$cabecalho = array("&nbsp;Ações&nbsp;", "Descrição da Subação", "Ano da Subação", "Valor da Subação" );
				*/

	$sql =
		"select DISTINCT
			'<img style=\"cursor:pointer\" title=\"Visualizar Obra\" src=\"/imagens/icone_lupa.png\" onclick=\"janela(\'par.php?modulo=principal/subacaoObras&acao=A&sbaid=' || sba.sbaid || '&ano='||sobano||'&preid='||ppc.preid||'\',800,600,\'Subação Obra\');\" border=\"0\" />'
			|| '&nbsp; <a href=\'#\'> <span title=\"Visualizar Subação\" style=\"font-size:15px; color:#228B22;\" onclick=\"janela(\'par.php?modulo=principal/subacao&acao=A&sbaid=' || sba.sbaid || '&anoatual='||sobano||'\',800,600,\'Subação Obra\');\" class=\"glyphicon glyphicon-pushpin\"></span></a>'				
			as acao,
			pre.predescricao,
			pto.ptodescricao,
			sobano::text || ' ' as ano,
			prevalorobra
		
		FROM 
			par.processoobrasparcomposicao ppc
			LEFT JOIN par.subacaoobra  so ON so.preid = ppc.preid
			LEFT JOIN par.subacao sba 	ON so.sbaid = sba.sbaid
			INNER JOIN obras.preobra pre ON pre.preid = ppc.preid
			INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
		WHERE
			
			 ppc.pocstatus = 'A'
			AND sba.sbastatus = 'A'
			AND ppc.proid = ".$_POST['proid']."
			
		GROUP BY 
			sba.sbaid,
			sobano,
			ppc.preid,
			pre.predescricao,
			pto.ptodescricao,
			prevalorobra"
	;
//ver(simec_htmlentities($sql));
	$cabecalho = array("Ação","Descrição da Obra", "Tipo da Obra",  "Ano da OBra", "Valor da Obra" );
	$db->monta_lista($sql,$cabecalho,50000,5,'N','95%','S');

}

function carregaDadoSubacaoObras(){
	global $db;
	
	$sql = "SELECT
                                '<img border=\"0\" src=\"/imagens/icone_lupa.png\" onclick=\"abreReformulacao('||pre.preid||', '||pre.muncod||');\" style=\"cursor:pointer\" />' AS acao,
				pre.predescricao,
				pto.ptodescricao,
				prevalorobra
			FROM 
				par.processoobraspaccomposicao ppc
			INNER JOIN obras.preobra pre ON pre.preid=ppc.preid AND pre.prestatus = 'A'
			INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
			WHERE
				ppc.pocstatus = 'A' and
				ppc.proid = ".$_POST['proid'];
	
	$cabecalho = array("Ação","Descrição da Obra", "Tipo da Obra", "Valor da Obra" );
	$db->monta_lista($sql,$cabecalho,50000,5,'N','95%','S');

}

function carregaDadoObrasPar(){
	global $db;
	
	$sql = "SELECT
                            '<img src=../imagens/alterar.gif title=abrir style=cursor:pointer; onclick=\"exibir('||pre.preid||','||s.sbaid||','||so.sobano||');\">' AS acao,
                            pre.predescricao,
                            pto.ptodescricao,
                            prevalorobra
			FROM 
                            par.subacaoobra so
			INNER JOIN par.subacaodetalhe sd ON sd.sbaid = so.sbaid AND sd.sbdano = so.sobano
			INNER JOIN par.subacao s ON s.sbaid = sd.sbaid AND s.sbastatus <> 'C'
			INNER JOIN obras.preobra pre ON pre.preid=so.preid AND pre.prestatus = 'A'
			inner join par.processoobrasparcomposicao pc on pc.preid = pre.preid and pc.pocstatus = 'A'
			INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
			WHERE
                            sd.sbdid = ".$_POST['sbdid'];

	$cabecalho = array("Ação", "Descrição da Obra", "Tipo da Obra", "Valor da Obra" );
	$db->monta_lista($sql,$cabecalho,50000,5,'N','95%','S');
}

function carregaDadoItens(){
	global $db;

	$cronograma = $db->pegaUm( "SELECT sbacronograma FROM par.subacao s INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid WHERE sd.sbdid = ".$_POST['sbdid'] );
	
        
//        ver($_POST['sbaid'],d);
        
	if( $cronograma == 1 ){ // GLOBAL
	
	$sql = "SELECT DISTINCT
				sic.icodescricao,
				sic.icoquantidadetecnico as quantidade,
				TO_CHAR( sic.icovalor, '999G999G999D99'),
				TO_CHAR( ( sic.icoquantidadetecnico * sic.icovalor), '999G999G999D99') as valortotal,
                emi.adesao
			FROM
				par.subacaoitenscomposicao sic
			INNER JOIN par.propostaitemcomposicao pic ON pic.picid = sic.picid
			INNER JOIN par.subacaodetalhe sd ON sd.sbaid = sic.sbaid AND sd.sbdano = sic.icoano
			LEFT JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = sic.sbaid AND emi.epistatus = 'A' AND emi.epiano = sd.sbdano
            --LEFT JOIN par.processoitemadesaopregao piap ON sic.icoid = piap.icoid AND piap.piastatus = 'A'
			WHERE
				sic.icovalidatecnico = 'S' AND
				sd.sbdid = ".$_POST['sbdid'];

	} else { // ESCOLA
		
	$sql = "SELECT          
                        sic.icodescricao,
                        par.recuperaquantidadeitemvalidado(sic.icoid) as quantidade,
                        TO_CHAR( sic.icovalor, '999G999G999D99'),
                        TO_CHAR(SUM( sse.seiqtdtecnico * sic.icovalor ), '999G999G999D99') as valortotal,
                        emi.adesao
                FROM
                        par.subacaoitenscomposicao sic
                INNER JOIN par.subescolas_subitenscomposicao  sse ON sse.icoid = sic.icoid
                INNER JOIN par.propostaitemcomposicao pic ON pic.picid = sic.picid
				INNER JOIN par.subacaodetalhe sd ON sd.sbaid = sic.sbaid AND sd.sbdano = sic.icoano
				LEFT JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = sic.sbaid AND emi.epistatus = 'A' AND emi.epiano = sd.sbdano
                --LEFT JOIN par.processoitemadesaopregao piap ON sic.icoid = piap.icoid
                WHERE
                        sic.icovalidatecnico = 'S' AND
                        sd.sbdid = ".$_POST['sbdid']."
                GROUP BY
                        sic.icodescricao, sic.icovalor, sic.icoquantidadetecnico, emi.adesao, sic.icoid";
	}

	$cabecalho = array("Descrição do Item", "Quantidade", "Valor", "Valor Total", 'Número da adesão' );
	$db->monta_lista($sql,$cabecalho,50000,5,'N','90%','N');

}

//include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

/*if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){
	$mnuid = array( 0 => 12172, 1 => 12185 ); // Esconde a Aba "Reformular Projeto" e "Histórico de Reformulações"
} else {
	$mnuid = array( 0 => 11861, 1 => 11862 , 2 => 11863 ); // Esconde a Aba "Reformular Projeto", "Histórico de Reformulações" e "Montar Projeto"
}*/

//$abacod_tela = 57584;
//$db->cria_aba( $abacod_tela, $url, $parametros, $mnuid );

echo montarAbasArray( $obProjeto->criaAbaPar(), "par.php?modulo=principal/projetos&acao=A&abas=administrarProjeto" );

$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];
$nmEntidadePar = EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar,$_SESSION['par']['itrid']);
monta_titulo( $nmEntidadePar, 'Administrar Projeto' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript"
	src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css"
	href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css" />
<style type="">
.ui-dialog-titlebar {
	text-align: center;
}
</style>

<table cellspacing="0"  border="0"  align="center" style="border-top: none; border-bottom: none;" class="tabela">
		<tr bgcolor="#eeeeee" >
			<td colspan="20"><b>Legenda:</b></td>
		</tr>
		<tr align="center">
			<td height="50px" width="1%" bgcolor="#eeeeee">
				<span style="font-size:40px" ><img  style="width:10px;height: 10px;" border="0" src="/imagens/mais.gif" style="cursor:pointer"></span>
			</td>
			<td bgcolor="#eeeeee">
				= <b>Detalhes do Projeto</b> &nbsp; &nbsp;
			</td>
			<td bgcolor="#eeeeee">
				<span style="color:#228B22;font-size:25px" title="Aderir ao pregão" class="glyphicon glyphicon-transfer"></span> 
			</td>
			<td bgcolor="#eeeeee">
				= <b>Aderir ao Pregão</b> &nbsp; &nbsp;
			</td>
			<td bgcolor="#eeeeee">
				<span style="color:#228B22;font-size:25px" title="Aderir ao pregão" class="glyphicon glyphicon-pencil"></span>
			</td>
			<td bgcolor="#eeeeee">
				= <b>Editar Processo</b> &nbsp; &nbsp;
			</td>
			
			<td bgcolor="#eeeeee">
				<span style="font-size:25px; color:#228B22;" class="glyphicon glyphicon-share"></span>
			</td>
			<td bgcolor="#eeeeee">
				 =<b> Reformular</b> &nbsp; &nbsp;
			</td>
 			<td bgcolor="#eeeeee"> 
			<span style="font-size:25px; color:#228B22;" class="glyphicon glyphicon-edit"></span>
			</td>
			<td bgcolor="#eeeeee">
				 =<b> Em reformulação</b> &nbsp; &nbsp;
			</td>
			<td bgcolor="#eeeeee">
				<span style="font-size:25px; color:red;" class="glyphicon glyphicon-remove"></span>
			</td>
			<td bgcolor="#eeeeee">
				 = <b>Cancelar Processo</b> &nbsp; &nbsp;
			</td>
			<td bgcolor="#eeeeee">
				
					<span style="font-size:40px" ><img  style="width:33px;height: 33px;" border="0" src="../imagens/par/cadeado-aberto.png" title="Finalizar Execução" style="cursor:pointer"></span>
			</td>
			<td bgcolor="#eeeeee">
				 = <b>Finalizar Execução</b> &nbsp; &nbsp;
			</td>
			<td bgcolor="#eeeeee">
					
					<span style="font-size:25px; color:black;" class="glyphicon glyphicon-lock"></span>
					
			</td>
			<td bgcolor="#eeeeee">
				 = <b>Processo Finalizado</b> &nbsp; &nbsp;
			</td>
			<td bgcolor="#eeeeee">
					<span style="font-size:25px; color:black;" class="glyphicon glyphicon-folder-open"></span>
			</td>
			<td bgcolor="#eeeeee">
				 = <b>Anexar Documento</b> &nbsp; &nbsp;
			</td>		
			
			<td bgcolor="#eeeeee">
					<span style="font-size:25px; color:black;" class="glyphicon glyphicon-time"></span>
			</td>
			<td bgcolor="#eeeeee">
				 = <b>Histórico de Reprogramação</b> &nbsp; &nbsp;
			</td>
		</tr>
	</table>
<form name="formulario" id="formulario" method="post" action="">
	<input type=hidden name="requisicaoFinalizar" id="requisicaoFinalizar"
		value=""> <input type=hidden name="processo" id="processo" value=""> <input
		type=hidden name="tipo" id="tipo" value="">
	<table align="center" border="0" width="95%" class="tabela"
		cellpadding="3" cellspacing="2">
		<tr id="tr_div">
			<td><script type="text/javascript" src="../../includes/remedial.js"></script>
				<script type="text/javascript" src="../../includes/superTitle.js"></script>
				<link rel="stylesheet" type="text/css"
					href="../../includes/superTitle.css" />
			<?
			if( $_SESSION['par']['itrid'] == 1 ){
				$tratamento = "WHERE pro.prostatus = 'A' AND pro.estuf = '".$_SESSION['par']['estuf']."' AND pro.muncod IS NULL";
			} else {
				$tratamento = "WHERE pro.prostatus = 'A' AND pro.muncod = '".$_SESSION['par']['muncod']."'";
			}
			
			
			$onclickPAR =  "onclick=\"inativaProcesso( '|| prp.prpid|| ',
													   \'' ||
						 									substring(prp.prpnumeroprocesso from 1 for 5)||'.'||
															substring(prp.prpnumeroprocesso from 6 for 6)||'/'||
															substring(prp.prpnumeroprocesso from 12 for 4)||'-'||
															substring(prp.prpnumeroprocesso from 16 for 2)
															||
														'\' , \'PAR\' );\"";
			
			
			$onclickObrasPar =  "onclick=\"inativaProcesso( '|| pro.proid|| ',
													   \'' ||
						 									substring(pro.pronumeroprocesso from 1 for 5)||'.'||
															substring(pro.pronumeroprocesso from 6 for 6)||'/'||
															substring(pro.pronumeroprocesso from 12 for 4)||'-'||
															substring(pro.pronumeroprocesso from 16 for 2)
															||
														'\' , \'OBRAS_PAR\' );\"";
			
			$onclickPAC =  "onclick=\"inativaProcesso( '|| pro.proid|| ',
													   \'' ||
						 									substring(pro.pronumeroprocesso from 1 for 5)||'.'||
															substring(pro.pronumeroprocesso from 6 for 6)||'/'||
															substring(pro.pronumeroprocesso from 12 for 4)||'-'||
															substring(pro.pronumeroprocesso from 16 for 2)
															||
														'\' , \'PAC\' );\"";
			

			$sqlReformuladoPAC = "(SELECT DISTINCT
									TRUE
								FROM
									obras.preobra pre
								INNER JOIN par.processoobraspaccomposicao poc ON poc.preid = pre.preid
								WHERE
									prereformulacao IS TRUE
									AND pre.prestatus = 'A'
									AND poc.proid = pro.proid)";
			
			$sql = " -- PROCESSO PAR
					(SELECT
						acao,
						'<span class=\"processo_detalhe\">' || processo || '</span>' as processo,
						termo,
						/*TO_CHAR( SUM ( COALESCE(valor,0) ), '999G999G999D99') as valor,*/
						(select case when (dopvalortermo) is not null then TO_CHAR( COALESCE(dopvalortermo,0.00), '999G999G999D99') else null end  
						from par.vm_documentopar_ativos where prpid = foo.prpid order by dopdatainclusao desc limit 1 ) as valor,
						Coalesce((select sum(vrlempenhocancelado) from par.v_vrlempenhocancelado where processo = foo.processo ), 0.00)
						as valorempenhado,
						(
						   Select 
							  coalesce(sum(pg.pagvalorparcela),0.00) as pagvalorparcela_efetivada
						   From par.pagamento pg
						   join par.empenho em on em.empid = pg.empid and empstatus = 'A' AND pg.pagstatus = 'A' 
						   where trim(pg.pagsituacaopagamento) = '2 - EFETIVADO' and empnumeroprocesso = foo.processo
						   group by empnumeroprocesso, pagstatus  
						) as valorpagamento,
						CASE WHEN tipo IS NOT NULL THEN tipo ELSE '' END as tipo,
						--dialogo,
						'<center><img style=\"cursor:pointer\" id=\"img_plano_' || processo || '\" src=\"/imagens/book.png\" onclick=\"carregarPlanoTrabalhoPAR(\''||prpid||'\');\" border=\"0\" /></center>' as plano_trabalho,
						'<center>
						<a href=\"#\"><span style=\"font-size:15px; color:black;\" class=\"glyphicon glyphicon-folder-open\" onclick=\"mostraDocumento('||prpid||', \'par\');\" title=\"Anexar Documento \" ></span></a>
						</center>' || '</td></tr>
						            	<tr style=\"display:none\" id=\"listaDocumentos_' || prpid || '\" >
						            		<td id=\"trV_' || prpid || '\" colspan=8 ></td>
						            </tr>' as documento
					FROM
					(
						SELECT DISTINCT
							'<left><img style=\"cursor:pointer\" id=\"img_dimensao_' || prp.prpid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaSubacao(this.id,\'' || prp.prpid || '\');\" border=\"0\" /> &nbsp;'||
							CASE WHEN prp.prpfinalizado IS NULL 
								THEN
									CASE WHEN dop.prpid IS NOT NULL 
										THEN 
											CASE WHEN dop.dopreformulacao IS TRUE 
												THEN
													CASE WHEN emi.prpid IS NOT NULL 
														THEN '<a href=\"javascript:aderirPregao1(\'' || iu.inuid || '\', \'' || prp.prpid || '\')\" >
																<span  title=\"Aderir ao pregão\" class=\"glyphicon glyphicon-transfer\" style=\"color:#228B22;font-size:15px\"></span></a> &nbsp;' 
														ELSE '' 
													END
													|| '<a title=\"Em reformulação\" href=\'#\'><span style=\"font-size:15px; color:#228B22;\" onclick=\"reformula('||prp.prpid||');\" class=\"glyphicon glyphicon-edit\"></span></a>'
												ELSE
													CASE WHEN emi.prpid IS NOT NULL 
														THEN '<a href=\"javascript:aderirPregao1(\'' || iu.inuid || '\', \'' || prp.prpid || '\')\" ><span title=\"Aderir ao pregão\" class=\"glyphicon glyphicon-transfer\" style=\"color:#228B22;font-size:15px\"></span></a> &nbsp;' ELSE '' 
													END
													|| '<a title=\"Reformular\" href=\'#\'><span    style=\"font-size:15px; color:#228B22;\" onclick=\"reformula('||prp.prpid||');\" class=\"glyphicon glyphicon-share\"></span></a>'
												END
										ELSE 
							 				'<a href=\"#\"><span style=\"color:#228B22;font-size:15px\" title=\"Editar Processo\" onclick=\"addSub('||prp.prpid||', 1);\" border=\"0\" class=\"glyphicon glyphicon-pencil\"></span></a>'
									END ||
		                      		CASE WHEN (SELECT DISTINCT count(empid) from par.empenho where empnumeroprocesso = prp.prpnumeroprocesso and empstatus = 'A') > 0 
		                      			THEN 
	                        				CASE WHEN (/*sum(vve.vrlempenhocancelado)*/ vep.saldo > 0) 
	                        					THEN '<span title=\"Cancelar\" style=\"font-size:15px; color: #BEBEBE;\" class=\"glyphicon glyphicon-remove\"></span>'
												ELSE '<a href=\"#\"><span title=\"Cancelar\" style=\"font-size:15px; color: red;\" {$onclickPAR} class=\"glyphicon glyphicon-remove\"></span></a>'						
											END
	                        			ELSE '<a href=\"#\"><span title=\"Cancelar\" style=\"font-size:15px; color: red;\" {$onclickPAR} class=\"glyphicon glyphicon-remove\"></span></a>'
									END || 
									CASE WHEN prp.prpfinalizado IS NULL 
										THEN '<a href=\"#\"><span style=\"font-size:40px\" ><img  style=\"width:20px;height: 20px;\" border=\"0\" onclick=\"finalizarProcesso('||prp.prpid||', 1);\" title=\"Finalizar Execução\"  src=\"../imagens/par/cadeado-aberto.png\" style=\"cursor:pointer\"></span></a>' 
						 				ELSE '' 
						 			END 
                        		ELSE 
                        			'<span title=\"Processo Finalizado\" style=\"font-size:15px; color:black;\" class=\"glyphicon glyphicon-lock\"></span>' 
                        	END || '</left>' 
                        	|| 
                        	CASE WHEN exists(select hsrid from par.historicosolicitacaoreprogramacao where prpid = prp.prpid limit 1) THEN
                         		'<a href=\'#\' onclick=\'javascript:abrepopuphistorico( '|| prp.prpid ||' )\'>&nbsp;<span title=\"Histórico de Reprogramação\" style=\"font-size:15px; color:black;\" class=\"glyphicon glyphicon-time\"></span></a>'
                         	else
                         		''
                         	END
                        	as acao,
							prp.prpnumeroprocesso as processo,
							prp.sisid,
							dop.dopnumerodocumento || '/' || date_part('YEAR', dop.dopdatainclusao) || ' - ' || mdo.mdonome as termo,
							CASE WHEN prp.prptipo = 'B' 
								THEN 'Subações do Brasil-Pró' 
						    	ELSE 
						    		CASE WHEN prp.sisid = 57 
						    			THEN 'Subações de Emendas no PAR' 
						    			ELSE 'Subações Genérico do PAR' 
						    		end
							END as tipo,
							prp.prpid,
							vep.saldo as valor
						FROM 
							par.instrumentounidade iu
						INNER JOIN par.processopar  				prp ON prp.inuid = iu.inuid AND prp.prpstatus = 'A' 
						LEFT  JOIN par.vm_documentopar_ativos		dop ON dop.prpid = prp.prpid
						LEFT  JOIN par.modelosdocumentos 			mdo ON mdo.mdoid = dop.mdoid AND mdo.tpdcod IN ( 102, 16 )
						LEFT  JOIN par.empenho 						emp ON emp.empnumeroprocesso = prp.prpnumeroprocesso AND empcodigoespecie NOT IN ('03', '13', '02', '04') AND empstatus = 'A'
						LEFT  JOIN par.vm_saldo_empenho_do_processo vep ON vep.processo = prp.prpnumeroprocesso
						LEFT  JOIN par.empenhosubacao 				ems ON ems.empid = emp.empid AND eobstatus = 'A'
						LEFT  JOIN par.empenhopregaoitemperdido 	emi ON emi.prpid = prp.prpid
						WHERE 
							iu.inuid = {$_SESSION['par']['inuid']} 
						ORDER BY
							prp.prpnumeroprocesso) as foo
					GROUP BY
						acao, processo, termo, prpid, tipo, sisid
					)
					UNION ALL
					-- PROCESSO DE OBRAS DO PAR
					SELECT
						acao,
						'<span class=\"processo_detalhe\">' || pronumeroprocesso || '</span>' as processo,
						termo,
						valor, 
						valorempenhado,
						(
							SELECT COALESCE(sum(pg.pagvalorparcela),0.00) FROM par.pagamento pg 
							INNER JOIN par.empenho em ON em.empid=pg.empid and empstatus = 'A' 
							WHERE em.empnumeroprocesso= fooObras.pronumeroprocesso AND pg.pagstatus='A'
						) as valorpagamento,
						CASE WHEN tipo IS NOT NULL THEN tipo ELSE '' END as tipo,
						'' as plano_trabalho,
						'<center>
						<a href=\"#\"><span style=\"font-size:15px; color:black;\" class=\"glyphicon glyphicon-folder-open\" onclick=\"mostraDocumento('||proid||', \'obras\');\" title=\"Anexar Documento \" ></span></a>
						</center>' || '</td></tr>
						            	<tr style=\"display:none\" id=\"listaDocumentos3_' || proid || '\" >
						            		<td id=\"trO_' || proid || '\" colspan=8 ></td>
						            </tr>' as documento
					FROM(
						SELECT DISTINCT
							'<left><img style=\"cursor:pointer\" id=\"img_dimensao_' || pro.proid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaSubacaoObrasPar(this.id,\'' || pro.proid || '\');\" border=\"0\" /> &nbsp;'||
							CASE WHEN pro.profinalizado IS NULL 
								THEN
									CASE WHEN dop.proid IS NOT NULL 
										THEN 
											CASE WHEN dop.dopreformulacao IS TRUE 
												THEN '<a title=\"Em Reformulação\" href=\'#\'><span  style=\"font-size:15px; color:#228B22;\"  onclick=\"reformulaObrasPar('||pro.proid||');\" class=\"glyphicon glyphicon-edit\"></span></a>'
												ELSE '<a title=\"Reformular\" href=\'#\'><span    style=\"font-size:15px; color:#228B22;\" onclick=\"reformulaObrasPar('||pro.proid||');\" class=\"glyphicon glyphicon-share\"></span></a>'
											END
										ELSE '<a href=\"#\"><span style=\"color:#228B22;font-size:15px\" title=\"Editar Processo\" onclick=\"addSub('||pro.proid||', 2);\" border=\"0\" class=\"glyphicon glyphicon-pencil\"></span></a>'
									END ||
			                        CASE WHEN (SELECT COUNT(empid) FROM par.empenho WHERE empnumeroprocesso = pro.pronumeroprocesso AND empstatus = 'A') > 0 
			                        	THEN 
											CASE WHEN ( vep.saldo > 0) 
												THEN '<span title=\"Cancelar\" style=\"font-size:15px; color: #BEBEBE;\" class=\"glyphicon glyphicon-remove\"></span>'
												ELSE '<a href=\"#\"><span title=\"Cancelar\" style=\"font-size:15px; color: red;\" {$onclickObrasPar} class=\"glyphicon glyphicon-remove\"></span></a>'
											END
									 	ELSE '<a href=\"#\"><span title=\"Cancelar\" style=\"font-size:15px; color: red;\" {$onclickObrasPar} class=\"glyphicon glyphicon-remove\"></span><a/>'
									END || 
									'<a href=\"#\"><span style=\"font-size:40px\" ><img  style=\"width:20px;height: 20px;\" border=\"0\" onclick=\"finalizarProcesso('||pro.proid||', 2);\"  src=\"../imagens/par/cadeado-aberto.png\" title=\"Finalizar Execução\" style=\"cursor:pointer\"></span></a>'
	                    		ELSE '<span title=\"Processo Finalizado\" style=\"font-size:15px; color:black;\" class=\"glyphicon glyphicon-lock\"></span>' 
	                    	END || '</left>' as acao,
							pro.pronumeroprocesso,
							pro.proid,
							pro.sisid,
							dop.dopnumerodocumento || '/' || DATE_PART('YEAR', dop.dopdatainclusao) || ' - ' || mdo.mdonome as termo,
							CASE WHEN (dop.dopvalortermo) IS NOT NULL THEN TO_CHAR( COALESCE(dopvalortermo,0.00), '999G999G999D99') END as valor,
							CASE WHEN pro.protipo = 'B' 
								THEN 'Obras do PAR - Brasil-Pró' 
						    	ELSE CASE WHEN pro.sisid = 57 THEN 'Obras de Emendas' ELSE 'Obras PAR' END
							END as tipo,
							vep.saldo as valorempenhado
						FROM 
							par.instrumentounidade iu
						INNER JOIN par.processoobraspar pro ON pro.inuid = iu.inuid and pro.prostatus = 'A' 
						LEFT  JOIN par.vm_documentopar_ativos dop ON dop.proid = pro.proid
						LEFT  JOIN par.modelosdocumentos mdo ON mdo.mdoid = dop.mdoid AND mdo.tpdcod IN ( 102, 16 )
						LEFT  JOIN par.vm_saldo_empenho_do_processo vep ON vep.processo = pro.pronumeroprocesso
						INNER JOIN par.processoobrasparcomposicao ppc ON ppc.proid = pro.proid and ppc.pocstatus = 'A'
						INNER JOIN obras.preobra pre ON pre.preid = ppc.preid AND pre.prestatus = 'A'
						WHERE 
							iu.inuid = ".$_SESSION['par']['inuid']."
						ORDER BY
							pro.pronumeroprocesso
					) as fooObras
						
					UNION ALL
					
					-- PROCESSO DE OBRAS DO PAC
					(SELECT DISTINCT
						'<left><img style=\"cursor:pointer\" id=\"img_dimensao_' || pro.proid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaSubacaoObras(this.id,\'' || pro.proid || '\');\" border=\"0\" /> &nbsp;'||
						CASE WHEN pro.profinalizado IS NULL 
							THEN
								CASE WHEN ter.terid IS NOT NULL 
									THEN 
									CASE WHEN $sqlReformuladoPAC = TRUE
										THEN '<a title=\"Em Reformulação\" href=\'#\'><span  style=\"font-size:15px; color:#228B22;\"  onclick=\"reformulaObras('||pro.proid||');\" class=\"glyphicon glyphicon-edit\"></span></a>'
										ELSE '<a title=\"Reformular\" href=\'#\'><span    style=\"font-size:15px; color:#228B22;\" onclick=\"reformulaObras('||pro.proid||');\" class=\"glyphicon glyphicon-share\"></span></a>'
									END
								ELSE '<a href=\"#\"><span style=\"color:#228B22;font-size:15px\" title=\"Editar Processo\" onclick=\"addSub('||pro.proid||', 3);\" border=\"0\" class=\"glyphicon glyphicon-pencil\"></span></a>' 
								END ||
		                        CASE WHEN (select count(empid) from par.empenho where empnumeroprocesso = pro.pronumeroprocesso and empstatus = 'A') > 0 
		                        	THEN 
										CASE WHEN (select sum(vrlempenhocancelado) from par.v_vrlempenhocancelado where processo = pro.pronumeroprocesso) > 0 
											THEN '<span title=\"Cancelar\" style=\"font-size:15px; color: #BEBEBE;\" class=\"glyphicon glyphicon-remove\"></span>'
										 	ELSE '<a href=\"#\"><span title=\"Cancelar\" style=\"font-size:15px; color: red;\" {$onclickPAC} class=\"glyphicon glyphicon-remove\"></span><a>'
										END
									ELSE '<a href=\"#\"><span title=\"Cancelar\" style=\"font-size:15px; color:red;\" {$onclickPAC} class=\"glyphicon glyphicon-remove\"></span><a>'
								END || '<a href=\"#\"><span style=\"font-size:40px\" ><img  style=\"width:20px;height: 20px;\" border=\"0\" onclick=\"finalizarProcesso('||pro.proid||', 3);\"  src=\"../imagens/par/cadeado-aberto.png\" title=\"Finalizar Execução\" style=\"cursor:pointer\"></span></a>'
	                    	ELSE '<span title=\"Processo Finalizado\" style=\"font-size:15px; color:black;\" class=\"glyphicon glyphicon-lock\"></span>' 
	                    END || '</left>' as acao,
                        '<span class=\"processo_detalhe\">' || pro.pronumeroprocesso || '</span>' as processo,
						r.resdescricao as termo,
						TO_CHAR( SUM ( pre.prevalorobra ), '999G999G999D99') as valor,
						vep.saldo,
						(SELECT COALESCE(sum(pg.pagvalorparcela),0.00) FROM par.pagamento pg 
						    INNER JOIN par.empenho em ON em.empid=pg.empid and empstatus = 'A' 
						    WHERE em.empnumeroprocesso=pro.pronumeroprocesso AND pg.pagstatus='A') as valorpagamento,
						'Obras do PAC' as tipo,
						'' as plano_trabalho,
						'<center>
						<a href=\"#\"><span style=\"font-size:15px; color:black;\" class=\"glyphicon glyphicon-folder-open\" onclick=\"mostraDocumento('||pro.proid||', \'pac\');\" title=\"Anexar Documento \" ></span></a>
						</center>' || '</td></tr>
						            	<tr style=\"display:none\" id=\"listaDocumentos4_' || pro.proid || '\" >
						            		<td id=\"trO2_' || pro.proid || '\" colspan=8 ></td>
						            </tr>' as documento
					FROM 
						par.processoobra pro 
					LEFT JOIN par.vm_saldo_empenho_do_processo 	vep ON vep.processo = pro.pronumeroprocesso
					LEFT JOIN par.termocompromissopac 			ter ON ter.proid = pro.proid AND ter.terstatus = 'A'
					LEFT JOIN par.resolucao 					r   ON r.resid 	 = pro.resid
					LEFT JOIN par.processoobraspaccomposicao 	ppc ON ppc.proid = pro.proid AND ppc.pocstatus = 'A'
					LEFT JOIN obras.preobra 					pre ON pre.preid = ppc.preid AND pre.prestatus = 'A'
					{$tratamento}
					GROUP BY
						pro.proid, ter.terid, pro.pronumeroprocesso, r.resdescricao, pro.profinalizado, vep.saldo
					ORDER BY processo)";

// 			ver(simec_htmlentities($sql));
// 			$cabecalho = array('Ações', 'Nº Processo', 'Termo / Projeto', 'Valor do Termo', 'Tipo', 'Diálogo', 'PTA Consolidado', 'Documentos Diversos');
			$cabecalho = array('Ações', 'Nº Processo', 'Termo / Projeto', 'Valor do Termo', 'Valor empenhado(R$)', 'Valor pagamento(R$)', 'Tipo', 'PTA Consolidado', 'Documentos Diversos');
			$db->monta_lista($sql, $cabecalho, 500, 5, 'N', '100%', 'S');
			?></td>
		</tr>
	</table>
	<div id="debug"></div>
	<script type="text/javascript">

function abreProtocolo( hsrid )
{
	window.open('par.php?modulo=principal/popUpProtocoloReprogramacao&acao=A&hsrid=' + hsrid ,  '' ,'width=800,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left='+ (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 300) / 2);
}
	
function abrepopuphistorico(prpid)
{

	$.ajax({
   		type: "POST",
   		url: "par.php?modulo=principal/administrarProjeto&acao=A",
   		data: "requisicao=historicoReformulacao&prpid="+prpid,
   		async: false,
   		success: function(msg)
   		{
   			jQuery( "#dialog-historico" ).html(msg);	
   			//jQuery( "#dialog-historico" ).show();
   			jQuery( "#dialog-historico" ).dialog({
   				resizable: false,
   				height:600,
   				width:800,
   				modal: true,
   				show: { effect: 'drop', direction: "up" },
   				buttons: {
   					"OK": function() {
   						jQuery( this ).dialog( "close" );							
   					}
   				}
   			});
   		}
	});
	
	
		

}
	
function mostraDocumento(codigo, tipo){
	url = 'par.php?modulo=principal/popupProcessoDocumento&acao=A&codigo='+codigo+'&tipo='+tipo;
	window.open(url,'Documentos Diversos',"height=400,width=800,scrollbars=yes,top=50,left=200" );
} 

function mostraDocumento(codigo, tipo){
	url = 'par.php?modulo=principal/popupProcessoDocumento&acao=A&codigo='+codigo+'&tipo='+tipo;
	window.open(url,'Documentos Diversos',"height=400,width=800,scrollbars=yes,top=50,left=200" );
} 
function carregarPlanoTrabalhoPAR( prpid )
{
	url = 'par.php?modulo=principal/planoTrabalhoConsolidado&acao=A&prpid='+prpid;
	window.open(url,'Plano de Trabalho Consolidado',"height=400,width=800,scrollbars=yes,top=50,left=200" );
}
function aderirPregao(sbdid, inuid, prpid){

	divCarregando();
	if( confirm('Tem certeza que deseja aderir ao pregão?') ){
		
		$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/administrarProjeto&acao=A",
	   		data: "requisicao=aderirPregao&sbdid="+sbdid+"&inuid="+inuid+"&prpid="+prpid,
	   		async: false,
	   		success: function(msg){
	   			
	   			
	   			$( "#dialog-confirm" ).html(msg);
	
				$( "#dialog-confirm" ).dialog({
					resizable: true,
					height:400,
					width:400,
					modal: true,
					buttons: {
						"OK": function() {
							$( this ).dialog( "close" );
							window.location.reload();
							
						}
						
					}
				});
	   		}
		});
	}
	divCarregado();
}

function aderirPregao1(inuid, prpid){

	divCarregando();
	if( confirm('Tem certeza que deseja aderir ao pregão?') ){
		
		$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/administrarProjeto&acao=A",
	   		data: "requisicao=aderirPregao1&inuid="+inuid+"&prpid="+prpid,
	   		async: false,
	   		success: function(msg){
	   			
	   			
	   			$( "#dialog-confirm" ).html(msg);
	
				$( "#dialog-confirm" ).dialog({
					resizable: true,
					height:400,
					width:400,
					modal: true,
					buttons: {
						"OK": function() {
							$( this ).dialog( "close" );
							window.location.reload();
							
						}
						
					}
				});
	   		}
		});
		
		//carregarListaEmpenhoProcesso(wsempid,wsprocesso);
		
	}
	divCarregado();
}

function carregarListaSubacao(idImg, prpid){
	var img 	 = $( '#'+idImg );
	var tr_nome = 'listaDocumentos_'+ prpid;
	var td_nome  = 'trV_'+ prpid;
	
	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == ""){
		$('#'+td_nome).html('Carregando...');
		img.attr ('src','../imagens/menos.gif');
		carregaListaSubacao(prpid, td_nome);
	}
	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != ""){
		$('#'+tr_nome).css('display','');
		img.attr('src','../imagens/menos.gif');
	} else {
		$('#'+tr_nome).css('display','none');
		img.attr('src','/imagens/mais.gif');
	}
}

function carregarListaSubacaoObrasPar(idImg, proid){
	var img 	 = $( '#'+idImg );
	var tr_nome = 'listaDocumentos3_'+ proid;
	var td_nome  = 'trO_'+ proid;
	
	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == ""){
		$('#'+td_nome).html('Carregando...');
		img.attr ('src','../imagens/menos.gif');
		carregaListaSubacaoObrasPar(proid, td_nome);
	}
	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != ""){
		$('#'+tr_nome).css('display','');
		img.attr('src','../imagens/menos.gif');
	} else {
		$('#'+tr_nome).css('display','none');
		img.attr('src','/imagens/mais.gif');
	}
}

function inativaProcesso(  proid, nprocesso , tipo )
{

if( tipo == 'PAR')
{
	requisicao  = "inativaProcessoPar"
}
else if( tipo == 'OBRAS_PAR')
{
	requisicao  = "inativaProcessoParObras"
}
else if( tipo == 'PAC')
{
	requisicao = "inativaProcessoPac"
}
else
{
	alert('Erro na requisição');
}

	
	
	var r = confirm("Tem certeza que deseja inativar o processo " + nprocesso);
	if (r == true) {
		$.ajax({
       		type: "POST",
       		url: window.location.href,
       		data: "requisicao="+requisicao+"&proid="+proid+"&nprocesso==" + nprocesso,
       		async: false,
       		success: function(msg){
       			alert(msg);
       			window.location.reload();
       			
       		}
    	});
	} else {
	  
	}
}


function carregarListaSubacaoObras(idImg, proid){
	var img 	 = $( '#'+idImg );
	var tr_nome = 'listaDocumentos4_'+ proid;
	var td_nome  = 'trO2_'+ proid;

	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == ""){
		$('#'+td_nome).html('Carregando...');
		img.attr ('src','../imagens/menos.gif');
		carregaListaSubacaoObras(proid, td_nome);
	}
	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != ""){
		$('#'+tr_nome).css('display','');
		img.attr('src','../imagens/menos.gif');
	} else {
		$('#'+tr_nome).css('display','none');
		img.attr('src','/imagens/mais.gif');
	}
}

function carregarListaItens(idImg, sbdid){
	var img 	 = $( '#'+idImg );
	var tr_nome = 'listaDocumentos2_'+ sbdid;
	var td_nome  = 'trI_'+ sbdid;

	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == ""){
		$('#'+td_nome).html('Carregando...');
		img.attr ('src','../imagens/menos.gif');
		carregaListaItens(sbdid, td_nome);
	}
	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != ""){
		$('#'+tr_nome).css('display','');
		img.attr('src','../imagens/menos.gif');
	} else {
		$('#'+tr_nome).css('display','none');
		img.attr('src','/imagens/mais.gif');
	}
}

function carregarListaObrasPar(idImg, sbdid){
	var img 	 = $( '#'+idImg );
	var tr_nome = 'listaDocumentos5_'+ sbdid;
	var td_nome  = 'trO5_'+ sbdid;
	
	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == ""){
		$('#'+td_nome).html('Carregando...');
		img.attr ('src','../imagens/menos.gif');
		carregaListaObrasPar(sbdid, td_nome);
	}
	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != ""){
		$('#'+tr_nome).css('display','');
		img.attr('src','../imagens/menos.gif');
	} else {
		$('#'+tr_nome).css('display','none');
		img.attr('src','/imagens/mais.gif');
	}
}

function cancelarProcesso(codigo, tipo){
	$('.ui-icon ui-icon-closethick').hide();
	$( '#dialog-aut' ).hide();
	$( '#dialog-confirm' ).hide();
	$( '#dialog-aut' ).dialog({
		resizable: false,
		width: 400,
		modal: true,
		show: { effect: 'drop', direction: "up" },
		buttons: {
			'Ok': function() {
				$( this ).dialog( 'close' );
				solicitaCancelarProcesso( codigo, tipo );
			},
			'Cancel': function() {
				$( this ).dialog( 'close' );
				//window.location.reload();
			}
		}
	});
}

function finalizarProcesso(processo, tipo){
	if(confirm("Tem certeza que deseja finalizar esse processo?")){
		$('#requisicaoFinalizar').val('finaliza');
		$('#processo').val(processo);
		$('#tipo').val(tipo);
		$('[name="formulario"]').submit();
	}
}

function solicitaCancelarProcesso(codigo, tipo){
	divCarregando();
	var usuario = $('#wsusuario').val();
	var senha = $('#wssenha').val();
	
	if( usuario == '' ){
		$('#resposta').html('Usuário está em branco!');
		return false;
	}
	if( senha == '' ){
		$('#resposta').html('Senha está em branco!');
		return false;
	}
	
	$.ajax({
   		type: "POST",
   		url: "par.php?modulo=principal/administrarProjeto&acao=A",
   		data: "wsusuario=" + usuario + "&wssenha=" + senha + "&requisicao=cancelarProcesso&codigo="+codigo+"&tipo="+tipo,
   		async: false,
   		success: function(msg){
   			$('.ui-icon ui-icon-closethick').hide();
			$( '#dialog-aut' ).hide();
			$( '#dialog-confirm' ).hide();
   			$( "#dialog-confirm" ).html(msg);		
			$( "#dialog-confirm" ).dialog({
				resizable: false,
				height:300,
				width:500,
				modal: true,
				show: { effect: 'drop', direction: "up" },
				buttons: {
					"OK": function() {
						$( this ).dialog( "close" );
						window.location.reload();								
					}
					
				}
			});
   		}
	});
		
	$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/administrarProjeto&acao=A",
	   		data: "requisicao=cancelarProcesso&codigo="+codigo+'&tipo='+tipo,
	   		async: false,
	   		success: function(msg){
	   			$('#debug').html(msg);
	   		}
	});
	divCarregado();
}

function carregaListaSubacao(prpid, td_nome){
	divCarregando();
	$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/administrarProjeto&acao=A",
	   		data: "requisicao=carregaDadosSubacao&prpid="+prpid,
	   		async: false,
	   		success: function(msg){
	   			$('#'+td_nome).html(msg);
	   			divCarregado();
	   		}
		});
}

function carregaListaObrasPar(sbdid, td_nome){
	divCarregando();
	$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/administrarProjeto&acao=A",
	   		data: "requisicao=carregaDadosObrasPar&sbdid="+sbdid,
	   		async: false,
	   		success: function(msg){
	   			$('#'+td_nome).html(msg);
	   			divCarregado();
	   		}
		});
}

function carregaListaSubacaoObrasPar(proid, td_nome){
	divCarregando();
	$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/administrarProjeto&acao=A",
	   		data: "requisicao=carregaDadosSubacaoObrasPar&proid="+proid,
	   		async: false,
	   		success: function(msg){
	   			$('#'+td_nome).html(msg);
	   			divCarregado();
	   		}
		});
}

function carregaListaSubacaoObras(proid, td_nome){
	divCarregando();
	$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/administrarProjeto&acao=A",
	   		data: "requisicao=carregaDadosSubacaoObras&proid="+proid,
	   		async: false,
	   		success: function(msg){
	   			$('#'+td_nome).html(msg);
	   			divCarregado();
	   		}
		});
}

function carregaListaItens(sbdid, td_nome){
	divCarregando();
	$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/administrarProjeto&acao=A",
	   		data: "requisicao=carregaDadosItens&sbdid="+sbdid,
	   		async: false,
	   		success: function(msg){
	   			$('#'+td_nome).html(msg);
	   			divCarregado();
	   		}
		});
}

function addSub(codigo, tipo){
	url = 'par.php?modulo=principal/popupAddSubacaoProjeto&acao=A&cod='+codigo+'&tipo='+tipo;
	window.open(url,'Editar Processo',"fullscreen=yes,scrollbars=yes" );
}

function reformula(prpid){
	//location.href = "par.php?modulo=principal/reformularProjeto&acao=A&prpid=" + prpid;
	location.href = "par.php?modulo=principal/projetos&acao=A&abas=reformularProjeto&prpid=" + prpid;
}

function alterarDadosEmpenho(prpid){
	location.href = "par.php?modulo=principal/projetos&acao=A&abas=distribuirEmpenho&prpid=" + prpid;
}

function reformulaObrasPar(proid){
	location.href = "par.php?modulo=principal/projetos&acao=A&abas=reformularProjetoObrasPar&proid=" + proid;
}

function reformulaObras(proid){
	location.href = "par.php?modulo=principal/projetos&acao=A&abas=reformularProjetoObras&proid=" + proid;
}

function abreReformulacao(preid, muncod) {
    return window.open('par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=dados&preid='+preid+'&muncod='+muncod,
        'ProInfancia', "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();
}
</script>
</form>
<div class="demo">
	<div id="dialog-confirm" title="Montagem do Projeto"
		style="overflow-x: auto; overflow-y: auto; width: 100%; height: 250px;"></div>
</div>

<div id="dialog-historico" title="Solicitações do Processo"style="display: none;">

</div>

<div id="dialog-dialogo" title="Solicitações do Processo"
	style="display: none;"></div>

<div id="dialog-aut" title="Autenticação" style="display: none;">
	<div id="dialog-content-aut">
		<table align="center" border="0" class="tabela" cellpadding="3"
			cellspacing="1">
			<tr>
				<td width="30%"
					style="text-align: right; font-size: 1.5em; color: #333333; font-weight: bold;">Usuário:</td>
				<td><input type="text"
					class="ui-button ui-widget ui-state-default ui-corner-all ui-button-text-only"
					title="Usuário" onblur="MouseBlur(this);"
					onmouseout="MouseOut(this);"
					onfocus="MouseClick(this);this.select();"
					onmouseover="MouseOver(this);" value="<?=$usuario ?>" size="23"
					id="wsusuario" name="wsusuario"> <img border="0"
					title="Indica campo obrigatório." src="../imagens/obrig.gif"></td>
			</tr>
			<tr>
				<td
					style="text-align: right; font-size: 1.5em; color: #333333; font-weight: bold;">Senha:</td>
				<td><input type="password"
					class="ui-button ui-widget ui-state-default ui-corner-all ui-button-text-only"
					title="Senha" onblur="MouseBlur(this);"
					onmouseout="MouseOut(this);"
					onfocus="MouseClick(this);this.select();"
					onmouseover="MouseOver(this);" value="<?=$senha ?>" size="23"
					id="wssenha" name="wssenha"> <img border="0"
					title="Indica campo obrigatório." src="../imagens/obrig.gif"></td>
			</tr>
		</table>
	</div>
	<div id="resposta" style="color: red; font-size: 0.9em"></div>
</div>
