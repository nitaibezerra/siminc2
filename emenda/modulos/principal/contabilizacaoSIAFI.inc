<?php

if( $_REQUEST['popup'] ){
	ob_clean();
	$abtcodigo = $_REQUEST['abtcodigo'];
	$abtconteudo = $_SESSION['emendas']['abtconteudo'];
	$_SESSION['emendas']['boGerar'] = true;
	define( "K_NR_CODIGO_UG_RESPONSAVEL", 153173 );
	header("Content-Type: text/html; charset=ISO-8859-1");
	header("Content-type: application/octet-stream");
	$nomeArquivo = "CV".K_NR_CODIGO_UG_RESPONSAVEL.".".$abtcodigo;
	header("Content-Disposition: attachment; filename=\"$nomeArquivo\"");
	echo $abtconteudo;
	unset($_SESSION['emendas']['abtconteudo']);
	exit;
}


switch ( $_REQUEST["requisicao"] ){
	
	case 'filtrapesquisa':
		$filtros = filtroPtaAnalise( $_REQUEST );
	break;
	case 'downloadPorPtrid':
		downloadArquivoSIAFIporPtrid( $_REQUEST['ptridD'] );
	break;
	case 'download':
		downloadArquivoSIAFI( $_REQUEST['ptridD'] );
	break;
	
}

if(count($_POST['chkSIAFI']) && (!isset($_SESSION['emendas']['boGerar']) || !$_SESSION['emendas']['boGerar']) ){
	downloadArquivoConversaoConvenioSIAFI($_POST['chkSIAFI']);
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );

// Cria o título da tela
monta_titulo( 'Conversao Convenio no SIAFI', $subtitulo_modulo );
//dbg($_POST['chkSIAFI']);
?>
<script type="text/javascript" src="/includes/JQuery/jquery.js"></script>
<script>
	function pesquisaPTAAnalise(){
		enbnome	=	document.getElementsByName("enbnome")[0];
		
		if(enbnome.value.length > 0 && enbnome.value.length < 3) {
			alert("Para realizar a busca por nome do município é necessário informar pelo menos 3 caracteres.");
			enbnome.focus();
			btnPesquisar.disabled 	= false;
			return;
		}
		document.getElementById('formulario').submit();
	}
	function gerarArquivoSIAFI(){
		var inputs = $("#geraConvenio :checkbox").serialize();
		$("#geraConvenio").submit();
	}
	function marcarTodos(){
		if(document.getElementById("chkMarcarTodos").checked == true){
			$("#geraConvenio :checkbox").attr("checked","checked");
		}else{
			$("#geraConvenio :checkbox").attr("checked","");
		}
	}
	function exibeArquivoConversaoPorPtrid(ptrid){
		window.location.href='emenda.php?modulo=principal/contabilizacaoSIAFI&acao=A&requisicao=downloadPorPtrid&ptridD=' + ptrid;
	}
	function exibeArquivoConversao(ptrid){
		window.location.href='emenda.php?modulo=principal/contabilizacaoSIAFI&acao=A&requisicao=download&ptridD=' + ptrid;
	}

	function visualizarAnalise( ptrid ){
		window.open("?modulo=principal/informacoesGerais&acao=A&ptridAnalise=" + ptrid+'&tipo=popup', "PTA","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=800,height=600");
	}
		
	<? if(count($_SESSION['emenda']['chkSIAFI'])>0) : ?>
	$(document).ready(function() {
		<? foreach(array_keys($_SESSION['emenda']['chkSIAFI']) as $ptrid) { ?>
			document.getElementById('idChkSIAFI_<? echo $ptrid; ?>').checked = true;
		<? } ?>
	});
	<? unset($_SESSION['emenda']['chkSIAFI']); ?>
	<? endif;?>
		
		
