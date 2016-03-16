<?php

// Salvar a redistribuição do empenho
if( $_POST['requisicaoAjax'] == 'salvarRedistribuirEmpenho' ){
	ob_clean();
	salvarRedistribuirEmpenho($_POST);	
}

if( $_POST['requisicaoAjax'] == 'CarregaModalredistribuirEmpenhoObra' ){
	ob_clean();
	CarregaModalredistribuirEmpenhoObra($_POST);
}

if( $_POST['requisicaoAjax'] == 'CarregaModalredistribuirEmpenho' ){
	ob_clean();
	CarregaModalredistribuirEmpenho($_POST);
}

if ($_POST['requisicaoAjax'] == 'excluirNotaEmpenho') {
	ob_clean();
	$empid = $_POST['empid'];
	$preid = $_POST['preid'];
	$strLabel = ($_POST['tipo'] == '2') ? 'spar' : ''; #Concatena o nome da tabela de acordo com o tipo da obra PAR: empenhoobraspar PAC: empenhoobra
	$sql = "SELECT empid FROM par.empenho WHERE empid = {$empid} OR empidpai = {$empid} AND empcodigoespecie IN ('03', '01', '13')";
	$arrEmpenho = $db->carregar($sql);
	if ($arrEmpenho) {
		foreach ($arrEmpenho as $empenho) {
			$sql = "update par.empenhoobra{$strLabel} set eobstatus = 'I' where preid = {$preid} and empid = {$empenho['empid']};";
			$db->executar($sql);
		}
		
		$sql = "SELECT empnumero FROM par.empenho WHERE empid = {$empenho['empid']}";
		$empnumero = $db->pegaUm($sql);
			
		insereHistoricoStatusObra( $empenho['empid'], Array(), 'I', "Obra inativada pelo canelamento do empenho $empnumero" );
							
		inativaObras2SemSaldoEmpenho( $empenho['empid'], Array() );
		
		echo ($db->commit()) ? '1':'0';
		die();
	}
}

if ($_POST['requisicaoAjax'] == 'desvinculaObra') 
{
	ob_clean();
	
	$preid 	= $_POST['preid'];
	$tipo	= $_POST['tipo'];
	$proid  = $_POST['proid'];
	
	
	if($tipo == 'PAC')
	{
		$sql2 = "UPDATE  par.processoobraspaccomposicao SET pocstatus = 'I' WHERE preid = {$preid} and proid =  {$proid}";
		if($db->executar( $sql2))
		echo ($db->commit()) ? '1':'0';
		die();
	}
	
}

