<?php 
include_once APPRAIZ . "includes/workflow.php";

function htmlValorSubacao( $dados ){
	
	global $db;
	
	$sql = "SELECT
       			s.sbaid,
       			sd.sbdano as ano,
               (SELECT par.recuperaValorValidadosSubacaoPorAno(s.sbaid , sd.sbdano)::numeric(20,2)) AS valor
			FROM
			       par.subacao s
			INNER JOIN par.subacaodetalhe   sd ON sd.sbaid = s.sbaid
			WHERE
			       s.sbaid = ".$dados['sbaid']."
			       AND sbastatus = 'A'";
	$valores = $db->carregar($sql);
	
	$valor = Array();
	if( is_array($valores) ){
		foreach( $valores as $val ){
			$valor[$val['ano']] = $val['valor'];
		}
	}
	
	$html = '<td class="2011_'.$dados['frmid'].' total_'.$dados['frmid'].'" > R$ '.str_replace(Array('.',',','{}'), Array('{}','.',','), number_format($valor[2011],2)).'</td>
			<td class="2012_'.$dados['frmid'].' total_'.$dados['frmid'].'" > R$ '.str_replace(Array('.',',','{}'), Array('{}','.',','), number_format($valor[2012],2)).'</td>
			<td class="2013_'.$dados['frmid'].' total_'.$dados['frmid'].'" > R$ '.str_replace(Array('.',',','{}'), Array('{}','.',','), number_format($valor[2013],2)).'</td>
			<td class="2014_'.$dados['frmid'].' total_'.$dados['frmid'].'" > R$ '.str_replace(Array('.',',','{}'), Array('{}','.',','), number_format($valor[2014],2)).'</td>';
	echo $html;	
}

if( $_REQUEST['reqAjax'] ){
	$_REQUEST['reqAjax']($_REQUEST);
	die();
}

if( $_SESSION['par']['inuid'] ){
	$docid = parPegarDocidParaEntidade($_SESSION['par']['inuid']);
	$estadoAtualEntidade = wf_pegarEstadoAtual( $docid );
	
	if( $estadoAtualEntidade['esdid'] == WF_ELABORACAO ){
		$_SESSION['par']['boExcluiSubacao'] = 'S';
	} else {
		$_SESSION['par']['boExcluiSubacao'] = 'N';
	}
}else{
	echo "
		<script>
			alert('Sessão expirada.');
			location.href='par.php?modulo=inicio&acao=C';
 		</script>";
	die();
}

if($_POST['tramitarSubacoes']){
	echo "<script>
					alert('Não é possivel enviar as subações.');
					location.href='par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=listaAgrupada';
		 		</script>";
	die();
	include_once APPRAIZ . "includes/workflow.php";
	$aedids = implode(',',$_POST['aedid']);
	$sql = "SELECT s.docid, s.sbadsc, aed.aedid  FROM par.subacao s
			INNER JOIN workflow.documento d ON d.docid = s.docid
			INNER JOIN workflow.estadodocumento ed on ed.esdid = d.esdid
			INNER JOIN workflow.acaoestadodoc aed on aed.esdidorigem = ed.esdid
			WHERE s.sbaid in (".$_POST['subacoes'].")
			AND aed.aedid in(".$aedids.")";
	
	$lista = $db->carregar($sql);
	if($lista[0]) {
		//$mensagem = 'As subações abaixo foram enviadas com sucesso:'
		foreach($lista as $l) {
			$docid = $l['docid'];
			$aedid = $l['aedid'];
			$dados = array();
			$result = wf_alterarEstado( $docid, $aedid, $cmddsc = '', $dados);
			//$mensagem = "<br>".$l['sbadsc']."<br>";		
		}
	}
	if($result){
		echo "<script>
					alert('As subações foram enviadas com sucesso.');
					location.href='par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=listaAgrupada';
		 		</script>";
	}
	die();
}

if($_POST['listasubacaotramitacao']){
	$sql = "SELECT sbadsc FROM par.subacao s
			INNER JOIN workflow.documento d ON d.docid = s.docid
			INNER JOIN workflow.estadodocumento ed on ed.esdid = d.esdid
			INNER JOIN workflow.acaoestadodoc aed on aed.esdidorigem = ed.esdid
			WHERE s.sbaid in (".$_POST['sbaid'].")
			AND aed.aedid =".$_POST['aedid'];
	
	$sbadscs = $db->carregarColuna($sql);
	$cont = 1;
	echo ' <input type="hidden" name="subacoes" value="'.$_POST['sbaid'].'"  />
	<table id="pesquisas" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">';
	
	foreach($sbadscs as $sbadsc){
		echo '
						<tr>
							<td>'.$cont.' - '.$sbadsc.'</td>
						</tr>';
		
		$cont++;
	}
		echo	'</table>';
	die();
}

if( $_POST['tramita'] ){
	$sbaid = $_POST['sbaid'];

	$sql 	= "	SELECT DISTINCT docid
				from par.subacao
				where sbastatus = 'A' AND
					sbaid in(".$_POST['sbaid'].")
				AND docid is not null;	
				";
	$docids = $db->carregarColuna($sql);
	$docids = implode(",", $docids);
	
	if($docids){
		$sql = "SELECT DISTINCT ed.esdid,
						ed.esddsc
				FROM workflow.documento d 
				INNER JOIN workflow.estadodocumento ed on ed.esdid = d.esdid
				WHERE d.docid IN ( ".$docids.")";
		
		$estadosDocumentos = $db->carregar($sql);
		if(is_array($estadosDocumentos)){
			echo '<form id="formTramita" name="formTramita" method="post" action=""> 
					<input type="hidden" name="tramitarSubacoes" id="tramitarSubacoes" value="false" />';
			foreach($estadosDocumentos as $estados){
				
				if($estados['esdid']){
					
					$sql = "SELECT DISTINCT
								aed.aedid as codigo,
								aed.aeddscrealizar as descricao  
							FROM 	
								workflow.acaoestadodoc aed 
							INNER JOIN workflow.estadodocumentoperfil esp ON esp.aedid = aed.aedid
							WHERE esdidorigem = ".$estados['esdid']."";
					// inserir perfil neste sql
			
				
				echo '
					<table id="pesquisas" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
						<tr>
							<td class="SubTituloEsquerda"  colspan="2">Estado Atual: '.$estados['esddsc'].'</td>
						</tr>
						<tr>
							<td class="SubTituloDireita">Enviar para:</td>
							<td class="SubTituloEsquerda">'.$db->monta_combo("aedid[]",$sql,"S","Selecione...","listaSuacoesTramita(this.value,'$sbaid','{$estados['esdid']}');fake","","","","S", "aedid","true").'</td>
						</tr>
						<tr>
							<td class="SubTituloEsquerda"  colspan="2">Subações a serem enviadas:</td>
						</tr>
						<tr>
							<td class="SubTituloEsquerda"   colspan="2">
							<div id="listasubacoesPossiveisDeTramitacao_'.$estados['esdid'].'" ></div>
							</td>
						</tr>
						</table> <br>';
				}
			
			}
			echo '<table id="pesquisas2" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
						<td style="text-align: center;"><button id="btnWFSubacao" onclick="enviarProximaFaseSub();" >Enviar subações </button></td>
						</tr></table></form>';
		}
		
	}
	die();
}


$desc = '';
$perfil = pegaArrayPerfil($_SESSION['usucpf']);


if( in_array( PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil ) ||
	in_array( PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil ) ||
	in_array( PAR_PERFIL_PREFEITO, $perfil ) ||
	in_array( PAR_PERFIL_EQUIPE_ESTADUAL, $perfil ) ||
	in_array( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil )){
		
		verificaPreenchimentoDadosUnidadePorData();
		
}



if( in_array( PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil ) ||
	in_array( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil )){
	
	$sql = "SELECT * FROM par.basenacionalcomum WHERE inuid = ".$_SESSION['par']['inuid'];
	$dadoBase = $db->pegaLinha($sql);
	
	if( !$dadoBase['bncid'] ){
		
		$largura = "600px";
		$altura = "250px";
		$id = "div_auth_consulta";	
	
	?> 
		<style>
	.popup_alerta_consulta{
			width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura/2 ?>;margin-left:-<?php echo $largura/2 ?>;border:solid 2px black;background-color:#FFFFFF;}
</style>
<div id="<?php echo $id ?>" class="popup_alerta_consulta" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
	</div>
	<div >
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td align="justify" style="font-family: Calibri;"><b>Caro Dirigente,</b><br/><br/>

						A <b>Diretoria de Currículos e Educação Integral</b> da <b>Secretaria de Educação Básica</b> está realizando um levantamento sobre as 
						<b>Propostas Curriculares</b> construídas nos sistemas públicos de ensino (estados, municípios e Distrito Federal) a partir da homologação das novas 
						<b>Diretrizes Curriculares Nacionais</b> nos anos de 2009 para a <b>Educação Infantil</b>, 2010 para o <b>Ensino Fundamental</b> e 2012 para o 
						<b>Ensino Médio</b>. Para tanto, solicitamos sua contribuição respondendo algumas perguntas e, nos casos de existência de proposta curricular, 
						enviando os arquivos digitais das mesmas. Esclarecemos, também, que as propostas enviadas servirão de subsídios necessários à construção coletiva 
						de uma <b>Base Nacional Comum</b> para o currículo da Educação Básica e <b>serão divulgados</b> em site do Ministério da Educação sobre a Política 
						Curricular Nacional. Acesse a aba Base Nacional Comum no PAR e contribua conosco!
				</td>
			</tr>
			<tr>
				<td align="center" bgcolor="#D5D5D5" colspan="2">
					<input type="button" name="btn_enviar" onclick="responderBaseNacional()" value="ok" />
				</td>
			</tr>
		</table>
	</div>
</div> <?php
}
}

if( in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array( PAR_PERFIL_PREFEITO, $perfil ) || in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) || in_array( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil ) || in_array( PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO, $perfil ) ){
	if($_SESSION['par']['itrid'] == 1){
		$sql = "SELECT 	
				CASE WHEN terassinado = 't' THEN 0 ELSE count( tc.terid ) END as usu
			FROM 
				par.termocompromissopac  tc
			LEFT JOIN
				seguranca.usuario u ON u.usucpf = tc.usucpfassinatura 
			WHERE 
				tc.terstatus = 'A'	
				AND tc.estuf = '".$_SESSION['par']['estuf']."'
			GROUP BY
				terassinado,
				terdataassinatura";
	} else {
		$sql = "SELECT 	
				CASE WHEN terassinado = 't' THEN 0 ELSE count( tc.terid ) END as usu
			FROM 
				par.termocompromissopac  tc
			LEFT JOIN
				seguranca.usuario u ON u.usucpf = tc.usucpfassinatura 
			WHERE 
				tc.terstatus = 'A'	
				AND tc.muncod = '".$_SESSION['par']['muncod']."'
			GROUP BY
				terassinado,
				terdataassinatura";
	}
	$qnt = $db->pegaUm( $sql );

	if( $qnt >> 0 ){
		$desc = "<span style='color: red'>Existem documentos para serem aceitos e validados na lista de Documentos.</span><br>";
	} else {
		$desc = '';
	}
}

