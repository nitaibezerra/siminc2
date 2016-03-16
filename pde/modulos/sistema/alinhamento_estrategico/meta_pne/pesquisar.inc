<?php
// Exclui meta PNE
if ( ($_REQUEST['action'] == 'delete') && !empty($_REQUEST['id'])){
	$id = $_REQUEST['id'];
	//Verifica a existência de vinculos
	$sql = "SELECT SUM(total) as total
			FROM(
				SELECT count(0) as total FROM pde.ae_estrategia WHERE metid = {$id}
				UNION ALL
				SELECT count(0) as total FROM pde.ae_metapnexppa WHERE mpneid = {$id}
				UNION ALL
				SELECT count(0) as total FROM pde.ae_desafioxmetapne WHERE mpneid = {$id}
				UNION ALL
				SELECT count(0) as total FROM pde.ae_acaoxmetapne WHERE mpneid = {$id}
				UNION ALL
				SELECT count(0) as total FROM pde.ae_metapnexindicador WHERE mpneid = {$id}
				UNION ALL
				SELECT count(0) as total FROM pde.ae_objetivoestrategicoxmetapne WHERE mpneid = {$id}
				UNION ALL
				SELECT count(0) as total FROM pto.metasolucao WHERE mpneid = {$id}
			) AS foo";
	$valor = $db->pegaum($sql);

	if ($valor > 0){
		echo "<script>
		alert('A meta não pode ser excluída, pois existem vínculos!');		  
		</script>";
	} else {
		$sql = "DELETE FROM pde.ae_metapne WHERE mpneid = {$id}";
		$db->executar($sql);
		$db->commit();
		echo "<script>
		alert('Meta excluída com sucesso!');
		</script>";
	}
}

include  APPRAIZ."includes/cabecalho.inc";

echo "<br>";

if(isset($_SESSION['id'])){
    unset($_SESSION['id']);
}

//tratando abas que serao ativadas apartir do cadastro de acões
$menu = array();
$menu[0] = array("descricao" => "Pesquisar meta PNE", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/meta_pne/pesquisar&acao=A");
$menu[1] = array("descricao" => "Cadastrar meta PNE", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/meta_pne/cadastrar&acao=A");
echo montarAbasArray($menu, "estrategico.php?modulo=sistema/alinhamento_estrategico/meta_pne/pesquisar&acao=A");

monta_titulo( $titulo_modulo , '');
?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario" action="estrategico.php?modulo=sistema/alinhamento_estrategico/meta_pne/pesquisar&acao=A">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td align='right' class="SubTituloDireita">Nome:</td>
    <td><? 
    	echo campo_texto('nome','N','S','',100,255,'','', '' , '' , '' , '' , '' , $_REQUEST['nome']); 
    	?>
    </td>
</tr>
<tr bgcolor="#cccccc">
    <td></td>
        <td>
            <input type="button" class="botao" name="bta" value="Pesquisar" onclick="submeter();">
        </td>
    </tr>
</table>
</form>
<?
 if(!empty($_REQUEST['nome'])){
	$where = " AND upper(mpnenome) like upper('%$_REQUEST[nome]%') ";
 }	
 else {
	$where = "";
 }
$sql = "SELECT '<center>
					<a style=\"cursor:pointer;\" onclick=\"editar(\''||mpneid||'\');\">
						<img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
					</a>
					<a  style=\"cursor:pointer;\" onclick=\"deletar(\''||mpneid||'\');\">
						<img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
					</a>
				</center>' as acao,
		    mpneordem,
			mpnenome,
			to_char(mpneprazo,'DD/MM/YYYY') AS prazo
			FROM pde.ae_metapne
			WHERE mpneid <> 99999999
			$where
			ORDER BY mpneordem";
$cabecalho = array("Ação", "Ordem","Título","Prazo");
$db->monta_lista($sql,$cabecalho,100,5,'N','95%',$par2);
?>
<script type="text/javascript">
    function submeter(){
            document.formulario.submit();	
    }

    function deletar(id){
            var conf = confirm("Você realmente deseja excluir esta meta?");	
            if(conf) {
                    window.location.href = window.location.href + "&action=delete&id="+id;	
            }
    }
    function editar(id){
        window.location.href = 'estrategico.php?modulo=sistema/alinhamento_estrategico/meta_pne/cadastrar&acao=A&id='+id;
    }
</script>
