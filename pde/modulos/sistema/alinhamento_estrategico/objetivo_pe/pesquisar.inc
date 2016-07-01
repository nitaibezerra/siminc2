<?php
// Exclui objetivo PE
if ( ($_REQUEST['action'] == 'delete') && !empty($_REQUEST['id'])){
	$id = $_REQUEST['id'];
	//Verifica a existência de vinculos
	$sql = "SELECT COUNT(0) as total FROM pde.ae_desafio WHERE opeid = {$id}";
	$valor = $db->pegaUm($sql);

	if ($valor > 0){
		echo "<script>
		alert('O objetivo não pode ser excluído, pois existem desafios vinculados!');		  
		</script>";
	} else {
		$sql = "DELETE FROM pde.ae_objetivope WHERE opeid = {$id}";
		$db->executar($sql);
		$db->commit();
		echo "<script>
		alert('Objetivo excluído com sucesso!');
		</script>";
	}
}

include  APPRAIZ."includes/cabecalho.inc";

echo "<br>";

if(isset($_SESSION['id'])){
    unset($_SESSION['id']);
}

//tratando abas que serao ativadas a partir do cadastro objetivo PE
$menu = array();
$menu[0] = array("descricao" => "Pesquisar objetivo PE", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_pe/pesquisar&acao=A");
$menu[1] = array("descricao" => "Cadastrar objetivo PE", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_pe/cadastrar&acao=A");
echo montarAbasArray($menu, "estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_pe/pesquisar&acao=A");

monta_titulo( $titulo_modulo , '');
?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario" action="estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_pe/pesquisar&acao=A">
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
	$where = " WHERE upper(openome) like upper('%$_REQUEST[nome]%') ";
 }	
 else {
	$where = "";
 }
$sql = "SELECT '<center>
										<a style=\"cursor:pointer;\" onclick=\"editar(\''||opeid||'\');\">
												<img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
										</a>
										<a  style=\"cursor:pointer;\" onclick=\"deletar(\''||opeid||'\');\">
												<img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
										</a>
								</center>' as acao, 
						openome
						FROM pde.ae_objetivope
						$where
						ORDER BY openome";
$cabecalho = array("Ação","Título");
$db->monta_lista($sql,$cabecalho,100,5,'N','95%',$par2);
?>
<script type="text/javascript">
    function submeter(){
            document.formulario.submit();	
    }

    function deletar(id){
            var conf = confirm("Você realmente deseja excluir este objetivo?");	
            if(conf) {
                    window.location.href = window.location.href + "&action=delete&id="+id;	
            }
    }
    function editar(id){
        window.location.href = 'estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_pe/cadastrar&acao=A&id='+id;
    }
</script>
