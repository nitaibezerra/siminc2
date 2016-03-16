<?php
require_once APPRAIZ . "emenda/classes/PlanoTrabalho.class.inc";
require_once APPRAIZ . "emenda/classes/ExecucaoFinanceira.class.inc";
require_once APPRAIZ . "emenda/classes/ContaCorrente.class.inc";

# Ajax
if($_POST['execFinan']){
	$sql = "SELECT 
				--efh.efhid,
				--efh.usucpf,       
				u.usunome, 
				--efh.exfid,
				se.semcod || ' - ' ||se.semdesc as situacao,
				efh.exfvalor,
				CASE WHEN efh.exfespecieempenho = '01' THEN 'Original'
				 WHEN efh.exfespecieempenho = '02' THEN 'Reforço'
				 WHEN efh.exfespecieempenho = '03' THEN 'Anulação'
				END AS especieempenho,
				to_char(efh.efhiddataalteracao, 'DD/MM/YYYY HH24:mi:ss') as efhiddataalteracao,
				CASE WHEN efh.exfverifsiafi = true THEN 'Sim'
				 ELSE 'Não'
				END AS siafi,
				CASE WHEN efh.exfverifcadin = true THEN 'Sim'
				 ELSE 'Não'
				END AS cadin
			 FROM emenda.execfinanceirahistorico efh
				inner join emenda.execucaofinanceira ef on efh.exfid = ef.exfid and ef.exfstatus = 'A'
				inner join emenda.situacaoempenho se on efh.semid = se.semid 
				inner join seguranca.usuario u on efh.usucpf = u.usucpf
			 WHERE efh.exfid = {$_POST['exfid']}";
	
	
	$sqlSiconv = "SELECT distinct dsusernamemovimento, anconvenio, nuconvenio, nuconveniosiconv, nuempenhooriginal, nuempenhosiafi, nupropostasiconv, 
  						nuseqdocsiafi, vlempenho, cocentrogestaoaprov, coespecieempenho, cofonterecursoaprov, cogestaoemitente, conaturezadespesaaprov, 
						coplanointernoaprov, coptresaprov, cosituacaodocsiafi, cotipodocumento, counidadegestoraemitente, 
  						to_char(dtmovimento, 'DD/MM/YYYY HH24:mi:ss') as dtmovimento
  					FROM
  						emenda.sicempenhohistorico
  					WHERE
  						exfid = {$_POST['exfid']}";

  	if( $_SESSION['exercicio'] >=	2012 ){
		$cabecalho = array( "Usuário", "Ano Convenio", 'Nº Convênio', 'Nº Convênio SICONV', 'Nº Empenho Original', 'Nº Empenho SIAF', 'Nº Proposta SICONV', 'Seq Doc SIAFI', 'Valor Empenho', "Gestão", "Espécie", "Fonte Recurso", "Gestão Emitente", "Natureza de Despesa", 'PI', 'PTRES', 'Situação doc SIAF', 'Tipo Documento', 'Unidade Gestora', 'Data Movimento' );
		$db->monta_lista_simples( $sqlSiconv, $cabecalho, 100, 30, 'N', '100%');
  	} else {
		$cabecalho = array( "Usuário", "Situação", "Valor (R$)", "Espécie", "Data Alteração", "Verif. SIAFI", "Verif. CADIN" );
		$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%');
  	}
	die;
}

if($_POST['contaCorrente']){
	$sql = "SELECT 
				u.usunome,
				cch.cocandamento,
				to_char(cch.ccdatainclusao, 'DD/MM/YYYY HH24:mi:ss') as ccdatainclusao,
				cch.cocsituacao_conta,
				cch.cocdsproblema
			 FROM emenda.contacorrentehistorico cch
				inner join emenda.contacorrente cc on cch.cocid = cc.cocid
				inner join seguranca.usuario u on cch.usucpf = u.usucpf
			 WHERE cc.cocid = {$_POST['cocid']} ORDER BY cch.ccdatainclusao desc";

	$cabecalho = array( "Usuário", "Andamento", "Data Inclusão", "Situação", "Descrição do Problema");
	$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%');
	die;
}
# Ajax -----

$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];
$ptrid = $_SESSION["emenda"]["ptridAnalise"];

$obPTA = new PTA();

$obPTA->validaSessionPTA( $ptrid );
$federal = $obPTA->buscaEmendaDescentraliza( $ptrid );

if( $federal == 'S' ){
	$label = 'Nota de Crédito';
} else {
	$label = 'Nº Empenho Original';
}

