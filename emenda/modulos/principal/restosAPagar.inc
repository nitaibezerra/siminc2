<?php

switch ( $_REQUEST["requisicao"] ){
	
	case 'filtrapesquisa':
		$filtros = filtroPtaAnalise( $_REQUEST );
	break;
	case 'download':
		downloadArquivoRAP( $_REQUEST['ptridD'] );
	break;
	
}

if(count($_POST['chkSIAFI'])){
	downloadArquivoRestosAPagar($_POST['chkSIAFI']);
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );

// Cria o título da tela
monta_titulo( 'Restos a pagar', $subtitulo_modulo );
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
	function exibeArquivoCSV(ptrid){
		window.location.href='emenda.php?modulo=principal/restosAPagar&acao=A&requisicao=download&ptridD=' + ptrid;
		
	}
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
		<tr>
			<td class="subtitulodireita">Situação:</td>
			<td>
				<?php
					$situacaorp = $_REQUEST['situacaorp'];
					
					$arrSituacao = array ( array("codigo" => "1", "descricao" => "Gerado"), array("codigo" => "2", "descricao" => "Não Gerado")  );
					
					$db->monta_combo( "situacaorp", $arrSituacao , 'S', 'Selecione...', '', '', '', '150' );
				?>
			</td>
		<tr>
		<tr>
			<td class="subtitulodireita">Nome do Arquivo:</td>
			<td>
				<?php
					$nomeArqRAP = $_REQUEST['nomearqrap'];
					$sql = "select 
								rapnomearquivo as descricao
								, rapid as codigo
							from emenda.arquivorap
							order by rapdatageracao";
					$db->monta_combo( "nomearqrap", $sql, 'S', 'Selecione...', '', '', '', '250' );
				?>
			</td>
		<tr>
		
		<tr bgcolor="#D0D0D0">
			<td><input type="checkbox" name="chkMarcarTodos" id="chkMarcarTodos" checked="checked" onclick="marcarTodos();" style="cursor: pointer;"> Marcar Todos / Desmarcar Todos</td>
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
				<input type="button" value="Gerar arquivo conversão" name="btnConversao" id="btnConversao" onclick="gerarArquivoSIAFI();" style="cursor: pointer;">
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

	//'<center><input type=\"checkbox\" name=\"chkSIAFI[' || ptr.ptrid || ']\" checked=\"checked\" /></center>' as acoes,
		
	$sql = "SELECT DISTINCT
				CASE WHEN pmc.pmcdatarap is not null 
					THEN '<center><img title=\"Visualizar arquivo de restos a pagar\" onclick=\"exibeArquivoCSV(' || ptr.ptrid || ');\" style=\"cursor:pointer\" src=\"../imagens/anexo.gif\" /></center>'
					ELSE '<center><input type=\"checkbox\" name=\"chkSIAFI[' || ptr.ptrid || ']\" checked=\"checked\" /></center>' END as acoes,
				
				ptr.ptrid,
				ent.enbcnpj,
				ent.enbnome,
				ent.estuf,
				mun.mundescricao,
				res.resassunto,
				coalesce( (select sum(exfvalor) from emenda.execucaofinanceira where ptrid = ptr.ptrid and exfstatus = 'A' and exfnumempenhooriginal is not null),0) as valorTotal,
				ptr.ptrnumprocessoempenho,
				exf.exfnumempenhooriginal, 
				case when pmc.pmcdtatualizaconveniosiafi is null then
                    pmc.pmcnumconveniosiafi
                else 
					pmc.pmcnumconveniosiafi || '/' || date_part('year',pmc.pmcdatainclusao) end as numconveniosiafi,
				ptr.ptrnumconvenio || '/' || ptr.ptranoconvenio as numconvenio,
				coalesce(to_char(date(pmc.pmcdatarap), 'DD/MM/YYYY'),'N/A') as pmcdatarap,
				rap.rapnomearquivo

			FROM  emenda.planotrabalho ptr 
            INNER JOIN emenda.ptminutaconvenio pmc ON pmc.ptrid = ptr.ptrid and pmc.pmcstatus = 'A' 
            INNER JOIN emenda.entidadebeneficiada ent ON ptr.enbid = ent.enbid  
            INNER JOIN territorios.municipio mun ON mun.muncod = ent.muncod 
            INNER JOIN emenda.responsavel res ON ptr.resid = res.resid
            INNER JOIN emenda.execucaofinanceira exf ON exf.ptrid = ptr.ptrid and exf.exfstatus = 'A'
            left join emenda.ordembancaria orb on orb.exfid = exf.exfid and orb.orbid is null
			LEFT JOIN
				emenda.arquivorapplanotrabalho rpt 
			 	INNER JOIN emenda.arquivorap rap ON rpt.rapid = rap.rapid
			ON rpt.ptrid = ptr.ptrid 
			WHERE
			    ptr.ptrstatus = 'A'
			    AND ptr.ptrexercicio = ".$_SESSION['exercicio']."
			    AND pmc.pmcnumconveniosiafi <> ''
			    {$filtros}
		    	{$filtroResid}
			ORDER BY
				ptr.ptrid,
				ent.estuf,
				ent.enbnome";

	$arDados = $db->carregar($sql);	
	
	if( $arDados ){
		$registro = array();
		$arDadosArray = array();

		foreach ($arDados as $key => $value) {
			
			$registro = array( "acao" => $value['acoes'],
							   "ptrid" => $value['ptrid']."/".$_SESSION['exercicio'],
							   "enbcnpj" => $value['enbcnpj'],
							   "enbnome" => $value['enbnome'],			
							   "estuf" => $value['estuf'],	
							   "mundescricao" => $value['mundescricao'],	
							   "resassunto" => $value['resassunto'],	
							   "valortotal" => $value['valortotal'],
							   "ptrnumprocessoempenho" => $value['ptrnumprocessoempenho'],
							   "exfnumempenhooriginal" => $value['exfnumempenhooriginal'],
							   "numconveniosiafi" => $value['numconveniosiafi'],
							   "numconvenio" => $value['numconvenio'],
							   "pmcdatarap" => $value['pmcdatarap'],
							   "rapnomearquivo" => $value['rapnomearquivo']
							  );
							  
			array_push($arDadosArray, $registro);
		}
	}
	$cabecalho = array("Ação", "Nº PTA", "CNPJ", "Órgão ou Entidade", "UF", "Município", "Nível de Ensino", "Valor Total"
	,"Nº Processo", "Nº Empenho", "Nº Convenio SIAFI", "Nº Convenio FNDE", "Data RAP", "Nome Arquivo");
	
	$db->monta_lista_array($arDadosArray, $cabecalho, 1000, 4, 'N','Center');
}

