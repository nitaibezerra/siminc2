<?php 
function solicitarDiaria($dados){
	
	if($dados['empcod'] && $dados['ungcod']){
		$_SESSION['evento']['ungcod'] = $dados['ungcod'];
		$_SESSION['evento']['empcod'] = $dados['empcod'];
		echo "<script>
					window.location = 'evento.php?modulo=principal/solicitacaoDiarias&acao=A';
			  </script>";
	}else{
		$_SESSION['evento']['undid'] = $dados['undid'];
		echo "<script>
					window.location = 'evento.php?modulo=principal/solicitacaoDiarias&acao=A';
			  </script>";
	}
}

function montacomboEmpenho($dados){
	
	global $db;
	
	if(strlen($dados['ungcod'])==5){
		$campoWh = 'unicod';
	}else{
		$campoWh = 'ungcod';
	}
	
	$sql = "SELECT DISTINCT
				empcod as codigo,
				empcod||' - '||coalesce(empdsc,'Empenho') as descricao
			FROM
				evento.empenho_unidadegestora
			WHERE
				$campoWh = '".$dados['ungcod']."'";
	
	echo $db->monta_combo('empcod',$sql,'S','Selecione...','','','','','S', 'empcod');
	
}

if($_REQUEST['req']){
	$_REQUEST['req']($_REQUEST);
	die();
}

$notUngcod = Array();
$notUnicod = Array();
if(!$db->testa_superuser()){
	
	$perfils = Array(EVENTO_PERFIL_SOLICITADOR, EVENTO_PERFIL_ADMINISTRADOR_DIARIAS);
	
	if( possuiPerfil( $perfils ) ){
		$sql = "SELECT DISTINCT 
					p.ungcod 
				FROM 
					evento.usuarioresponsabilidade ur
				INNER JOIN public.unidadegestora p on
					ur.ungcod = p.ungcod
				WHERE
					ur.rpustatus = 'A' and
					ur.usucpf = '".$_SESSION['usucpf']."' and
					ur.pflcod = ".EVENTO_PERFIL_SOLICITADOR." and
					ur.prsano = '".$_SESSION['exercicio']."'";
		$notUngcod = $db->carregarColuna($sql);
		$sql = "SELECT DISTINCT 
					p.unicod as codigo, 
					p.unicod || ' - ' || p.unidsc as descricao
				FROM 
					evento.usuarioresponsabilidade ur
				INNER JOIN public.unidade p ON ur.unicod = p.unicod
				WHERE
					ur.rpustatus = 'A' and
					ur.usucpf = '".$_SESSION['usucpf']."' and
					ur.pflcod in ( ".implode(',',$perfils)." ) and
					ur.prsano = '".$_SESSION['exercicio']."'";
		$notUnicod = $db->carregarColuna($sql);
	}else{
		echo "<script>
				alert('Acesso negado.');
				window.history.back(-1); 
			  </script>";
	}
}


include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

$abas = Array(Array('descricao'=>'Lista de Solicitação de Diarias',
			  		'link'     =>'evento.php?modulo=principal/listaSolicitacaoDiarias&acao=A'),
			  Array('descricao'=>'Solicitações de Diárias',
		  			'link'     =>'evento.php?modulo=principal/empenhoSolicitacaoDiarias&acao=A')
				);
if(possuiPerfil( Array(EVENTO_PERFIL_AGENTE_FINANCEIRO,EVENTO_PERFIL_ADMINISTRADOR_DIARIAS) )){
	array_push($abas, Array('descricao'=>'Solicitações de Diárias - Tramitação em Lotes',
		  				   'link'     =>'evento.php?modulo=principal/tramitaSolicitacaoDiarias&acao=A')
			 );
}
$url = 'evento.php?modulo=principal/empenhoSolicitacaoDiarias&acao=A';

echo montarAbasArray($abas, $url);

$subtitulo = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Campo obrigatório.';
monta_titulo( 'Solicitações de Diárias', $subtitulo );

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


	jQuery('#voltar').click(function(){

		window.history.back(-1);
	});

	jQuery('#inserir').click(function(){

		var pendencia;
		
		jQuery('.obrigatorio').each(function(){
			if( jQuery(this).val() == '' ){
				alert('Campo Obrigatório.');
				jQuery(this).focus();
				pendencia = true;
				return false;
			}
		});

		if(!pendencia){
			jQuery('#req').val('solicitarDiaria');
			jQuery('#formListaSolDia').submit();
		}
	});

	jQuery('#ungcod').change(function(){

		var ungcod = jQuery(this).val();

		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: "req=montacomboEmpenho&ungcod="+ungcod,
			async: false,
			success: function(msg){
				jQuery('#tdEmpenho').html(msg);
			}
		});
	});

});

</script>
<form method="post" name="formListaSolDia" id="formListaSolDia" action="">
	<input type="hidden" name="req" id="req" value=""/>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita">Unidade</td>
			<td>
			<?php 
				// Unidades EBSERH
				$sql = "select und.undid as codigo, 
					case when und.muncod is null then
						und.undsigla ||' - '|| btrim(und.unddsc)
					else
						und.undsigla ||' - '|| btrim(und.unddsc) ||' - '|| btrim(mun.mundescricao) ||'/'|| btrim(mun.estuf)
					end as descricao    	
				from ouvidoria.unidadedemanda as und
				left join territoriosgeo.municipio mun ON (mun.muncod = und.muncod)
				where und.undstatus = 'A' 
					order by und.unddsc;";
				$db->monta_combo('undid',$sql,'S','Selecione...','','','','','S', 'undid');
			?>
			</td>
		</tr>
		<!-- 
		<tr>
			<td class="SubTituloDireita">Empenho</td>
			<td id="tdEmpenho">
			</td>
		</tr>
		 -->
		<tr>
			<td class="SubTituloCentro" colspan="2" >
				<div style="float:left">
					<input type="button" id="voltar" value="Voltar" />	
				</div>
				<input type="button" id="inserir" value="Inserir Nova Solicitação" />	
			</td>
		</tr>
	</table>
</form>