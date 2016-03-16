<?php
include_once APPRAIZ . 'includes/workflow.php';

if( $_REQUEST['requisicao'] == 'formRegeraTermo' ){
	include '_funcoes_termo.php';
	
	formRegeraTermo($_REQUEST);
	exit();
}

if( $_REQUEST['requisicao'] == 'carregaMunicipios' ){
	echo carregaMunicipios( $_REQUEST );
	exit();
}

if( $_REQUEST['requisicao'] == 'mostraHistoricoWorkflow' ){
	echo mostraHistoricoWorkflowObraMI( $_REQUEST );
	exit();
}

if( $_REQUEST['requisicao'] == 'carregarListaProcesso' ){
	ob_clean();
	
	carregarListaProcessoObrasMI( $_REQUEST, 'T' );
		
	exit();
}

if( $_REQUEST['requisicao'] == 'carregarListaObras' ){
	ob_clean();
	carregarObrasProcesso($_POST);
	
	exit();
}

function formRegeraTermo($req){
	global $db;

	extract($_REQUEST);
	?>
	<center><b>Deseja regerar este termo?</b></center>
	<form method="post" name="formRegerar" id="formRegerar">
		<?=listaPreObrasTermo($_REQUEST) ?>
	</form>
<?php 
exit();
}

include APPRAIZ."includes/cabecalho.inc";
echo'<br>';