if( $_POST['preid'] ){

	ini_set("memory_limit","1000M");
	set_time_limit(0);

	$terid = $db->pegaUm("SELECT ter.terid 
							FROM
								par.processoobra  pro
							INNER JOIN par.termocompromissopac ter ON ter.proid = pro.proid AND ter.terstatus = 'A'
							WHERE pro.prostatus = 'A'  and pro.proid = ".$_REQUEST['proid']);

	if( is_array( $_POST['preid'] ) ){
		
		//Atualizo o termo para em reformulação
		$db->executar( "UPDATE par.termocompromissopac SET terreformulacao = TRUE WHERE terid = ".$terid );
		
		foreach( $_POST['preid'] as $preid ){

			$db->executar( "UPDATE obras.preobra SET prereformulacao = TRUE WHERE preid = ".$preid );
			
//			$sql = "select count(supvid) 
//					from obr as.obr ainfraestrutura o 
//						inner join obras.supervisao s on o.obrid = s.obrid
//					where o.preid = '".$preid."' 
//					and s.supstatus = 'A'";
					
			$sql = "SELECT 
						count(supid) 
					FROM 
						obras2.obras o
					INNER JOIN obras2.supervisao s ON o.obrid = s.obrid
					WHERE 
						o.preid = '$preid' 
						AND  s.supstatus = 'A'";
	
			$num_vistorias = $db->pegaUm($sql);
			
			if($num_vistorias > 0) {
				die("<script>
						alert('Esta obra não pode ser reformulada, pois existem vistorias cadastradas.');
						location.href='par.php?modulo=principal/projetos&acao=A';
				      </script>");
			}
			// necessita depara stoid = esdid
			
//			$sql = "SELECT COALESCE(obrpercexec, 0) as percexec, obrid, stoid 
//					FROM ob ras.o brainfraestrutura 
//					WHERE preid = ".$preid." and obsstatus = 'A'";
					
			$sql = "SELECT 
						COALESCE(obrpercentultvistoria, 0) as percexec, 
						obrid, 
						doc.esdid 
					FROM 
						obras2.obras obr
					INNER JOIN workflow.documento doc ON doc.docid = obr.docid
					WHERE 
						preid = $preid 
						AND obrstatus = 'A'";
			
			$arObra = $db->pegaLinha( $sql );	
			if($arObra['percexec'] > 0) {
				die("<script>
						alert('Esta obra não pode ser reformulada, pois a obra esta com ".$percexec."% de execução.');
						location.href='par.php?modulo=principal/projetos&acao=A';
				      </script>");
			}
			
			include_once APPRAIZ . "includes/workflow.php";
			
			$docid = $db->pegaUm("SELECT docid FROM obras.preobra WHERE preid='".$preid."'");
			$result = wf_alterarEstado( $docid, $aedid = WF_AEDID_APROVADO_ENVIAR_EM_REFORMULACAO, true, $d = array('preid' => $preid));

			if($result) {
			
				$sql = "UPDATE obras.preobra SET predatareformulacao=NOW(), preusucpfreformulacao='".$_SESSION['usucpf']."' WHERE preid='".$preid."'";
				$db->executar($sql);
				
				
				// criar novo preid (replicar obras.preobra) 
				$sql = "INSERT INTO obras.preobra(
			            docid, presistema, preidsistema, ptoid, preobservacao, 
			            prelogradouro, precomplemento, estuf, muncod, precep, prelatitude, 
			            prelongitude, predtinclusao, prebairro, preano, qrpid, predescricao, 
			            prenumero, pretipofundacao, prestatus, entcodent, preprioridade, 
			            terid, resid, prevalorobra, tooid, muncodpar, estufpar, premcmv, 
			            preidpai, predatareformulacao, preusucpfreformulacao) 
			            (SELECT NULL, presistema, preidsistema, ptoid, preobservacao, 
			            prelogradouro, precomplemento, estuf, muncod, precep, prelatitude, 
			            prelongitude, predtinclusao, prebairro, preano, NULL, predescricao, 
			            prenumero, pretipofundacao, prestatus, entcodent, preprioridade, 
			            terid, resid, prevalorobra, tooid, muncodpar, estufpar, premcmv, 
			            {$preid}, predatareformulacao, preusucpfreformulacao FROM obras.preobra WHERE preid = {$preid}) RETURNING preid";
				
				$novopreid = $db->pegaUm($sql);
				
				// buscar o qrpid da preobraanalise antiga
				$sql = "SELECT qrpid FROM obras.preobraanalise WHERE preid = {$preid}";
				$antigoqrpid = $db->pegaUm($sql);
				
				// buscar o qrpid da preobra2 antiga
				$sql = "SELECT qrpid FROM obras.preobra WHERE preid = {$preid}";
				$antigoqrpid2 = $db->pegaUm($sql);
				
				// criar novo qrpid (replicar questionario.questionarioresposta)
				$sql = "INSERT INTO questionario.questionarioresposta(
			            queid, qrptitulo, qrpdata)
			            (SELECT queid, 'OBRAS (".$novopreid.")', qrpdata FROM questionario.questionarioresposta WHERE qrpid='".$antigoqrpid."') RETURNING qrpid";
				$novoqrpid = $db->pegaUm($sql);
				
				// pegando descricao o municipio
				$mundescricao = $db->pegaUm("SELECT m.mundescricao FROM obras.preobra p 
										     LEFT JOIN territorios.municipio m ON m.muncod = p.muncod 
										     WHERE preid = {$preid}");
				
				// criar novo qrpid (replicar questionario.questionarioresposta)
				$sql = "INSERT INTO questionario.questionarioresposta(
			            queid, qrptitulo, qrpdata)
			            (SELECT queid, 'OBRAS (".$novopreid." - ".$mundescricao.")', qrpdata FROM questionario.questionarioresposta WHERE qrpid='".$antigoqrpid2."') RETURNING qrpid";
				$novoqrpid2 = $db->pegaUm($sql);
				
				
				$db->executar("UPDATE obras.preobra SET qrpid='".$novoqrpid2."' WHERE preid='".$novopreid."'");
				
				
				// criar novo panid (replicar) 
				$sql = "INSERT INTO obras.preobraanalise(
			            preid, poadataanalise, poastatus, poausucpfinclusao, qrpid, poaindeferido, 
			            poajustificativa)
			            (SELECT '".$novopreid."', poadataanalise, poastatus, poausucpfinclusao, ".(($novoqrpid)?"'".$novoqrpid."'":"NULL").", poaindeferido, 
			            poajustificativa FROM obras.preobraanalise WHERE preid = {$preid})";
				$db->executar($sql);
				
				$sql = "INSERT INTO questionario.resposta(
			            perid, qrpid, usucpf, itpid, resdsc)
			            (SELECT perid, '".$novoqrpid."', usucpf, itpid, resdsc FROM questionario.resposta WHERE qrpid='".$antigoqrpid."')";
				$db->executar($sql);
				
				$sql = "INSERT INTO questionario.resposta(
			            perid, qrpid, usucpf, itpid, resdsc)
			            (SELECT perid, '".$novoqrpid2."', usucpf, itpid, resdsc FROM questionario.resposta WHERE qrpid='".$antigoqrpid2."')";
				$db->executar($sql);
				
				$sql = "INSERT INTO obras.preobrafotos(
			            pofdescricao, preid, arqid) 
			            (SELECT pofdescricao, '".$novopreid."', arqid FROM obras.preobrafotos WHERE preid = {$preid})";
				$db->executar($sql);
				
				$sql = "INSERT INTO obras.preobraanexo(
			            preid, poadescricao, arqid, podid, datainclusao, usucpf, 
			            poasituacao)
			            (SELECT '".$novopreid."', poadescricao, arqid, podid, datainclusao, usucpf, 
			            poasituacao FROM obras.preobraanexo WHERE preid = {$preid})";
				$db->executar($sql);
				
				$sql = "INSERT INTO obras.preplanilhaorcamentaria(
			            preid, itcid, ppovalorunitario) 
			            (SELECT '".$novopreid."', itcid, ppovalorunitario FROM obras.preplanilhaorcamentaria WHERE preid = {$preid})";
				$db->executar($sql);
				
				if( $arObra['obrid'] ){
					
					$sql = "SELECT
								aedid
							FROM workflow.acaoestadodoc 
							WHERE 
								esdiddestino = ".OBR_ESDID_EM_REFORMULACAO."
								AND esdidorigem = {$arObra['esdid']}";
		
					$aedid = $db->pegaUm($sql);
					
					if( $aedid == '' ){
						$sql = "INSERT INTO workflow.acaoestadodoc 
									(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada, 
									esdsncomentario, aedvisivel, aedcodicaonegativa)
								VALUES( 
									{$arObra['esdid']}, ".OBR_ESDID_EM_REFORMULACAO.", 'Enviar para reformulação', 'A', 'Enviada para reformulação', 
									true, false, false )
								RETURNING
									aedid";
						
						$aedid = $db->pegaUm($sql);
					}
		
					wf_alterarEstado( $arObra['docid'], $aedid, 'Tramitado por reformular projeto obra preid = '.$preid, Array( 'docid' => $arObra['docid'] ) );
					
//					$sql = "INSERT INTO obras.movsituacaoobra(obrid, stoid, usucpf, msodtinclusao, msoorigem) 
//							VALUES (".$arObra['obrid'].", ".$arObra['stoid'].", '".$_SESSION['usucpf']."', now(), 'P')";
//					$db->executar($sql);
//					//necessita depara stoid = esdid
//					$sql = "UPDATE obr as.ob rainfraestrutura SET stoid = '9' WHERE preid = {$preid} and obsstatus = 'A'";
//					$db->executar($sql);
				}
			
			} else {
				echo "<script>
						alert('Tramitação não foi possivel. Possivelmente não possui perfil');
						location.href = 'par.php?modulo=principal/projetos&acao=A';
				      </script>";
				exit();
			}	
			if($db->commit()){
				//enviaEmailPlanodeMetasObrasReformulacao( $_SESSION['par']['inuid'], $preid );
			}
		}
		
		echo "<script>
				alert('Obra(s) selecionada(s) em fase de reformulação./nEntre na obra para poder reformula-las.');
				location.href = window.location;
	 		</script>";
		exit();
	}
}