if(in_array(PAR_PERFIL_EQUIPE_FINANCEIRA ,$perfil)){
	$permitido = verifcaMunEstUsusario(PAR_PERFIL_EQUIPE_FINANCEIRA);
}elseif	(in_array(PAR_PERFIL_EQUIPE_TECNICA ,$perfil)){
	$permitido = verifcaMunEstUsusario(PAR_PERFIL_EQUIPE_TECNICA);
}

if($permitido == 'N'){
	$desc .= "<span style='color: red'>Município ou Estado não associado ao usuário.</span>";
}
/*!@
 * Carrega os detalhes da subação para exibição no dialog
 */
if ($_REQUEST['titleFor']) {
	
	$sql = "
    SELECT
        CASE WHEN
            sa.sbacronograma = 2 THEN 'Por escola'
        ELSE
            'Global'
        END as sbacronograma,
        usu.usunome,
        to_char(sa.sbadataalteracao,'dd/mm/YYYY') as data,
        coalesce(ss.ssudescricao, 'Nao informado') as ssudescricao,
        -- coalesce(fat.foadsc, 'Nao informado')      as fatendimento,
        coalesce(fex.frmdsc, 'Nao informado')      as frmexecucao
    FROM
        par.subacao sa
    LEFT JOIN
        par.subacaoparecertecnico spt on sa.sbaid = spt.sbaid AND sptano = date_part('year', current_date)
    LEFT JOIN
        par.statussubacao ss on ss.ssuid = spt.ssuid
    LEFT JOIN
        par.formaatendimento fat on fat.foaid = sa.foaid 
    LEFT JOIN
        par.formaexecucao fex on fex.frmid = sa.frmid 
    LEFT JOIN
        seguranca.usuario AS usu ON (sa.usucpf = usu.usucpf)
    WHERE
        sa.sbastatus = 'A' AND sa.sbaid = " . (integer) $_REQUEST['titleFor'];
/*
	$dados = array(
					'usunome' => 'victor benzi',
					'data'	  => '13/11/2011',
					'sbacronograma' => 'escola',
					'frmexecucao' => 'forma'
	);
*/	
	if (($dados = $db->pegaLinha($sql)) !== false) {
		 
		if($_REQUEST['titleFor']){
			$sql = 'SELECT sptano,
	    	   			   coalesce(ss.ssudescricao, \'Não informado\') as ssudescricao FROM par.subacaoparecertecnico spt
					LEFT JOIN par.statussubacao ss on ss.ssuid = spt.ssuid
					WHERE spt.sbaid = ' . (integer) $_REQUEST['titleFor'] .' ORDER BY sptano ';
			$arStatus = $db->carregar($sql);
		}
		$arStatus = (is_array($arStatus)) ? $arStatus : array();
		$stStatus = "";
		foreach ($arStatus as $status){
			$stStatus .= "&nbsp;&nbsp;&nbsp;".$status['sptano'] .': '. $status['ssudescricao']."<br />";
		}
		 
		$stStatus = ($stStatus) ? $stStatus : "&nbsp;&nbsp;&nbsp;Não Informado";
		header("Content-Type: text/html; charset=ISO-8859-1");
		echo ''
		,'Última atualização feita por '        , str_replace( "'", "\'", $dados['usunome'] ), ' em ' , $dados['data'] , '<br /><br />'
		,'<strong>Status: </strong>',  '<br />'
		,$stStatus,  '<br />'
		,'<strong>Cronograma: </strong>'        , $dados['sbacronograma'] , '<br />'
		,'<strong>Forma de execução: </strong>' , $dados['frmexecucao'];
	} else {
		echo '<em>Nenhuma atualização realizada.</em>';
	}

	die();
}

if($_REQUEST['filtros']){

	
	
	
//************** FILTROS **************//	
	// CLÁUSULAS COMUNS PARA TODOS OS FILTROS E FORMAS DE EXECUÇÃO
	if(!empty($_REQUEST['frmid'])){
		$whereInterno 	= ' AND s.frmid IN( '.$_REQUEST['frmid'].')  ';
		$whereExterno 	= ' AND ds.frmid IN( '.$_REQUEST['frmid'].')  ';
		$InnerInterno 	= ' ';
	}
	
	// CLÁUSULAS COMUNS PARA TODOS OS FILTROS E FASE DO WORKFLOW
	if(!empty($_REQUEST['esdid'])){
		$whereInterno 	= ' AND esd.esdid IN( '.$_REQUEST['esdid'].')  ';
		$whereExterno 	= ' AND ds.esdid IN( '.$_REQUEST['esdid'].')  ';
		$InnerInterno 	= ' ';
	}	
	
	switch ($_REQUEST['filtro']) {
//SUBACOES ANALISADAS **********************************************************************************//
	    case SUBACOES_ANALISADAS: 
	    	// subações com parecer e o status da subação dado 
			$InnerInterno 	.= ' INNER JOIN par.subacaodetalhe spt ON spt.sbdparecer is not null and ssuid is not null AND s.sbaid = spt.sbaid and spt.sbdano = date_part(\'year\', now() ) ';
//			$InnerInterno 	.= ' INNER JOIN par.subacaoparecertecnico spt ON spt.sptparecer is not null and ssuid is not null AND s.sbaid = spt.sbaid and spt.sptano = date_part(\'year\', now() ) ';	
		break;
//SUBACOES PENDENTES DE ANALISE ************************************************************************//
		case SUBACOES_PENDENTESDEANALISE: 
	    	// subações com parecer e o status da subação vazio 
	    	$InnerInterno 	.= ' LEFT JOIN par.subacaodetalhe spt ON s.sbaid = spt.sbaid and spt.sbdano = date_part(\'year\', now() ) ';
	    	$whereInterno 	.= ' AND spt.sbdparecer is null AND spt.ssuid is null';
//			$InnerInterno 	.= ' LEFT JOIN par.subacaoparecertecnico spt ON spt.sptparecer is null and ssuid is null AND s.sbaid = spt.sbaid and spt.sptano = date_part(\'year\', now() ) ';
		break;
//SUBACOES COM ITENS **********************************************************************************//
		case SUBACOES_COMITENS: 
	    	// subações com itens de composição
			$InnerInterno 	.= ' INNER JOIN (	SELECT  distinct sbaid  
												FROM par.subacaoitenscomposicao sic 
												WHERE icoano = date_part(\'year\', now())  
											 ) sbitens ON   sbitens.sbaid = s.sbaid ';
		break;
//SUBACOES SEM ITENS **********************************************************************************//
		case SUBACOES_SEMITENS: 
			// busca apenas pela subação de um tipo de forma de execução.
			$whereInterno 	.= ' AND s.frmid in( '.$_REQUEST['frmid'].') 
								 AND s.sbaid not in (	SELECT  distinct sbaid  
														FROM par.subacaoitenscomposicao sic 
														WHERE icoano = date_part(\'year\', now())  
													 ) ';	
		break;
//SUBACOES COM OBRAS ***********************************************************************************//
		case SUBACOES_COM_OBRAS: 
			// subações que estão cadastradas na tabelas de obras. (caso tenha na tabela de obras a subação tem obras)
			$InnerInterno 	.= 'INNER JOIN par.subacaoobra so ON so.sbaid = s.sbaid and so.sobano = date_part(\'year\', now() ) ';
		break;
//SUBACOES CONVENIADA ***********************************************************************************//
		case SUBACOES_CONVENIADA: 
	    	
		break;
//SUBACOES GERADOTERMO *********************************************************************************//
		case SUBACOES_GERADOTERMO: 
	    	
		break;
//SEM FILTRO *******************************************************************************************//
		case NULL: 
	    	
		break;
	} 
	
	$cabecalho 		= array("Ação","Localização", "Subação", "Estado da Subação");
	$obSubacoes 	= new Dimensao();
	$listaSubacoes 	= $obSubacoes->lista('array', $_SESSION['par']['itrid'], $whereExterno, $whereInterno, $InnerInterno);

	switch ($_REQUEST['frmid']) {
//AÇÕES DO PAC *******************************************************************************************//
	    case 1: //Ações do PAC 
			$arrSubacoesPAC 	= array();
			$contSubacoesPAC 	= 0;
			if(is_array($listaSubacoes)){
				foreach($listaSubacoes as $chave => $subacoes){
					$arrSubacoesPAC[$contSubacoesPAC]['acao'] 		 = '<input type="checkbox"> <img src="../imagens/alterar.gif" onclick="javascript:listarSubacao(\' '.$subacoes['sbaid'].' \');" >';
					$arrSubacoesPAC[$contSubacoesPAC]['localizacao'] = $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
					$arrSubacoesPAC[$contSubacoesPAC]['subacao'] 	 = delimitadorTexto($subacoes['sbadsc']);
					$arrSubacoesPAC[$contSubacoesPAC]['estado'] 	 = delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_1 total" value="'.$subacoes['sbaid'].'"/>';
					$contSubacoesPAC++;
				}
			}
			$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" 
										  id="1" src="../imagens/mais.gif">&nbsp;';
			
			if( $arrSubacoesPAC[0]){
				echo $db->monta_lista_array($arrSubacoesPAC,$cabecalho,100,5,"N","center",$html=array(),$arrayDeTiposParaOrdenacao=array(),$formName = "formlista",$param = Array('ordenar'=>false)); 
			}else{
				echo "<br><div style=\"font-weight: bold; text-align: center;\">Não foram encontrados subações para este filtro.</div></br>";
			}
	        break;
//ASSISTÊNCIA TÉCNICA DO MEC *******************************************************************************************//	        
	    case 2: //Assistência técnica do MEC
			$arrSubacoesAssTec 	= array();
			$contSubacoesAssTec 	= 0;
			if(is_array($listaSubacoes)){
				foreach($listaSubacoes as $chave => $subacoes){
					$arrSubacoesAssTec[$contSubacoesAssTec]['acao'] 		= '<input type="checkbox"> <img src="../imagens/alterar.gif" onclick="javascript:listarSubacao(\' '.$subacoes['sbaid'].' \');" >';
					$arrSubacoesAssTec[$contSubacoesAssTec]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
					$arrSubacoesAssTec[$contSubacoesAssTec]['subacao'] 	 	= delimitadorTexto($subacoes['sbadsc']);
					$arrSubacoesAssTec[$contSubacoesAssTec]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_2 total" value="'.$subacoes['sbaid'].'"/>';
					$contSubacoesAssTec++;
				}
			}
			$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" 
										  id="2" src="../imagens/mais.gif">&nbsp;';
			if( $arrSubacoesAssTec[0]){
				echo $db->monta_lista_array($arrSubacoesAssTec,$cabecalho,100,5,"N","center",$html=array(),$arrayDeTiposParaOrdenacao=array(),$formName = "formlista",$param = Array('ordenar'=>false)); 
			}else{
				echo "<br><div style=\"font-weight: bold; text-align: center;\">Não foram encontrados subações para este filtro.</div></br>";
			}
	        break;
//EXECUTADA PELO MUNICÍPIO *******************************************************************************************//	        
	    case 4: //Executada pelo município
			$arrSubacoesExePeloMunic 	= array();
			$contSubacoesExePeloMunic 	= 0;
			if(is_array($listaSubacoes)){
				foreach($listaSubacoes as $chave => $subacoes){
					$arrSubacoesExePeloMunic[$contSubacoesExePeloMunic]['acao'] 		= '<input type="checkbox"> <img src="../imagens/alterar.gif" onclick="javascript:listarSubacao(\' '.$subacoes['sbaid'].' \');" >';
					$arrSubacoesExePeloMunic[$contSubacoesExePeloMunic]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
					$arrSubacoesExePeloMunic[$contSubacoesExePeloMunic]['subacao'] 	 	= delimitadorTexto($subacoes['sbadsc']);
					$arrSubacoesExePeloMunic[$contSubacoesExePeloMunic]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_4 total" value="'.$subacoes['sbaid'].'"/>';
					$contSubacoesExePeloMunic++;
				}
			}
			$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" 
										  id="4" src="../imagens/mais.gif">&nbsp;';
			
			if( $arrSubacoesExePeloMunic[0]){
				echo $db->monta_lista_array($arrSubacoesExePeloMunic,$cabecalho,100,5,"N","center",$html=array(),$arrayDeTiposParaOrdenacao=array(),$formName = "formlista",$param = Array('ordenar'=>false)); 
			}else{
				echo "<br><div style=\"font-weight: bold; text-align: center;\">Não foram encontrados subações para este filtro.</div></br>";
			}
	        break;
//FINANCIAMENTO BNDES *******************************************************************************************//	        
	    case 5: //Financiamento BNDES
			$arrSubacoesFinancBNDES 	= array();
			$contSubacoesFinancBNDES 	= 0;
			if(is_array($listaSubacoes)){
				foreach($listaSubacoes as $chave => $subacoes){
					$arrSubacoesFinancBNDES[$contSubacoesFinancBNDES]['acao'] 			= '<input type="checkbox"> <img src="../imagens/alterar.gif" onclick="javascript:listarSubacao(\' '.$subacoes['sbaid'].' \');" >';
					$arrSubacoesFinancBNDES[$contSubacoesFinancBNDES]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
					$arrSubacoesFinancBNDES[$contSubacoesFinancBNDES]['subacao'] 	 	= delimitadorTexto($subacoes['sbadsc']);
					$arrSubacoesFinancBNDES[$contSubacoesFinancBNDES]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_5 total" value="'.$subacoes['sbaid'].'"/>';
					$contSubacoesFinancBNDES++;
				}
			}
			$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" 
										  id="5" src="../imagens/mais.gif">&nbsp;';
			
			if( $arrSubacoesFinancBNDES[0]){
				echo $db->monta_lista_array($arrSubacoesFinancBNDES,$cabecalho,100,5,"N","center",$html=array(),$arrayDeTiposParaOrdenacao=array(),$formName = "formlista",$param = Array('ordenar'=>false)); 
			}else{
				echo "<br><div style=\"font-weight: bold; text-align: center;\">Não foram encontrados subações para este filtro.</div></br>";
			}
	        break;
//ASSISTÊNCIA FINANCEIRA DO MEC *******************************************************************************************//	        
	    case 6: //Assistência financeira do MEC
			$arrSubacoesAssFinanceira 	= array();
			$contSubacoesAssFinanceira 	= 0;
			if(is_array($listaSubacoes)){
				foreach($listaSubacoes as $chave => $subacoes){
					$arrSubacoesAssFinanceira[$contSubacoesAssFinanceira]['acao'] 		 = '<input type="checkbox"> <img src="../imagens/alterar.gif" onclick="javascript:listarSubacao(\' '.$subacoes['sbaid'].' \');" >';
					$arrSubacoesAssFinanceira[$contSubacoesAssFinanceira]['localizacao'] = $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
					$arrSubacoesAssFinanceira[$contSubacoesAssFinanceira]['subacao'] 	 = delimitadorTexto($subacoes['sbadsc']);
					$arrSubacoesAssFinanceira[$contSubacoesAssFinanceira]['estado'] 	 = delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_6 total" value="'.$subacoes['sbaid'].'"/>';
					$contSubacoesAssFinanceira++;
				}
			}
			$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" 
										  id="6" src="../imagens/mais.gif">&nbsp;';
			
			if( $arrSubacoesAssFinanceira[0]){
				echo $db->monta_lista_array($arrSubacoesAssFinanceira,$cabecalho,100,5,"N","center",$html=array(),$arrayDeTiposParaOrdenacao=array(),$formName = "formlista",$param = Array('ordenar'=>false)); 
			}else{
				echo "<br><div style=\"font-weight: bold; text-align: center;\">Não foram encontrados subações para este filtro.</div></br>";
			}
	        break;
//ASSISTÊNCIA FINANCEIRA DO MEC - OBRAS *******************************************************************************************//	        
	    case 11: //Assistência financeira do MEC - Obras
			$arrSubacoesAssFinancObras 		= array();
			$contSubacoesAssFinancObras 	= 0;
			if(is_array($listaSubacoes)){
				foreach($listaSubacoes as $chave => $subacoes){
					$arrSubacoesAssFinancObras[$contSubacoesAssFinancObras]['acao'] 		= '<input type="checkbox"> <img src="../imagens/alterar.gif" onclick="javascript:listarSubacao(\' '.$subacoes['sbaid'].' \');" >';
					$arrSubacoesAssFinancObras[$contSubacoesAssFinancObras]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
					$arrSubacoesAssFinancObras[$contSubacoesAssFinancObras]['subacao'] 	 	= delimitadorTexto($subacoes['sbadsc']);
					$arrSubacoesAssFinancObras[$contSubacoesAssFinancObras]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_11 total" value="'.$subacoes['sbaid'].'"/>';
					$contSubacoesAssFinancObras++;
				}
			}
			$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" 
										  id="11" src="../imagens/mais.gif">&nbsp;';
			
			if( $arrSubacoesAssFinancObras[0]){
				echo $db->monta_lista_array($arrSubacoesAssFinancObras,$cabecalho,100,5,"N","center",$html=array(),$arrayDeTiposParaOrdenacao=array(),$formName = "formlista",$param = Array('ordenar'=>false)); 
			}else{
				echo "<br><div style=\"font-weight: bold; text-align: center;\">Não foram encontrados subações para este filtro.</div></br>";
			}
	        break;
//EXECUTADA PELO ESTADO *******************************************************************************************//	        
	    case 12: //Executada pelo estado
			$arrSubacoesExePeloEstado 	= array();
			$contSubacoesExePeloEstado 	= 0;
			if(is_array($listaSubacoes)){
				foreach($listaSubacoes as $chave => $subacoes){
					$arrSubacoesExePeloEstado[$contSubacoesExePeloEstado]['acao'] 			= '<input type="checkbox"> <img src="../imagens/alterar.gif" onclick="javascript:listarSubacao(\' '.$subacoes['sbaid'].' \');" >';
					$arrSubacoesExePeloEstado[$contSubacoesExePeloEstado]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
					$arrSubacoesExePeloEstado[$contSubacoesExePeloEstado]['subacao'] 	 	= delimitadorTexto($subacoes['sbadsc']);
					$arrSubacoesExePeloEstado[$contSubacoesExePeloEstado]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_12 total" value="'.$subacoes['sbaid'].'"/>';
					$contSubacoesExePeloEstado++;
				}
			}
			$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" 
										  id="12" src="../imagens/mais.gif">&nbsp;';
			
			if( $arrSubacoesExePeloEstado[0]){
				echo $db->monta_lista_array($arrSubacoesExePeloEstado,$cabecalho,100,5,"N","center",$html=array(),$arrayDeTiposParaOrdenacao=array(),$formName = "formlista",$param = Array('ordenar'=>false)); 
			}else{
				echo "<br><div style=\"font-weight: bold; text-align: center;\">Não foram encontrados subações para este filtro.</div></br>";
			}
	        break;
	}
	
	switch ($_REQUEST['esdid']) {
//ELABORACAO *******************************************************************************************//
	    case 451: //Elaboração 
			$arrSubacoesElaboracao 		= array();
			$contSubacoesElaboracao 	= 0;
			if(is_array($listaSubacoes)){
				foreach($listaSubacoes as $chave => $subacoes){
					$arrSubacoesElaboracao[$contSubacoesElaboracao]['acao'] 		 = '<input type="checkbox"> <img src="../imagens/alterar.gif" onclick="javascript:listarSubacao(\' '.$subacoes['sbaid'].' \');" >';
					$arrSubacoesElaboracao[$contSubacoesElaboracao]['localizacao']   = $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
					$arrSubacoesElaboracao[$contSubacoesElaboracao]['subacao'] 	 	 = delimitadorTexto($subacoes['sbadsc']);
					$arrSubacoesElaboracao[$contSubacoesElaboracao]['estado'] 	 	 = delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_1 total" value="'.$subacoes['sbaid'].'"/>';
					$contSubacoesElaboracao++;
				}
			}
			$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="451" src="../imagens/mais.gif">&nbsp;';
			
			if( $arrSubacoesElaboracao[0]){
				echo $db->monta_lista_array($arrSubacoesElaboracao,$cabecalho,100,5,"N","center",$html=array(),$arrayDeTiposParaOrdenacao=array(),$formName = "formlista",$param = Array('ordenar'=>false)); 
			}else{
				echo "<br><div style=\"font-weight: bold; text-align: center;\">Não foram encontrados subações para este filtro.</div></br>";
			}
	        break;
//ANALISE *******************************************************************************************//	        
	    case 452: //Análise
			$arrSubacoesAnalise 	= array();
			$contSubacoesAnalise 	= 0;
			if(is_array($listaSubacoes)){
				foreach($listaSubacoes as $chave => $subacoes){
					$arrSubacoesAnalise[$contSubacoesAnalise]['acao'] 			= '<input type="checkbox"> <img src="../imagens/alterar.gif" onclick="javascript:listarSubacao(\' '.$subacoes['sbaid'].' \');" >';
					$arrSubacoesAnalise[$contSubacoesAnalise]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
					$arrSubacoesAnalise[$contSubacoesAnalise]['subacao'] 	 	= delimitadorTexto($subacoes['sbadsc']);
					$arrSubacoesAnalise[$contSubacoesAnalise]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_2 total" value="'.$subacoes['sbaid'].'"/>';
					$contSubacoesAnalise++;
				}
			}
			$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="452" src="../imagens/mais.gif">&nbsp;';
			if( $arrSubacoesAnalise[0]){
				echo $db->monta_lista_array($arrSubacoesAnalise,$cabecalho,100,5,"N","center",$html=array(),$arrayDeTiposParaOrdenacao=array(),$formName = "formlista",$param = Array('ordenar'=>false)); 
			}else{
				echo "<br><div style=\"font-weight: bold; text-align: center;\">Não foram encontrados subações para este filtro.</div></br>";
			}
	        break;
//COOPERACAO TECNICA *******************************************************************************************//	        
	    case 453: //Geração do Termo de Cooperação Técnica
			$arrSubacoesGerTermoCooperacao 		= array();
			$contSubacoesGerTermoCooperacao 	= 0;
			if(is_array($listaSubacoes)){
				foreach($listaSubacoes as $chave => $subacoes){
					$arrSubacoesGerTermoCooperacao[$contSubacoesGerTermoCooperacao]['acao'] 			= '<input type="checkbox"> <img src="../imagens/alterar.gif" onclick="javascript:listarSubacao(\' '.$subacoes['sbaid'].' \');" >';
					$arrSubacoesGerTermoCooperacao[$contSubacoesGerTermoCooperacao]['localizacao'] 		= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
					$arrSubacoesGerTermoCooperacao[$contSubacoesGerTermoCooperacao]['subacao'] 	 		= delimitadorTexto($subacoes['sbadsc']);
					$arrSubacoesGerTermoCooperacao[$contSubacoesGerTermoCooperacao]['estado'] 			= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_4 total" value="'.$subacoes['sbaid'].'"/>';
					$contSubacoesGerTermoCooperacao++;
				}
			}
			$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="453" src="../imagens/mais.gif">&nbsp;';
			
			if( $arrSubacoesGerTermoCooperacao[0]){
				echo $db->monta_lista_array($arrSubacoesGerTermoCooperacao,$cabecalho,100,5,"N","center",$html=array(),$arrayDeTiposParaOrdenacao=array(),$formName = "formlista",$param = Array('ordenar'=>false)); 
			}else{
				echo "<br><div style=\"font-weight: bold; text-align: center;\">Não foram encontrados subações para este filtro.</div></br>";
			}
	        break;
//DILIGENCIA *******************************************************************************************//	        
	    case 454: //Diligência
			$arrSubacoesDiligencia 		= array();
			$contSubacoesDiligencia 	= 0;
			if(is_array($listaSubacoes)){
				foreach($listaSubacoes as $chave => $subacoes){
					$arrSubacoesDiligencia[$contSubacoesDiligencia]['acao'] 			= '<input type="checkbox"> <img src="../imagens/alterar.gif" onclick="javascript:listarSubacao(\' '.$subacoes['sbaid'].' \');" >';
					$arrSubacoesDiligencia[$contSubacoesDiligencia]['localizacao'] 		= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
					$arrSubacoesDiligencia[$contSubacoesDiligencia]['subacao'] 	 		= delimitadorTexto($subacoes['sbadsc']);
					$arrSubacoesDiligencia[$contSubacoesDiligencia]['estado'] 			= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_5 total" value="'.$subacoes['sbaid'].'"/>';
					$contSubacoesDiligencia++;
				}
			}
			$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="454" src="../imagens/mais.gif">&nbsp;';
			
			if( $arrSubacoesDiligencia[0]){
				echo $db->monta_lista_array($arrSubacoesDiligencia,$cabecalho,100,5,"N","center",$html=array(),$arrayDeTiposParaOrdenacao=array(),$formName = "formlista",$param = Array('ordenar'=>false)); 
			}else{
				echo "<br><div style=\"font-weight: bold; text-align: center;\">Não foram encontrados subações para este filtro.</div></br>";
			}
	        break;
//ANALISE DO GESTOR *******************************************************************************************//	        
	    case 455: //Aguardando Análise do Gestor
			$arrSubacoesAgAnaliseGestor 	= array();
			$contSubacoesAgAnaliseGestor 	= 0;
			if(is_array($listaSubacoes)){
				foreach($listaSubacoes as $chave => $subacoes){
					$arrSubacoesAgAnaliseGestor[$contSubacoesAgAnaliseGestor]['acao'] 		 = '<input type="checkbox"> <img src="../imagens/alterar.gif" onclick="javascript:listarSubacao(\' '.$subacoes['sbaid'].' \');" >';
					$arrSubacoesAgAnaliseGestor[$contSubacoesAgAnaliseGestor]['localizacao'] = $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
					$arrSubacoesAgAnaliseGestor[$contSubacoesAgAnaliseGestor]['subacao'] 	 = delimitadorTexto($subacoes['sbadsc']);
					$arrSubacoesAgAnaliseGestor[$contSubacoesAgAnaliseGestor]['estado'] 	 = delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_6 total" value="'.$subacoes['sbaid'].'"/>';
					$contSubacoesAgAnaliseGestor++;
				}
			}
			$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="455" src="../imagens/mais.gif">&nbsp;';
			
			if( $arrSubacoesAgAnaliseGestor[0]){
				echo $db->monta_lista_array($arrSubacoesAgAnaliseGestor,$cabecalho,100,5,"N","center",$html=array(),$arrayDeTiposParaOrdenacao=array(),$formName = "formlista",$param = Array('ordenar'=>false)); 
			}else{
				echo "<br><div style=\"font-weight: bold; text-align: center;\">Não foram encontrados subações para este filtro.</div></br>";
			}
	        break;
//EMPENHO *******************************************************************************************//	        
	    case 456: //Empenho
			$arrSubacoesEmpenho 		= array();
			$contSubacoesEmpenho	 	= 0;
			if(is_array($listaSubacoes)){
				foreach($listaSubacoes as $chave => $subacoes){
					$arrSubacoesEmpenho[$contSubacoesEmpenho]['acao'] 			= '<input type="checkbox"> <img src="../imagens/alterar.gif" onclick="javascript:listarSubacao(\' '.$subacoes['sbaid'].' \');" >';
					$arrSubacoesEmpenho[$contSubacoesEmpenho]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
					$arrSubacoesEmpenho[$contSubacoesEmpenho]['subacao'] 	 	= delimitadorTexto($subacoes['sbadsc']);
					$arrSubacoesEmpenho[$contSubacoesEmpenho]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_11 total" value="'.$subacoes['sbaid'].'"/>';
					$contSubacoesEmpenho++;
				}
			}
			$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="456" src="../imagens/mais.gif">&nbsp;';
			
			if( $arrSubacoesEmpenho[0]){
				echo $db->monta_lista_array($arrSubacoesEmpenho,$cabecalho,100,5,"N","center",$html=array(),$arrayDeTiposParaOrdenacao=array(),$formName = "formlista",$param = Array('ordenar'=>false)); 
			}else{
				echo "<br><div style=\"font-weight: bold; text-align: center;\">Não foram encontrados subações para este filtro.</div></br>";
			}
	        break;
//PAGAMENTO *******************************************************************************************//	        
	    case 457: //Pagamento
			$arrSubacoesPagamento 	= array();
			$contSubacoesPagamento 	= 0;
			if(is_array($listaSubacoes)){
				foreach($listaSubacoes as $chave => $subacoes){
					$arrSubacoesPagamento[$contSubacoesPagamento]['acao'] 			= '<input type="checkbox"> <img src="../imagens/alterar.gif" onclick="javascript:listarSubacao(\' '.$subacoes['sbaid'].' \');" >';
					$arrSubacoesPagamento[$contSubacoesPagamento]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
					$arrSubacoesPagamento[$contSubacoesPagamento]['subacao'] 	 	= delimitadorTexto($subacoes['sbadsc']);
					$arrSubacoesPagamento[$contSubacoesPagamento]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_12 total" value="'.$subacoes['sbaid'].'"/>';
					$contSubacoesPagamento++;
				}
			}
			$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="457" src="../imagens/mais.gif">&nbsp;';
			
			if( $arrSubacoesPagamento[0]){
				echo $db->monta_lista_array($arrSubacoesPagamento,$cabecalho,100,5,"N","center",$html=array(),$arrayDeTiposParaOrdenacao=array(),$formName = "formlista",$param = Array('ordenar'=>false)); 
			}else{
				echo "<br><div style=\"font-weight: bold; text-align: center;\">Não foram encontrados subações para este filtro.</div></br>";
			}
	        break;
//INDEFERIDA *******************************************************************************************//	        
	    case 458: //Indeferida
			$arrSubacoesIndeferida 	= array();
			$contSubacoesIndeferida 	= 0;
			if(is_array($listaSubacoes)){
				foreach($listaSubacoes as $chave => $subacoes){
					$arrSubacoesIndeferida[$contSubacoesIndeferida]['acao'] 		= '<input type="checkbox"> <img src="../imagens/alterar.gif" onclick="javascript:listarSubacao(\' '.$subacoes['sbaid'].' \');" >';
					$arrSubacoesIndeferida[$contSubacoesIndeferida]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
					$arrSubacoesIndeferida[$contSubacoesIndeferida]['subacao'] 	 	= delimitadorTexto($subacoes['sbadsc']);
					$arrSubacoesIndeferida[$contSubacoesIndeferida]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_12 total" value="'.$subacoes['sbaid'].'"/>';
					$contSubacoesIndeferida++;
				}
			}
			$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="458" src="../imagens/mais.gif">&nbsp;';
			
			if( $arrSubacoesIndeferida[0]){
				echo $db->monta_lista_array($arrSubacoesIndeferida,$cabecalho,100,5,"N","center",$html=array(),$arrayDeTiposParaOrdenacao=array(),$formName = "formlista",$param = Array('ordenar'=>false)); 
			}else{
				echo "<br><div style=\"font-weight: bold; text-align: center;\">Não foram encontrados subações para este filtro.</div></br>";
			}
	        break;
//ANALISE COMISSAO *******************************************************************************************//	        
	    case 462: //Em análise da comissão
			$arrSubacoesEmAnaliseCom 	= array();
			$contSubacoesEmAnaliseCom 	= 0;
			if(is_array($listaSubacoes)){
				foreach($listaSubacoes as $chave => $subacoes){
					$arrSubacoesEmAnaliseCom[$contSubacoesEmAnaliseCom]['acao'] 		= '<input type="checkbox"> <img src="../imagens/alterar.gif" onclick="javascript:listarSubacao(\' '.$subacoes['sbaid'].' \');" >';
					$arrSubacoesEmAnaliseCom[$contSubacoesEmAnaliseCom]['localizacao'] 	= $subacoes['dimcod'].".".$subacoes['arecod'].".".$subacoes['indcod'];
					$arrSubacoesEmAnaliseCom[$contSubacoesEmAnaliseCom]['subacao'] 	 	= delimitadorTexto($subacoes['sbadsc']);
					$arrSubacoesEmAnaliseCom[$contSubacoesEmAnaliseCom]['estado'] 		= delimitadorTexto($subacoes['esddsc']).'<input type="hidden" class="totais_12 total" value="'.$subacoes['sbaid'].'"/>';
					$contSubacoesEmAnaliseCom++;
				}
			}
			$cabecalho[3]  .= '&nbsp;<img class="totaisAno" title="Totais por ano." style="cursor:pointer;" id="462" src="../imagens/mais.gif">&nbsp;';
			
			if( $arrSubacoesEmAnaliseCom[0]){
				echo $db->monta_lista_array($arrSubacoesEmAnaliseCom,$cabecalho,100,5,"N","center",$html=array(),$arrayDeTiposParaOrdenacao=array(),$formName = "formlista",$param = Array('ordenar'=>false)); 
			}else{
				echo "<br><div style=\"font-weight: bold; text-align: center;\">Não foram encontrados subações para este filtro.</div></br>";
			}
	        break;
	}	
	die();	
}



function delimitadorTexto($texto){
	if(strlen($texto) > 180){
		$texto = substr($texto,0,180).'...';
	}
	return $texto;
}


if( $_POST['excluiSubacaoArvore'] ){
	echo excluirSubacaoArvore($_POST['sbaid']);
	die();
}

if( $_POST['excluiAcaoArvore'] ){
	echo excluirAcaoArvore($_POST['aciid']);
	die();
}

if( $_POST['regraExclusao'] ){
	echo quantidadeSubacao($_POST['aciid']);
	die();
}

include_once APPRAIZ . "includes/workflow.php";
//require_once APPRAIZ . "includes/classes/AbaAjax.class.inc";
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

if( !$db->testa_superuser() ){
	//include_once "escolasMaiseducacao.php";
}

// Este parâmetro ($arrAbas) irá servir para retirar a aba "Acompanhamento" que só deverá estar visível quando ela mesma estiver ativa
$arrAbas = array( PAR_ESCONDE_ABA_ACOMPANHAMENTO );

if( in_array( array(PAR_PERFIL_SUPER_USUARIO, PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO), $perfil )){
	$arrAbas = array( PAR_ESCONDE_ABA_ACOMPANHAMENTO, 15356 );
}

//$parm = '&inuid='.$_SESSION['par']['inuid'];
//$db->cria_aba( $abacod_tela, $url, $parm , $arrAbas);
$db->cria_aba( $abacod_tela, $url, '' , $arrAbas);
switch ($_GET['tipoDiagnostico']) {
	case 'arvore':
		$tipoDiagnostico = 'arvore';
		$checkArvore = "checked=\"checked\"";
	break;
	case 'listaAgrupada':
		$tipoDiagnostico = 'listaAgrupada';
		$checkListaAgrupada = "checked=\"checked\"";
		if($_REQUEST['visualizacaoListaAgrp']== NULL){
			$_POST['visualizacaoListaAgrp'] = 'fe';
			$checkListaAgrupadaTipofe = "checked=\"checked\"";
		}else if($_REQUEST['visualizacaoListaAgrp'] == 'fe'){
			$checkListaAgrupadaTipofe = "checked=\"checked\"";
		}else if($_REQUEST['visualizacaoListaAgrp'] == 'wf'){
			$checkListaAgrupadaTipowf = "checked=\"checked\"";
		}
		
	break;
	case 'programa':
		$tipoDiagnostico = 'programa';
		$checkPrograma = "checked=\"checked\"";
	break;
	case 'proInfancia':
		$tipoDiagnostico = 'proInfancia';
		$checkPrograma = "checked=\"checked\"";
	break;
	default:
		$tipoDiagnostico = 'arvore';
		$checkArvore = "checked=\"checked\"";
	break;
}

$obEntidadeParControle = new EntidadeParControle();

$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];

$nmEntidadePar = EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar,$_SESSION['par']['itrid']);