//include_once APPRAIZ . 'includes/workflow.php';
//include  APPRAIZ."includes/cabecalho.inc";
//print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );

// Cria o título da tela
monta_titulo( 'Tela Exibição Exec. Financeira', "");

echo cabecalhoPlanoTrabalho($ptrid, true);

# Atualiza a tabela de PlanoTrabalho.
$obPlanoTrabalho = new PlanoTrabalho($ptrid);
?>
<html>
<head>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="JavaScript" src="../includes/prototype.js"></script>
</head>
<body>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
 	<tr>
 		<td colspan="4" class="subtitulocentro">Dados do Convênio</td>
 	</tr>
	<tr>
		<td style="text-align: right; width:7.5%;" class="SubTituloEsquerda">Número do Convênio:</td>
		<td style="width:30%; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?php echo $obPlanoTrabalho->ptrnumconvenio."/".$obPlanoTrabalho->ptranoconvenio; ?></td>
	</tr>
	<tr>
 		<td colspan="6" class="subtitulocentro">Dados da execução Financeira</td>
 	</tr>
 	<tr>
 		<td colspan="2">
 			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="150" name="tabela_execfinanceira" id="tabela_execfinanceira">
				<tr>
					<td class="subtitulocentro">Ação</td>
					<td class="subtitulocentro">Município SIAFI</td>
					<td class="subtitulocentro">Nº Sol Empenho</td>
					<td class="subtitulocentro">Espécie Empenho</td>
					<td class="subtitulocentro">Fonte SIAFI</td>
					<td class="subtitulocentro">Situação</td>
					<td class="subtitulocentro">Data Alteração</td>
					<td class="subtitulocentro">Status</td>
					<td class="subtitulocentro">Usuário</td>
					<td class="subtitulocentro">Ano Original</td>
					<td class="subtitulocentro"><?=$label; ?></td>
					<td class="subtitulocentro">Natureza Despesa</td>
					<td class="subtitulocentro">Verif. SIAFI</td>
					<td class="subtitulocentro">Verif. CADIN</td>
					<td class="subtitulocentro">Data Inclusão</td>
					<td class="subtitulocentro">Valor Total (R$)</td>
				</tr>
				<?php
				
				$sql = "SELECT distinct
						    ef.exfid,
						    ef.tpeid,
						    ef.pliid,
						    ef.ptrid,
						    ef.pedid,
						    ef.exfvalor,
						    ef.exfcodmunicipiosiafi  || ' - ' || mun.mundescricao as municipiosiafi,
						    ef.exfnumsolempenho,
						    u.usunome,
						    CASE WHEN ef.exfespecieempenho = '01' THEN 'Original'
							 WHEN ef.exfespecieempenho = '02' THEN 'Reforço'
							 WHEN ef.exfespecieempenho = '03' THEN 'Anulação'
						    END AS especieempenho,
						    CASE WHEN ef.exfstatus = 'A' THEN 'Ativo'
							 ELSE 'Inativo'
						    END AS status,
						    ef.exfcodfontesiafi,
						    se.semcod || ' - ' ||se.semdesc as situacao,
						    to_char(ef.exfdataalteracao, 'DD/MM/YYYY HH24:mi:ss') as exfdataalteracao,
						    ef.usucpf,
						    ef.exfanooriginal, 
						    ef.exfnumempenhooriginal,
						    ef.exfidpai,
						    ef.exfnaturezadespesa,
						    CASE WHEN ef.exfverifsiafi = true THEN 'Sim'
							 ELSE 'Não'
						    END AS siafi,
						    CASE WHEN ef.exfverifcadin = true THEN 'Sim'
							 ELSE 'Não'
						    END AS cadin,
						    to_char(ef.exfdatainclusao, 'DD/MM/YYYY HH24:mi:ss') as exfdatainclusao
						FROM emenda.execucaofinanceira ef
						   inner join monitora.pi_planointerno pli on pli.plicod = ef.plicod
						   inner join monitora.pi_planointernoptres plpt on pli.pliid = plpt.pliid
						   inner join monitora.ptres ptr on ptr.ptrid = plpt.ptrid and ptr.ptres = ef.ptres
						   inner join emenda.planotrabalho pt on ef.ptrid = pt.ptrid
						   inner join emenda.tipoensino te on ef.tpeid = te.tpeid
						   inner join emenda.ptemendadetalheentidade ped on ef.pedid = ped.pedid
						   inner join emenda.situacaoempenho se on ef.semid = se.semid 
						   inner join emenda.v_emendadetalheentidade vede on vede.edeid = ped.edeid
						   inner join emenda.entidadebeneficiada eb on eb.enbid = vede.entid 
						   -- inner join entidade.endereco ende on vede.entid = ende.entid
						   inner join territorios.municipio mun on eb.muncod = mun.muncod
						   inner join seguranca.usuario u on ef.usucpf = u.usucpf
						WHERE ef.ptrid = {$_REQUEST['ptrid']} 
						  --  AND ef.exfidpai is null
						  and ef.exfid = {$_REQUEST['exfid']}
						  and ef.exfstatus = 'A'
						order by exfdatainclusao";
				$arDados = $db->carregar($sql);
				$arDados = ($arDados) ? $arDados : array();
				//unset($obExecucaoFinanceira);
				
				$i = 0;
				foreach( $arDados as $dados){
						$cor = ($i % 2) ? "#f4f4f4" : "#e0e0e0";
						echo  "<tr bgColor='{$cor}'>";
							//if($obExecucaoFinanceira->temFilho($dados["exfid"])){
						echo  "	   <td align='center' id='exec_acao{$dados["exfid"]}'>"
							. "        <img src='/imagens/mais.gif' style='cursor:pointer;' onclick='abreHistoricoExec({$dados["exfid"]});'/>"
							. "	   </td>";
							//} else {
					//	echo  "	   <td align='center'> - </td>";
							//}
						echo  "	   <td >"
							.          $dados["municipiosiafi"]
							. "	   </td>"
							. "	   <td >"
							.          $dados["exfnumsolempenho"]
							. "	   </td>"
							. "	   <td >"
							.          $dados["especieempenho"]
							. "	   </td>"
							. "	   <td >"
							.          $dados["exfcodfontesiafi"]
							. "	   </td>"
							. "	   <td >"
							.          $dados["situacao"]
							. "	   </td>"
							. "	   <td >"
							.          $dados["exfdataalteracao"]
							. "	   </td>"
							. "	   <td >"
							.          $dados["status"]
							. "	   </td>"
							. "	   <td >"
							.          $dados["usunome"]
							. "	   </td>"
							. "	   <td align='center'>"
							.          $dados["exfanooriginal"]
							. "	   </td>"
							. "	   <td align='center'>"
							.          $dados["exfnumempenhooriginal"]
							. "	   </td>"
							. "	   <td >"
							.          $dados["exfnaturezadespesa"]
							. "	   </td>"
							. "	   <td >"
							.          $dados["siafi"]
							. "	   </td>"
							. "	   <td >"
							.          $dados["cadin"]
							. "	   </td>"
							. "	   <td >"
							.          $dados["exfdatainclusao"]
							. "	   </td>"
							. "	   <td align='right'>"
							.          number_format( $dados["exfvalor"], 2, ",", "." )
							. "	   </td>"
							. "</tr>"
							. "<tr id='exec_{$dados["exfid"]}' style='display:none;'>"
							. "    <td align='center' valign='center'><img src='/imagens/seta_filho.gif'/></td>"
							. "    <td colspan='15' id='exec_historico_{$dados["exfid"]}'></td>"
							. "</tr>";
						$i++;
					}
				
				?>
			</table> 		
 		</td>
 	</tr>
	<tr>
 		<td colspan="6" class="subtitulocentro">Dados da Conta Corrente</td>
 	</tr>
 	<tr>
 		<td colspan="2">
 			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="150">
				<tr>
					<td class="subtitulocentro">Ação</td>
					<td class="subtitulocentro">Conta Corrente Fovorecido</td>
					<td class="subtitulocentro">Razão Social</td>
					<td class="subtitulocentro">Andamento</td>
					<td class="subtitulocentro">Num. Banco</td>
					<td class="subtitulocentro">Num. Agência</td>
					<td class="subtitulocentro">Num. Conta Corente</td>
					<td class="subtitulocentro">CNPJ</td>
					<td class="subtitulocentro">Situação da Conta</td>
					<td class="subtitulocentro">Data Inclusão</td>
					<td class="subtitulocentro">Agência Bancaria</td>
				</tr>
				<?php
				$obContaCorrente = new ContaCorrente();
				$arDados = $obContaCorrente->carregaContaCorrentePorPtrid($ptrid);
				
				foreach( $arDados as $dados){
					$cor = ($i % 2) ? "#f4f4f4" : "#e0e0e0";
					
					echo  "<tr bgColor='{$cor}'>" 
						. "	   <td align='center' id='cc_acao{$dados["cocid"]}'>"
						. "        <img src='/imagens/mais.gif' style='cursor:pointer;' onclick='abreHistoricoCC({$dados["cocid"]});'/>"
						. "	   </td>"
						. "	   <td >"
						.          $dados["cocseq_conta"]
						. "	   </td>"
						. "	   <td >"
						.          $dados["cocds_razao_social"]
						. "	   </td>"
						. "	   <td >"
						.          $dados["cocandamento"]
						. "	   </td>"
						. "	   <td >"
						.          $dados["cocnu_banco"]
						. "	   </td>"
						. "	   <td >"
						.          $dados["cocnu_agencia"]
						. "	   </td>"
						. "	   <td >"
						.          $dados["cocnu_conta_corrente"]
						. "	   </td>"
						. "	   <td >"
						.          $dados["coccnpj"]
						. "	   </td>"
						. "	   <td >"
						.          $dados["cocsituacao_conta"]
						. "	   </td>"
						. "	   <td >"
						.          $dados["cocdatainclusao"]
						. "	   </td>"
						. "	   <td >"
						.          $dados["ptragenciabancaria"]
						. "	   </td>"
						. "</tr>"
						. "<tr id='cc_{$dados["cocid"]}' style='display:none;'>"
						. "    <td align='center' valign='center'><img src='/imagens/seta_filho.gif'/></td>"
						. "    <td colspan='15' id='cc_historico_{$dados["cocid"]}'></td>"
						. "</tr>";
						
				}
				
				?>
			</table> 		
 		</td>
 	</tr>
