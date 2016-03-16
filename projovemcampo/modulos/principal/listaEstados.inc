<?
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Projovem Campo', 'Lista de estados. Selecione os Estados abaixo.');

unset($_SESSION['projovemcampo']['muncod']);

if( $_REQUEST['requisicao'] ){
	$_REQUEST['requisicao']();
	die();
}

$acao = "'<a href=\"#\" onclick=\"window.location=\'projovemcampo.php?modulo=principal/instrucao&acao=A&estuf='||e.estuf||'\';\" title=\"Ir para o termo de compromisso\"><img src=\"../imagens/alterar.gif\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'";
$cancelar = "" . ((! $habilitado) ? "'<img src=../imagens/excluir.gif border=0 width=13px
                      title=\"Cancelar Adesão\" style=cursor:pointer; 
                      id='||c.apcid ||' class=cancelar est = \"'||e.estdescricao||'\" />'" : "&nbsp;") . "";
$reativar = "" . ((! $habilitado) ? "'<img src=../imagens/alterar.gif border=0 width=13px
                      title=\"Reativar Adesão\" style=cursor:pointer;
                      id='||c.apcid ||' class=reativar est = \"'||e.estdescricao||'\" />'" : "&nbsp;") . "";
if(!$db->testa_superuser()) {

	$perfis = pegaPerfilGeral();
	
	if(in_array(PFL_SECRETARIO_ESTADUAL, $perfis)) {
		$where = "WHERE c.apcstatus = 't'";
		$inner_estado = "inner join projovemcampo.usuarioresponsabilidade ur on ur.usucpf='".$_SESSION['usucpf']."' and ur.estuf=e.estuf AND rpustatus = 'A'";
		$acao = "'<a href=\"#\" onclick=\"window.location=\'projovemcampo.php?modulo=principal/instrucao&acao=A&estuf='||e.estuf||'\';\" title=\"Ir para o termo de compromisso\"><img src=\"../imagens/alterar.gif\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>' ";
		$cancelar = "''";
		$reativar = "''";
	}
	elseif(in_array(PFL_COORDENADOR_ESTADUAL, $perfis)) {
		$where = "WHERE c.apcstatus = 't'";
		$inner_estado = "inner join projovemcampo.secretaria sec on sec.secaid = c.secaid and sec.secoordcpf='".$_SESSION['usucpf']."'";
		$acao = "'<a href=\"#\" onclick=\"window.location=\'projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A&estuf='||e.estuf||'\';\" title=\"Ir para o termo de compromisso\"><img src=\"../imagens/alterar.gif\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>' ";
		$cancelar = "''";
		$reativar = "''";
	}elseif(in_array(PFL_CONSULTA,$perfis)){
		$acao = "'<a href=\"#\" onclick=\"window.location=\'projovemcampo.php?modulo=principal/instrucao&acao=A&estuf='||e.estuf||'\';\" title=\"Ir para o termo de compromisso\"><img src=\"../imagens/alterar.gif\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'";
		$cancelar = "''";
		$reativar = "''";
	}
	
}

#VALIDAÇÃO NÃO ENTENDIDA.
/*
 * 
 * CASE 
				WHEN esd.esddsc IS NULL THEN 
					CASE 
						WHEN c.apctermoaceito=TRUE THEN '<a href=\"#\" onclick=\"window.location=\'projovemcampo.php?modulo=principal/instrucao&acao=A&estuf='||e.estuf||'\';\" title=\"Ir para o termo de compromisso\"><img src=\"../imagens/alterar.gif\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>' 
						ELSE 
							CASE 
								WHEN apctermoaceito = 'f' THEN '<a title=\"Não possui adesão\"><img src=\"../imagens/alterar_pb.gif\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'
								ELSE '<a href=\"#\" onclick=\"window.location=\'projovemcampo.php?modulo=principal/instrucao&acao=A&estuf='||e.estuf||'\';\" title=\"Ir para o termo de compromisso\"><img src=\"../imagens/alterar.gif\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'
							END 
					END 
				ELSE '<a href=\"#\" onclick=\"window.location=\'projovemcampo.php?modulo=principal/instrucao&acao=A&estuf='||e.estuf||'\';\" title=\"Ir para o termo de compromisso\"><img src=\"../imagens/alterar.gif\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'
			END as acao,
 */ 
 
$sql = "select distinct
			CASE
				WHEN c.apcstatus = 'f'
					THEN $reativar
					ELSE $acao||'&nbsp;&nbsp;'||$cancelar
			END as acao,
			CASE
				WHEN c.apcstatus = 'f'
					THEN '<label style=\"color: red\">'||e.estuf||'</label>'
					ELSE e.estuf
			END as estuf,
			CASE
				WHEN c.apcstatus = 'f'
					THEN '<label style=\"color: red\">'||e.estdescricao||'</label>'
					ELSE e.estdescricao
			END as estdescricao,
			CASE
				WHEN c.apcstatus = 'f'
					THEN '<label style=\"color: red\">'||'Cancelado'||'</label>'
					ELSE
			   			CASE 
							WHEN esd.esddsc IS NULL THEN 
								CASE 
									WHEN c.apctermoaceito=TRUE THEN 'Em elaboração' 
									ELSE 'Sem adesão' 
								END 
							ELSE esd.esddsc
						END 
			END as situacao,
			CASE 
				WHEN ht.htddata IS NULL 
					THEN 'Não enviado' 
					ELSE to_char(ht.htddata,'dd/mm/YYYY HH24:MI') 
			END as data
			
		from territorios.estado e 
		inner join projovemcampo.adesaoprojovemcampo c on c.apccodibge::character(2) = e.estcod and apcesfera='E' 
		{$inner_estado}
		left join workflow.documento dd on dd.docid = c.docid 
		left join workflow.historicodocumento ht on ht.hstid = dd.hstid  
		left join workflow.estadodocumento esd on esd.esdid = dd.esdid 
		$where
		order by estuf, estdescricao";
// 		ver($sql,d);
$alinhamento	= array( 'center','center','center','center','center' );
$db->monta_lista( $sql, array( "Ação", "UF",  "Descrição", "Situação","Data Tramitação" ), 30, 10, 'N', 'center', '','', $tamanho,$alinhamento);

?>
<form id="form" enctype="multipart/form-data" name="form" method="POST" >
<input type="hidden" value="" name="requisicao" id="requisicao" />
<input type="hidden" value="" name="apcid" id="apcid" />
<input type="hidden" value="" name="estdescricao" id="estdescricao" />
</form>
<script language="javascript" type="text/javascript">
jQuery(document).ready(function() {
	 jQuery('.cancelar').live('click',function(){
	        var apcid = jQuery(this).attr('id');
	        var estdescricao = jQuery(this).attr('est');
	        if ( confirm( "Deseja cancelar a adesão ao Projovem Campo do estado "+ estdescricao +"?" ) ) {
		        jQuery('#requisicao').val('cancelarAdesao');
		        jQuery('#apcid').val(apcid);
		        jQuery('#form').submit();
	     	}
	    });
	 jQuery('.reativar').live('click',function(){
	     var apcid = jQuery(this).attr('id');
	     var estdescricao = jQuery(this).attr('est');
	     if ( confirm( "Deseja reativar a adesão ao Projovem Campo do estado "+ estdescricao +"?" ) ) {
		     jQuery('#requisicao').val('reativarAdesao');
		     jQuery('#apcid').val(apcid);
		     jQuery('#form').submit();
	     }
	 });
});
</script>