if( $_POST['cancelaReformulacao'] == 'cancela' ){
	global $db;

	//$dado = deletaReformulacaoObra( $_POST['preidCancelamento'], $_POST['anoCancelamento'], $_REQUEST['proid'] );
	
	$dado = cancelarReformulacaoObra( $_POST['preidCancelamento'] );
	$db->executar( "UPDATE obras.preobra SET prereformulacao = false WHERE preid = ".$_POST['preidCancelamento'] );
	$db->commit();
	echo "<script>
				alert('Reformulação cancelada com sucesso!');
				location.href='par.php?modulo=principal/projetos&acao=A&abas=reformularProjetoObras&proid=".$_REQUEST['proid']."';
	 		</script>";
	exit;
}

if( !$_REQUEST['proid'] ){
	echo '<script>alert("Escolha um Processo para reformular!");
			history.back(-1);
	</script>';
	die();
}


if( $_REQUEST['itrid'] ) $_SESSION['par']['itrid'] = $_REQUEST['itrid'] ;
if( $_REQUEST['estuf'] ) $_SESSION['par']['estuf'] = $_REQUEST['estuf'] ;
if( $_REQUEST['muncod'] ) $_SESSION['par']['muncod'] = $_REQUEST['muncod'] ;
if( $_REQUEST['inuid'] ) $_SESSION['par']['inuid'] = $_REQUEST['inuid'] ;