function downloadArquivoRestosAPagar( $chkSIAFI = array() ){
	
	global $db;
	
	if(count($chkSIAFI) < 1){
		echo '<script>
			alert("Escolha pelo menos um Plano de Trabalho!");
			window.location = "emenda.php?modulo=principal/contabilizacaoSIAFI&acao=A";
		  </script>';
		die;
	}else{
		
		
	$sql = "select 	distinct ptr.ptrid, 
					exf.exfnumempenhooriginal, 
					ptr.ptrnumprocessoempenho, 
					ent.enbcnpj, 
					pmc.pmcnumconveniosiafi || '/' || date_part('year',pmc.pmcdtatualizaconveniosiafi) as numconveniosiafi,
					ptr.ptrnumconvenio || '/' || ptr.ptranoconvenio as numconvenio,
					exf.exfnaturezadespesa, 
					substr( ('000' || (select count(1) from emenda.ptparceladesembolso where ptrid = ptr.ptrid and prdminuta = 'M')), char_length('000' || (select count(1) from emenda.ptparceladesembolso where ptrid = ptr.ptrid and prdminuta = 'M')) -2 , char_length('000' || (select count(1) from emenda.ptparceladesembolso where ptrid = ptr.ptrid and prdminuta = 'M')) ) as numparcelas,
					(select sum(exfvalor) from emenda.execucaofinanceira where ptrid = ptr.ptrid and exfstatus = 'A' and exfnumempenhooriginal is not null) as valor
			from 
			emenda.planotrabalho ptr
			inner join emenda.execucaofinanceira exf on ptr.ptrid = exf.ptrid and exf.exfstatus = 'A'
			inner join emenda.ptminutaconvenio pmc on pmc.ptrid = exf.ptrid and pmc.pmcstatus = 'A'
			inner join emenda.entidadebeneficiada ent on ent.enbid = ptr.enbid 
			where ptr.ptrid in (".implode(",",array_keys($chkSIAFI)).")
			ORDER BY  ptr.ptrid";
		
	}
	$arrDado = $db->carregar($sql);
	$arrDado = !$arrDado ? array() : $arrDado;

	
	foreach($arrDado as $arrDados){

		$arrLinha["stEmpenho"] 		 	= $arrDados["exfnumempenhooriginal"].';';
		$arrLinha["stProcesso"] 	 	= $arrDados["ptrnumprocessoempenho"].';';
		$arrLinha["stCNPJ"] 		 	= $arrDados["enbcnpj"].';';
		$arrLinha["stConvenioSIAFI"] 	= $arrDados["numconveniosiafi"].';';
		$arrLinha["stConvenioFNDE"] 	= $arrDados["numconvenio"].';';
		$arrLinha["stNaturezaDespesas"]	= $arrDados["exfnaturezadespesa"].';';
		$arrLinha["stParcelas"] 	 	= $arrDados["numparcelas"].';';
		$arrLinha["nrValor"] 		 	= ereg_replace("[^0-9]", "", $arrDados["valor"] ).';';
		$arrLinha["Quebra de Linha"] 	= "\r\n";// Quebra de Linha
		
		foreach($arrLinha as $linha){
			$textoLinha .= $linha;
		}
	}
	
	$nomeArquivo = "NL_".date("dmY_Hi")."_".count($arrDado).".csv";
	$sql = "update emenda.ptminutaconvenio set pmcdatarap = now() where pmcstatus = 'A' and ptrid in (".implode(",",array_keys($chkSIAFI)).")";
	$db->executar($sql);
	
	$sql = "insert into emenda.arquivorap (rapdatageracao, rapnomearquivo, rapconteudo) values (now(), '$nomeArquivo','$textoLinha') returning rapid";
	$rapid = $db->pegaUm($sql);
	
	foreach($chkSIAFI as $ptrid => $key){
		$sqlIn .= "insert into emenda.arquivorapplanotrabalho (rapid,ptrid) values ($rapid,$ptrid);";
	}
	$db->executar($sqlIn);
	
	$db->commit();
	
	header("Content-Type: text/html; charset=ISO-8859-1");
	header("Content-type: application/octet-stream");
	header("Content-Disposition: attachment; filename=\"$nomeArquivo\"");
	echo $textoLinha;
	exit;	
} 