</script>
<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="filtrapesquisa">
	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<td width=18% class="subtitulodireita">Número do PTA:</td>
			<td colspan="3">
				<?php
					$ptrid = $_REQUEST['ptrid']; 
					echo campo_texto( 'ptrid', 'N', 'S', '', 27, 27, '[#]', '', '', '', 0, '' );
				?>
			</td>
		<tr>
		<tr>
			<td class="subtitulodireita">Autor:</td>
			<td colspan="3">
				<?php
					$autid = $_REQUEST['autid'];
					$sql = "SELECT 
							  autid as codigo,
							  autnome as descricao
							FROM 
							  emenda.autor
							WHERE 
							  autstatus = 'A'
							ORDER BY autnome";
					$db->monta_combo( "autid", $sql, 'S', 'Selecione...', '', '', '', '265' );
				?>
			</td>
		<tr>
		<tr>
			<td class="subtitulodireita">Responsável:</td>
			<td colspan="3">
				<?php
					$resid = $_REQUEST['resid'];
					echo comboResponsavel($_SESSION['usucpf'], $resid, 'verificaResponsavel(this)');
				?>
			</td>
		<tr>
		<tr>
			<td class="subtitulodireita">CNPJ:</td>
			<td>
				<?php
					$enbcnpj = $_REQUEST['enbcnpj']; 
					echo campo_texto( 'enbcnpj', 'N', 'S', '', 27, 20, '', '', '', '', 0, '', "this.value=mascaraglobal('##.###.###/####-##',this.value);" );
				?>
			</td>
			<td class="subtitulodireita" width="190px">Nome do Órgão ou Entidade:</td>
			<td>
				<?php
					$entnome = $_REQUEST['enbnome']; 
					echo campo_texto( 'enbnome', 'N', 'S', '', 35, 50, '', '', '', '', 0, '' );
				?>
			</td>
		<tr>
		<tr>
			<td class="subtitulodireita">UF:</td>
			<td>
				<?php
					$estuf = $_REQUEST['estuf'];
					$sql = "SELECT
								estuf AS codigo,
								estdescricao AS descricao
							FROM
								territorios.estado
							order by descricao";
					$db->monta_combo( "estuf", $sql, 'S', 'Selecione...', '', '', '', '150' );
				?>
			</td>
			<td class="subtitulodireita">Município:</td>
			<td>
				<?php
					$mundescricao = $_REQUEST['mundescricao']; 
					echo campo_texto( 'mundescricao', 'N', 'S', '', 35, 50, '', '', '', '', 0, '' );
				?>
			</td>
		<tr>
			<td class="subtitulodireita">Situação da geração do arquivo:</td>
			<td>
				<?php
					$situacao = $_REQUEST['situacao'];
					
					$arrSituacao = array ( array("codigo" => "1", "descricao" => "Gerado arquivo"), array("codigo" => "2", "descricao" => "Não Gerado arquivo")  );
					
					$db->monta_combo( "situacao", $arrSituacao , 'S', 'Selecione...', '', '', '', '150' );
				?>
			</td>
		<tr>
		<tr>
			<td class="subtitulodireita">Situação do Convênio no SIAFI:</td>
			<td>
				<?php
					$sitconvenio = $_REQUEST['sitconvenio'];
					
					$arrSituacaoConvenio = array ( array("codigo" => "1", "descricao" => "Convertido"), array("codigo" => "2", "descricao" => "Não Convertido")  );
					
					$db->monta_combo( "sitconvenio", $arrSituacaoConvenio , 'S', 'Selecione...', '', '', '', '150' );
				?>
			</td>
		<tr>
		<tr>
			<td class="subtitulodireita">Nome do Arquivo:</td>
			<td>
				<?php
					$nomearq = $_REQUEST['nomearq'];
					$sql = "select 
								'CV153173.' || substr( date_part('year',abtdatageracao)::varchar(4) , 3,2) || abtcodigo || ' - ' || to_char(date(abtdatageracao), 'DD/MM/YYYY') as descricao
							, abtid as codigo
							from emenda.arquivobtsiafi
							order by abtdatageracao";
					$db->monta_combo( "nomearq", $sql, 'S', 'Selecione...', '', '', '', '250' );
				?>
			</td>
		<tr>
		<tr>
			<td class="subtitulodireita">Possui Pendências:</td>
			<td >
				<input type="radio" value="S" id="pendencia" name="pendencia" <? if($_REQUEST["pendencia"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="pendencia" name="pendencia" <? if($_REQUEST["pendencia"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value="" id="pendencia" name="pendencia"  <? if($_REQUEST["pendencia"] == "") { echo "checked"; } ?> /> Todos
			</td>
		</tr>
		
		<tr bgcolor="#D0D0D0">
			<td><input type="checkbox" name="chkMarcarTodos" id="chkMarcarTodos" onclick="marcarTodos();" style="cursor: pointer;"> Marcar Todos / Desmarcar Todos</td>
			<td colspan="3">
				<input type="button" value="Pesquisar" name="btnPesquisar" id="btnPesquisar" onclick="pesquisaPTAAnalise();" style="cursor: pointer;">
			</td>
		</tr>
	</table>
</form>
<form action="" method="post" name="geraConvenio" id="geraConvenio">
<?php listaPtaGeraConvenioSIAFI( $filtros ); ?>
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
<tr bgcolor="#D0D0D0">
			<td align="center">
				<input type="button" value="Gerar arquivo conversão" name="btnConversao" id="btnConversao" onclick="gerarArquivoSIAFI()" style="cursor: pointer;">
			</td>
		</tr>
</table>
</form>
<?php


function listaPtaGeraConvenioSIAFI( $filtros = '' ){
	global $db;
	
	if(pegaPerfil($_SESSION["usucpf"]) == ADMINISTRADOR_INST){
		$resid 		 = recuperaResponsaveis( $_SESSION["usucpf"] );
		$filtroResid = count($resid) > 0 ? " AND (res.resid in (" . implode( ", ", $resid ) . ") OR res.resid is null)" : "";
	}


	$sql = "SELECT DISTINCT
				CASE 
					WHEN ptmin.pmcdataconversaosiafi is not null 
						THEN '<center><img title=\"Visualizar arquivo de conversão\" onclick=\"exibeArquivoConversao(' || ptr.ptrid || ');\" style=\"cursor:pointer\" src=\"../imagens/anexo.gif\" /></center>'
					WHEN ptmin.pmcnumconveniosiafi is not null
						THEN '<center><input type=\"checkbox\" id=\"idChkSIAFI_' || ptr.ptrid || '\" name=\"chkSIAFI[' || ptr.ptrid || ']\" /></center>'
					ELSE '<center><input type=\"checkbox\" name=\"chkSIAFI[' || ptr.ptrid || ']\" checked=\"checked\" /></center>' 
					END as acoes,
				
					'<a href=\"javascript:visualizarAnalise( '|| ptr.ptrid || ' );\">' || ptr.ptrcod || '/' || ptr.ptrexercicio || '</a>' as pta,
				ptr.ptrnumconvenio||'/'||ptr.ptranoconvenio as numconvenio,
				case when trim(ptmin.pmcresolucao)='' then ptr.ptrjustificativa else 'resolução ' || ptmin.pmcresolucao end as ptrjustificativa,
				ent.enbcnpj,
				ent.enbnome,
				coalesce(ent.enblog,'') || ' ' || coalesce(ent.enbnum,'') || ' ' || coalesce(ent.enbbai,'') as endereco, 
				ent2.entnumcpfcnpj as entnumcpfcnpjresp,
				ent2.entnome as entnomeresp,
				mun.estuf,
				mun.muncodsiafi || ' - ' || mun.mundescricao as mundescricao,
				res.resassunto,
				v.ptivalorproponente,
				v.ptivalorconcedente,
				v.ptivalortotal,
				ptmin.pmcdataconversaosiafi,
				to_char(date(ptv.vigdatainicio), 'DD/MM/YYYY') as pmcdatainicio,
				to_char(date(ptp.pubdatapublicacao), 'DD/MM/YYYY') as pmcdatapublicacao,
				to_char(date(ptmin.pmcdataassinatura), 'DD/MM/YYYY') as pmcdataassinatura,
				to_char(date(ptmin.pmcdataconversaosiafi), 'DD/MM/YYYY') as pmcdataconversaosiafi,
				CASE WHEN ptmin.pmcdataconversaosiafi is not null 
					THEN '<center><a href=\"javascript:exibeArquivoConversao(' || ptr.ptrid || ')\" >' || 'CV153173.' || substr( date_part('year',arq.abtdatageracao)::varchar(4) , 3,2) || arq.abtcodigo || ' </a></center>'
					ELSE 'N/A'
				END as codigo,
				ptmin.pmcnumconveniosiafi || '/' || date_part('year',ptmin.pmcdtatualizaconveniosiafi) as pmcnumconveniosiafi,
				ptr.ptrid,
				emenda.verificapendencias(ptr.ptrid) as pendencias
			FROM emenda.planotrabalho ptr
			INNER JOIN emenda.v_planotrabalho v on v.ptrid=ptr.ptrid 
			INNER JOIN emenda.ptminutaconvenio ptmin ON ptmin.ptrid = ptr.ptrid and ptmin.pmcstatus = 'A'
			INNER JOIN emenda.ptvigencia ptv on ptv.ptrid = ptr.ptrid and ptv.vigstatus = 'A'
            INNER JOIN emenda.ptpublicacao ptp on ptp.pmcid = ptmin.pmcid and ptp.pubstatus = 'A'
			INNER JOIN emenda.entidadebeneficiada ent ON ptr.enbid = ent.enbid
			INNER JOIN entidade.entidade ent2 ON ptr.entiddirigente = ent2.entid  
			INNER JOIN territorios.municipio mun ON mun.muncod = ent.muncod 
			INNER JOIN emenda.responsavel res ON ptr.resid = res.resid
			INNER JOIN workflow.documento doc ON doc.docid = ptr.docid
			LEFT JOIN
				emenda.arquivobtsiafiplanotrabalho aptr 
			 	INNER JOIN emenda.arquivobtsiafi arq ON aptr.abtid = arq.abtid
			ON aptr.ptrid = ptr.ptrid 
			WHERE
			    ptr.ptrstatus = 'A'
			    AND doc.esdid = ".EM_PRE_CONVENIO."
			    AND ptv.vigdatainicio is not null
			    AND ptp.pubdatapublicacao is not null
			    AND ptmin.pmcdataassinatura is not null 
			    AND ptr.ptrexercicio = ".$_SESSION['exercicio']."
			    {$filtros}
		    	{$filtroResid}	
		    	-- retirar depois
		    	-- AND ptrnumconvenio IN (select ptrnumconvenio from carga.convenios )	
				-- fim do retirar	    
			ORDER BY 
				ptr.ptrid,
				mun.estuf,
				ent.enbnome";

	$arDados = $db->carregar($sql);
	
	if( $arDados ){
		$registro = array();
		$arDadosArray = array();

		foreach ($arDados as $key => $value) {
			$sqlItens = "SELECT	e.espnome,
				  substr(e.espunidademedida,0,6) as espunidademedida,
				  ptie.ptequantidade as pteqtd
				FROM 
					emenda.ptiniciativa pti
					INNER JOIN emenda.ptiniciativaespecificacao ptie ON pti.ptiid = ptie.ptiid
					INNER JOIN emenda.iniciativaespecificacao ie ON (ptie.iceid = ie.iceid) 
					INNER JOIN emenda.especificacao e ON (ie.espid = e.espid)
					inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
				WHERE 
				    --ie.icestatus = 'A' 
				    /*AND e.espstatus = 'A'
				    and*/ ptie.ptestatus = 'A'
				    AND pti.ptrid = ".$value['ptrid'];
			
			$itens = monta_lista_itens($sqlItens,array("Descr.","Unid.","qtd."));
			
			$registro = array( "acao" => $value['acoes'],
								"numconvenio" => $value['numconvenio'],
								"ptrid" => $value['pta'],
							   "enbcnpj" => $value['enbcnpj'],
							   "enbnome" => $value['enbnome'],
								"endereco" => strtoupper(removeAcentos($value['endereco'])),
			 					"mundescricao" => strtoupper(removeAcentos($value['mundescricao'])),
								 "estuf" => $value['estuf'],
								"entnumcpfcnpjresp" => $value['entnumcpfcnpjresp'],
								"entnomeresp" => strtoupper(removeAcentos($value['entnomeresp'])),			
							  	"itens" => $itens,
								"justificativa" => substr(strtoupper(removeAcentos(removeEnter($value['ptrjustificativa']))),0,350),
							   "resassunto" => $value['resassunto'],
								"ptivalorproponente" => $value['ptivalorproponente'],
								"ptivalorconcedente" => $value['ptivalorconcedente'],
								"ptivalortotal" => $value['ptivalortotal'],	
								"pmcdatainicio" => $value['pmcdatainicio'],
								"pmcdataassinatura" => $value['pmcdataassinatura'],
								"pmcdatapublicacao" => $value['pmcdatapublicacao'],
								"pmcdataconversaosiafi" => $value['pmcdataconversaosiafi'],
								"codigo" => $value['codigo'],
								"pmcnumconveniosiafi" => $value['pmcnumconveniosiafi'],
								"pendecias" => $value['pendencias']
								
							  );
							  
			array_push($arDadosArray, $registro);
		}
	}
	$cabecalho = array("Ação", "Nº Convênio FNDE<br> (Nº Original)" , "Nº PTA", "CNPJ", "Órgão ou Entidade", "Endereço", "Município", "UF", "CPF do Responsável", "Nome do Responsável", "Ítens / Especificação", "Justificativa", "Nível de Ensino", "Valor Proponente", "Valor Concedente", "Valor Total", "Dt Vigência", "Dt Assinatura", "Dt Publicação", "Data Conversão","Nome Arquivo", "Nº Convênio SIAFI", "Pendências");
	$db->monta_lista_array($arDadosArray, $cabecalho, 800, 4, 'N','Center');
}

function downloadArquivoConversaoConvenioSIAFI($chkSIAFI = array()){
	global $db;
 	ob_clean();
	define( "K_NR_CODIGO_UG_RESPONSAVEL", 153173 );
	if(count($chkSIAFI) < 1){
		echo '<script>
			alert("Escolha pelo menos um Plano de Trabalho!");
			window.location = "emenda.php?modulo=principal/contabilizacaoSIAFI&acao=A";
		  </script>';
		die;
	}else{
		
		$_SESSION['emenda']['chkSIAFI'] = $chkSIAFI;
		
		$arrHeader["Tipo de Registro"] = "1"; // Constante 1 (Passada pelo FNDE)
		$arrHeader["Identificação do Arquivo"] = "PRE-CONVENIO"; //Identificação do Arquivo (Passado pelo FNDE)
		$arrHeader["Data de Geração"] = date("dmY"); //Data da Geração da Fita (Data atual)
		$arrHeader["Hora de Geração"] = date("hm"); //Hora da Geração da Fita (Hora atual)
		$arrHeader["Código UG Responsável"] = K_NR_CODIGO_UG_RESPONSAVEL; //Código da UG Responsável pela criação do arquivo (Passado pelo FNDE)
		$arrHeader["Hora de Geração"] = date("hm"); //Hora da Geração da Fita (Hora atual)
		$arrHeader["Código UG Responsável"] = K_NR_CODIGO_UG_RESPONSAVEL; //Código da UG Responsável pela criação do arquivo (Passado pelo FNDE)
		$arrHeader["CPF"] = $_SESSION['usucpf']; //Código da UG Responsável pela criação do arquivo (Passado pelo FNDE)
		$arrHeader["Filler"] = preencheEspacoVazio(1266,"");
		$arrHeader["Quebra de linha"] = "\r\n"; // Ver se tem
		foreach($arrHeader as $header){
			$textoHeader .= $header;
		}
		
		$sql = "select
				plt.ptrid,
				plt.ptrcod,
				'00378257000181' as cnpjfnde,
				plt.ptrnumconvenio||'/'||plt.ptranoconvenio as numconvenio,
				--'40841693404' as cpfpresidentefnde,
				'38826658404' as cpfpresidentefnde,
				plt.ptrnumprocessoempenho as numprocesso,
				(select enbcnpj from emenda.entidadebeneficiada where enbid = plt.enbid) as enbbeneficiada,  
				(select entnumcpfcnpj from entidade.entidade where entid = plt.entiddirigente) as cpfdirigente,
				(
					select distinct
						(CASE
							WHEN vede.mapcod = '90' THEN '0'
							WHEN vede.mapcod = '30' THEN '1'
							WHEN vede.mapcod = '40' THEN '2'
							WHEN (vede.mapcod = '50' or vede.mapcod = '60') THEN '3'
							ELSE '' END) as esferaadm
					from 
						emenda.v_emendadetalheentidade vede
					inner join 
						emenda.ptemendadetalheentidade ped on vede.edeid = ped.edeid
					where 
						ped.ptrid = ptmin.ptrid
				) as esfera,
				-- execfin.exfnumempenhooriginal as numempenho,
				(
					select 
						itc.entidinterveniente
					from 
						emenda.intervenienteconvenio itc
					inner join 
						emenda.ptminutaconvenio pmc on itc.pmcid = pmc.pmcid
					where 
						itc.pmcid = ptmin.pmcid
					and 
						pmc.ptrid = ptmin.ptrid
					and 
						itcstatus = 'A'
					order by 
						itc.entidinterveniente
					limit 
						1
				) as cnpjinterveniente,
				(
					select 
						itc.entiddirigente
					from 
						emenda.intervenienteconvenio itc
					inner join
						emenda.ptminutaconvenio pmc on itc.pmcid = pmc.pmcid
					where 
						itc.pmcid = ptmin.pmcid
					and 
						pmc.ptrid = ptmin.ptrid
					and 
						itcstatus = 'A'
					order by 
						itc.entidinterveniente
					limit
						1
				) as cpfinterveniente,
				ptv.vigdatainicio as datainicio,
				ptv.vigdatafim as datafim,
				-- ptp.pubdatapublicacao as datafim,
				plt.ptrvalorproponente::numeric(17,2) as valorcontrapartida,
				(
					select 
						sum(exfvalor)
					from 
						emenda.execucaofinanceira
					where 
						ptrid = ptmin.ptrid
					and 
						exfstatus = 'A'
					and 
						exfidpai is null
				)::numeric(17,2) as valorconcedente,
				--	ptmin.pmcobjeto as objeto,
			--	obj.obcdsc as objeto,
				-- case when trim(ptmin.pmcresolucao)='' then plt.ptrjustificativa else 'resolução ' || ptmin.pmcresolucao end as resolucao,
				-- ptmin.pmcresolucao as resolucao,
				-- ini.iniresolucao as resolucao,
				case when trim(ptmin.pmcresolucao)='' then ini.iniresolucao else 'resolução ' || ptmin.pmcresolucao end as resolucao,
				ptv.vigdatainicio as celebracao,
				ptp.pubdatapublicacao as publicacao,
				Case when trim(coalesce(coc.cocnu_agencia,'')) = '' then plt.bcoid::character varying(5) else coc.cocnu_banco end as cocnu_banco,
				Case when trim(coalesce(coc.cocnu_agencia,'')) = '' then substr(plt.ptragenciabancaria,1,4) else coc.cocnu_agencia end as cocnu_agencia				
			from 
				emenda.planotrabalho as plt
			inner join 
				emenda.ptminutaconvenio ptmin ON ptmin.ptrid = plt.ptrid
				and ptmin.pmcstatus = 'A'
			inner join 
				emenda.ptvigencia ptv on ptv.ptrid = plt.ptrid 
				and ptv.vigstatus = 'A'
			inner join emenda.ptpublicacao ptp on ptp.pmcid = ptmin.pmcid 
				and ptp.pubstatus = 'A'
			left join 
				emenda.contacorrente coc ON coc.ptrid = plt.ptrid
			left join
				emenda.ptiniciativa pti ON pti.ptrid = plt.ptrid
			left join
				emenda.iniciativa ini ON ini.iniid = pti.iniid and ini.inistatus = 'A'
			
			where
				plt.ptrstatus = 'A' AND
				plt.ptrid in (".implode(",",array_keys($chkSIAFI)).")";
		
	}
		$arrDado = $db->carregar($sql);
		$arrDado = !$arrDado ? array() : $arrDado;
		$arrcontlinhas[2] = 0;
		$arrcontlinhas[3] = 0;
		$arrcontlinhas[4] = 0;

	foreach($arrDado as $arrDados){
		$sql2 = "select
					obj.obcdsc as objeto
				from
					emenda.objetoconvenio obj
				inner join emenda.objetominutaconvenio objm on obj.obcid = objm.obcid and obcstatus = 'A'
				inner join emenda.ptminutaconvenio ptmin ON objm.pmcid = ptmin.pmcid and ptmin.pmcstatus = 'A'
				where
					ptmin.ptrid = ".$arrDados['ptrid']."";
		
			$obj = $db->carregarColuna( $sql2 );
			if($obj) $objetoCC = implode(", ", $obj);

		/* Registro tipo 2 */
		$arrLinha2["Tipo de Registro"] = preencheEspacoVazio(1,"2");// Constante 2 (Passada pelo FNDE)
		$arrLinha2["Tipo de Cadastro"] = preencheEspacoVazio(1,"1");// Constante 1 (Passada pelo FNDE)
		$arrLinha2["UG do Concedente"] = preencheEspacoVazio(14,"153173");// UG
		$arrLinha2["Gestão do Concedente"] = preencheEspacoVazio(5,"15253");// Gestão do Concedente
		$arrLinha2["Número do Convênio"] = preencheEspacoVazio(20,$arrDados['numconvenio']);// Número do Convênio
		$arrLinha2["CPF Presidente FNDE"] = preencheEspacoVazio(11,$arrDados['cpfpresidentefnde']);// CPF do Presidente do FNDE
		$arrLinha2["Número do Processo"] = preencheEspacoVazio(20,$arrDados['numprocesso']);// Número do Processo
		$arrLinha2["CNPJ da Entidade Beneficiada"] = preencheEspacoVazio(14,$arrDados['enbbeneficiada']);// CNPJ da Entidade Beneficiada
		$arrLinha2["Gestão do Convenente"] = preencheEspacoVazio(5,"");// Gestão do Convenente (em branco)
		$arrLinha2["Responsável do Convenente"] = preencheEspacoVazio(11,$arrDados['cpfdirigente']);// CPF do Dirigente
		$arrLinha2["Esfera"] = preencheEspacoVazio(1,$arrDados['esfera']);// Esfera
		// inicio loop
		$sql = "select exfnumempenhooriginal as numempenho from emenda.execucaofinanceira where exfstatus='A' and exfidpai is null and ptrid=".$arrDados['ptrid'];
//		$sql = "select exfnumempenhooriginal as numempenho from emenda.execucaofinanceira where exfstatus='A' and exfidpai is null and semid = 4 and ptrid=".$arrDados['ptrid'];
		$arrEmpenhos = $db->carregar($sql);

		// Recupera os empenhos
		/*
		foreach ($arrEmpenhos as $key => $v) {
			$arrLinha2["UG do Documento $key"] = preencheEspacoVazio(6, !$v['numempenho']? "" : "153173");// UG do Documento (153173)
			$arrLinha2["Gestão do Documento $key"] = preencheEspacoVazio(5, !$v['numempenho'] ? "" : "15253");// Gestão do Documento (15253)
			$arrLinha2["Número do Documento $key"] = preencheEspacoVazio(12,$v['numempenho']);// Número do Empenho
		}
		*/
		for( $i = 0; $i <= 9 ; $i++){
			$arrLinha2["UG do Documento $i"] = preencheEspacoVazio(6, !$arrEmpenhos[$i]['numempenho']? "" : "153173");// UG do Documento (153173)
			$arrLinha2["Gestão do Documento $i"] = preencheEspacoVazio(5, !$arrEmpenhos[$i]['numempenho'] ? "" : "15253");// Gestão do Documento (15253)
			$arrLinha2["Número do Documento $i"] = preencheEspacoVazio(12,$arrEmpenhos[$i]['numempenho']);// Número do Empenho	
		}
		// fim loop
		$arrLinha2["CNPJ Interveniente"] = preencheEspacoVazio(14,$arrDados['cnpjinterveniente']);// CNPJ Interveniente
		$arrLinha2["CPF Interveniente"] = preencheEspacoVazio(11,$arrDados['cpfinterveniente']);// CPF Interveniente
		$arrLinha2["Início de Vigência"] = preencheEspacoVazio(8, ($arrDados['datainicio'] = !$arrDados['datainicio'] ? "" : date("dmY",strtotime($arrDados['datainicio']) )));// Data de Início de Vigência
		$arrLinha2["Final de Vigência"] = preencheEspacoVazio(8, ($arrDados['datafim'] = !$arrDados['datafim'] ? "" : date("dmY",strtotime($arrDados['datafim']) )));// Data de Final de Vigência
		$arrLinha2["Valor da Contra Partida"] = preencheEspacoVazio(17, ( str_replace(array(".",","), "", $arrDados['valorcontrapartida']) ), 'E');// Valor da Contra Partida
		$arrLinha2["Valor Concedente"] = preencheEspacoVazio(17, ( str_replace(array(".",","), "", $arrDados['valorconcedente']) ), 'E');// Valor Concedente
		$arrLinha2["Descrição da Transferencia"] = preencheEspacoVazio(490,substr($objetoCC,0,490));// Objeto do Convênio
		$arrLinha2["Justificativa"] = preencheEspacoVazio(350,substr(removeEnter($arrDados['resolucao']),0,350));// Resolução
		$arrLinha2["Código do Banco"] = preencheEspacoVazio(3,substr($arrDados['cocnu_banco'],0,3),'E', '0'); // (em branco)
		$arrLinha2["Código da Agência"] = preencheEspacoVazio(4,substr($arrDados['cocnu_agencia'],0,4)); // (em branco)
		$arrLinha2["CPF do Mandatário"] = preencheEspacoVazio(11,""); // (em branco)
		$arrLinha2["Data de Celebração"] = preencheEspacoVazio(8, ($arrDados['celebracao'] = !$arrDados['celebracao'] ? "" : date("dmY",strtotime($arrDados['celebracao']) )));// Data de Celebração
		$arrLinha2["Data de Publicação"] = preencheEspacoVazio(8, ($arrDados['publicacao'] = !$arrDados['publicacao'] ? "" : date("dmY",strtotime($arrDados['publicacao']) )));// Data de Publicação
		$arrLinha2["Tipo de Convênio"] = preencheEspacoVazio(1,"0");// Tipo de Convênio (constante 0)
		$arrLinha2["Executor"] = preencheEspacoVazio(14,"");// Executor (em branco)
		$arrLinha2["CPF Responsável do Executor"] = preencheEspacoVazio(11,"");// CPF Responsável do Executor(em branco)
		$arrLinha2["Quebra de Linha"] = "\r\n";// Quebra de Linha

		$exc2 = array( "Gestão do Convenente", "CPF do Mandatário", "Executor", "CPF Responsável do Executor", "Quebra de Linha", "CNPJ Interveniente", "CPF Interveniente" );

/*	
		if( $arrLinha2["CNPJ Interveniente"] == '' || $arrLinha2["CPF Interveniente"] == '' || $arrLinha2["Início de Vigência"] == '' || 
		$arrLinha2["Final de Vigência"] == '' || $arrLinha2["Valor da Contra Partida"] == '' || $arrLinha2["Valor Concedente"] == '' || 
		$arrLinha2["Descrição da Transferencia"] == '' || $arrLinha2["Justificativa"] == '' || $arrLinha2["Código do Banco"] == '' || 
		$arrLinha2["Código da Agência"] == '' || $arrLinha2["CPF do Mandatário"] == '' || $arrLinha2["Data de Celebração"] == '' || 
		$arrLinha2["Data de Publicação"] == '' || $arrLinha2["Tipo de Convênio"] == '' || $arrLinha2["Executor"] == '' || 
		$arrLinha2["CPF Responsável do Executor"] == '' ){
			echo '<script>
				alert("Faltam dados na linha 2 do !");
				window.location = "emenda.php?modulo=principal/contabilizacaoSIAFI&acao=A";
			  </script>';
			die;
		}
	*/	
		foreach($arrLinha2 as $linha2 => $v2){
			if( trim($v2) == '' && !in_array( $linha2, $exc2 ) ){
				$add = "";
				if( $linha2 == "Descrição da Transferencia" ){
					$add = " (objeto)";
				}
				if( substr( $linha2, 0, 19 ) == "Gestão do Documento" || substr( $linha2, 0, 15 ) == "UG do Documento" || substr( $linha2, 0, 19 ) == "Número do Documento" ){
					if( $linha2 == "Gestão do Documento 0" ){
						$erros.= "Não existe empenho cadastrado no SIGEF para o PTA: ".$arrDados['ptrcod']."!\\n";
					}
				} else {
					$erros.= "Falta o campo ".$linha2.$add." para o PTA: ".$arrDados['ptrcod']."!\\n";
				}
			}
			$textoLinha .= $v2;
		}
		
		if( $erros ){
			echo '<script>
						alert("'.$erros.'");
						window.location=\'emenda.php?modulo=principal/contabilizacaoSIAFI&acao=A\';
					  </script>';
			exit;
		}
		$arrcontlinhas[2]++;
		/* Fim Registro tipo 2 */
		
		/* Registro tipo 3 */
		$sqlItens = "SELECT coalesce(ed.enblog,'') || ' ' || coalesce(ed.enbnum,'') || '/' || coalesce(ed.enbbai,'') as endereco, ed.estuf as uf, m.muncodsiafi,
				e.espnome,
				  e.espunidademedida,
				  ptie.ptequantidade::numeric(9,1) as pteqtd
				FROM 
					emenda.ptiniciativa pti
					INNER JOIN emenda.ptiniciativaespecificacao ptie ON pti.ptiid = ptie.ptiid
					INNER JOIN emenda.iniciativaespecificacao ie ON (ptie.iceid = ie.iceid) 
					INNER JOIN emenda.especificacao e ON (ie.espid = e.espid)
					inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
					INNER JOIN emenda.planotrabalho ptr ON ptr.ptrid=pti.ptrid
					left join emenda.entidadebeneficiada ed on ed.enbid=ptr.enbid
					left join territorios.municipio m on m.muncod=ed.muncod
				WHERE 
				    ie.icestatus = 'A' 
				    AND e.espstatus = 'A'
				    AND ptie.ptestatus = 'A'
				    AND pti.ptrid = ".$arrDados['ptrid'];
		
		$arrDadoItem = $db->carregar($sqlItens);
		$arrDadoItem = !$arrDadoItem ? array() : $arrDadoItem;
		
		foreach($arrDadoItem as $arrDadoItens){

			$arrLinha3["Tipo de Registro"] = preencheEspacoVazio(1,"3");// Constante 2 (Passada pelo FNDE)
			$arrLinha3["Tipo de Cadastro"] = preencheEspacoVazio(1,"1");// Constante 1 (Passada pelo FNDE)
			$arrLinha3["UG do Concedente"] = preencheEspacoVazio(14,"153173");// UG
			$arrLinha3["Gestão do Concedente"] = preencheEspacoVazio(5,"15253");// Gestão do Concedente
			$arrLinha3["Número do Convênio"] = preencheEspacoVazio(20,$arrDados['numconvenio']);// Número do Convênio
			$arrLinha3["Descrição do Item"] = preencheEspacoVazio(140,$arrDadoItens['espnome']);
			$arrLinha3["Endereço do Item"] = preencheEspacoVazio(60,substr($arrDadoItens['endereco'],0,60));
			$arrLinha3["Unidade de Medida"] = preencheEspacoVazio(6,"UNID");
			$arrLinha3["Quantidade de Itens"] = preencheEspacoVazio(9,str_replace(array(".",","), "", $arrDadoItens['pteqtd']),'E');
			$arrLinha3["Código do Município Beneficiado"] = preencheEspacoVazio(4,$arrDadoItens['muncodsiafi']);
			$arrLinha3["UF Beneficiada"] = preencheEspacoVazio(2,$arrDadoItens['uf']);
			$arrLinha3["Filler"] = preencheEspacoVazio(1046,"");
			$arrLinha3["Quebra de linha"] = "\r\n";

			$exc3 = array( "Filler", "Quebra de linha" );
	/*		
			if( $arrLinha3["Tipo de Registro"] == '' || $arrLinha3["Tipo de Cadastro"] == '' || $arrLinha3["UG do Concedente"] == '' ||
			$arrLinha3["Gestão do Concedente"] == '' || $arrLinha3["Número do Convênio"] == '' || $arrLinha3["Descrição do Item"] == '' ||
			$arrLinha3["Endereço do Item"] == '' || $arrLinha3["Unidade de Medida"] == '' || $arrLinha3["Quantidade de Itens"] == '' ||
			$arrLinha3["Código do Município Beneficiado"] == '' || $arrLinha3["UF Beneficiada"] == '' || $arrLinha3["Filler"] == '' ){
				echo '<script>
					alert("Faltam dados na linha 3!");
					window.location = "emenda.php?modulo=principal/contabilizacaoSIAFI&acao=A";
				  </script>';
				die;
			}
		*/
			foreach($arrLinha3 as $linha3 => $v3){
				if( trim($v3) == '' && !in_array( $linha3, $exc3 ) ){
					$erros3.= "Falta o campo ".$linha3." para o PTA: ".$arrDados['ptrcod']."!\\n ";
				}
				$textoLinha .= $v3;
				
			}
			
			if( $erros3 ){
				echo '<script>
							alert("'.$erros3.'");
							window.location=\'emenda.php?modulo=principal/contabilizacaoSIAFI&acao=A\';
						  </script>';
				exit;
			}
			$arrcontlinhas[3]++;
		}
		
		/* Fim Registro tipo 3 */
		
		
		/* Registro tipo 4 */
		
		$sqlparcela = "select p.ptrid, prddata, case when extract(days from (prddata -  now()))::int < 10 then 10 else extract(days from (prddata -  now()))::int + 10 end as prazoparcela
					, pi.privalor as valorparcela, ptr.ptivalorconcedente as valortotal
		 			, (pi.privalor * 100 / ptr.ptivalorconcedente)::numeric(5,2) as percentual from 
			emenda.ptparceladesembolso p
			inner join emenda.ptparcelainiciativa pi on p.prdid=pi.prdid
			inner join emenda.v_planotrabalho ptr on p.ptrid=ptr.ptrid
			inner join emenda.ptminutaconvenio pm on p.ptrid=pm.ptrid and pm.pmcstatus='A'
			where p.prdminuta='M'
			and p.ptrid=".$arrDados['ptrid'];
		
		$arrDadoParcela = $db->carregar($sqlparcela);
		$arrDadoParcela = !$arrDadoParcela ? array() : $arrDadoParcela;
		$numParcela = 1;
		foreach($arrDadoParcela as $arrDadoParcelas){
			
			$arrLinha4["Tipo de Registro"] = preencheEspacoVazio(1,"4");// Constante 2 (Passada pelo FNDE)
			$arrLinha4["Tipo de Cadastro"] = preencheEspacoVazio(1,"1");// Constante 1 (Passada pelo FNDE)
			$arrLinha4["UG do Concedente"] = preencheEspacoVazio(14,"153173");// UG
			$arrLinha4["Gestão do Concedente"] = preencheEspacoVazio(5,"15253");// Gestão do Concedente
			$arrLinha4["Número do Convênio"] = preencheEspacoVazio(20,$arrDados['numconvenio']);// Número do Convênio
			$arrLinha4["Número da Parcela"] = preencheEspacoVazio(3,$numParcela, 'E', '0');
			$arrLinha4["Prazo da Parcela"] = preencheEspacoVazio(3,$arrDadoParcelas['prazoparcela'], 'E', '0');
			$arrLinha4["Valor da Parcela"] = preencheEspacoVazio(17, ( str_replace(array(".",","), "", $arrDadoParcelas['valorparcela']) ), 'E');
			$arrLinha4["Percentual da Parcela"] = preencheEspacoVazio(5, ( str_replace(array(".",","), "", $arrDadoParcelas['percentual']) ), 'E');
			$arrLinha4["Filler"] = preencheEspacoVazio(1239,"");
			$arrLinha4["Quebra de linha"] = "\r\n";

			$exc4 = array( "Filler", "Quebra de linha" );
		/*	
			if( $arrLinha4["Tipo de Registro"] == '' || $arrLinha4["Tipo de Cadastro"] == '' || $arrLinha4["UG do Concedente"] == '' ||
			$arrLinha4["Gestão do Concedente"] == '' || $arrLinha4["Número do Convênio"] == '' || $arrLinha4["Número da Parcela"] == '' ||
			$arrLinha4["Prazo da Parcela"] == '' || $arrLinha4["Valor da Parcela"] == '' || $arrLinha4["Percentual da Parcela"] == '' ||
			$arrLinha4["Filler"] == '' ){
				echo '<script>
					alert("Faltam dados na linha 4!");
					window.location = "emenda.php?modulo=principal/contabilizacaoSIAFI&acao=A";
				  </script>';
				die;
			}
			*/
			foreach($arrLinha4 as $linha4 => $v4){
				if( trim($v4) == '' && !in_array( $linha4, $exc4 ) ){
					$erros4.= "Falta o campo ".$linha4." para o PTA: ".$arrDados['ptrcod']."!\\n ";
				}
				$textoLinha .= $v4;
			}
			if( $erros4 ){
				echo '<script>
							alert("'.$erros4.'");
							window.location=\'emenda.php?modulo=principal/contabilizacaoSIAFI&acao=A\';
						  </script>';
				exit;
			}
			$numParcela ++;
			$arrcontlinhas[4]++;
		}
		
		/* Fim Registro tipo 4 */

	}
	$totalregistros = $arrcontlinhas[2] + $arrcontlinhas[3] + $arrcontlinhas[4];
	$arrTrailler["Tipo de Registros"] = "9";
	$arrTrailler["Quantidade de Registros"] = preencheEspacoVazio(7, $totalregistros, 'E' );
	$arrTrailler["Quantidade de Registros 2"] = preencheEspacoVazio(7, $arrcontlinhas[2], 'E');
	$arrTrailler["Quantidade de Registros 3"] = preencheEspacoVazio(7, $arrcontlinhas[3], 'E');
	$arrTrailler["Quantidade de Registros 4"] = preencheEspacoVazio(7, $arrcontlinhas[4], 'E');
	$arrTrailler["Filler"] = preencheEspacoVazio(1279,"");
	$arrTrailler["Quebra de linha"] = "\r\n";
	
	foreach($arrTrailler as $Trailler){
		$textoTrailler .= $Trailler;
	}
	
	$abtconteudo = $textoHeader.$textoLinha.$textoTrailler;
	
	$sql = "update emenda.ptminutaconvenio set pmcdataconversaosiafi = now() where pmcstatus = 'A' and ptrid in (".implode(",",array_keys($chkSIAFI)).")";
	$db->executar($sql);

	
	$sql = "select coalesce(MAX(abtcodigo),0) from emenda.arquivobtsiafi where to_char(date(abtdatageracao), 'YYYY/MM/DD') = to_char(date(now()), 'YYYY/MM/DD')";
	$abtcodigo = $db->pegaUm($sql);
	$abtcodigo = $abtcodigo + 1;
	$sql = "insert into emenda.arquivobtsiafi (abtdatageracao, abtcodigo,abtconteudo) values (now(), $abtcodigo,'$abtconteudo') returning abtid";
	$abtid = $db->pegaUm($sql);
	
	foreach($chkSIAFI as $ptrid => $key){
		$sqlIn .= "insert into emenda.arquivobtsiafiplanotrabalho (abtid,ptrid) values ($abtid,$ptrid);";
	}
	
	$db->executar($sqlIn);
	
	$db->commit();
//	$db->rollback();

	/*
	header("Content-Type: text/html; charset=ISO-8859-1");
	header("Content-type: application/octet-stream");
	$nomeArquivo = "CV".K_NR_CODIGO_UG_RESPONSAVEL.".".date("y").$abtcodigo;
	header("Content-Disposition: attachment; filename=\"$nomeArquivo\"");
	echo $abtconteudo;
	*/
	unset($_SESSION['emendas']['boGerar']);
	$_SESSION['emendas']['abtconteudo'] = trim($abtconteudo);
	echo "<script>window.open('emenda.php?modulo=principal/contabilizacaoSIAFI&acao=A&popup=true&abtcodigo=".date('y').$abtcodigo."','page','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=400,height=500');</script>";
	echo "<script>window.opener.pesquisaPTAAnalise();</script>";
		
//	exit;
		
} 

function preencheEspacoVazio($tam,$valor,$lado = 'D', $caracter = ' '){
	$valor = strtoupper(removeAcentos($valor));
	
	$valor = str_replace(array(chr(13),chr(10)), ' ',$valor);
	
	if(strlen($valor) > $tam){
		return "***** >>> Quantidade de caracteres (".strlen($valor)." - $valor) exede o tamanho do campo ($tam). <<< ***** ";
	}elseif(strlen($valor) < $tam){
		$numEspacos = $tam - (strlen($valor));
		for($i = 1; $i <= $numEspacos;$i++){
			$espacoVazio .= $caracter;
		} 
		if ($lado == 'D')
			return $valor.$espacoVazio;
		elseif ($lado == 'E')
			return $espacoVazio.$valor;
	}else{
		return $valor;
	}
}

function downloadArquivoSIAFI($ptrid){
	global $db;
	define( "K_NR_CODIGO_UG_RESPONSAVEL", 153173 );
	
	$sql = "select distinct aptr.abtid 
			from emenda.arquivobtsiafi arq
			inner join emenda.arquivobtsiafiplanotrabalho aptr ON aptr.abtid = arq.abtid
			where ptrid = $ptrid";
	$abtid = $db->pegaUm($sql);
	
	if(!$abtid){
		echo "<script>alert('Arquivo indisponível!');window.location.href='emenda.php?modulo=principal/contabilizacaoSIAFI&acao=A'</script>";
	}else{
		$sql = "select abtconteudo, abtcodigo from emenda.arquivobtsiafi where abtid = $abtid";
		$arrDado = $db->pegaLinha($sql);
		
		if(!$arrDado){
			echo "<script>alert('Erro na recuperação do arquivo!');window.location.href='emenda.php?modulo=principal/contabilizacaoSIAFI&acao=A'</script>";
		}else{
			$abtconteudo = $arrDado['abtconteudo'];
			$abtcodigo = $arrDado['abtcodigo'];
			
			header("Content-Type: text/html; charset=ISO-8859-1");
			header("Content-type: application/octet-stream");
			$nomeArquivo = "CV".K_NR_CODIGO_UG_RESPONSAVEL.".".date("y").$abtcodigo;
			header("Content-Disposition: attachment; filename=\"$nomeArquivo\"");
			echo $abtconteudo;
			exit;
		}
	}	
}

function removeEnter($valor)
{
	$valor = str_replace(chr(10),' ',$valor);
	$valor = str_replace(chr(13),'',$valor);
	
	return $valor;
}

function monta_lista_itens($sql,$cabecalho="") {
	global $db;
	$largura = '100%';
	$tab='';

    if (is_array($sql))
        $RS = $sql;
    else
        $RS = $db->carregar($sql);

	$nlinhas = $RS ? count($RS) : 0;
	if (! $RS) $nl = 0; else $nl=$nlinhas;
	$reg_fim = $nlinhas;
	$numero=1;
	$tab .= '<table width="'. $largura . '" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';
	if ($nlinhas>0)
	{
		//Monta Cabeçalho
		if(is_array($cabecalho))
		{
			$tab.= '<thead><tr>';
			for ($i=0;$i<count($cabecalho);$i++)
			{
				$tab.= '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">'.$cabecalho[$i].(($i===$totalregistro)?' (Total:'.count($RS).')':'').'</label>';
			}
			$tab.=  '</tr> </thead>';
		}
        $tab.= '<tbody>';
		//Monta Listagem
		for ($i=($numero-1);$i<$reg_fim;$i++)
		{
			$c = 0;
			if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
			$tab.=  '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ccFF88\';" onmouseout="this.bgColor=\''.$marcado.'\';">';
			
			foreach($RS[$i] as $k=>$v) {
				// Clausula que faz soma numeros monetários
				if (!is_numeric($v)) {
					if (is_numeric(str_replace(array(".", ","),array("", "."),$v))) {
						$v = str_replace(array(".", ","),array("", "."),$v);
						$valormonetario="S";
					}
				}
				// fim - Clausula que faz soma numeros monetários
				$tab.=  '<td valign="top" title="'.$cabecalho[$c].'">'.strtoupper(removeAcentos($v));
				$tab.=  '</td>';
				$c = $c + 1;
			}
			$tab.=  '</tr>';
		}
		
        $tab.=  '</tbody>';

	}
	else {
		$tab.=  '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
	}
	$tab.=  '</table>';
	return $tab;
}
?>