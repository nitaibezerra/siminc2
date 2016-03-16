<?php
// Exclui meta PNE
if ( ($_REQUEST['action'] == 'delete') && !empty($_REQUEST['id'])){
	$id = $_REQUEST['id'];
	//Verifica a existência de vinculos
	$sql = "SELECT SUM(total) as total
			FROM(
				SELECT count(0) as total FROM pto.artigosolucao WHERE artid = {$id}
			) AS foo";
	$valor = $db->pegaum($sql);

	if ($valor > 0){
		echo "<script>
		alert('A meta não pode ser excluída, pois existem vínculos!');		  
		</script>";
	} else {
		$sql = "DELETE FROM pde.ae_artigo WHERE artid = {$id}";
		$db->executar($sql);
		$db->commit();
		echo "<script>
		alert('Artigo excluído com sucesso!');
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
$menu[0] = array("descricao" => "Pesquisar artigo", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/artigo/pesquisar&acao=A");
$menu[1] = array("descricao" => "Cadastrar artigo", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/artigo/cadastrar&acao=A");
echo montarAbasArray($menu, "estrategico.php?modulo=sistema/alinhamento_estrategico/artigo/pesquisar&acao=A");

monta_titulo( $titulo_modulo , '');
?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario" action="estrategico.php?modulo=sistema/alinhamento_estrategico/artigo/pesquisar&acao=A">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td align='right' class="SubTituloDireita">Artigo:</td>
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
	$where = " WHERE upper(artnome) like upper('%$_REQUEST[nome]%') ";
 }	
 else {
	$where = "";
 }
$sql = "SELECT
            '<center>
                <a style=\"cursor:pointer;\" onclick=\"editar(\''||artid||'\');\">
                    <img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
                </a>
                <a style=\"cursor:pointer;\" onclick=\"deletar(\''||artid||'\');\">
                    <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
                </a>
            </center>' as acao,
            art.artordem,
            art.artnome,
            to_char(art.artprazo,'DD/MM/YYYY') AS prazo,
            sit.sitdsc
        FROM pde.ae_artigo art
        INNER JOIN pde.ae_situacao sit ON sit.sitid = art.sitid
        $where
        ORDER BY art.artordem";
$cabecalho = array("Ação", "Ordem","Artigo","Prazo","Situação");
$db->monta_lista($sql,$cabecalho,100,5,'N','95%',$par2);
?>
<script type="text/javascript">
    function submeter(){
            document.formulario.submit();	
    }

    function deletar(id){
            var conf = confirm("Você realmente deseja excluir este artigo?");
            if(conf) {
                    window.location.href = window.location.href + "&action=delete&id="+id;	
            }
    }
    function editar(id){
        window.location.href = 'estrategico.php?modulo=sistema/alinhamento_estrategico/artigo/cadastrar&acao=A&id='+id;
    }
</script>