function preencheEspacoVazio($tam,$valor){
	if(strlen($valor) > $tam){
		return "***** >>> Quantidade de caracteres (".strlen($valor)." - $valor) exede o tamanho do campo ($tam). <<< ***** ";
	}elseif(strlen($valor) < $tam){
		$numEspacos = $tam - (strlen($valor));
		for($i = 1; $i <= $numEspacos;$i++){
			$espacoVazio .= " ";
		} 
		return $valor.$espacoVazio;
	}else{
		return $valor;
	}
}


function downloadArquivoRAP($ptrid){
	global $db;
	
	$sql = "select distinct rapnomearquivo 
			from emenda.arquivorap arq
			inner join emenda.arquivorapplanotrabalho aptr ON aptr.rapid = arq.rapid
			where ptrid = $ptrid";
	$abtcodigo = $db->pegaUm($sql);
	
	if(!$abtcodigo){
		echo "<script>alert('Arquivo indisponível!');window.location.href='emenda.php?modulo=principal/restosAPagar&acao=A'</script>";
	}else{
		$sql = "select rapconteudo from emenda.arquivorap where rapnomearquivo = '$abtcodigo'";
		$abtconteudo = $db->pegaUm($sql);
		
		header("Content-Type: text/html; charset=ISO-8859-1");
		header("Content-type: application/octet-stream");
		$nomeArquivo = $abtcodigo;
		header("Content-Disposition: attachment; filename=\"$nomeArquivo\"");
		echo $abtconteudo;
		exit;
	}	
}
?>