//include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

include_once APPRAIZ.'par/classes/Projeto.class.inc';
$obProjeto = new Projeto( $_REQUEST );

echo montarAbasArray( $obProjeto->criaAbaPar(4), "par.php?modulo=principal/projetos&acao=A&abas=reformularProjetoObrasPar&proid={$_REQUEST['proid']}" );

$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];
$nmEntidadePar = EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar,$_SESSION['par']['itrid']);
monta_titulo( $nmEntidadePar, 'Reformular Projeto' );

$sql = "select 
			substring(p.pronumeroprocesso, 12, 4) as ano, 
		    p.pronumeroprocesso as processo, 
		    'Obras do PAC' as tipo
		from par.processoobra p where p.prostatus = 'A' and proid = {$_REQUEST['proid']}";
$arProcesso = $db->pegaLinha($sql);
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="../estrutura/js/funcoes.js"></script>
<form name="formulario" id="formulario" method="post">   
    <input type="hidden" name="preidCancelamento" id="preidCancelamento" value="">
    <input type="hidden" name="anoCancelamento" id="anoCancelamento" value="">
    <input type="hidden" name="cancelaReformulacao" id="cancelaReformulacao" value="">
    <input type="hidden" name="requisicaoAjax" id="requisicaoAjax" value="">
	<input type="hidden" name="cod" id="cod" value="<?=$_REQUEST['cod'] ?>">
	<input type="hidden" name="tipo" id="tipo" value="<?=$_REQUEST['tipo'] ?>">
    <?=cabecalhoProcesso($arProcesso); ?>
    <table cellspacing="0"  border="0"  align="center" style="border-top: none; border-bottom: none;" class="tabela">
		<tr>
			<td width="2%" bgcolor="#eeeeee">
				
			</td>
			<td bgcolor="#eeeeee">
				<img align="absmiddle" src="../imagens/money_cancel.gif">
				<b>= Redistribuir Empenho. </b>
			</td>
		</tr>
	</table>
	<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
		<tr>
			<td class="SubTituloDireita"><center><b>Obras a serem reformuladas</b></center></td>
		</tr>
		<tr id="tr_div">
			<td> 
			<?
			$sql = "SELECT DISTINCT
							'<center>'||
							-- CASO SEJA REFORMULACAO
							CASE WHEN ( pre.prereformulacao IS TRUE AND ter.terreformulacao IS TRUE ) 
							THEN 
								-- CASO EXISTA EMPENHO
								CASE WHEN EXISTS (SELECT  
									     e.empnumero
									FROM par.empenho e
									INNER JOIN par.empenhoobra eo ON e.empid = eo.empid
									WHERE                 e.empstatus = 'A'
										       AND eo.eobstatus = 'A'
										       AND eo.preid = pre.preid )
								THEN
								-- MOSTRA PARA REDISTRIBUIR
								'<span style=\"white-space: nowrap;\">'
								|| '<img align=\"absmiddle\" onclick=\"redistribuirEmpenho(' || pre.preid || ' , \'PAC\', '|| poc.proid ||');\" title=\"Redistribuir Empenho\" style=\"cursor:pointer;\" src=\"../imagens/money_cancel.gif\">&nbsp;'
								ELSE -- CASO NÃO EXISTA EMPENHO
									
									-- CASO A OBRA NÃO PERTENÇA A NENHUM TERMO MOSTRA O BOTÃO DE CANCELAR A VINCULAÇÃO DA OBRA AO CONTRATO
									CASE WHEN pre.preid not in ( 
										select distinct preid from par.termocomposicao  t
										INNER JOIN par.vm_documentopar_ativos d on d.dopid = t.dopid
										where preid is not null
										union all
										select distinct preid from par.termoobra o
										INNER JOIN par.termocompromissopac t on t.terid = o.terid and terstatus = 'A'
									
									)THEN
										'<span style=\"white-space: nowrap;\">'
										|| '<img align=\"absmiddle\" onclick=\"desvinculaObra(' || pre.preid || ', {$_REQUEST['proid']}  )\" title=\"Desvincular obra do processo\" style=\"cursor:pointer;\" src=\"../imagens/excluir.gif\">' 
										'<img align=\"absmiddle\" onclick=\"desvinculaObra(' || pre.preid || ', {$_REQUEST['proid']} )\" title=\"Desvincular obra do processo\" style=\"cursor:pointer;\" src=\"../imagens/excluir.gif\">' 
									ELSE
										'<span style=\"white-space: nowrap;\">'
										||
										'<img align=\"absmiddle\" id=\'' || pre.preid  || '\' title=\"Esta obra já está vinculada a um termo, não pode ser desvinculada do processo\" style=\"width:15px; height:15px; cursor:pointer;\" src=\"../imagens/exclamacao_checklist_vermelho.png\">&nbsp;'
									
									END
								
								END
								||'<span style=\"white-space: nowrap;\">'
								||'<img style=\"cursor:pointer\" title=\"Editar Reformulação\" src=\"/imagens/editar_nome.gif\" onclick=\"janela(\'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&preid=' || pre.preid || '\',800,600,\'ObrasPac\');\" border=\"0\" />&nbsp;</span>' 
								
							ELSE 
								-- CASO EXISTA EMPENHO
								CASE WHEN EXISTS (SELECT  
									     e.empnumero
									FROM par.empenho e
									INNER JOIN par.empenhoobra eo ON e.empid = eo.empid
									WHERE                 e.empstatus = 'A'
										       AND eo.eobstatus = 'A'
										       AND eo.preid = pre.preid )
								THEN
								-- MOSTRA PARA REDISTRIBUIR
									'<img align=\"absmiddle\" onclick=\"redistribuirEmpenho(' || pre.preid || ' , \'PAC\', '|| poc.proid ||');\" title=\"Redistribuir Empenho\" style=\"cursor:pointer;\" src=\"../imagens/money_cancel.gif\">&nbsp;'
								ELSE -- CASO NÃO EXISTA EMPENHO
									
									-- CASO A OBRA NÃO PERTENÇA A NENHUM TERMO MOSTRA O BOTÃO DE CANCELAR A VINCULAÇÃO DA OBRA AO CONTRATO
									CASE WHEN pre.preid not in ( 
										select distinct preid from par.termocomposicao  t
										INNER JOIN par.vm_documentopar_ativos d on d.dopid = t.dopid
										where preid is not null
										union all
										select distinct preid from par.termoobra o
										INNER JOIN par.termocompromissopac t on t.terid = o.terid and terstatus = 'A'
									)THEN
										
										'<img align=\"absmiddle\" onclick=\"desvinculaObra(' || pre.preid || ', {$_REQUEST['proid']} )\" title=\"Desvincular obra do processo\" style=\"cursor:pointer;\" src=\"../imagens/excluir.gif\">' 
									ELSE
										'<img align=\"absmiddle\" id=\'' || pre.preid  || '\' title=\"Esta obra já está vinculada a um termo, não pode ser desvinculada do processo\" style=\"width:15px; height:15px; cursor:pointer;\" src=\"../imagens/exclamacao_checklist_vermelho.png\">&nbsp;'
									END
								
								END
								||
								CASE WHEN (	select
											ed.esdid
										from workflow.documento d
										inner join workflow.estadodocumento ed on ed.esdid = d.esdid
										where
										d.docid = pre.docid) in (".WF_PAR_OBRA_APROVADA.", ".WF_TIPO_OBRA_APROVADA.") 
								THEN
									'<input type=\"checkbox\" name=\"preid[]\" id=\"preid_' || pre.preid || '\" value=\"' || pre.preid || '\">' 
								ELSE
									'<img align=\"absmiddle\" id=\'' || pre.preid  || '\' title=\"Apenas obras na situação \'Obra Aprovada\'podem ser reformuladas\" style=\"cursor:pointer;\" src=\"../imagens/exclama.gif\">'
								END
							END||'</center>' as acao,
							pre.preid || '  ' as codigo,
							pre.predescricao as predescricao,
							CASE WHEN ( pre.prereformulacao IS TRUE AND ter.terreformulacao IS TRUE  ) THEN '<center><b>Em Reformulação</b></center>' ELSE '' END as situacao,
							pre.preano||'&nbsp;' as ano,
                            pre.prevalorobra as valor
						FROM 
							par.processoobraspaccomposicao poc
						INNER JOIN obras.preobra 			pre ON pre.preid = poc.preid
						LEFT  JOIN par.termocompromissopac 	ter ON ter.proid = poc.proid AND ter.terreformulacao = true AND ter.terstatus = 'A'
						WHERE
							poc.pocstatus = 'A' and
							poc.proid = {$_REQUEST['proid']} 
						ORDER BY
							predescricao";
