<?php 

function verificarComentario($dados) {
	global $db;
	
	ob_clean();
	
	$esdsncomentario = $db->pegaUm("SELECT esdsncomentario FROM workflow.acaoestadodoc WHERE aedid='".$dados['aedid']."'");
	
	if($esdsncomentario=="t") {
		echo "TRUE";
	} else {
		echo "FALSE";
	}
	
}

function comboAdvogados( $dados ){
	
	global $db;
	
	$sql = "SELECT 
				adv.advid as codigo, 
				ent.entnome as descricao 
			FROM 
				profeinep.advogados adv 
			INNER JOIN entidade.entidade ent ON ent.entid = adv.entid 
			WHERE 
				adv.coonid='".$dados['cooid']."' 
				AND ent.entstatus = 'A' 
				AND adv.advstatus = 'A'
		   	ORDER BY ent.entnome";
	$db->monta_combo('advid', $sql, 'S', 'Selecione...', '', '', '', 300, 'S','advid','',$advid);
}

function tramitar($dados){
	
	global $db;
	
	require_once APPRAIZ . 'includes/workflow.php';
	foreach( $dados['docid'] as $prcid => $docid){
	
		$test = wf_alterarEstado( $docid, $dados['aedid'], $dados['comentario'], array( 'docid' => $docid, 'prcid' => $prcid, 'emlote' => '1', "cooid" => $dados['cooid'], "advid" => $dados['advid'] ) );
		if( $dados['advid']!='' && $test ){
			$sql = "INSERT INTO profeinep.historicoadvogados(prcid,advid) VALUES ($prcid,".$dados['advid'].")";
			$db->executar($sql);
		}
		
		if( $_REQUEST['dtprazo']!='' && $test ){
			$dt = explode('/',$_REQUEST['dtprazo']);
			$dt = $dt[2]."/".$dt[1]."/".$dt[0];
			
			$sql = "UPDATE profeinep.estruturaprocesso SET 
						espdtrespexterna = '{$dt}'
					WHERE
						prcid = {$prcid} ";
			$db->executar($sql);
		}
		
		$guia[] = $prcid;
	}
	
	
	if( true ){
		$db->commit();
		$html = "<body>";
		$html .= "<form method=\"post\" id=\"formGuia\" action=\"/profeinep/profeinep.php?modulo=principal/guiaDistribuicao&acao=A\">";
		foreach($guia as $prcid){
			$hstid = $db->pegaUm("SELECT DISTINCT
										hst.hstid
									FROM
										profeinep.processoprofeinep prc
									INNER JOIN profeinep.estruturaprocesso    esp ON esp.prcid = prc.prcid
									INNER JOIN workflow.documento 	       doc ON doc.docid = esp.docid
									INNER JOIN workflow.estadodocumento    esd ON esd.esdid = doc.esdid
									INNER JOIN workflow.historicodocumento hst ON hst.docid = doc.docid 
															AND hst.htddata = (SELECT max(h.htddata) 
																	FROM workflow.historicodocumento h 
																	WHERE h.docid = doc.docid )
									WHERE
										prc.prcstatus = 'A'
										AND prc.prcid = $prcid");
			$html .= "<input type=\"hidden\" name=\"guia[".$hstid."]\" value=\"".$prcid."\" />";		
		}
		$html .= "</form>";
		$html .= "</body>";
		
		$html .= "<script>
				var form = document.getElementById('formGuia');
				form.target = 'guia';
				var janela 	= window.open( '', 'guia', 'width=1300,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
				form.submit();
				janela.focus();
		      </script>";
		echo $html;

		$db->sucesso('principal/tramitaProcessos','');
	}else{
		echo "<script>alert('Erro ao tramitar.');window.history.back();</script>";
	}
}

function montaAcoes($dados){
	
	global $db;
	
	$sql = "SELECT 
				p.pflcod 
			FROM 
				seguranca.perfilusuario pu 
			INNER JOIN seguranca.perfil p ON pu.pflcod = p.pflcod 
			WHERE 
				pu.usucpf = '".$_SESSION['usucpf']."' 
				AND p.pflstatus = 'A' 
				AND p.sisid = ".$_SESSION['sisid'];
				
	$perfil = $db->carregarColuna($sql);
	
	$sql = "SELECT DISTINCT
				aed.aedid as codigo,
				aed.aeddscrealizar as descricao  
			FROM 	
				workflow.acaoestadodoc aed 
			INNER JOIN workflow.estadodocumentoperfil esp ON esp.aedid = aed.aedid
			WHERE 
				aedstatus = 'A' 
				AND aed.aedid NOT IN (53)
				AND aed.esdidorigem = ".$dados['esdid']."".( $perfil && !$db->testa_superuser() ? " AND pflcod in (".implode(',',$perfil).")" : "" )." 
			ORDER  BY 
				2";
	
	$db->monta_combo('aedid',$sql,'S','Selecione...','','','','','S', 'aedid');
}


function lista( $dados = null ){
	
	global $db;
	
	require_once APPRAIZ . 'includes/workflow.php';

	$filtroprocesso[] = " AND prcstatus = 'A' ";
	$filtro_estado 	  = " ";
	
	if($dados['tprid']) {
		$filtroprocesso[] = "tprid='".$dados['tprid']."'";
	}
	if($dados['prcnumsidoc']) {
		$filtroprocesso[] = "prcnumsidoc='".eregi_replace('([^0-9])','',$dados['prcnumsidoc'])."'";
	}
	if($dados['prcnomeinteressado']) {
		$filtroprocesso[] = "removeacento(prcnomeinteressado) ilike(removeacento('%".$dados['prcnomeinteressado']."%'))";
	}
	
	if($dados['prcdtentradaini']) {
		$dataformatadainicio = formata_data_sql($dados['prcdtentradaini']);
	
		$filtroprocesso[] = "prcdtentrada >= '".$dataformatadainicio."'";
	}
	if($dados['prcdtentradafim']) {
		$dataformatadafim = formata_data_sql($dados['prcdtentradafim']);
	
		$filtroprocesso[] = "prcdtentrada <= '".$dataformatadafim."'";
	}
	if($dados['prcdtultimaini']) {
		$dataultimainicio = formata_data_sql($dados['prcdtultimaini']);
	
		$filtroprocesso[] = "TO_CHAR(data, 'YYYY-MM-DD') >= '".$dataultimainicio."'";
	}
	if($dados['prcdtultimafim']) {
		$dataultimafim = formata_data_sql($dados['prcdtultimafim']);
	
		$filtroprocesso[] = "TO_CHAR(data, 'YYYY-MM-DD') <= '".$dataultimafim."'";
	}
	if($dados['advid']) {
		$advid = $dados['advid'];
		$filtroprocesso[] = "advid = '".$dados['advid']."'";
	}
	
	if($dados['anxdesc']) {
		$filtroprocesso[] = "anx.anxdesc ilike '%".$dados['anxdesc']."%'";
	}
	
	if(strlen($dados['proid'])!=0) {
		$code = explode("&", $dados['proid']);
		$filtroprocesso[] = "pro.proid='".$code[0]."' ";
	}
	if($dados['tasid']) {
		$filtroprocesso[] = "tasid='".$dados['tasid']."'";
	}
	if($dados['cooid']) {
		$filtroprocesso[] = "cooid='".$dados['cooid']."'";
	}
	if($dados['esdid']) {
		$filtroprocesso[] = "esd.esdid='".$dados['esdid']."'";
	}
	if($dados['expressaochave']) {
		$filtroprocesso[] = "removeacento(excdsc) ilike (removeacento('%".$dados['expressaochave']."%'))";
	}
	if($dados['tacid']) {
		$filtroprocesso[] = "tac.tacid = ".$dados['tacid'];
	}
	if($dados['prcnumeroprocjudicial']) {
		$filtroprocesso[] = "prc.prcnumeroprocjudicial='".$dados['prcnumeroprocjudicial']."'";
	}
	
	if($_POST['com_sem_adv'] == "1"){
		$filtroprocesso[] = "prc.advid is not null";
	}
	if($_POST['com_sem_adv'] == "0"){
		$filtroprocesso[] = "prc.advid is null";
	}
	
	$cabecalho = array('Ações','Nº do Processo SIDOC','Interessado','Data de Entrada','Situação PROFE/INEP');
		
	$condicoes = ( (count($filtroprocesso) > 1) ? implode(" AND ", $filtroprocesso)  : " AND prcstatus = 'A' " );
				// no filtro inicial, não pode ser listado: Situação Arquivado (30), Processo Encerrado (80), e Processo Finalizado (99).
	
	$qualquer_situacao = $dados['prcnumsidoc']==false && $dados['prcnumeroprocjudicial']==false && $dados['prcnumeroprocjudantigo']==false; 
	
	if($dados['esdid']==false && $qualquer_situacao ) {
//		$condicoes .= " AND esd.esdid not in (30,99,".EST_ANEXADO.") ";
		$condicoes .= " AND esd.esdid not in (30,99) ";
	}
	
	if($dados['encerrados'] != 'sim' && $dados['esdid']!=ESDID_ENCERRADO && $qualquer_situacao ) {
		$condicoes .= " AND esd.esdid <> ". ESDID_ENCERRADO;
	}
	
	if($dados['prioritarios'] == 'sim' ) {
		$condicoes .= " AND prc.prcprioritario = true ";
	}
	
	if($dados['prcnumeroprocjudantigo'] && $qualquer_situacao==false) {
		$condicoes .= " AND prc.prcnumeroprocjudantigo='".$dados['prcnumeroprocjudantigo']."' ";
	}
	
	if($dados['tpdid']) {
		$condicoes .= " AND nud.tpdid = ".$dados['tpdid']." ";
	}
	
	if($dados['nudnumero']) {
		$condicoes .= " AND to_char(nud.nudnumero,'9999999999') LIKE '%".$dados['nudnumero']."%' AND nud.nudano = ".$dados['nudano'];
	}elseif($dados['nudano']){
		$condicoes .= " AND nud.nudano = ".$dados['nudano'];
	}
	
	if($dados['cooid']) {
		$cooid = $dados['cooid'];
		$condicoes .= " AND prc.cooid = ".$dados['cooid'];
		if(!($db->testa_superuser()||possuiPerfil(array(PRF_ADMINISTRADOR))) ){
			if(testaRespCordPerfil()){
				$coordUsu = pegaCoordUsu();
				if(count($coordUsu)>0){
					$condicoes .= " AND prc.cooid in (".implode(",",$coordUsu).") ";
				}else{
					$condicoes .= " AND 1=0 ";
				}
			}
		}
	}
	
	
	$sql = "SELECT DISTINCT
				'<div align=\"center\">
					<input type=\"checkbox\" class=\"doc\" id=\"docid'||prc.prcid||'\" name=\"docid['||prc.prcid||']\" value=\"'||esp.docid||'\" />
				</div>'  as mais,
				CASE WHEN (esd.esdid <> 80) 
					THEN
		            CASE WHEN (current_date - INTERVAL '15 days' > prcdtentrada AND tpr.tipid = 2 ) 
		            	THEN '<font color=red title=\"Passaram 15 dias desde a entrada.\">' || prcnumsidoc || '</font>' 
		                ELSE 
		                CASE WHEN (current_date - INTERVAL '5 days' > prcdtentrada AND tpr.tipid = 1 ) 
		                	THEN '<font color=red title=\"Passaram 5 dias desde a entrada.\">' || prcnumsidoc || '</font>' 
		                    ELSE prcnumsidoc 
		                END 
		            END 
		            ELSE prcnumsidoc 
		        END as prcnumsidoc,
				prcnomeinteressado,
				tpr.tipdsc as prioridade,
				CASE WHEN prc.prcprioritario IS TRUE
					THEN 'Sim'
					ELSE 'Não'
				END as prioritario,           	
				CASE WHEN wultimadata.dataultimocadastro is not null 
					THEN 
	                CASE WHEN (current_date - INTERVAL '15 days' > wultimadata.dataultimocadastro AND tpr.tipid = 2 ) 
	                	THEN '<font color=red title=\"Passaram 15 dias desde o ultimo cadastro.\">' || to_char(wultimadata.dataultimocadastro, 'DD/MM/YYYY') || '</font>' 
	                    ELSE 
	                    CASE WHEN (current_date - INTERVAL '5 days' > wultimadata.dataultimocadastro AND tpr.tipid = 1 ) 
	                    	THEN '<font color=red title=\"Passaram 5 dias desde o ultimo cadastro.\">' || to_char(wultimadata.dataultimocadastro, 'DD/MM/YYYY') || '</font>' 
	                        ELSE to_char(wultimadata.dataultimocadastro, 'DD/MM/YYYY') 
	                    END 
	                END				
		        	ELSE 
	                CASE WHEN (current_date - INTERVAL '15 days' > prcdtentrada AND tpr.tipid = 2 ) 
	                	THEN '<font color=red title=\"Passaram 15 dias desde a entrada.\">' || to_char(prcdtentrada, 'DD/MM/YYYY') || '</font>' 
	                    ELSE 
	                    CASE WHEN (current_date - INTERVAL '5 days' > prcdtentrada AND tpr.tipid = 1 ) 
	                    	THEN '<font color=red title=\"Passaram 5 dias desde a entrada.\">' || to_char(prcdtentrada, 'DD/MM/YYYY') || '</font>' 
	                        ELSE to_char(prcdtentrada, 'DD/MM/YYYY') 
	                    END 
	            	END
				END as dataultimocadastro,
				CASE WHEN data is not null 
					THEN to_char(data	, 'DD/MM/YYYY')
					ELSE '-' 
				END as dttramite,
				coalesce(coo.coodsc,' - - ' ) as coord,
				CASE 
		        	WHEN ( esp.espnumdiasrespexterna IS NOT NULL ) 
						THEN 
							CASE WHEN ( ( current_date - esp.espnumdiasrespexterna )  > prcdtentrada ) 
								THEN 
								CASE WHEN esd.esdid = 376 
									THEN '<font color=red title=\"Situação em atraso.\">' || esd.esddsc || '<br> (' || to_char(prcdtentrada::date + esp.espnumdiasrespexterna, 'DD/MM/YYYY' ) || ') </font>'
									ELSE '<font color=red title=\"Situação em atraso.\">' || esd.esddsc || '</font>' 
								END
								ELSE 
								CASE WHEN esd.esdid = 376
									THEN esd.esddsc || '<br> (' || to_char(prcdtentrada::date + esp.espnumdiasrespexterna, 'DD/MM/YYYY' ) || ')'
									ELSE esd.esddsc
							END 
						END
					WHEN ( espdtrespexterna IS NOT NULL ) 
						THEN 
						CASE WHEN ( current_date > espdtrespexterna ) 
							THEN 
							CASE WHEN esd.esdid = 376 
								THEN '<font color=red title=\"Situação em atraso.\">' || esd.esddsc || '<br> (' || to_char(espdtrespexterna, 'DD/MM/YYYY' ) || ') </font>'
								ELSE '<font color=red title=\"Situação em atraso.\">' || esd.esddsc || '</font>' 
							END
							ELSE 
							CASE WHEN esd.esdid = 376
								THEN esd.esddsc || '<br> (' || to_char(espdtrespexterna, 'DD/MM/YYYY' ) || ')'
								ELSE esd.esddsc
							END 
						END
					ELSE esd.esddsc 
				END as esddsc,
				prc.prcid,
				prc.cooid
		    FROM 
		    	profeinep.processoprofeinep prc 
	    	LEFT JOIN profeinep.estruturaprocesso esp ON prc.prcid = esp.prcid 
	    	LEFT JOIN profeinep.coordenacao coo ON coo.coonid = prc.cooid
	    	LEFT JOIN workflow.documento doc ON doc.docid = esp.docid 
	    	LEFT JOIN (SELECT max(htddata) as data, docid FROM workflow.historicodocumento GROUP BY docid ) wd ON wd.docid = doc.docid
	    	LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
	    	LEFT JOIN profeinep.expressaochave exp ON exp.prcid = prc.prcid  
	    	LEFT JOIN profeinep.procedencia pro ON pro.proid = prc.proid
	        LEFT JOIN profeinep.tipoprioridade tpr ON tpr.tipid = prc.tipid 
	        LEFT JOIN profeinep.anexos anx ON anx.prcid = prc.prcid 
	        LEFT JOIN profeinep.numeracaodocumento nud ON nud.nudid = anx.nudid
	        LEFT JOIN profeinep.tipoacao tac ON tac.tacid = prc.tacid
			LEFT JOIN (
					SELECT
						hd.docid as docid, 
						ed.esddsc,
						ac.aeddscrealizada,
						us.usunome,
						hd.htddata as dataultimocadastro,
						cd.cmddsc
					FROM 
						workflow.historicodocumento hd
					INNER JOIN workflow.acaoestadodoc 		ac ON ac.aedid = hd.aedid
					INNER JOIN workflow.estadodocumento 	ed ON ed.esdid = ac.esdidorigem
					INNER JOIN seguranca.usuario 			us ON us.usucpf = hd.usucpf
					LEFT  JOIN workflow.comentariodocumento cd ON cd.hstid = hd.hstid
					WHERE
						ac.esdidorigem = 77
						AND hd.htddata = (
								SELECT  	
									max(hd2.htddata) as htddata
								FROM 
									workflow.historicodocumento hd2
								INNER JOIN workflow.acaoestadodoc 		ac ON ac.aedid = hd2.aedid
								INNER JOIN workflow.estadodocumento 	ed ON ed.esdid = ac.esdidorigem
								INNER JOIN seguranca.usuario 			us ON us.usucpf = hd2.usucpf
								LEFT  JOIN workflow.comentariodocumento cd ON cd.hstid = hd2.hstid
								WHERE
									hd2.docid = hd.docid
									AND ac.esdidorigem = 77
								)
				) AS wultimadata ON wultimadata.docid = doc.docid
	    	WHERE 
		    	0=0 
				{$condicoes} 
			GROUP BY 
				prcnumsidoc, prcnomeinteressado, prcdtentrada,tpr.tipdsc, prc.prcprioritario, 
				esd.esddsc, coo.coodsc , esd.esdid, tpr.tipid, dataultimocadastro, esp.espnumdiasrespexterna, 
				data, esp.docid,prc.prcid,prc.cooid,esp.espdtrespexterna
    		ORDER BY 2 ";
	$arCabecalho = Array("&nbsp","N° do Processo SIDOC", "Interessado", "Prioridade","Relevante","Data de Entrada","Data da Ultima Tramitação", "Coordenação", "Situação PROFE/INEP");
	$arTipo      = Array("","","","","","","","",""); 

	unset($_REQUEST['req']);
	unset($_POST['req']);
	
	if( !($dados = $db->carregar($sql)) ){
		echo "Ocorreu um erro inesperado.";
	} 
	$linhas = Array();
	$count = 0;
	if( $_POST['aedid'] != '' && is_array( $dados ) ){
		foreach($dados as $dado){
			$docid = $db->pegaUm("SELECT docid 
								  FROM profeinep.processoprofeinep prc 
	    						  INNER JOIN profeinep.estruturaprocesso esp ON prc.prcid = esp.prcid  
								  WHERE prc.prcid = ".$dado['prcid']);
			if( wf_acaoPossivel2( $docid, $_POST['aedid'], array( 'docid' => $docid, 'prcid' => $dado['prcid'], 'emlote' => '1', 'cooid' => $dado['cooid'], 'advid' => $advid ) ) ){
				unset($dado['prcid']);
				unset($dado['cooid']);
				$linhas[$count] = $dado;
				$count++;
			}
		}
	}else{
		// e se não for paciência...  (Homenagem a Victor Mc Benzi.)
	}
	
	$table = '<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem">
				<tr style="background-color: rgb(230,230,230)">';
	foreach($arCabecalho as $cabecalho){
		$table .= '<td valign="top" bgcolor="" align="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#c0c0c0\';" 
					    style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					    class="title"><strong>'.$cabecalho.'</strong>
					</td>';
	}
	
	$table .= '</tr>';
	if(count($linhas)>0){
		foreach($linhas as $k=>$linha){
			
			if($k%2==0){
				$cor = '#F7F7F7';
			}else{
				$cor = '';
			}
			
			$table .= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">';
			
			$y = 0;
			foreach($linha as $coluna){
				if($arTipo[$y]=='numeric'){
					$estilo = 'align="right" style="color:blue"';
					$coluna = formata_valor($coluna);
				}else{
					$estilo = 'align="left"';
					$coluna = $coluna;
				}
				$table .= ' <td '.$estilo.' title="'.$arCabecalho[$y].'">
								'.$coluna.'
							</td>';
				$y++;
			}
			
			$table .= '</tr>';
		}
	}else{
		$table .= '<tr bgcolor="#F7F7F7" onmouseout="this.bgColor=\'#F7F7F7\';" onmouseover="this.bgColor=\'#ffffcc\';">
						<td align="center" style="color:red" colspan="'.count($arCabecalho).'">
							Nenhum registro encontrado.
						</td>
					</tr>';
	}
	$table .= '</table>';

	echo $table;
}

if($_REQUEST['req']){
	$_REQUEST['req']($_REQUEST);
	die();
}

$sql = "SELECT 
			p.pflcod 
		FROM 
			seguranca.perfilusuario pu 
		INNER JOIN seguranca.perfil p ON pu.pflcod = p.pflcod 
		WHERE 
			pu.usucpf = '".$_SESSION['usucpf']."' 
			AND p.pflstatus = 'A' 
			AND p.sisid = ".$_SESSION['sisid'];

$perfil = $db->carregarColuna($sql);

$notUngcod = Array();
$esdid = Array();

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
if($_SESSION['profeinep_var']['prcid']!=''){
	echo montarAbasArray(monta_abas_processo($_SESSION['profeinep_var']['tprid']),$_SERVER['REQUEST_URI']);
}else{
	montaAbaInicio("profeinep.php?modulo=principal/tramitaProcessos&acao=A");
}
monta_titulo( 'Tramitação em Lotes', '' );

?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

jQuery.noConflict();

jQuery(document).ready(function() {

	function aguarda(val){
		jQuery('#pesquisar').attr('disabled',val);
		jQuery('#tramita').attr('disabled',val);
		jQuery('#limpar').attr('disabled',val);
		if(val){
			jQuery('#aguardando').show();
		}else{
			jQuery('#aguardando').hide();
		}
	}
	
	jQuery('#esdid').change(function(){

		var esdid = jQuery(this).val();

		aguarda(true);

		if(esdid==293){
			jQuery('#trOB').show();
		}else{
			jQuery('#trOB').hide();
		}
		jQuery('#lista').html(' ');
		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: "req=montaAcoes&esdid="+esdid,
			async: false,
			success: function(msg){
				jQuery('#tdAcao').html(msg);
				aguarda(false);
			}
		});
	});

	jQuery('#aedid').live('change',function(){
		var aedid = jQuery(this).val();

		aguarda(true);
		if( aedid == '<?=AEDID_AGUARDAR_RESPOSTA_EXTERNA ?>' ){
			jQuery('.tr_dtprazo').show();
			jQuery('#dtprazo').attr('disabled',false);
			jQuery('#dtprazo').addClass('obrigatorio');
		}else{
			jQuery('.tr_dtprazo').hide();
			jQuery('#dtprazo').attr('disabled',true);
			jQuery('#dtprazo').removeClass('obrigatorio');
		}


		jQuery('#cooid').removeClass('obrigatorio');
		if(jQuery(this).val()=='<?=AEDID_ENCAMINHAR_PARA_O_PROCURADOR ?>'){
			jQuery('.tr_coordenacao').show();
			jQuery('#cooid').addClass('obrigatorio');
			aguarda(false);
			return false;
		}else{
			jQuery('.tr_coordenacao').hide();
			jQuery('#cooid').val('');
			jQuery('#cooid').removeClass('obrigatorio');
			jQuery('#combo_advogados').html(' ');
			jQuery('.tr_advogado').hide();
		}

		jQuery('#req').val('lista');
		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: "&"+jQuery('#formTramita').serialize(),
			async: false,
			success: function(msg){
				jQuery('#lista').html(msg);
				aguarda(false);
			}
		});
	});

	jQuery('#tramita').click(function(){
		var selecionado = false;
		aguarda(true);
		jQuery('.doc').each(function(){
			if(jQuery(this).attr('checked')==true){
				selecionado = true;
			}
		});
		var pendencia = false;
		jQuery('.obrigatorio').each(function(){
			var val = jQuery.trim(jQuery(this).val());
			if( val.length == 0 ){
				jQuery(this).focus();
				pendencia = true;
				aguarda(false);
				return false;
			}
		});
				
		if( pendencia ){
			alert('Campo obrigatório.');
			aguarda(false); 
			return false; 
		}
		
		// VALIDAÇÃO DO COMENTÁRIO
		
		if(jQuery('#aedid').val()=='') {
			alert('Selecione Ação de Tramitação');
			return false;
		}
		
		var exibeComentario=false;
		jQuery.ajax({
			type: "POST",
			url: 'profeinep.php?modulo=principal/tramitaProcessos&acao=A',
			data: "req=verificarComentario&aedid="+jQuery('#aedid').val(),
			async: false,
			success: function(msg){
				if(msg=="TRUE") {
					exibeComentario=true;
				}
			
			}
		});
		
		if(exibeComentario==true && jQuery('#comentario').val()=='') {
			jQuery('#div_comentario').show();
			jQuery( 'html, body' ).animate( { scrollTop: 0 }, 'slow' );
			aguarda(false);
			return false;
		}
		
		// FIM - VALIDAÇÃO DO COMENTÁRIO
		
		if( selecionado ){
			if(confirm('Deseja '+jQuery('#aedid option:selected').html()+'?')){
				jQuery('#req').val('tramitar');
				jQuery('#formTramita').submit();
			}
			aguarda(false);
			return false;
		}else{
			alert('Selecione pelo menos uma solicitação.');
			aguarda(false);
			return false;
		}
	});

	jQuery('#limpar').click(function(){
		jQuery('.normal').each(function(){
			jQuery(this).val('');
		});
		jQuery('.CampoEstilo').each(function(){
			jQuery(this).val('');
		});
		jQuery('.tr_acao').hide();
		jQuery('.tr_coordenacao').hide();
		jQuery('.tr_advogado').hide();
		jQuery('#lista').html(' ');
		jQuery('#pesquisar').click();
	});


	jQuery('#pesquisar').click(function(){
		
		aguarda(true);

		jQuery('#req').val('lista');
		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: jQuery('#formTramita').serialize(),
			async: false,
			success: function(msg){
				jQuery('#lista').html(msg);
				jQuery('#pesquisar').attr('disabled',false);
			}
		});

		aguarda(false);

	});
	
	jQuery('.tprid').click(function(){
		if(jQuery(this).val()=='2'){
			jQuery('.tr_judicial').show();
		}else{
			jQuery('.tr_judicial').hide();
			jQuery('#prcnumeroprocjudicial').val(' ');
			jQuery('#prcnumeroprocjudantigo').val(' ');
		}
	});
	
	jQuery('#tpdid').click(function(){
		if(jQuery(this).val()){
			jQuery('.tr_documento').show();
		}else{
			jQuery('.tr_documento').hide();
			jQuery('#nudnumero').val(' ');
			jQuery('#nudano').val(' ');
		}
	});

	jQuery('#unpid').click(function(){
		if(jQuery(this).val()){
			jQuery('.tr_procedencia').show();
		}else{
			jQuery('.tr_procedencia').hide();
			jQuery('#prodsc').val(' ');
		}
	});

	jQuery('#esdid').click(function(){
		if(jQuery(this).val()){
			jQuery('.tr_acao').show();
		}else{
			jQuery('.tr_acao').hide();
			jQuery('#aedid').val(null);
		}
	});

	jQuery('#cooid').change(function(){
		if(jQuery(this).val()){
			aguarda(true);
			jQuery('#req').val('lista');
			jQuery.ajax({
				type: "POST",
				url: window.location,
				data: jQuery('#formTramita').serialize(),
				async: false,
				success: function(msg){
					jQuery('#lista').html(msg);
				}
			});
			jQuery('.tr_advogado').show();
			jQuery('#req').val('comboAdvogados');
			jQuery.ajax({
				type: "POST",
				url: window.location,
				data: "&cooid="+jQuery(this).val()+"&"+jQuery('#formTramita').serialize(),
				async: false,
				success: function(msg){
					jQuery('#combo_advogados').html(msg);
				}
			});
			aguarda(false);
		}else{
			jQuery('.tr_advogado').hide();
			jQuery('#combo_advogados').html(' ');
		}
	});
	
	
});

