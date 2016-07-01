<?php
include APPRAIZ."includes/cabecalho.inc";
echo "<br/>";

$menu = array(0 => array("id" => 1, "descricao" => "Metas", "link" => "/monitora/monitora.php?modulo=principal/detalhesppa&acao=O&aba=metas&prgcod={$_REQUEST['prgcod']}&objid={$_REQUEST['objid']}"),
					  1 => array("id" => 2, "descricao" => "Indicadores da Meta",  "link" => "/monitora/monitora.php?modulo=principal/indicadorMeta&acao=A&prgcod={$_REQUEST['prgcod']}&objid={$_REQUEST['objid']}&metid=".$_REQUEST['metid']),
				  	  );
echo montarAbasArray($menu, "/monitora/monitora.php?modulo=principal/indicadorMeta&acao=A&prgcod={$_REQUEST['prgcod']}&objid={$_REQUEST['objid']}&metid=".$_REQUEST['metid']);
monta_titulo( "Indicadores da Meta", "&nbsp;");

$sql = "select		distinct
					'<img src=\"../imagens/consultar.gif\" title=\"Visualizar Indicador\" style=\"cursor:pointer;\" onclick=\"visualizarIndicador('||i.indid||');\">' as acao,
					i.indnome AS descricao
				from
					monitora.metaindicador m
				inner join
					painel.indicador i ON i.indid = m.indid
				inner join
  					painel.agendaindicador a ON a.indid = i.indid
				where
					--aggid = 8
				--and
					metid = {$_GET['metid']}
				and
					i.indstatus = 'A'";
$arrCab = array("Ação","Indicador");
$db->monta_lista($sql,$arrCab,100,10,"N","center","N");
?>
<script>
	function visualizarIndicador(indid)
	{
		var j = window.open('../painel/painel.php?modulo=principal/pc_indicador&acao=A&indid='+indid,'Indicador','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
		j.focus();
	}
</script>