// 			ver( simec_htmlentities($sql), d);
				$cabecalho = array("&nbsp;Ações&nbsp;", "Código", "Descrição da Obra", "Situação da Obra", "Ano da Obra", "Valor da Obra" );
				$db->monta_lista($sql,$cabecalho,50000,5,'N','95%','S','formulariomontalista');
			?></td>
		</tr>
		<tr>
			<td><input type="button" onclick="javascript: reformularSubacao();" value="Reformular Selecionadas">&nbsp;
			<input type="button" onclick="javascript: historicoReformulacao( <?=$_REQUEST['proid'] ?> );" value="Histórico de Reformulações"></td>
		</tr>
	</table>

	<div id="div_redistribuir" title="Tela de Redistribuição de Empenho" style="display: none; text-align: center;">
			
			<div id="div_obra_distribuicao" >
			</div> 
			<?php monta_titulo( "Remanejar Empenho",'' );?>
			<div id="div_distribuicao" >
			</div>
	</div>
	<div id="divdebug" ></div>	
</form>
<script type="text/javascript">

jQuery.noConflict();

jQuery(function(){

	jQuery('.campoSoma').live('keyup', function(){
		var arCampo = jQuery(this).attr('name').split('_');
		var preid = arCampo[1];
		atualizaValoresTotais( preid );
	});
	
	jQuery('.campoSoma').live('blur', function(){
		var arCampo = jQuery(this).attr('name').split('_');
		var preid 	= arCampo[1];  
        var empid  	= jQuery('[name="radioempenhoatual"]:checked').val();

		var valorobra 			= jQuery('[name="valorobra_'+preid+'"]').val();
		var valorempenhoobra 	= jQuery('[name="valorempenhoobra_'+preid+'"]').val();
		var vrldistibuido		= replaceAll(replaceAll(jQuery(this).val(), '.', ''), ',', '.');
		var vrlRestante			= jQuery('#vrlempenhoatual_'+empid).val();
		
		if( parseFloat(vrldistibuido) > parseFloat(vrlRestante) ){
			if( parseFloat(vrlRestante) < 0 ){
				alert('O valor empenhado está maior que o valor disponível para a obra');
				jQuery('[name="input_'+preid+'"]').val('');
				jQuery('[name="input_'+preid+'"]').attr('disabled',true);
				jQuery('[name="check_'+preid+'"]').attr('checked',false);
			} else {
				alert('O valor informado está maior que o valor disponível para empenho na obra');
				jQuery(this).val(number_format(vrlRestante, 2, ',', '.'));
			}
			atualizaValoresTotais( preid );
		}	
	});	
});

