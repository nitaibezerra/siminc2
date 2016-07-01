<?php
if($_GET['requisicao'] == "exibeGraficoMeta"){
	exibeGraficoMeta($_GET['micid']);
}
if($_GET['requisicao'] == "editaDataExecucao"){
	editaDataExecucao($_GET['sehid']);
}

include APPRAIZ . 'includes/classes/MedidorDesempenho.class.inc';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

if($_POST){
	extract($_POST);
}

$titulo = !$titulo ? str_replace("...","",$_SESSION['mnudsc']) : $titulo;
monta_titulo( $titulo, '&nbsp' );

//Perfis que pode editar a data de execução
$arrPerfil = pegaPerfilGeral();
if($db->testa_superuser() || in_array(PERFIL_DATA_EXECUCAO,$arrPerfil)){
	$permiteEditarData = true;
}

?>
<style>
.SubtituloTabela{background-color:#cccccc}
.negrito{font-weight:bold}
.bold{font-weight:bold}
.normal{font-weight:normal}
.center{text-align: center;}
.direita{text-align: right;}
.esquerda{text-align: left;}
.msg_erro{color:#990000}
.link{cursor: pointer}
.mini{width:12px;height:12px}
.sucess_msg{color: blue;}
.img_middle{vertical-align:middle}
.hidden{display:none}
.absolute{position:absolute;padding-top:5px;padding-bottom:5px;padding-left:5px;margin-top:5px;margin-left:50px;border:solid 1px black;background-color: #FFFFFF}
.fechar{position:relative;right:-5px;top:-26px;}
.img{background-color:#FFFFFF}
.red{color:#990000}
.mensagem{text-align: center; font-weight: bold;background-color:#cccccc;color: blue;}
.field_ano{width:100%;padding:5px;margin:3px}
.numero{color:blue}
.font_numero{font-size:14px;font-weight:bold}
</style>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script>
	jQuery.noConflict();
	function pesquisarAgenda()
	{
		/*if(!jQuery("[name='aggid']").val()){
			alert('Selecione a Agenda do Governo');
		}*/
		jQuery("[name='form_']").submit();
	}
	function wf_exibirHistorico( docid )
	{
		var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/historico.php' +
			'?modulo=principal/tramitacao' +
			'&acao=C' +
			'&docid=' + docid;
		window.open(
			url,
			'alterarEstado',
			'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
		);
	}
	
	function exibeGraficoMeta(micid){
		var url = 'estrategico.php?modulo=principal/gerenciamento_estrategico&acao=A&requisicao=exibeGraficoMeta&micid=' + micid;
		janela(url,600,400,'Gráfico da Meta');
	}
	
	function editaDataExecucao(sehid)
	{
		var url = 'estrategico.php?modulo=principal/gerenciamento_estrategico&acao=A&requisicao=editaDataExecucao&sehid=' + sehid;
		janela(url,600,400,'Data de Execução');
	}
	
</script>
<form method="post" name="form_" id="form_" >
	<input type="hidden" id="requisicao" name="requisicao" value="pesquisarAgenda"/>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="5" align="center" >
		<tr>
			<td align='right' class="SubTituloDireita" >Projeto:</td>
			<td>
				<?php
					$sql = "
						select
							a.atiid as codigo,
							a.atidescricao as descricao
						from pde.atividade a
						where
							 a.atiestrategico = true and
							 a.atiidpai is null and
							a.atistatus = 'A' and
							(a.atiid != " . PROJETO_PDE . " AND a.atiid != " . PROJETOENEM . " AND a.atiid != " . PROJETOSEB . " )
						order by
							a.atidescricao
						";
					$db->monta_combo("atiprojeto",$sql,"S","Selecione...","","","","200","N","","",$atiprojeto);
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" >Eixo:</td>
			<td>
				<?php
					$sql= "SELECT 
								exoid AS codigo,
								exodsc AS descricao 
							FROM 
								painel.eixo
							order by
								 descricao";
					$db->monta_combo("exoid",$sql,"S","Selecione...","","","","200","N","","",$exoid);
				?>
			</td>
		</tr>
		<tr>
			<td width="25%" class="SubtituloDireita" >Agenda:</td>
			<td>
			<?php
				$sql = "select
							aggid as codigo,
							aggdsc as descricao
						from
							painel.agendagoverno
						where
							aggstatus = 'A'
						order by
							aggdsc";
				$db->monta_combo("aggid",$sql,"S","Selecione...","","","","200","N","","",$aggid);
			?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Ação:</td>
			<td>
			<?php
				$sql= "select 
							acaid as codigo,
							acadsc as descricao
						from
							painel.acao
						where
							acastatus = 'A'
						order by
							acadsc";
				$db->monta_combo("acaid",$sql,"S","Selecione...","","","","200","N","","",$acaid);
			?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Ano de Referência:</td>
			<td>
				<?php 
					$ano = date("Y");
					$x=0;
					for($i=$ano;$i<($ano+10);$i++){
						$arrAnos[$x] = array("codigo" => $i, "descricao" => $i);
						$x++;
					}
				?>
				<?php $db->monta_combo("mnmexercicio",$arrAnos,"S","Selecione...","","","","","N","","",$mnmexercicio) ?>
			</td>
		</tr>
		<tr>
			<td width="25%" class="SubTituloDireita" >Período de Execução/Validação:</td>
			<td >
				<?php echo campo_data2("dmddatainiexecucao","N","S","","","","",$dmddatainiexecucao); ?>
				 até 
				<?php echo campo_data2("dmddatafimexecucao","N","S","","","","",$dmddatafimexecucao); ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloTabela center" colspan="2" >
				<input type="button" name="btn_pesquisar" value="Pesquisar" onclick="pesquisarAgenda()" />
				<input type="button" name="btn_vert_todos" value="Ver Todas" onclick="window.location.href=window.location" />
			</td>
		</tr>
	</table>
</form>
<?php 

if($_POST['acaid']){
	$arrWhere[] = "ind.acaid = {$_POST['acaid']}";
}
if($_POST['exoid']){
	$arrWhere[] = "ind.exoid = {$_POST['exoid']}";
}
if($_POST['mnmexercicio']){
	$arrWhere[] = "proj.atiexercicio = '{$_POST['mnmexercicio']}'";
}
if($_POST['dmddatainiexecucao']){
	$arrWhere[] = "dmidataexecucao >= '".formata_data_sql($_POST['dmddatainiexecucao'])."'";
}
if($_POST['dmddatafimexecucao']){
	$arrWhere[] = "dmidatavalidacao <= '".formata_data_sql($_POST['dmddatafimexecucao'])."'";
}
if($_POST['aggid']){
	$arrInner[] = " inner join pde.agendaatividade aga ON ati.atiid = aga.atiid and aga.aggid = {$_POST['aggid']}";
}
if($_POST['atiprojeto']){
	$arrWhere[] = "ati._atiprojeto = '{$_POST['atiprojeto']}'";
}

$sql = "select distinct
			/*ati.atiid,
			ati.atidescricao,
			ati.atiidpai,
			ati._atiprofundidade,
			ati._atiprojeto,
			ind.indid,
			ind.indnome,*/
			proj.atidescricao as projeto,
			proj.atiid as id_projeto
		from
			pde.atividade ati
		inner join
			pde.monitoraitemchecklist mic ON mic.atiid = ati.atiid
		inner join
			painel.indicador ind ON ind.indid = mic.indid
		inner join
			painel.metaindicador met ON met.indid = ind.indid
		inner join
			painel.detalhemetaindicador dmi ON met.metid = dmi.metid
		--inner join
			--painel.seriehistorica seh ON seh.dmiid = dmi.dmiid
		inner join
			pde.atividade proj ON proj.atiid = ati._atiprojeto
		".($arrInner ? implode("",$arrInner) : "")."
		where
			ati.atistatus = 'A'
		and
			mic.micstatus = 'A'
		and
			ind.indstatus = 'I'
		--and
			--ind.unmid != ".UNIDADEMEDICAO_BOLEANA."
		and
			met.metstatus = 'A'
		and
			dmi.dmistatus = 'A'
		and
			dmi.docid is not null
		".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
		order by
			proj.atidescricao";
$arrProjetos = $db->carregar($sql);
/*
$sql = "select distinct
			proj.atiid as id_projeto,
			proj.atidescricao as projeto,
			ati.atiid,
			ati._atinumero as num,
			ati.atidescricao
		from
			pde.atividade ati
		inner join
			pde.atividade proj ON ati._atiprojeto = proj.atiid
			".($arrInnerProjeto ? implode("",$arrInnerProjeto) : "")."
		where
			ati.atistatus = 'A'
		and
			ati._atiprofundidade = 1
			".($arrWhere ? implode(" and ",$arrWhere) : "")."
		and
			proj.atiid != " . PROJETO_PDE . " AND proj.atiid != " . PROJETOENEM . " AND proj.atiid != " . PROJETOSEB . "
		and
			proj.atiestrategico = true
		and
			proj.atiidpai is null 
		order by
			proj.atidescricao,ati._atinumero";

$arrProjetos = $db->carregar($sql);

if($arrProjetos){
	foreach($arrProjetos as $projeto){
		$arrProjeto[$projeto['atiid']] = $projeto;
	}
}
dbg($arrProjeto,1);*/
if($arrProjetos){
	?>
	<table style="margin-top:20px" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="5" align="center" >
		<tr>
			<td colspan="5" align="right" >
				<table border="0" cellpadding="1" >
					<tr>
						<td width="13px" ><div style="width:12px;height:12px;border:solid 1px black;background-color:#E95646"></div></td>
						<td><b>Crítico</b></td>
					</tr>
					<tr>
						<td width="13px" ><div style="width:12px;height:12px;border:solid 1px black;background-color:#FFC211"></div></td>
						<td><b>Atenção</b></td>
					</tr>
					<tr>
						<td width="13px" ><div style="width:12px;height:12px;border:solid 1px black;background-color:#80BC44"></div></td>
						<td><b>Estável</b></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="SubtituloTabela center bold" >Descrição</td>
			<td class="SubtituloTabela center bold" >Tipo</td>
			<td class="SubtituloTabela center bold" >Referência</td>
			<td class="SubtituloTabela center bold" >Executado/Validado</td>
			<td class="SubtituloTabela center bold" >Meta</td>
			<td class="SubtituloTabela center bold" >Medidor de Desempenho (%)</td>
		</tr>
	<?php foreach($arrProjetos as $projeto):  ?>
		<tr bgcolor="#DCDCDC" >
			<td colspan="6" >
				<p style="margin:5px;" >Projeto:<b><?php echo $projeto['projeto'] ?></b></p>
			</td>
		</tr>
		<?php $sql = "select distinct
							ati.atiid,
							ati.atiidpai,
							ati.atidescricao,
							ati._atinumero as num,
							ati._atiprofundidade,
							ati.atiordem
						from
							pde.atividade ati
						inner join
							pde.monitoraitemchecklist mic ON mic.atiid = ati.atiid
						inner join
							painel.indicador ind ON ind.indid = mic.indid
						inner join
							painel.metaindicador met ON met.indid = ind.indid
						inner join
							painel.detalhemetaindicador dmi ON met.metid = dmi.metid
						--inner join
							--painel.seriehistorica seh ON seh.dmiid = dmi.dmiid
						inner join
							pde.atividade proj ON proj.atiid = ati._atiprojeto
							".($arrInner ? implode("",$arrInner) : "")."
						where
							ati.atistatus = 'A'
						and
							mic.micstatus = 'A'
						and
							ind.indstatus = 'I'
						--and
							--ind.unmid != ".UNIDADEMEDICAO_BOLEANA."
						and
							met.metstatus = 'A'
						and
							dmi.dmistatus = 'A'
						and
							dmi.docid is not null
						and
							ati._atiprojeto = {$projeto['id_projeto']}
						".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
						order by
							ati._atinumero,ati.atiordem";
			$arrAtividades = $db->carregar($sql);
			$arrAtividades = !$arrAtividades ? array() : $arrAtividades;
		?>
		<?php $n=0;foreach($arrAtividades as $ati):  ?>
			<?php $n++;$cor = $n%2 == 1 ? "#ffffff" : "" ?>
			<tr bgcolor="<?php echo $cor ?>" >
				<td>
					<?php verificaAtividadePai($ati['atiid']) ?>
				</td>
				<td class="center" >-</td>
				<td class="center" >-</td>
				<td class="center" >-</td>
				<td class="center" >-</td>
				<td class="center" >-</td>
			</tr>
			<?php $sql = "select distinct
							met.metid,
							met.indid,
							met.metdesc as mnmdsc,
							ind.umeid,
							mnm.mtmid
						from
							pde.atividade ati
						inner join
							pde.monitoraitemchecklist mic ON mic.atiid = ati.atiid
						inner join
							painel.indicador ind ON ind.indid = mic.indid
						inner join
							painel.metaindicador met ON met.indid = ind.indid
						inner join
							pde.monitorameta mnm ON mnm.metid = met.metid
						inner join
							painel.detalhemetaindicador dmi ON met.metid = dmi.metid
						--inner join
							--painel.seriehistorica seh ON seh.dmiid = dmi.dmiid
						inner join
							pde.atividade proj ON proj.atiid = ati._atiprojeto
							".($arrInner ? implode("",$arrInner) : "")."
						where
							ati.atistatus = 'A'
						and
							mic.micstatus = 'A'
						and
							ind.indstatus = 'I'
						--and
							--ind.unmid != ".UNIDADEMEDICAO_BOLEANA."
						and
							met.metstatus = 'A'
						and
							dmi.dmistatus = 'A'
						and
							dmi.docid is not null
						and
							ati.atiid = {$ati['atiid']}
						".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
						order by
							met.metid";
			
			$arrMetas = $db->carregar($sql);
			$arrMetas = !$arrMetas ? array() : $arrMetas;
			
			foreach($arrMetas as $met):
				$n++;$cor = $n%2 == 1 ? "#ffffff" : "";
				$sql = "select 
							dmi.dmiid,
							dmi.dmiqtde as meta,
							dmi.dmivalor,
							dmi.docid,
							dpe.dpedsc as referencia,
							dmi.dmidataexecucao,
							dmi.dmidatavalidacao::date as dmidatavalidacao,
							dmi.dmdestavel as mnmqtdestavel,
							dmi.dmdcritico as mnmqtdcritico,
							mtidsc,
							mic.micid
						from 
							painel.detalhemetaindicador dmi
						inner join
							pde.monitorameta mnm ON mnm.metid = dmi.metid
						inner join
							pde.monitoraitemchecklist mic ON mic.micid = mnm.micid
						left join
							 pde.monitoratipoindicador mti ON mti.mtiid = mic.mtiid
						inner join
							painel.detalheperiodicidade dpe ON dpe.dpeid = dmi.dpeid
						inner join
							workflow.documento doc ON doc.docid = dmi.docid --and doc.esdid = ".WK_ESTADO_DOC_FINALIZADO."
						where 
							dmi.dmistatus = 'A' 
						and
							mnm.metid = {$met['metid']}
						order by
							--dpedatafim desc
							dmi.dmidatameta 
						limit 1";
				$arrDados = $db->pegaLinha($sql);
				if($arrDados['dmidataexecucao'] && $met['indid'] && $arrDados['dmiid']){
					$sql = "select 
								sehid,
								sehvalor as mnmvalor,
								sehqtde as mnmqtd,
								sehdtcoleta::date as mvddata 
							from
								painel.seriehistorica
							where
								indid = {$met['indid']}
							and
								dmiid = {$arrDados['dmiid']}
							--and
								--sehdtcoleta::date between '{$arrDados['dmddatainiexecucao']}' and '{$arrDados['dmddatafimexecucao']}'
							and
								sehstatus = 'A'
							order by
								sehid desc
							limit
								1";
					$arrExecucao = $db->pegaLinha($sql);
				}else{
					$arrExecucao = false;
				}
				
				//Verifica se a execução foi realizada em atrazo
				if($arrExecucao){
					if((int)str_replace("-","",$arrExecucao['mvddata']) > (int)str_replace("-","",$arrDados['dmidataexecucao'])){
						$atraso = " <span style=\"cursor:pointer\" title='Execução realizada com atraso' >(A)</span>";
					}else{
						$atraso = "";	
					}
				}else{
					$atraso = "";
				}
				
				/*
				$sql = "select
							COALESCE(mnmqtd,0) as mnmqtd,
							mnmvalor,
							mvddata
						from
							pde.monitoravalidacao
						where
							mnmid = {$met['mnmid']}
						and
							tpvid = 2";
				$arrExecucao = $db->pegaLinha($sql);
				*/
				
				if($arrExecucao['mnmqtd'] && $arrDados['meta']){
					
					$arrExecucao['mnmqtd'] = (float)$arrExecucao['mnmqtd'];
					$arrDados['meta'] = (float)$arrDados['meta'];
					$porcentagem = round((($arrExecucao['mnmqtd'] ? $arrExecucao['mnmqtd'] : 1)/($arrDados['meta'] ? $arrDados['meta'] : 1))*100,2);
				}else{
					$porcentagem = 0;
				}
				
				//Verifica se o executado é maior que a meta
				/*if($arrExecucao['mnmqtd'] > $arrDados['meta']){
					$porcentagem = 100;
				}*/
				
				
				if($ind['estid'] == 2){ //Menor melhor
					$img_indicador = "indicador-vermelha.png";
				}else{
					$img_indicador = "indicador-verde.png";
				}

				if($ind['estid'] == 2 && $arrDados['mnmqtdestavel'] && $arrDados['mnmqtdcritico']){ //Menor melhor
					$arrMedidor[0] = array("inicio" => 0, "fim" => $arrDados['mnmqtdestavel'], "cor" => "#80BC44", "bgcolor" => "#80BC44");
					$arrMedidor[1] = array("inicio" => $arrDados['mnmqtdestavel'], "fim" => $arrDados['mnmqtdcritico'], "cor" => "#FFFF00", "bgcolor" => "#FFC211");
					$arrMedidor[2] = array("inicio" => $arrDados['mnmqtdcritico'], "fim" => 100, "cor" => "#E95646", "bgcolor" => "#E95646");
				}elseif($ind['estid'] != 2 && $arrDados['mnmqtdestavel'] && $arrDados['mnmqtdcritico']){ //Maior Melhor
					$arrMedidor[0] = array("inicio" => 0, "fim" => $arrDados['mnmqtdcritico'], "cor" => "#E95646", "bgcolor" => "#E95646");
					$arrMedidor[1] = array("inicio" => $arrDados['mnmqtdcritico'], "fim" => $arrDados['mnmqtdestavel'], "cor" => "#FFC211", "bgcolor" => "#FFC211");
					$arrMedidor[2] = array("inicio" => $arrDados['mnmqtdestavel'], "fim" => 100, "cor" => "#80BC44", "bgcolor" => "#80BC44");
				}
				$valor = $porcentagem;
				
				/*
				Regras para cor do prazo
				Se for Processo
				1 - Vermelho se ñ foi feito e passou do prazo;
				2 - Amarelo faltando 5 dias do prazo e ñ foi feito;
				3 - Verde se ja foi feito dentro do prazo;
				4 - Verde com a data em vermelho se foi feito após o prazo.
				Se fro Produto ou Impacto
				1 - Manter as regars de cores da barra
				 */
				//$arrDados['dmidataexecucao'] = "2012-01-01";
				if($arrExecucao['mvddata']){
					$data_execucao = (int)str_replace("-","",$arrExecucao['mvddata']);
				}else{
					$data_execucao = false;
				}
				if($arrDados['dmidataexecucao']){
					$data_meta = (int)str_replace("-","",$arrDados['dmidataexecucao']);
				}else{
					$data_meta = false;
				}
				$img_atrazo = "";
				$cor_prazo = "";
				if(!$data_execucao && (int)date("Ymd") > $data_meta){
					$cor_prazo = "#E95646";
				}
				elseif(!$data_execucao && ($data_meta - (int)date("Ymd") <= 5) ){
					$cor_prazo = "#FFC211";
				}
				elseif($data_execucao && $data_execucao <= $data_meta){
					$cor_prazo = "#80BC44";
				}
				elseif($data_execucao && $data_meta <= $data_execucao){
					$cor_prazo = "#80BC44";
					$img_atrazo = "S";
				}
				//dbg(array($cor_prazo,$data_execucao,$data_meta,(int)date("Ymd"),$img_atrazo));
				?>
				<tr bgcolor="<?php echo $cor ?>" >
					<td>
						<img style="margin-left:<?php echo (($ati['_atiprofundidade']+1)*10) ?>px" src="../imagens/seta_retorno.gif" /> 
						<?php echo $met['mnmdsc']?> 
						<img class="img_middle link" onclick="wf_exibirHistorico(<?php echo $arrDados['docid']?>)" src="../imagens/fluxodoc.gif" />
						<?php if($met['unmid'] != UNIDADEMEDICAO_BOLEANA && $met['mtmid'] != 1): ?>
							<img class="img_middle link" onclick="exibeGraficoMeta(<?php echo $arrDados['micid']?>)" src="../imagens/graph.gif" />
						<?php endif; ?>
					</td>
					<td align="center">
						<?php echo $arrDados['mtidsc'] ? $arrDados['mtidsc'] : "-" ?>
					</td>
					<td class="center" >
						<?php echo $arrDados['referencia'] ? $arrDados['referencia'] : "-" ?>
					</td>
					<?php if($met['unmid'] == UNIDADEMEDICAO_BOLEANA || $met['mtmid'] == 1): ?>
						<?php 
							if($arrDados['referencia'] && $arrExecucao['mvddata']){
								$cor_td = "style=\"color:#FFFFFF;font-weight:bold;font-size:12px;border:solid 1px #000000;".($img_atrazo ? 'padding-left:50px;background-image:url(\'../imagens/atrasado.png\');background-repeat:no-repeat;' : false)."\" bgcolor='$cor_prazo' ";
							}else{
								$cor_td = "";
							}
						
						?>
						<td <?php if($permiteEditarData): ?> onclick="editaDataExecucao(<?php echo $arrExecucao['sehid']?>)" <?php endif; ?> <?php echo $cor_td ?> class="direita" ><?php echo $arrDados['referencia'] && $arrExecucao['mvddata'] ? ($arrExecucao['mvddata'] ? " em ".formata_data($arrExecucao['mvddata']) : "") : "-" ?></td>
						<td class="center" ></td>
						<td class="center" ></td>
					<?php else: ?>
						<td <?php if($permiteEditarData && $arrExecucao['sehid']): ?> onclick="editaDataExecucao(<?php echo $arrExecucao['sehid']?>)" <?php endif; ?> style="color:#FFFFFF;font-weight:bold;font-size:12px;<?php echo $img_atrazo ? "padding-left:50px;background-image:url('../imagens/atrasado.png');background-repeat:no-repeat;" : "" ?><?php echo $arrDados['referencia'] && $cor_prazo ? "border:solid 1px #000000" : "" ?>" bgcolor="<?php echo $cor_prazo ?>" class="direita numero" ><?php echo $arrExecucao['mnmqtd'] ? "<span class='font_numero' >".number_format($arrExecucao['mnmqtd'],0,'','.')."</span> <img src='../imagens/$img_indicador' /> (".str_replace(".",",",$porcentagem)."%) <br />".($arrExecucao['mvddata'] ? " em ".formata_data($arrExecucao['mvddata']) : "") : "<center>-<center>"?></td>
						<td class="direita numero" ><?php echo $arrDados['meta'] ? "<span class='font_numero' >".number_format($arrDados['meta'],0,'','.')."</span>".($arrDados['referencia'] ? "<br />".$arrDados['referencia'] : "") : "<center>-<center>" ?></td>
						<td class="center" >
						<?php
							echo "<center>";
							if($arrDados['referencia'] && $arrExecucao && $arrMedidor){
								$medidor = new MedidorDesempenho($arrMedidor,$valor);
							}else{
								echo "-";	
							}
							echo "</center>";
						?>
						</td>
					<?php endif; ?>
				</tr>
			<?php endforeach; ?>
		<?php endforeach; ?>
	<?php endforeach; ?>
	</table>
<?php }else{
	echo "<center style=\"margin-top:5px;color:#990000;font-weight:bold\" >Não exitem itens cadastrados.</center>";
}

?>