</table>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
	function abreHistoricoExec( id ){
		var acao  = 'exec_acao';
		var td    = 'exec_';
		var historico = 'exec_historico_';
		var carregando = "<img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\">";
		
		var url = "/emenda/emenda.php?modulo=principal/popupConsultaExecucaoPTA&acao=A";
		var pars = 'execFinan=1&exfid='+id;
		
		var cor = "#fafafa";
		var myAjax = new Ajax.Request(
				url, 
				{
					method: 'post', 
					parameters: pars, 
					asynchronous: false,
					onLoading: $(historico + id).update(carregando+' Aguarde enquanto sua solicitação é finalizada. '),
					onComplete: function(r)
					{
						$(historico + id).update(r.responseText)
						$('debug').update(r.responseText);
						//return false;
					}
				});
	
		$(td + id).style.display = 'table-row';
		
		document.getElementById( acao + id ).innerHTML = "<img src='/imagens/menos.gif' style='cursor:pointer;' onclick='fechaHistoricoExec(" + id + ");'/>";
	}

	function fechaHistoricoExec( id ){
		var acao  = 'exec_acao';
		var td    = 'exec_';
		var historico = 'exec_historico_';
		
		document.getElementById( td + id ).style.display = 'none';
		document.getElementById( historico + id ).innerHTML = '';
		document.getElementById( acao + id ).innerHTML = "<img src='/imagens/mais.gif' style='cursor:pointer;' onclick='abreHistoricoExec(" + id + ");'/>";
		
	}
	
	function abreHistoricoCC( id ){
		var acao  = 'cc_acao';
		var td    = 'cc_';
		var historico = 'cc_historico_';
		var carregando = "<img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\">";
		
		var url = "/emenda/emenda.php?modulo=principal/popupConsultaExecucaoPTA&acao=A";
		var pars = 'contaCorrente=1&cocid='+id;
		
		var cor = "#fafafa";
		var myAjax = new Ajax.Request(
				url, 
				{
					method: 'post', 
					parameters: pars, 
					asynchronous: false,
					onLoading: $(historico + id).update(carregando+' Aguarde enquanto sua solicitação é finalizada. '),
					onComplete: function(r)
					{
						$(historico + id).update(r.responseText)
						//$('debug').update(r.responseText);
						//return false;
					}
				});
	
		$(td + id).style.display = 'table-row';
		
		document.getElementById( acao + id ).innerHTML = "<img src='/imagens/menos.gif' style='cursor:pointer;' onclick='fechaHistoricoCC(" + id + ");'/>";
	}

	function fechaHistoricoCC( id ){
		var acao  = 'cc_acao';
		var td    = 'cc_';
		var historico = 'cc_historico_';
		
		document.getElementById( td + id ).style.display = 'none';
		document.getElementById( historico + id ).innerHTML = '';
		document.getElementById( acao + id ).innerHTML = "<img src='/imagens/mais.gif' style='cursor:pointer;' onclick='abreHistoricoCC(" + id + ");'/>";
		
	}
</script>
</body>
</html>