function atualizaValoresTotais( preid ){
	atualizaTotal('campoSoma', 'totaldistribuido', 'campo');
	
	var empidDistribuir  = jQuery('[name="radioempenhoatual"]:checked').val();
	var vrlEmpenhoAtual  = jQuery('#vrlempenhoatual_'+empidDistribuir).val();
	var totaldistribuido = replaceAll(replaceAll( jQuery('[name="totaldistribuido"]').val(), '.', ''), ',', '.');
	
	if( parseFloat(totaldistribuido) > parseFloat(vrlEmpenhoAtual) ){
	
		alert('O valor total informado está maior que o valor disponível para empenho na obra');
		jQuery('[name="input_'+preid+'"]').val(number_format('0', 2, ',', '.'));
		atualizaTotal('campoSoma', 'totaldistribuido', 'campo');
	}
	
	var total = parseFloat(vrlEmpenhoAtual) - parseFloat(replaceAll(replaceAll( jQuery('[name="totaldistribuido"]').val(), '.', ''), ',', '.'));
	jQuery('[name="totalrestante"]').val( number_format(total, 2, ',', '.') );
}


function habilitaDistribuicao( preid ){
	
	if( jQuery('[name="radioempenhoatual"]:checked').length == 0 ){
		alert('Selecione o número do empenho a distribuir');
		jQuery('[name="check_'+preid+'"]').attr('checked', false);
		return false;
	}
	if(jQuery('[name="check_'+preid+'"]:checked').length > 0){
		jQuery('[name="input_'+preid+'"]').attr('disabled',false);
		jQuery('[name="input_'+preid+'"]').focus();
	} else	{
		jQuery('[name="input_'+preid+'"]').val('');
		jQuery('[name="input_'+preid+'"]').attr('disabled',true);
		atualizaValoresTotais( preid );
	}
}
function reformularSubacao(){
	
	erro = 0;
	jQuery('[id^=preid_]').each(function(i, v) {
		
		if( jQuery(v).attr('checked') ){
			erro = 1;
		}
	});
	if( erro == 0 ){
		alert('Você precisa selecionar uma obra para reformular!');
	} else {
		if( confirm( "Tem certeza que deseja reformular essa(s) obra(s)?" ) ){
			divCarregando('','Reformulação em Andamento<br>Isso pode levar algum tempo. Aguarde...');
			jQuery('#formulariomontalista').submit();
		}
	}
}

