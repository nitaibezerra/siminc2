<?php

header("Content-Type: text/html; charset=ISO-8859-1",true);

function alterar(){
	
	global $db;
	
	extract($_POST);
	
	$sql = "SELECT
				gicid,
				gicnome,
				gicdescricao
			FROM
				par.grupo_itemcomposicao
			WHERE
				gicid = $gicid";
				
	$dados = $db->pegaLinha($sql);
	$dados['gicnome'] = utf8_encode($dados['gicnome']);
	$dados['gicdescricao'] = utf8_encode($dados['gicdescricao']);
	
	$sql = "SELECT
				p.ptsid as cod,
				p.ptsdescricao as descricao
			FROM
				par.propostagrupotiposubacao n
			INNER JOIN par.propostatiposubacao p ON p.ptsid = n.ptsid
			WHERE
				gicid = $gicid";
	
	$dados['ptsid'] = $db->carregar($sql);
	
	if( is_array($dados['ptsid']) ){
		foreach($dados['ptsid'] as $key => $ptsid){
			$dados['ptsid'][$key]['descricao'] = utf8_encode($dados['ptsid'][$key]['descricao']);
		}
	}
	
	echo simec_json_encode($dados);
}

function excluir(){
	
	global $db;
	
	extract($_POST);
	
	if( $gicid != '' ){
		$sql = "UPDATE par.grupo_itemcomposicao SET
					gicstatus = 'I'
				WHERE
					gicid = $gicid;";
					
		$db->executar($sql);
		$db->commit();
		
	}
	
}

function salvar(){
	
	global $db;
	
	extract($_POST);
	
	if( $gicid != '' ){
		$sql = "UPDATE par.grupo_itemcomposicao SET
					gicnome = '$gicnome',
					gicdescricao = '$gicdescricao'
				WHERE
					gicid = $gicid;";
		$db->executar($sql);
		$db->commit();
	}else{
		$sql = "INSERT INTO par.grupo_itemcomposicao(gicnome, gicdescricao, usucpf)
				VALUES('$gicnome', '$gicdescricao', '{$_SESSION['usucpf']}')
				RETURNING gicid;";
		$gicid = $db->pegaUm($sql);
	}
	
	$sql = "DELETE FROM par.propostagrupotiposubacao WHERE gicid = $gicid;";
	
	if( is_array($ptsid) ){
		foreach( $ptsid as $id ){
			$sql .= "INSERT INTO par.propostagrupotiposubacao(ptsid, gicid)
					 VALUES($id, $gicid);";
		}
	}
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Operação realizada com sucesso!');
			window.location.href = window.location.href;
		  </script>";
}

function listaGrupos(){
	
	global $db;
	
	$cabecalho = array("Ação","Nome","Status","Data de Inclusão");

	$sql = "SELECT DISTINCT
				'<center><img src=../imagens/alterar.gif title=Alterar style=cursor:pointer; class=\"alterar\" id=\"'||gic.gicid||'\"> &nbsp;'
				|| CASE WHEN pgc.picid IS NULL THEN
					'<img src=../imagens/excluir.gif title=Inativar style=cursor:pointer; class=\"excluir\" id=\"'||gic.gicid||'\">'
					ELSE ''
				END||'</center>' as acao,
				gicnome,
				gicdescricao as descricao,
				to_char(gicdatainclusao,'DD/MM/YYYY') as data_inclusao
			FROM
				par.grupo_itemcomposicao gic
			LEFT JOIN par.propostaitem_grupoitem pgc ON pgc.gicid = gic.gicid
			WHERE
				gicstatus = 'A'";
	
	return $db->monta_lista($sql,$cabecalho,100,5,'N','center','N','formprocesso');
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	exit;
}

require_once APPRAIZ . 'includes/cabecalho.inc';

echo '<br/>';
monta_titulo( $titulo_modulo, '' );
echo '<br/>';
$db->cria_aba($abacod_tela,$url,$parametros);
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

