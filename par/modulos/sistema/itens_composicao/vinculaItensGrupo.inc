<?php

if( $_REQUEST['popup'] ){ 
	
	global $db;
	
	$sql = "SELECT
				gicnome,
				picdescricao || ' - Quantidade: ' || pgiqtd || '<br>' as item
			FROM 
				par.grupo_itemcomposicao  gi
			LEFT JOIN par.propostaitem_grupoitem  pgi ON pgi.gicid = gi.gicid
			LEFT JOIN par.propostaitemcomposicao pic ON pic.picid = pgi.picid AND pic.picstatus = 'A'
			WHERE
				gi.gicstatus = 'A' AND
				gi.gicid = ".$_REQUEST['popup']."
			ORDER BY
				picdescricao";
	
	$dados = $db->carregar($sql);
	
	?>
	<html>
	<head>
		<title>Detalhamento do Grupo de Itens de Composição</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css"/>
	</head>
	<body>
		<form id="formsigarp" action="<? echo $_SERVER['REQUEST_URI'] ?>" method="post">
			<input type="hidden" name="prpidHidden" id="prpidHidden" value="" />
			<input type="hidden" name="requisicao" id="requisicao" value="" />
			<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td width="150px" class="SubTituloDireita">Grupo:</td>
					<td><?=$dados[0]['gicnome'] ?></td>
				</tr>
				<tr>
					<td width="150px" class="SubTituloDireita">Itens:</td>
					<td id="subacao"><?php if(is_array($dados)){ foreach( $dados as $item ){ echo $item['item']; } } ?></td>
				</tr>
			</table>
		</form>
	</body>

<?php
	
	die();
}


function salvar(){
	
	global $db;
	
	extract($_POST);
	
	if( $gicid && (is_array($chk) || $picid != '') ){
		if( $picid != '' ){
			
			$vlr = $vlr ? $vlr : '0';
			
			$sql = "SELECT pgiid FROM par.propostaitem_grupoitem WHERE gicid = $gicid AND picid = $picid";
			
			$pgiid = $db->pegaUm($sql);
			
			if( $pgiid != '' ){
				if( $atualizar == 't' ){
					$sql = "UPDATE par.propostaitem_grupoitem SET pgiqtd = $vlr WHERE pgiid = $pgiid";
				}else{
					$sql = "DELETE FROM par.propostaitem_grupoitem WHERE gicid = $gicid AND picid = $picid;";
				}
			}else{
				$sql = "INSERT INTO par.propostaitem_grupoitem(picid, gicid, pgiqtd)
						 VALUES($picid, $gicid, $vlr);";
			}
		}
		if( $sql != '' ){
			$db->executar($sql);
			$db->commit();
		}
//		else{
//	//		$sql = "DELETE FROM par.propostaitem_grupoitem WHERE gicid = $gicid;";
//	//		$sql = "";
//			foreach( $chk as $picid => $value ){
//				$sql .= "INSERT INTO par.propostaitem_grupoitem(picid, gicid)
//						 VALUES($picid, $gicid);";
//			}
//			if( $sql != '' ){
//				$db->executar($sql);
//				$db->commit();
//			}
//		}
	}
}

