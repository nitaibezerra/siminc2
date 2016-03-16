<?php

if($_REQUEST['requisicao'] == 'verHistorico'){
	exibeHistoricoMetasIndicador($_POST);
	die;
}

if($_REQUEST['requisicaoAjax']){
	$_REQUEST['requisicaoAjax']();
	exit;
}

include_once APPRAIZ."www/painel/_constantes.php";
include_once APPRAIZ."www/painel/_funcoesdetalhamentoindicador.php";
include APPRAIZ . 'includes/classes/MedidorDesempenho.class.inc';

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

if($_POST){
	extract($_POST);
}
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$titulo = !$titulo ? str_replace("...","",$_SESSION['mnudsc']) : $titulo;
monta_titulo( $titulo, '&nbsp' );

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
.painel{border:solid 1px black;background-color:#FFFFFF;margin:3px;padding:3px}
.bloco{margin:3px;padding:3px;width:100%;height:100%;border:solid 1px black;}
.titulo_bloco{font-weight:bold;font-size:16px;padding:0px;margin:0px 0px 2px 0px}
.controle_bloco{background-color:#F5F5F5;border-top:solid 1px black;margin-top:5px;}
</style>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script>
	$(function(){
		$('.historicoMetas').click(function(){
			
			id = this.id;			
			arDados = this.id.split('_');
			indid = arDados[1];
			metid = arDados[2];
			visao = $('#tr_'+indid+'_'+metid).css('display');
			
			params = {
				'indid' 		: indid,
				'metid'			: metid,
				'requisicao' 	: 'verHistorico'
				};

			$.ajax({
				url		: 'estrategico.php?modulo=principal/painel_estrategico&acao=A',
				type	: 'post',
				data	: params,
				success	: function(e){
							
					if(visao == 'none'){
						$('#tr_'+indid+'_'+metid).show().children().html(e);
						$('#img_'+indid+'_'+metid).attr('src', '../imagens/menos.gif');
					}else{
						$('#tr_'+indid+'_'+metid).hide().children().html('');
						$('#img_'+indid+'_'+metid).attr('src', '../imagens/mais.gif');
					}
					
				}
			});
		});
	});
</script>
<script>
	function pesquisarPainel(){
		jQuery("#form_projeto").submit();
	}
	function aumentaBloco(obj)
	{
	   jQuery("[class='bloco']").each(function() { 
			jQuery(this).attr("style","width:10%;height:10%;display:none");
		});
		var bloco = jQuery(obj).parent().parent();
		jQuery(bloco).attr("style","width:99%;height:98%;")
	}
	function diminuiBloco(obj)
	{
	   jQuery("[class='bloco']").each(function() { 
			jQuery(this).attr("style","");
		});
	}
	function exibeGraficoMeta(micid)
	{
		jQuery.ajax({
		   type: "POST",
		   url: window.location,
		   data: "requisicaoAjax=exibeListaGraficoMeta&micid=" + micid,
		   success: function(msg){
		   		jQuery("#bloco_lista_grafico").html(msg);
		   }
		 });
		
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
	function exibeGraficoMeta(indid)
	{
		window.open('/painel/painel.php?modulo=principal/pc_indicador&acao=A&indid='+indid, '_blank');
	}
	function exibeMapaIndicador(indid)
	{
		var url = 'estrategico.php?modulo=principal/painel_estrategico&acao=A&requisicao=exibeMapaIndicador&indid=' + indid;
		janela(url,1024,800,'Mapa do Indicador');
	}
	function editaDataExecucao(sehid)
	{
		var url = 'estrategico.php?modulo=principal/painel_estrategico&acao=A&requisicao=editaDataExecucao&sehid=' + sehid;
		janela(url,600,400,'Data de Execução');
	}
	function exibeFilhosIndicador(indid)
	{
		var url = 'estrategico.php?modulo=principal/painel_estrategico&acao=A&requisicao=exibeFilhosIndicador&indid=' + indid;
		janela(url,600,400,'Indicadores');
	}
	function exibeHistoricoMetas(indid)
	{
		var url = 'estrategico.php?modulo=principal/painel_estrategico&acao=A&requisicao=exibeHistoricoMetas&indid=' + indid;
		janela(url,600,400,'Histórico Metas');
	}
	function filtraAtividade(projeto)
	{
		jQuery("#atiidraiz").val("");
		jQuery("#form_projeto").submit();
	}
	function exibeHistoricoMetasIndicador(indid, metid)
	{
		var url = 'estrategico.php?modulo=principal/painel_estrategico&acao=A&requisicao=exibeHistoricoMetasIndicador&indid=' + indid + '&metid=' + metid;
		janela(url,600,400,'Histórico Metas Indicador');
	}
</script>
<?php
if($_REQUEST['atiprojeto']){
	$sql = "
			select
					a.atiid as atiprojeto,
					a.atidescricao as nome_projeto
				from pde.atividade a
				where
					 a.atiestrategico = true and
					 a.atiidpai is null and
					a.atistatus = 'A' and
					a.atiid = {$_REQUEST['atiprojeto']}
				order by
					a.atidescricao
				";
	$arrDados = $db->pegaLinha($sql);
}
if($arrDados){
	extract($arrDados);	
}

$arrPerfils = pegaPerfilGeral();
if(in_array(PERFIL_VALIDADOR,$arrPerfils) || in_array(PERFIL_EXECUTOR,$arrPerfils) && !$db->testa_superuser()){
	$sql = "select distinct
					ati.atiid,
					ati._atiprojeto as projeto
				from
					pde.monitoraetapascontrole eta
				inner join
					pde.monitorametaentidade me ON me.mnmid = eta.mnmid and eta.tpvid = me.tpvid
				inner join
					entidade.entidade ent ON ent.entid = me.entid
				inner join
					pde.monitorameta mnm ON mnm.mnmid = eta.mnmid
				inner join
					pde.monitoraitemchecklist mic ON mic.micid = mnm.micid
				inner join
					pde.atividade ati ON ati.atiid = mic.atiid
				where
					ent.entnumcpfcnpj = '{$_SESSION['usucpf']}'
				and
					eta.tpvid in (1,2)";
	$arrAtiid = $db->carregar($sql);
	if($arrAtiid){
		foreach($arrAtiid as $a){
			$arrFiltroAtiid[] = $a['atiid'];
			$arrFiltroProjeto[] = $a['projeto'];
		}
	}
}
?>
<form name="form_projeto" id="form_projeto" method="post" action="" >
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
								".($arrFiltroProjeto ? " and a.atiid in (".implode(",",$arrFiltroProjeto).") " : "")."
							order by
								a.atidescricao
							";
						$db->monta_combo("atiprojeto",$sql,"S","Selecione...","filtraAtividade","","","200","N","","",$atiprojeto);
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">
					Atividade
				</td>
				<td id="combo_atiidraiz" >
					<select id="atiidraiz" name="atiidraiz" class="CampoEstilo" style="width: 250px;">
						<option value="">Selecione...</option>
						<?php
						if($atiprojeto){
							$sql = "
								select
									a.atiid,
									a.atidescricao,
									a._atiprofundidade as profundidade,
									a._atinumero as numero
								from pde.atividade a
								where
									a.atistatus = 'A'
									and a._atiprofundidade < 3
									and a._atiprojeto = " . $atiprojeto . "
									".($arrFiltroProjeto ? " and a._atiprojeto in (".implode(",",$arrFiltroProjeto).") " : "")." 
									".($arrFiltroAtiid ? " and a.atiid in (".implode(",",$arrFiltroAtiid).") " : "")." 
								order by
									a._atiordem
							";
							$lista = $db->carregar( $sql );
						}
						$lista = $lista ? $lista : array();
						
						?>
						<?php foreach ( $lista as $item ) : ?>
							<option value="<?=$item['numero']?>" <?= $item['numero'] == $_REQUEST["atiidraiz"] ? 'selected="selected"' : '' ?>>
								<?= str_repeat( '&nbsp;', $item['profundidade'] * 5 ) ?>
								<?= $item['numero'] ?>
								<?= $item['atidescricao'] ?>
							</option>
						<?php endforeach; ?>
					</select>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita" >Tipo:</td>
				<td>
					<?php
						$sql = "
							select
								mtiid as codigo,
								mtidsc as descricao
							from 
								pde.monitoratipoindicador
							where
								mtistatus = 'A'
							order by
								mtidsc
							";
						$db->monta_combo("mtiid",$sql,"S","Selecione...","","","","200","N","","",$mtiid);
					?>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita" >Etapa:</td>
				<td>
					<select name=micetapa id=micetapa>
						<option value=''>Selecione ...</option>
						<option value='I' <?=$micetapa == 'I'? 'SELECTED' : ''?>>Implantação</option>
						<option value='E' <?=$micetapa == 'E'? 'SELECTED' : ''?>>Execução</option>
					</select>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita" >Críticos:</td>
				<td>
					<input type = checkbox name=micestrategico id=micestrategico  <?=$_POST['micestrategico']?'checked':'' ?> >
				</td>
			</tr>

			<tr>
				<td class="SubTituloDireita" ></td>
				<td class="SubTituloEsquerda" ><input type="button" onclick="pesquisarPainel()" name="btn_pesquisar" value="Pesquisar"  /></td>
			</tr>
	</table>
</form>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="5" align="center" >
	<tr>
		<td align="center" >
			<div id="principal_painel" class="painel principal" >
				<div class="bloco">
					<p class="titulo_bloco"><?php echo $nome_projeto ?></p>
					<div class="conteudo_bloco">
						<script>
						$(document).ready(function(){
							$('.sobe').click(function(){
								// Carrega parametros da linha
								var id = $(this).attr('id');
								id = id.split('_');
								var ordem = id[1];
								var indid = id[2];
								// Carrega parametros da linha acima
								var ordemCima = parseInt(ordem)-1;
								var idCima = $('[id*=tr_'+ordemCima+'_]').attr('id'); 
								idCima = idCima.split('_');
								var indidCima = idCima[2];
								var corCima = $('[id*=tr_'+ordemCima+'_]').attr('bgcolor'); 
								// Sobe a Linha
								$('[id*=tr_'+ordemCima+'_]').before($('[id*=tr_'+ordem+']'));
								// Troca a cor das linhas
								$('[id*=tr_'+ordemCima+'_]').attr('bgcolor',$('[id*=tr_'+ordem+']').attr('bgcolor'));
								$('[id*=tr_'+ordem+'_]').attr('bgcolor',corCima);
								// Atualiza setas p  cima
								if( ordemCima == 1 ){
									$('#sobe_'+ordemCima+'_'+indidCima).show();
									$('#sobe_'+ordem+'_'+indid).hide();
								}
								// Atualiza ids
								$('#tr_'+ordemCima+'_'+indidCima).attr('id','tr_'+ordem+'_'+indidCima);
								$('#tr_'+ordem+'_'+indid).attr('id','tr_'+ordemCima+'_'+indid);
								$('#sobe_'+ordemCima+'_'+indidCima).attr('id','sobe_'+ordem+'_'+indidCima);
								$('#sobe_'+ordem+'_'+indid).attr('id','sobe_'+ordemCima+'_'+indid);
								$('#desce_'+ordemCima+'_'+indidCima).attr('id','desce_'+ordem+'_'+indidCima);
								$('#desce_'+ordem+'_'+indid).attr('id','desce_'+ordemCima+'_'+indid);
								// Atualiza setas p baixo
								$('.desce').each(function(){
									var id = $(this).attr('id');
									id = id.split('_');
									var ordem = id[1];
									var indid = id[2];
									// Carrega parametros da linha acima
									var ordemBaixo = parseInt(ordem)+1;
									if(!$('[id*=desce_'+ordemBaixo+'_]').attr('id')){
										$(this).hide();
									}else{
										$(this).show();
									}
								});
								// Atualiza ordem no banco
								jQuery.ajax({
								   type: "POST",
								   url: window.location,
								   data: "requisicao=atualizaOrdem&indid="+indid+"&ordem="+ordemCima,
								});
								jQuery.ajax({
								   type: "POST",
								   url: window.location,
								   data: "requisicao=atualizaOrdem&indid="+indidCima+"&ordem="+ordem,
								});
							});
							
							$('.desce').click(function(){
								// Carrega parametros da linha
								var id = $(this).attr('id');
								id = id.split('_');
								var ordem = id[1];
								var indid = id[2];
								// Carrega parametros da linha acima
								var ordemBaixo = parseInt(ordem)+1;
								var idBaixo = $('[id*=tr_'+ordemBaixo+'_]').attr('id'); 
								idBaixo = idBaixo.split('_');
								var indidBaixo = idBaixo[2];
								var corBaixo = $('[id*=tr_'+ordemBaixo+'_]').attr('bgcolor'); 
								// Desce a Linha
								$('[id*=tr_'+ordemBaixo+'_]').after($('[id*=tr_'+ordem+']'));
								// Atualiza setas primeira linha
								if( ordem == 1 ){
									$('#sobe_'+ordemBaixo+'_'+indidBaixo).hide();
									$('#sobe_'+ordem+'_'+indid).show();
								}
								// Troca a cor das linhas
								$('[id*=tr_'+ordemBaixo+'_]').attr('bgcolor',$('[id*=tr_'+ordem+']').attr('bgcolor'));
								$('[id*=tr_'+ordem+'_]').attr('bgcolor',corBaixo);
								// Atualiza ids
								$('#tr_'+ordemBaixo+'_'+indidBaixo).attr('id','tr_'+ordem+'_'+indidBaixo);
								$('#tr_'+ordem+'_'+indid).attr('id','tr_'+ordemBaixo+'_'+indid);
								$('#desce_'+ordemBaixo+'_'+indidBaixo).attr('id','desce_'+ordem+'_'+indidBaixo);
								$('#desce_'+ordem+'_'+indid).attr('id','desce_'+ordemBaixo+'_'+indid);
								$('#sobe_'+ordemBaixo+'_'+indidBaixo).attr('id','sobe_'+ordem+'_'+indidBaixo);
								$('#sobe_'+ordem+'_'+indid).attr('id','sobe_'+ordemBaixo+'_'+indid);
								// Atualiza setas
								$('.desce').each(function(){
									var id = $(this).attr('id');
									id = id.split('_');
									var ordem = id[1];
									var indid = id[2];
									// Carrega parametros da linha acima
									var ordemBaixo = parseInt(ordem)+1;
									if(!$('[id*=desce_'+ordemBaixo+'_]').attr('id')){
										$(this).hide();
									}else{
										$(this).show();
									}
								});
								// Atualiza ordem no banco
								jQuery.ajax({
								   type: "POST",
								   url: window.location,
								   data: "requisicao=atualizaOrdem&indid="+indid+"&ordem="+ordemBaixo,
								 });
								jQuery.ajax({
								   type: "POST",
								   url: window.location,
								   data: "requisicao=atualizaOrdem&indid="+indidBaixo+"&ordem="+ordem,
								 });
							});

							$('.desce').each(function(){
								var id = $(this).attr('id');
								id = id.split('_');
								var ordem = id[1];
								var indid = id[2];
								// Carrega parametros da linha acima
								var ordemBaixo = parseInt(ordem)+1;
								if(!$('[id*=desce_'+ordemBaixo+'_]').attr('id')){
									$(this).hide();
								}else{
									$(this).show();
								}
							});
						});
						</script>
						<table class="tabela" width="100%" cellspacing="1" cellpadding="5" align="center">
							<tr bgcolor="#c5c5c5" >
								<td align="center" ><b>Descrição</b></td>
								<td align="center" ><b>Executor</b></td>
								<td align="center" ><b>Tipo</b></td>
								<td align="center" ><b>Executado</b></td>
								<!-- 
								<td align="center" ><b>Meta Projetada</b></td>
								 -->
								<td align="center" ><b>Meta</b></td>
								<td align="center" ><b>Desempenho</b></td>
							</tr>
							<?php //listaIndicadoresPainelMonitoramento(null,0,0,1,false,"#DCDCDC") ?>
							<?php $_REQUEST['atiprojeto'] ? listaIndicadoresPainelMonitoramento() : "" ?>
						</table>
					</div>
				</div>
			</div>
		</td>
	</tr>
</table>