</script>
<center>
	<div id="aguardando" style="display:none; position: absolute; background-color: white; height:100%; width:100%; opacity:0.4; filter:alpha(opacity=40)" >
		<div style="margin-top:250px; align:center;">
			<img border="0" title="Aguardando" src="../imagens/carregando.gif">
			Carregando...
		</div>
	</div>
</center>
<form method="post" name="formTramita" id="formTramita" action="">
	<input type="hidden" id="req" name="req" value=""/>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<?php if($db->testa_superuser()){?>
		<tr>
			<td class="SubTituloDireita" width="50%">Situação da Solicitação</td>
			<td>
			<?php 
				// Unidades Gestoras
				$sql = "SELECT DISTINCT
							esdid as codigo, 
							esddsc as descricao  
						FROM 	
							workflow.estadodocumento esd
						INNER JOIN workflow.acaoestadodoc 	  aed ON aed.esdidorigem = esd.esdid
						INNER JOIN workflow.estadodocumentoperfil esp ON esp.aedid = aed.aedid
						WHERE  
							tpdid='".TIPODOC."'
							AND esdstatus = 'A'
							AND esdid NOT IN('".ESDID_ARQUIVADO."')
						ORDER  BY 
							esddsc";
//							AND esdid NOT IN('".ESDID_ARQUIVADO."','".WF_ANEXADO."')
				$db->monta_combo('esdid',$sql,'S','Selecione...','','','','','S', 'esdid');
				
				$displayAcao = ($_REQUEST['esdid']) ? 'table-row' : 'none';
			?>
			</td>
		</tr>
		<?php }else{?>
		<tr>
			<td class="SubTituloDireita">Situação da Solicitação</td>
			<td>
			<?php 
				// Unidades Gestoras
				$sql = "SELECT DISTINCT
							esdid as codigo, 
							esddsc as descricao  
						FROM 	
							workflow.estadodocumento esd
						INNER JOIN workflow.acaoestadodoc 	  aed ON aed.esdidorigem = esd.esdid
						INNER JOIN workflow.estadodocumentoperfil esp ON esp.aedid = aed.aedid
						WHERE  
							tpdid='".TIPODOC."'
							AND aed.aedstatus = 'A'
							AND esd.esdstatus = 'A' 
							AND pflcod in (".($perfil ? implode(',',$perfil) : '0').") 
							AND esdid NOT IN('".ESDID_ARQUIVADO."')
						ORDER  BY 
							esddsc";
//							AND esdid NOT IN('".ESDID_ARQUIVADO."','".WF_ANEXADO."')
				$db->monta_combo('esdid',$sql,'S','Selecione...','','','','','S', 'esdid');
				
				$displayAcao = ($_REQUEST['esdid']) ? 'table-row' : 'none';
			?>
			</td>
		</tr>
		<?php }?>
		<tr class="tr_acao" style="display:<?=$displayAcao?>">
			<td class="SubTituloDireita">Ações de Tramitação</td>
			<td id="tdAcao">
				<?php 
				if($_REQUEST['esdid']){
					$sql = "SELECT DISTINCT
								aed.aedid as codigo,
								aed.aeddscrealizar as descricao  
							FROM 	
								workflow.acaoestadodoc aed 
							INNER JOIN workflow.estadodocumentoperfil esp ON esp.aedid = aed.aedid
							WHERE  
								tpdid = '".TIPODOC."' AND pflcod in (".implode(',',$perfil).")
							ORDER  BY 
								esddsc";
					
					$db->monta_combo('aedid',$sql,'S','Selecione...','','','','','S', 'aedid');
				}
				?>
			</td>
		</tr>
		<tr class="tr_coordenacao" style="display:none;">
			<td class="SubTituloDireita">Selecione coordenação:</td>
			<td>
			<?
				$sql = "SELECT 
							coonid as codigo, 
							coodsc as descricao 
						FROM 
							profeinep.coordenacao
						ORDER BY	
							2";
				$cooid = $_REQUEST['cooid'];
				$db->monta_combo('cooid', $sql, 'S', 'Selecione...', '', '', '', 300, 'S','cooid');
			?>
			</td>
		</tr>
		<tr class="tr_advogado" style="display:none">
			<td class="SubTituloDireita">Selecione advogado:</td>
			<td id="combo_advogados">
			</td>
		</tr>
		<tr class="tr_dtprazo" style="display:none">
			<td class="SubTituloDireita">Data para Resposta Externa:</td>
			<td>
				<?=campo_data2('dtprazo', 'S', 'S', '', 'DD/MM/YYYY','','','','','dtprazo') ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2" id="tdacao" >
				<input type="button" id="limpar" value="Limpar campos da pesquisa." />
			</td>
		</tr>
	</table>
	<div id="lista" >
	</div>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloCentro" colspan="2" id="tdacao" >
				<input type="button" id="tramita" value="Tramitar" />	
			</td>
		</tr>
	</table>
<style>
	.PopUphidden{display:none;width:500px;height:300px;position:absolute;z-index:0;top:50%;left:50%;margin-top:-150;margin-left:-250;border:solid 2px black;background-color:#FFFFFF;}
</style>
<div class="PopUphidden" id="div_comentario">
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('div_comentario').style.display='none'" />
	</div>
	<div style="padding:5px;overflow:auto;height:250px;" >	
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td class="SubTituloCentro">Digite o comentário</td>
	</tr>
	<tr>
		<td align="center"><?=campo_textarea( 'comentario', 'N', 'S', '', '70', '4', '200'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita"><input type="button" name="tramitar2" value="Tramitar" onclick="jQuery('#tramita').click();"> <input type="button" name="fechar" value="Fechar" onclick="document.getElementById('div_comentario').style.display='none';"></td>
	</tr>
	</table>
	</div>
</div>
</form>