function listaItens(){
	
	global $db;
	
	if( $_POST['gicid'] != '' ){
		$cabecalho = array("Ação","Quantidade","Nome","Detalhe");
	
		$where = Array("picstatus = 'A'");
		
		if( $_POST['picdescricao'] ){
			$descricao = utf8_decode($_POST['picdescricao']);
			$where[] = "UPPER(removeacento(picdescricao)) ilike '%'||UPPER(removeacento('{$descricao}'))||'%'";
		}
	
		$sql = "(
				SELECT DISTINCT
					'<input type=checkbox class=chk name=chk['|| pic.picid ||'] id='|| pic.picid ||' value=true '||
					CASE WHEN pgi.picid IS NOT NULL THEN ' checked=checked ' ELSE '' END ||'/>' as chk,
					'<input type=text class=\"vlr normal\"
							id='|| pic.picid ||' onblur=\"MouseBlur(this);this.value=mascaraglobal(\'[.###]\',this.value); \"; 
							onmouseout=MouseOut(this); onfocus=MouseClick(this);this.select(); 
							onmouseover=MouseOver(this); onkeyup=this.value=mascaraglobal(\'[.###]\',this.value); 
							value='|| coalesce(pgiqtd,0) ||' maxlength=11 size=12 name=vlr['|| pic.picid ||'] style=text-align >' as vlr,
					picdescricao as descricao,
					picdetalhe as detalhe
				FROM
					par.propostaitemcomposicao pic
				INNER JOIN par.propostaitemtiposubacao pis ON pis.picid = pic.picid
				INNER JOIN par.propostagrupotiposubacao pgs ON pgs.ptsid = pis.ptsid AND pgs.gicid = {$_POST['gicid']}
				LEFT JOIN par.propostaitem_grupoitem pgi ON pgi.picid = pic.picid AND pgi.gicid = {$_POST['gicid']}
				WHERE
					".implode(' AND ', $where)." AND pgi.picid IS NOT NULL
				)
				UNION ALL
				(
				SELECT DISTINCT
					'<input type=checkbox class=chk name=chk['|| pic.picid ||'] id='|| pic.picid ||' value=true '||
					CASE WHEN pgi.picid IS NOT NULL THEN ' checked=checked ' ELSE '' END ||'/>' as chk,
					'<input type=text class=\"vlr normal\"
							id='|| pic.picid ||' onblur=\"MouseBlur(this);this.value=mascaraglobal(\'[.###]\',this.value); \"; 
							onmouseout=MouseOut(this); onfocus=MouseClick(this);this.select(); 
							onmouseover=MouseOver(this); onkeyup=this.value=mascaraglobal(\'[.###]\',this.value); 
							value='|| coalesce(pgiqtd,0) ||' maxlength=11 size=12 name=vlr['|| pic.picid ||'] style=text-align >' as vlr,
					picdescricao as descricao,
					picdetalhe as detalhe
				FROM
					par.propostaitemcomposicao pic
				INNER JOIN par.propostaitemtiposubacao pis ON pis.picid = pic.picid
				INNER JOIN par.propostagrupotiposubacao pgs ON pgs.ptsid = pis.ptsid AND pgs.gicid = {$_POST['gicid']}
				LEFT JOIN par.propostaitem_grupoitem pgi ON pgi.picid = pic.picid AND pgi.gicid = {$_POST['gicid']}
				WHERE
					".implode(' AND ', $where)." AND pgi.picid IS NULL
				)";
//				ver($sql,d);	
		$dados = $db->carregar($sql);
		
		$porpagina = 100;
		$pages = 5;
		
		$qtd = count($dados);
		if( $qtd == 1 ){
			$qtd = $dados[0]['descricao'] != '' ? 1 : 0;
		}
		$paginacao = '<div class="div_paginacao_ajax" id="paginacao_ajax"> Páginas:';
		$num = $_POST['num'] > 0 ? $_POST['num'] : 1;
		$max = ($qtd/$porpagina);
		if( $qtd%$porpagina > 0 ){
			$max++;
		}
		for( $x=1; $x<=$max; $x++ ){
			
			if( $num == $x ){
				$classe = 'pagina_ativa';
			}else{
				$classe = 'pagina';
			}
			
			$de = 1+($porpagina*($x-1));
			$ate = ($porpagina*$x);
			
			$paginacao .= '<a class="'.$classe.'" title="De '.$de.' até '.$ate.'" id="'.$x.'">' .
					'		&nbsp;'.$x.'&nbsp;' .
					'	   </a>';
		}
		$paginacao .= '</div>';
		
		$de = 0+($porpagina*($num-1));
		$ate = ($porpagina*$num);
		
		$limit = "LIMIT $ate OFFSET $de";
		
		$sql = "SELECT * FROM ( $sql $limit ) as foo";
		
		$total = '<table id="total_ajax" align="center" width="95%" cellspacing="0" cellpadding="2" border="0" class="listagem">' .
				'	<tbody><tr bgcolor="#ffffff"><td><b>Total de Registros: '.$qtd.'</b></td><td></td></tr></tbody></table>';
        $db->monta_lista($sql, $cabecalho,$porpagina, 50, 'N', 'center', 'N', 'formitens', '', '', '', array('ordena' => false) ).$total.$paginacao;
	}
}

if( $_POST['req'] && $_REQUEST['numero'] == '' ){
	$_POST['req']();
	exit;
}


require_once APPRAIZ . 'includes/cabecalho.inc';

echo '<br/>';
monta_titulo( $titulo_modulo, '' );
echo '<br/>';
$db->cria_aba($abacod_tela,$url,$parametros);
?>
<style>
.pagina{
	background-color:#f5f5f5;
	color:#006699;
	BORDER-RIGHT: #a0a0a0 1px solid;
	text-decoration:none;
	cursor:pointer;
}
.pagina_ativa{
	background-color:#f5f5f5;
	color:#006699;
	BORDER-RIGHT: #a0a0a0 1px solid;
	font-weight:bold;
	cursor:pointer;
}
.div_paginacao_ajax{
	float:right;
	margin-right:45px;
}
</style>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

function listaItens( pagina ){
	
	$('#req').val('listaItens');
	$.ajax({
		type: "POST",
		url: window.location,
	   	data: $('#formFiltro').serialize()+'&num='+pagina,
	   	dataType: 'html',
	   	success: function(msg){
		   	$('#lista').html(msg);
			$('#lista').children('table').hide();
			$('#lista').children('div').hide();
			$('#total_ajax').show();
			$('#paginacao_ajax').show();
			$('input[type="text"][name^="vlr"]').each(function(){
				$(this).val(mascaraglobal('[.###]',$(this).val()));
			});
		}
	 });
}

function visualizagrupo(){
	if($('#gicid').val() == "" ){
		alert("Selecione o Grupo");
	} else {
		var gicid = $('#gicid').val();
		url = 'par.php?modulo=sistema/itens_composicao/vinculaItensGrupo&acao=A&popup='+gicid;
		window.open(url,'Detalhamento do Grupo de Itens de Composição',"height=600,width=600,scrollbars=yes,top=50,left=200" );
	}
}

$(document).ready(function(){
	
	$('.salvar').click(function(){
//		$('#req').val('salvar');
//		$.ajax({
//			type: "POST",
//			url: window.location,
//		   	data: $('#formFiltro').serialize()+'&'+$('#formitens').serialize(),
//		   	dataType: 'html',
//		   	success: function(msg){
//			}
//		 });
		window.location.href = window.location.href;
		alert('Itens vinculados.');
	});
	
	$('.chk').live('click',function(){
		$('#req').val('salvar');
		var picid 	= $(this).attr('id');
		var vlr 	= replaceAll($('[name="vlr['+picid+']"]').val(),'.','');
		$.ajax({
			type: "POST",
			url: window.location,
		   	data: $('#formFiltro').serialize()+'&picid='+picid+'&vlr='+vlr,
		   	dataType: 'html',
		   	success: function(msg){
			}
		 });
	});
	
	$('.vlr').live('blur',function(){
		$('#req').val('salvar');
		var picid 	= $(this).attr('id');
		if( $('[name="chk['+picid+']"]').attr('checked') ){
			var vlr 	= replaceAll($('[name="vlr['+picid+']"]').val(),'.','');
			$.ajax({
				type: "POST",
				url: window.location,
			   	data: $('#formFiltro').serialize()+'&atualizar=t&picid='+picid+'&vlr='+vlr,
			   	dataType: 'html',
			   	success: function(msg){
				}
			 });
		}
	});
	
	$('.pagina').live('click',function(){
		listaItens($(this).attr('id'));		
	});
	
	$('.pesquisar').live('click',function(){
		if( $('#gicid').val() != '' ){
			listaItens(1);
		}
	});
	
	$('#gicid').change(function(){
		listaItens(1);
	});
	
	$('#picdescricao').keyup(function(){
		if( $('#gicid').val() != '' ){
			listaItens(1);
		}
	});
	
	if( $('#gicid').val() != '' ){
		listaItens(1);
	}
});


</script>
<form method="post" name="formFiltro" id="formFiltro">
	<input type="hidden" id="req" name="req" value="" />
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" valign="bottom" width="30%">Grupo</td>
			<td>
				<?php 
					$gicid = $_REQUEST['gicid']; 
					$sql = "
                        SELECT
                            gicid as codigo,
                            gicnome as descricao
                        FROM
                            par.grupo_itemcomposicao
                        WHERE
                            gicstatus = 'A'
                        ORDER BY
                            descricao";
					$db->monta_combo('gicid',$sql,'S','Selecione...',$acao,$opc,'','200','S', 'gicid', $return = false, '', $title= null); 
				?> <img border="0" src="../imagens/icone_lupa.png" title="Visualizar grupo de itens" onclick="visualizagrupo()">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="bottom" width="30%">Nome do Item</td>
			<td>
				<?php 
				$picdescricao = $_REQUEST['picdescricao']; 
				echo campo_texto( 'picdescricao', 'S', 'S', '', 40, 200, '', '','','','','id="picdescricao"' ); 
				?>
			</td>
		</tr>
		<!--<tr>
			<td class="SubTituloDireita" valign="bottom"></td>
			<td class="SubTituloDireita" align="left" style="text-align:left">
	         	<input type="button" class="pesquisar" value="Pesquisar" />	
			</td>
		</tr>-->
	</table>
</form>
<div id="lista">
</div>
<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td class="SubTituloDireita" valign="bottom"></td>
		<td class="SubTituloDireita" align="left" style="text-align:left">
         	<input type="button" class="salvar" value="Gravar" />	
		</td>
	</tr>
</table> 