function cancelaReformulacaoObra(preid, ano){
	if(confirm("Você tem certeza que deseja cancelar essa reformulação?\nTodas as alterações serão perdidas.")){
		jQuery('#preidCancelamento').val(preid)
		jQuery('#anoCancelamento').val(ano)
		jQuery('#cancelaReformulacao').val('cancela')
		jQuery('#formulario').submit();
	}
}

function historicoReformulacao(proid){
	//location.href = "par.php?modulo=principal/historicoReformulacao&acao=A&prpid=" + prpid;
	location.href = "par.php?modulo=principal/projetos&acao=A&abas=historicoReformulacaoObraPar&proid=" + proid;
}

function salvarDistribuicao()
{
	var inputs = '';
	var checks = '';
	var boolcheck = false;
	var totalDistribuir = jQuery('#total_distribuir').val();
	var tipo 			= jQuery('#tipo_distribuir').val();
	var preidDistribuir = jQuery('#preid_distribuir').val();
	var empidDistribuir = jQuery('[name="radioempenhoatual"]:checked').val();
	var vrlEmpenhoAtual = jQuery('#vrlempenhoatual_'+empidDistribuir).val();
	var valorDistribuir = 0;

	if( jQuery('[name="radioempenhoatual"]:checked').length == 0 )
	{
		alert('Selecione o número do empenho a distribuir');
		return false;
	}
	
	jQuery.each(jQuery( ":input" ),function( i, val ) {
			
		  var idinput = this.id;
		  
		  if( idinput.indexOf("input_") == 0 )
		  {
			  inputs = inputs + this.id +'='+ this.value + '|';
		  }
		  if( idinput.indexOf("check_") == 0 )
		  {
					  
			  if(jQuery('#'+idinput+':checked').length ) 
			  {
				  var vrlInformado = replaceAll(replaceAll(jQuery('[name="input_'+this.value+'"]').val(), '.', ''), ',', '.');
				  valorDistribuir = parseFloat(valorDistribuir) + parseFloat(vrlInformado);
				   valCheck = true;
				   boolcheck = true;
			  }
			  else
			  {
				  valCheck = false;
			  }
				  
			  checks = checks + this.id +'='+ valCheck + '|';
		  }
	});
	
	if( parseFloat(valorDistribuir) > parseFloat(vrlEmpenhoAtual) ){
		alert('O valor distribuido R$ '+number_format(valorDistribuir, 2, ',', '.')+' está maior que valor empenhado R$ '+number_format(vrlEmpenhoAtual, 2, ',', '.'));
		return false;
	}

	if(!boolcheck)
	{
		alert('Selecione pelo menos uma obra para distribuir o empenho');
		return false;
	}

	var totaldistribuido = replaceAll(replaceAll( jQuery('[name="totaldistribuido"]').val(), '.', ''), ',', '.');
	
	jQuery.ajax({
	    type: "POST",
	    url: window.location.href,
   		data: "requisicaoAjax=salvarRedistribuirEmpenho&arraycheck="+checks+ "&arrayinputs="+inputs+"&total="+totalDistribuir+"&empidDistribuir="+empidDistribuir+"&tipo="+tipo+"&preidDistribuir="+preidDistribuir+'&totaldistribuido='+totaldistribuido+'&'+jQuery('#formdistribuir').serialize(),
   		async: false,
   		success: function(msg){
//   			alert(msg);
				//jQuery( "#divdebug" ).html(msg);
   				var arrResposta = msg.split("|");
   				var status = arrResposta[0];
  				var mensagem = arrResposta[1]; 
				if( status == 'ERRO')
				{
				  	alert(mensagem);
				}
				else
				{
					alert('Operação realizada com sucesso!');
					jQuery( '#div_redistribuir' ).dialog( 'close' );
        			window.location.reload();
				}
	    				
		    			
    		}
    	});
}              
                