/*if($_SESSION['par']['arMuncod']){
	$menu[0] = array("descricao" => "Lista de Município", "link"=> "/par/par.php?modulo=principal/listaMunicipios&acao=A");
	$menu[1] = array("descricao" => ($_REQUEST['tipoDiagnostico'] == "arvore" ? "Árvore" : "Programa") , "link"=> $_SERVER["REQUEST_URI"] );
	echo montarAbasArray($menu,$_SERVER["REQUEST_URI"]);
}elseif($_SESSION['par']['arEstuf']){
	$menu[0] = array("descricao" => "Lista de Estados", "link"=> "/par/par.php?modulo=principal/listaEstados&acao=A");
	$menu[1] = array("descricao" => ($_REQUEST['tipoDiagnostico'] == "arvore" ? "Árvore" : "Programa") , "link"=> $_SERVER["REQUEST_URI"] );
	echo montarAbasArray($menu,$_SERVER["REQUEST_URI"]);
}*/
monta_titulo( $nmEntidadePar, $desc );

?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript" src="/includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<script type="text/javascript">

jQuery.noConflict();

jQuery(document).ready(function() {
	jQuery('.verificaVisualizacao').click(function() {
		return jQuery(location).attr('href', 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico='+jQuery(' input.verificaVisualizacao[type=radio]:checked').val());
	});	
	
	jQuery('[name=visualizacaoListaAgrp]').click(function() {
		return jQuery(location).attr('href', 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=listaAgrupada&visualizacaoListaAgrp='+jQuery('[name=visualizacaoListaAgrp]:checked').val());
	});	
});

function popupSubacao(sbaid)
{
	url = 'par.php?modulo=principal/popupSubacao&acao=A&anoref='+<?php echo date('Y')+1 ?>+'&sbaid='+sbaid;
	window.open(url, 'popupSubacao', "height=600,width=800,scrollbars=yes,top=50,left=200" );
}

function abrepopUpAvisos()
{
	url = 'par.php?modulo=principal/popupAvisoObras&acao=A';
	window.open(url, 'popupAvisoObras', "height=600,width=930,scrollbars=yes,top=50,left=200" );
}

function abrepopUpManual()
{
	url = 'MANUAL - REFORMULAÇÃO MI-CONVENCIONAL.pdf';
	window.open(url, 'popupAvisoObras', "height=600,width=930,scrollbars=yes,top=50,left=200" );
}

function abreObra(preid, tooid, sbaid, sobano)
{

    if( tooid == 1 ){
		url = 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&preid='+preid;
    }else{
		url = 'par.php?modulo=principal/subacaoObras&acao=A&preid='+preid+'&ano='+sobano+'&sbaid='+sbaid;
    }
	window.open(url, 'popupAvisoObras', "height=600,width=930,scrollbars=yes,top=50,left=200" );
}

function responderBaseNacional()
{
	location.href='par.php?modulo=principal/baseNacionalComum&acao=A';
}

function dadosDemograficosuf(estuf)
{	
	var estados =new Array();
	estados['AC']=12;
	estados['AL']=27;
	estados['AP']=16;
	estados['AM']=13;
	estados['BA']=29;
	estados['CE']=23;
	estados['DF']=53;
	estados['ES']=32;
	estados['GO']=52;
	estados['MA']=21;
	estados['MT']=51;
	estados['MS']=50;
	estados['MG']=31;
	estados['PA']=15;
	estados['PB']=25;
	estados['PR']=41;
	estados['PE']=26;
	estados['PI']=22;
	estados['RJ']=33;
	estados['RN']=24;
	estados['RS']=43;
	estados['RO']=11;
	estados['RR']=14;
	estados['SC']=42;
	estados['SP']=35;
	estados['SE']=28;
	estados['TO']=17;
	
	var janela = window.open( 'http://ide.mec.gov.br/2014/estados/relatorio/uf/'+estados[estuf], 'blank', 'height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
		janela.focus();
}

function visualizarConvenios(){
	url = 'par.php?modulo=principal/listaDocumentosParaAssinar&acao=A';
	window.open(url,'Visualizar Pendências de Preenchimento do Plano de Trabalho',"height=600,width=800,scrollbars=yes,top=50,left=200" );
}

function dadosDemograficosMun(muncod)
{
	var janela = window.open( 'http://ide.mec.gov.br/2014/municipios/relatorio/coibge/' + muncod + '', 'blank', 'height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
		janela.focus();
}

function visualizaPendencias(){
	url = 'par.php?modulo=principal/pendenciaDePreenchimento&acao=A';
	window.open(url,'Visualizar Pendências de Preenchimento do Plano de Trabalho',"height=600,width=800,scrollbars=yes,top=50,left=200" );
}

function visualizaTermo(terid){
	url = 'par.php?modulo=principal/termoAnalise&acao=A&terid='+terid;
	window.open(url,'Visualizar Termo de Envio de Análise',"height=600,width=800,scrollbars=yes,top=50,left=200" );
}

function abrePropostaEmenda(){
	url = 'par.php?modulo=principal/propostaEmenda&acao=A&aba=emenda&ano=2015';
	window.open(url,'Proposta Emenda',"fullscreen=yes,scrollbars=yes" );
}

function relatorioImpressao( status ){
	url = 'par.php?modulo=relatorio/impressao_par&acao=A&status='+status;
	window.open(url,'Impressão do PAR',"height=600,width=800,scrollbars=yes,top=50,left=200" );
}

function abrirsistema( sisid ) {
	location.href = "../muda_sistema.php?sisid=" + sisid;
} 

function listaSuacoesTramita(dado, sbaid, esdid){
		jQuery.ajax({
			type: "POST",
			url: "par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=listaAgrupada",
			data: "listasubacaotramitacao=1&sbaid="+sbaid+"&aedid="+dado,
			async: false,
			success: function(msg){
				jQuery('#listasubacoesPossiveisDeTramitacao_'+esdid).html(msg);
			
			}
		});
	}
function fake()
{

}

function enviarProximaFaseSub(){
		jQuery('#tramitarSubacoes').val(true);
		jQuery('#formTramita').submit();
}


</script>

<?php $perfil = pegaArrayPerfil($_SESSION['usucpf']); 
$habilitaCabec = false;
if( in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || 
		in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) || 
		in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $perfil) || 
		in_array(PAR_PERFIL_EQUIPE_TECNICA, $perfil)  || 
		in_array(PAR_PERFIL_ANALISTA_MERITOS, $perfil) || 
		in_array(PAR_PERFIL_ANALISTA_QUESTOES_PONTUAIS, $perfil) || 
		in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) || 
		in_array(PAR_PERFIL_EMPENHADOR, $perfil) ||	
		in_array(PAR_PERFIL_MANUTENCAO_TABELAS_APOIO, $perfil) ||
		in_array(PAR_PERFIL_GERADOR_DOCUMENTO, $perfil) ||
		in_array(PAR_PERFIL_PAGADOR, $perfil) ||
		in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
		in_array(PAR_PERFIL_PROFUNC_PREANALISEPF, $perfil) ||
		in_array(PAR_PERFIL_PROFUNC_ANALISEPF, $perfil) ||
		in_array(PAR_PERFIL_ALTA_GESTAO_MEC, $perfil) 
	){
	$habilitaCabec = true;
}



?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="0" align="center">
	<tr>
		<td valign="middle">
			<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="0" align="center" style="border: none;" >
		 		<?php if(!in_array(PAR_PERFIL_PROFUNC_ANALISEPF,$perfil)){?>
		 		<tr>
		 		<td width="450px"> Visualização:
		 			<input type="radio" name="visualizacao" id="arvore" value="arvore" class="verificaVisualizacao" <?php echo $checkArvore; ?> /> <label for="arvore">Árvore</label> 
		 			<input type="radio" name="visualizacao" id=programa value="programa" class="verificaVisualizacao" <?php echo $checkPrograma; ?> /> <label for="programa">Programa</label>
					<?php 					
					if( $habilitaCabec ){
					if( $_REQUEST['tipoDiagnostico'] != 'listaAgrupada' ){ ?>
							<input type="radio" name="visualizacao" id=listaAgrupada value="listaAgrupada" class="verificaVisualizacao" <?php echo $checkListaAgrupada; ?> /> <label for="listaAgrupada">Lista Agrupada</label>
				</td>
				<?php } else { ?>
		 		<td width="140px" class="SubtituloEsquerda" style="font-weight: normal;" >
                                    <input type="radio" name="visualizacao" id=listaAgrupada value="listaAgrupada" class="verificaVisualizacao" <?php echo $checkListaAgrupada; ?> /> <label for="listaAgrupada">Lista Agrupada</label>
				</td>
		 		</tr>
				<tr>
					<td nowrap="nowrap" class="SubtituloEsquerda" style="padding-left: 30px; font-weight: normal;" colspan="2">Listar por : 
						<input type="radio" name="visualizacaoListaAgrp" class="visualizacaoListaAgr"  value="fe" <?php echo $checkListaAgrupadaTipofe; ?>  /> Forma de execução
						<input type="radio" name="visualizacaoListaAgrp" class="visualizacaoListaAgr"  value="wf" <?php echo $checkListaAgrupadaTipowf; ?> /> Fase do Workflow
					</td>
					<td class="SubtituloEsquerda" style="width: 55%;">&nbsp;</td>
				</tr>
				<?php 		}
					} 
				?>
				</td>
				</tr>
				<?php } ?>
			</table>		
		</td>
		<td>
			<div style="float: right;">
				<a href="http://portal.mec.gov.br/index.php?option=com_content&view=article&id=157&Itemid=365" target="blank" >
					<img border="0" style="vertical-align: middle;" src="../imagens/livro.gif"/>
					<b>Manual para elaboração do PAR</b>
				</a>
			</div>
		</td>
	</tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-top: none;">
	<tr>
		<td valign="top">
			<?php  
                            $obEntidadeParControle->visualizacaoDiagnostico($tipoDiagnostico);
			?>
		</td>
		<?php if($tipoDiagnostico != 'listaAgrupada'){  ?>
		<td valign="top" align="right">
			
			<?php  // WORKFLOW
			if($tipoDiagnostico == 'arvore'){
					$docid = parPegarDocidParaEntidade($_SESSION['par']['inuid']);
					wf_desenhaBarraNavegacao( $docid, array('itrid' => $_SESSION['par']['itrid'], 'inuid' => $_SESSION['par']['inuid'] ) ); 
					
			?>
			
			<?php
				$perfil_usu = pegaArrayPerfil($_SESSION['usucpf']);
				
				#Regra definida em 23/10/2012. A pedido do Julia Viana.
				#Bloco de código comentado para que qualquer usuário independente de perfil possa ver o documento e "imprimi-lo".
				/*if(	in_array(PAR_PERFIL_SUPER_USUARIO,$perfil_usu) || 
					in_array(PAR_PERFIL_PREFEITO,$perfil_usu) || 
					in_array(PAR_PERFIL_EQUIPE_TECNICA,$perfil_usu) || 
					in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,$perfil_usu) || 
					in_array(PAR_PERFIL_CONSULTA,$perfil_usu) ||
					in_array(PAR_PERFIL_CONSULTA_ESTADUAL,$perfil_usu) ||
					in_array(PAR_PERFIL_CONSULTA_MUNICIPAL,$perfil_usu) || 
					in_array(PAR_PERFIL_EQUIPE_FINANCEIRA,$perfil_usu) || 
					in_array(PAR_PERFIL_ANALISTA_QUESTOES_PONTUAIS,$perfil_usu) ||
					in_array(PAR_PERFIL_COORDENADOR_GERAL,$perfil_usu) ||
					in_array(PAR_PERFIL_EMPENHADOR, $perfil) ||	
					in_array(PAR_PERFIL_MANUTENCAO_TABELAS_APOIO, $perfil) ||
					in_array(PAR_PERFIL_GERADOR_DOCUMENTO, $perfil) ||
					in_array(PAR_PERFIL_PAGADOR, $perfil) ||
					in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
					in_array(PAR_PERFIL_ALTA_GESTAO_MEC, $perfil) ||
					in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) ||
					in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil)
				){*/
				if( !in_array(PAR_PERFIL_ORGAO_DE_CONTROLE,$perfil) ){
			?>
			<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
				<tr style="background-color: #c9c9c9; text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<span title="Lista de Documentos para assinatura."><strong>Documentos</strong></span> 
                    </td>
				</tr>
				
				<tr style="text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<img src="/imagens/icones/monitora.jpg" style="cursor:pointer;" alt="Assinatura de convênios" onclick="visualizarConvenios()">
					</td>
				</tr>
			</table><br><br>
			<?php }	
	
				$dadosObrasAvisoMI = verificarProcessoObraMI($_SESSION['par']['inuid']);
				
				$sql = "SELECT
						count(pro.proid)
					FROM
						par.processoobra pro
						inner join par.instrumentounidade		inu ON ( inu.muncod = pro.muncod OR ( inu.estuf = pro.estuf AND pro.muncod IS NULL ) )
                        inner join par.processopedidoreformulacao pp on pp.proid = pro.proid and pp.pprstatus = 'A'
					WHERE
						pro.prostatus = 'A'
						and inu.inuid = {$_SESSION['par']['inuid']}";
				$boObraMiConvencional = $db->pegaUm($sql);
				
				if( (in_array(PAR_PERFIL_SUPER_USUARIO,$perfil) ||
						in_array(PAR_PERFIL_ADMINISTRADOR,$perfil) || 
						in_array(PAR_PERFIL_EQUIPE_MUNICIPAL,$perfil) || 
						in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$perfil) || 
						in_array(PAR_PERFIL_PREFEITO,$perfil) || 
						in_array(PAR_PERFIL_EQUIPE_ESTADUAL,$perfil) || 
						in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,$perfil) || 
						in_array(PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO,$perfil)) && ($dadosObrasAvisoMI == 't' || (int)$boObraMiConvencional > 0)
						){
			?>
			<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
				<tr style="background-color: #c9c9c9; text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<span title="Solicitação/Acompanhamento"><strong>Solicitação/ Acompanhamento</strong></span> 
                    </td>
				</tr>
				<tr style="text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<a style="cursor: pointer" onclick="javascript: location.href = 'par.php?modulo=principal/pedidoReformulacaoObras&acao=A';" title="Solicitação MI para convencional">Solicitação MI para convencional</a>
					</td>
				</tr>
			</table><br><br>
			<?php }
				//Regra atribuida devido estado atual ser Elaboração e Diagnóstico
				include_once APPRAIZ ."includes/workflow.php";
				$arrEstado = wf_pegarEstadoAtual($docid);
				if($arrEstado['esdid'] != 314 && ($arrEstado['esdid'] != 313)){
				if( (in_array(PAR_PERFIL_SUPER_USUARIO,$perfil_usu) || 
					in_array(PAR_PERFIL_ADMINISTRADOR,$perfil_usu) || 
//					in_array(PAR_PERFIL_EQUIPE_TECNICA,$perfil_usu) || 
//					in_array(PAR_PERFIL_PREFEITO,$perfil_usu) || 
					in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil) ||
					in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) ||
					in_array(PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO, $perfil) ||
//					in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) ||
//					in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) || 
					in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $perfil) ||
                    in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $perfil) ||
                    in_array($_SESSION['usucpf'], array('', '')) )
				){ ?>
			<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
				<?php
				

				?>
				<tr style="background-color: #c9c9c9; text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<span title="Atualização do PAR"><strong>Atualização</strong></span> 
                    </td>
				</tr>
				
				<tr style="text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<a style="cursor: pointer" onclick="javascript: location.href = 'par.php?modulo=principal/justificativaAtualizacaoPAR&acao=A';" title="Atualização do PAR">Atualização do PAR</a>
					</td>
				</tr>
			</table><br><br>
				<?php } ?>
			<?php } ?>
			<?php 
			if( in_array(PAR_PERFIL_SUPER_USUARIO,$perfil_usu) || 
					in_array(PAR_PERFIL_ADMINISTRADOR,$perfil_usu) || 
					in_array(PAR_PERFIL_EQUIPE_TECNICA,$perfil_usu) || 
					in_array(PAR_PERFIL_PREFEITO,$perfil_usu) || 
					in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil_usu) ||
					in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil_usu) ||
					in_array(PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO, $perfil_usu) ||
					in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil_usu) ||
					in_array(PAR_PERFIL_CONSULTA, $perfil_usu) ||
					in_array(PAR_PERFIL_CONSULTA_ESTADUAL, $perfil_usu) ||
                    in_array(PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL, $perfil_usu) ||
					in_array(PAR_PERFIL_PARLAMENTAR , $perfil_usu) ||
					in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $perfil_usu) ||
                    in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $perfil_usu) ||
					in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil_usu)
				){ ?>
			<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
				<?php
				

				?>
				<tr style="background-color: #c9c9c9; text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<span title="Atualização do PAR"><strong>EMENDAS</strong></span> 
                    </td>
				</tr>
				
				<tr style="text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<a style="cursor: pointer" onclick="abrePropostaEmenda();" title="Propostas de Emendas">Propostas de Emendas</a>
					</td>
				</tr>
			</table><br><br>
			<?php } 
			$sql = "select usucpf from seguranca.usuario_sistema where usucpf = '".$_SESSION['usucpf']."' AND sisid = 13 AND susstatus = 'A'";
			if( 
				$db->pegaUm( $sql ) 
				&& !in_array(PAR_PERFIL_ORGAO_DE_CONTROLE,$perfil)
				){
			
			?>
			<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
				<tr style="background-color: #c9c9c9; text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<span title="Lista de Documentos para assinatura."><strong>PAR</strong></span> 
                    </td>
				</tr>
				
				<tr style="text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<a style="cursor: pointer" onclick="abrirsistema(13);" title="Abrir o PAR - Plano de Metas">Abrir o PAR Plano de Metas</a>
					</td>
				</tr>
			</table><br><br>
						
			
			<?php  } 
					$docid = parPegarDocidParaEntidade($_SESSION['par']['inuid']);
					$estadoAtual = wf_pegarEstadoAtual( $docid );
					
					if( $estadoAtual['esdid'] == WF_ANALISE ){
						if( in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || 
							in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)|| 
							in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) || 
							in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil)|| 
							in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil)|| 
							in_array(PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO, $perfil)|| 
							in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil)|| 
							in_array(PAR_PERFIL_EQUIPE_TECNICA, $perfil)|| 
							in_array(PAR_PERFIL_ANALISTA_MERITOS, $perfil)|| 
							in_array(PAR_PERFIL_PREFEITO, $perfil) ||
							in_array(PAR_PERFIL_ALTA_GESTAO_MEC, $perfil)
						){
						?>
							<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
								<tr style="background-color: #c9c9c9; text-align: center;">
									<td style="font-size: 7pt; text-align: center;">
										<span title="Lista de Documentos para assinatura."><strong>Diligência</strong></span> 
				                    </td>
								</tr>
								
								<tr style="text-align: center;">
									<td style="font-size: 7pt; text-align: center;">
										<a style="cursor: pointer" onclick="return window.open('par.php?modulo=principal/visualizarAnaliseDiligencia&acao=A','Visualizar Diligência','width=650,height=500,scrollbars=1');" title="Abrir">Abrir</a>
									</td>
								</tr>
							</table><br><br>

						<?php
						}
					}
					
					if($estadoAtual["esdid"] == WF_ELABORACAO || in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || 
						in_array( PAR_PERFIL_ADMINISTRADOR, $perfil )){ 
				?>
				<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
					<tr style="background-color: #c9c9c9; text-align: center;">
						<td style="font-size: 7pt; text-align: center;">
							<span title="Pendências de Preenchimento do Plano de Trabalho"><strong>Pendências</strong></span> 
	                    </td>
					</tr>
					
					<tr style="text-align: center;">
						<td style="font-size: 7pt; text-align: center;">
							<a style="cursor: pointer" onclick="visualizaPendencias();" title="Visualizar Pendências de Preenchimento do Plano de Trabalho">Visualizar Pendências</a>
						</td>
					</tr>
				</table><br>
				
				<?php
					}
					if( in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || 
						in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) || 
						in_array( PAR_PERFIL_EQUIPE_TECNICA, $perfil ) ||
						in_array( PAR_PERFIL_ALTA_GESTAO_MEC, $perfil )
					){
				?>
				<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
				<tr style="background-color: #c9c9c9; text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<span title="Lista de Documentos para assinatura."><strong>Controle de Diligência</strong></span> 
                    </td>
				</tr>
				<tr style="text-align: center;">
					<td style="font-size: 7pt; text-align: center;">
						<a style="cursor: pointer" onclick="return window.open('par.php?modulo=principal/visualizarControleDiligencia&acao=A','Visualizar Controle de Diligência','width=650,height=500,scrollbars=1');" title="Abrir">Abrir</a>
					</td>
				</tr>
			</table><br><br>
				
				<?php	
					}
				//	$podeEnviarAnalise = array(1907,4115,729,1606,4,2824,1734);
				//	if(in_array($_SESSION['par']['inuid'],$podeEnviarAnalise) ){
					if($estadoAtual['esdid'] == WF_ELABORACAO || $estadoAtual['esdid'] == WF_ANALISE ){
						if(	in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || 
							in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) ||
							in_array( PAR_PERFIL_CONSULTA, $perfil )||  
							in_array( PAR_PERFIL_CONSULTA_ESTADUAL, $perfil )||
                            in_array( PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL, $perfil )||
                            in_array( PAR_PERFIL_CONSULTA_MUNICIPAL,$perfil) ||
                            in_array( PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL,$perfil) ||
                            in_array( PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil ) ||
							in_array( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil )||
							in_array( PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO, $perfil )||
							in_array( PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil ) ||  
							in_array( PAR_PERFIL_PREFEITO, $perfil )|| 
							in_array( PAR_PERFIL_EQUIPE_FINANCEIRA, $perfil) || 
							in_array( PAR_PERFIL_ANALISTA_QUESTOES_PONTUAIS, $perfil) ||
							in_array( PAR_PERFIL_EQUIPE_TECNICA, $perfil) ||
							in_array(PAR_PERFIL_COORDENADOR_GERAL ,$perfil) ||
							in_array(PAR_PERFIL_EMPENHADOR, $perfil)||	
							in_array(PAR_PERFIL_MANUTENCAO_TABELAS_APOIO, $perfil) ||
							in_array(PAR_PERFIL_GERADOR_DOCUMENTO, $perfil) ||
							in_array(PAR_PERFIL_PAGADOR, $perfil) ||
							in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
							in_array(PAR_PERFIL_ALTA_GESTAO_MEC, $perfil)
						){
/*					
						if(	!(
								in_array(PAR_PERFIL_COORDENADOR_TECNICO, $perfil ) ||
								in_array(PAR_PERFIL_ANALISE_PI, $perfil )||
								in_array(PAR_PERFIL_PROFUNC_PREANALISEPF, $perfil )||
								in_array(PAR_PERFIL_PROFUNC_ANALISEPF,$perfil_usu) ||
								in_array(PAR_PERFIL_PROFUNC_PREANALISEPF_TUTOR, $perfil ) ||
								in_array(PAR_PERFIL_PROFUNC_ANALISEPF_TUTOR, $perfil )||
								in_array(PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, $perfil )||
								in_array(PAR_PERFIL_ANALISTA_MERITOS, $perfil)
							)
						){*/
							$arqid = $db->pegaUm( "SELECT arqid FROM par.protocolo WHERE terid = 3 AND inuid = ".$_SESSION['par']['inuid'] );
							if( $arqid ){
								$textoAnalise = "Visualizar Protocolo";
								$liberado = true;
							}else{
								$textoAnalise = "Enviar para Análise";
								$liberado = false;
							}
							if(	in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || 
								in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) ||
								in_array( PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil ) ||
								in_array( PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil ) ||  
								in_array( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil )|| 
								in_array( PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO, $perfil )|| 
								in_array( PAR_PERFIL_PREFEITO, $perfil ) 
							){
								$liberado = true;
							}
				?>
				<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
					<tr style="background-color: #c9c9c9; text-align: center;">
						<td style="font-size: 7pt; text-align: center;">
							<span title="Pendências de Preenchimento do Plano de Trabalho"><strong>Análise</strong></span> 
	                    </td>
					</tr>
					
					<tr style="text-align: center;">
						<td style="font-size: 7pt; text-align: center;">
							<?php
								if( $liberado == true ){
							?>
								<a href="" onclick="visualizaTermo(3);" title="<?=$textoAnalise ?>"><?=$textoAnalise ?></a>
							<?php } else { ?>
								<b style="color: #909090"><?=$textoAnalise ?></b>
							<?php } ?>
						</td>
					</tr>
				</table><br><br>
			<?php } } 
			
				$arqid = $db->pegaUm( "SELECT arqid FROM par.protocolo WHERE terid = 1 AND inuid = ".$_SESSION['par']['inuid'] );
				if( $arqid ){
					$textoAnaliseBP = "Visualizar Protocolo";
					$liberadoBP = true;
				}else{
					$textoAnaliseBP = "Enviar para Análise";
					$liberadoBP = false;
				}
				if(	in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || 
					in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) ||
					in_array( PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil ) ||
					in_array( PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil ) ||  
					in_array( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil )|| 
					in_array( PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO, $perfil )|| 
					in_array( PAR_PERFIL_PREFEITO, $perfil ) 
				){
					$liberadoBP = true;
				}
				if($_SESSION['par']['itrid'] == 1){
				?>
				<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
					<tr style="background-color: #c9c9c9; text-align: center;">
						<td style="font-size: 7pt; text-align: center;">
							<span title="Pendências de Preenchimento do Plano de Trabalho"><strong>Brasil Pró</strong></span> 
	                    </td>
					</tr>
					
					<tr style="text-align: center;">
						<td style="font-size: 7pt; text-align: center;">
							<?php
								if( $liberadoBP == true ){
							?>
								<a href="" onclick="visualizaTermo(1);" title="<?=$textoAnaliseBP ?>"><?=$textoAnaliseBP ?></a>
							<?php } else { ?>
								<b style="color: #909090"><?=$textoAnaliseBP ?></b>
							<?php } ?>
						</td>
					</tr>
				</table><br><br>
			<?php } $perfil = pegaArrayPerfil($_SESSION['usucpf']);
			// if( $estadoAtual['esdid'] == WF_ANALISE ){ Desfeito a pedido da Chris ( 28/03/2012 )
				if( in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || 
					in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) || 
					in_array( PAR_PERFIL_EQUIPE_TECNICA, $perfil) || 
					in_array(PAR_PERFIL_ALTA_GESTAO_MEC, $perfil)
				){
				?>
					<table border="0" cellpadding="3" cellspacing="0"
						style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
						<tr style="background-color: #c9c9c9; text-align: center;">
							<td style="font-size: 7pt; text-align: center;"><span
								title="Cadastramento de parecer"><strong>PTA</strong></span> <script
								type="text/javascript">
							function visualizarPTA(){
								window.open('http://<?=$_SERVER['SERVER_NAME']?>/par/par.php?modulo=principal/pta/visualizarPTA&acao=A','Visualizar PTA','width=500,height=300,scrollbars=1');
							}
						</script></td>
						</tr>
						<tr style="text-align: center;">
							<td style="font-size: 7pt; text-align: center;"><a
								href="javascript:void(0);" onclick="return visualizarPTA();"
								title="Visualizar PTA">Visualizar PTA</a></td>
						</tr>
					</table>
					<br />
				<?php
				}
				if(in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array( PAR_PERFIL_ADMINISTRADOR, $perfil )){ ?>
					<table border="0" cellpadding="3" cellspacing="0"
						style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
						<tr style="background-color: #c9c9c9; text-align: center;">
							<td style="font-size: 7pt; text-align: center;"><span
								title="Cadastramento de parecer"><strong>Visualização</strong></span> <script
								type="text/javascript">
							function visualizarDadosPref(){
								window.open('http://<?=$_SERVER['SERVER_NAME']?>/par/prefeitos/prefeitos.php?muncod=<?=$_SESSION['par']['muncod'] ?>','Visualizar PTA','width=760,height=680,scrollbars=1');
							}
						</script></td>
						</tr>
						<tr style="text-align: center;">
							<td style="font-size: 7pt; text-align: center;"><a
								href="javascript:void(0);" onclick="return visualizarDadosPref();"
								title="Visualizar Dados">Ações do MEC</a></td>
						</tr>
					</table>
					<br />
				<?php }
			//}
			if($estadoAtual['esdid'] == WF_ELABORACAO || $estadoAtual['esdid'] == WF_ANALISE ){
				if(	in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || 
							in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) ||
							in_array( PAR_PERFIL_CONSULTA, $perfil )||  
							in_array( PAR_PERFIL_CONSULTA_ESTADUAL, $perfil )||
                            in_array( PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL, $perfil )||
							in_array( PAR_PERFIL_EQUIPE_FINANCEIRA, $perfil) || 
							in_array(PAR_PERFIL_COORDENADOR_GERAL ,$perfil) ||
							in_array(PAR_PERFIL_EMPENHADOR, $perfil)||	
							in_array(PAR_PERFIL_GERADOR_DOCUMENTO, $perfil) ||
							in_array(PAR_PERFIL_PAGADOR, $perfil) ||
							in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
							in_array( PAR_PERFIL_EQUIPE_ESTADUAL, $perfil )|| 
							in_array( PAR_PERFIL_EQUIPE_ESTADUAL_BRASIL_PRO, $perfil )|| 
							in_array( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil )|| 
							in_array( PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil )||
							in_array( PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil )||
							in_array( PAR_PERFIL_PREFEITO, $perfil )||
							in_array(PAR_PERFIL_ALTA_GESTAO_MEC, $perfil) ||
							in_array(PAR_PERFIL_PROFUNC_ANALISEPF, $perfil) ||
							in_array(PAR_PERFIL_PROFUNC_PREANALISEPF, $perfil) ||
							in_array(PAR_PERFIL_MANUTENCAO_TABELAS_APOIO, $perfil)
							|| in_array(PAR_PERFIL_ORGAO_DE_CONTROLE,$perfil)
				){			
					
			?>
			<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
				<tr style="background-color: #c9c9c9; text-align: center;">
					<td style="font-size: 7pt; text-align: center;" colspan="2">
						<span title="Pendências de Preenchimento do Plano de Trabalho"><strong>Impressão</strong></span> 
                    </td>
				</tr>
				<tr style="text-align: center;">
					<td style="font-size: 7pt; text-align: left;">
						Atual: 
					</td>
					<td style="font-size: 7pt; text-align: left;">
						<a style="cursor: pointer" title="Versão para impressão (Atual)" onclick="return false;">
							<img src="/imagens/print.png" alt="Versão para impressão" onclick="relatorioImpressao('A')">
						</a>
					</td>
				</tr>
				<?php if( $estadoAtual['esdid'] == WF_ANALISE ){?>
				<tr style="text-align: center;">
					<td style="font-size: 7pt; text-align: left;">
						Planejado: 
					</td>
					<td style="font-size: 7pt; text-align: left;">
						<a style="cursor: pointer" title="Versão para impressão (Planejado)" onclick="return false;">
							<img src="/imagens/print.png" alt="Versão para impressão" onclick="relatorioImpressao('C')">
						</a>
					</td>
				</tr>
				<?php }?>
			</table>
		<?php } }	}  } ?>
		</td>
	</tr>
</table>