function listaGrupos(){
	$.ajax({
		type: "POST",
		url: window.location,
	   	data: "req=listaGrupos",
	   	dataType: 'html',
	   	success: function(msg){
		   	$('#lista').html(msg);
		}
	 });
}

$(document).ready(function(){
	
	listaGrupos();
	
	$('.salvar').click(function(){
		$('#ptsid').children().attr('selected',true)
		$('#req').val('salvar');
		$('#formGrupo').submit();
	});
	
	$('.alterar').live('click',function(){
		$.ajax({
			type: "POST",
			url: window.location,
		   	data: "req=alterar&gicid="+$(this).attr('id'),
		   	dataType: 'json',
		   	success: function(msg){
				$('#gicid').val(msg['gicid']);
				$('#gicnome').val(msg['gicnome']);
				$('#gicdescricao').val(msg['gicdescricao']);
				if( msg['ptsid'].length > 0 ){
					var html = '';
					for( var x=0; x < msg['ptsid'].length; x++ ){
						html = html+'<option value="'+msg['ptsid'][x]['cod']+'">'+msg['ptsid'][x]['descricao']+'</option>';
					}
					$('#ptsid').html(html);
				}else{
					$('#ptsid').html('<option value="">Duplo clique para selecionar da lista</option>');
				}
			}
		 });
		 $('#limpar').show();
	});
	
	$('.excluir').live('click',function(){
		$.ajax({
			type: "POST",
			url: window.location,
		   	data: "req=excluir&gicid="+$(this).attr('id'),
		   	dataType: 'json',
		   	success: function(msg){
			}
		 });
		listaGrupos();
		$('#gicid').val('');
		$('#gicnome').val('');
		$('#gicdescricao').val('');
		$('#limpar').hide();
   		alert('Operação realizada com sucesso!');
	});
	
	$('#limpar').click(function(){
		$('#gicid').val('');
		$('#gicnome').val('');
		$('#gicdescricao').val('');
		$('#limpar').hide();
	});
});


</script>
<form method="post" name="formGrupo" id="formGrupo">
	<input type="hidden" id="req" name="req" value="" />
	<input type="hidden" id="gicid" name="gicid" value="" />
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" valign="bottom" width="30%">Nome</td>
			<td>
				<?php 
				$gicnome = $_POST['gicnome']; 
				echo campo_texto( 'gicnome', 'S', 'S', '', 40, 200, '', '','','','','id="gicnome"' ); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="bottom">Descrição</td>
			<td>
				<?php 
				$gicdescricao = $_POST['gicdescricao']; 
				echo  campo_textarea( 'gicdescricao', 'S', 'S', $label, 100, 5, $max, $funcao = '', $acao = 0, $txtdica = '', $tab = false, $title = NULL, $gicdescricao); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tipo de Subação</td>
			<td>
			<?php 
				// Tipo de Subação
				$sql = "SELECT 
							pts.ptsid as codigo, 
							pts.ptsdescricao || ' - ' || fe.frmdsc as descricao
						FROM 
							par.propostatiposubacao pts
						INNER JOIN par.formaexecucao fe ON fe.frmid = pts.frmid
						where
							pts.ptsstatus = 'A'
						order by
							pts.ptsdescricao";
				$stSqlCarregados = "";
				combo_popup( "ptsid", $sql, "Tipo de Subação", "192x400", 0, array(), "", "S", false, false, 5, 400 );
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="bottom"></td>
			<td class="SubTituloDireita" align="left" style="text-align:left">
	         	<input type="button" class="salvar" value="Gravar" />	
	         	<input type="button" id="limpar" value="Limpar" style="display:none"/>
			</td>
		</tr>
	</table>
</form>
<table align="center" cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" style="border-top: none; border-bottom: none;" class="tabela">
	<tbody>
		<tr>
			<td align="center" width="100%"><label style="color:#000000;" class="TituloTela">Lista de Grupos</label></td>
		</tr>
	</tbody>
</table>
<div id="lista">
</div>