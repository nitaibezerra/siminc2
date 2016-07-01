<?
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Projovem Urbano', 'Lista de estados. Selecione os Estados abaixo.');

if( $_REQUEST['requisicao'] ){
	$_REQUEST['requisicao']();
	die();
}

//unset($_SESSION['projovemurbano']);
unset($_SESSION['projovemurbano']['muncod']);
unset($_SESSION['projovemurbano']['pjuid']);

include_once APPRAIZ.'projovemurbano/modulos/principal/filtroAnoExercicio.inc';

$perfis = pegaPerfilGeral();
if( $_SESSION['projovemurbano']['ppuano'] && $_SESSION['projovemurbano']['ppuid'] ){
	
	$acao = "'<a href=\"#\" onclick=\"window.location=\'projovemurbano.php?modulo=principal/instrucao&acao=A&estuf='||e.estuf||'\';\" title=\"Ir para o termo de compromisso\"><img src=\"../imagens/alterar.gif\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'";
	
	if(in_array(612,$perfis) || in_array(683, $perfis) ||
		in_array(616, $perfis) ){
		$cancelar = "" . ((! $habilitado) ? "'<img src=../imagens/excluir.gif border=0 width=13px
	                      title=\"Cancelar Adesão\" style=cursor:pointer;
	                      id='||p.pjuid ||' class=cancelar est = \"'||e.estdescricao||'\" />'" : "&nbsp;") . "";
		
		$reativar = "" . ((! $habilitado) ? "'<img src=../imagens/alterar.gif border=0 width=13px
	                      title=\"Reativar Adesão\" style=cursor:pointer;
	                      id='||p.pjuid ||' class=reativar est = \"'||e.estdescricao||'\" />'" : "&nbsp;") . "";
	}
	if(!$db->testa_superuser()) {
		
		if(in_array(645, $perfis) || ( in_array(647, $perfis) /*&& $_SESSION['projovemurbano']['ppuid'] == '2'*/ ) ) {
			$inner_estado = "inner join projovemurbano.usuarioresponsabilidade ur on ur.usucpf='".$_SESSION['usucpf']."' and ur.estuf=e.estuf AND rpustatus = 'A'";
			$where = "AND pjustatus = 'A'";
			$acao = "'<a href=\"#\" onclick=\"window.location=\'projovemurbano.php?modulo=principal/instrucao&acao=A&estuf='||e.estuf||'\';\" title=\"Ir para o termo de compromisso\"><img src=\"../imagens/alterar.gif\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'";
			$cancelar = "''";
			$reativar = "''";
		}elseif(in_array(647, $perfis)) {
			$inner_estado = "inner join projovemurbano.projovemurbano pr on pr.estuf=e.estuf 
						     inner join projovemurbano.coordenadorresponsavel cr on pr.pjuid=cr.pjuid and cr.corcpf='".$_SESSION['usucpf']."'";
			$where = "AND p.pjustatus = 'A'";
			$acao = "'<a href=\"#\" onclick=\"window.location=\'projovemurbano.php?modulo=principal/instrucao&acao=A&estuf='||e.estuf||'\';\" title=\"Ir para o termo de compromisso\"><img src=\"../imagens/alterar.gif\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'";
			$cancelar = "''";
			$reativar = "''";
		}
	}
		if(!in_array(694,$perfis) ){
			$case = "CASE
						WHEN p.pjustatus = 'A'
						THEN 
							CASE 
								WHEN esd.esddsc IS NULL THEN 
									CASE 
										WHEN p.adesaotermo=TRUE THEN  $acao||'&nbsp;&nbsp;'||$cancelar
										ELSE 
											CASE 
												WHEN ppu.ppuano::integer < ".date('Y')." THEN '<a title=\"Não possui adesão\"><img src=\"../imagens/alterar_pb.gif\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'
												ELSE $acao||'&nbsp;&nbsp;'||$cancelar
											END 
									END 
								ELSE $acao||'&nbsp;&nbsp;'||$cancelar
							END 
						ELSE $reativar 
					END as acao";
		}else{
			$case = "$acao as acao";
		}
	
	if( $_SESSION['projovemurbano']['ppuid'] == '2' ){
		$vlr = " AND cmemeta::integer > 0 ";
	}
	
	$sql = "select distinct
				$case,
				CASE
					WHEN p.pjustatus = 'A'
					THEN e.estuf
					ELSE '<label style=\"color: red\">'||e.estuf||'</label>'
				END as estuf,
				CASE
					WHEN p.pjustatus = 'A'
					THEN e.estdescricao
					ELSE '<label style=\"color: red\">'||e.estdescricao||'</label>'
				END as estdescricao,
				CASE
				WHEN p.pjustatus = 'I'
					THEN '<label style=\"color: red\">'||'Cancelado'||'</label>'
					ELSE
			   			CASE 
							WHEN esd.esddsc IS NULL 
							THEN 
								CASE 
									WHEN p.adesaotermo=TRUE THEN 'Em elaboração' 
									ELSE 'Sem adesão' 
								END 
							ELSE esd.esddsc 
						END 
				END as situacao,
				CASE WHEN ht.htddata IS NULL THEN 'Não enviado' ELSE to_char(ht.htddata,'dd/mm/YYYY HH24:MI') END as data
			from territorios.estado e 
			{$inner_estado}
			inner join projovemurbano.cargameta c on c.cmecodibge::character(2) = e.estcod and cmetipo='E' $vlr
			left join projovemurbano.projovemurbano p on p.estuf = e.estuf and p.ppuid = {$_SESSION['projovemurbano']['ppuid']} 
			left join projovemurbano.programaprojovemurbano ppu ON ppu.ppuid = p.ppuid
			left join workflow.documento dd on dd.docid = p.docid 
			left join workflow.historicodocumento ht on ht.hstid = dd.hstid  
			left join workflow.estadodocumento esd on esd.esdid = dd.esdid 
			where c.ppuid = {$_SESSION['projovemurbano']['ppuid']}  $where
			order by estuf, estdescricao";
// ver($sql,d);
	$db->monta_lista( $sql, array( "Ação", "UF", "Descrição", "Situação","Data Tramitação"), 30, 10, 'N', 'center', '','', $tamanho,$alinha);
}

?>
<form id="form" enctype="multipart/form-data" name="form" method="POST" >
<input type="hidden" value="" name="requisicao" id="requisicao" />
<input type="hidden" value="" name="pjuid" id="pjuid" />
<input type="hidden" value="" name="esddsc" id="esddsc" />
</form>
<script language="javascript" type="text/javascript">
jQuery(document).ready(function() {
	 jQuery('.cancelar').live('click',function(){
	        var pjuid = jQuery(this).attr('id');
	        var esddsc = jQuery(this).attr('est');
	        if ( confirm( "Deseja cancelar a adesão ao Projovem Urbano do estado "+ esddsc +"?" ) ) {
		        jQuery('#requisicao').val('cancelarAdesao');
		        jQuery('#pjuid').val(pjuid);
		        jQuery('#form').submit();
	     	}
	    });
	 jQuery('.reativar').live('click',function(){
	     var pjuid = jQuery(this).attr('id');
	     var esddsc = jQuery(this).attr('est');
	     if ( confirm( "Deseja reativar a adesão ao Projovem Urbano do estado "+ esddsc +"?" ) ) {
		     jQuery('#requisicao').val('reativarAdesao');
		     jQuery('#pjuid').val(pjuid);
		     jQuery('#form').submit();
	     }
	 });
});
</script>