function excluirNotaEmpenho(empid, preid){
	var tipo = 'PAC';
	
    if(confirm('Tem certeza que deseja excluir essa nota de empenho da obra?')){
	    jQuery.ajax({
	    	type: "POST",
	        url: window.location.href,
	        data: "requisicaoAjax=excluirNotaEmpenho&empid="+empid+"&preid="+preid+"&tipo="+tipo,
	        async: false,
	        success: function(msg){
	        	if(msg == '1'){
	            	alert('Operação realizada com sucesso!');
	                window.location.reload();
	            } else {
	            	alert('Falha na Operação!');
	            }
		}});

    }
}
function desvinculaObra(preid, proid){
	var tipo = 'PAC';
	
    if(confirm('Tem certeza desvincular a obra do processo?'))
    {
	    jQuery.ajax({
	    	type: "POST",
	        url: window.location.href,
	        data: "requisicaoAjax=desvinculaObra&preid="+preid+"&tipo="+tipo+"&proid="+proid,
	        async: false,
	        success: function(msg){
	        	if(msg == '1'){
	            	alert('Operação realizada com sucesso!');
	                window.location.reload();
	            } else {
	            	alert('Falha na Operação!');
	            }
		}});

    }
}

function redistribuirEmpenho( preid , tipo, proid) {

	jQuery.ajax({
    type: "POST",
    url: window.location.href,
    data: "requisicaoAjax=CarregaModalredistribuirEmpenhoObra&preid="+preid+ "&tipo=" + tipo + "&proid=" + proid,
    async: false,
    success: function(msg){
			if(msg != 'erro')
			{
					jQuery( "#div_obra_distribuicao" ).html(msg);
					
					jQuery.ajax({
		    		type: "POST",
		    		url: window.location.href,
		    		data: "requisicaoAjax=CarregaModalredistribuirEmpenho&preid="+preid+ "&tipo=" + tipo + "&proid=" + proid,
		    		async: false,
		    		success: function(msg){
							if(msg != 'erro'){
							  	jQuery( "#div_distribuicao" ).html(msg);
							  	jQuery( "#div_redistribuir" ).show();
							  	jQuery( '#div_redistribuir' ).dialog({
				    				resizable: true,
				    				minWidth: 1000,
				    				modal: true,
				    				show: { effect: 'drop', direction: "up" },
				    				buttons: {
				    					'Salvar': function() {
				    						salvarDistribuicao();
				    					},
				    					'Fechar': function() {
				    						jQuery( this ).dialog( 'close' );
				    					}
				    				}
				    			});
								
							}
							else
							{
								alert('erro');
							}
		    				
			    			
			    		}
			    	}); 
			}
			else
			{
				alert('erro');
			}
    		
	  		}
	  	});
	jQuery('input[type="text"], input[type="button"], input[type="password"]').css('font-size', '14px');
   }

</script>
</form>