$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( 'Reformulação RMC - Aguardando Geração de Termo', '' );
?>
<form name="formularioAnalise" id="formularioAnalise" method="post" action="" >
	<input type="hidden" name="reqs" id="reqs" value="">
	<input type="hidden" name="tipoaba" id="tipoaba" value="<?php echo ( empty($_REQUEST['tipoaba']) ? 'pendente' : $_REQUEST['tipoaba']); ?>">
	
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="30%" >Número de Processo:</td>
			<td>
			<?php
				$filtro = simec_htmlentities( $_REQUEST['numeroprocesso'] );
				$numeroprocesso = $filtro;
				echo campo_texto( 'numeroprocesso', 'N', 'S', '', 50, 200, '#####.######/####-##', ''); 
			?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%">UF:</td>
			<td>
				<?php
					$estuf = $_POST['estuf'];
					$sql = "SELECT e.estuf as codigo, e.estdescricao as descricao 
							FROM territorios.estado e ORDER BY e.estdescricao ASC";
					$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
				?>
			</td>
		</tr>
		<tr id="tr_muncod">
			<td class="subtituloDireita" width="30%"> Município:</td>
			<td id="td_muncod">
				<?php
					if( $estuf ){
						$muncod = $_POST['muncod'];
						$sql = "SELECT muncod as codigo, mundescricao as descricao 
								FROM territorios.municipio 
								WHERE estuf = '$estuf'
								ORDER BY 2 ASC";
						$db->monta_combo( "muncod", $sql, 'S', 'Todos municípios', '', '' );
					}else{
						echo "Escolha uma UF.";
					}
				?>
			</td>
		</tr>		
		<tr>
			<td class="subtituloDireita" width="30%">WorkFlow:</td>
			<td>
				<?php
				$esdid = $_POST['esdid'];
				
				$sql = "SELECT distinct
							es.esdid as codigo,
							es.esddsc as descricao
						FROM obras.preobra p
							inner join workflow.documento d on d.docid = p.docid
						    inner join workflow.estadodocumento es on es.esdid = d.esdid
						WHERE d.tpdid = 37
						 	and (es.esddsc ilike '%RMC - %' or es.esdid = ".WF_TIPO_OBRA_APROVADA.")
						order by es.esddsc";
				$db->monta_combo( "esdid", $sql, 'S', 'Todos as Ações', '', '', '', '400' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Aguardando Geração de Termo:</td>
			<td>
				<input type="radio" value="S" id="geracao_termo" name="geracao_termo" <? if($_REQUEST["geracao_termo"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="geracao_termo" name="geracao_termo" <? if($_REQUEST["geracao_termo"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value="" id="geracao_termo" name="geracao_termo"<? if($_REQUEST["geracao_termo"] == "") { echo "checked"; } ?> /> Todos
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Processos com Obras com Contrapartida:</td>
			<td>
				<input type="radio" value="S" id="contrapartida" name="contrapartida" <? if($_REQUEST["contrapartida"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="contrapartida" name="contrapartida" <? if($_REQUEST["contrapartida"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value="" id="contrapartida" name="contrapartida"<? if($_REQUEST["contrapartida"] == "") { echo "checked"; } ?> /> Todos
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Processo com obras para cancelar:</td>
			<td>
				<input type="radio" value="S" id="obracancelar" name="obracancelar" <? if($_REQUEST["obracancelar"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="obracancelar" name="obracancelar" <? if($_REQUEST["obracancelar"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value="" id="obracancelar" name="obracancelar"<? if($_REQUEST["obracancelar"] == "") { echo "checked"; } ?> /> Todos
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="30%"> </td>
			<td>
				<input type="button" onclick="pesquisarTermo();" value="Pesquisar"/>
				<input type="button" onclick="window.location.href = window.location" value="Todos"/>
			</td>
		</tr>
	</table>
</form>

<div id="dialog_jquery" title="" style="display: none" >
	<div style="padding:5px;text-align:justify;" id="retornoDialog_jquery"></div>
</div>

<div id="debug"></div>
<?php
$arWere = array();
if( $_REQUEST['reqs'] == 'pesquisar' ){
	if( !empty($_REQUEST['muncod']) ){
		array_push($arWere, "po.muncod = '{$_REQUEST['muncod']}'");
	} else {
		if( !empty($_REQUEST['estuf']) ){
			array_push($arWere, "po.estuf = '{$_REQUEST['estuf']}'");
		}
	}
}

if( !empty($_REQUEST['numeroprocesso']) ){
	array_push($arWere, "po.pronumeroprocesso = '".str_replace(Array('-', '/', '.'), '', $_REQUEST['numeroprocesso'])."'");
}

if( !empty($_REQUEST['esdid']) ){
	$filtroObra = " and d.esdid = {$_REQUEST['esdid']} ";
}

if( $_REQUEST['contrapartida'] == 'S' ){
	$filtroObra = " and sr.sfocontrapartida > 0 ";
}

if( $_REQUEST['contrapartida'] == 'N' ){
	$filtroObra = " and sr.sfocontrapartida < 1 ";
}

if( $_REQUEST['obracancelar'] == 'S' ){
	$filtroObra = " and sr.sfoacaosolicitada = 'CO' ";
}

if( $_REQUEST['obracancelar'] == 'N' ){
	$filtroObra = " and sr.sfoacaosolicitada not in ('CO') ";
}

if( $_REQUEST['geracao_termo'] == 'S' ){
	$filtroSql = " and po.proid in (
									select proid from (
									    select proid, sum(totalDif) as totalDif
									    from ( 
									        select  pc.proid,
									            case when d.esdid = ".WF_TIPO_OBRA_APROVADA." then count(p.preid) else '0' end as totalAprov,
						                        case when d.esdid not in (".WF_TIPO_OBRA_APROVADA.") then count(p.preid) else '0' end as totalDif
									        from obras.preobra p
									            inner join par.processoobraspaccomposicao pc on pc.preid = p.preid and pc.pocstatus = 'A'
									            inner join workflow.documento d on d.docid = p.docid and p.prestatus = 'A'
									            inner join workflow.historicodocumento h on h.hstid = d.hstid and h.docid = d.docid
									            inner join par.solicitacaoreformulacaoobras sr on sr.preid = p.preid and sr.sfostatus = 'A'
									            left join par.termoobraspaccomposicao toc on toc.preid = p.preid
									            left join par.termocompromissopac tc on tc.terid = toc.terid and tc.terstatus = 'A'
									        where h.htddata > coalesce(tc.terdatainclusao, '2000-01-01')  
									        	$filtroObra
									        group by d.esdid, pc.proid
									    ) as foo
									    group by proid
									) as foo1
									where totalDif = 0
									)  ";
} else if( $_REQUEST['geracao_termo'] == 'N' ){
	$filtroSql = " and po.proid not in (
									select proid from (
									    select proid, sum(totalDif) as totalDif
									    from ( 
									        select  pc.proid,
									            case when d.esdid = ".WF_TIPO_OBRA_APROVADA." then count(p.preid) else '0' end as totalAprov,
						                        case when d.esdid not in (".WF_TIPO_OBRA_APROVADA.") then count(p.preid) else '0' end as totalDif
									        from obras.preobra p
									            inner join par.processoobraspaccomposicao pc on pc.preid = p.preid and pc.pocstatus = 'A'
									            inner join workflow.documento d on d.docid = p.docid and p.prestatus = 'A'
									            inner join workflow.historicodocumento h on h.hstid = d.hstid and h.docid = d.docid
									            inner join par.solicitacaoreformulacaoobras sr on sr.preid = p.preid and sr.sfostatus = 'A'
									            left join par.termoobraspaccomposicao toc on toc.preid = p.preid
									            left join par.termocompromissopac tc on tc.terid = toc.terid and tc.terstatus = 'A'
									        where h.htddata > coalesce(tc.terdatainclusao, '2000-01-01')  
									        	$filtroObra
									        group by d.esdid, pc.proid
									    ) as foo
									    group by proid
									) as foo1
									where totalDif = 0
									)  ";
} else {
	$filtroSql = "and po.proid in (select pc.proid FROM par.processoobraspaccomposicao pc
			                            inner join obras.preobra p on p.preid = pc.preid and pc.pocstatus = 'A' 
			                            inner join workflow.documento d on d.docid = p.docid
			                            inner join workflow.historicodocumento h on h.hstid = d.hstid
			                            inner join par.solicitacaoreformulacaoobras sr on sr.preid = p.preid and sr.sfostatus = 'A'
		                            where p.prestatus = 'A' $filtroObra)";
}

$imgMais = "<span style=\"font-size:16px; cursor:pointer;\" title=\"mais\" id=\"image_'||po.proid||'\" class=\"glyphicon glyphicon-download\" onclick=\"carregarListaObras(\''||po.pronumeroprocesso||'\', '||po.proid||', '||iu.inuid||', this);\"></span>
			<span style=\"font-size:16px; cursor:pointer;\" title=\"Redistribuir Empenho\" class=\"glyphicon glyphicon-registration-mark\" onclick=\"redistribuirEmpenho('||po.proid||', '||iu.inuid||', '||iu.itrid||', '||(CASE WHEN mun.muncod IS NOT NULL THEN mun.muncod ELSE '' end)||', \''||(CASE WHEN mun.muncod IS NOT NULL THEN mun.estuf ELSE est.estuf end)||'\');\"></span>
			<span style=\"font-size:28px; cursor:pointer; top: 1px; position: relative;\" title=\"Gerar Termo\" valign=\"top\">
				<img src=\"../imagens/gerar_termo.png\" style=\"cursor:pointer;\" width=\"18px\" onclick=\"gerarTermo('||po.proid||', '||iu.inuid||', '||(CASE WHEN mun.muncod IS NOT NULL THEN mun.muncod ELSE '' end)||', \''||(CASE WHEN mun.muncod IS NOT NULL THEN mun.estuf ELSE est.estuf end)||'\', \''||po.protipo||'\');\" border=\"0\">
			</span>";

$sql = "select
			'<center>
				$imgMais
			</center>' as mais,
            po.pronumeroprocesso,
            CASE WHEN mun.muncod IS NOT NULL THEN mun.mundescricao ELSE est.estdescricao end as municipio,
		    CASE WHEN mun.muncod IS NOT NULL THEN mun.estuf ELSE est.estuf end as estado,
            ter.termonumero,
			ter.vrltermo,
			con.valorprojeto,
            ((select sum(p.prevalorobra) from obras.preobra p
				inner join par.processoobraspaccomposicao pc on pc.preid = p.preid
			where p.prestatus = 'A' and pc.proid = po.proid and pc.pocstatus = 'A') - con.vlrcontrapartida) as vrlfnde,
			con.vlrcontrapartida,
			con.totalobra,
            con.totalobraaprov,
            con.totalobracancel,
            'PAC' as tipo,
            substring(po.pronumeroprocesso, 12, 4) as ano
		from par.processoobra po
		    inner join par.instrumentounidade iu on ( iu.muncod = po.muncod and iu.mun_estuf is not null ) OR  ( po.estuf = iu.estuf AND po.muncod is null )
            inner join(
			    	select sum(p.prevalorobra) as vrltermo, p.preano, 
			        	(select par.retornanumerotermopac(tc.proid)) as termonumero, tc.proid 
			        from par.termocompromissopac tc
			        	inner join par.termoobraspaccomposicao toc on toc.terid = tc.terid
			            inner join obras.preobra p on p.preid = toc.preid and p.prestatus = 'A'
			        where
			        	tc.terstatus = 'A'
			        	and p.ptoid IN (43, 42, 44, 45, 73, 74)
			        group by tc.proid, p.preano
			    ) ter on ter.proid = po.proid
            inner join(
			    	SELECT
					     sum(sr.sfocontrapartida) as vlrcontrapartida, 
					     count(p.preid) as totalobra, 
					     coalesce(oba.totalobraaprov,0) as totalobraaprov,
					     coalesce(obc.totalobracancel,0) as totalobracancel,
					     pc.proid, 
					     iu1.inuid, 
					     coalesce(sum(pir.valorprojeto), 0) as valorprojeto
					FROM par.processoobraspaccomposicao pc
					    inner join obras.preobra p on p.preid = pc.preid and pc.pocstatus = 'A' and p.prestatus = 'A'
					    inner join par.solicitacaoreformulacaoobras sr on sr.preid = p.preid and sr.sfostatus = 'A' --and sr.sfoacaosolicitada = 'RE'
					    inner join par.processoobra po on po.proid = pc.proid and po.prostatus = 'A' 
                        inner join par.instrumentounidade iu1 on ( iu1.muncod = po.muncod and iu1.mun_estuf is not null ) OR  ( po.estuf = iu1.estuf AND po.muncod is null )
                        left join(
					    	select sum(pr.pirvalor * pr.pirqtd) as valorprojeto, pic.ptoid, pr.estuf 
					    	from obras.preitencomposicao_regiao pr
					            inner join obras.preitenscomposicao pic on pic.itcid = pr.itcid
					        where pic.itcstatus = 'A' and pr.pirstatus = 'A'
					        group by pic.ptoid, pr.estuf
					    ) pir on pir.ptoid = sr.ptoidsolicitado and pir.estuf = p.estuf
					    left join(
                        	select count(p1.preid) as totalobraaprov, pc1.proid 
                            from obras.preobra p1 
                            	inner join par.processoobraspaccomposicao pc1 on pc1.preid = p1.preid and pc1.pocstatus = 'A'
                        		inner join workflow.documento d1 on d1.docid = p1.docid and d1.esdid = ".WF_TIPO_OBRA_APROVADA."
								inner join par.solicitacaoreformulacaoobras sr1 on sr1.preid = p1.preid and sr1.sfostatus = 'A' --and sr1.sfoacaosolicitada = 'RE'
                        	where p1.prestatus = 'A'
                            group by pc1.proid
                        ) oba on oba.proid = pc.proid
                        left join(
                        	select count(p1.preid) as totalobracancel, pc1.proid 
                            from obras.preobra p1 
                            	inner join par.processoobraspaccomposicao pc1 on pc1.preid = p1.preid and pc1.pocstatus = 'A'
								inner join par.solicitacaoreformulacaoobras sr1 on sr1.preid = p1.preid and sr1.sfostatus = 'A' and sr1.sfoacaosolicitada = 'CO'
                        	where p1.prestatus = 'A'
                            group by pc1.proid
                        ) obc on obc.proid = pc.proid
					WHERE
					    p.ptoid IN (43, 42, 44, 45, 73, 74)  
					GROUP BY pc.proid, iu1.inuid, oba.totalobraaprov, obc.totalobracancel
			    ) con on con.proid = po.proid and con.inuid = iu.inuid
    		left join territorios.municipio mun on mun.muncod = po.muncod
		    left join territorios.estado est on est.estuf = po.estuf
		where
		    po.prostatus = 'A'
		     $filtroSql						                        		
			".($arWere ? ' and '.implode(' and ', $arWere) : '')."";

$cabecalho = array('Abrir/Fechar', 'Processo', 'Município', 'UF', 'Nº Termo', 'Valor Pactuado', 'Novo Valor Proposto', 'Valor FNDE', 'Valor Contrapartida', 'QTD Obras no Processo', 'Qtd Obras Aprovadas', 'Qtd Obras a Cancelar', 'Tipo', 'Ano' );
$db->monta_lista($sql,$cabecalho,20,10,'N','center','N','formprocesso', '', '', '');

//$db->monta_lista_simples( $sql, $cabecalho,100,5,'N','100%','S', true, false, false, true);

?>

<div id="dialog" title="Recarregar Termo" style="display:none;"></div>

<script>
	function gerarTermo(proid, inuid, muncod, estuf, protipo){
		var estuf1 = estuf;
		if( muncod != '' ) estuf = '';
		jQuery.ajax({
			type: "POST",
			url: "par.php?modulo=principal/aguardandoGeracaoTermoRMC&acao=A",
			data: "requisicao=formRegeraTermo&tipo=simples&proid="+proid+"&muncod="+muncod+"&estuf="+estuf,
			async: false,
			success: function(msg){
				jQuery( "#dialog" ).html(msg);	
				
				jQuery( "#dialog" ).dialog({
					resizable: true,
					width:800,
					modal: true,
					buttons: {
						"Sim": function() {

							var arPreid = '';
							jQuery('#formRegerar').find('[name="preids[]"]').each(function(i, obj){
								if(arPreid==''){
									arPreid = jQuery(obj).val();
								}else{
									arPreid += ","+jQuery(obj).val();
								}
							});
							if(arPreid=='') {
								alert('Nenhuma obra carregada');
								jQuery( "#dialog" ).html('')
								return false;
							}
							
							window.open('par.php?modulo=principal/modeloTermoObra&acao=A&regerar=true&proid='+proid+'&arPreid='+arPreid+"&muncod="+muncod+'&estuf='+estuf1+'&tipoobra='+protipo, 
							        	'modelo', 
							       	 	"height=600,width=400,scrollbars=yes,top=0,left=0" );
				       	 	
							jQuery( this ).dialog( "close" );
						},
						"Não": function() {
							jQuery( this ).dialog( "close" );
						}
		
					}
				});
			}
		});
	}
	
	function redistribuirEmpenho(proid, inuid, itrid, muncod, estuf){
		location.href = "par.php?modulo=principal/projetos&acao=A&abas=reformularProjetoObras&proid="+proid+'&inuid='+inuid+'&itrid='+itrid+'&muncod='+muncod+'&estuf='+estuf;
	}
	
	function pesquisarTermo(){
		jQuery('[name="reqs"]').val('pesquisar');

		if( jQuery('[name="geracao_termo"]:checked').val() == 'S' ){
			jQuery('[name="esdid"]').val('');			
		}
		jQuery('[name="formularioAnalise"]').submit();
	}

	function abreObraAnalise( preid ){
		return window.open('par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&preid='+preid, 
		    	   			'ProInfancia', 
		    	   			"height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();	
	}
	
	function mostraHistoricoWorkflow( docid, proid ){

		jQuery.ajax({
			type: "POST",
			url: window.location.href,
			data: "requisicao=mostraHistoricoWorkflow&docid="+docid,
			async: false,
			success: function(msg){
				jQuery( "#dialog_workflow_"+proid ).attr('title', 'Histórico de Tramitações');
				jQuery( "#dialog_workflow_"+proid ).show();
				jQuery( "#mostraWorkflow_"+proid ).html(msg);
				jQuery( '#dialog_workflow_'+proid ).dialog({
						resizable: false,
						width: 800,
						modal: true,
						show: { effect: 'drop', direction: "up" },
						buttons: {
							'Fechar': function() {
								jQuery( this ).dialog( 'close' );
							}
						}
				});
			}
		});
	}
	
	function carregarListaObras(processo, proid, inuid, obj) {
		
		var linha = obj.parentNode.parentNode.parentNode;
		var tabela = obj.parentNode.parentNode.parentNode.parentNode;

		if(obj.title == 'mais') {  			
			obj.title='menos';
			jQuery('#image_'+proid).attr('class', 'glyphicon glyphicon-upload');
			var nlinha = tabela.insertRow(linha.rowIndex);
			var ncolbranco = nlinha.insertCell(0);
			ncolbranco.innerHTML = '&nbsp;';
			var ncol = nlinha.insertCell(1);
			ncol.colSpan=12;
			ncol.innerHTML="Carregando...";
			ncol.innerHTML="<div id='listaobraprocesso_" + processo + "' ></div>";
			
			carregarListaDivObras( processo, proid, inuid);
		} else {
			obj.title='mais';
			jQuery('#image_'+proid).attr('class', 'glyphicon glyphicon-download');
			var nlinha = tabela.deleteRow(linha.rowIndex);
		}
	}

	function carregarListaDivObras( processo, proid, inuid ){
		
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "requisicao=carregarListaObras&processo="+processo+'&proid='+proid+'&inuid='+inuid,
	   		async: false,
	   		success: function(msg){
	   	   		jQuery('#listaobraprocesso_'+processo).html(msg);
	   		}
		});
	}
	
	(function(jQuery) {

		/* Carrega Municpios de acordo com o estado
		*/
		jQuery('[name="estuf"]').change(function(){
			if( jQuery('[name="esfera"]').val() != 'E' ){
				if( jQuery(this).val() != '' ){
					jQuery('#aguardando').show();
					jQuery.ajax({
				   		type: "POST",
				   		url: window.location.href,
				   		data: "requisicao=carregaMunicipios&estuf="+jQuery(this).val(),
				   		async: false,
				   		success: function(resp){
				   			jQuery('#td_muncod').html(resp);
				   			jQuery('#aguardando').hide();
				   		}
				 	});
				}else{
					jQuery('#td_muncod').html('Escolha uma UF.');
				}
			}
		});
		  
	})(jQuery);
</script>
<?php 
function carregaMunicipios( $dados ){

	global $db;

	extract($dados);

	$sql = "SELECT muncod as codigo, mundescricao as descricao
	FROM territorios.municipio
	WHERE estuf = '$estuf'
	ORDER BY 2 ASC";

	$db->monta_combo( "muncod", $sql, 'S', 'Todos municípios', '', '' );
}

function carregarObrasProcesso( $post ){
	global $db;
	
	$sql = "select distinct
			    p.preid||'&nbsp;' as preid,
			    o.obrid||'&nbsp;' as obrid,
			    p.predescricao,
			    (select cast(prevalorobra as numeric(20,2)) from obras.preobra where prestatus = 'A' and preidpai = p.preid limit 1 ) as vlranterior,
    			cast(p.prevalorobra as numeric(20,2)) as vlrobraatual,
			    vs.saldo as vlrempenho,
			    sum(pgo.pobvalorpagamento) as vrlpagamento,
			    sr.sfocontrapartida,
				pt.ptodescricao,
			    es1.esddsc as situacaopreobra,
			    str.strdsc as situacaoobra2,
			    case when sr.sfoacaosolicitada = 'RE' then 'Reformular'
                	when sr.sfoacaosolicitada = 'CO' then 'Cancelar Obra'
                	when sr.sfoacaosolicitada = 'SA' then 'Sem alteração'
                else '' end as acaosolicitada
			from
			    par.processoobra po
			    inner join par.processoobraspaccomposicao pc on pc.proid = po.proid and pc.pocstatus = 'A'
			    inner join obras.preobra p on p.preid = pc.preid and p.prestatus = 'A'
			    inner join obras.pretipoobra pt on pt.ptoid = p.ptoid
			    
			    LEFT  JOIN obras2.obras o
	                INNER JOIN workflow.documento d ON d.docid = o.docid AND d.tpdid = 105 AND o.obrstatus = 'A' AND o.obridpai IS NULL
	                INNER JOIN workflow.estadodocumento es ON es.esdid = d.esdid
	                LEFT JOIN obras2.situacao_registro_documento srd     ON srd.esdid = d.esdid
					LEFT JOIN obras2.situacao_registro str               ON str.strid = srd.strid
	            ON o.preid = p.preid AND obridpai IS NULL AND o.obrstatus = 'A'			    
			    
			    inner join workflow.documento d1 on d1.docid = p.docid
			    inner join workflow.estadodocumento es1 on es1.esdid = d1.esdid
			   	inner join par.solicitacaoreformulacaoobras sr on sr.preid = p.preid and sr.sfostatus = 'A'
			    left join par.v_saldo_empenho_por_obra vs 
			    	inner join par.pagamentoobra pgo on pgo.preid = vs.preid
			    	inner join par.pagamento pg on pg.pagid = pgo.pagid and pg.pagstatus = 'A' and pg.pagsituacaopagamento not ilike '%cancelado%'
			    on vs.preid = p.preid
			where
			    po.proid = {$post['proid']}
				--and sr.sfoacaosolicitada = 'RE'
			group by p.preid, o.obrid, p.predescricao, p.prevalorobra,
			    vs.saldo, sr.sfocontrapartida, sr.sfocontrapartidainformada, es1.esddsc,
			    pt.ptodescricao, sr.sfoacaosolicitada, str.strdsc
			order by p.predescricao";
	
	$cabecalho = array('Preid', 'Obrid', 'Nome da Obra', 'Valor da Obra Anterior', 'Valor Novo da Obra Atual', 'Valor Empenho', 'Valor Pago', 'Valor Contrapartida', 'Tipo da Obra', 'Situação no PAR', 'Situação no Obras2', 'Ação Solicitada');
	$db->monta_lista_simples( $sql, $cabecalho,100,5,'S','100%','S', true, false, false, false);
}
?>