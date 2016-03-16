<?php
// Exclui objetivo estratégico
if ( ($_REQUEST['action'] == 'delete') && !empty($_REQUEST['id'])){
	$id = $_REQUEST['id'];
	//Verifica a existência de vinculos
    $sql = "SELECT SUM(total) as total
			FROM(
				SELECT count(0) as total FROM pto.objetivosolucao WHERE obeid = {$id}
				UNION ALL
				SELECT count(0) as total FROM pde.ae_objetivoestrategicoxmetappa WHERE obeid = {$id}
				UNION ALL
				SELECT count(0) as total FROM pde.ae_objetivoestrategicoxmetapne WHERE obeid = {$id}
				UNION ALL
				SELECT count(0) as total FROM pde.ae_objetivoestrategicoxiniciativa WHERE obeid = {$id}
				UNION ALL
				SELECT count(0) as total FROM pde.ae_objetivoestrategicoxacao WHERE obeid = {$id}
			) AS foo";
    $valor = $db->pegaUm($sql);
	if ($valor > 0){
		echo "<script>
		alert('O objetivo não pode ser excluído, pois existem metas pne vinculadas!');
		</script>";
	} else {
		$sql = "DELETE FROM pde.ae_objetivoestrategico WHERE obeid = {$id}";
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
$menu[0] = array("descricao" => "Pesquisar objetivo estratégico", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_estrategico/pesquisar&acao=A");
$menu[1] = array("descricao" => "Cadastrar objetivo estratégico", "link"=> "estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_estrategico/cadastrar&acao=A");
echo montarAbasArray($menu, "estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_estrategico/pesquisar&acao=A");

monta_titulo( $titulo_modulo , '');
?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario" action="estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_estrategico/pesquisar&acao=A">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td align='right' class="SubTituloDireita">Objetivo estratégico:</td>
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
	$where = " WHERE upper(obenome) like upper('%$_REQUEST[nome]%') ";
 }	
 else {
	$where = "";
 }
$sql = "SELECT '<center>
										<a style=\"cursor:pointer;\" onclick=\"editar(\''||obeid||'\');\">
												<img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
										</a>
										<a  style=\"cursor:pointer;\" onclick=\"deletar(\''||obeid||'\');\">
												<img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
										</a>
								</center>' as acao, 
						obenome
						FROM pde.ae_objetivoestrategico
						$where
						ORDER BY obenome";
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
        window.location.href = 'estrategico.php?modulo=sistema/alinhamento_estrategico/objetivo_estrategico/cadastrar&acao=A&id='+id;
    }
</script>
