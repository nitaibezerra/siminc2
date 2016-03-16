<?php 

function montaAcoes($dados){
	
	global $db;
	
	$sql = "SELECT DISTINCT
				aed.aedid as codigo,
				aed.aeddscrealizar as descricao  
			FROM 	
				workflow.acaoestadodoc aed 
			INNER JOIN workflow.estadodocumentoperfil 	esp ON esp.aedid 	= aed.aedid
			INNER JOIN seguranca.perfilusuario 			pus ON pus.pflcod 	= esp.pflcod
			WHERE 
				aedstatus = 'A' 
				AND aed.esdidorigem = ".$dados['esdid']." 
				AND pus.usucpf = '{$_SESSION['usucpf']}'
			ORDER  BY 
				2";
	
	$db->monta_combo('aedid',$sql,'S','Selecione...','','','','','S', 'aedid');
}

function lista( $dados = null ){
	
	global $db;
	
	$where = array('tpgid = 1');
	
	extract($_POST);
	
	$stJoin = '';
	
	if ($_POST['esdid'])
		$where[] = "doc.esdid = {$esdid}";
	
	if ($_POST['aedid'])
		$where[] = "hst.aedid = {$aedid}";
	
	if ($_POST['inicio']){
		$arrDt = explode('/',$_POST['inicio']);
		$where[] = "to_char(grpdtcriacao,'YYYYMMDD')::integer >= ".$arrDt[2].$arrDt[1].$arrDt[0];
	}
	
	if ($_POST['fim']){
		$arrDt = explode('/',$_POST['fim']);
		$where[] = "to_char(grpdtcriacao,'YYYYMMDD')::integer <= ".$arrDt[2].$arrDt[1].$arrDt[0];
	}
	
	$sql = "SELECT DISTINCT
				grp.grpid as codigo,
				'Guia n° '||lpad(grp.grpid::text,6,'0') as descricao,
				usunome,
				to_char(grpdtcriacao,'DD/MM/YYYY') as data_criacao,
				es1.esddsc as origem,
				aed.aeddscrealizar as acao,
				es2.esddsc as destino
			FROM
				monitora.tramitalote_grupo grp
			INNER JOIN seguranca.usuario 				usu ON usu.usucpf = grp.usucpf
			INNER JOIN monitora.tramitalote_itensgrupo 	igr ON igr.grpid  = grp.grpid
			INNER JOIN workflow.historicodocumento 		hst ON hst.hstid  = igr.hstid
			INNER JOIN workflow.acaoestadodoc			aed ON aed.aedid  = hst.aedid
			INNER JOIN workflow.estadodocumento			es1 ON es1.esdid  = aed.esdidorigem
			INNER JOIN workflow.estadodocumento			es2 ON es2.esdid  = aed.esdiddestino
			INNER JOIN workflow.documento				doc ON doc.docid  = hst.docid
			". ( $where ? ' WHERE ' . implode(' AND ', $where) : '' ) . "
			ORDER BY 2";
			
	$checkTodos = '<center><input type="checkbox" class="todos" value=""/></center>';
	$arCabecalho = array("Ação", "Guia", "Guia gerada por", "Data de criação", "Estado Origem", "Ação Tramitada", "Estado Fim");
	
	unset($_REQUEST['req']);
	unset($_POST['req']);
	
	$linhas = $db->carregar($sql);
	
	$table = '<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem">
				<tr style="background-color: rgb(230,230,230)">';
	foreach($arCabecalho as $cabecalho){
		$table .= '<td valign="top" bgcolor="" align="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#c0c0c0\';" 
					    style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					    class="title"><strong>'.$cabecalho.'</strong>
					</td>';
	}
	
	$table .= '</tr>';
	if( count($linhas) > 0 && $linhas != '' ){
		foreach($linhas as $k=>$linha){
			
			if($k%2==0){
				$cor = '#F7F7F7';
			}else{
				$cor = '';
			}
			
			$table .= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">';
			
			$cod = '';
			$y = 0;
			foreach($linha as $y => $coluna){
				
				if( $y == 'codigo' ){ 
					$cod = $coluna;
					$table .= ' <td '.$estilo.' align="center" >
									<img border="0" 	title="Mostra Itens." 		src="../imagens/mais.gif"		style="cursor:pointer" 
										 class="abre" 	id="abre_'.$cod.'" 			name="'.$cod.'">&nbsp;
									<img border="0" 	title="Visualizar Guia." 	src="../imagens/icone_lupa.png"	style="cursor:pointer"
										 class="ver"	name="'.$cod.'">
									<img border="0" 	title="Gerar Planilha." 	src="../imagens/excel.gif"		style="cursor:pointer"
										 class="xls"	name="'.$cod.'">
								</td>';
				}else{
					if($arTipo[$y]=='numeric'){
						$estilo = 'align="right" style="color:blue"';
						$coluna = formata_valor($coluna);
					}else{
						$estilo = 'align="left"';
						$coluna = $coluna;
					}
					$table .= ' <td '.$estilo.' >
									'.$coluna.'
								</td>';
				}
				
				$y++;
			}
			
			$table .= '</tr>';
			
			$table .= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" 
							onmouseover="this.bgColor=\'#ffffcc\';" id="itens_'.$cod.'" style="display:none">
							<td id="itens_html_'.$cod.'" colspan="'.count($linha).'"></td></tr>';
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

function listaItens( $dados = null ){
	
	global $db;
	
	extract($_POST);
	
	if( !$grpid ){
		die();
	}
	
	$sql = "SELECT DISTINCT
				'Termo: '||tcp.tcpid as descricao
			FROM
				monitora.tramitalote_itensgrupo igr
			INNER JOIN workflow.historicodocumento	hst ON hst.hstid = igr.hstid
			INNER JOIN workflow.documento		doc ON doc.docid = hst.docid
			INNER JOIN monitora.termocooperacao	tcp ON tcp.docid = doc.docid 
			WHERE 
				grpid = $grpid";
			
	$checkTodos = '<center><input type="checkbox" class="todos" value=""/></center>';
	$arCabecalho = array("Termo");
	
	unset($_REQUEST['req']);
	unset($_POST['req']);
	
	$linhas = $db->carregar($sql);
	
	$table = '<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem">
				<tr style="background-color: rgb(230,230,230)">';
	foreach($arCabecalho as $cabecalho){
		$table .= '<td valign="top" bgcolor="" align="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#c0c0c0\';" 
					    style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					    class="title"><strong>'.$cabecalho.'</strong>
					</td>';
	}
	
	$table .= '</tr>';
	if( count($linhas) > 0 && $linhas != '' ){
		foreach($linhas as $k=>$linha){
			
			if($k%2==0){
				$cor = '#F7F7F7';
			}else{
				$cor = '';
			}
			
			$table .= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">';
			
			$cod = '';
			$y = 0;
			foreach($linha as $y => $coluna){
				
				if( $y == 'codigo' ){ 
					$cod = $coluna;
					$table .= ' <td '.$estilo.' align="center" >
									<img border="0" 	title="Mostra Itens." 		src="../imagens/mais.gif"		style="cursor:pointer" 
										 class="abre" 	id="abre_'.$cod.'" 			name="'.$cod.'">&nbsp;
									<img border="0" 	title="Visualizar Guia." 	src="../imagens/icone_lupa.png"	style="cursor:pointer">
									<img border="0" 	title="Gerar Planilha." 	src="../imagens/excel.gif"		style="cursor:pointer">
								</td>';
				}else{
					if($arTipo[$y]=='numeric'){
						$estilo = 'align="right" style="color:blue"';
						$coluna = formata_valor($coluna);
					}else{
						$estilo = 'align="left"';
						$coluna = $coluna;
					}
					$table .= ' <td '.$estilo.' >
									'.$coluna.'
								</td>';
				}
				
				$y++;
			}
			
			$table .= '</tr>';
			
			$table .= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" 
							onmouseover="this.bgColor=\'#ffffcc\';" id="itens_'.$cod.'" style="display:none"><td colspan="'.count($linha).'">Arg</td></tr>';
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

require_once '_funcoes_termoCooperacao.php';
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
montaAbaTermoCooperacaoListaUGs();
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

function aguarda(val){
	jQuery('#pesquisar').attr('disabled',val);
	jQuery('#limpar').attr('disabled',val);
	if(val){
		jQuery('#aguardando').show();
	}else{
		jQuery('#aguardando').hide();
	}
}

jQuery(document).ready(function() {

	
	jQuery('#esdid').click(function(){
		if(jQuery(this).val()){
			jQuery('.tr_acao').show();
		}else{
			jQuery('.tr_acao').hide();
			jQuery('#aedid').val(null);
		}
	});
	
	jQuery('#esdid').change(function(){

		var esdid = jQuery(this).val();

		aguarda(true);

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

	jQuery('.todos').live('click',function(){
		if( jQuery(this).attr('checked') ){
			jQuery('[name="docid[]"]').attr('checked',true);
		}else{
			jQuery('[name="docid[]"]').attr('checked',false);
		}
	});

	jQuery('.abre').live('click',function(){
		
		aguarda(true);
		
		if( jQuery('#itens_'+jQuery(this).attr('name')).css('display') == 'none' ){
			
			jQuery('#itens_'+jQuery(this).attr('name')).show();

			var grpid = jQuery(this).attr('name');
			
			if( jQuery('#itens_html_'+grpid).html() == '' ){
	
				jQuery('#req').val('listaItens');
				jQuery('#grpid').val( grpid );
				jQuery.ajax({
					type: "POST",
					url: window.location,
					data: jQuery('#formTramita').serialize(),
					async: false,
					success: function(msg){
						jQuery('#itens_html_'+grpid).html(msg);
					}
				});
			}

			jQuery(this).attr('src','../imagens/menos.gif');
			
		}else{
			
			jQuery('#itens_'+jQuery(this).attr('name')).hide();
			jQuery(this).attr('src','../imagens/mais.gif');
			
		}
		aguarda(false);
	});

	jQuery('.ver').live('click',function(){
		var grpid = jQuery(this).attr('name');
		var janela 	= window.open( 'elabrev.php?modulo=principal/termoCooperacao/guiaTramitacao&acao=A&tipo=ver&grpid='+grpid, 'guia', 'width=800,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
	});

	jQuery('.xls').live('click',function(){
		var grpid = jQuery(this).attr('name');
		var janela 	= window.open( 'elabrev.php?modulo=principal/termoCooperacao/guiaTramitacao&acao=A&tipo=xls&grpid='+grpid, 'guia', 'width=800,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
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

});

</script>
<style>
	.PopUphidden{display:none;width:500px;height:300px;position:absolute;z-index:0;top:50%;left:50%;margin-top:-150;margin-left:-250;border:solid 2px black;background-color:#FFFFFF;}
</style>
<center>
	<div id="aguardando" style="display:none; position: absolute; background-color: white; height:100%; width:100%; opacity:0.4; filter:alpha(opacity=40)" >
		<div style="margin-top:250px; align:center;">
			<img border="0" title="Aguardando" src="../imagens/carregando.gif">
			Carregando...
		</div>
	</div>
</center>
<form method="post" name="formTramita" id="formTramita" action="">
	<input type="hidden" id="req" 	name="req" value=""/>
	<input type="hidden" id="grpid" name="grpid" value=""/>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="40%">Situação da Solicitação</td>
			<td>
			<?php 
				$sql = "SELECT DISTINCT
							esdid as codigo, 
							esddsc as descricao  
						FROM 	
							workflow.estadodocumento esd
						INNER JOIN workflow.acaoestadodoc 	  		aed ON aed.esdidorigem 	= esd.esdid
						INNER JOIN workflow.estadodocumentoperfil 	esp ON esp.aedid 		= aed.aedid
						INNER JOIN seguranca.perfilusuario 			pus ON pus.pflcod 		= esp.pflcod
						WHERE  
							tpdid='".WF_TPDID_DESCENTRALIZACAO."'
							AND aed.aedstatus = 'A'
							AND esd.esdstatus = 'A' 
							AND pus.usucpf = '".$_SESSION['usucpf']."'
						ORDER  BY 
							esddsc";
				$db->monta_combo('esdid',$sql,'S','Selecione...','','','','','S', 'esdid');
				
				$displayAcao = ($_REQUEST['esdid']) ? 'table-row' : 'none';
			?>
			</td>
		</tr>
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
							INNER JOIN workflow.estadodocumentoperfil 	esp ON esp.aedid 	= aed.aedid
							INNER JOIN seguranca.perfilusuario 			pus ON pus.pflcod 	= esp.pflcod
							WHERE  
								esdidorigem = {$_REQUEST['esdid']}
								AND tpdid = '".WF_TPDID_DESCENTRALIZACAO."' 
								AND pus.usucpf = '".$_SESSION['usucpf']."'
							ORDER  BY 
								esddsc";
					
					$db->monta_combo('aedid',$sql,'S','Selecione...','','','','','S', 'aedid');
				}
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Periodo</td>
			<td>
				De <?=campo_data2('inicio', 'N', 'S', '', 'DD/MM/YYYY') ?> até <?=campo_data2('fim', 'N', 'S', '', 'DD/MM/YYYY') ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2" id="tdacao" >
				<input type="button" id="limpar" value="Limpar campos da pesquisa." />
				<input type="button" id="pesquisar" value="Pesquisar" />	
			</td>
		</tr>
	</table>
	<div id="lista" >
	</div>
</form>